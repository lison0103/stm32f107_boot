#include "initial_devices.h"
#include "includes.h"



#ifdef GEC_CB_MAIN


#else

u32 timecounter = 0;

#endif







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
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);    
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


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */
