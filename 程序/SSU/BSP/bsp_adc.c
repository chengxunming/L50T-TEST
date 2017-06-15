#include "bsp_adc.h"

static int AC_Value[50],DC_Value[50];										//电压数据存储数组
static int Average_AC[20],Average_DC[20];									//平均AC及DC电压存储
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
* 名    称： sqrt_16
* 功    能： 平方根计算
* 入口参数： 无
* 出口参数： 无
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
//* 名    称： swRoot
//* 功    能： 函数对输入参数进行求平方根计算
//* 入口参数： dwNumber  输入参数            
//* 出口参数： 输入参数的平方根
//*******************************************************************
uint16_t swRoot(uint32_t dwNumber)			//求平方根函数
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
* 名    称： square_sum_average
* 功    能： 有效值计算
* 入口参数： 无
* 出口参数： 无
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


//计算AC电压有效值
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

//计算DC电压有效值
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

//ADC1配置
void ADC1_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	ADC_GPIO_Init();
	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}

//ADC取值函数，2个通道。
void Get_Adc(uint8_t num)  	
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	AC_Value[num] = ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	DC_Value[num] = ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

//计算有效值
void Get_VoltRMS(uint8_t num)
{
	Average_DC[num] = GetDCVoltRMS();   	//读取DC电压有效值 
	Average_AC[num] = GetACVoltRMS();	//读取AC电压有效值
}
//计算平均值
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
//获取交流电压值
uint8_t Get_AC_Average(void)
{
	return AC_Val;
}
//获取DC电压值
uint8_t Get_DC_Average(void)
{
	return DC_Val;
}
