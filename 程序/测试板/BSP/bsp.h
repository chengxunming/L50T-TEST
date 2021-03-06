#ifndef __BSP_H
#define __BSP_H 			   

#include "sys.h"
#include "global.h"
#include "delay.h"

void CAN_Respone_SendCheck(void);
void MFU_TestCheck(void);
void NET_UDP_SocketConfig(void);
void NET_UDP_LoopBack(void);
void TEST_Finish_Check(void);
void CAN_RxMessage_Deal(Frame_DefType * FrameRx);
uint8_t DataCheckSum(uint8_t *data,uint8_t len);
void SYS_RESET(void);

#endif

