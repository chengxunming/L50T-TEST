#ifndef __global_H
#define __global_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


/* Private typedef -----------------------------------------------------------*/
//typedef enum  
//{
//	FALSE=0,
//	TRUE=1
//}BOOL;

//通信帧功能码定义
typedef enum  
{
	FUN_CAN=1,
	FUN_RESULT,
	FUN_END
}FUN_TYPE;
//电路类型定义
typedef enum  
{
	CIRCUIT_CAN=1,
	CIRCUIT_CH1,
	CIRCUIT_CH2,
	CIRCUIT_CH3,
	CIRCUIT_CH4,
	CIRCUIT_CH5,
	CIRCUIT_CH6,
	CIRCUIT_CH7,
	CIRCUIT_CH8
}CIRCUIT_TYPE;
//通信帧定义
typedef struct
{
	uint8_t module_id;
	uint8_t fun_code;
	uint8_t dataH;
	uint8_t dataL;
	uint8_t check_sum;
}Frame_DefType;
//CAN通信测试状态类型
typedef enum  
{
	CAN_EV_None=0,
	CAN_EV_ResOk,
	CAN_EV_ResFail,
	CAN_EV_ResTimeOut,
	CAN_EV_ResReady,
}CAN_EV_TYPE;
//线圈检测状态定义
typedef enum 
{
	COIL_STA_START=0,
	COIL_STA_DET,
	COIL_STA_OK
}COIL_STA_TYPE;
//通道状态定义
typedef struct  
{
	uint32_t FREQCnt;		//频率计数
	uint32_t Frequnce;		//通道频率
	bool state;
} Loop_DefType;
/* Private define ------------------------------------------------------------*/
#define DET_CAN_ID  0x05
#define CPU_F		72000000


extern uint8_t Time_CanRespone_Count;

extern bool fault_canTest;

extern CAN_EV_TYPE ev_CanRespone;
extern COIL_STA_TYPE state_Coil;

#endif
