#ifndef __BSP_COIL_H
#define __BSP_COIL_H 			   

#include "sys.h"
#include "global.h"
//通道开启/关闭控制
#define CH1_Enable		GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define CH1_Disable		GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define CH2_Enable		GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define CH2_Disable		GPIO_SetBits(GPIOC,GPIO_Pin_9)

#define CH3_Enable		GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define CH3_Disable		GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define CH4_Enable		GPIO_ResetBits(GPIOC,GPIO_Pin_7)
#define CH4_Disable		GPIO_SetBits(GPIOC,GPIO_Pin_7)

#define CH5_Enable		GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define CH5_Disable		GPIO_SetBits(GPIOC,GPIO_Pin_6)

#define CH6_Enable		GPIO_ResetBits(GPIOD,GPIO_Pin_15)
#define CH6_Disable		GPIO_SetBits(GPIOD,GPIO_Pin_15)

#define CH7_Enable		GPIO_ResetBits(GPIOD,GPIO_Pin_14)
#define CH7_Disable		GPIO_SetBits(GPIOD,GPIO_Pin_14)

#define CH8_Enable		GPIO_ResetBits(GPIOD,GPIO_Pin_13)
#define CH8_Disable		GPIO_SetBits(GPIOD,GPIO_Pin_13)

extern Loop_DefType CH1,CH2,CH3,CH4,CH5,CH6,CH7,CH8;

void COIL_GPIO_Init(void);
void Exti_Configuration(void);
void All_Channel_OFF(void);
void Channel_enable(uint8_t channel);
void COIL_Check(void);
void COIL_Detect(void);

#endif

