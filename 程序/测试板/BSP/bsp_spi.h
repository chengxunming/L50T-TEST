#ifndef __BSP_SPI_H
#define __BSP_SPI_H 			   

#include "sys.h"
#include "w5500.h"

#define SPI2_CLK_ENABLE()			RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE )//SPI2时钟使能 

#define SPI2_MOSI_PIN	GPIO_Pin_15
#define SPI2_MISO_PIN	GPIO_Pin_14
#define SPI2_CLK_PIN	GPIO_Pin_13
#define SPI2_CS_PIN		GPIO_Pin_12
#define	SPI2_GPIO		GPIOB
#define SPI2_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE )//PORTB时钟使能 

void SPI2_Init(void);
void SPI2_WriteByte(uint8_t TxData);
uint8_t SPI2_ReadByte(void);
void SPI2_CS_Select(void);
void SPI2_CS_Deselect(void);
void SPI_CrisEnter(void);
void SPI_CrisExit(void);

void W5500_SPI_Config(void);

#endif


