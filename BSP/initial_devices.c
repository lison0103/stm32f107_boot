/*******************************************************************************
* File Name          : initial_devices.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "initial_devices.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;


/*******************************************************************************
* Function Name  : Input_Output_PinInit
* Description    : Initializes the Input and Output GPIO
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void Bsp_Init(void)
{

        /** set system interrupt priority group 2 **/
	NVIC_Configuration();
        
        /** delay init **/
	delay_init();  
        
        /** LED init **/
	LED_Init();     
       
        
#ifdef GEC_CB_MAIN        

        /**  RTC init **/
        while( RTC_Init() )		
	{           
            /** rtc init error , cystal problem **/
            delay_ms(800);	
	}	
        
        /** ewdt init **/
        EWDT_Drv_pin_config();
        power_on_bsp_check();         
        
        /** input and relay output test init **/
        Input_Output_PinInit();
        
        /** usart init **/
	USART3_Init();		
        USART2_Init();        	                	
               
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
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb); 
        
#endif
        
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : 
*                  
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{

#ifdef GEC_CB_MAIN  
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);
#else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    
    INTX_ENABLE();

}

/******************************  END OF FILE  *********************************/



