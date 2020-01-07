#ifndef __BSP_IWDG_H
#define __BSP_IWDG_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_iwdg.h"

#elif defined STM8
#endif

//#include "data_typedef.h"

void IWDG_Init(uint8_t IWDG_Prescaler, uint16_t Reload);
void IWDG_Feed(void);

#endif
