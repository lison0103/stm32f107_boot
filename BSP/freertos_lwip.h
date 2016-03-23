/*******************************************************************************
* File Name          : freertos_lwip.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FREERTOS_LWIP_H
#define __FREERTOS_LWIP_H

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32_eth_bsp.h"
#include "netconf.h"
#include "lwip/sockets.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void led_task(void *arg);
void TCPServer(void *arg);
void TCPClient(void *arg);



#endif /* __IAP_H */


/******************************  END OF FILE  *********************************/


