#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H 			   

#include "sys.h"

//IO��������
#define DS18B20_IO_IN()  {GPIOC->CRL&=0x0FFFFFFF;GPIOC->CRL|=(uint32_t)8<<28UL;}
#define DS18B20_IO_OUT() {GPIOC->CRL&=0x0FFFFFFF;GPIOC->CRL|=(uint32_t)3<<28UL;}
////IO��������											   
#define	DS18B20_DQ_OUT PCout(7) //���ݶ˿�	PC7 
#define	DS18B20_DQ_IN  PCin(7)  //���ݶ˿�	PC7 

uint8_t DS18B20_Init(void);
short DS18B20_Get_Temp(void);

#endif

