/*******************************************************************************
* File Name          : iap.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains prototype of stm32 in applacation programming functions.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IAP_H
#define __IAP_H

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"

/* Exported types ------------------------------------------------------------*/
/** Define a function type parameter **/
typedef  void (*iapfun)(void);				

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** First application starting address (stored in FLASH) **/
/** Reserved 0X08000000 ~ 0X0800FFFF space for Bootloader use (64KB) **/
#define FLASH_APP1_ADDR		0x08010000	

/* Exported functions ------------------------------------------------------- */
void iap_load_app(u32 appxaddr);			      
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	



#endif /* __IAP_H */


/******************************  END OF FILE  *********************************/




	

   
	   









































