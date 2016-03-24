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
void NVIC_Configuration(void);
void RCC_Configuration(void);



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

        /** RCC  Configuration **/
        RCC_Configuration(); 
        
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



