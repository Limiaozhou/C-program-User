#include "app_uart.h"
#include "check.h"
#include "string.h"
#include "mb_rtu_master.h"
#include "data_typedef.h"

#define UART_NET Uart1
#define UART_SENSOR Uart3
#define UART_DEBUG Uart4

static uint8_t net_addr[8] = { 0x4F, 0x45, 0x41, 0x03, 0x00, 0x32, 0x00, 0x00 };  //硬件编号地址
static Sensor_Data sensor;  //传感器联合数据
static Sensor_485_Type sensor_type = Sensor485_1;  //传感器类型
static uint8_t resend_num = 0;  //重发次数

static void sensor_485_type_loop(void);  //读取485传感器类型循环
static void sensor_485_deal(uint8_t * pdata, uint32_t len);  //处理485传感器数据

//发送数据到网络
void network_data_write(void)
{
    Send_NetData send_netdata;
    
	send_netdata.head = 0xAA55;
    send_netdata.len = sizeof(send_netdata) - 3;
	send_netdata.ver = 38;
    
	memcpy(send_netdata.addr, net_addr, 8);
    
	send_netdata.data.cmd = 0xD0;
	send_netdata.data.len = sizeof(send_netdata.data) - 2;
    
    send_netdata.data.temp.key[0] = 0x00;
	send_netdata.data.temp.key[1] = 0x80;
	memcpy(send_netdata.data.temp.dat, &sensor.temp, 4);
    
    send_netdata.data.humi.key[0] = 0x00;
	send_netdata.data.humi.key[1] = 0x82;
	memcpy(send_netdata.data.humi.dat, &sensor.humi, 4);
    
    send_netdata.data.pm25.key[0] = 0x00;
	send_netdata.data.pm25.key[1] = 0x68;
	memcpy(send_netdata.data.pm25.dat, &sensor.pm25, 4);
    
    send_netdata.data.pm10.key[0] = 0x00;
	send_netdata.data.pm10.key[1] = 0x6A;
	memcpy(send_netdata.data.pm10.dat, &sensor.pm10, 4);
    
    send_netdata.data.noise.key[0] = 0x00;
	send_netdata.data.noise.key[1] = 0x08;
	memcpy(send_netdata.data.noise.dat, &sensor.noise, 4);
    
    send_netdata.data.fs.key[0] = 0x00;
	send_netdata.data.fs.key[1] = 0x02;
	memcpy(send_netdata.data.fs.dat, &sensor.fs, 4);
    
    send_netdata.data.fx.key[0] = 0x00;
	send_netdata.data.fx.key[1] = 0x04;
	memcpy(send_netdata.data.fx.dat, &sensor.fx, 4);
    
    send_netdata.data.pressure.key[0] = 0x00;
	send_netdata.data.pressure.key[1] = 0x84;
	memcpy(send_netdata.data.pressure.dat, &sensor.pressure, 4);
    
    send_netdata.fill[0] = 0;
    send_netdata.fill[1] = 0;
    
	send_netdata.crc16 = check_crc16_modbus_calc(&send_netdata.ver, send_netdata.len - 2);  //从ver开始crc校验
    
    uart_write( UART_NET, (uint8_t *)&send_netdata, sizeof(send_netdata) );
    uart_write( UART_DEBUG, (uint8_t *)&send_netdata, sizeof(send_netdata) );
    uart_write( UART_DEBUG, (uint8_t *)&sensor, sizeof(sensor) );
}

//发送485传感器读取指令
void sensor_485_write(void)
{
    uint8_t send_sensor[8] = {0};
    
    switch(sensor_type)
    {
        case Sensor485_1 :
            mb_rtu_read_holdreg(send_sensor, 0x01, 0x03, 0x00, 0x0D);
        break;
        
        case Sensor485_2 :
            mb_rtu_read_holdreg(send_sensor, 0x02, 0x03, 0x00, 0x01);
        break;
        
        case Sensor485_3 :
            mb_rtu_read_holdreg(send_sensor, 0x03, 0x03, 0x00, 0x01);
        break;
    }
    
    if(++resend_num >= 3)
        sensor_485_type_loop();
    
    uart_write( UART_SENSOR, send_sensor, sizeof(send_sensor) );
    uart_write( UART_DEBUG, send_sensor, sizeof(send_sensor) );
}

//接收处理485传感器数据
void sensor_485_read(void)
{
    uart_read(UART_SENSOR, sensor_485_deal);
}

//debug串口发送数据
void debug_uart_nodma_send(void)
{
    uart_nodma_send_loop(UART_DEBUG);
}

//读取485传感器类型循环
static void sensor_485_type_loop(void)
{
    if(++sensor_type >= num_of_sensor_485)
        sensor_type = Sensor485_1;
    
    resend_num = 0;
}

//处理485传感器数据
static void sensor_485_deal(uint8_t * pdata, uint32_t len)
{
//    Float_Byte data_float;
    uint32_t check_len = 0;  //要校验的帧数据长度
    int16_t data_int16 = 0;
    uint32_t data_uint32 = 0;
    
    uart_write( UART_DEBUG, pdata, len );
    
    while(len)
    {
        switch(*pdata)  //头校验
        {
            case 0x01 :
                check_len = *(pdata + 2) + 5;
                if( (len >= 7) && (len >= check_len) && (!check_crc16_modbus_calc(pdata, check_len)) )  //crc16校验为0表示通过
                {
                    data_uint32 = *(pdata + 4) + ((uint32_t)*(pdata + 3) << 8);
                    if(data_uint32 <= 1000)
                        sensor.humi = data_uint32 / 10.0;
                    
                    data_int16 = *(pdata + 6) + ((int16_t)*(pdata + 5) << 8);
                    if((data_int16 >= -400) && (data_int16 <= 800))
                        sensor.temp = data_int16 / 10.0;
                    
                    data_uint32 = *(pdata + 12) + ((uint32_t)*(pdata + 11) << 8);
                    if(data_uint32 <= 999)
                        sensor.pm25 = data_uint32;
                    
                    data_uint32 = *(pdata + 22) + ((uint32_t)*(pdata + 21) << 8);
                    if(data_uint32 <= 999)
                        sensor.pm10 = data_uint32;
                    
                    data_uint32 = *(pdata + 26);
                    data_uint32 += (uint32_t)*(pdata + 25) << 8;
                    data_uint32 += (uint32_t)*(pdata + 24) << 16;
                    data_uint32 += (uint32_t)*(pdata + 23) << 24;
                    if((data_uint32 >= 1000) && (data_uint32 <= 120000))
                        sensor.pressure = data_uint32 / 1000.0;
                    
                    data_uint32 = *(pdata + 28) + ((uint32_t)*(pdata + 27) << 8);
                    if((data_uint32 >= 300) && (data_uint32 <= 1300))
                        sensor.noise = data_uint32 / 10.0;
                    
                    sensor_485_type_loop();
                    
                    pdata += check_len;
                    len -= check_len;
                }
                else
                {
                    ++pdata;
                    --len;
                }
            break;
            
            case 0x02 :
                check_len = *(pdata + 2) + 5;
                if( (len >= 7) && (len >= check_len) && (!check_crc16_modbus_calc(pdata, check_len)) )
                {
                    data_uint32 = *(pdata + 4) + ((uint32_t)*(pdata + 3) << 8);
                    if(data_uint32 <= 360)
                        sensor.fx = data_uint32;
                    
                    sensor_485_type_loop();
                    
                    pdata += check_len;
                    len -= check_len;
                }
                else
                {
                    ++pdata;
                    --len;
                }
            break;
            
            case 0x03 :
                check_len = *(pdata + 2) + 5;
                if( (len >= 7) && (len >= check_len) && (!check_crc16_modbus_calc(pdata, check_len)) )
                {
                    data_uint32 = *(pdata + 4) + ((uint32_t)*(pdata + 3) << 8);
                    if(data_uint32 <= 300)
                        sensor.fs = data_uint32 / 10.0;
                    
                    sensor_485_type_loop();
                    
                    pdata += check_len;
                    len -= check_len;
                }
                else
                {
                    ++pdata;
                    --len;
                }
            break;
            
            default :
                ++pdata;
                --len;
            break;
        }
    }
}
