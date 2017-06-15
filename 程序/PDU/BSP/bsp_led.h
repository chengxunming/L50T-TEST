#ifndef __BSP_LED_H
#define __BSP_LED_H 			   

#include "sys.h"

//LED¿ØÖÆ
#define RUNLEDON		GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define RUNLEDOFF		GPIO_SetBits(GPIOC,GPIO_Pin_2)


void LED_Init(void);


#endif

