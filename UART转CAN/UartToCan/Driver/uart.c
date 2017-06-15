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
** Descriptions:        编写示例代码
**
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/
#include "lpc17xx.h"                              /* LPC17xx definitions    */
#include "uart.h"
#include "sys.h"

#define FOSC                        12000000                            /*  振荡器频率                  */

#define FCCLK                      (FOSC  * 8)                          /*  主时钟频率<=100Mhz          */
                                                                        /*  FOSC的整数倍                */
#define FCCO                       (FCCLK * 3)                          /*  PLL频率(275Mhz~550Mhz)      */
                                                                        /*  与FCCLK相同，或是其的偶数倍 */
#define FPCLK                      (FCCLK / 4)                          /*  外设时钟频率,FCCLK的1/2、1/4*/
                                                                        /*  或与FCCLK相同               */

#define UART0_BPS     115200                                             /* 串口0通信波特率             */
#define UART2_BPS     115200                                             /* 串口2通信波特率             */


volatile uint8_t UART0TxEmpty = 1;
volatile uint8_t UART2TxEmpty = 1;
	

/*********************************************************************************************************
** Function name:       UART0_Init
** Descriptions:        按默认值初始化串口0的引脚和通讯参数。设置为8位数据位，1位停止位，无奇偶校验
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART0_Init (void)
{
	uint16_t usFdiv;
	
    /* UART0 */
    LPC_PINCON->PINSEL0 |= (1 << 4);             /* Pin P0.2 used as TXD0 (Com0) */
    LPC_PINCON->PINSEL0 |= (1 << 6);             /* Pin P0.3 used as RXD0 (Com0) */
  
  	LPC_UART0->LCR  = 0x83;                      /* 允许设置波特率               */
    usFdiv = (FPCLK / 16) / UART0_BPS;           /* 设置波特率                   */
    LPC_UART0->DLM  = usFdiv / 256;
    LPC_UART0->DLL  = usFdiv % 256; 
    LPC_UART0->LCR  = 0x03;                      /* 锁定波特率                   */
    LPC_UART0->FCR  = 0x06; 				   
}
/*********************************************************************************************************
** 函数名称：UARTInit
** 函数描述：串口初始化
** 输入参数：UARTx    	UART端口号
**           bandrate   设定的串口波特率
** 返回值  ：无
*********************************************************************************************************/
void Uart_Init(LPC_UART_TypeDef* UARTx,uint32_t baudrate)
{
	uint32_t usFdiv;
	
	if(UARTx==LPC_UART0)
	{
		LPC_SC->PCONP |= (1<<3);                	 //打开UART0功率、时钟控制位
		
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
		LPC_SC->PCONP |= (1<<24);                	 //打开UART2功率、时钟控制位  PCUART2

		LPC_PINCON->PINSEL0 &= ~(0x03<<20);
		LPC_PINCON->PINSEL0 |=  (0x01<<20);             /* Pin P0.10 used as TXD2 (Com2) */
		LPC_PINCON->PINSEL0 &= ~(0x03<<22);
		LPC_PINCON->PINSEL0 |=  (0x01<<22);             /* Pin P0.11 used as RXD2 (Com2) */
		
		NVIC_ClearPendingIRQ(UART2_IRQn);
		NVIC_SetPriority(UART2_IRQn,NVIC_EncodePriority(NVIC_PriorityGroup_3_8_4,0,2));
		NVIC_EnableIRQ(UART2_IRQn);
	}
    /*
     * 8为数据位，无奇偶检验位，1位停止位
     */
	UARTx->LCR  = 0x83;                      /* 允许设置波特率               */
	usFdiv = (SystemCoreClock/4/16) / baudrate;        /* 设置波特率                   */
	UARTx->DLM  = usFdiv / 256;
	UARTx->DLL  = usFdiv % 256; 
	UARTx->LCR  = 0x03;                      /* 锁定波特率                   */
	UARTx->FCR  = 0x07;						 /* 使能并复位FIFO               */	

    UARTx->IER = 0x01;                       //使能接收中断
	/* 
     * 使能接收FIFO不为空中断、接收线状态中断
     */
    UARTx->IER = IER_RBR | IER_RLS;
	
}
/*********************************************************************************************************
** 函数名称: Uart_SendByte
** 函数描述: 从串口发送数据
** 输入参数: UARTx	UART端口号
**			ucData	发送的数据
** 返回值  : 无
*********************************************************************************************************/
void Uart_SendByte(LPC_UART_TypeDef* UARTx,uint8_t ucData)
{
	while (!(UARTx->LSR & 0x20));
	UARTx->THR = ucData;
}
/*********************************************************************************************************
** 函数名称：UARTSend
** 函数描述：串口发送多个数据
** 输入参数：UARTx       UART端口号
**           BufferPtr  发送数据缓冲区
**           Length     发送数据长度
** 返回值  ：无
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
** Descriptions:        从串口0发送数据
** input parameters:    data: 发送的数据
** output parameters:   无
** Returned value:      无
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
** Descriptions:	    向串口发送字符串
** input parameters:    s:   要发送的字符串指针
** output parameters:   无
** Returned value:      无
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
** Descriptions:        按默认值初始化串口2的引脚和通讯参数。设置为8位数据位，1位停止位，无奇偶校验
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART2_Init (void)
{
	uint16_t usFdiv;
    /* UART2 */
    LPC_PINCON->PINSEL0 |= (1 << 20);             /* Pin P0.10 used as TXD2 (Com2) */
    LPC_PINCON->PINSEL0 |= (1 << 22);             /* Pin P0.11 used as RXD2 (Com2) */

   	LPC_SC->PCONP = LPC_SC->PCONP|(1<<24);	      /*打开UART2电源控制位	           */

    LPC_UART2->LCR  = 0x83;                       /* 允许设置波特率                */
    usFdiv = (FPCLK / 16) / UART2_BPS;            /* 设置波特率                    */
    LPC_UART2->DLM  = usFdiv / 256;
    LPC_UART2->DLL  = usFdiv % 256; 
    LPC_UART2->LCR  = 0x03;                       /* 锁定波特率                    */
    LPC_UART2->FCR  = 0x06;
}

/*********************************************************************************************************
** Function name:       UART2_SendByte
** Descriptions:        从串口2发送数据
** input parameters:    data: 发送的数据
** output parameters:   无
** Returned value:      无
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
** Descriptions:	    向串口发送字符串
** input parameters:    s:   要发送的字符串指针
** output parameters:   无
** Returned value:      无
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
