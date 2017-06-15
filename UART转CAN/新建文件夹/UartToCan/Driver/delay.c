#include "delay.h"

//Systick时钟 寄存器
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTRL--->控制寄存器
		BIT0 : Systick使能位,		0-->关闭Systick功能				1-->开启Systick功能
		BIT1 : Systick中断使能位,	0-->关闭Systick中断				1-->开启Systick中断
		BIT2 : Systick 时钟源选择位, 0-->使用CCLK/8作为Systick时钟	1-->使用CCLK作为Systick时钟
		BIT16: Systick计数比较标志,如果在上次读取本寄存器后,SysTick已经数到了0，则该位为1;如果读取该位,该位将自动清零
		 
LOAD--->重载寄存器 
		Systick是一个递减的定时器,当定时器递减至0时,重载寄存器中的值就会被重装载,继续开始递减,LOAD重载寄存器是个24位的寄存器,最大计数0xFFFFFF
		
VAL --->当前值寄存器
		24位的寄存器,读取时返回当前倒计数的值,写它则使之清零,同时还会清除在SysTick控制及状态寄存器中的COUNTFLAG标志
		
CALIB--->校准值寄存器
		BIT0-23 : TENMS,校准值,该值为0则无法使用校准功能
		BIT30 	: SKEW		0-->校准值是准确的1ms    1-->校准值不是准确的1ms
		BIT31 	: NOREF 	0-->外部参考时钟可用		1-->没有外部参考时钟（STCLK不可用）
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


//SysTick定时器时钟源选择  定义SysTick_CLKSource_CoreCLK_Div8表示时钟为内核时钟8分频，注释掉该宏定义表示时钟为内核时钟
#define  SysTick_CLKSource_CoreCLK_Div8	    


static uint32_t fac_ms,fac_us;

void delay_init(void)
{
	uint32_t SysClkMhz_Val;
	
	SysTick->CTRL &= ~( ((uint32_t)SysTick_CTRL_ENABLE_Msk<<SysTick_CTRL_ENABLE_Pos) | ((uint32_t)SysTick_CTRL_TICKINT_Msk<<SysTick_CTRL_TICKINT_Pos) );//关闭Systick功能和Systick中断
	
#if  defined(SysTick_CLKSource_CoreCLK_Div8)
	SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk<<SysTick_CTRL_CLKSOURCE_Pos);
	SysClkMhz_Val=SystemCoreClock/1000000;
#else 
	SysTick->CTRL &= ~((uint32_t)SysTick_CTRL_CLKSOURCE_Msk<<SysTick_CTRL_CLKSOURCE_Pos);
	SysClkMhz_Val=SystemCoreClock/8000000;
#endif
	
	fac_us=SysClkMhz_Val;
	fac_ms=fac_us*1000;
}

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//SYSCLK单位为Hz,nms单位为ms
//对100M条件下,nms<=0xffffff*8*1000/SYSCLK,nms<=167
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;	
	uint16_t count_100ms;
	
	SysTick->LOAD=fac_ms-1;//时间加载1ms(SysTick->LOAD为24bit)
	for(count_100ms=0;count_100ms<nms;count_100ms++)
	{
		SysTick->VAL =0x00;           //清空计数器
		SysTick->CTRL |= ((uint32_t)SysTick_CTRL_ENABLE_Msk<<SysTick_CTRL_ENABLE_Pos);      //开始倒数 
		do
		{
			temp=SysTick->CTRL;
		}
		while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
		SysTick->CTRL &= ~((uint32_t)SysTick_CTRL_ENABLE_Msk<<SysTick_CTRL_ENABLE_Pos);     //关闭计数器	
	}
	SysTick->VAL =0x00;       //清空计数器
	
		  	    
}   
//延时nus
//nus为要延时的us数.	最大为150000us	    								   
void delay_us(uint32_t nus)
{		
	uint32_t temp;	
	
	SysTick->LOAD=nus*fac_us-1; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL |= ((uint32_t)SysTick_CTRL_ENABLE_Msk<<SysTick_CTRL_ENABLE_Pos);      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL &= ~((uint32_t)SysTick_CTRL_ENABLE_Msk<<SysTick_CTRL_ENABLE_Pos);     //关闭计数器
	SysTick->VAL =0x00;       //清空计数器	 
}



