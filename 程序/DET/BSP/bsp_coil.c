#include "bsp_coil.h"

Loop_DefType CH1,CH2,CH3,CH4,CH5,CH6,CH7,CH8;
static uint8_t Group1active,Group2active;											//线圈组当前激活的线圈
static uint16_t	NormalworkCnt = 0;
static uint8_t Channel_number = 1;

void COIL_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,  ENABLE );
	/*设置 线圈使能控制输出*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	All_Channel_OFF();
	
	//频率检测
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;		//FREQ1
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;		//FREQ2
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
//	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;		//TEST KEY
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//外部中断初始化
void Exti_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//TEST KEY
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
}
//关闭所有通道
void All_Channel_OFF(void)
{
	CH1_Disable;CH2_Disable;CH3_Disable;CH4_Disable;
	CH5_Disable;CH6_Disable;CH7_Disable;CH8_Disable;
}
//通道控制函数
void Channel_enable(uint8_t channel)
{
	switch (channel)
	{
		case 1:
			CH1_Enable;CH2_Disable;CH3_Disable;CH4_Disable;
			CH5_Enable;CH6_Disable;CH7_Disable;CH8_Disable;
			CH1.FREQCnt = 0;
			CH5.FREQCnt = 0;
			Group1active = 1;
			Group2active = 1;
			
		break;
		case 2:
			CH1_Disable;CH2_Enable;CH3_Disable;CH4_Disable;
			CH5_Disable;CH6_Enable;CH7_Disable;CH8_Disable;
			CH2.FREQCnt = 0;
			CH6.FREQCnt = 0;
			Group1active = 2;
			Group2active = 2;
		break;
		case 3:
			CH1_Disable;CH2_Disable;CH3_Enable;CH4_Disable;
			CH5_Disable;CH6_Disable;CH7_Enable;CH8_Disable;
			CH3.FREQCnt = 0;
			CH7.FREQCnt = 0;
			Group1active = 3;
			Group2active = 3;
		break;
		case 4:
			CH1_Disable;CH2_Disable;CH3_Disable;CH4_Enable;
			CH5_Disable;CH6_Disable;CH7_Disable;CH8_Enable;
			CH4.FREQCnt = 0;
			CH8.FREQCnt = 0;
			Group1active = 4;
			Group2active = 4;
		break;
		default:
		break;
	}
}
//线圈电路状态检测
void COIL_Check(void)
{
	if(CH1.Frequnce>30000)CH1.state=true;
	else CH1.state=false;
	
	if(CH2.Frequnce>30000)CH2.state=true;
	else CH2.state=false;
	
	if(CH3.Frequnce>30000)CH3.state=true;
	else CH3.state=false;
	
	if(CH4.Frequnce>30000)CH4.state=true;
	else CH4.state=false;
	
	if(CH5.Frequnce>30000)CH5.state=true;
	else CH5.state=false;
	
	if(CH6.Frequnce>30000)CH6.state=true;
	else CH6.state=false;
	
	if(CH7.Frequnce>30000)CH7.state=true;
	else CH7.state=false;
	
	if(CH8.Frequnce>30000)CH8.state=true;
	else CH8.state=false;
}
//线圈检测
void COIL_Detect(void)
{
	if(NormalworkCnt==0)
	{
		Channel_number = 1;
		Channel_enable(Channel_number);
	}
	else if(NormalworkCnt==1)
	{	
		CH1.Frequnce = CH1.FREQCnt*40;
		CH5.Frequnce = CH5.FREQCnt*40;
		Channel_number = 2; 
	}
	else if(NormalworkCnt==2)
	{
		CH2.Frequnce = CH2.FREQCnt*40;
		CH6.Frequnce = CH6.FREQCnt*40;
		Channel_number = 3; 
	}
	else if(NormalworkCnt==3)
	{
		CH3.Frequnce = CH3.FREQCnt*40;
		CH7.Frequnce = CH7.FREQCnt*40;
		Channel_number = 4; 
	}
	else if(NormalworkCnt==4)
	{
		CH4.Frequnce = CH4.FREQCnt*40;
		CH8.Frequnce = CH8.FREQCnt*40;
		Channel_number = 1; 
	}
	Channel_enable(Channel_number);
	
	NormalworkCnt++;
	
	if(NormalworkCnt>=5)
	{
		NormalworkCnt=0;
		All_Channel_OFF();
		state_Coil=COIL_STA_OK;
	}
}

//外部中断1，计算线圈频率使用
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
		if(Group1active == 1)
			CH1.FREQCnt++;
		else if(Group1active == 2)
			CH2.FREQCnt++;
		else if(Group1active == 3)
			CH3.FREQCnt++;
		else if(Group1active == 4)
			CH4.FREQCnt++;
	}
}

//外部中断4，计算线圈频率使用
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		if(Group2active == 1)
			CH5.FREQCnt++;
		else if(Group2active == 2)
			CH6.FREQCnt++;
		else if(Group2active == 3)
			CH7.FREQCnt++;
		else if(Group2active == 4)
			CH8.FREQCnt++;
	}
}

//TEST KEY
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line12);

	}
}
