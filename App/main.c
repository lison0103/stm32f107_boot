#include "lsys.h"
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
#include "iap.h"



#ifdef GEC_CB_MAIN

#include "hw_test.h"
#include "freertos_lwip.h" 

extern u8 dis_data[3];

u8 canbuf_send[8];

extern void modbus_socket_init(void);

#else

u32 timecounter = 0;

#endif


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;



/******************************************************************************* 
*******************************************************************************/
void Bsp_Init(void)
{
    
        /** set system interrupt priority group 2 **/
	NVIC_Configuration();
        
        /** delay init **/
	delay_init();  
        
        /** LED init **/
	LED_Init();

        /** ewdt init **/
        EWDT_Drv_pin_config();
//        power_on_bsp_check();      

       
        
#ifdef GEC_CB_MAIN        

        /** input and relay output test init **/
        HW_TEST_INIT();
        
        /**usart init **/
	uart_init(115200);		
                	                	
               
        /** digital led init **/
        digital_led_gpio_init();               
//        digital_led_check();


        /** MB85RCXX init **/
        eep_init();
        if(MB85RCXX_Check())
        {
            printf("MB85RCXX_Check fail \n");
          
        }        

        /** AT24CXX init **/
        AT24CXX_Init();        
        if(AT24CXX_Check())
        {
            printf("AT24CXX_Check fail \n");
          
        }       
        
#else       
        /** mem init **/	
	mmem_init(); 
        
        /** fatfs apply memory **/ 
 	if(exfuns_init())			
        {
            printf("fatfs memory apply fail \n");
        
        }
             
	/** USB HOST init **/
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); 
        
#endif
        
}

/******************************************************************************* 
*******************************************************************************/
void Task_Loop(void)
{
  
    u8 t;
  
  
#ifdef GEC_CB_MAIN 

#if   1

	ETH_BSP_Config();
	/* Initilaize the LwIP stack */
	LwIP_Init();

	xTaskCreate(led0_task, "LED0", configMINIMAL_STACK_SIZE, NULL, LED0_TASK_PRIO, NULL);
        xTaskCreate(led1_task, "LED1", configMINIMAL_STACK_SIZE, NULL, LED1_TASK_PRIO, NULL);
//	xTaskCreate(TCPClient, "TCPClient",  configTCP_STACK_SIZE, NULL, TCP_TASK_PRIO, NULL);
//        xTaskCreate(TCPServer, "TCPServer",  configTCP_STACK_SIZE, NULL, TCP_TASK_PRIO, NULL);
        modbus_socket_init();
	/* Start scheduler */
	vTaskStartScheduler();


#endif    
    
#if   0   
    
        /** mem init **/	
	mmem_init(); 
        
        /** fatfs apply memory **/ 
 	if(exfuns_init())			
        {
            printf("fatfs memory apply fail \n");
        
        }
             
	/** USB HOST init **/
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);    
    
        while(1)
        {
              USBH_Process(&USB_OTG_Core, &USB_Host);
              delay_ms(1);
              t++;
              
              if(t==200)
              {
                  LED0=!LED0;
                  t=0;
                  
                  EWDT_TOOGLE();
              }
        }
#endif
          

#if   0
                      
        hw_can_test();
               
#endif        
        
#if   0        
        //can����
//        can_test();
//        can1_can2_test();
        
        
        u8 len = 0;
        
        //485test
        while(1)
        {
          
//        USART3_SEND("ABCDE",6);
                    
          if(USART_RX_STA&0x8000)
          {					   
              len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
              
              USART3_SEND(USART_RX_BUF,len);
              USART_RX_STA=0;
          }
                
          EWDT_TOOGLE();
        }
#endif   
        
        
#else
        
        
	while(1)
	{
          
		USBH_Process(&USB_OTG_Core, &USB_Host);
		delay_ms(1);
		t++;
                timecounter++;
                
                if(timecounter == 3000)
                {
                    LED0 = 1;
                    LED1 = 1;
                    iap_load_app(FLASH_APP1_ADDR);                               
                }
 
		if(t==200)
		{

			LED0=!LED0;
			t=0;
                        
                        EWDT_TOOGLE();
		}
	}        
          
#endif
        
}

/******************************************************************************* 
*******************************************************************************/
int main(void)
{        
 	        
        Bsp_Init();
        Task_Loop();
        
}



/******************************************************************************* 
//�û�����������
//����ֵ:0,����
//       1,������
*******************************************************************************/
u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	printf("�豸���ӳɹ�!.\n");	 
        
        LED0 = 1;

#ifdef GEC_CB_BOOTLOADER

        timecounter = 0;
        
        if(!isFileExist("0:GEC-CB.bin"))
        {
            UpdateApp("0:GEC-CB.bin");
        }
        
//        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, DISABLE);
        LED1 = 1;
        iap_load_app(FLASH_APP1_ADDR);
        
#else 
        
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
              

        }
#endif        
        
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


