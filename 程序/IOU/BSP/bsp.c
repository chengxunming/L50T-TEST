#include "bsp.h"
#include "bsp_can.h"
#include "bsp_io.h"
#include "bsp_led.h"

Frame_DefType send_Frame;

static void ChannelX_ResultSend(uint8_t chNum,uint8_t chState);
//CAN接口测试
void IOU_CanTest(void)
{
	uint8_t can_test_num=0;
	while(1)
	{
		can_test_num++; //can测试次数累加
		
		send_Frame.module_id=IOU_CAN_ID;
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

	send_Frame.module_id=IOU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=CIRCUIT_CAN;
	if( ev_CanRespone==CAN_EV_ResOk )send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;
	
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
	
}
//逻辑通道检测
void IOU_ChannelTest(void)
{
	uint8_t ChState[8];
	uint8_t i;
	
	memset(ChState,0,8);
	
	IO_OUT_ALL_OFF();
	GetChannelStatus();
	for(i=0;i<8;i++)
	{
		ChState[i]|=CH_IN[i]*0x0f;
	}
	
	IO_OUT_ALL_ON();
	GetChannelStatus();
	for(i=0;i<8;i++)
	{
		ChState[i]|=(CH_IN[i]*0x0f)<<4;
		
		ChannelX_ResultSend(i+CIRCUIT_CH1,ChState[i]);
	}
}

//发送测试结束帧
void IOU_TestEnd(void)
{
	send_Frame.module_id=IOU_CAN_ID;
	send_Frame.fun_code=FUN_END;
	send_Frame.dataH=0xff;
	send_Frame.dataL=0xff;
	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧	
}

//CAN接口测试命令响应处理
void IOU_FunCan_Respone_Deal(Frame_DefType * data)
{
	if( (data->dataH == CIRCUIT_CAN ) && (data->dataL==0xff) && (data->module_id==IOU_CAN_ID) )
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
	send_Frame.module_id=IOU_CAN_ID;
	send_Frame.fun_code=FUN_RESULT;
	send_Frame.dataH=chNum;
	if(chState==0xf0)send_Frame.dataL=0x01;
	else send_Frame.dataL=0x02;

	send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
			
	Send_CAN_DataFrame((uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
}
