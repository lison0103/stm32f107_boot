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

#include "includes.h"


#ifdef GEC_CB_MAIN

#include "hw_test.h"
#include "freertos_lwip.h" 
#include "modbusTask.h"

#define LED_TASK_PRIO			( tskIDLE_PRIORITY + 1 )
#define TCP_TASK_PRIO			( tskIDLE_PRIORITY + 3 )
#define DHCP_TASK_PRIO                  ( tskIDLE_PRIORITY + 4 ) 

extern u8 dis_data[3];

u8 canbuf_send[8];

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
        
        /** usart init **/
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
  
    u16 t = 0;
  
  
#ifdef GEC_CB_MAIN 

/******************************************************************************/    
#if   TCP_SERVER_TEST

	ETH_BSP_Config();
	/* Initilaize the LwIP stack */
	LwIP_Init();

//	xTaskCreate(TCPClient, "TCPClient",  DEFAULT_THREAD_STACKSIZE * 2, NULL, TCP_TASK_PRIO, NULL);
        xTaskCreate(TCPServer, "TCPServer",  DEFAULT_THREAD_STACKSIZE * 2, NULL, TCP_TASK_PRIO, NULL);

#ifdef USE_DHCP
        /* Start DHCPClient */
        xTaskCreate(LwIP_DHCP_task, "DHCP", configMINIMAL_STACK_SIZE * 2, NULL,DHCP_TASK_PRIO, NULL);
#endif                	
        
        xTaskCreate(led_task, "LED", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);
        
	/* Start scheduler */
	vTaskStartScheduler();

#endif
/******************************************************************************/ 
#if   MODBUS_TCP_TEST

        ETH_BSP_Config();
	/* Initilaize the LwIP stack */
	LwIP_Init();

        modbus_socket_init();
#ifdef USE_DHCP
        /* Start DHCPClient */
        xTaskCreate(LwIP_DHCP_task, "DHCP", configMINIMAL_STACK_SIZE * 2, NULL,DHCP_TASK_PRIO, NULL);
#endif        
        	       
        xTaskCreate(led_task, "LED", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);
        
	/* Start scheduler */
	vTaskStartScheduler();
    
#endif
/******************************************************************************/     
    
#if   MODBUS_RTU_TEST      
        	
        modbus_rtu_init();
        
        xTaskCreate(led_task, "LED", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);
        
	/* Start scheduler */
	vTaskStartScheduler();


#endif    
/******************************************************************************/     
#if   USB_HOST_MASS_STORAGE_TEST   
    
        /** mem init **/	
	mmem_init(); 
        
        /** fatfs apply memory **/ 
 	if(exfuns_init())			
        {
            printf("fatfs memory apply fail \n");
        
        }
             
	/** USB HOST init **/
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);    
        printf("wait usb mass storage connected...\n");
        
        while(1)
        {
              USBH_Process(&USB_OTG_Core, &USB_Host);
              
              delay_ms(1);
              t++;
              
              if(t==500)
              {
                  
                  LED0=!LED0;
                  t=0;
                  
                  EWDT_TOOGLE();
              }
        }
#endif
/******************************************************************************/           
#if   INPUT_RELAY_OUTPUT_AND_CAN_TEST
                      
        hw_can_test();
               
#endif        
/******************************************************************************/         
#if   RX485_TEST
        
        //can测试
//        can_test();
//        can1_can2_test();
        
        
        u8 len = 0;
        
        //485test
        while(1)
        {
          
//        USART3_SEND("ABCDE",6);
//          delay_ms(10);          
          if(USART_RX_STA&0x8000)
          {					   
              len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
              
              USART3_SEND(USART_RX_BUF,len);
              USART_RX_STA=0;
              
//              USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//              USART3_TRX_CONTROL = 0;
          }
                
          EWDT_TOOGLE();
        }
#endif   
/******************************************************************************/         
        
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
//用户测试主程序
//返回值:0,正常
//       1,有问题
*******************************************************************************/
u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	printf("设备连接成功!.\n");	 
        
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
		printf("U Disk Total Size:  %d   MB\n",total);//显示U盘总容量 MB	 
		printf("U Disk  Free Size:  %d   MB\n",free); 	    	
	} 

       
        if(isFileExist("0:123.txt"))
        {
            printf("文件不存在\n");
        }
        else
        {
              printf("文件存在\n");
              
              DeleteFile("0:abc.txt");
              
//              if(isFileExist("0:abc.txt"))
//              {
              
                  CreateFile("0:123.txt", "0:abc.txt");
//              }
              
//              DeleteFile("0:123.txt");
              

        }
#endif        
        
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//设备连接成功
	{	
		LED1=!LED1;
		delay_ms(200);
                
                EWDT_TOOGLE();
                res=exf_getfree("0:",&total,&free);
                if(res==0)
                {	   
                  printf("FATFS OK!\n");	
                  printf("U Disk Total Size:  %d   MB\n",total>>10);//显示U盘总容量 MB	 
                  printf("U Disk  Free Size:  %d   MB\n",free>>10); 	    	
                }
	}
   
	printf("设备连接中...\n");

	return res;
}


