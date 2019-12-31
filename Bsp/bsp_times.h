#ifndef __BSP_TIMES_H
#define __BSP_TIMES_H

#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x_tim.h"

#elif defined STM8
#include "stm8s.h"  //数据类型重定义包含头文件
#endif

typedef enum
{
	Timer3 = 0,
	number_of_timer
}Timer_Port;  //定时器端口

typedef struct
{
    TIM_TypeDef* TIMx;
    uint32_t timer_ticks;  //时间滴答
    uint32_t timer_interval;  //时间间隔
    void (*timer_task)(uint32_t time_interval);  //优先任务，中断调度执行
}Timer_PortInfo;  //定时器端口信息

#if defined STM32_STANDARD
#define TIMER_CONFIG_LIST {\
    {\
        {.TIMx = TIM3, .RCC_APBPeriph = RCC_APB1Periph_TIM3, .RCC_APBPeriph_Sel = Timer_RCC_APB1Periph_Sel},\
        {.NVIC_IRQChannel = TIM3_IRQn, .NVIC_IRQChannelPreemptionPriority = 0,\
         .NVIC_IRQChannelSubPriority = 3, .NVIC_IRQChannelCmd = ENABLE}\
    },\
}

typedef enum
{
	Timer_RCC_APB1Periph_Sel = 0,
	Timer_RCC_APB2Periph_Sel
}Timer_RCC_APBPeriph_Sel;  //外设时钟选择

typedef struct
{
    Timer_RCC_APBPeriph_Sel RCC_APBPeriph_Sel;
    uint16_t TIM_Prescaler;
    uint16_t TIM_Period;
    TIM_TypeDef* TIMx;
    uint32_t RCC_APBPeriph;
}Timer_BaseType;

typedef struct
{
    Timer_BaseType Timer_Base;
    NVIC_InitTypeDef NVIC_InitStructure;
}Timer_ConfigType;  //timer配置
#endif

void Timer_Init(Timer_Port timer, uint16_t prescaler, uint16_t period);
void Timer_PriorityTask_Regist(Timer_Port timer, void (*timer_task)(uint32_t time_interval), uint32_t timer_interval);  //优先任务函数注册
uint32_t get_timer_ticks(Timer_Port timer);  //获取定时器滴答数

#endif
