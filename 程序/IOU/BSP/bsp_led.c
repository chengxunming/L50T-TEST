#include "bsp_led.h"

//LED引脚初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE );
                              
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_10);//关灯
}
//IO输出通道引脚初始化
void IO_Channel_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,  ENABLE );
	/*************************************************
	      设置 通道输出状态指示灯控制输出(OUTx)
  ****************************************************/	
	//通道输出状态指示灯控制输入V1.1
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_12|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);	//置1,默认上拉输出
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_9);
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_12|GPIO_Pin_14);
	
	  /*************************************************
	       设置 通道输入状态指示灯控制输出(INPUTx)
  ****************************************************/		
	//通道输入状态指示灯控制输入v1.1
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_8);	//置1,默认上拉输出
	GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
}

void IO_OUT_ALL_ON(void)
{
	CH1_outON;
	CH2_outON;
	CH3_outON;
	CH4_outON;
	CH5_outON;
	CH6_outON;
	CH7_outON;
	CH8_outON;
}

void IO_OUT_ALL_OFF(void)
{
	CH1_outOFF;
	CH2_outOFF;
	CH3_outOFF;
	CH4_outOFF;
	CH5_outOFF;
	CH6_outOFF;
	CH7_outOFF;
	CH8_outOFF;
}

void IO_InLed_ALL_ON(void)
{
	CH1_inON;
	CH2_inON;
	CH3_inON;
	CH4_inON;
	CH5_inON;
	CH6_inON;
	CH7_inON;
	CH8_inON;
}

void IO_InLed_ALL_OFF(void)
{
	CH1_inOFF;
	CH2_inOFF;
	CH3_inOFF;
	CH4_inOFF;
	CH5_inOFF;
	CH6_inOFF;
	CH7_inOFF;
	CH8_inOFF;
}

void LED_ALL_ON(void)
{
	RUNLEDON;
	IO_OUT_ALL_ON();
	IO_InLed_ALL_ON();
}

