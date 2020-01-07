#ifndef __DATA_TYPEDEF_H
#define __DATA_TYPEDEF_H

//是否使用用户重定义的基本数据类型，1为使用
#ifndef USER_DATA_BASE_DEF
#define USER_DATA_BASE_DEF 0
#endif

//数据类型声明，若不使用用户基本数据，则要包含其他库文件
#if USER_DATA_BASE_DEF == 1
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint16_t;
typedef signed int int16_t;  //8位机int为16,32位机int为32
typedef unsigned long int uint32_t;
typedef signed long int int32_t;
typedef unsigned long long int uint64_t;
typedef signed long long int int64_t;

#else
#if defined STM32_HAL
#include "stm32f7xx_hal.h"

#elif defined STM32_STANDARD
#include "stm32f10x.h"

#elif defined STM8
#include "stm8s.h"  //数据类型重定义包含头文件
#endif
#endif

typedef union
{
    float data;
    struct
    {
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
        uint8_t byte4;
    }byte;
}Float_Byte;

#endif
