#include "app_uart.h"
#include "bsp_usart.h"
#include "string.h"

#define UART_BLUETOOTH Uart1
#define UART_ZIGBEE Uart2
#define UART_DEBUG Uart3

#define CONFIG_BLUETOOTH_EVENT 0x01  //蓝牙配置事件
#define CONFIG_ZIGBEE_EVENT 0x02

static uint8_t bluetooth_config_mutex = 0;  //蓝牙配置互斥量，初始不可用，等待蓝牙连接后释放
static uint8_t zigbee_config_mutex = 1;  //zigbee配置互斥量

static uint8_t bluetooth_config_flag = 0;  //蓝牙配置标志，连接后才能指令退出配置模式

static void debug_deal(uint8_t * pdata, uint32_t len);  //处理debug数据
static void zigbee_deal(uint8_t * pdata, uint32_t len);  //处理蓝牙数据
static void bluetooth_deal(uint8_t * pdata, uint32_t len);  //处理zigbee数据

//接收处理debug数据
void debug_read(void)
{
    uart_read(UART_DEBUG, debug_deal);
}

//接收处理蓝牙数据
void bluetooth_read(void)
{
    uart_read(UART_BLUETOOTH, bluetooth_deal);
}

//接收处理zigbee数据
void zigbee_read(void)
{
    uart_read(UART_ZIGBEE, zigbee_deal);
}

//处理debug数据
static void debug_deal(uint8_t * pdata, uint32_t len)
{
    static uint8_t device_config_event = 0;  //设备配置事件
    
    if(strcmp((char*)pdata, "++bluetooth") == 0)
    {
        bluetooth_config_mutex = 0;
        device_config_event |= CONFIG_BLUETOOTH_EVENT;
    }
    else if((strcmp((char*)pdata, "--bluetooth") == 0) && (device_config_event & CONFIG_BLUETOOTH_EVENT))
    {
        if(bluetooth_config_flag)
            bluetooth_config_mutex = 1;
        device_config_event &= ~CONFIG_BLUETOOTH_EVENT;
    }
    else if(strcmp((char*)pdata, "++zigbee") == 0)
    {
        zigbee_config_mutex = 0;
        device_config_event |= CONFIG_ZIGBEE_EVENT;
    }
    else if((strcmp((char*)pdata, "--zigbee") == 0) && (device_config_event & CONFIG_ZIGBEE_EVENT))
    {
        zigbee_config_mutex = 1;
        device_config_event &= ~CONFIG_ZIGBEE_EVENT;
    }
    
    if(device_config_event & CONFIG_BLUETOOTH_EVENT)
    {
        uart_write( UART_DEBUG, "bluetooth config\r\n", strlen("bluetooth config\r\n") );
        uart_write( UART_BLUETOOTH, pdata, len );
    }
    else if(device_config_event & CONFIG_ZIGBEE_EVENT)
    {
        uart_write( UART_DEBUG, "zigbee config\r\n", strlen("zigbee config\r\n") );
        uart_write( UART_ZIGBEE, pdata, len );
    }
    else
    {
        if(bluetooth_config_mutex)
            uart_write( UART_BLUETOOTH, pdata, len );
        if(zigbee_config_mutex)
            uart_write( UART_ZIGBEE, pdata, len );
    }
}

//处理蓝牙数据
static void bluetooth_deal(uint8_t * pdata, uint32_t len)
{
    static uint8_t device_config_event = 0;  //设备配置事件
    
    if(strstr((char*)pdata, "Update:") != NULL)
    {
        bluetooth_config_flag = 1;
        bluetooth_config_mutex = 1;
    }
    else if(strstr((char*)pdata, "Disconnected!") != NULL)
        bluetooth_config_mutex = 0;
    else if(strcmp((char*)pdata, "++zigbee") == 0)
    {
        zigbee_config_mutex = 0;
        device_config_event |= CONFIG_ZIGBEE_EVENT;
    }
    else if((strcmp((char*)pdata, "--zigbee") == 0) && (device_config_event & CONFIG_ZIGBEE_EVENT))
    {
        zigbee_config_mutex = 1;
        device_config_event &= ~CONFIG_ZIGBEE_EVENT;
    }
    
    if(device_config_event & CONFIG_ZIGBEE_EVENT)
    {
        uart_write( UART_BLUETOOTH, "zigbee config\r\n", strlen("zigbee config\r\n") );
        uart_write( UART_ZIGBEE, pdata, len );
    }
    else if(bluetooth_config_mutex)
    {
        if(zigbee_config_mutex)
            uart_write( UART_ZIGBEE, pdata, len );
    }
    uart_write( UART_DEBUG, pdata, len );
}

//处理zigbee数据
static void zigbee_deal(uint8_t * pdata, uint32_t len)
{
    static uint8_t device_config_event = 0;  //设备配置事件
    
    if(strcmp((char*)pdata, "++bluetooth") == 0)
    {
        bluetooth_config_mutex = 0;
        device_config_event |= CONFIG_BLUETOOTH_EVENT;
    }
    else if((strcmp((char*)pdata, "--bluetooth") == 0) && (device_config_event & CONFIG_BLUETOOTH_EVENT))
    {
        bluetooth_config_mutex = 1;
        device_config_event &= ~CONFIG_BLUETOOTH_EVENT;
    }
    
    if(device_config_event & CONFIG_BLUETOOTH_EVENT)
    {
        uart_write( UART_ZIGBEE, "bluetooth config\r\n", strlen("bluetooth config\r\n") );
        uart_write( UART_BLUETOOTH, pdata, len );
    }
    else
    {
        if(bluetooth_config_mutex)
            uart_write( UART_BLUETOOTH, pdata, len );
    }
    uart_write( UART_DEBUG, pdata, len );
}
