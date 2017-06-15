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
typedef enum  
{
	CAN_EV_None=0,
	CAN_EV_ResOk,
	CAN_EV_ResFail,
	CAN_EV_ResTimeOut,
	CAN_EV_ResReady,
}CAN_EV_TYPE;
//通信帧定义
typedef struct
{
	uint8_t module_id;
	uint8_t fun_code;
	uint8_t dataH;
	uint8_t dataL;
	uint8_t check_sum;
}Frame_DefType;

/* Private define ------------------------------------------------------------*/
#define IOU_CAN_ID  0x06
#define CPU_F		72000000


extern uint8_t Time_CanRespone_Count;

extern bool fault_canTest;

extern CAN_EV_TYPE ev_CanRespone;

#endif
