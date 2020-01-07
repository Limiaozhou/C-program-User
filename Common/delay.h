#ifndef __DELAY_H
#define __DELAY_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "misc.h"

#elif defined STM8
#include "stm8s.h"  //数据类型重定义包含头文件
#endif

//#include "data_typedef.h"  //数据类型重定义包含头文件

void Delay_Init(uint8_t sysclk);  //延时基准初始化
void delay_us(uint16_t nus);  //延时n us
void delay_ms(uint32_t nms);  //延时n ms

#endif
