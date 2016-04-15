/*******************************************************************************
* File Name          : can.h
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H
#define __CAN_H	 

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 CAN_Mode_Init(CAN_TypeDef* CANx,u8 mode);//CAN��ʼ�� 
void Can_Send_Msg(CAN_TypeDef* CANx,u8* msg,u8 len);				//��������
u8 Can_Receive_Msg(CAN_TypeDef* CANx,u8 *buf);					//��������

extern u8 can1_receive;
extern u8 can2_receive;

#endif /* __CAN_H */


/******************************  END OF FILE  *********************************/



















