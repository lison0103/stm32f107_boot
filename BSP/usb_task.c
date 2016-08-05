/*******************************************************************************
* File Name          : usb_task.c
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : This file contains usb mass storage task funcitons.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

#ifdef GEC_CB_BOOTLOADER
u16 t = 0;
u32 timecounter = 0;
u8 AppUpdateFinsh = 0;
#endif


/*******************************************************************************
* Function Name  : USH_User_App
* Description    : User test main program
*                  
* Input          : None
*                  None
* Output         : None
* Return         : 0:ok  1:error
*******************************************************************************/ 
u8 USH_User_App(void)
{ 
	
	u8 res=0;
	/* Device connect success! */	 
        
        LED0 = 1;

#ifdef GEC_CB_BOOTLOADER

        timecounter = 0;
        
        if(!isFileExist("0:GEC-CB-A.bin"))
        {
            UpdateApp("0:GEC-CB-A.bin");
            AppUpdateFinsh = 1;
        }

        while(HCD_IsDeviceConnected(&USB_OTG_Core))
	{	
            if( AppUpdateFinsh == 1 )
            {
                LED1=!LED1;
                delay_ms(200); 
            }
            else
            {
                LED1 = 0;
            }
	}
//        iap_load_app(FLASH_APP1_ADDR);
        
#else 
        u32 total,free;
        
	res=exf_getfree("0:",&total,&free);
	if(res==0)
	{	   
                total = total>>10;
                free  = free>>10;
		printf("FATFS OK!\n");	
		printf("U Disk Total Size:  %d   MB\n",total);
		printf("U Disk  Free Size:  %d   MB\n",free); 	    	
	} 

       
        if(isFileExist("0:123.txt"))
        {
            /* File not exists */
        }
        else
        {
              /* File exists */
              
              DeleteFile("0:abc.txt");
         
              CopyFile("0:123.txt", "0:abc.txt");

        }
        
        
	while(HCD_IsDeviceConnected(&USB_OTG_Core))
	{	
		LED1=!LED1;
		vTaskDelay(200);
                
                res=exf_getfree("0:",&total,&free);
                if(res==0)
                {	   
                  /* FATFS OK! */
                  printf("U Disk Total Size:  %d   MB\n",total>>10); 
                  printf("U Disk  Free Size:  %d   MB\n",free>>10); 	    	
                }
	}
   
	/* Device is connecting... */
        
#endif
	return res;
}



#ifdef GEC_CB_MAIN

/*******************************************************************************
* Function Name  : usb_task
* Description    : None
*                  
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void usb_task(void *arg)
{
  
        /** mem init **/	
	mmem_init(); 
        
        /** fatfs apply memory **/ 
 	if(exfuns_init())			
        {
            /* fatfs memory apply fail */
        
        }
             
	/** USB HOST init **/
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);    
        /* wait usb mass storage connected... */
        
        
	for( ; ; )
	{
                USBH_Process(&USB_OTG_Core, &USB_Host);
		vTaskDelay( 1 );
	} 
         
}


/*******************************************************************************
* Function Name  : usb_mass_storage_init
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void usb_mass_storage_init(void)
{
	xTaskCreate(usb_task, "USB MASS STORAGE", DEFAULT_THREAD_STACKSIZE * 2, NULL, USB_THREAD_PRIO, NULL);
}

#endif


#ifdef GEC_CB_BOOTLOADER
/*******************************************************************************
* Function Name  : usb_process
* Description    : Wait 3s to upgrade , overtime enter APP
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void usb_process(void)
{
  
        /** mem init **/	
	mmem_init(); 
        
        /** fatfs apply memory **/ 
 	if(exfuns_init())			
        {
            /* fatfs memory apply fail */
        
        }
             
	/** USB HOST init **/
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);   
        
        
	while(1)
	{
          
		USBH_Process(&USB_OTG_Core, &USB_Host);
		delay_ms(1);
		t++;
                timecounter++;
                
                
                if( AppUpdateFinsh == 1 )
                {
                    while(1)
                    {
                        NVIC_SystemReset();
                    }
                }
                else if( timecounter == 1000 )
                {
                    LED0 = 1;
                    LED1 = 1;
                    iap_load_app(FLASH_APP1_ADDR);                               
                }
 
		if( t == 200 )
		{

			LED0=!LED0;
			t = 0;
                        
                        EWDT_TOOGLE();
		}
	} 
         
}
#endif

/******************************  END OF FILE  *********************************/


