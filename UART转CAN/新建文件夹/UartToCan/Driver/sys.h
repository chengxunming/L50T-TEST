#ifndef __SYS_H
#define __SYS_H	

#include "lpc17xx.h" 	 																	   

#define NVIC_PriorityGroup_0	0
#define NVIC_PriorityGroup_1	1
#define NVIC_PriorityGroup_2    2
#define NVIC_PriorityGroup_3	3
#define NVIC_PriorityGroup_4	4
#define NVIC_PriorityGroup_5	5
#define NVIC_PriorityGroup_6	6
#define NVIC_PriorityGroup_7	7

void NVIC_GroupingConfiguration(void);



#endif

