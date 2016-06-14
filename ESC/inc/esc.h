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
#define EEP_ERROR_RECORD_ITEM_ADR  996
#define EEP_ERROR_RECORD_ADR  1000
#define EEP_ERROR_RECORD_NUM  2400  

/* Exported functions ------------------------------------------------------- */

extern u8 Modbuff[3000];
extern u8 EscErrorBuff[EEP_ERROR_RECORD_NUM];
extern u16 *const ptErrorRecordFirstItem;
extern u16 *const ptErrorRecordCounter;
extern u8 *const pcMbRtccBuff;
extern u8 *const pcEscErrorCodeBuff;
extern u8 *const pcStateBuff;
extern u32 *const plPowerOnTms;

#define ESC_ERROR_CODE pcEscErrorCodeBuff
#define ESC_STATE1      pcStateBuff[0]
#define POWER_ON_TMS    plPowerOnTms[0]
#define EN_ERROR1       Modbuff[600]

#endif  /* __ESC_H */


/******************************  END OF FILE  *********************************/







