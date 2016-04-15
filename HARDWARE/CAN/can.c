/*******************************************************************************
* File Name          : can.c
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : 
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "led.h"
#include "delay.h"

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
//CAN接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.
#define CAN2_RX0_INT_ENABLE	1		//0,不使能;1,使能.

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
 
u8 can1_receive = 0;
u8 can2_receive = 0;


/*******************************************************************************
* Function Name  : CAN_Mode_Init
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
/** CAN init
tsjw:CAN_SJW_1tq~ CAN_SJW_4tq
tbs2:CAN_BS2_1tq~CAN_BS2_8tq;
tbs1:CAN_BS1_1tq ~CAN_BS1_16tq
brp:1~1024;  tq=(brp)*tpclk1
baud rate=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
mode:CAN_Mode_Normal;CAN_Mode_LoopBack;
if Fpclk is 36M,CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
baud rate:36M/((8+9+1)*4)=500Kbps **/
u8 CAN_Mode_Init(CAN_TypeDef* CANx,u8 mode)
{ 
	GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
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
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
            GPIO_Init(GPIOD, &GPIO_InitStructure);			//初始化IO

            /* Configure CAN1 RX pin */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
            GPIO_Init(GPIOD, &GPIO_InitStructure);			//初始化IO
            
            /* CAN1 register init */
            CAN_DeInit(CANx);
            
            /* Struct init*/
            CAN_StructInit(&CAN_InitStructure);
            
            /* CAN2 cell init */
            CAN_InitStructure.CAN_TTCM=DISABLE;			//非时间触发通信模式  
            CAN_InitStructure.CAN_ABOM=DISABLE;			//软件自动离线管理	 
            CAN_InitStructure.CAN_AWUM=DISABLE;			//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
            CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;			//禁止报文自动传送 
            CAN_InitStructure.CAN_RFLM=DISABLE;		 	//报文不锁定,新的覆盖旧的  
            CAN_InitStructure.CAN_TXFP=DISABLE;			//优先级由报文标识符决定 
            CAN_InitStructure.CAN_Mode= mode;	        //模式设置： mode:0,普通模式;1,回环模式; 
	
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
            CAN_Init(CANx, &CAN_InitStructure);        	//初始化CAN1 
            
            /* CAN1 filter init */
            CAN_FilterInitStructure.CAN_FilterNumber=0;	//过滤器0
            CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//屏蔽位模式
            CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32位宽  
            
            //any id
//            CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;	//32位ID
//            CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
//            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
//            CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;            
            
            //std id
//            CAN_FilterInitStructure.CAN_FilterIdHigh=(0x12)<<5;	//32位ID
//            CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
//            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;//0x0000;//32位MASK  
//            CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xfffc;//0x0000; // 标识符屏蔽位，位： 1：必须匹配，0：不用关心
            
            //ext id
            CAN_FilterInitStructure.CAN_FilterIdHigh=(((u32)0x1314<<3)&0xFFFF0000)>>16;	
            CAN_FilterInitStructure.CAN_FilterIdLow=(((u32)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;//32位MASK
            CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xfccf;              
            CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
            CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//激活过滤器0      
            
            CAN_FilterInit(&CAN_FilterInitStructure);	
             
#if CAN1_RX0_INT_ENABLE 
            /* IT Configuration for CAN1 */ 
            /* FIFO 0 message pending Interrupt ，full Interrupt ， overrun Interrupt */
            CAN_ITConfig(CAN1,CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE); 		    

            NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
#endif             
                        
            
        }
        else if(CANx == CAN2)
        {	                   											 	            
            /* Remap CAN2 GPIOs */
            GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);

            /* Configure CAN2 TX pin */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
            GPIO_Init(GPIOB, &GPIO_InitStructure);			//初始化IO

            /* Configure CAN2 RX pin */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
            GPIO_Init(GPIOB, &GPIO_InitStructure);			//初始化IO

            /* CAN2 register init */
            CAN_DeInit(CANx);
            
            /* Struct init*/
            CAN_StructInit(&CAN_InitStructure);            

            /* CAN2 cell init */
            CAN_InitStructure.CAN_TTCM=DISABLE;			//非时间触发通信模式  
            CAN_InitStructure.CAN_ABOM=DISABLE;			//软件自动离线管理	 
            CAN_InitStructure.CAN_AWUM=DISABLE;			//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
            CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;			//禁止报文自动传送 
            CAN_InitStructure.CAN_RFLM=DISABLE;		 	//报文不锁定,新的覆盖旧的  
            CAN_InitStructure.CAN_TXFP=DISABLE;			//优先级由报文标识符决定 
            CAN_InitStructure.CAN_Mode= mode;	        //模式设置： mode:0,普通模式;1,回环模式; 
	
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
            CAN_Init(CANx, &CAN_InitStructure);        	//初始化CAN1 

            /* CAN2 filter init */
            CAN_FilterInitStructure.CAN_FilterNumber=14;	//过滤器0
            CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//屏蔽位模式
            CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32位宽 
            CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;	//32位ID
            CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
            CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
            CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
            CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//激活过滤器0

            CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
        
            
#if CAN2_RX0_INT_ENABLE
            /* IT Configuration for CAN2 */ 
            CAN_ITConfig(CAN1,CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE); 						    

            NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
#endif             
                       
        
        }
	
           
	return 0;
}   


/*******************************************************************************
* Function Name  : CAN1_RX0_IRQHandler
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
#if CAN1_RX0_INT_ENABLE	//enable CAN1 RX0 interrupt
			    
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
    
    if( CAN_GetITStatus(CAN1,CAN_IT_FF0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
    }
    else if( CAN_GetITStatus(CAN1,CAN_IT_FOV0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);
    }
    else
    {
        
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        /** SF urge data RECEIVE **/
        if( ( RxMessage.ExtId == 0x1234 ) && ( RxMessage.IDE == CAN_ID_EXT ) )
        {
            can1_receive = 1;        
            for( u8 i = 0; i < RxMessage.DLC; i++ )
            {
                /* receive data */
                printf("CAN1_RX0[%d]:%d\r\n",i,RxMessage.Data[i]);
            }
        }
        /** SF normal data RECEIVE **/
        else if( ( RxMessage.ExtId == 0x1314 ) && ( RxMessage.IDE == CAN_ID_EXT ) )
        {
            can1_receive = 1;        
            for( u8 i = 0; i < RxMessage.DLC; i++ )
            {
                /* receive data */
                printf("CAN1_RX0[%d]:%d\r\n",i,RxMessage.Data[i]);
            }
        } 
    }
}
#endif


/*******************************************************************************
* Function Name  : CAN2_RX0_IRQHandler
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
#if CAN2_RX0_INT_ENABLE	//enable CAN2 RX0 interrupt
			    
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
    else if( CAN_GetITStatus(CAN2,CAN_FLAG_FMP0) != RESET )
    {
        CAN_ClearITPendingBit(CAN2,CAN_FLAG_FMP0);
        
        CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
        if( ( RxMessage.ExtId == 0x3234 ) && ( RxMessage.IDE == CAN_ID_EXT ) )
        {
            can2_receive = 1;        
            for( u8 i = 0; i < RxMessage.DLC; i++ )
            {
                /* receive data */
                printf("CAN1_RX0[%d]:%d\r\n",i,RxMessage.Data[i]);
            }
        }
    }        
}
#endif



/*******************************************************************************
* Function Name  : Can_Send_Msg
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
/** CAN send data
len:data len(max len is 8)				     
msg:Data Pointer.
return:0,success;
**/		 
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
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
/** CAN receive data
buf:data cache;	 
return:0,no data receive;
other,Length of the received data;
**/
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


/******************************  END OF FILE  *********************************/












