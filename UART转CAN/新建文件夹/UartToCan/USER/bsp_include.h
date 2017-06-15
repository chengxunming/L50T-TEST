#ifndef __BSP_INCLUDE_H
#define __BSP_INCLUDE_H

/* Includes ------------------------------------------------------------------*/
//lib
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//设备相关
#include "lpc17xx.h"                              /* LPC17xx definitions    */

#include "sys.h"

#include "CAN.h"                                /* LPC1766 CAN adaption layer */
#include "GLCD.h"                               /* GLCD function prototypes */
#include "LED.h"                                /* LED function prototypes */
#include "ADC.h"                                /* ADC function prototypes */
#include "uart.h" 

#endif

