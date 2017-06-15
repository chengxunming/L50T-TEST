#include "stm32f10x.h"
#include "bsp.h"

/* Extern variables-----------------------------------------------------------*/
MODULE_TYPE Module_Test=MD_NONE;
MFUTEST_STATE_TYPE MFU_TestState=MFU_TEST_WAIT;
uint8_t Time_KeyCheck_Count=0;//Ϊ������ѯ����ṩʱ��
uint8_t Time_MFUCheck_Count=0;//ΪMFU״̬����ṩʱ��
uint8_t Time_BUZZER_Count=0;//�����������ʱ
BuzzerHandle_DefType  ring;//������
bool FLAG_SYS_RUN=false;
bool FLAG_TEST_FINISH=false;

int main(void)
{	
	SystemInit();			 //ϵͳ��ʼ��	 	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init(72);	    	 //��ʱ������ʼ��
	
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



