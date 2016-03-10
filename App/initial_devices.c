#include "initial_devices.h"

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

/******************************************************************************* 
*******************************************************************************/
void Bsp_Init(void)
{
#ifdef GEC_CB_MAIN
        /** stm32 self test **/
        self_test();
#endif
        /** set system interrupt priority group 2 **/
	NVIC_Configuration();
        
        /** delay init **/
	delay_init();  
        
        /** LED init **/
	LED_Init();     
       
        
#ifdef GEC_CB_MAIN        

        /** ewdt init **/
        EWDT_Drv_pin_config();
        power_on_bsp_check();         
        
        /** input and relay output test init **/
        Input_Output_PinInit();
        
        /** usart init **/
	uart_init(115200);		
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