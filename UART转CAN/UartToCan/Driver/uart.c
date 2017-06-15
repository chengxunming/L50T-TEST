/****************************************Copyright (c)****************************************************
**                                 http://www.PowerAVR.com
**								   http://www.PowerMCU.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           uart.c
** Last modified Date:  2010-05-12
** Last Version:        V1.00
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          PowerAVR
** Created date:        2010-05-10
** Version:             V1.00
** Descriptions:        ��дʾ������
**
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/
#include "lpc17xx.h"                              /* LPC17xx definitions    */
#include "uart.h"
#include "sys.h"

#define FOSC                        12000000                            /*  ����Ƶ��                  */

#define FCCLK                      (FOSC  * 8)                          /*  ��ʱ��Ƶ��<=100Mhz          */
                                                                        /*  FOSC��������                */
#define FCCO                       (FCCLK * 3)                          /*  PLLƵ��(275Mhz~550Mhz)      */
                                                                        /*  ��FCCLK��ͬ���������ż���� */
#define FPCLK                      (FCCLK / 4)                          /*  ����ʱ��Ƶ��,FCCLK��1/2��1/4*/
                                                                        /*  ����FCCLK��ͬ               */

#define UART0_BPS     115200                                             /* ����0ͨ�Ų�����             */
#define UART2_BPS     115200                                             /* ����2ͨ�Ų�����             */


volatile uint8_t UART0TxEmpty = 1;
volatile uint8_t UART2TxEmpty = 1;
	

/*********************************************************************************************************
** Function name:       UART0_Init
** Descriptions:        ��Ĭ��ֵ��ʼ������0�����ź�ͨѶ����������Ϊ8λ����λ��1λֹͣλ������żУ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART0_Init (void)
{
	uint16_t usFdiv;
	
    /* UART0 */
    LPC_PINCON->PINSEL0 |= (1 << 4);             /* Pin P0.2 used as TXD0 (Com0) */
    LPC_PINCON->PINSEL0 |= (1 << 6);             /* Pin P0.3 used as RXD0 (Com0) */
  
  	LPC_UART0->LCR  = 0x83;                      /* �������ò�����               */
    usFdiv = (FPCLK / 16) / UART0_BPS;           /* ���ò�����                   */
    LPC_UART0->DLM  = usFdiv / 256;
    LPC_UART0->DLL  = usFdiv % 256; 
    LPC_UART0->LCR  = 0x03;                      /* ����������                   */
    LPC_UART0->FCR  = 0x06; 				   
}
/*********************************************************************************************************
** �������ƣ�UARTInit
** �������������ڳ�ʼ��
** ���������UARTx    	UART�˿ں�
**           bandrate   �趨�Ĵ��ڲ�����
** ����ֵ  ����
*********************************************************************************************************/
void Uart_Init(LPC_UART_TypeDef* UARTx,uint32_t baudrate)
{
	uint32_t usFdiv;
	
	if(UARTx==LPC_UART0)
	{
		LPC_SC->PCONP |= (1<<3);                	 //��UART0���ʡ�ʱ�ӿ���λ
		
		LPC_PINCON->PINSEL0 &= ~(0x03<<4);
		LPC_PINCON->PINSEL0 |=  (0x01<<4);       /* Pin P0.2 used as TXD0 (Com0) */
		LPC_PINCON->PINSEL0 &= ~(0x03<<6);
		LPC_PINCON->PINSEL0 |=  (0x01<<6);       /* Pin P0.3 used as RXD0 (Com0) */	

		NVIC_ClearPendingIRQ(UART0_IRQn);
		NVIC_SetPriority(UART0_IRQn,NVIC_EncodePriority(NVIC_PriorityGroup_3_8_4,0,1));
		NVIC_EnableIRQ(UART0_IRQn); 
	}
	else if(UARTx==LPC_UART2)
	{
		LPC_SC->PCONP |= (1<<24);                	 //��UART2���ʡ�ʱ�ӿ���λ  PCUART2

		LPC_PINCON->PINSEL0 &= ~(0x03<<20);
		LPC_PINCON->PINSEL0 |=  (0x01<<20);             /* Pin P0.10 used as TXD2 (Com2) */
		LPC_PINCON->PINSEL0 &= ~(0x03<<22);
		LPC_PINCON->PINSEL0 |=  (0x01<<22);             /* Pin P0.11 used as RXD2 (Com2) */
		
		NVIC_ClearPendingIRQ(UART2_IRQn);
		NVIC_SetPriority(UART2_IRQn,NVIC_EncodePriority(NVIC_PriorityGroup_3_8_4,0,2));
		NVIC_EnableIRQ(UART2_IRQn);
	}
    /*
     * 8Ϊ����λ������ż����λ��1λֹͣλ
     */
	UARTx->LCR  = 0x83;                      /* �������ò�����               */
	usFdiv = (SystemCoreClock/4/16) / baudrate;        /* ���ò�����                   */
	UARTx->DLM  = usFdiv / 256;
	UARTx->DLL  = usFdiv % 256; 
	UARTx->LCR  = 0x03;                      /* ����������                   */
	UARTx->FCR  = 0x07;						 /* ʹ�ܲ���λFIFO               */	

    UARTx->IER = 0x01;                       //ʹ�ܽ����ж�
	/* 
     * ʹ�ܽ���FIFO��Ϊ���жϡ�������״̬�ж�
     */
    UARTx->IER = IER_RBR | IER_RLS;
	
}
/*********************************************************************************************************
** ��������: Uart_SendByte
** ��������: �Ӵ��ڷ�������
** �������: UARTx	UART�˿ں�
**			ucData	���͵�����
** ����ֵ  : ��
*********************************************************************************************************/
void Uart_SendByte(LPC_UART_TypeDef* UARTx,uint8_t ucData)
{
	while (!(UARTx->LSR & 0x20));
	UARTx->THR = ucData;
}
/*********************************************************************************************************
** �������ƣ�UARTSend
** �������������ڷ��Ͷ������
** ���������UARTx       UART�˿ں�
**           BufferPtr  �������ݻ�����
**           Length     �������ݳ���
** ����ֵ  ����
*********************************************************************************************************/
void Uart_SendMultByte( LPC_UART_TypeDef* UARTx, uint8_t *BufferPtr, uint32_t Length )
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		while (!(UARTx->LSR & 0x20));
		UARTx->THR = *BufferPtr++;
	}
}


/*********************************************************************************************************
** Function name:       UART0_SendByte
** Descriptions:        �Ӵ���0��������
** input parameters:    data: ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int UART0_SendByte (int ucData)
{
	while (!(LPC_UART0->LSR & 0x20));
    return (LPC_UART0->THR = ucData);
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int UART0_GetChar (void) 
{
  	while (!(LPC_UART0->LSR & 0x01));
  	return (LPC_UART0->RBR);
}

/*********************************************************************************************************
Write character to Serial Port
** Function name:       UART0_SendString
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    s:   Ҫ���͵��ַ���ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART0_SendString (unsigned char *s) 
{
  	while (*s != 0) 
	{
   		UART0_SendByte(*s++);
	}
}

/*********************************************************************************************************
** Function name:       UART2_Init
** Descriptions:        ��Ĭ��ֵ��ʼ������2�����ź�ͨѶ����������Ϊ8λ����λ��1λֹͣλ������żУ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART2_Init (void)
{
	uint16_t usFdiv;
    /* UART2 */
    LPC_PINCON->PINSEL0 |= (1 << 20);             /* Pin P0.10 used as TXD2 (Com2) */
    LPC_PINCON->PINSEL0 |= (1 << 22);             /* Pin P0.11 used as RXD2 (Com2) */

   	LPC_SC->PCONP = LPC_SC->PCONP|(1<<24);	      /*��UART2��Դ����λ	           */

    LPC_UART2->LCR  = 0x83;                       /* �������ò�����                */
    usFdiv = (FPCLK / 16) / UART2_BPS;            /* ���ò�����                    */
    LPC_UART2->DLM  = usFdiv / 256;
    LPC_UART2->DLL  = usFdiv % 256; 
    LPC_UART2->LCR  = 0x03;                       /* ����������                    */
    LPC_UART2->FCR  = 0x06;
}

/*********************************************************************************************************
** Function name:       UART2_SendByte
** Descriptions:        �Ӵ���2��������
** input parameters:    data: ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int UART2_SendByte (int ucData)
{
	while (!(LPC_UART2->LSR & 0x20));
    return (LPC_UART2->THR = ucData);
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int UART2_GetChar (void) 
{
  	while (!(LPC_UART2->LSR & 0x01));
  	return (LPC_UART2->RBR);
}

/*********************************************************************************************************
** Write character to Serial Port
** Function name:       UART2_SendString
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    s:   Ҫ���͵��ַ���ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART2_SendString (unsigned char *s) 
{
  	while (*s != 0) 
	{
   		UART2_SendByte(*s++);
	}
}

void UART0_SendChar(uint16_t disp)
{
	uint16_t dispbuf[4];
	uint8_t i;

	dispbuf[3] = disp%10 + '0';
	dispbuf[2] = disp/10%10 + '0';
	dispbuf[1] = disp/10/10%10 + '0';
	dispbuf[0] = disp/10/10/10%10 + '0';
	for(i=0;i<4;i++)
		UART0_SendByte(dispbuf[i]);	
}
