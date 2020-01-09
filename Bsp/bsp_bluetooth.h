#ifndef __BSP_BLUETOOTH_H
#define __BSP_BLUETOOTH_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_gpio.h"

#elif defined STM8
#endif

//#include "data_typedef.h"  //数据类型声明

typedef enum
{
	BTControl_On = 0,
	BTControl_Off,
	BTControl_Toggle  //翻转
}BTControl_GPIO_OutState;  //蓝牙控制引脚输出状态

void BTControl_GPIO_Init(void);
void BTControl_GPIO_Write(BTControl_GPIO_OutState state);
uint8_t BTControl_GPIO_Read(void);

#endif
