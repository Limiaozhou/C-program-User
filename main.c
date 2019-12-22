/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY1_EVENT (0x01 << 0)  //设置事件掩码的位0
#define KEY2_EVENT (0x01 << 1)  //设置事件掩码的位1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* 定义线程控制块 */
static rt_thread_t led1_thread_t = RT_NULL;
//static rt_thread_t key_thread_t = RT_NULL;
static rt_thread_t receive_thread_t = RT_NULL;
static rt_thread_t send_thread_t = RT_NULL;

/* 定义消息队列控制块 */
//static rt_mq_t test_mq = RT_NULL;

/* 定义信号量控制块 */
//static rt_sem_t test_sem = RT_NULL;

/* 定义互斥量控制块 */
//static rt_mutex_t test_mux = RT_NULL;

/* 定义事件控制块(句柄) */
static rt_event_t test_event = RT_NULL;

/* 定义线软件定时器制块 */
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
//                    uwRet = rt_thread_suspend(led1_thread_t);  /* 挂起LED1线程 */
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
//                uwRet = rt_thread_resume(led1_thread_t);/* 恢复LED1线程！ */
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
//        uwRet = rt_mq_recv(test_mq,  /* 读取（接收）队列的ID(句柄) */
//                           &r_queue,  /* 读取（接收）的数据保存位置 */
//                           sizeof(r_queue),  /* 读取（接收）的数据的长度 */
//                           RT_WAITING_FOREVER);  /* 等待时间：一直等 */
//        if (RT_EOK == uwRet)
//            rt_kprintf("本次接收到的数据是：%d\n",r_queue);  /* 串口输出中文，文件要为ANSI编码 */
//        else
//            rt_kprintf("数据接收出错,错误代码: 0x%lx\n",uwRet);
//        
//        rt_thread_delay(200);
//    }
//    while(1)
//    {
////        rt_sem_take(test_sem,  /* 获取信号量 */
////                    RT_WAITING_FOREVER);  /* 等待时间：一直等 */
//        rt_mutex_take(test_mux,  /* 获取互斥量 */
//                      RT_WAITING_FOREVER);  /* 等待时间：一直等 */
//        
//        if ( ucValue [ 0 ] == ucValue [ 1 ] )
//            rt_kprintf ( "Successful\n" );
//        else
//            rt_kprintf ( "Fail\n" );
//        
////        rt_sem_release( test_sem );  //释放信号量
//        rt_mutex_release( test_mux );  //释放互斥量
//        rt_thread_delay ( 1000 );
//    }
//    while(1)
//    {
//        if(!Key_GPIO_Read(KEY0))
//        {
//            uwRet = rt_sem_take(test_sem,  /* 获取信号量 */
//                                0);  /* 等待时间：0 */
//            if ( RT_EOK == uwRet )
//                rt_kprintf( "KEY1被单击：成功申请到停车位。\r\n" );
//            else
//                rt_kprintf( "KEY1被单击：不好意思，现在停车场已满！\r\n" );
//        }
//        rt_thread_delay ( 20 );
//    }
    while(1)
    {
        rt_event_recv(test_event,  /* 事件对象句柄 */
                      KEY1_EVENT | KEY2_EVENT,  /* 接收线程感兴趣的事件 */
                      RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,  /* 接收选项 */
                      RT_WAITING_FOREVER,  /* 指定超时事件,一直等 */
                      &recved);  /* 指向接收到的事件 */
        if (recved == (KEY1_EVENT | KEY2_EVENT))  /* 如果接收完成并且正确 */
            rt_kprintf ( "Key1与Key2都按下\n");
        else
            rt_kprintf ( "事件错误！\n");
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
//                uwRet = rt_mq_send(test_mq,  /* 写入（发送）队列的ID(句柄) */
//                                   &send_data1,  /* 写入（发送）的数据 */
//                                   sizeof(send_data1));  /* 数据的长度 */
//                if (RT_EOK != uwRet)
//                    rt_kprintf("数据不能发送到消息队列！错误代码: %lx\n",uwRet);
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
//                rt_kprintf("数据不能发送到消息队列！错误代码: %lx\n",uwRet);
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
//        rt_thread_yield();  //放弃剩余时间片，进行一次线程切换
//    }
//    while(1)
//    {
//        if(!Key_GPIO_Read(KEY1))
//        {
//            uwRet = rt_sem_release(test_sem);
//            if ( RT_EOK == uwRet )
//                rt_kprintf ( "KEY2被单击：释放1个停车位。\r\n" );
//            else
//                rt_kprintf ( "KEY2被单击：但已无车位可以释放！\r\n" );
//        }
//        rt_thread_delay(20);
//    }
    while(1)
    {
        if(!Key_GPIO_Read(KEY0))
        {
            rt_kprintf ( "KEY0被单击\n" );
            rt_event_send(test_event, KEY1_EVENT);
        }
        if(!Key_GPIO_Read(KEY1))
        {
            rt_kprintf ( "KEY1被单击\n" );
            rt_event_send(test_event, KEY2_EVENT);
        }
        rt_thread_delay(20);
    }
}

static void swtmr1_callback(void* parameter)
{
    uint32_t tick_num1;
    
    TmrCb_Count1++;
    tick_num1 = (uint32_t)rt_tick_get();  /* 获取滴答定时器的计数值 */
    rt_kprintf("swtmr1_callback函数执行 %d 次\n", TmrCb_Count1);
    rt_kprintf("滴答定时器数值=%d\n", tick_num1);
}

static void swtmr2_callback(void* parameter)
{
    uint32_t tick_num2;
    
    TmrCb_Count2++;
    tick_num2 = (uint32_t)rt_tick_get();  /* 获取滴答定时器的计数值 */
    rt_kprintf("swtmr2_callback函数执行 %d 次\n", TmrCb_Count2);
    rt_kprintf("滴答定时器数值=%d\n", tick_num2);
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
    
//	Delay_Init(72);  //延时函数基准配置
//    Led_GPIO_Init();
//    Key_GPIO_Init();
//    TIM3_Init(719, 99, Timer_Update);  //720 * 100 / 72000000 = 0.001s = 1ms
//    Uart_Init(Uart1, 115200, 200, 200, UartTx_Interrupt_Sel);  //usart1，115200，发送、接收缓存大小200，中断发送模式
//    Uart_Init(Uart2, 115200, 50, 50, UartTx_Interrupt_Sel);
//    Uart_Init(Uart3, 9600, 50, 50, UartTx_Interrupt_Sel);
//    Uart_Init(Uart4, 115200, 50, 50, UartTx_Interrupt_Sel);
//    Uart_Init(Uart5, 115200, 50, 200, UartTx_Interrupt_Sel);
//    IWDG_Init(IWDG_Prescaler_64, 1000);  //1.6s溢出
    
//    timer_task_start(100, 0, 0, printf_test);
//    timer_task_start(1000, 1000, 0, IWDG_Feed);
//    timer_task_start(10000, 10000, 0, network_data_write);
//    timer_task_start(2000, 2000, 0, sensor_485_write);
//    timer_task_start(100, 100, 0, sensor_485_read);
//    timer_task_start(100, 100, 0, uart_debug_send);
//    timer_task_start(1000, 1000, 0, led);
    
    led1_thread_t =  /* 线程控制块指针 */
        rt_thread_create( "led1",  /* 线程名字 */
                         led1_thread_entry,  /* 线程入口函数 */
                         RT_NULL,  /* 线程入口函数参数 */
                         512,  /* 线程栈大小 */
                         4, /* 线程的优先级 */
                         20); /* 线程时间片 */
    if (led1_thread_t != RT_NULL)
        rt_thread_startup(led1_thread_t);  /* 启动线程，开启调度 */
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
    
//    test_mq = rt_mq_create("test_mq",  /* 消息队列名字 */
//                           40,  /* 消息的最大长度 */
//                           20,  /* 消息队列的最大容量 */
//                           RT_IPC_FLAG_FIFO);  /* 队列模式 FIFO(0x00)*/
//    if (test_mq != RT_NULL)
//        rt_kprintf("消息队列创建成功！\n\n");
    
//    test_sem = rt_sem_create("test_sem",  /* 信号量名字 */
//                             5,  /* 信号量初始值 */
//                             RT_IPC_FLAG_FIFO);  /* 信号量模式 FIFO(0x00)*/
//    if (test_sem != RT_NULL)
//        rt_kprintf("信号量创建成功！\n\n");
    
//    test_mux = rt_mutex_create("test_mux",RT_IPC_FLAG_PRIO);
//    if (test_mux != RT_NULL)
//        rt_kprintf("互斥量创建成功！\n\n");
    
    test_event = rt_event_create("test_event",  /* 事件标志组名字 */
                                 RT_IPC_FLAG_PRIO);  /* 事件模式 FIFO(0x00)*/
    if (test_event != RT_NULL)
        rt_kprintf("事件创建成功！\n\n");
    
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
    
    swtmr1 = rt_timer_create("swtmr1_callback",  /* 软件定时器的名称 */
                             swtmr1_callback,  /* 软件定时器的超时函数 */
                             0,  /* 定时器超时函数的入口参数 */
                             5000,  /* 软件定时器的超时时间(周期超时时间) */
                             RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);  /* 一次模式 软件定时器模式 */
    if (swtmr1 != RT_NULL)
        rt_timer_start(swtmr1);
    
    swtmr2 = rt_timer_create("swtmr2_callback",
                             swtmr2_callback,
                             0,
                             1000,
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);  /* 周期模式 */
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
