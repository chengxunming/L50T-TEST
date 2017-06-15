#ifndef __BSP_H
#define __BSP_H 			   

#include "sys.h"
#include "global.h"

void DET_CanTest(void);
void DET_ChannelTest(void);
void DET_TestEnd(void);
void DET_FunCan_Respone_Deal(Frame_DefType * data);
uint8_t DataCheckSum(uint8_t *data,uint8_t len);

#endif

