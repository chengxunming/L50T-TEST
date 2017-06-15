/****************************************Copyright (c)****************************************************
**                                 http://www.PowerAVR.com
**								   http://www.PowerMCU.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           uart.h
** Last modified Date:  2010-05-12
** Last Version:        V1.00
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          PowerAVR
** Created date:        2010-05-10
** Version:             V1.00
** Descriptions:        ±àÐ´Ê¾Àý´úÂë
**
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/
#ifndef __UART_H
#define __UART_H

#include "sys.h"

#define COM1	LPC_UART0
#define COM2	LPC_UART2

#define IER_RBR     0x01
#define IER_THRE    0x02
#define IER_RLS     0x04

void UART0_Init (void);
int  UART0_SendByte (int ucData);
int  UART0_GetChar (void);
void UART0_SendString (unsigned char *s); 
void UART2_Init (void);
int  UART2_SendByte (int ucData);
int  UART2_GetChar (void);
void UART2_SendString (unsigned char *s);
void UART0_SendChar(uint16_t disp);

void Uart_Init(LPC_UART_TypeDef* UARTx,uint32_t baudrate);
void Uart_SendByte(LPC_UART_TypeDef* UARTx,uint8_t ucData);
void Uart_SendMultByte( LPC_UART_TypeDef* UARTx, uint8_t *BufferPtr, uint32_t Length );

#endif
