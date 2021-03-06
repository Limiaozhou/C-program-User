#ifndef __BSP_LED_H
#define __BSP_LED_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_gpio.h"

#elif defined STM8
#endif

//#include "data_typedef.h"  //数据类型声明

#define LED_GPIO_LIST {\
{.GPIOx = GPIOA, .Pin = GPIO_Pin_4, .active_level = 0, .RCC_APB2Periph = RCC_APB2Periph_GPIOA},\
{.GPIOx = GPIOA, .Pin = GPIO_Pin_5, .active_level = 0, .RCC_APB2Periph = RCC_APB2Periph_GPIOA},\
{.GPIOx = GPIOA, .Pin = GPIO_Pin_6, .active_level = 0, .RCC_APB2Periph = RCC_APB2Periph_GPIOA},\
{.GPIOx = GPIOD, .Pin = GPIO_Pin_2, .active_level = 0, .RCC_APB2Periph = RCC_APB2Periph_GPIOD}\
}

typedef enum
{
	Led_Off = 0,
	Led_On,
	Led_Toggle  //翻转
}LED_GPIO_OutState;  //LED输出状态

typedef enum
{
	Led1 = 0,
	Led2,
    Led3,
    Led4,
	number_of_led
}LED_GPIO_Port;  //LED端口

typedef struct
{
	GPIO_TypeDef* GPIOx;
    uint32_t RCC_APB2Periph;  //外设时钟使能
	uint16_t Pin;
	uint8_t active_level;  //有效电平
}LED_GPIOType;  //LED引脚结构体

void Led_GPIO_Init(void);
void Led_GPIO_Write(LED_GPIO_Port ledx, LED_GPIO_OutState state);
uint8_t Led_GPIO_Read(LED_GPIO_Port ledx);

#endif
