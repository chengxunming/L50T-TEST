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
	
	COIL_LED_GPIO_Init();
}
//线圈指示灯引脚初始化
void COIL_LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE,  ENABLE );

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	COIL_LED_OFF();
}


void COIL_LED_OFF(void)//关闭所有线圈LED
{
	CH1_Check_Reset;CH2_Check_Reset;CH3_Check_Reset;CH4_Check_Reset;CH5_Check_Reset;CH6_Check_Reset;CH7_Check_Reset;CH8_Check_Reset;
	CH1_Error_Reset;CH2_Error_Reset;CH3_Error_Reset;CH4_Error_Reset;CH5_Error_Reset;CH6_Error_Reset;CH7_Error_Reset;CH8_Error_Reset;
}

void COIL_LED_ON(void)//打开所有线圈LED
{
	CH1_Check_Set;CH2_Check_Set;CH3_Check_Set;CH4_Check_Set;CH5_Check_Set;CH6_Check_Set;CH7_Check_Set;CH8_Check_Set;
	CH1_Error_Set;CH2_Error_Set;CH3_Error_Set;CH4_Error_Set;CH5_Error_Set;CH6_Error_Set;CH7_Error_Set;CH8_Error_Set;
}

void LED_ALL_ON(void)
{
	RUNLEDON;
	COIL_LED_ON();
}

