#include "stm32f10x.h"
#include "global.h"
#include "bsp.h"
#include "bsp_can.h"
#include "bsp_time.h"
#include "bsp_io.h"
#include "bsp_led.h"
#include "delay.h"
/* Extern variables-----------------------------------------------------------*/
uint8_t Time_CanRespone_Count=0;
bool fault_canTest=false;
CAN_EV_TYPE ev_CanRespone=CAN_EV_None;

int main(void)
{	
	SystemInit();			 //ϵͳ��ʼ��	 	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init(72);	    	 //��ʱ������ʼ��
	
	IO_Channel_GPIO_Init();
	IO_InCheck_GPIO_Init();
	LED_Init();
	CAN_Configuration();
	Time_Init();
	
	delay_ms(1000);			//�ϵ��ӳ�1s������Ƭ���ȶ�
	
	IOU_CanTest();
	IOU_ChannelTest();
	LED_ALL_ON();
	IOU_TestEnd();
	while(1)
	{
		
	}
}

