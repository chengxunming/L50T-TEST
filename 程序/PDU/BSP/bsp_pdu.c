#include "bsp_pdu.h"
#include "global.h"

uint8_t ChannelVoltage[12];				//12个通道的电压状态
//PDU通道控制检测引脚初始化
void PDU_ChannelGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,  ENABLE );
	
	/*设置 通道控制输出*/
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	ChannelAlloff();
	
	/*设置 通道状态检测输入*/
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}
//关闭所有外灯
void ChannelAlloff(void)
{
	CH1ROFF;CH1YOFF;CH1GOFF;
	CH2ROFF;CH2YOFF;CH2GOFF;
	CH3ROFF;CH3YOFF;CH3GOFF;
	CH4ROFF;CH4YOFF;CH4GOFF;
}
//打开所有外灯
void ChannelAllon(void)
{
	CH1RON;CH1YON;CH1GON;
	CH2RON;CH2YON;CH2GON;
	CH3RON;CH3YON;CH3GON;
	CH4RON;CH4YON;CH4GON;
}

//获取外灯电压状态
void GetChannelVoltageStatus(void)
{
	ChannelVoltage[0] |= !CH1RCHECK;
	ChannelVoltage[1] |= !CH1YCHECK;
	ChannelVoltage[2] |= !CH1GCHECK;
	ChannelVoltage[3] |= !CH2RCHECK;
	ChannelVoltage[4] |= !CH2YCHECK;
	ChannelVoltage[5] |= !CH2GCHECK;
	ChannelVoltage[6] |= !CH3RCHECK;
	ChannelVoltage[7] |= !CH3YCHECK;
	ChannelVoltage[8] |= !CH3GCHECK;
	ChannelVoltage[9] |= !CH4RCHECK;
	ChannelVoltage[10] |= !CH4YCHECK;
	ChannelVoltage[11] |= !CH4GCHECK;
}

