#include "bsp_io.h"

uint8_t CH_IN[8];
void IO_InCheck_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC ,  ENABLE );
		  /*************************************************
	         设置 通道输入状态检测下拉输入(CHx-SV)
  ****************************************************/		
	//通道输入状态检测下拉输入V1.1	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
		
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
}

/*******************************************************************
* 名    称： GetChannelStatus
* 功    能： 获取监测接口状态信息
* 入口参数： 无
* 出口参数： 无
*******************************************************************/
void GetChannelStatus(void)
{
	CH_IN[0] = !CH1CHECK;//检测通道1为有车或行人请求存在，0为无车或行人请求存在
	CH_IN[1] = !CH2CHECK;
	CH_IN[2] = !CH3CHECK;
	CH_IN[3] = !CH4CHECK;
	CH_IN[4] = !CH5CHECK;
	CH_IN[5] = !CH6CHECK;
	CH_IN[6] = !CH7CHECK;
	CH_IN[7] = !CH8CHECK;
}

