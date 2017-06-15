#ifndef __BSP_H
#define __BSP_H 			   

#include "sys.h"
#include "global.h"

void PDU_CanTest(void);
void PDU_ChannelTest(void);
void PDU_TestEnd(void);
void PDU_FunCan_Respone_Deal(Frame_DefType * data);
uint8_t DataCheckSum(uint8_t *data,uint8_t len);

#endif

