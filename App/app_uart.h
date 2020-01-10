#ifndef __APP_UART_H
#define __APP_UART_H

void debug_read(void);  //接收处理debug数据
void bluetooth_read(void);  //接收处理蓝牙数据
void zigbee_read(void);  //接收处理zigbee数据

#endif
