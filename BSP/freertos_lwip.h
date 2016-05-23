/*******************************************************************************
* File Name          : freertos_lwip.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains prototype of lwip tcp demo.
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
void tcp_client_init(void);
void tcp_server_init(void);
void lwip_dhcp_init(void);


#endif /* __IAP_H */


/******************************  END OF FILE  *********************************/


