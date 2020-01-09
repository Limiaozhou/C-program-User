#include "bsp_bluetooth.h"

/* �����������ų�ʼ�� */
void BTControl_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    BTControl_GPIO_Write(BTControl_On);  //��ʼ������͸����������
}

/* д�������ƿڣ�state:���״̬ */
void BTControl_GPIO_Write(BTControl_GPIO_OutState state)
{
    uint8_t bitstatus_read = 0;
    
    if(state != BTControl_Toggle)  //���ǵ�ƽ��ת
        GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)state);
    else
    {
        bitstatus_read = BTControl_GPIO_Read();
        GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)!bitstatus_read);
    }
}

uint8_t BTControl_GPIO_Read(void)
{
    uint8_t bitstatus = 0;
    
    bitstatus = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8);
    
    return bitstatus;
}
