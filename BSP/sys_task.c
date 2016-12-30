/*******************************************************************************
* File Name          : sys_task.c
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : This file contains some task funcitons.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "esc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void led_ewdt_task(void *arg);
void rx485_task(void *arg);
void rtc_task(void *arg);

EventGroupHandle_t xCreatedEventGroup = NULL;
TimerHandle_t xTimers = NULL;
u16 g_u16LedFreq = FREQ_1HZ;

/*******************************************************************************
* Function Name  : led_ewdt_task
* Description    : Led flashes and kick the dog.
*                  
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void led_ewdt_task(void *arg)
{
    EventBits_t uxbits;

    LED0 = 0;
  
    for( ; ; )
    {
        LED0 =!LED0;
        if( testmode != 0 )
        {
            LED1 =!LED1;
        }
        
        uxbits = xEventGroupWaitBits(xCreatedEventGroup,
                            TASK_BIT_ALL,
                            pdTRUE,
                            pdTRUE,
                            0);
        
       if((uxbits & TASK_BIT_ALL) == TASK_BIT_ALL )
       {
          EWDT_TOOGLE();
       }

        vTaskDelay( g_u16LedFreq );
    }
}

/*******************************************************************************
* Function Name  : rx485_task
* Description    : Rx485 port test.
*                  
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void rx485_task(void *arg)
{
    u8 len = 0;
    
    for( ; ; )
    {        
        if(USART_RX_STA&0x8000)
        {					   
            len=USART_RX_STA&0x3fff;
            
            USART3_SEND(USART_RX_BUF,len);
            USART_RX_STA=0;
            
        }
        
        vTaskDelay( 10 );
    }              
}


/*******************************************************************************
* Function Name  : rtc_task
* Description    : Rtc clock.
*                  
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void rtc_task(void *arg)
{
        
    for( ; ; )
    {
        
        if( timeset == 1 )
        {
            RTC_SetTime( ESC_RTCC );
            timeset = 0;
        }  
        else
        {  
            RTC_GetTime( ESC_RTCC );
        }                
        
        vTaskDelay( 500 );
    }              

}

/*******************************************************************************
* Function Name  : vTimerCallback
* Description    :            
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/ 
void vTimerCallback(xTimerHandle pxTimer)
{
    u32 ulTimerID;
    
//    configASSERT(pxTimer);
    
    ulTimerID = (u32)pvTimerGetTimerID(pxTimer);
           
    if( ulTimerID == 0 )
    {
        if( timeset == 1 )
        {
            RTC_SetTime( ESC_RTCC );
            timeset = 0;
        }  
        else
        {  
            RTC_GetTime( ESC_RTCC );
        } 
    }

}

/*******************************************************************************
* Function Name  : rx485_test_init
* Description    : Create Rx485 test task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void rx485_test_init(void)
{
	xTaskCreate(rx485_task, "RX485", configMINIMAL_STACK_SIZE * 2, NULL, RX485_THREAD_PRIO, NULL);
}

/*******************************************************************************
* Function Name  : rtc_clock_init
* Description    : Create rtc clock task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void rtc_clock_init(void)
{
    u8 TimerID = 0u;
    
    xTimers = xTimerCreate("Timer",
                           500,
                           pdTRUE,
                           (void*)TimerID,
                           vTimerCallback);
    
    
    if(xTimers != NULL)
    {
        if( xTimerStart(xTimers, 500) != pdPASS )
        {
            
        }
    }
//	xTaskCreate(rtc_task, "RTC CLOCK", configMINIMAL_STACK_SIZE, NULL, RTC_THREAD_PRIO, NULL);  
}


/*******************************************************************************
* Function Name  : led_ewdt_init
* Description    : Create led and ewdt task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void led_ewdt_init(void)
{
      xCreatedEventGroup = xEventGroupCreate();
      
      if( xCreatedEventGroup != NULL )
      {
          xTaskCreate(led_ewdt_task, "LED_EWDT", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);
      }
}


/******************************  END OF FILE  *********************************/


