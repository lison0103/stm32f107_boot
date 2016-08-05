/*******************************************************************************
* File Name          : initial_devices.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : Contains the devices initinal.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "initial_devices.h"
#ifdef GEC_CB_MAIN 
#include "esc_eeprom_process.h"
#include "esc_parameter_process.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void NVIC_Configuration(void);
void RCC_Configuration(void);



/*******************************************************************************
* Function Name  : Initial_Device
* Description    : Initialize the CPU1,CPU2 and peripherals
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void Initial_Device(void)
{

        /** set system interrupt priority group 2 **/
	NVIC_Configuration();

        /** RCC  Configuration **/
        RCC_Configuration(); 
        
        /** delay init **/
	Delay_Init();  
        
        /** LED init **/
	LED_Init();     
       
        
#ifdef GEC_CB_MAIN        

        /**  RTC init **/
        if( RTC_Init() )		
	{           
            /** rtc init error , cystal problem **/	
	}	
        
        /** ewdt init **/
        EWDT_Drv_pin_config();
        ExtWdtCheck();         
        
        /** input and relay output test init **/
        Input_Output_PinInit();
        
        /** usart init, usart3 for 485, usart for external comm device **/
	USART3_Init();		
        USART2_Init();        	                	
               
        /** digital led init **/
        digital_led_gpio_init();               
//        digital_led_check();


        /** MB85RCXX init **/
        eep_init();
        if(MB85RCXX_Check())
        {
            /** MB85RCXX_Check fail **/
          
        }        

        /** AT24CXX init **/
        AT24CXX_Init();        
        if(AT24CXX_Check())
        {
            /** AT24CXX_Check fail **/
          
        } 
        else
        {
            get_error_record_from_eeprom();
        }
        
        /** CAN module init **/
	CAN_Int_Init(CAN1);    
        CAN_Int_Init(CAN2);      
        
        /* HardwareTest */
        HardwareTEST();
        
        /* Parameters Loading */
        ParametersLoading();        
        
        
        /* Self test routines initialization ---------------------------------*/
        STL_InitRunTimeChecks();
        
#else       
        
#endif
        
}


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2); 

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
   
  
                                        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
  
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA
                           |RCC_APB2Periph_GPIOB
                           |RCC_APB2Periph_GPIOC
                           |RCC_APB2Periph_GPIOD
                           |RCC_APB2Periph_GPIOE ,
                           ENABLE); 
     
      RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1 , ENABLE);   
    #ifdef GEC_CB_MAIN
      /* Enable GPIO and Peripherals clocks */   
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_CRC, ENABLE);
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE );
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1 , ENABLE); 
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN2 , ENABLE);    
      RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4 , ENABLE);   
    #endif
}



/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configuring NVIC interrupt group.
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
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
    
    INTX_ENABLE();

}

/******************************  END OF FILE  *********************************/



