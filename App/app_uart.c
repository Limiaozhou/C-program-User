#include "app_uart.h"
#include "bsp_usart.h"
#include "string.h"
#include "bsp_lora.h"

#define UART_BLUETOOTH Uart1
#define UART_ZIGBEE Uart2
#define UART_DEBUG Uart3
#define UART_LORA Uart5

#define CONFIG_BLUETOOTH_EVENT 0x01  //蓝牙配置事件
#define CONFIG_ZIGBEE_EVENT 0x02
#define CONFIG_LORA_EVENT 0x04

static uint8_t bluetooth_config_mutex = 0;  //蓝牙配置互斥量，初始不可用，等待蓝牙连接后释放
static uint8_t zigbee_config_mutex = 1;  //zigbee配置互斥量
static uint8_t lora_config_mutex = 1;  //lora配置互斥量

static uint8_t bluetooth_config_flag = 0;  //蓝牙配置标志，连接后才能指令退出配置模式

static void debug_deal(uint8_t * pdata, uint32_t len);  //处理debug数据
static void zigbee_deal(uint8_t * pdata, uint32_t len);  //处理蓝牙数据
static void bluetooth_deal(uint8_t * pdata, uint32_t len);  //处理zigbee数据
static void lora_deal(uint8_t * pdata, uint32_t len);  //处理lora数据

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

//接收处理lora数据
void lora_read(void)
{
    uart_read(UART_LORA, lora_deal);
}

//lora串口发送循环，因为没使用DMA和不是用立即阻塞发送模式，需循环调用
void lora_send_loop(void)
{
    uart_nodma_send_loop(UART_LORA);
}

//处理debug数据
static void debug_deal(uint8_t * pdata, uint32_t len)
{
    static uint8_t device_config_event = 0;  //设备配置事件
    
    if(strcmp((char*)pdata, "++bluetooth") == 0)
    {  //收到开始配置蓝牙信息
        bluetooth_config_mutex = 0;
        device_config_event |= CONFIG_BLUETOOTH_EVENT;
    }
    else if((strcmp((char*)pdata, "--bluetooth") == 0) && (device_config_event & CONFIG_BLUETOOTH_EVENT))
    {  //收到结束配置蓝牙信息
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
    else if(strcmp((char*)pdata, "++lora") == 0)
    {
        lora_config_mutex = 0;
        device_config_event |= CONFIG_LORA_EVENT;
        lora_mode_control(LoraMode_Configure);  //lora模块到配置模式
    }
    else if((strcmp((char*)pdata, "--lora") == 0) && (device_config_event & CONFIG_LORA_EVENT))
    {
        lora_config_mutex = 1;
        device_config_event &= ~CONFIG_LORA_EVENT;
        lora_mode_control(LoraMode_Transmission);  //lora模块到透传模式
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
    else if(device_config_event & CONFIG_LORA_EVENT)
    {
        uart_write( UART_DEBUG, "lora config\r\n", strlen("lora config\r\n") );
        uart_write( UART_LORA, pdata, len );
    }
    else
    {
        if(bluetooth_config_mutex)
            uart_write( UART_BLUETOOTH, pdata, len );
        if(zigbee_config_mutex)
            uart_write( UART_ZIGBEE, pdata, len );
        if(lora_config_mutex)
            uart_write( UART_LORA, pdata, len );
    }
}

//处理蓝牙数据
static void bluetooth_deal(uint8_t * pdata, uint32_t len)
{
    static uint8_t device_config_event = 0;  //设备配置事件
    
    if(strstr((char*)pdata, "Update:") != NULL)
    {  //收到蓝牙连接信息
        bluetooth_config_flag = 1;
        bluetooth_config_mutex = 1;
    }
    else if(strstr((char*)pdata, "Disconnected!") != NULL)  //收到蓝牙断开信息
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
    else if(strcmp((char*)pdata, "++lora") == 0)
    {
        lora_config_mutex = 0;
        device_config_event |= CONFIG_LORA_EVENT;
        lora_mode_control(LoraMode_Configure);
    }
    else if((strcmp((char*)pdata, "--lora") == 0) && (device_config_event & CONFIG_LORA_EVENT))
    {
        lora_config_mutex = 1;
        device_config_event &= ~CONFIG_LORA_EVENT;
        lora_mode_control(LoraMode_Transmission);
    }
    
    if(device_config_event & CONFIG_ZIGBEE_EVENT)
    {
        uart_write( UART_BLUETOOTH, "zigbee config\r\n", strlen("zigbee config\r\n") );
        uart_write( UART_ZIGBEE, pdata, len );
    }
    else if(device_config_event & CONFIG_LORA_EVENT)
    {
        uart_write( UART_BLUETOOTH, "lora config\r\n", strlen("lora config\r\n") );
        uart_write( UART_LORA, pdata, len );
    }
    else if(bluetooth_config_mutex)
    {
        if(zigbee_config_mutex)
            uart_write( UART_ZIGBEE, pdata, len );
        if(lora_config_mutex)
            uart_write( UART_LORA, pdata, len );
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
    else if(strcmp((char*)pdata, "++lora") == 0)
    {
        lora_config_mutex = 0;
        device_config_event |= CONFIG_LORA_EVENT;
        lora_mode_control(LoraMode_Configure);
    }
    else if((strcmp((char*)pdata, "--lora") == 0) && (device_config_event & CONFIG_LORA_EVENT))
    {
        lora_config_mutex = 1;
        device_config_event &= ~CONFIG_LORA_EVENT;
        lora_mode_control(LoraMode_Transmission);
    }
    
    if(device_config_event & CONFIG_BLUETOOTH_EVENT)
    {
        uart_write( UART_ZIGBEE, "bluetooth config\r\n", strlen("bluetooth config\r\n") );
        uart_write( UART_BLUETOOTH, pdata, len );
    }
    else if(device_config_event & CONFIG_LORA_EVENT)
    {
        uart_write( UART_ZIGBEE, "lora config\r\n", strlen("lora config\r\n") );
        uart_write( UART_LORA, pdata, len );
    }
    else if(zigbee_config_mutex)
    {
        if(bluetooth_config_mutex)
            uart_write( UART_BLUETOOTH, pdata, len );
        if(lora_config_mutex)
            uart_write( UART_LORA, pdata, len );
    }
    uart_write( UART_DEBUG, pdata, len );
}

static void lora_deal(uint8_t * pdata, uint32_t len)
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
        uart_write( UART_LORA, "bluetooth config\r\n", strlen("bluetooth config\r\n") );
        uart_write( UART_BLUETOOTH, pdata, len );
    }
    else if(device_config_event & CONFIG_ZIGBEE_EVENT)
    {
        uart_write( UART_LORA, "zigbee config\r\n", strlen("zigbee config\r\n") );
        uart_write( UART_ZIGBEE, pdata, len );
    }
    else if(lora_config_mutex)
    {
        if(bluetooth_config_mutex)
            uart_write( UART_BLUETOOTH, pdata, len );
        if(zigbee_config_mutex)
            uart_write( UART_ZIGBEE, pdata, len );
    }
    uart_write( UART_DEBUG, pdata, len );
}
