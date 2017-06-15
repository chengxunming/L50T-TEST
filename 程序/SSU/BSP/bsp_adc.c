#include "bsp_adc.h"

static int AC_Value[50],DC_Value[50];										//��ѹ���ݴ洢����
static int Average_AC[20],Average_DC[20];									//ƽ��AC��DC��ѹ�洢
static uint8_t AC_Val,DC_Val;

static void ADC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE );
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/*******************************************************************
* ��    �ƣ� sqrt_16
* ��    �ܣ� ƽ��������
* ��ڲ����� ��
* ���ڲ����� ��
*******************************************************************/

unsigned int sqrt_16(unsigned long M)
{
  unsigned int N, i;
  unsigned long tmp, ttp;
  if (M == 0)
  return 0;
  N = 0;
  tmp = (M >> 30);
  M <<= 2;
  if (tmp > 1)
  {
      N ++;
      tmp -= N;
  }
  for (i=15; i>0; i--)
  {
      N <<= 1;
      tmp <<= 2;
      tmp += (M >> 30);
      ttp = N;
      ttp = (ttp<<1)+1;
      M <<=2;
      if(tmp>=ttp)
      {
              tmp-=ttp;
              N++;
      }
  }
  return N;
}


//*******************************************************************
//* ��    �ƣ� swRoot
//* ��    �ܣ� �������������������ƽ��������
//* ��ڲ����� dwNumber  �������            
//* ���ڲ����� ���������ƽ����
//*******************************************************************
uint16_t swRoot(uint32_t dwNumber)			//��ƽ��������
{

	uint32_t dwSquareRoot;

	if (dwNumber==0)
		return 0;
	else if (dwNumber<=4194304)
		dwSquareRoot=(dwNumber>>10)+63;
	else if (dwNumber<=134217728)
		dwSquareRoot=(dwNumber>>12)+63;
	else
		dwSquareRoot=(dwNumber>>14)+63;
	for (uint8_t i=0;i<5;i++)
		dwSquareRoot=(dwSquareRoot+dwNumber/dwSquareRoot)/2;

	return((uint16_t)dwSquareRoot);
}	
/*******************************************************************
* ��    �ƣ� square_sum_average
* ��    �ܣ� ��Чֵ����
* ��ڲ����� ��
* ���ڲ����� ��
*******************************************************************/
int square_sum_average(int *p, int n)
{
        uint32_t sum_average=0;
        long int square_sum=0;
        int i;
        for(i=0;i<n;i++)
        {
			square_sum+=(*(p+i))*(*(p+i));
        }
        sum_average = (square_sum/n);
        sum_average = sqrt_16(sum_average);
        return sum_average;
}


//����AC��ѹ��Чֵ
uint16_t GetACVoltRMS(void)
{
	uint16_t wLineVoltRMS;
	float temp;
	temp = square_sum_average(AC_Value,50);
//	wLineVoltRMS=(uint32_t)((temp*33*13)/4096); //V2.0
	
	wLineVoltRMS=(uint32_t)(temp*33*14.5)/4096; //V1.3
	
	wLineVoltRMS=wLineVoltRMS+6;

	return wLineVoltRMS;
	
}

//����DC��ѹ��Чֵ
uint8_t GetDCVoltRMS(void)
{
	int DCTemp;
	float DCVolt;
	uint8_t DCRMS=0;
	
	DCTemp = square_sum_average(DC_Value,50);
	DCVolt = ((((float)DCTemp)/4096)*33*6);
	DCRMS = (uint8_t)DCVolt;
	return DCRMS;
}

//ADC1����
void ADC1_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	ADC_GPIO_Init();
	ADC_DeInit(ADC1);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
}

//ADCȡֵ������2��ͨ����
void Get_Adc(uint8_t num)  	
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	AC_Value[num] = ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
	
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	DC_Value[num] = ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

//������Чֵ
void Get_VoltRMS(uint8_t num)
{
	Average_DC[num] = GetDCVoltRMS();   	//��ȡDC��ѹ��Чֵ 
	Average_AC[num] = GetACVoltRMS();	//��ȡAC��ѹ��Чֵ
}
//����ƽ��ֵ
void Cal_Average(void) 
{
	uint16_t ACtemp=0,DCtemp=0;
	for (uint8_t i =1;i<20;i++)
	{
		ACtemp += Average_AC[i];
	}
	AC_Val = (uint8_t)(ACtemp/19)-89;
	
	for (uint8_t i=1;i<20;i++)
	{
		DCtemp += Average_DC[i];
	}
	DC_Val = (DCtemp/19)+1;
	
}
//��ȡ������ѹֵ
uint8_t Get_AC_Average(void)
{
	return AC_Val;
}
//��ȡDC��ѹֵ
uint8_t Get_DC_Average(void)
{
	return DC_Val;
}
