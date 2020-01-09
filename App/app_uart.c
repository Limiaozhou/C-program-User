#include "app_uart.h"
#include "check.h"
#include "string.h"
#include "mb_rtu_master.h"
#include "data_typedef.h"

#define UART_BLUETOOTH Uart1
#define UART_ZIGBEE Uart2
#define UART_DEBUG Uart3

static void debug_deal(uint8_t * pdata, uint32_t len);
static void zigbee_deal(uint8_t * pdata, uint32_t len);

//接收处理debug数据
void debug_read(void)
{
    uart_read(UART_DEBUG, debug_deal);
}

//接收处理zigbee数据
void zigbee_read(void)
{
    uart_read(UART_ZIGBEE, zigbee_deal);
}

//处理debug数据
static void debug_deal(uint8_t * pdata, uint32_t len)
{
    uart_write( UART_BLUETOOTH, pdata, len );
}

//处理zigbee数据
static void zigbee_deal(uint8_t * pdata, uint32_t len)
{
    uart_write( UART_DEBUG, pdata, len );
}
