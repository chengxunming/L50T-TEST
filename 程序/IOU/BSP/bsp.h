#ifndef __BSP_H
#define __BSP_H 			   

#include "sys.h"
#include "global.h"

void IOU_CanTest(void);
void IOU_ChannelTest(void);
void IOU_TestEnd(void);
void IOU_FunCan_Respone_Deal(Frame_DefType * data);
uint8_t DataCheckSum(uint8_t *data,uint8_t len);

#endif

