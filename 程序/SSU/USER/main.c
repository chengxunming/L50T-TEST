#include "stm32f10x.h"
#include "global.h"
#include "bsp.h"
#include "bsp_can.h"
#include "bsp_time.h"
#include "bsp_led.h"
#include "delay.h"
#include "bsp_adc.h"
#include "bsp_ds18b20.h"
/* Extern variables-----------------------------------------------------------*/
uint8_t Time_CanRespone_Count=0;

bool fault_canTest=false;
	
CAN_EV_TYPE ev_CanRespone=CAN_EV_None;
ADC_STA_TYPE state_ADC=ADC_STA_Start;
void ac_test(void);
int main(void)
{
	SystemInit();			 //ϵͳ��ʼ��	 	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init(72);	    	 //��ʱ������ʼ��
	
	LED_Init();
	RELAY_GPIO_Init();
	ADC1_Config();
	DS18B20_Init();
	CAN_Configuration();
	Time_Init();
	
	delay_ms(1000);			//�ϵ��ӳ�1s������Ƭ���ȶ�
	
//	SSU_CanTest();
//	SSU_VoltTest();
//	SSU_DS18B20Test();
//	SSU_RelayTest();
	LED_AllOn();
//	PDU_TestEnd();
	while(1)
	{

	}
}

