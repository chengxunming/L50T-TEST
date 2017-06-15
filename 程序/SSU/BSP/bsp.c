#include "bsp.h"
#include "bsp_adc.h"
#include "bsp_can.h"
#include "bsp_led.h"
#include "bsp_ds18b20.h"

Frame_DefType send_Frame;

//SSU板CAN接口测试
void SSU_CanTest(void)
{
	uint8_t can_test_num=0;
	while(1)
	{
		can_test_num++; //can测试次数累加
		
		send_Frame.module_id=SSU_CAN_ID;
		send_Frame.fun_code=FUN_CAN;
		send_Frame.dataH=CIRCUIT_CAN;
		send_Frame.dataL=0xff;
		send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
		Send_CAN_DataFrame_SSU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
		Time_CanRespone_Count=0;
		ev_CanRespone=CAN_EV_ResReady;
		while(ev_CanRespone==CAN_EV_ResReady);
		if(ev_CanRespone!=CAN_EV_ResTimeOut)
		{
			fault_canTest=false;
			break;
		}
		else if(can_test_num>=3)
		{
			fault_canTest=true;
			break;
		}
	}

	send_Frame.module_id=SSU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=CIRCUIT_CAN;
	if( ev_CanRespone==CAN_EV_ResOk )send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;
	
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame_SSU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
	
}
//AC采集电路测试
void SSU_ACcheckTest(void)
{
	uint8_t ac_val;
	ac_val=Get_AC_Average();
	
	send_Frame.module_id=SSU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=CIRCUIT_AC;
	if( (ac_val>176) && (ac_val<255) )send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;
	
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame_SSU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧	
	
	
}
//DC采集电路测试
void SSU_DCcheckTest(void)
{
	uint8_t dc_val;
	dc_val=Get_DC_Average();
	
	send_Frame.module_id=SSU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=CIRCUIT_DC;
	if( (dc_val>100) && (dc_val<130) )send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;
	
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame_SSU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧

}
void ac_test(void)
{
	uint8_t ac_val,dc_val;
	state_ADC=ADC_STA_COL;
	while(state_ADC!=ADC_STA_Ok);
	
	ac_val=Get_AC_Average();
	dc_val=Get_DC_Average();
	
	send_Frame.module_id=SSU_CAN_ID;
	send_Frame.fun_code=ac_val;
	send_Frame.dataH=ac_val+90;
	send_Frame.dataL=dc_val;
	
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame_SSU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
	
	
	state_ADC=ADC_STA_Start;
}
//电压采集电路测试
void SSU_VoltTest(void)
{
	state_ADC=ADC_STA_COL;
	while(state_ADC!=ADC_STA_Ok);
	SSU_ACcheckTest();
	SSU_DCcheckTest();
	state_ADC=ADC_STA_Start;
}
//温度采集电路测试
void SSU_DS18B20Test(void)
{
	short temp;
	temp=DS18B20_Get_Temp();
	
	send_Frame.module_id=SSU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=CIRCUIT_DS18B20;
	if( (temp>10) && (temp<50) )send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;
		
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame_SSU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧	

}
//继电器测试
void SSU_RelayTest(void)
{
	DIMRelayON;
	delay_ms(500);
	MFURelayON;
	delay_ms(500);
	PDURelayON;
}
//发送测试结束帧
void PDU_TestEnd(void)
{
	send_Frame.module_id=SSU_CAN_ID;
	send_Frame.fun_code=FUN_END;
	send_Frame.dataH=0xff;
	send_Frame.dataL=0xff;
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame_SSU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧	
}

//CAN接口测试命令响应处理
void SSU_FunCan_Respone_Deal(Frame_DefType * data)
{
	if( (data->dataH == CIRCUIT_CAN ) && (data->dataL==0xff) && (data->module_id==SSU_CAN_ID) )
	{
		ev_CanRespone=CAN_EV_ResOk;
	}
	else 
	{
		ev_CanRespone=CAN_EV_ResFail;
	}
}

//计算和校验
uint8_t DataCheckSum(uint8_t *data,uint8_t len)
{
	uint8_t sum=0;
	uint8_t i;
	for(i=0;i<len;i++)
	{
		sum+=data[i];
	}
	sum=0xff-sum;
	return sum;
}


