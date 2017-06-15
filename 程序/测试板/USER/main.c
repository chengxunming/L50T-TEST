#include "stm32f10x.h"
#include "bsp.h"

/* Extern variables-----------------------------------------------------------*/
MODULE_TYPE Module_Test=MD_NONE;
MFUTEST_STATE_TYPE MFU_TestState=MFU_TEST_WAIT;
uint8_t Time_KeyCheck_Count=0;//为按键轮询检测提供时基
uint8_t Time_MFUCheck_Count=0;//为MFU状态检测提供时基
uint8_t Time_BUZZER_Count=0;//蜂鸣器响铃计时
BuzzerHandle_DefType  ring;//蜂鸣器
bool FLAG_SYS_RUN=false;
bool FLAG_TEST_FINISH=false;

int main(void)
{	
	SystemInit();			 //系统初始化	 	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init(72);	    	 //延时函数初始化
	
	CAN_Configuration();
	MFU_GPIO_Init();
	KEY_Init();
	LED_Init();
	BUZZER_Init();
	Time_Init();
	
	NET_UDP_SocketConfig();

	FLAG_SYS_RUN=true;
	while(1)
	{
		KEY_Scan();
		MFU_TestCheck();
		CAN_Respone_SendCheck();
		NET_UDP_LoopBack();
		TEST_Finish_Check();
	}
}



