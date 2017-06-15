#ifndef __BSP_H
#define __BSP_H 			   

#include "sys.h"
#include "global.h"

void SSU_CanTest(void);
void SSU_ACcheckTest(void);
void SSU_DCcheckTest(void);
void SSU_VoltTest(void);
void SSU_DS18B20Test(void);
void SSU_RelayTest(void);
void PDU_TestEnd(void);
void SSU_FunCan_Respone_Deal(Frame_DefType * data);
uint8_t DataCheckSum(uint8_t *data,uint8_t len);

#endif

