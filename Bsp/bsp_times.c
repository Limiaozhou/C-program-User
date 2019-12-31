#include "bsp_times.h"
#include "stdlib.h"  //����NULL

static Timer_PortInfo Timer_PortInfoList[number_of_timer] = {NULL};

static void Timer_BaseInit(Timer_BaseType * pTimer_Base);  //��ʱ��������������
static void Timer_IRQHandler_Deal(Timer_Port timer);  //��ʱ���жϴ���

//��ʱ����ʼ����timer����ʱ���˿ڣ�prescaler����Ƶֵ��period���Զ�����ֵ
void Timer_Init(Timer_Port timer, uint16_t prescaler, uint16_t period)
{
    Timer_ConfigType Timer_ConfigList[number_of_timer] = TIMER_CONFIG_LIST;
    
    if( (timer < number_of_timer) && (Timer_ConfigList[timer].Timer_Base.TIMx) )
    {
        Timer_ConfigList[timer].Timer_Base.TIM_Prescaler = prescaler;
        Timer_ConfigList[timer].Timer_Base.TIM_Period = period;
        
        Timer_PortInfoList[timer].TIMx = Timer_ConfigList[timer].Timer_Base.TIMx;
        
        Timer_BaseInit(&Timer_ConfigList[timer].Timer_Base);
        NVIC_Init(&Timer_ConfigList[timer].NVIC_InitStructure);  //��ʼ��NVIC�Ĵ�����ִ������Ȳ����ж�
    }
}

//��ʱ��������������
static void Timer_BaseInit(Timer_BaseType * pTimer_Base)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    if(pTimer_Base->RCC_APBPeriph_Sel == Timer_RCC_APB1Periph_Sel)
        RCC_APB1PeriphClockCmd(pTimer_Base->RCC_APBPeriph, ENABLE);  //ʱ��ʹ��
    else
        RCC_APB2PeriphClockCmd(pTimer_Base->RCC_APBPeriph, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Prescaler = pTimer_Base->TIM_Prescaler;  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_Period = pTimer_Base->TIM_Period;  //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(pTimer_Base->TIMx, &TIM_TimeBaseStructure);  //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ITConfig(pTimer_Base->TIMx, TIM_IT_Update, ENABLE);  //ʹ��ָ����TIMx�ж�,��������ж�
    TIM_Cmd(pTimer_Base->TIMx, ENABLE);  //ʹ��TIMx
}

//����������ע��
void Timer_PriorityTask_Regist(Timer_Port timer, void (*timer_task)(uint32_t timer_interval), uint32_t timer_interval)
{
    if( (timer < number_of_timer) && (Timer_PortInfoList[timer].TIMx) )
    {
        Timer_PortInfoList[timer].timer_interval = timer_interval;
        Timer_PortInfoList[timer].timer_task = timer_task;
    }
}

//��ʱ���жϴ���
static void Timer_IRQHandler_Deal(Timer_Port timer)
{
    if( (timer < number_of_timer) && (Timer_PortInfoList[timer].TIMx) )
    {
        if(TIM_GetITStatus(Timer_PortInfoList[timer].TIMx, TIM_IT_Update) != RESET)  //���TIM�����жϷ������
        {
            ++Timer_PortInfoList[timer].timer_ticks;
            
            if(Timer_PortInfoList[timer].timer_task)
                Timer_PortInfoList[timer].timer_task(Timer_PortInfoList[timer].timer_interval);
            
            TIM_ClearITPendingBit(Timer_PortInfoList[timer].TIMx, TIM_IT_Update);  //���TIMx�����жϱ�־
        }
    }
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)  //TIM3�ж�
{
    if(Timer_PortInfoList[Timer3].TIMx == TIM3)
        Timer_IRQHandler_Deal(Timer3);
}

//��ȡ��ʱ���δ���
uint32_t get_timer_ticks(Timer_Port timer)
{
    if( (timer < number_of_timer) && (Timer_PortInfoList[timer].TIMx) )
        return Timer_PortInfoList[timer].timer_ticks;
    
    return 0;  //������Ч�ͷ���0
}
