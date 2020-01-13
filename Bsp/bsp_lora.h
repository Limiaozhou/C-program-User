#ifndef __BSP_LORA_H
#define __BSP_LORA_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_gpio.h"

#elif defined STM8
#endif

//#include "data_typedef.h"  //数据类型声明

#define LORACONTROL_M0_GPIOX     GPIOB
#define LORACONTROL_M0_GPIO_PIN  GPIO_Pin_3
#define LORACONTROL_M1_GPIOX     GPIOB
#define LORACONTROL_M1_GPIO_PIN  GPIO_Pin_4

typedef enum
{
	LoraMode_Transmission = 0,  //透传
	LoraMode_WOR,  //远程唤醒
	LoraMode_Configure,  //配置
    LoraMode_Sleep  //休眠
}Lora_WorkMode_TypeDef;  //lora工作模式

void LoraControl_GPIO_Init(void);
void lora_mode_control(Lora_WorkMode_TypeDef mode);  //lora工作模式控制
Lora_WorkMode_TypeDef lora_mode_read(void);  //lora工作模式读取

#endif
