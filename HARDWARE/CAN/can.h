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
#define canbuffsize 100

/* Exported functions ------------------------------------------------------- */
u8 CAN_Mode_Init(CAN_TypeDef* CANx,u8 mode);//CAN��ʼ�� 
u8 Can_Send_Msg(CAN_TypeDef* CANx,u32 exid,u8* msg,u8 len);			//��������
u8 Can_Receive_Msg(CAN_TypeDef* CANx,u8 *buf);					//��������

/* CAN1 */
extern u8 CAN1_TX_Buff[canbuffsize];
extern u8 CAN1_RX_Buff[canbuffsize];
extern u8 CAN1_TX_Data[canbuffsize];
extern u8 CAN1_RX_Data[canbuffsize];
/* CAN2 */
extern u8 CAN2_TX_Buff[canbuffsize];
extern u8 CAN2_RX_Buff[canbuffsize];
extern u8 CAN2_TX_Data[canbuffsize];
extern u8 CAN2_RX_Data[canbuffsize];

extern u8 can1_receive;
extern u8 can2_receive;
extern u8 can1_data_packet;
extern u8 can1_recv_len;

#endif /* __CAN_H */


/******************************  END OF FILE  *********************************/



















