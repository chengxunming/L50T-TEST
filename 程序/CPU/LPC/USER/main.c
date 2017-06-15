#include "bsp_include.h" 
#include "app_can.h"

unsigned int val_Tx = 0, val_Rx = 0;            /* Globals used for display */

char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//uint8_t str_data0[]="This is COM1[UART0] send test!\r\n";
//uint8_t str_data2[]="This is COM2[UART2] send test!\r\n";
/*----------------------------------------------------------------------------
  insert a delay time.
 *----------------------------------------------------------------------------*/
void delay(unsigned int nCount)	{

  for(; nCount != 0; nCount--);
}


/*------------------------------------------------------------------------------
  convert one byte to string in hexadecimal notation
 *------------------------------------------------------------------------------*/
void Hex_Str (unsigned char hex, char *str) {
  *str++ = '0';
  *str++ = 'x';
  *str++ = hex_chars[hex >>  4];
  *str++ = hex_chars[hex & 0xF];
}



/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)  {

  SystemInit();                                   /* initialize MCU clocks 100MHz*/
//  adc_Init ();                                    /* initialise A/D converter */
  LED_init ();                                    /* Initialize the LEDs */
//  can_Init ();                                    /* initialise CAN interface */
delay_init();
//	UART0_Init();
//	Uart_Init(COM1,115200);
	Uart_Init(COM2,115200);
	CAN1_Init();
	CAN2_Init();
	
//	CAN2_TEST();

  while (1) 
  {
		CAN_RX(CAN2);
  }
}
