#ifndef __BSP_LED_H
#define __BSP_LED_H 			   

#include "sys.h"
#include <stdbool.h>

//故障指示灯
#define Led1_ON		GPIO_ResetBits(GPIOA,GPIO_Pin_9)
#define Led1_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_9)

#define Led2_ON		GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define Led2_OFF	GPIO_SetBits(GPIOC,GPIO_Pin_9)

#define Led3_ON		GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define Led3_OFF	GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define Led4_ON		GPIO_ResetBits(GPIOC,GPIO_Pin_7)
#define Led4_OFF	GPIO_SetBits(GPIOC,GPIO_Pin_7)

#define Led5_ON		GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define Led5_OFF	GPIO_SetBits(GPIOC,GPIO_Pin_6)

#define Led6_ON		GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define Led6_OFF	GPIO_SetBits(GPIOB,GPIO_Pin_11)

#define Led7_ON		GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define Led7_OFF	GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define Led8_ON		GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define Led8_OFF	GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define Led9_ON		GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define Led9_OFF	GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define Led10_ON	GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#define Led10_OFF	GPIO_SetBits(GPIOC,GPIO_Pin_5)

//单板测试指示灯
#define LedCPU_ON	GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define LedCPU_OFF	GPIO_SetBits(GPIOC,GPIO_Pin_4)

#define LedCOU_ON	GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LedCOU_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define LedSSU_ON	GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define LedSSU_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define LedPDU_ON	GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define LedPDU_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LedDET_ON	GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define LedDET_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define LedIOU_ON	GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define LedIOU_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define LedMAU_ON	GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define LedMAU_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_2)

#define LedMFU_ON	GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define LedMFU_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_1)
//测试板运行指示灯
#define LedRun_ON	GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define LedRun_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_0)
//测试完成指示灯
#define LedOK_ON	GPIO_ResetBits(GPIOA,GPIO_Pin_10)
#define LedOK_OFF	GPIO_SetBits(GPIOA,GPIO_Pin_10)

//蜂鸣器控制
#define  BUZZER_ON           GPIO_SetBits(GPIOC,GPIO_Pin_1)//蜂鸣器打开
#define  BUZZER_OFF          GPIO_ResetBits(GPIOC,GPIO_Pin_1)//蜂鸣器关闭

//按键对象结构体
typedef struct
{
	uint16_t 	RingCount;		//响铃计时
	bool 		IsRing;			//是否打开蜂鸣器
}BuzzerHandle_DefType;

void LED_Init(void);
void BUZZER_Init(void);
void BUZZER_Ring(void);

#endif

