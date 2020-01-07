#ifndef __CHECK_H
#define __CHECK_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x.h"

#elif defined STM8
#include "stm8s.h"  //数据类型重定义包含头文件
#endif

//#include "data_typedef.h"  //数据类型重定义包含头文件

uint16_t check_sum16_calculate(void * pdata, uint16_t len);

uint16_t check_crc16_modbus_calc(void * pdata, uint16_t len);

#endif
