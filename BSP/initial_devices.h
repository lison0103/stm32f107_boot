/*******************************************************************************
* File Name          : initial_devices.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INITIAL_DEVICES_H
#define __INITIAL_DEVICES_H

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"  
#include "malloc.h" 
#include "ff.h"  
#include "exfuns.h"     
#include "usbh_usr.h" 
#include "24cxx.h"
#include "mb85rcxx.h"
#include "digital_led.h"
#include "ewdt.h"
#include "update.h"
#include "iap.h"
#include "rtc.h"
#ifdef GEC_CB_MAIN
#include "hw_test.h"
#include "freertos_lwip.h" 
#include "modbusTask.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifdef GEC_CB_MAIN
#define LED_TASK_PRIO			( tskIDLE_PRIORITY + 1 )
#define TCP_TASK_PRIO			( tskIDLE_PRIORITY + 3 )
#define DHCP_TASK_PRIO                  ( tskIDLE_PRIORITY + 4 ) 
#endif

/* Exported functions ------------------------------------------------------- */
void Bsp_Init(void);
void NVIC_Configuration(void);

extern USBH_HOST  USB_Host;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;



#endif /* __INITIAL_DEVICES_H */


/******************************  END OF FILE  *********************************/



