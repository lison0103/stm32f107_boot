/*******************************************************************************
* File Name          : sys_task.h
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SYS_TASK_H
#define _SYS_TASK_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void rx485_test_init(void);
void rtc_clock_init(void);
void led_ewdt_init(void);
void self_check_init(void);

#endif /* _SYS_TASK_H */


/******************************  END OF FILE  *********************************/



