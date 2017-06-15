#include "bsp.h"
#include "bsp_can.h"
#include "bsp_pdu.h"
#include "delay.h"

Frame_DefType send_Frame;

static void ChannelX_ResultSend(uint8_t chNum,uint8_t chState);
//PDU板CAN接口测试
void PDU_CanTest(void)
{
	uint8_t can_test_num=0;
	while(1)
	{
		can_test_num++; //can测试次数累加
		
		send_Frame.module_id=PDU_CAN_ID;
		send_Frame.fun_code=FUN_CAN;
		send_Frame.dataH=CIRCUIT_CAN;
		send_Frame.dataL=0xff;
		send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
		Send_CAN_DataFrame_PDU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
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

	send_Frame.module_id=PDU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=CIRCUIT_CAN;
	if( ev_CanRespone==CAN_EV_ResOk )send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;
	
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame_PDU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
	
}
//4通道外灯控制电路测试
void PDU_ChannelTest(void)
{
	uint8_t ChState[4];
	uint8_t i;
	
	memset(ChState,0,4);
	
	ChannelAlloff();//关灯
	GetChannelVoltageStatus();//检测外灯电压状态
	for(i=0;i<4;i++)
	{
		ChState[i]|=ChannelVoltage[3*i];
		ChState[i]|=ChannelVoltage[3*i+1]<<1;
		ChState[i]|=ChannelVoltage[3*i+2]<<2;
	}
	
	ChannelAllon();//开灯
	delay_ms(1000);
	GetChannelVoltageStatus();//检测外灯电压状态
	for(i=0;i<4;i++)
	{
		ChState[i]|=ChannelVoltage[3*i]<<4;
		ChState[i]|=ChannelVoltage[3*i+1]<<5;
		ChState[i]|=ChannelVoltage[3*i+2]<<6;
	}
	
	ChannelX_ResultSend(CIRCUIT_CH1,ChState[0]);
	ChannelX_ResultSend(CIRCUIT_CH2,ChState[1]);
	ChannelX_ResultSend(CIRCUIT_CH3,ChState[2]);
	ChannelX_ResultSend(CIRCUIT_CH4,ChState[3]);
}

//发送测试结束帧
void PDU_TestEnd(void)
{
	send_Frame.module_id=PDU_CAN_ID;
	send_Frame.fun_code=FUN_END;
	send_Frame.dataH=0xff;
	send_Frame.dataL=0xff;
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame_PDU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧	
}

//CAN接口测试命令响应处理
void PDU_FunCan_Respone_Deal(Frame_DefType * data)
{
	if( (data->dataH == CIRCUIT_CAN ) && (data->dataL==0xff) && (data->module_id==PDU_CAN_ID) )
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
static void ChannelX_ResultSend(uint8_t chNum,uint8_t chState)
{
	uint8_t sumCheckVal;
	memset((uint8_t *)(&send_Frame),0,sizeof(send_Frame));
	send_Frame.module_id=PDU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=chNum;
	if(chState==0x70)send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;

	sumCheckVal=DataCheckSum((uint8_t *)(&send_Frame),4);
	send_Frame.check_sum=sumCheckVal;
			
	Send_CAN_DataFrame_PDU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
}

void wd_test(void)
{
	uint8_t ChState[4];
	uint8_t i;
	
	memset(ChannelVoltage,0x00,12);
	memset(ChState,0,4);
	
	ChannelAlloff();//关灯
	delay_ms(1000);		
	GetChannelVoltageStatus();//检测外灯电压状态
	for(i=0;i<4;i++)
	{
		ChState[i]|=ChannelVoltage[3*i];
		ChState[i]|=ChannelVoltage[3*i+1]<<1;
		ChState[i]|=ChannelVoltage[3*i+2]<<2;
	}
	
	ChannelAllon();//开灯
	delay_ms(1000);	
	GetChannelVoltageStatus();//检测外灯电压状态
	for(i=0;i<4;i++)
	{
		ChState[i]|=ChannelVoltage[3*i]<<4;
		ChState[i]|=ChannelVoltage[3*i+1]<<5;
		ChState[i]|=ChannelVoltage[3*i+2]<<6;
	}
	
	memset((uint8_t *)(&send_Frame),0,sizeof(send_Frame));
	send_Frame.module_id=PDU_CAN_ID;
	send_Frame.fun_code=ChState[0];
	send_Frame.dataH=ChState[1];
	send_Frame.dataL=ChState[2];

	send_Frame.check_sum=ChState[3];
			
	Send_CAN_DataFrame_PDU((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
}
