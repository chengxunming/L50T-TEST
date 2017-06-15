#include "bsp.h"
#include "bsp_net.h"

/* Private define ------------------------------------------------------------*/
#define SOCK_UDP_FD        	 0
#define SOCK_UDP_PORT        49489

static bool Circuit_ErrState[10];
static CANTX_STATE_TYPE CanSend_State=CAN_TX_WAIT;
Frame_DefType send_Frame;
//��·״̬��λ
static void Circuit_State_Set(uint8_t CrtNo,uint8_t CrtState)
{
	bool stateErr_temp=true;
	uint8_t i;
	
	for(i=0;i<10;i++)
	{
		Circuit_ErrState[i]=false;
	}
		
	if(CrtState==1)stateErr_temp=false;
	else stateErr_temp=true;
	
	switch(CrtNo)
	{
		case CIRCUIT_1:
		case CIRCUIT_2:
		case CIRCUIT_3:
		case CIRCUIT_4:
		case CIRCUIT_5:
		case CIRCUIT_6:
		case CIRCUIT_7:
		case CIRCUIT_8:
		case CIRCUIT_9:
		case CIRCUIT_10:
			Circuit_ErrState[CrtNo-1]=stateErr_temp;
			break;
	}
}
//ˢ�¹��ϵ�
static void FaultLed_Refresh(void)
{
	if(Circuit_ErrState[0])Led1_OFF;
	else Led1_ON;
	
	if(Circuit_ErrState[1])Led2_OFF;
	else Led2_ON;
	
	if(Circuit_ErrState[2])Led3_OFF;
	else Led3_ON;
	
	if(Circuit_ErrState[3])Led4_OFF;
	else Led4_ON;
	
	if(Circuit_ErrState[4])Led5_OFF;
	else Led5_ON;
	
	if(Circuit_ErrState[5])Led6_OFF;
	else Led6_ON;
	
	if(Circuit_ErrState[6])Led7_OFF;
	else Led7_ON;
	
	if(Circuit_ErrState[7])Led8_OFF;
	else Led8_ON;
	
	if(Circuit_ErrState[8])Led9_OFF;
	else Led9_ON;
	
	if(Circuit_ErrState[9])Led10_OFF;
	else Led10_ON;
}
//CAN������Ϣ����
void CAN_RxMessage_Deal(Frame_DefType * FrameRx)
{
	switch(FrameRx->fun_code)
	{
		case FUN_CAN:
			memcpy(&send_Frame,FrameRx,sizeof(send_Frame));
			switch(FrameRx->module_id)
			{
				case MD_CPU:
					Module_Test=MD_CPU;
					CanSend_State=CAN_TX_START;
					LedCPU_ON;
					break;
				case MD_SSU:
					Module_Test=MD_SSU;
					CanSend_State=CAN_TX_START;
					LedSSU_ON;
					break;
				case MD_PDU:
					Module_Test=MD_PDU;
					CanSend_State=CAN_TX_START;
					LedPDU_ON;
					break;
				case MD_DET:
					Module_Test=MD_DET;
					CanSend_State=CAN_TX_START;
					LedDET_ON;
					break;
				case MD_IOU:
					Module_Test=MD_IOU;
					CanSend_State=CAN_TX_START;
					LedIOU_ON;
					break;
				case MD_MAU:
					Module_Test=MD_MAU;
					CanSend_State=CAN_TX_START;
					LedMAU_ON;
					break;
				default:
					Module_Test=MD_NONE;
					break;
			}
			break;
		case FUN_RESULT:
			if(FrameRx->module_id != Module_Test)return;
			Circuit_State_Set(FrameRx->dataH,FrameRx->dataL);
			break;
		case FUN_END:
			if( (FrameRx->module_id == Module_Test) && (FrameRx->dataH==0xff) && (FrameRx->dataL==0xff) )
			{
//				CanSend_State=CAN_TX_OK;
				FLAG_TEST_FINISH=true;
			}
			break;
		default:break;
	}
}
//������ɼ�⣬����������
void TEST_Finish_Check(void)
{
	if(FLAG_TEST_FINISH)
	{
		FLAG_TEST_FINISH=false;
		FaultLed_Refresh();
		LedOK_ON;//�����������ָʾ��
		BUZZER_Ring();//����1S��ʾ������Ա�������
	}
}
//CANͨ�Ų�����Ӧ����֡������������
void CAN_Respone_SendCheck(void)
{
	if(CanSend_State==CAN_TX_START)
	{
		Send_CAN_DataFrame(Module_Test,(uint8_t *)(&send_Frame),(uint8_t)sizeof(send_Frame));//����һ֡
		CanSend_State=CAN_TX_WAIT;
	}
//	else if(CanSend_State==CAN_TX_OK)
//	{
//		
//		CanSend_State=CAN_TX_WAIT;
//	}
}
//MFU���Լ�⣬����������
void MFU_TestCheck(void)
{
	uint8_t i;
	if(MFU_TestState==MFU_TEST_OK)
	{
		MFU_TestState=MFU_TEST_WAIT;
		for(i=0;i<10;i++)
		{
			Circuit_ErrState[i]=false;
		}
		
		if( (MFU_hs_State[0]!=MFU_hs_State[1]) && (MFU_Normal_State[0]==MFU_Normal_State[1]) )
		{
			Circuit_ErrState[0]=false;
		}
		else 
		{
			Circuit_ErrState[0]=true;
		}
		
		FLAG_TEST_FINISH=true;
	}
}
//�������socket����������������
void NET_UDP_SocketConfig(void)
{
	W5500_Config();
	W5500_UDP_SocketCreat(SOCK_UDP_FD,SOCK_UDP_PORT);
}
//����ػ����ԣ�����������
void NET_UDP_LoopBack(void)
{
	W5500_UDP_LoopBack(SOCK_UDP_FD,SOCK_UDP_PORT);
}

//�����У��
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
//ϵͳ��λ
//��λ����ִ�к����һ����ʱ����ʱ�ڼ������Ӧ�жϣ���˸�λǰҪ�ȹر��ж�
void SYS_RESET(void)
{
	__set_FAULTMASK(1);     //�ر������ж�
	NVIC_SystemReset();		//ϵͳ��λ
}
