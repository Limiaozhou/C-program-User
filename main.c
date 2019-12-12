/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* 定义线程控制块 */
static rt_thread_t led1_thread_t = RT_NULL;
static rt_thread_t led2_thread_t = RT_NULL;

/* Private function prototypes -----------------------------------------------*/
//void printf_test(void);
static void led1_thread_entry(void* parameter);
static void led2_thread_entry(void* parameter);

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
    while(1)
    {
        Led_GPIO_Write(LED0, LED_TOGGLE);
        rt_thread_mdelay(500);
    }
}

static void led2_thread_entry(void* parameter)
{
    while(1)
    {
        Led_GPIO_Write(LED1, LED_TOGGLE);
        rt_thread_mdelay(1000);
        rt_kprintf("led1_thread running, LED1_TOGGLE\r\n");
    }
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
                         3, /* 线程的优先级 */
                         20); /* 线程时间片 */
    if (led1_thread_t != RT_NULL)
        rt_thread_startup(led1_thread_t);  /* 启动线程，开启调度 */
    else
        return -1;
    
    led2_thread_t =  /* 线程控制块指针 */
        rt_thread_create( "led2",  /* 线程名字 */
                         led2_thread_entry,  /* 线程入口函数 */
                         RT_NULL,  /* 线程入口函数参数 */
                         512,  /* 线程栈大小 */
                         4, /* 线程的优先级 */
                         20); /* 线程时间片 */
    if (led2_thread_t != RT_NULL)
        rt_thread_startup(led2_thread_t);  /* 启动线程，开启调度 */
    else
        return -1;
    
	/* Infinite loop */
//	while(1)
//	{
////        timeout_task_loop();
//        Led_GPIO_Write(LED0, LED_TOGGLE);
//        rt_thread_mdelay(1000);
//	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
