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
//CAN����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.
#define CAN2_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
 
u8 can1_receive = 0;
u8 can2_receive = 0;


/*******************************************************************************
* Function Name  : CAN_Mode_Init
* Description    : None
*                  
* Input          : None
*                  None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//������Ϊ:36M/((8+9+1)*4)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 
//u8 CAN_Mode_Init(CAN_TypeDef* CANx,u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
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
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
            GPIO_Init(GPIOD, &GPIO_InitStructure);			//��ʼ��IO

            /* Configure CAN1 RX pin */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
            GPIO_Init(GPIOD, &GPIO_InitStructure);			//��ʼ��IO
            
            /* CAN1 register init */
            CAN_DeInit(CANx);
            
            /* Struct init*/
            CAN_StructInit(&CAN_InitStructure);
            
            /* CAN2 cell init */
            CAN_InitStructure.CAN_TTCM=DISABLE;			//��ʱ�䴥��ͨ��ģʽ  
            CAN_InitStructure.CAN_ABOM=DISABLE;			//����Զ����߹���	 
            CAN_InitStructure.CAN_AWUM=DISABLE;			//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
            CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;			//��ֹ�����Զ����� 
            CAN_InitStructure.CAN_RFLM=DISABLE;		 	//���Ĳ�����,�µĸ��Ǿɵ�  
            CAN_InitStructure.CAN_TXFP=DISABLE;			//���ȼ��ɱ��ı�ʶ������ 
            CAN_InitStructure.CAN_Mode= mode;	        //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; 
            //���ò�����
//            CAN_InitStructure.CAN_SJW=tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//            CAN_InitStructure.CAN_BS1=tbs1; 			//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
//            CAN_InitStructure.CAN_BS2=tbs2;				//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
//            CAN_InitStructure.CAN_Prescaler=brp;        //��Ƶϵ��(Fdiv)Ϊbrp+1	
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
   #error "Please select first the CAN Baudrate in Private defines in main.c "
#endif  /* CAN_BAUDRATE == 1000 */            
            
            /* Initializes the CAN1 */
            CAN_Init(CANx, &CAN_InitStructure);        	//��ʼ��CAN1 
            
            /* CAN1 filter init */
            CAN_FilterInitStructure.CAN_FilterNumber=0;	//������0
            CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//����λģʽ
            CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32λ��  
            
            //any id
//            CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;	//32λID
//            CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
//            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
//            CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;            
            
            //std id
//            CAN_FilterInitStructure.CAN_FilterIdHigh=(0x12)<<5;	//32λID
//            CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
//            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;//0x0000;//32λMASK  
//            CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xfffc;//0x0000; // ��ʶ������λ��λ�� 1������ƥ�䣬0�����ù���
            
            //ext id
            CAN_FilterInitStructure.CAN_FilterIdHigh=(u16)((0x1314) >> 13);	//32λID
            CAN_FilterInitStructure.CAN_FilterIdLow=(u16)(0x1314 << 3) | CAN_ID_EXT;
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;//32λMASK
            CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xfffc;              
            CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
            CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//���������0      
            
             CAN_FilterInit(&CAN_FilterInitStructure);	
             
#if CAN1_RX0_INT_ENABLE 
        /* IT Configuration for CAN1 */ 
	CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
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
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
            GPIO_Init(GPIOB, &GPIO_InitStructure);			//��ʼ��IO

            /* Configure CAN2 RX pin */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
            GPIO_Init(GPIOB, &GPIO_InitStructure);			//��ʼ��IO

            /* CAN2 register init */
            CAN_DeInit(CANx);
            
            /* Struct init*/
            CAN_StructInit(&CAN_InitStructure);            

            /* CAN2 cell init */
            CAN_InitStructure.CAN_TTCM=DISABLE;			//��ʱ�䴥��ͨ��ģʽ  
            CAN_InitStructure.CAN_ABOM=DISABLE;			//����Զ����߹���	 
            CAN_InitStructure.CAN_AWUM=DISABLE;			//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
            CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;			//��ֹ�����Զ����� 
            CAN_InitStructure.CAN_RFLM=DISABLE;		 	//���Ĳ�����,�µĸ��Ǿɵ�  
            CAN_InitStructure.CAN_TXFP=DISABLE;			//���ȼ��ɱ��ı�ʶ������ 
            CAN_InitStructure.CAN_Mode= mode;	        //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; 
            //���ò�����
//            CAN_InitStructure.CAN_SJW=tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//            CAN_InitStructure.CAN_BS1=tbs1; 			//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
//            CAN_InitStructure.CAN_BS2=tbs2;				//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
//            CAN_InitStructure.CAN_Prescaler=brp;        //��Ƶϵ��(Fdiv)Ϊbrp+1	

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
   #error "Please select first the CAN Baudrate in Private defines in main.c "
#endif  /* CAN_BAUDRATE == 1000 */             
            
            /* Initializes the CAN2 */
            CAN_Init(CANx, &CAN_InitStructure);        	//��ʼ��CAN1 

            /* CAN2 filter init */
            CAN_FilterInitStructure.CAN_FilterNumber=14;	//������0
            CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//����λģʽ
            CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32λ�� 
            CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;	//32λID
            CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
            CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
            CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
            CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//���������0

            CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
        
            
#if CAN2_RX0_INT_ENABLE
        /* IT Configuration for CAN2 */ 
	CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    

	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif             
                       
        
        }
	
           
	return 0;
}   
 
#if CAN1_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
void CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
        can1_receive = 1;
        CAN_Receive(CAN1, 0, &RxMessage);
}
#endif

#if CAN2_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
void CAN2_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
        can2_receive = 1;
        CAN_Receive(CAN2, 0, &RxMessage);
}
#endif


/*******************************************************************************
* Function Name  : CAN_Mode_Init
* Description    : None
*                  
* Input          : None
*                  None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
void Can_Send_Msg(CAN_TypeDef* CANx,u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x10;			// ��׼��ʶ�� 
	TxMessage.ExtId=0x1234;			// ������չ��ʾ�� 
//	TxMessage.IDE=CAN_Id_Standard; 	        // ��׼֡
        TxMessage.IDE=CAN_Id_Extended; 	        // ext id
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
	TxMessage.DLC=len;				// Ҫ���͵����ݳ���
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			          
	CAN_Transmit(CANx, &TxMessage);   	 
}


/*******************************************************************************
* Function Name  : CAN_Mode_Init
* Description    : None
*                  
* Input          : None
*                  None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 Can_Receive_Msg(CAN_TypeDef* CANx,u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
        if( CAN_MessagePending(CANx,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
        CAN_Receive(CANx, CAN_FIFO0, &RxMessage);//��ȡ����	
        for(i=0;i<8;i++)
        buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}














