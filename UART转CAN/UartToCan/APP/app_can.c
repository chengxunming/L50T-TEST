#include "app_can.h"

MODULE_TYPE Module_Test=MD_NONE;
static CANTX_STATE_TYPE CanSend_State=CAN_TX_WAIT;

static Frame_DefType receive_Frame;

uint8_t canUartData[256];
uint8_t candataLen=0;

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


void CAN1_Init(void)
{
	CAN_setup (1);                                  /* setup CAN Controller #1 */

//	CAN_wrFilter (1, 33, STANDARD_FORMAT);          /* Enable reception of messages */

	CAN_start (1);                                  /* start CAN Controller #2 */

	CAN_waitReady (1);                              /* wait til tx mbx is empty */	
}

void CAN2_Init(void)
{
	CAN_setup (2);                                  /* setup CAN Controller #2 */
//	CAN_wrFilter (1, 33, STANDARD_FORMAT);          /* Enable reception of messages */

	CAN_start (2);                                  /* start CAN Controller #2 */

	CAN_waitReady (2);                              /* wait til tx mbx is empty */
}

void Send_CAN_DataFrame(uint32_t ctrl,uint8_t addr,uint8_t *ptr,uint8_t len)
{
	uint8_t i;
	CAN_TxMsg[ctrl].id = addr;                           /* initialise message to send */
	for (i = 0; i < 8; i++) CAN_TxMsg[ctrl].data[i] = 0;
	CAN_TxMsg[ctrl].len = len;
	CAN_TxMsg[ctrl].format = STANDARD_FORMAT;
	CAN_TxMsg[ctrl].type = DATA_FRAME;
	
	for(i=0; i<len; i++)
	{
		CAN_TxMsg[ctrl].data[i] = *ptr++;
	}
	
	CAN_waitReady(ctrl); 
	if (CAN_TxRdy[ctrl]) 
	{
		CAN_TxRdy[ctrl] = 0; 
		CAN_wrMsg (ctrl+1, &CAN_TxMsg[ctrl]);
	}
}

//CAN接收消息处理
void CAN_RxMessage_Deal(Frame_DefType * FrameRx)
{
	switch(FrameRx->fun_code)
	{
		case FUN_CAN:
//			memcpy(&send_Frame,FrameRx,sizeof(send_Frame));
			switch(FrameRx->module_id)
			{
				case MD_CPU:
					Module_Test=MD_CPU;
					CanSend_State=CAN_TX_START;

					break;
				case MD_SSU:
					Module_Test=MD_SSU;
					CanSend_State=CAN_TX_START;

					break;
				case MD_PDU:
					Module_Test=MD_PDU;
					CanSend_State=CAN_TX_START;

					break;
				case MD_DET:
					Module_Test=MD_DET;
					CanSend_State=CAN_TX_START;

					break;
				case MD_IOU:
					Module_Test=MD_IOU;
					CanSend_State=CAN_TX_START;

					break;
				case MD_MAU:
					Module_Test=MD_MAU;
					CanSend_State=CAN_TX_START;

					break;
				default:
					Module_Test=MD_NONE;
					break;
			}
			break;
		case FUN_RESULT:
//			if(FrameRx->module_id != Module_Test)return;
//			Circuit_State_Set(FrameRx->dataH,FrameRx->dataL);
			break;
		case FUN_END:
			if( (FrameRx->dataH==0xff) && (FrameRx->dataL==0xff) )
			{
//				CanSend_State=CAN_TX_OK;
//				FLAG_TEST_FINISH=true;
//				Uart_SendMultByte(COM2,(uint8_t *)(&receive_Frame),5);
				Uart_SendMultByte(COM2,(uint8_t *)canUartData,candataLen);
				memset((uint8_t *)canUartData,0,candataLen);
				candataLen=0;
				
			}
			
			break;
		default:break;
	}
}

//CAN通信测试响应数据帧，主函数调用
void CAN_Respone_SendCheck(uint32_t ctrl)
{
	if(CanSend_State==CAN_TX_START)
	{
		Send_CAN_DataFrame(ctrl,Module_Test,(uint8_t *)(&receive_Frame),(uint8_t)sizeof(receive_Frame));//发送一帧
		CanSend_State=CAN_TX_WAIT;
	}
}

//接收CAN数据帧，作功能识别
void Receive_CAN_DataFrame(uint8_t* canRx,uint8_t num)
{
	uint8_t i;
	uint8_t *prt_rx;
	if(num!=5)
	{
		return;
	}
	prt_rx=(uint8_t *)(&receive_Frame);
	for(i=0; i<num; i++)
	{
		*(prt_rx+i)= canRx[i];
	}
	if(DataCheckSum((uint8_t *)(&receive_Frame),4)!=receive_Frame.check_sum)
	{
		return;
	}
	
	CAN_RxMessage_Deal(&receive_Frame);

}

void CAN_RX(uint32_t ctrl)
{
//	uint8_t i;
	if(CAN_RxRdy[ctrl])
	{
		
//		for(i=0;i<CAN_RxMsg[ctrl].len;i++)
//		{
//			canUartData[candataLen++]=CAN_RxMsg[ctrl].data[i];
//		}
		CAN_RxRdy[ctrl] = 0;
		
//		Uart_SendMultByte(COM2,CAN_RxMsg[ctrl].data,CAN_RxMsg[ctrl].len);
		
		Receive_CAN_DataFrame(CAN_RxMsg[ctrl].data,CAN_RxMsg[ctrl].len);
		
		CAN_Respone_SendCheck(ctrl);
	}
}

void CAN2_TEST(void)
{
	Frame_DefType send_Frame;
	
	send_Frame.module_id=0x04;
		send_Frame.fun_code=1;
		send_Frame.dataH=1;
		send_Frame.dataL=0xff;
		send_Frame.check_sum=DataCheckSum((uint8_t *)(&send_Frame),4);
		
	Send_CAN_DataFrame(CAN2,0x04,(uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//发送一帧
}

