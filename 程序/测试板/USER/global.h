#ifndef __global_H
#define __global_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "bsp_led.h"
#include "bsp_can.h"
#include "bsp_key.h"
#include "bsp_mfu.h"
#include "bsp_time.h"
/* Private typedef -----------------------------------------------------------*/

//typedef enum  
//{
//	FALSE=0,
//	TRUE=1
//}BOOL;

typedef enum  
{
	CAN_TX_WAIT=0,
	CAN_TX_START,
	CAN_TX_OK,
}CANTX_STATE_TYPE;

typedef enum
{
	MFU_TEST_WAIT=0,
	MFU_TEST_START,
	MFU_TEST_OK
}MFUTEST_STATE_TYPE;
//单板类型定义
typedef enum  
{
	MD_NONE=0,
	MD_CPU=1,
	MD_COU=2,
	MD_SSU=3,
	MD_PDU=4,
	MD_DET=5,
	MD_IOU=6,
	MD_MAU=7,
	MD_MFU=8
}MODULE_TYPE;
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
	CIRCUIT_1=1,
	CIRCUIT_2,
	CIRCUIT_3,
	CIRCUIT_4,
	CIRCUIT_5,
	CIRCUIT_6,
	CIRCUIT_7,
	CIRCUIT_8,
	CIRCUIT_9,
	CIRCUIT_10
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
typedef struct
{
	bool errState;
	
}CircuitErr_DefType;

/* Private define ------------------------------------------------------------*/
#define CAN_ID  	0x02
#define CPU_F		72000000



extern MODULE_TYPE Module_Test;
extern MFUTEST_STATE_TYPE MFU_TestState;
extern uint8_t Time_KeyCheck_Count;
extern uint8_t Time_MFUCheck_Count;
extern bool FLAG_SYS_RUN;
extern bool FLAG_TEST_FINISH;
extern BuzzerHandle_DefType  ring;

extern uint8_t MFU_hs_State[2],MFU_Normal_State[2];


#endif
