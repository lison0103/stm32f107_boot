/*******************************************************************************
* File Name          : led.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"	
#include "lsys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define USART3_TRX_EN         	1

#ifdef USART3_TRX_EN
#define USART3_TRX_GPIO        	GPIOB
#define USART3_TRX_RCC         	RCC_APB2Periph_GPIOB
#define USART3_TRX_PIN        	GPIO_Pin_14	
#define USART3_TRX_CONTROL      PBout(14)
#endif

/* Exported functions ------------------------------------------------------- */
void USART3_Init(void);
void USART3_SEND(u8 * str,int len);
void USART2_Init(void);
uint32_t BSP_USART_Receive(USART_TypeDef* USARTx,uint8_t *buff,uint32_t mlen);
void BSP_USART_Send(USART_TypeDef* USARTx,uint8_t *buff,uint32_t len);
 

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	


#endif  /* __USART_H */


/******************************  END OF FILE  *********************************/




