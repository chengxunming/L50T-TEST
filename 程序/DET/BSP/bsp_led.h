#ifndef __BSP_LED_H
#define __BSP_LED_H 			   

#include "sys.h"

//LED控制
#define RUNLEDON		GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define RUNLEDOFF		GPIO_SetBits(GPIOC,GPIO_Pin_10)


//各通道检测LED/输出光耦及故障LED控制
#define CH1_Check_Set		GPIO_ResetBits(GPIOC,GPIO_Pin_11)
#define CH1_Check_Reset		GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define CH1_Error_Set		GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define CH1_Error_Reset		GPIO_SetBits(GPIOC,GPIO_Pin_12)

#define CH2_Check_Set		GPIO_ResetBits(GPIOD,GPIO_Pin_0)
#define CH2_Check_Reset		GPIO_SetBits(GPIOD,GPIO_Pin_0)
#define CH2_Error_Set		GPIO_ResetBits(GPIOD,GPIO_Pin_1)
#define CH2_Error_Reset		GPIO_SetBits(GPIOD,GPIO_Pin_1)

#define CH3_Check_Set		GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define CH3_Check_Reset		GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define CH3_Error_Set		GPIO_ResetBits(GPIOD,GPIO_Pin_3)
#define CH3_Error_Reset		GPIO_SetBits(GPIOD,GPIO_Pin_3)

#define CH4_Check_Set		GPIO_ResetBits(GPIOD,GPIO_Pin_4)
#define CH4_Check_Reset		GPIO_SetBits(GPIOD,GPIO_Pin_4)
#define CH4_Error_Set		GPIO_ResetBits(GPIOD,GPIO_Pin_5)
#define CH4_Error_Reset		GPIO_SetBits(GPIOD,GPIO_Pin_5)

#define CH5_Check_Set		GPIO_ResetBits(GPIOD,GPIO_Pin_6)
#define CH5_Check_Reset		GPIO_SetBits(GPIOD,GPIO_Pin_6)
#define CH5_Error_Set		GPIO_ResetBits(GPIOD,GPIO_Pin_7)
#define CH5_Error_Reset		GPIO_SetBits(GPIOD,GPIO_Pin_7)

#define CH6_Check_Set		GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define CH6_Check_Reset		GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define CH6_Error_Set		GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define CH6_Error_Reset		GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define CH7_Check_Set		GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define CH7_Check_Reset		GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define CH7_Error_Set		GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define CH7_Error_Reset		GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define CH8_Check_Set		GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define CH8_Check_Reset		GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define CH8_Error_Set		GPIO_ResetBits(GPIOE,GPIO_Pin_0)
#define CH8_Error_Reset		GPIO_SetBits(GPIOE,GPIO_Pin_0)

void LED_Init(void);
void COIL_LED_GPIO_Init(void);

void COIL_LED_OFF(void);
void COIL_LED_ON(void);
void LED_ALL_ON(void);

#endif

