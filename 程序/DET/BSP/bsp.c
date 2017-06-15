#include "bsp.h"
#include "bsp_can.h"
#include "bsp_coil.h"
#include "bsp_led.h"
#include "delay.h"

Frame_DefType send_Frame;

static void ChannelX_ResultSend(uint8_t chNum,bool chState);
//CAN接口测试
void DET_CanTest(void)
{
	uint8_t can_test_num=0;
	while(1)
	{
		can_test_num++; //can测试次数累加
		
		send_Frame.module_id=DET_CAN_ID;
		send_Frame.fun_code=FUN_CAN;
		send_Frame.dataH=CIRCUIT_CAN;
		send_Frame.dataL=0xff;
		send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
		Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
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

	send_Frame.module_id=DET_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=CIRCUIT_CAN;
	if( ev_CanRespone==CAN_EV_ResOk )send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;
	
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
	
}
//逻辑通道检测
void DET_ChannelTest(void)
{
	state_Coil=COIL_STA_DET;
	while(state_Coil!=COIL_STA_OK);
	COIL_Check();
	ChannelX_ResultSend(CIRCUIT_CH1,CH1.state);
	ChannelX_ResultSend(CIRCUIT_CH2,CH2.state);
	ChannelX_ResultSend(CIRCUIT_CH3,CH3.state);
	ChannelX_ResultSend(CIRCUIT_CH4,CH4.state);
	ChannelX_ResultSend(CIRCUIT_CH5,CH5.state);
	ChannelX_ResultSend(CIRCUIT_CH6,CH6.state);
	ChannelX_ResultSend(CIRCUIT_CH7,CH7.state);
	ChannelX_ResultSend(CIRCUIT_CH8,CH8.state);

}

//发送测试结束帧
void DET_TestEnd(void)
{
	send_Frame.module_id=DET_CAN_ID;
	send_Frame.fun_code=FUN_END;
	send_Frame.dataH=0xff;
	send_Frame.dataL=0xff;
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧	
}

//CAN接口测试命令响应处理
void DET_FunCan_Respone_Deal(Frame_DefType * data)
{
	if( (data->dataH == CIRCUIT_CAN ) && (data->dataL==0xff) && (data->module_id==DET_CAN_ID) )
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

//通道测试结果发送到测试板
static void ChannelX_ResultSend(uint8_t chNum,bool chState)
{
	send_Frame.module_id=DET_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=chNum;
	if(chState==true)send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;

	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
			
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
}



static void coilVal_show(uint8_t chNum,uint32_t data)
{
	send_Frame.module_id=DET_CAN_ID;
	send_Frame.fun_code=data>>24;
	
	send_Frame.dataH=data>>16;
	send_Frame.dataL=data>>8;


	send_Frame.check_sum=data;
			
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
	DET_TestEnd();
	delay_ms(200);
}

void coilVal_test(void)
{	
	state_Coil=COIL_STA_DET;
	while(state_Coil!=COIL_STA_OK);

	coilVal_show(CIRCUIT_CH1,CH1.Frequnce);
	coilVal_show(CIRCUIT_CH2,CH2.Frequnce);
	coilVal_show(CIRCUIT_CH3,CH3.Frequnce);
	coilVal_show(CIRCUIT_CH4,CH4.Frequnce);
	coilVal_show(CIRCUIT_CH5,CH5.Frequnce);
	coilVal_show(CIRCUIT_CH6,CH6.Frequnce);
	coilVal_show(CIRCUIT_CH7,CH7.Frequnce);
	coilVal_show(CIRCUIT_CH8,CH8.Frequnce);	
}

