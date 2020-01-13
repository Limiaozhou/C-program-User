#include "bsp_lora.h"

/* lora控制引脚初始化 */
void LoraControl_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = LORACONTROL_M0_GPIO_PIN;
    GPIO_Init(LORACONTROL_M0_GPIOX, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = LORACONTROL_M1_GPIO_PIN;
    GPIO_Init(LORACONTROL_M1_GPIOX, &GPIO_InitStruct);
    
    lora_mode_control(LoraMode_Transmission);  //初始打开蓝牙透传控制引脚
}

/* lora工作模式控制 */
void lora_mode_control(Lora_WorkMode_TypeDef mode)
{
    switch(mode)
    {
        case LoraMode_Transmission :  //M0:0, M1:0
            GPIO_WriteBit(LORACONTROL_M0_GPIOX, LORACONTROL_M0_GPIO_PIN, Bit_RESET);
            GPIO_WriteBit(LORACONTROL_M1_GPIOX, LORACONTROL_M1_GPIO_PIN, Bit_RESET);
        break;
        
        case LoraMode_WOR :  //M0:1, M1:0
            GPIO_WriteBit(LORACONTROL_M0_GPIOX, LORACONTROL_M0_GPIO_PIN, Bit_SET);
            GPIO_WriteBit(LORACONTROL_M1_GPIOX, LORACONTROL_M1_GPIO_PIN, Bit_RESET);
        break;
        
        case LoraMode_Configure :  //M0:0, M1:1
            GPIO_WriteBit(LORACONTROL_M0_GPIOX, LORACONTROL_M0_GPIO_PIN, Bit_RESET);
            GPIO_WriteBit(LORACONTROL_M1_GPIOX, LORACONTROL_M1_GPIO_PIN, Bit_SET);
        break;
        
        case LoraMode_Sleep :  //M0:1, M1:1
            GPIO_WriteBit(LORACONTROL_M0_GPIOX, LORACONTROL_M0_GPIO_PIN, Bit_SET);
            GPIO_WriteBit(LORACONTROL_M1_GPIOX, LORACONTROL_M1_GPIO_PIN, Bit_SET);
        break;
        
        default :  //M0:0, M1:0
            GPIO_WriteBit(LORACONTROL_M0_GPIOX, LORACONTROL_M0_GPIO_PIN, Bit_RESET);
            GPIO_WriteBit(LORACONTROL_M1_GPIOX, LORACONTROL_M1_GPIO_PIN, Bit_RESET);
        break;
    }
}

Lora_WorkMode_TypeDef lora_mode_read(void)
{
    Lora_WorkMode_TypeDef mode = LoraMode_Transmission;
    uint8_t state = 0;  //引脚状态缓存
    
    state = GPIO_ReadOutputDataBit(LORACONTROL_M0_GPIOX, LORACONTROL_M0_GPIO_PIN);
    state |= (uint8_t)GPIO_ReadOutputDataBit(LORACONTROL_M1_GPIOX, LORACONTROL_M1_GPIO_PIN) << 1;
    
    switch(state)
    {
        case 0x00 :
            mode = LoraMode_Transmission;
        break;
        
        case 0x01 :
            mode = LoraMode_WOR;
        break;
        
        case 0x02 :
            mode = LoraMode_Configure;
        break;
        
        case 0x03 :
            mode = LoraMode_Sleep;
        break;
    }
    
    return mode;
}
