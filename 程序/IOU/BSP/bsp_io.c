#include "bsp_io.h"

uint8_t CH_IN[8];
void IO_InCheck_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC ,  ENABLE );
		  /*************************************************
	         ���� ͨ������״̬�����������(CHx-SV)
  ****************************************************/		
	//ͨ������״̬�����������V1.1	
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
* ��    �ƣ� GetChannelStatus
* ��    �ܣ� ��ȡ���ӿ�״̬��Ϣ
* ��ڲ����� ��
* ���ڲ����� ��
*******************************************************************/
void GetChannelStatus(void)
{
	CH_IN[0] = !CH1CHECK;//���ͨ��1Ϊ�г�������������ڣ�0Ϊ�޳��������������
	CH_IN[1] = !CH2CHECK;
	CH_IN[2] = !CH3CHECK;
	CH_IN[3] = !CH4CHECK;
	CH_IN[4] = !CH5CHECK;
	CH_IN[5] = !CH6CHECK;
	CH_IN[6] = !CH7CHECK;
	CH_IN[7] = !CH8CHECK;
}

