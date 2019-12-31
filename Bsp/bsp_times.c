#include "bsp_times.h"
#include "stdlib.h"  //包含NULL

static Timer_PortInfo Timer_PortInfoList[number_of_timer] = {NULL};

static void Timer_BaseInit(Timer_BaseType * pTimer_Base);  //定时器基本参数配置
static void Timer_IRQHandler_Deal(Timer_Port timer);  //定时器中断处理

//定时器初始化，timer：定时器端口；prescaler：分频值；period：自动重载值
void Timer_Init(Timer_Port timer, uint16_t prescaler, uint16_t period)
{
    Timer_ConfigType Timer_ConfigList[number_of_timer] = TIMER_CONFIG_LIST;
    
    if( (timer < number_of_timer) && (Timer_ConfigList[timer].Timer_Base.TIMx) )
    {
        Timer_ConfigList[timer].Timer_Base.TIM_Prescaler = prescaler;
        Timer_ConfigList[timer].Timer_Base.TIM_Period = period;
        
        Timer_PortInfoList[timer].TIMx = Timer_ConfigList[timer].Timer_Base.TIMx;
        
        Timer_BaseInit(&Timer_ConfigList[timer].Timer_Base);
        NVIC_Init(&Timer_ConfigList[timer].NVIC_InitStructure);  //初始化NVIC寄存器，执行完就先产生中断
    }
}

//定时器基本参数配置
static void Timer_BaseInit(Timer_BaseType * pTimer_Base)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    if(pTimer_Base->RCC_APBPeriph_Sel == Timer_RCC_APB1Periph_Sel)
        RCC_APB1PeriphClockCmd(pTimer_Base->RCC_APBPeriph, ENABLE);  //时钟使能
    else
        RCC_APB2PeriphClockCmd(pTimer_Base->RCC_APBPeriph, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Prescaler = pTimer_Base->TIM_Prescaler;  //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_Period = pTimer_Base->TIM_Period;  //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(pTimer_Base->TIMx, &TIM_TimeBaseStructure);  //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ITConfig(pTimer_Base->TIMx, TIM_IT_Update, ENABLE);  //使能指定的TIMx中断,允许更新中断
    TIM_Cmd(pTimer_Base->TIMx, ENABLE);  //使能TIMx
}

//优先任务函数注册
void Timer_PriorityTask_Regist(Timer_Port timer, void (*timer_task)(uint32_t timer_interval), uint32_t timer_interval)
{
    if( (timer < number_of_timer) && (Timer_PortInfoList[timer].TIMx) )
    {
        Timer_PortInfoList[timer].timer_interval = timer_interval;
        Timer_PortInfoList[timer].timer_task = timer_task;
    }
}

//定时器中断处理
static void Timer_IRQHandler_Deal(Timer_Port timer)
{
    if( (timer < number_of_timer) && (Timer_PortInfoList[timer].TIMx) )
    {
        if(TIM_GetITStatus(Timer_PortInfoList[timer].TIMx, TIM_IT_Update) != RESET)  //检查TIM更新中断发生与否
        {
            ++Timer_PortInfoList[timer].timer_ticks;
            
            if(Timer_PortInfoList[timer].timer_task)
                Timer_PortInfoList[timer].timer_task(Timer_PortInfoList[timer].timer_interval);
            
            TIM_ClearITPendingBit(Timer_PortInfoList[timer].TIMx, TIM_IT_Update);  //清除TIMx更新中断标志
        }
    }
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)  //TIM3中断
{
    if(Timer_PortInfoList[Timer3].TIMx == TIM3)
        Timer_IRQHandler_Deal(Timer3);
}

//获取定时器滴答数
uint32_t get_timer_ticks(Timer_Port timer)
{
    if( (timer < number_of_timer) && (Timer_PortInfoList[timer].TIMx) )
        return Timer_PortInfoList[timer].timer_ticks;
    
    return 0;  //输入无效就返回0
}
