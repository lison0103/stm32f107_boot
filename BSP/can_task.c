/*******************************************************************************
* File Name          : can_task.c
* Author             : lison
* Version            : V1.0
* Date               : 04/26/2016
* Description        : 
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/*******************************************************************************
* Function Name  : can_comm_thread
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void can_comm_thread(void *arg)
{
  

        
	while(1)
	{

            vTaskDelay( 20 );
	}										 
}	


/*******************************************************************************
* Function Name  : can_comm_init
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void can_comm_init(void)
{	
        xTaskCreate(can_comm_thread, "CAN", DEFAULT_THREAD_STACKSIZE * 2, NULL, CAN_THREAD_PRIO, NULL);
}



/******************************  END OF FILE  *********************************/


