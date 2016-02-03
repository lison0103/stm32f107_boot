#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"  
#include "malloc.h" 
#include "ff.h"  
#include "exfuns.h"     
#include "usbh_usr.h" 
#include "24cxx.h"
#include "mb85rcxx.h"
#include "digital_led.h"
#include "ewdt.h"
#include "update.h"
#include "can.h"
#include "hw_test.h"

void can_test(void);
void can1_can2_test(void);
void hw_can_test(void);

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

extern u8 dis_data[3];

u8 canbuf_send[8];

//void EXTI1_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
//  {
//      USB_Host.usr_cb->OverCurrentDetected();
//      EXTI_ClearITPendingBit(EXTI_Line1);
//  }
//}

//�û�����������
//����ֵ:0,����
//       1,������
u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	printf("�豸���ӳɹ�!.\n");	 
	res=exf_getfree("0:",&total,&free);
	if(res==0)
	{	   
                total = total>>10;
                free  = free>>10;
		printf("FATFS OK!\n");	
		printf("U Disk Total Size:  %d   MB\n",total);//��ʾU�������� MB	 
		printf("U Disk  Free Size:  %d   MB\n",free); 	    	
	} 
        
        if(isFileExist("0:123.txt"))
        {
            printf("�ļ�������\n");
        }
        else
        {
              printf("�ļ�����\n");
              
              DeleteFile("0:abc.txt");
              
//              if(isFileExist("0:abc.txt"))
//              {
              
                  CreateFile("0:123.txt", "0:abc.txt");
//              }
              
//              DeleteFile("0:123.txt");
              
//            UpdateApp("0:stm32f107_cb.bin");
//            INTX_DISABLE();
//            iap_load_app(FLASH_APP1_ADDR);
        }
        
        LED0=1;
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//�豸���ӳɹ�
	{	
		LED1=!LED1;
		delay_ms(200);
                
                EWDT_TOOGLE();
                res=exf_getfree("0:",&total,&free);
                if(res==0)
                {	   
                  printf("FATFS OK!\n");	
                  printf("U Disk Total Size:  %d   MB\n",total>>10);//��ʾU�������� MB	 
                  printf("U Disk  Free Size:  %d   MB\n",free>>10); 	    	
                }
	}
   
	printf("�豸������...\n");

	return res;
} 


int main(void)
{        
	u8 t;
        RCC_ClocksTypeDef RCC_Clocks;
        //����ϵͳ�ж����ȼ�����2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        delay_init();
        
        //�ⲿ���Ź���ʼ��
//        EWDT_Drv_pin_config();
//        power_on_bsp_check(); 
        
        //��ʼ�����ڲ�����Ϊ115200
	uart_init(115200);		
        
        //��ʼ����LED���ӵ�Ӳ���ӿ�
	LED_Init();		
        
        //��ʼ��USMART	
//	usmart_dev.init(84); 	 
               
        //��ʼ���ڲ��ڴ��	
	mem_init();	
               
        //����ܳ�ʼ��
        digital_led_gpio_init();               
        digital_led_check();


        //MB85RCXX��ʼ��
        eep_init();
        if(MB85RCXX_Check())
        {
            printf("MB85RCXX_Checkʧ��\n");
          
        }        

        //AT24CXX��ʼ��
        AT24CXX_Init();        
        if(AT24CXX_Check())
        {
            printf("AT24CXX_Checkʧ��\n");
          
        }
//RCC_GetClocksFreq(&RCC_Clocks); 
#if 0        
        //Ϊfatfs��ر��������ڴ� 
 	if(exfuns_init())			
        {
            printf("fatfs�ڴ�����ʧ��\n");
        
        }
        
//        //����U��  
//  	f_mount(fs[0],"0:",1); 	
//     	 RCC_GetClocksFreq(&RCC_Clocks);      
	//��ʼ��USB����
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);  
	while(1)
	{
		USBH_Process(&USB_OTG_Core, &USB_Host);
		delay_ms(1);
		t++;
		if(t==200)
		{
//                        RCC_GetClocksFreq(&RCC_Clocks); 
			LED0=!LED0;
			t=0;
                        
                        EWDT_TOOGLE();
		}
	}	
#else

#if 1
        HW_TEST_INIT();
//        HW_TEST();
        
        GRL1 = 0;
        GRL2 = 0;
        GRL3 = 0;
        GRL4 = 0;
        GRL5 = 0;
        GRL6 = 0;
        GRL7 = 0;
        GRL8 = 0;
        GRL9 = 0;
        
        GSFR1 = 0;
        GSFR2 = 0;
        GSFR3 = 0;
        GSFR4 = 0;
        
        TRANS_CTRL1 = 0;
        TRANS_CTRL2 = 0;
              
        hw_can_test();
               

#else        
        //can����
//        can_test();
        can1_can2_test();
        
        //485test
        while(1){
//        USART3_SEND("ABCDE",6);
          if( (USART_GetITStatus(USART3, USART_IT_RXNE) == RESET) && (USART_RX_STA != 0))
          {
//                USART3->SR &= ~0x00000040;
                USART3_SEND(USART_RX_BUF,USART_RX_STA);
                USART_RX_STA = 0;
          }
        }
#endif   
        
#endif
        
}




#define USE_CAN CAN1


void can_test(void)
 {	 
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf_send[8],canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_LoopBack;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ

	 	
        CAN_Mode_Init(CAN1,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,10,mode);
	CAN_Mode_Init(USE_CAN,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,10,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������500Kbps    

	
 	while(1)
	{

	
                for(i=0;i<8;i++)
                {
                  canbuf_send[i]=cnt+i;//��䷢�ͻ�����
                  
                  printf("%s",canbuf_send[i]);	//��ʾ����
                }
                
                res=Can_Send_Msg(USE_CAN,canbuf_send,8);//����8���ֽ� 
                if(res)printf("Failed");		//��ʾ����ʧ��
                else printf("OK    ");	 		//��ʾ���ͳɹ�								   
                
                can_rcv=Can_Receive_Msg(USE_CAN,canbuf_recv);
		if(can_rcv)//���յ�������
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
                              printf("%s",canbuf_recv[i]);	//��ʾ����
 			}
		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED1=!LED1;//��ʾϵͳ��������	
			t=0;
			cnt++;
			printf("%d",cnt);	//��ʾ����
		}		   
	}
}

void can1_can2_test(void)
 {	 
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf_send[8],canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ

	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN��ʼ������ģʽ,������200Kbps    
        CAN_Mode_Init(CAN2,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN��ʼ������ģʽ,������200Kbps    

	
 	while(1)
	{
                //CAN1����
		if(mode==CAN_Mode_Normal)
		{
			for(i=0;i<8;i++)
			{
				canbuf_send[i]=cnt+i;//��䷢�ͻ�����

				printf("%s",canbuf_send[i]);	//��ʾ����
 			}
			res=Can_Send_Msg(CAN1,canbuf_send,8);//����8���ֽ� 
			if(res)
                          printf("Failed");		//��ʾ����ʧ��
			else 
                          printf("OK    ");	 		//��ʾ���ͳɹ�								   
		}

                //CAN2����  
		can_rcv=Can_Receive_Msg(CAN2,canbuf_recv);
		if(can_rcv)//���յ�������
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
                              printf("%s",canbuf_recv[i]);	//��ʾ����
 			}
		}
                
                
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED1=!LED1;//��ʾϵͳ��������	
			t=0;
			cnt++;
			printf("%d",cnt);	//��ʾ����
		}		   
	}
}

void hw_can_test(void)
 {	 
	u8 i=0,t=0;
	u8 canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
        u32 aa = 0;
	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,mode);//CAN��ʼ������ģʽ,������250Kbps    
//        CAN_Mode_Init(CAN2,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,mode);//CAN��ʼ������ģʽ,������250Kbps    

	
 	while(1)
	{
          
                aa++;
          
                  //CAN1����
          
                  HW_TEST();
                  
                  if(aa == 100)
                  {
                  
                        res=Can_Send_Msg(CAN1,canbuf_send,2);//����2���ֽ� 
                        
                        if(res)
                          printf("Failed");		//��ʾ����ʧ��
                        else 
                          printf("OK    ");	 		//��ʾ���ͳɹ�	
                        
                        LED0=!LED0;
                        aa = 0;
                  }

                //CAN2����  
		can_rcv=Can_Receive_Msg(CAN1,canbuf_recv);
		if(can_rcv)//���յ�������
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
                              printf("%s",canbuf_recv[i]);	//��ʾ����
 			}
		}
                
                
		t++; 
		delay_ms(10);
		if(t==50)
		{
			LED1=!LED1;//��ʾϵͳ��������	
			t=0;
		}		   
	}
}
