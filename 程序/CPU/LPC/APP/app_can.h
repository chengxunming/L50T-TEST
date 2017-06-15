#ifndef __APP_CAN_H
#define __APP_CAN_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_include.h"

void CAN1_Init(void);
void CAN2_Init(void);
void CAN_RX(uint32_t ctrl);

void CAN2_TEST(void);

#endif

