#ifndef __APP_UART_H
#define __APP_UART_H

void debug_read(void);  //接收处理debug数据
void bluetooth_read(void);  //接收处理蓝牙数据
void zigbee_read(void);  //接收处理zigbee数据
void lora_read(void);  //接收处理zigbee数据

//lora串口发送循环，因为没使用DMA和不是用立即阻塞发送模式，需循环调用
void lora_send_loop(void);

#endif
