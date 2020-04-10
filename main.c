/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
rt_uint8_t flag1;
rt_uint8_t flag2;

/* 定义线程控制块 */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;

ALIGN(RT_ALIGN_SIZE)
/* 定义线程栈 */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];

/* Private function prototypes -----------------------------------------------*/
void delay(uint32_t count);
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);

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

/* 软件延时 */
void delay(uint32_t count)
{
	for(; count!=0; count--);
}

/* 线程1 */
void flag1_thread_entry(void *p_arg)
{
	for(;;)
	{
		flag1 = 1;
		delay(100);
		flag1 = 0;
		delay(100);
		
		/* 线程切换，这里是手动切换 */
		rt_schedule();
	}
}

/* 线程2 */
void flag2_thread_entry(void *p_arg)
{
	for(;;)
	{
		flag2 = 1;
		delay(100);
		flag2 = 0;
		delay(100);
		
		rt_schedule();
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
//
//	/* Configure the system clock */
//	CLK_SYSCLK_Config();
//
//#if defined STM32_STANDARD
//	NVIC_PriorityGroupInit();
//	SysTick_Init();
//#endif

	/* 调度器初始化 */
	rt_system_scheduler_init();

	/* 初始化线程 */
	rt_thread_init( &rt_flag1_thread, /* 线程控制块 */
					flag1_thread_entry, /* 线程入口地址 */
					RT_NULL, /* 线程形参 */
					&rt_flag1_thread_stack[0], /* 线程栈起始地址 */
					sizeof(rt_flag1_thread_stack) ); /* 线程栈大小，单位为字节 */
	/* 将线程插入到就绪列表 */
	rt_list_insert_before( &(rt_thread_priority_table[0]),&(rt_flag1_thread.tlist) );

	rt_thread_init( &rt_flag2_thread,
					flag2_thread_entry,
					RT_NULL,
					&rt_flag2_thread_stack[0],
					sizeof(rt_flag2_thread_stack) );
	rt_list_insert_before( &(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist) );
	
	/* 启动系统调度器 */
	rt_system_scheduler_start();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
