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
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void error_record_check(void);
void get_error_record_from_eeprom(void);
void write_error_record_to_eeprom(void);


#endif  /* __ESC_EEPROM_PROCESS_H */


/******************************  END OF FILE  *********************************/







