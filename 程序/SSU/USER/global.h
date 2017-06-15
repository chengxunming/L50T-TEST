#ifndef __global_H
#define __global_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "delay.h"
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
	CIRCUIT_AC,
	CIRCUIT_DC,
	CIRCUIT_DS18B20
}CIRCUIT_TYPE;
//CAN通信测试状态
typedef enum  
{
	CAN_EV_None=0,
	CAN_EV_ResOk,
	CAN_EV_ResFail,
	CAN_EV_ResTimeOut,
	CAN_EV_ResReady,
}CAN_EV_TYPE;
//ADC采集状态
typedef enum  
{
	ADC_STA_Start=0,
	ADC_STA_Ok,
	ADC_STA_COL,
}ADC_STA_TYPE;
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
#define SSU_CAN_ID  0x03
#define CPU_F		72000000


extern uint8_t Time_CanRespone_Count;

extern bool fault_canTest;

extern CAN_EV_TYPE ev_CanRespone;
extern ADC_STA_TYPE state_ADC;

#endif
