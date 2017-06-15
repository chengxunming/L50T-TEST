#ifndef __BSP_LED_H
#define __BSP_LED_H 			   

#include "sys.h"

//LED¿ØÖÆ
#define RUNLEDON		GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define RUNLEDOFF		GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define FAULTLEDON	GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define FAULTLEDOFF	GPIO_SetBits(GPIOA,GPIO_Pin_2)

//¼ÌµçÆ÷¿ØÖÆ
#define DIMRelayON		GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define DIMRelayOFF		GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define PDURelayON		GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define PDURelayOFF		GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define MFURelayON		GPIO_ResetBits(GPIOC,GPIO_Pin_9)  
#define MFURelayOFF		GPIO_SetBits(GPIOC,GPIO_Pin_9)   //MFU WORK


void LED_Init(void);
void LED_AllOn(void);
void RELAY_GPIO_Init(void);

#endif

