/*******************************************************************************
* File Name          : comm_display_board.c
* Author             : lison
* Version            : V1.0
* Date               : 08/05/2016
* Description        : This file contains communication with external display board.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "comm_display_board.h"
#include "includes.h"
#include "usart.h" 
#include "crc16.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static u8 buff[300];

/*******************************************************************************
* Function Name  : Comm_DisplayBoard_Task
* Description    : communication with external display board thread.               
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.          
* Output         : None
* Return         : None
*******************************************************************************/ 		
void Comm_DisplayBoard_Task(void *arg)
{
 
    u16 i=0,len=0,tlen=0;

    
    
    while(1)
    {
        len = BSP_USART_Receive(USART2,buff,0);
        
        if(len<5)
        {
            
        }  
        else if( ((buff[0]==1)||(buff[0]==2)) && (!MB_CRC16(buff, len)) )   	
        {
//            DB_Comm_Addr = buff[0];
//            switch(buff[1])                
//            {
//               case 3:                     
//                tlen = modbus_slave_03();                       
//                break; 
//               case 16:	
//                tlen = modbus_slave_16();
//                break;                    
//            }	
            
            if(tlen)	
            {			
                i = MB_CRC16(buff, tlen-2);
                buff[tlen-1] = i>>8;
                buff[tlen-2] = i;
                BSP_USART_Send(USART3,buff,tlen);
            }			
            
        }		
        
        else if( (buff[0]==0x11) && (!MB_CRC16(buff, len)) )
        {
            
        }  
        
        vTaskDelay( 100 );
        
    }

}

/*******************************************************************************
* Function Name  : comm_db_init
* Description    : Create communicaion with display board task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void comm_db_init(void)
{
        xTaskCreate(Comm_DisplayBoard_Task, "Display board", configMINIMAL_STACK_SIZE * 2, NULL, COMM_DB_THREAD_PRIO, NULL);	
}

/******************************  END OF FILE  *********************************/