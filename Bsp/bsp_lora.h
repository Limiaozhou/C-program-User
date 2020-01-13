#ifndef __BSP_LORA_H
#define __BSP_LORA_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_gpio.h"

#elif defined STM8
#endif

//#include "data_typedef.h"  //������������

#define LORACONTROL_M0_GPIOX     GPIOB
#define LORACONTROL_M0_GPIO_PIN  GPIO_Pin_3
#define LORACONTROL_M1_GPIOX     GPIOB
#define LORACONTROL_M1_GPIO_PIN  GPIO_Pin_4

typedef enum
{
	LoraMode_Transmission = 0,  //͸��
	LoraMode_WOR,  //Զ�̻���
	LoraMode_Configure,  //����
    LoraMode_Sleep  //����
}Lora_WorkMode_TypeDef;  //lora����ģʽ

void LoraControl_GPIO_Init(void);
void lora_mode_control(Lora_WorkMode_TypeDef mode);  //lora����ģʽ����
Lora_WorkMode_TypeDef lora_mode_read(void);  //lora����ģʽ��ȡ

#endif
