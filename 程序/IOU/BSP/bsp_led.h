#ifndef __BSP_LED_H
#define __BSP_LED_H 			   

#include "sys.h"

//LED控制
#define RUNLEDON		GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define RUNLEDOFF		GPIO_SetBits(GPIOC,GPIO_Pin_10)

//通道指示灯控制(Out&Input)
//CH1
//V1.1
#define CH1_inON		  GPIO_ResetBits(GPIOC,GPIO_Pin_11)//将引脚置0
#define CH1_inOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_11)  //将引脚置1
#define CH1_outON		  GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define CH1_outOFF		GPIO_SetBits(GPIOC,GPIO_Pin_12)

//CH2
//V1.1
#define CH2_inON		  GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define CH2_inOFF		  GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define CH2_outON		  GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define CH2_outOFF		GPIO_SetBits(GPIOB,GPIO_Pin_5)

//CH3
//V1.1
#define CH3_inON		  GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define CH3_inOFF		  GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define CH3_outON		  GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define CH3_outOFF		GPIO_SetBits(GPIOB,GPIO_Pin_7)

//CH4
//V1.1
#define CH4_inON		  GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define CH4_inOFF		  GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define CH4_outON		  GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define CH4_outOFF		GPIO_SetBits(GPIOB,GPIO_Pin_9)

//CH5
//V1.1
#define CH5_inON		  GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define CH5_inOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define CH5_outON		  GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define CH5_outOFF		GPIO_SetBits(GPIOC,GPIO_Pin_14)

//CH6
//V1.1
#define CH6_inON		  GPIO_ResetBits(GPIOC,GPIO_Pin_15)
#define CH6_inOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_15)
#define CH6_outON		  GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define CH6_outOFF		GPIO_SetBits(GPIOC,GPIO_Pin_0)

//CH7
//V1.1
#define CH7_inON		  GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define CH7_inOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define CH7_outON		  GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define CH7_outOFF		GPIO_SetBits(GPIOC,GPIO_Pin_2)

//CH8
//V1.1
#define CH8_inON		  GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define CH8_inOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define CH8_outON		  GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define CH8_outOFF		GPIO_SetBits(GPIOA,GPIO_Pin_0)

void LED_Init(void);
void IO_Channel_GPIO_Init(void);

void IO_OUT_ALL_ON(void);
void IO_OUT_ALL_OFF(void);
void IO_InLed_ALL_ON(void);
void IO_InLed_ALL_OFF(void);
void LED_ALL_ON(void);

#endif

