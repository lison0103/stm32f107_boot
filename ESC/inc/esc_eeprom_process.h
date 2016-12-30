/*******************************************************************************
* File Name          : esc_eeprom_process.h
* Author             : lison
* Version            : V1.0
* Date               : 06/13/2016
* Description        : This file contains prototype of esc eeprom process.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ESC_EEPROM_PROCESS_H
#define __ESC_EEPROM_PROCESS_H	

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* FAULT STORE */
#define ESC_ERROR_ADR 0u                                /* NO.0 page */
#define ESC_ERROR_BACKUP_ADR (64u * 2u)                 /* NO.2 page */
#define ESC_ERROR_NUM 80u

/* PARA STORE */
#define ESC_PARA_ADR (64u * 4u)                         /* NO.4 page */
#define ESC_PARA_BACKUP_ADR (64u * 28u)                 /* NO.28 page */
#define ESC_PARA_NUM (ESC_CB_PARAMETER_DATA_LEN)

/* FAULT RECORD STORE */
#define ESC_ERROR_RECORD_ADR  (64u * 52u)               /* NO.52 page */
#define ESC_ERROR_RECORD_NUM  2400u 

/* EVENT RECORD STORE */


/* Exported functions ------------------------------------------------------- */
void error_record_check(void);
void get_error_record_from_eeprom(void);
void write_error_record_to_eeprom(void);
void eeprom_task_init(void);
u8 Check_Error_Present_Memory(void);

#endif  /* __ESC_EEPROM_PROCESS_H */


/******************************  END OF FILE  *********************************/







