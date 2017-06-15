#include "bsp_time.h"
#include "global.h"

//��ʱ����ʼ��
void Time_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

    //-----------------------------------------------------------------TIM2	  1ms  ����ʱ��
	//���� timer2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		
	TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=(1000-1);		 					/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */					//��ʱ��ʽ��(TIM_Period + 1) * (TIM_Prescaler + 1) / TIMx Clock
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    	/* ʱ��Ԥ��Ƶ�� 72M/36000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* �������жϱ�־ */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM2, ENABLE);												/* ����ʱ�� */
	
	//-----------------------------------------------------------------TIM3		100ms 
	//���� timer3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
	TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Period=(200-1);		 					/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    /* ʱ��Ԥ��Ƶ�� 72M/36000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    /* �������жϱ�־ */
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM3, ENABLE);												/* ����ʱ�� */
}

//��ʱ��2�жϻص����� 1ms
static void TIM2_ISR_Callback(void)
{
	Time_KeyCheck_Count++;
}
//��ʱ��3�жϻص����� 100ms
static void TIM3_ISR_Callback(void)
{
	static uint8_t time_led_count=0;
	
	if(MFU_TestState==MFU_TEST_START)
	{
		Time_MFUCheck_Count++;
		MFU_CheckStatus();
	}
	
	if(FLAG_SYS_RUN)
	{
		time_led_count++;
		if(time_led_count==5)
		{
			LedRun_ON;
		}
		else if(time_led_count==10)
		{
			time_led_count=0;
			LedRun_OFF;
		}
	}
	
	if(ring.IsRing)
	{
		ring.RingCount++;
		if(ring.RingCount>=10)
		{
			BUZZER_OFF;
			ring.IsRing=false;
			ring.RingCount=0;
		}
	}
}
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		TIM2_ISR_Callback();
	}
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		TIM3_ISR_Callback();
	}
}


