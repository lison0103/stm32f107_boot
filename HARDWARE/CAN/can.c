/*******************************************************************************
* File Name          : can.c
* Author             : lison
* Version            : V1.0
* Date               : 09/30/2016
* Last modify date   : 10/09/2016
* Description        : This file contains can functions.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "led.h"
#include "delay.h"
#include "crc16.h"
#include "esc.h"
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define CAN_BAUDRATE  1000      /* 1MBps   */
//#define CAN_BAUDRATE  500  /* 500kBps */
#define CAN_BAUDRATE  250  /* 250kBps */
//#define CAN_BAUDRATE  125  /* 125kBps */
//#define CAN_BAUDRATE  100  /* 100kBps */ 
//#define CAN_BAUDRATE  50   /* 50kBps  */ 
//#define CAN_BAUDRATE  20   /* 20kBps  */ 
//#define CAN_BAUDRATE  10   /* 10kBps  */

/* Private macro -------------------------------------------------------------*/
#define CAN1_RX0_INT_ENABLE	1		
#define CAN2_RX0_INT_ENABLE	1		


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Can_Queue_Data(u8 rxmsg[], u16 canid, BaseType_t *TaskWoken);

CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
u8 can1_receive = 0;
u8 can2_receive = 0;

u8 g_u8CAN1SendFail = 0u;

/*******************************************************************************
* Function Name  : CAN_Int_Init
* Description    : Initialization can.
* CAN_SJW: CAN_SJW_1tq~ CAN_SJW_4tq
* CAN_BS2: CAN_BS2_1tq~CAN_BS2_8tq;
* CAN_BS1: CAN_BS1_1tq ~CAN_BS1_16tq
* CAN_Prescaler: 1~1024;  tq=(brp)*tpclk1
* baud rate = Fpclk1/((tbs1+1+tbs2+1+1)*brp)
* if Fpclk is 36M, baud rate:36M/((1+3+2)*24)=250Kbps               
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
u8 CAN_Int_Init(CAN_TypeDef* CANx)
{ 
	GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
        CAN_FilterInitTypeDef  	CAN_FilterInitStructure2;
#if CAN1_RX0_INT_ENABLE || CAN2_RX0_INT_ENABLE
	NVIC_InitTypeDef  		NVIC_InitStructure;
#endif

        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        
        if(CANx == CAN1)
        {	                   											 
            /* Remap CAN1 GPIOs */
            GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE);

            /* Configure CAN1 TX pin */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
            GPIO_Init(GPIOD, &GPIO_InitStructure);			

            /* Configure CAN1 RX pin */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
            GPIO_Init(GPIOD, &GPIO_InitStructure);			
            
            /* CAN1 register init */
            CAN_DeInit(CANx);
            
            /* Struct init*/
            CAN_StructInit(&CAN_InitStructure);
            
            /*  non-time-triggered communication mode */
            CAN_InitStructure.CAN_TTCM=DISABLE;			
            /* automatic offline management software */
            CAN_InitStructure.CAN_ABOM=ENABLE;//DISABLE;				 
            /* wake-sleep mode via software (Clear CAN-> MCR's SLEEP bit) */
            CAN_InitStructure.CAN_AWUM=ENABLE;			
            /* message is automatically transferred, in accordance with the CAN standard, */
            /* CAN hardware failure when sending packets would have been automatic retransmission until sent successfully */
            CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;	
            /* message is not locked, the new over the old one */
            CAN_InitStructure.CAN_RFLM=DISABLE;		 	
            /* priority is determined by the packet identifier */
            CAN_InitStructure.CAN_TXFP=ENABLE;			
            CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;
	
            /* set baud rate */
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
            CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;   
            
#if CAN_BAUDRATE == 1000 /* 1MBps */
            CAN_InitStructure.CAN_Prescaler =6;
#elif CAN_BAUDRATE == 500 /* 500KBps */
            CAN_InitStructure.CAN_Prescaler =12;
#elif CAN_BAUDRATE == 250 /* 250KBps */
            CAN_InitStructure.CAN_Prescaler =24;
#elif CAN_BAUDRATE == 125 /* 125KBps */
            CAN_InitStructure.CAN_Prescaler =48;
#elif  CAN_BAUDRATE == 100 /* 100KBps */
            CAN_InitStructure.CAN_Prescaler =60;
#elif  CAN_BAUDRATE == 50 /* 50KBps */
            CAN_InitStructure.CAN_Prescaler =120;
#elif  CAN_BAUDRATE == 20 /* 20KBps */
            CAN_InitStructure.CAN_Prescaler =300;
#elif  CAN_BAUDRATE == 10 /* 10KBps */
            CAN_InitStructure.CAN_Prescaler =600;
#else
#error "Please select first the CAN Baudrate in Private defines "
#endif  /* CAN_BAUDRATE == 1000 */            
            
            /* Initializes the CAN1 */
            CAN_Init(CANx, &CAN_InitStructure);        	
            
            /* CAN1 filter init */
            /* 16 bit mask: STDID[10:0], IDE, RTR, EXTDID[17:15] */
            /* 32 bit mask: STDID[10:0], EXTDID[17:0], IDE, RTR,0 */            
            CAN_FilterInitStructure.CAN_FilterNumber=0;	
            CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	
            CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	
            
            //any id
            CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;	//32-bit ID
            CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32-bit MASK
            CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;            

            CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
            CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;                 
            CAN_FilterInit(&CAN_FilterInitStructure);	
             
#if CAN1_RX0_INT_ENABLE 
            /* IT Configuration for CAN1 */ 
            /* FIFO 0 message pending Interrupt £¬full Interrupt */
            CAN_ITConfig(CAN1,CAN_IT_FMP0 , ENABLE); 		    
            CAN_ITConfig(CAN1,CAN_IT_FF0, ENABLE); 		    

            /* FIFO 1 message pending Interrupt £¬full Interrupt */
            CAN_ITConfig(CAN1,CAN_IT_FMP1 , ENABLE); 		    
            CAN_ITConfig(CAN1,CAN_IT_FF1, ENABLE); 		                

            NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;    
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            
            NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);            
#endif             
            CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);                
            /* Enable CAN1 TX0 interrupt IRQ channel */
            NVIC_InitStructure.NVIC_IRQChannel = (u8)CAN1_TX_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5u;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0u;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);                         
            
        }
        else if(CANx == CAN2)
        {	                   											 	            
            /* Remap CAN2 GPIOs */
            GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);

            /* Configure CAN2 TX pin */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
            GPIO_Init(GPIOB, &GPIO_InitStructure);			

            /* Configure CAN2 RX pin */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
            GPIO_Init(GPIOB, &GPIO_InitStructure);			

            /* CAN2 register init */
            CAN_DeInit(CANx);
            
            /* Struct init*/
            CAN_StructInit(&CAN_InitStructure);            

            /* CAN2 cell init */
            CAN_InitStructure.CAN_TTCM=DISABLE;		  
            CAN_InitStructure.CAN_ABOM=ENABLE;				 
            CAN_InitStructure.CAN_AWUM=ENABLE;			
            CAN_InitStructure.CAN_NART=DISABLE;			 
            CAN_InitStructure.CAN_RFLM=DISABLE;		 	  
            CAN_InitStructure.CAN_TXFP=ENABLE;			 
            CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;
	
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
            CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;  
            
#if CAN_BAUDRATE == 1000 /* 1MBps */
            CAN_InitStructure.CAN_Prescaler =6;
#elif CAN_BAUDRATE == 500 /* 500KBps */
            CAN_InitStructure.CAN_Prescaler =12;
#elif CAN_BAUDRATE == 250 /* 250KBps */
            CAN_InitStructure.CAN_Prescaler =24;
#elif CAN_BAUDRATE == 125 /* 125KBps */
            CAN_InitStructure.CAN_Prescaler =48;
#elif  CAN_BAUDRATE == 100 /* 100KBps */
            CAN_InitStructure.CAN_Prescaler =60;
#elif  CAN_BAUDRATE == 50 /* 50KBps */
            CAN_InitStructure.CAN_Prescaler =120;
#elif  CAN_BAUDRATE == 20 /* 20KBps */
            CAN_InitStructure.CAN_Prescaler =300;
#elif  CAN_BAUDRATE == 10 /* 10KBps */
            CAN_InitStructure.CAN_Prescaler =600;
#else
#error "Please select first the CAN Baudrate in Private defines "
#endif  /* CAN_BAUDRATE == 1000 */             
            
            /* Initializes the CAN2 */
            CAN_Init(CANx, &CAN_InitStructure);        	

            /* CAN2 filter init */
            CAN_FilterInitStructure2.CAN_FilterNumber=14;	
            CAN_FilterInitStructure2.CAN_FilterMode=CAN_FilterMode_IdMask; 	
            CAN_FilterInitStructure2.CAN_FilterScale=CAN_FilterScale_32bit; 	
            
            //any id
            CAN_FilterInitStructure2.CAN_FilterIdHigh=0x0000;	//32-bit ID
            CAN_FilterInitStructure2.CAN_FilterIdLow=0x0000;
            CAN_FilterInitStructure2.CAN_FilterMaskIdHigh=0x0000;//32-bit MASK
            CAN_FilterInitStructure2.CAN_FilterMaskIdLow=0x0000;  
            
            CAN_FilterInitStructure2.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
            CAN_FilterInitStructure2.CAN_FilterActivation=ENABLE;
            CAN_FilterInit(&CAN_FilterInitStructure2);			
        
            
#if CAN2_RX0_INT_ENABLE
            /* IT Configuration for CAN2 */ 
            CAN_ITConfig(CAN2,CAN_IT_FMP0, ENABLE); 						    
            CAN_ITConfig(CAN2,CAN_IT_FF0, ENABLE); 						    
            CAN_ITConfig(CAN2,CAN_IT_FOV0, ENABLE); 						    

            NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;     
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
#endif             
                       
        
        }
	
           
	return 0;
}   

u32 g_u32Test= 0u;
/*******************************************************************************
* Function Name  : CAN1_RX0_IRQHandler
* Description    : This function handles CAN1 RX0 interrupt request.                
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if CAN1_RX0_INT_ENABLE	
			    
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    if( CAN_GetITStatus(CAN1,CAN_IT_FF0) != RESET)
    {
        /* Clears the CAN1 interrupt pending bit */
        CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
        
        /* CAN filter init "FIFO1 " */
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 1u;
        CAN_FilterInit(&CAN_FilterInitStructure); 
    }
    else
    {       
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        
        /** SF data RECEIVE **/
        if(( RxMessage.ExtId >= CAN1RX_SAFETY_DATA1_ID ) && ( RxMessage.ExtId <= CAN1RX_SAFETY_DATA3_ID ))
        {
            if( ( RxMessage.DLC == CAN_FRAME_LEN ) && ( RxMessage.IDE == CAN_ID_EXT ))
            {
                can1_receive = 1;            
                g_u32Test |= ( 1u << ( RxMessage.ExtId - CAN1RX_SAFETY_DATA1_ID ));
                Can_Queue_Data(RxMessage.Data, RxMessage.ExtId, &xHigherPriorityTaskWoken);
            }
        }
        /** Para data RECEIVE **/
        if((( RxMessage.ExtId >= CAN1RX_PARA_STATUS_ID ) && ( RxMessage.ExtId <= CAN1RX_PARA_CONTROL_ID2 ))
           || (( RxMessage.ExtId >= CAN1RX_PARA_SAFETY_ID1 ) && ( RxMessage.ExtId <= CAN1RX_PARA_SAFETY_ID2 )))
        {
            if( ( RxMessage.DLC == CAN_FRAME_LEN ) && ( RxMessage.IDE == CAN_ID_EXT ))
            {
                can1_receive = 1;   
                Can_Queue_Data(RxMessage.Data, RxMessage.ExtId, &xHigherPriorityTaskWoken);
            }
        }        
        /* Test Mode */        
        else if( ( RxMessage.ExtId == CAN1_TEST_ID ) && ( RxMessage.IDE == CAN_ID_EXT ) )
        {
            can1_receive = 1;        
            
        } 
        
        CAN_FIFORelease(CAN1,CAN_FIFO0);
    }
    
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*******************************************************************************
* Function Name  : CAN1_RX0_IRQHandler
* Description    : This function handles CAN1 RX0 interrupt request.                
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN1_RX1_IRQHandler(void)
{ 
    CanRxMsg RxMessage;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    if(CAN_GetFlagStatus(CAN1,CAN_FLAG_FF1) != RESET)
    {     
        /* Clears the CAN1 interrupt pending bit */    
        CAN_ClearITPendingBit(CAN1,CAN_IT_FF1);
        
        /* CAN filter init "FIFO0" */
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
        CAN_FilterInit(&CAN_FilterInitStructure); 
    }
    else
    {       
        CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
        
        /** SF data RECEIVE **/
        if(( RxMessage.ExtId >= CAN1RX_SAFETY_DATA1_ID ) && ( RxMessage.ExtId <= CAN1RX_SAFETY_DATA3_ID ))
        {
            if( ( RxMessage.DLC == CAN_FRAME_LEN ) && ( RxMessage.IDE == CAN_ID_EXT ))
            {
                can1_receive = 1;            
                g_u32Test |= ( 1u << ( RxMessage.ExtId - CAN1RX_SAFETY_DATA1_ID ));
                Can_Queue_Data(RxMessage.Data, RxMessage.ExtId, &xHigherPriorityTaskWoken);
            }
        }
        /** Para data RECEIVE **/
        if((( RxMessage.ExtId >= CAN1RX_PARA_STATUS_ID ) && ( RxMessage.ExtId <= CAN1RX_PARA_CONTROL_ID2 ))
           || (( RxMessage.ExtId >= CAN1RX_PARA_SAFETY_ID1 ) && ( RxMessage.ExtId <= CAN1RX_PARA_SAFETY_ID2 )))
        {
            if( ( RxMessage.DLC == CAN_FRAME_LEN ) && ( RxMessage.IDE == CAN_ID_EXT ))
            {
                can1_receive = 1;            
                Can_Queue_Data(RxMessage.Data, RxMessage.ExtId, &xHigherPriorityTaskWoken);
            }
        }        
        /* Test Mode */        
        else if( ( RxMessage.ExtId == CAN1_TEST_ID ) && ( RxMessage.IDE == CAN_ID_EXT ) )
        {
            can1_receive = 1;        
            
        } 
        
        CAN_FIFORelease(CAN1,CAN_FIFO1);
    }    
    
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif


/*******************************************************************************
* Function Name  : CAN2_RX0_IRQHandler
* Description    : This function handles CAN2 RX0 interrupt request.                 
* Input          : None             
* Output         : None
* Return         : None
*******************************************************************************/
#if CAN2_RX0_INT_ENABLE	
			    
void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
        
    if( CAN_GetITStatus(CAN2,CAN_IT_FF0) != RESET)
    {
        CAN_ClearITPendingBit(CAN2,CAN_IT_FF0);
    }
    else if( CAN_GetITStatus(CAN2,CAN_IT_FOV0) != RESET)
    {
        CAN_ClearITPendingBit(CAN2,CAN_IT_FOV0);
    }
    else
    {
        
        CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
        if( ( RxMessage.ExtId == 0x3234 ) && ( RxMessage.IDE == CAN_ID_EXT ) )
        {
            can2_receive = 1;        
            for( u8 i = 0; i < RxMessage.DLC; i++ )
            {
                /* receive data */

            }
        }
        /* Test Mode */        
        else if( ( RxMessage.ExtId == CAN1_TEST_ID ) && ( RxMessage.IDE == CAN_ID_EXT ) )
        {
            can2_receive = 1;        
            
        }   
        
        CAN_FIFORelease(CAN2,CAN_FIFO0);
    }        
}
#endif


/*******************************************************************************
* Function Name  : CAN1_TX_IRQHandler
* Description    : This function handles CAN1 TX interrupt request.               
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void CAN1_TX_IRQHandler(void)
{
    u8 result;
    
    CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP0);
    CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);
    
    if( g_u8CAN1SendFail & 0x01u )
    {
        result = Can_Send_Msg(CAN1, CAN1TX_CONTROL_DATA_ID, &EscDataToSafety[0][0], CAN_FRAME_LEN ); 
        if( result )
        {
            /* No mail box, send fail */
            g_u8CAN1SendFail |= 0x01u;
            CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);               
        }
        else
        {
            g_u8CAN1SendFail &= ~0x01u;
        }
    }
    
    if( g_u8CAN1SendFail & 0x02u )
    {
        result = Can_Send_Msg(CAN1, CAN1TX_CONTROL_DATA_ID+1u, &EscDataToSafety[1][0], CAN_FRAME_LEN ); 
        if( result )
        {
            /* No mail box, send fail */
            g_u8CAN1SendFail |= 0x02u;
            CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);        
        } 
        else
        {
            g_u8CAN1SendFail &= ~0x02u;
        }
    }
#if 0    
    if( g_u8CAN1SendFail & 0x04u )
    {
        result = Can_Send_Msg(CAN1, CAN1TX_CONTROL_DATA_ID+2u, &EscDataToSafety[2][0], CAN_FRAME_LEN );  
        if( result )
        {
            /* No mail box, send fail */
            g_u8CAN1SendFail |= 0x04u;
            CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
        } 
        else
        {
            g_u8CAN1SendFail &= ~0x04u;
        }
    }
#endif    
}

/*******************************************************************************
* Function Name  : Can_Send_Msg
* Description    : CAN send data
*                  
* Input          : len: data len(max len is 8)
*                  msg: Data Pointer.
*                  exid: Extended identifier ID.
*                  CANx: CAN1 or CAN2
* Output         : None
* Return         : 0: success
*                  1: fail, no send mailbox 
*******************************************************************************/		 
u8 Can_Send_Msg(CAN_TypeDef* CANx,u32 exid,u8* msg,u8 len)
{	
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x10;			// STD ID
	TxMessage.ExtId=exid;			// EXT ID 
//	TxMessage.IDE=CAN_Id_Standard; 	        
        TxMessage.IDE=CAN_Id_Extended; 	        
	TxMessage.RTR=CAN_RTR_Data;		
	TxMessage.DLC=len;			
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			          
	if( CAN_TxStatus_NoMailBox == CAN_Transmit(CANx, &TxMessage) )
        {
            return 1;
        }
        
        return 0;
}


/*******************************************************************************
* Function Name  : Can_Receive_Msg
* Description    : CAN receive data                 
* Input          : buf:data cache
* Output         : None
* Return         : 0: no data receive; 
*                  other: Length of the received data;
*******************************************************************************/
u8 Can_Receive_Msg(CAN_TypeDef* CANx,u8 *buf)
{		   		   
      u32 i;
      CanRxMsg RxMessage;
      if( CAN_MessagePending(CANx,CAN_FIFO0)==0)return 0;		 
      
      CAN_Receive(CANx, CAN_FIFO0, &RxMessage);	
      for(i=0;i<8;i++)
      {
          buf[i]=RxMessage.Data[i];  
      }
      return RxMessage.DLC;	
}


/*******************************************************************************
* Function Name  : Can_Receive_Data
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
static void Can_Queue_Data(u8 rxmsg[], u16 canid, BaseType_t *TaskWoken)
{	
    BaseType_t xStatus;  
    CAN_MSG DataFromSF;
    u8 num;
    
    DataFromSF.msg_id = canid;
    for( num = 0u; num < 8u; num++ )
    {
        DataFromSF.msg_data[num] = rxmsg[num];
    } 
    
    if( xQueueIsQueueFullFromISR(xQueue) == pdFALSE )
    {
        xStatus = xQueueSendFromISR( xQueue, &DataFromSF, TaskWoken ); 
        if( xStatus != pdPASS ) 
        { 
            /* error */
            EN_ERROR48 |= 0x04u;
        }  
    }
    else
    {
        /* errQUEUE_FULL */
        EN_ERROR48 |= 0x04u;
    }
}


/******************************  END OF FILE  *********************************/












