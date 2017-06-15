#ifndef __BSP_PDU_H
#define __BSP_PDU_H 			   

#include "sys.h"


//通道控制
//CH1
#define CH1RON		GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define CH1ROFF		GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define CH1YON		GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define CH1YOFF		GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define CH1GON		GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define CH1GOFF		GPIO_SetBits(GPIOB,GPIO_Pin_0)
//CH2
#define CH2RON		GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define CH2ROFF		GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define CH2YON		GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define CH2YOFF		GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define CH2GON		GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define CH2GOFF		GPIO_SetBits(GPIOB,GPIO_Pin_13)
//CH3
#define CH3RON		GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define CH3ROFF		GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define CH3YON		GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define CH3YOFF		GPIO_SetBits(GPIOC,GPIO_Pin_6)
#define CH3GON		GPIO_ResetBits(GPIOC,GPIO_Pin_7)
#define CH3GOFF		GPIO_SetBits(GPIOC,GPIO_Pin_7)
//CH4
#define CH4RON		GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define CH4ROFF		GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define CH4YON		GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define CH4YOFF		GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define CH4GON		GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define CH4GOFF		GPIO_SetBits(GPIOB,GPIO_Pin_9)

//通道状态读取
//通道状态读取
#define CH1RCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define CH1YCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define CH1GCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define CH2RCHECK	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define CH2YCHECK	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)
#define CH2GCHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)
#define CH3RCHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)
#define CH3YCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)
#define CH3GCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)
#define CH4RCHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)
#define CH4YCHECK	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define CH4GCHECK	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)

extern uint8_t ChannelVoltage[12];

void PDU_ChannelGPIO_Init(void);
void ChannelAlloff(void);
void ChannelAllon(void);
void GetChannelVoltageStatus(void);

#endif

