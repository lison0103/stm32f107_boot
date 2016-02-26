#include "lsys.h"
#include "usart.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if EN_USART3_PRINTF
//#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 
//
//}; 
//
//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
    USART3->DR = (u8) ch;      
	return ch;
}
#else

int fputc(int ch, FILE *f)
{      
      return ch;
}

#endif 

 

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);	//GPIOC AFIOʱ��
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);          //ʹ��USART3
        
        GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
        
        
 	USART_DeInit(USART3);  //��λ����3
	 //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PB10
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);  //��ʼ��PB11
    
#ifdef USART3_TRX_EN
    RCC_APB2PeriphClockCmd(USART3_TRX_RCC, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin = USART3_TRX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART3_TRX_GPIO , &GPIO_InitStructure);
    
    USART3_TRX_CONTROL = 0;
#endif    
    

   //Usart3 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);//�����ж�
    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

void USART3_SEND(u8 * str,int len)
{
    USART3_TRX_CONTROL = 1;
    delay_ms(1);
    
    for(int t = 0;t < len; t++)
    {      
      USART3->DR = (u8) str[t];
      while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
       
    }
    
//    if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
//    {
        USART3_TRX_CONTROL = 0;
        delay_ms(1);
//    }
}

int a;
#if EN_USART3_RX   //���ʹ���˽���
//void USART3_IRQHandler(void)                	//����1�жϷ������
//{
//  u8 Res;
//  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//  {
// 
//      Res =USART_ReceiveData(USART3);
//      if((USART_RX_STA&0x8000)==0)//����δ���
//      {
//          if(USART_RX_STA&0x4000)//���յ���0x0d
//          {
//              if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//              else USART_RX_STA|=0x8000;	//��������� 
//          }
//          else //��û�յ�0X0D
//          {	
//              if(Res==0x0d)USART_RX_STA|=0x4000;
//              else
//              {
//                  USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//                  USART_RX_STA++;
//                  if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//              }		 
//          }
//      }      
//      
//  }
//} 
#endif	

