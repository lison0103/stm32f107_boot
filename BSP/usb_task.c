/*******************************************************************************
* File Name          : usb_task.c
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : 
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
#endif


/*******************************************************************************
* Function Name  : USH_User_App
* Description    : �û�����������
*                  
* Input          : None
*                  None
* Output         : None
* Return         : 0,���� 1,������
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



#ifdef GEC_CB_MAIN

/*******************************************************************************
* Function Name  : usb_task
* Description    : None
*                  
* Input          : None
*                  None
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
            printf("fatfs memory apply fail \n");
        
        }
             
	/** USB HOST init **/
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);    
        printf("wait usb mass storage connected...\n");
        
        
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
* Description    : None
*                  
* Input          : None
*                  None
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
            printf("fatfs memory apply fail \n");
        
        }
             
	/** USB HOST init **/
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);   
        
        
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
         
}
#endif

/******************************  END OF FILE  *********************************/


