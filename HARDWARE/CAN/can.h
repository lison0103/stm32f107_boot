/*******************************************************************************
* File Name          : can.h
* Author             : lison
* Version            : V1.0
* Date               : 09/30/2016
* Last modify date   : 10/09/2016
* Description        : This file contains prototype of can functions.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H
#define __CAN_H	 

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"

/* Exported types ------------------------------------------------------------*/
typedef struct  
{
  u16 msg_id;
  u8 msg_data[8];
  
}CAN_MSG;

  
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define CAN_FRAME_LEN   8
#define CAN_SEND_LEN    3*CAN_FRAME_LEN

#define CAN1_TEST_ID      0xfaf1

/* SF <---> CB */
#define CAN1TX_CONTROL_DATA_ID  0x0200u
#define CAN1TX_CONTROL_DATA_ID2  0x0202u
#define CAN1RX_SAFETY_DATA1_ID  0x0100u
#define CAN1RX_SAFETY_DATA3_ID  0x0114u

/* Parameter */
/* Parameter status Control -----> Safety */
#define CAN1TX_PARA_STATUS_ID  0x0500u
#define CAN1RX_PARA_STATUS_ID  0x0300u

/* C0001.bin Safety -----> Control */
#define CAN1RX_PARA_CONTROL_ID1  0x0301u
#define CAN1RX_PARA_CONTROL_ID2  0x03FAu

/* S0001.bin Safety -----> Control */
#define CAN1RX_PARA_SAFETY_ID1  0x0400u
#define CAN1RX_PARA_SAFETY_ID2  0x0440u

/* CS001.bin Control -----> Safety */
#define CAN1TX_PARA_CB_IN_SAFETY_ID1  0x0501u
#define CAN1TX_PARA_CB_IN_SAFETY_ID2  0x052Du

/* DS001.bin Control -----> Safety */
#define CAN1TX_PARA_DDU_IN_SAFETY_ID1  0x0530u
#define CAN1TX_PARA_DDU_IN_SAFETY_ID2  0x0531u


/* Exported functions ------------------------------------------------------- */
u8 CAN_Int_Init(CAN_TypeDef* CANx); 
u8 Can_Send_Msg(CAN_TypeDef* CANx,u32 exid,u8* msg,u8 len);			

extern u8 can1_receive;
extern u8 can2_receive;
extern u8 g_u8CAN1SendFail;

#endif /* __CAN_H */


/******************************  END OF FILE  *********************************/



















