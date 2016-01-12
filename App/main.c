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

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

extern u8 dis_data[3];

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
		printf("FATFS OK!\n");	
		printf("U Disk Total Size:  %d   MB\n",total>>10);//��ʾU�������� MB	 
		printf("U Disk  Free Size:  %d   MB\n",free>>10); 	    	
	} 
        
        if(isFileExist("0:123.txt"))
        {
            printf("�ļ�������\n");
        }
        else
        {
              printf("�ļ�����\n");
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
        EWDT_Drv_pin_config();
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
#if 1        
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
        HW_TEST();

#else        
        //can����
        can_test();
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

	 	
   
	CAN_Mode_Init(USE_CAN,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������500Kbps    

	
 	while(1)
	{

		if(mode==CAN_Mode_LoopBack)//KEY0����,����һ������
		{
			for(i=0;i<8;i++)
			{
				canbuf_send[i]=cnt+i;//��䷢�ͻ�����

				printf("%s",canbuf_send[i]);	//��ʾ����
 			}
			res=Can_Send_Msg(USE_CAN,canbuf_send,8);//����8���ֽ� 
			if(res)printf("Failed");		//��ʾ����ʧ��
			else printf("OK    ");	 		//��ʾ���ͳɹ�								   
		}else if(mode==CAN_Mode_Normal)//WK_UP���£��ı�CAN�Ĺ���ģʽ
		{	   
//			mode=!mode;
  			CAN_Mode_Init(USE_CAN,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN��ͨģʽ��ʼ��, ������500Kbps 

			if(mode==0)//��ͨģʽ����Ҫ2��������
			{
				printf("Nnormal Mode ");	    
			}else //�ػ�ģʽ,һ��������Ϳ��Բ�����.
			{
 				printf("LoopBack Mode");
			}

		}		 
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


