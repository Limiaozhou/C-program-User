/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY1_EVENT (0x01 << 0)  //�����¼������λ0
#define KEY2_EVENT (0x01 << 1)  //�����¼������λ1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* �����߳̿��ƿ� */
static rt_thread_t led1_thread_t = RT_NULL;
//static rt_thread_t key_thread_t = RT_NULL;
static rt_thread_t receive_thread_t = RT_NULL;
static rt_thread_t send_thread_t = RT_NULL;

/* ������Ϣ���п��ƿ� */
//static rt_mq_t test_mq = RT_NULL;

/* �����ź������ƿ� */
//static rt_sem_t test_sem = RT_NULL;

/* ���廥�������ƿ� */
//static rt_mutex_t test_mux = RT_NULL;

/* �����¼����ƿ�(���) */
static rt_event_t test_event = RT_NULL;

/* �����������ʱ���ƿ� */
static rt_timer_t swtmr1 = RT_NULL;
static rt_timer_t swtmr2 = RT_NULL;

//uint8_t ucValue [ 2 ] = { 0x00, 0x00 };
static uint32_t TmrCb_Count1 = 0;
static uint32_t TmrCb_Count2 = 0;

/* Private function prototypes -----------------------------------------------*/
//void printf_test(void);
static void led1_thread_entry(void* parameter);
//static void key_thread_entry(void* parameter);
static void receive_thread_entry(void* parameter);
static void send_thread_entry(void* parameter);
static void swtmr1_callback(void* parameter);
static void swtmr2_callback(void* parameter);

/* Private functions ---------------------------------------------------------*/
//void printf_test(void)
//{
//    printf("Uart_ConfigType size : %d byte\r\n", sizeof(Uart_ConfigType));
//    printf("Uart_DMAType size : %d byte\r\n", sizeof(Uart_DMAType));
//    printf("Uart_GPIOType size : %d byte\r\n", sizeof(Uart_GPIOType));
//    printf("Uart_UartType size : %d byte\r\n", sizeof(Uart_UartType));
//    printf("Uart_NVICType size : %d byte\r\n", sizeof(Uart_NVICType));
//    printf("Uart_PortInfo size : %d byte\r\n", sizeof(Uart_PortInfo));
//    printf("UartTx_Mode_SelType size : %d byte\r\n", sizeof(UartTx_Mode_SelType));
//    printf("Current parameters value: file %s on line %d\r\n", (uint8_t *)__FILE__, __LINE__);
//}

static void led1_thread_entry(void* parameter)
{
//    uint32_t i, j;
    while(1)
    {
        Led_GPIO_Write(LED0, LED_TOGGLE);
//        rt_kprintf("led1_thread running, LED0_TOGGLE\n");
//        for(i = 0; i <= 2000; ++i)
//        {
//            for(j = 0; j <= 1000; ++j)
//                ;
//        }
        rt_thread_mdelay(500);
    }
}

//static void key_thread_entry(void* parameter)
//{
//    rt_err_t uwRet = RT_EOK;
//    static uint8_t cnt = 0, debounce_cnt = 0;
//    static uint8_t suspend_flag = 0, resume_flag = 1;
//    
//    while(1)
//    {
//        if(!Key_GPIO_Read(KEY1))
//        {
//            if((++debounce_cnt) >= 2)
//            {
//                debounce_cnt = 0;
//                cnt = 0;
//                if(!suspend_flag)
//                {
//                    suspend_flag = 1;
//                    resume_flag = 0;
//                    rt_kprintf("suspend LED1 thread\n", cnt);
//                    uwRet = rt_thread_suspend(led1_thread_t);  /* ����LED1�߳� */
//                    if (RT_EOK == uwRet)
//                        rt_kprintf("suspend LED1 thread success!\n");
//                    else
//                        rt_kprintf("suspend LED1 thread failure! failure code: 0x%lx\n",uwRet);
//                }
//            }
//        }
//        else
//        {
//            debounce_cnt = 0;
//            ++cnt;
//        }
//        
//        if(cnt >= 200)
//        {
//            cnt = 0;
//            if(!resume_flag)
//            {
//                resume_flag = 1;
//                suspend_flag = 0;
//                rt_kprintf("resume LED1 thread\n");
//                uwRet = rt_thread_resume(led1_thread_t);/* �ָ�LED1�̣߳� */
//                if (RT_EOK == uwRet)
//                    rt_kprintf("resume LED1 thread success!\n");
//                else
//                    rt_kprintf("resume LED1 thread failure! failure code: 0x%lx\n",uwRet);
//            }
//        }
//        
//        rt_thread_mdelay(20);
//    }
//}

static void receive_thread_entry(void* parameter)
{
//    rt_err_t uwRet = RT_EOK;
//    uint32_t r_queue;
    rt_uint32_t recved;
//    
//    while(1)
//    {
//        uwRet = rt_mq_recv(test_mq,  /* ��ȡ�����գ����е�ID(���) */
//                           &r_queue,  /* ��ȡ�����գ������ݱ���λ�� */
//                           sizeof(r_queue),  /* ��ȡ�����գ������ݵĳ��� */
//                           RT_WAITING_FOREVER);  /* �ȴ�ʱ�䣺һֱ�� */
//        if (RT_EOK == uwRet)
//            rt_kprintf("���ν��յ��������ǣ�%d\n",r_queue);  /* ����������ģ��ļ�ҪΪANSI���� */
//        else
//            rt_kprintf("���ݽ��ճ���,�������: 0x%lx\n",uwRet);
//        
//        rt_thread_delay(200);
//    }
//    while(1)
//    {
////        rt_sem_take(test_sem,  /* ��ȡ�ź��� */
////                    RT_WAITING_FOREVER);  /* �ȴ�ʱ�䣺һֱ�� */
//        rt_mutex_take(test_mux,  /* ��ȡ������ */
//                      RT_WAITING_FOREVER);  /* �ȴ�ʱ�䣺һֱ�� */
//        
//        if ( ucValue [ 0 ] == ucValue [ 1 ] )
//            rt_kprintf ( "Successful\n" );
//        else
//            rt_kprintf ( "Fail\n" );
//        
////        rt_sem_release( test_sem );  //�ͷ��ź���
//        rt_mutex_release( test_mux );  //�ͷŻ�����
//        rt_thread_delay ( 1000 );
//    }
//    while(1)
//    {
//        if(!Key_GPIO_Read(KEY0))
//        {
//            uwRet = rt_sem_take(test_sem,  /* ��ȡ�ź��� */
//                                0);  /* �ȴ�ʱ�䣺0 */
//            if ( RT_EOK == uwRet )
//                rt_kprintf( "KEY1���������ɹ����뵽ͣ��λ��\r\n" );
//            else
//                rt_kprintf( "KEY1��������������˼������ͣ����������\r\n" );
//        }
//        rt_thread_delay ( 20 );
//    }
    while(1)
    {
        rt_event_recv(test_event,  /* �¼������� */
                      KEY1_EVENT | KEY2_EVENT,  /* �����̸߳���Ȥ���¼� */
                      RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,  /* ����ѡ�� */
                      RT_WAITING_FOREVER,  /* ָ����ʱ�¼�,һֱ�� */
                      &recved);  /* ָ����յ����¼� */
        if (recved == (KEY1_EVENT | KEY2_EVENT))  /* ���������ɲ�����ȷ */
            rt_kprintf ( "Key1��Key2������\n");
        else
            rt_kprintf ( "�¼�����\n");
    }
}

static void send_thread_entry(void* parameter)
{
//    rt_err_t uwRet = RT_EOK;
//    uint32_t send_data1 = 1;
//    uint32_t send_data2 = 2;
//    static uint8_t cnt = 0, debounce_cnt = 0;
//    
//    while(1)
//    {
//        if(!Key_GPIO_Read(KEY1))
//        {
//            if((++debounce_cnt) >= 2)
//            {
//                debounce_cnt = 0;
//                cnt = 0;
//                uwRet = rt_mq_send(test_mq,  /* д�루���ͣ����е�ID(���) */
//                                   &send_data1,  /* д�루���ͣ������� */
//                                   sizeof(send_data1));  /* ���ݵĳ��� */
//                if (RT_EOK != uwRet)
//                    rt_kprintf("���ݲ��ܷ��͵���Ϣ���У��������: %lx\n",uwRet);
//            }
//        }
//        else
//        {
//            debounce_cnt = 0;
//            ++cnt;
//        }
//        
//        if(cnt >= 50)
//        {
//            cnt = 0;
//            uwRet = rt_mq_send(test_mq,
//                               &send_data2,
//                               sizeof(send_data2));
//            if (RT_EOK != uwRet)
//                rt_kprintf("���ݲ��ܷ��͵���Ϣ���У��������: %lx\n",uwRet);
//        }
//        
//        rt_thread_delay(20);
//    }
//    while(1)
//    {
////        rt_sem_take(test_sem,
////                    RT_WAITING_FOREVER);
//        rt_mutex_take(test_mux,
//                      RT_WAITING_FOREVER);
//        
//        ucValue [ 0 ] ++;
//        rt_thread_delay ( 100 );
//        ucValue [ 1 ] ++;
////        rt_sem_release(test_sem);
//        rt_mutex_release(test_mux);
//        rt_thread_yield();  //����ʣ��ʱ��Ƭ������һ���߳��л�
//    }
//    while(1)
//    {
//        if(!Key_GPIO_Read(KEY1))
//        {
//            uwRet = rt_sem_release(test_sem);
//            if ( RT_EOK == uwRet )
//                rt_kprintf ( "KEY2���������ͷ�1��ͣ��λ��\r\n" );
//            else
//                rt_kprintf ( "KEY2�������������޳�λ�����ͷţ�\r\n" );
//        }
//        rt_thread_delay(20);
//    }
    while(1)
    {
        if(!Key_GPIO_Read(KEY0))
        {
            rt_kprintf ( "KEY0������\n" );
            rt_event_send(test_event, KEY1_EVENT);
        }
        if(!Key_GPIO_Read(KEY1))
        {
            rt_kprintf ( "KEY1������\n" );
            rt_event_send(test_event, KEY2_EVENT);
        }
        rt_thread_delay(20);
    }
}

static void swtmr1_callback(void* parameter)
{
    uint32_t tick_num1;
    
    TmrCb_Count1++;
    tick_num1 = (uint32_t)rt_tick_get();  /* ��ȡ�δ�ʱ���ļ���ֵ */
    rt_kprintf("swtmr1_callback����ִ�� %d ��\n", TmrCb_Count1);
    rt_kprintf("�δ�ʱ����ֵ=%d\n", tick_num1);
}

static void swtmr2_callback(void* parameter)
{
    uint32_t tick_num2;
    
    TmrCb_Count2++;
    tick_num2 = (uint32_t)rt_tick_get();  /* ��ȡ�δ�ʱ���ļ���ֵ */
    rt_kprintf("swtmr2_callback����ִ�� %d ��\n", TmrCb_Count2);
    rt_kprintf("�δ�ʱ����ֵ=%d\n", tick_num2);
}

/* Main program */
int main(void)
{
//#if defined STM32_HAL
//	/* STM32 Configure the MPU attributes as Write Through */
//	MPU_Config();
//    
//	/* STM32 Enable the CPU Cache */
//	CPU_CACHE_Enable();
//	
//	/* STM32F7xx HAL library initialization */
//	HAL_Init();
//#endif
    
//	/* Configure the system clock */
//	CLK_SYSCLK_Config();
//	
//#if defined STM32_STANDARD
//    NVIC_PriorityGroupInit();
//    SysTick_Init();
//#endif
    
//	Delay_Init(72);  //��ʱ������׼����
//    Led_GPIO_Init();
//    Key_GPIO_Init();
//    TIM3_Init(719, 99, Timer_Update);  //720 * 100 / 72000000 = 0.001s = 1ms
//    Uart_Init(Uart1, 115200, 200, 200, UartTx_Interrupt_Sel);  //usart1��115200�����͡����ջ����С200���жϷ���ģʽ
//    Uart_Init(Uart2, 115200, 50, 50, UartTx_Interrupt_Sel);
//    Uart_Init(Uart3, 9600, 50, 50, UartTx_Interrupt_Sel);
//    Uart_Init(Uart4, 115200, 50, 50, UartTx_Interrupt_Sel);
//    Uart_Init(Uart5, 115200, 50, 200, UartTx_Interrupt_Sel);
//    IWDG_Init(IWDG_Prescaler_64, 1000);  //1.6s���
    
//    timer_task_start(100, 0, 0, printf_test);
//    timer_task_start(1000, 1000, 0, IWDG_Feed);
//    timer_task_start(10000, 10000, 0, network_data_write);
//    timer_task_start(2000, 2000, 0, sensor_485_write);
//    timer_task_start(100, 100, 0, sensor_485_read);
//    timer_task_start(100, 100, 0, uart_debug_send);
//    timer_task_start(1000, 1000, 0, led);
    
    led1_thread_t =  /* �߳̿��ƿ�ָ�� */
        rt_thread_create( "led1",  /* �߳����� */
                         led1_thread_entry,  /* �߳���ں��� */
                         RT_NULL,  /* �߳���ں������� */
                         512,  /* �߳�ջ��С */
                         4, /* �̵߳����ȼ� */
                         20); /* �߳�ʱ��Ƭ */
    if (led1_thread_t != RT_NULL)
        rt_thread_startup(led1_thread_t);  /* �����̣߳��������� */
    else
        return -1;
    
//    key_thread_t =
//        rt_thread_create( "key",
//                         key_thread_entry,
//                         RT_NULL,
//                         512,
//                         2,
//                         20);
//    if (key_thread_t != RT_NULL)
//        rt_thread_startup(key_thread_t);
//    else
//        return -1;
    
//    test_mq = rt_mq_create("test_mq",  /* ��Ϣ�������� */
//                           40,  /* ��Ϣ����󳤶� */
//                           20,  /* ��Ϣ���е�������� */
//                           RT_IPC_FLAG_FIFO);  /* ����ģʽ FIFO(0x00)*/
//    if (test_mq != RT_NULL)
//        rt_kprintf("��Ϣ���д����ɹ���\n\n");
    
//    test_sem = rt_sem_create("test_sem",  /* �ź������� */
//                             5,  /* �ź�����ʼֵ */
//                             RT_IPC_FLAG_FIFO);  /* �ź���ģʽ FIFO(0x00)*/
//    if (test_sem != RT_NULL)
//        rt_kprintf("�ź��������ɹ���\n\n");
    
//    test_mux = rt_mutex_create("test_mux",RT_IPC_FLAG_PRIO);
//    if (test_mux != RT_NULL)
//        rt_kprintf("�����������ɹ���\n\n");
    
    test_event = rt_event_create("test_event",  /* �¼���־������ */
                                 RT_IPC_FLAG_PRIO);  /* �¼�ģʽ FIFO(0x00)*/
    if (test_event != RT_NULL)
        rt_kprintf("�¼������ɹ���\n\n");
    
    receive_thread_t =
        rt_thread_create( "receive",
                         receive_thread_entry,
                         RT_NULL,
                         512,
                         3,
                         20);
    if (receive_thread_t != RT_NULL)
        rt_thread_startup(receive_thread_t);
    else
        return -1;
    
    send_thread_t =
        rt_thread_create( "send",
                         send_thread_entry,
                         RT_NULL,
                         512,
                         2,
                         20);
    if (send_thread_t != RT_NULL)
        rt_thread_startup(send_thread_t);
    else
        return -1;
    
    swtmr1 = rt_timer_create("swtmr1_callback",  /* �����ʱ�������� */
                             swtmr1_callback,  /* �����ʱ���ĳ�ʱ���� */
                             0,  /* ��ʱ����ʱ��������ڲ��� */
                             5000,  /* �����ʱ���ĳ�ʱʱ��(���ڳ�ʱʱ��) */
                             RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);  /* һ��ģʽ �����ʱ��ģʽ */
    if (swtmr1 != RT_NULL)
        rt_timer_start(swtmr1);
    
    swtmr2 = rt_timer_create("swtmr2_callback",
                             swtmr2_callback,
                             0,
                             1000,
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);  /* ����ģʽ */
    if (swtmr2 != RT_NULL)
        rt_timer_start(swtmr2);
    
	/* Infinite loop */
//	while(1)
//	{
////        timeout_task_loop();
//        Led_GPIO_Write(LED0, LED_TOGGLE);
//        rt_thread_mdelay(1000);
//	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
