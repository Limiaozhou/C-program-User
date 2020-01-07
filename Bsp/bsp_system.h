#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_rcc.h"

#elif defined STM8
#include "stm8s.h"  //使用库函数不能包括"iostm8s103F3.h"
#include "stm8s_clk.h"
#endif

//#include "data_typedef.h"  //数据类型声明

#if defined STM32_HAL
void MPU_Config(void);
void CPU_CACHE_Enable(void);
#endif

void CLK_SYSCLK_Config(void);

#if defined STM32_STANDARD
void NVIC_PriorityGroupInit(void);
void SysTick_Init(void);
#endif

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t * file, uint32_t line);
#endif

#endif
