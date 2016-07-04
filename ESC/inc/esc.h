/*******************************************************************************
* File Name          : esc.h
* Author             : lison
* Version            : V1.0
* Date               : 06/13/2016
* Description        : This file contains esc parameters.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ESC_H
#define __ESC_H	

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define EEP_ERROR_RECORD_ADR  10
#define EEP_ERROR_RECORD_NUM  2452  

/* Exported functions ------------------------------------------------------- */

extern u8 Modbuff[5000];
extern u8 EscRTBuff[200];
extern u8 *const pcEscErrorBuff;
extern u16 *const ptErrorRecordFirstItem;
extern u16 *const ptErrorRecordCounter;
extern u8 *const pcMbRtccBuff;
extern u8 *const pcEscErrorCodeBuff;
extern u16 *const pcStateBuff;
extern u32 *const plPowerOnTms;
extern u8 *const pcCurrentRecordErrorBuff;

#define ESC_ERROR_CODE pcEscErrorCodeBuff
#define SF_ESC_STATE      pcStateBuff[0]
#define POWER_ON_TMS    plPowerOnTms[0]
#define EN_ERROR1       Modbuff[600]


#define INPUT_PORT1_8       EscRTBuff[4]
#define INPUT_PORT9_16      EscRTBuff[5]

#define INPUT_PORT1_MASK        ( 1 << 0 )
#define INPUT_PORT2_MASK        ( 1 << 1 )
#define INPUT_PORT3_MASK        ( 1 << 2 )
#define INPUT_PORT4_MASK        ( 1 << 3 )
#define INPUT_PORT5_MASK        ( 1 << 4 )
#define INPUT_PORT6_MASK        ( 1 << 5 )
#define INPUT_PORT7_MASK        ( 1 << 6 )
#define INPUT_PORT8_MASK        ( 1 << 7 )

#define INPUT_PORT9_MASK         ( 1 << 0 )
#define INPUT_PORT10_MASK        ( 1 << 1 )
#define INPUT_PORT11_MASK        ( 1 << 2 )
#define INPUT_PORT12_MASK        ( 1 << 3 )
#define INPUT_PORT13_MASK        ( 1 << 4 )
#define INPUT_PORT14_MASK        ( 1 << 5 )
#define INPUT_PORT15_MASK        ( 1 << 6 )
#define INPUT_PORT16_MASK        ( 1 << 7 )


#define CMD_OUTPUT_PORT         EscRTBuff[30]
#define CMD_OUTPUT_PORT2        EscRTBuff[31]

#endif  /* __ESC_H */


/******************************  END OF FILE  *********************************/







