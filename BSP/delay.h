/*******************************************************************************
* File Name          : delay.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : Contains the prototype of timing delays.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H 

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Delay_Init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);


#endif  /* __DELAY_H */


/******************************  END OF FILE  *********************************/




