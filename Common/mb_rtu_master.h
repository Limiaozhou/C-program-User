#ifndef __MB_RTU_MASTER_H
#define __MB_RTU_MASTER_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x.h"

#elif defined STM8
#include "stm8s.h"  //数据类型重定义包含头文件
#endif

//modbus rtu 协议读取保持寄存器，reg_adr和reg_len高位在前
void mb_rtu_read_holdreg(uint8_t * pbuf, uint8_t dev_adr, uint8_t func, uint16_t reg_adr, uint16_t reg_num);

#endif
