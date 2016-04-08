/*******************************************************************************
* File Name          : sys_task.c
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : 
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
void selfcheck_task(void *arg);


/*******************************************************************************
* Function Name  : selfcheck_task
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void selfcheck_task(void *arg)
{
        
	for( ; ; )
	{
                /* self check */
                STL_DoRunTimeChecks();
                vTaskDelay( 5 );
	}
}

/*******************************************************************************
* Function Name  : led_ewdt_task
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void led_ewdt_task(void *arg)
{
        LED0 = 0;
        
	for( ; ; )
	{
                LED0 =!LED0;
                EWDT_TOOGLE();
		vTaskDelay( 500 );
	}
}

/*******************************************************************************
* Function Name  : rx485_task
* Description    : None
*                  
* Input          : None
*                  None
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
                    len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
                    
                    USART3_SEND(USART_RX_BUF,len);
                    USART_RX_STA=0;
                  
                }
		vTaskDelay( 10 );
	}              

}


/*******************************************************************************
* Function Name  : rtc_task
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void rtc_task(void *arg)
{

        u32 t = 0;
        
	for( ; ; )
	{

            if(CMD_FLAG10 & 0x40)
            {
                RTC_SetTime( &Modbuff[50] );
                CMD_FLAG10 &= ~0x40;
            }  
            else
            {  
                if( RTC_GetTime( &Modbuff[50] ) )
                {
                    CMD_FLAG10 |= 0x20;
                }
                else
                {
                    CMD_FLAG10 &= ~0x20;
                }
            }  
               
#if DEBUG_PRINTF            
            if(t!=calendar.sec)
            {
                t=calendar.sec;
                printf("%d - %02d - %02d \n", calendar.w_year, calendar.w_month, calendar.w_date);
           
                switch(calendar.week)
                {
                    case 0:
                      printf("Sunday \n");
                      break;
                    case 1:
                      printf("Monday \n");
                      break;
                    case 2:
                      printf("Tuesday \n");
                      break;
                    case 3:
                      printf("Wednesday \n");
                      break;
                    case 4:
                      printf("Thursday \n");
                      break;
                    case 5:
                      printf("Friday \n");
                      break;
                    case 6:
                      printf("Saturday \n");
                      break;  
                    default:
                      printf("error \n");
                      break;
                }
                printf("%02d : %02d : %02d \n", calendar.hour, calendar.min, calendar.sec);

            }
#endif
            
            vTaskDelay( 100 );
	}              

}


/*******************************************************************************
* Function Name  : rx485_test_init
* Description    : None
*                  
* Input          : None
*                  None
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
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void rtc_clock_init(void)
{
	xTaskCreate(rtc_task, "RTC CLOCK", configMINIMAL_STACK_SIZE * 2, NULL, RTC_THREAD_PRIO, NULL);  
}


/*******************************************************************************
* Function Name  : rtc_clock_init
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void led_ewdt_init(void)
{
	xTaskCreate(led_ewdt_task, "LED_EWDT", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);
}


/*******************************************************************************
* Function Name  : self_check_init
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void self_check_init(void)
{
	xTaskCreate(selfcheck_task, "SELF_CHECK", configMINIMAL_STACK_SIZE * 4, NULL, SELFCHEK_TASK_PRIO, NULL);
}


/******************************  END OF FILE  *********************************/


