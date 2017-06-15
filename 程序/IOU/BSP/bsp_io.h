#ifndef __BSP_IO_H
#define __BSP_IO_H 			   

#include "sys.h"


//通道输入状态读取(CHx-SV)
//V1.1对应引脚驱动
#define CH1CHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)//默认为高，1为无输入，0为有输入
#define CH2CHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define CH3CHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)
#define CH4CHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)
#define CH5CHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define CH6CHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)
#define CH7CHECK	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
#define CH8CHECK	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)

extern uint8_t CH_IN[8];

void IO_InCheck_GPIO_Init(void);
void GetChannelStatus(void);

#endif

