#ifndef __BSP_ADC_H
#define __BSP_ADC_H 			   

#include "sys.h"

void ADC1_Config(void);
void Get_Adc(uint8_t num);
void Get_VoltRMS(uint8_t num);
void Cal_Average(void);
uint8_t Get_AC_Average(void);
uint8_t Get_DC_Average(void);

#endif

