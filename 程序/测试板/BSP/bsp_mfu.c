#include "bsp_mfu.h"

#define MFU_Check_1 	1
#define MFU_Check_2 	2
#define MFU_Check_OK	3

static uint8_t MFUStatus;
static uint8_t MFU_Check_Process=0;

uint8_t MFU_hs_State[2],MFU_Normal_State[2];
//MFU引脚初始化
void MFU_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	MFU_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.GPIO_Pin = MFU_CONTROL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MFU_GPIO, &GPIO_InitStructure);//初始化GPIOC
	
	GPIO_SetBits(MFU_GPIO,MFU_CONTROL_PIN);	//黄闪使能，MFU工作
	
	//MFU工作状态检测
	GPIO_InitStructure.GPIO_Pin   = MFU_CHECK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(MFU_GPIO,&GPIO_InitStructure);
}
//MFU工作状态检测，定时器中断调用
void MFU_CheckStatus(void)
{
	static uint8_t state_count=0;
	if(Time_MFUCheck_Count == 5)
	{
		if(MFU_Check_Process==MFU_Check_1)
		{
			MFU_hs_State[state_count]=MFUStatus;
			state_count++;
			
			if(state_count>=2)
			{
				state_count=0;
				MFU_DISABLE;//切换正常模式
				MFU_Check_Process=MFU_Check_2;//MFU测试第二阶段标志位
			}
		}
		else if(MFU_Check_Process==MFU_Check_2)
		{
			MFU_Normal_State[state_count]=MFUStatus;
			state_count++;
			
			if(state_count>=2)
			{
				state_count=0;
				MFU_Check_Process=MFU_Check_OK;
				MFU_TestState=MFU_TEST_OK;
			}
		}
		Time_MFUCheck_Count=0;
	}
}
//启动MFU测试
void MFU_Test_Start(void)
{
	MFU_ENABLE;//打开黄闪模式
	MFU_Check_Process=MFU_Check_1;//MFU测试第一阶段标志位
	MFU_TestState=MFU_TEST_START;//开始测试标志位置位
}



