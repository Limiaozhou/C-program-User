#include "app_uart.h"
#include "check.h"
#include "string.h"

#define UART_NET Uart1
#define UART_SENSOR Uart3
#define UART_DEBUG Uart5

static uint8_t net_addr[8] = { 0x4F, 0x45, 0x41, 0x03, 0x00, 0x32, 0x00, 0x00 };  //硬件编号地址
static Sensor_Data sensor;  //传感器联合数据
static Sensor_485_Type sensor_type = Sensor485_1;  //传感器类型
static uint8_t resend_num = 0;  //重发次数

static void sensor_485_type_loop(void);  //读取485传感器类型循环

//modbus rtu 协议读取保持寄存器，reg_adr和reg_len高位在前
static void mb_rtu_read_holdreg(uint8_t * pbuf, uint8_t dev_adr, uint8_t func, uint16_t reg_adr, uint16_t reg_num);

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
    
//    send_netdata.data.light.key[0] = 0x00;
//	send_netdata.data.light.key[1] = 0x06;
//	memcpy(send_netdata.data.light.dat, &sensor.light, 4);
    
//    send_netdata.data.fs.key[0] = 0x00;
//	send_netdata.data.fs.key[1] = 0x02;
//	memcpy(send_netdata.data.fs.dat, &sensor.fs, 4);
    
//    send_netdata.data.fx.key[0] = 0x00;
//	send_netdata.data.fx.key[1] = 0x02;
//	memcpy(send_netdata.data.fx.dat, &sensor.fx, 4);
    
//	send_netdata.data.turbidity.key[0] = 0x00;
//	send_netdata.data.turbidity.key[1] = 0xAA;
//	memcpy(send_netdata.data.turbidity.dat, &sensor.turbidity, 4);
    
//	send_netdata.data.ca.key[0] = 0x00;
//	send_netdata.data.ca.key[1] = 0x1A;
//	memcpy(send_netdata.data.ca.dat, &sensor.ca, 4);
    
    send_netdata.data.ph.key[0] = 0x00;
	send_netdata.data.ph.key[1] = 0xA6;
	memcpy(send_netdata.data.ph.dat, &sensor.ph, 4);
    
//    send_netdata.data.Do.key[0] = 0x00;
//	send_netdata.data.Do.key[1] = 0xA4;
//	memcpy(send_netdata.data.Do.dat, &sensor.Do, 4);
    
    send_netdata.data.nh4.key[0] = 0x00;
	send_netdata.data.nh4.key[1] = 0xA2;
	memcpy(send_netdata.data.nh4.dat, &sensor.nh4, 4);
    
    send_netdata.data.phosphorus.key[0] = 0x00;
	send_netdata.data.phosphorus.key[1] = 0x1E;
	memcpy(send_netdata.data.phosphorus.dat, &sensor.phosphorus, 4);
    
    send_netdata.data.potassium.key[0] = 0x00;
	send_netdata.data.potassium.key[1] = 0x20;
	memcpy(send_netdata.data.potassium.dat, &sensor.potassium, 4);
    
    send_netdata.data.EC.key[0] = 0x00;
	send_netdata.data.EC.key[1] = 0x16;
	memcpy(send_netdata.data.EC.dat, &sensor.EC, 4);
    
//    send_netdata.data.rainfall.key[0] = 0x00;
//	send_netdata.data.rainfall.key[1] = 0x74;
//	memcpy(send_netdata.data.rainfall.dat, &sensor.rainfall, 4);
    
//    send_netdata.data.h2s.key[0] = 0x00;
//	send_netdata.data.h2s.key[1] = 0x90;
//	memcpy(send_netdata.data.h2s.dat, &sensor.h2s, 4);
    
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
//            mb_rtu_read_holdreg(send_sensor, 0x01, 0x03, 0x01, 0x02);
            mb_rtu_read_holdreg(send_sensor, 0x01, 0x03, 0x00, 0x01);
        break;
        
        case Sensor485_2 :
//            mb_rtu_read_holdreg(send_sensor, 0x02, 0x03, 0x01, 0x02);
            mb_rtu_read_holdreg(send_sensor, 0x02, 0x03, 0x1E, 0x03);
        break;
        
        case Sensor485_3 :
//            mb_rtu_read_holdreg(send_sensor, 0x03, 0x04, 0x07, 0x04);
            mb_rtu_read_holdreg(send_sensor, 0x03, 0x03, 0x04, 0x03);
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
void uart_debug_send(void)
{
    uart_send_loop(UART_DEBUG);
}

//读取485传感器类型循环
static void sensor_485_type_loop(void)
{
    if(++sensor_type >= num_of_sensor_485)
        sensor_type = Sensor485_1;
    
    resend_num = 0;
}

//modbus rtu 协议读取保持寄存器，reg_adr和reg_len高位在前
static void mb_rtu_read_holdreg(uint8_t * pbuf, uint8_t dev_adr, uint8_t func, uint16_t reg_adr, uint16_t reg_num)
{
    uint16_t crc = 0;
    
    *(pbuf + 0) = dev_adr;
    *(pbuf + 1) = func;
    *(pbuf + 2) = (reg_adr >> 8) & 0xFF;
    *(pbuf + 3) = reg_adr & 0xFF;
    *(pbuf + 4) = (reg_num >> 8) & 0xFF;
    *(pbuf + 5) = reg_num & 0xFF;
    
    crc = check_crc16_modbus_calc(pbuf, 6);
    
    *(pbuf + 6) = crc & 0xFF;
    *(pbuf + 7) = (crc >> 8) & 0xFF;
}

//处理485传感器数据
static void sensor_485_deal(uint8_t * pdata, uint32_t len)
{
//    Float_Byte float_data;
    uint32_t check_len = 0;  //要校验的帧数据长度
    
    uart_write( UART_DEBUG, pdata, len );
    
    while(len)
    {
        switch(*pdata)  //头校验
        {
            case 0x01 :
                check_len = *(pdata + 2) + 5;
                if( (len >= check_len) && (!check_crc16_modbus_calc(pdata, check_len)) )  //crc16校验为0表示通过
                {
                    sensor.ph = *(pdata + 4);
                    sensor.ph += (uint32_t)*(pdata + 3) << 8;
                    sensor.ph /= 100;
                    
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
                if( (len >= check_len) && (!check_crc16_modbus_calc(pdata, check_len)) )
                {
                    sensor.phosphorus = *(pdata + 4);
                    sensor.phosphorus += (uint32_t)*(pdata + 3) << 8;
                    sensor.nh4 = *(pdata + 6);
                    sensor.nh4 += (uint32_t)*(pdata + 5) << 8;
                    sensor.potassium = *(pdata + 8);
                    sensor.potassium += (uint32_t)*(pdata + 7) << 8;
                    
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
                if( (len >= check_len) && (!check_crc16_modbus_calc(pdata, check_len)) )
                {
                    sensor.humi = *(pdata + 4);
                    sensor.humi += (uint32_t)*(pdata + 3) << 8;
                    sensor.humi /= 10;
                    sensor.temp = *(pdata + 6);
                    sensor.temp += (uint32_t)*(pdata + 5) << 8;
                    sensor.temp /= 10;
                    sensor.EC = *(pdata + 8);
                    sensor.EC += (uint32_t)*(pdata + 7) << 8;
                    
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
