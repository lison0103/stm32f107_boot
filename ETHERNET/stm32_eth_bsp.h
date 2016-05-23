/*******************************************************************************
* File Name          : stm32_eth_bsp.h
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : This file contains prototype of stm32 ethernet low level functions.
*                      
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_ETH_BSP_H__
#define __STM32_ETH_BSP_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f107_eth_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
 extern "C" {
#endif

//#define MII_MODE          /* MII mode  */
#define RMII_MODE       /* RMII mode  */

/* Reset pin */   
#define KSZ8041NL_RST_ON() 		   GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET) 	
#define KSZ8041NL_RST_OFF() 		   GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET) 
   
void  ETH_BSP_Config(void);
uint32_t Eth_Link_PHYITConfig(uint16_t PHYAddress);
void Eth_Link_EXTIConfig(void);
void Eth_Link_ITHandler(uint16_t PHYAddress);

#ifdef __cplusplus
 }
#endif

#endif //__STM32_ETH_BSP_H__


/******************************  END OF FILE  *********************************/