/*******************************************************************************
* File Name          : ewdt.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EWDT_H
#define __EWDT_H 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define EWDT_TOOGLE() 	GPIOC->ODR ^= 0X0001
#define EWDT_ON       PCout(0)

/* Exported functions ------------------------------------------------------- */
void power_on_bsp_check(void);
void EWDT_Drv_pin_config(void);


#endif  /* __EWDT_H */


/******************************  END OF FILE  *********************************/
