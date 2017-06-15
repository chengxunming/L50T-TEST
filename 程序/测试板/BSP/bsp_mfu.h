#ifndef __BSP_MFU_H
#define __BSP_MFU_H 			   

#include "sys.h"
#include "global.h"

#define MFU_CONTROL_PIN		GPIO_Pin_8
#define MFU_CHECK_PIN		GPIO_Pin_9
#define	MFU_GPIO			GPIOB
#define MFU_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE )//PORTB时钟使能

//黄闪使能控制
#define MFU_ENABLE		GPIO_SetBits(MFU_GPIO,MFU_CONTROL_PIN)//黄闪模式，MFU工作
#define MFU_DISABLE		GPIO_ResetBits(MFU_GPIO,MFU_CONTROL_PIN)//正常模式
//黄闪状态检测
#define MFUSTATUSCHECK		GPIO_ReadInputDataBit(MFU_GPIO,MFU_CHECK_PIN)

void MFU_GPIO_Init(void);
void MFU_Test_Start(void);
void MFU_CheckStatus(void);
	
#endif

