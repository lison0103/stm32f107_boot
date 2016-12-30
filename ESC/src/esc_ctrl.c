/*******************************************************************************
* File Name          : esc_ctrl.c
* Author             : lison
* Version            : V1.0
* Date               : 09/30/2016
* Last modify date   : 10/26/2016
* Description        : Contains the esc output control.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "delay.h"
#include "hw_test.h"
#include "led.h"
#include "digital_led.h"
#include "usart.h"
#include "ewdt.h"
#include "can.h"
#include "bsp_iocfg.h"
#include "includes.h"
#include "crc16.h"
#include "esc.h"
#include "esc_eeprom_process.h"
#include "esc_ctrl.h"
#include "esc_comm.h"
#include "esc_state.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void esc_run_process(void);


/*******************************************************************************
* Function Name  : esc_run_process
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void esc_run_process(void)
{
     static u16 stat_u16StarDeltaChange = 0u;
     static u16 stat_u16TimerStarDelta = 0u;
     static u8 stat_u8StarDeltaChangeFinish = 0u;
     static u8 stat_u8TimeUpRelay = 0u,stat_u8UpRelayOnFlag = 0u;
     static u8 stat_u8TimeDownRelay = 0u,stat_u8DownRelayOnFlag = 0u;
     static u8 stat_u8TimeStarRelay = 0u,stat_u8StarRelayOnFlag = 0u;
     static u8 stat_u8TimeDeltaRelay = 0u,stat_u8DeltaRelayOnFlag = 0u;
     static u8 stat_RelayOutput = 0u;
          
     if( stat_u8TimeUpRelay < 255u ) 
     {
         stat_u8TimeUpRelay++;
     }
     if( stat_u8TimeDownRelay < 255u ) 
     {
         stat_u8TimeDownRelay++;
     }
     if( stat_u8TimeStarRelay < 255u ) 
     {
         stat_u8TimeStarRelay++;
     }
     if( stat_u8TimeDeltaRelay < 255u ) 
     {
         stat_u8TimeDeltaRelay++;
     }
     
     /* star or delta */
     if( ( !STAR_DELTA_DELAY ) || ( stat_u8StarDeltaChangeFinish ) )
     {
         CMD_ESC_RUN &= ~ESC_MOTOR_STAR;
         CMD_ESC_RUN |= ESC_MOTOR_DELTA;
     }
     else if( stat_u16StarDeltaChange == 1u )
     {
        CMD_ESC_RUN &= ~ESC_MOTOR_DELTA;
        CMD_ESC_RUN &= ~ESC_MOTOR_STAR;
     }
     else
     {
         CMD_ESC_RUN &= ~ESC_MOTOR_DELTA;
         CMD_ESC_RUN |= ESC_MOTOR_STAR;
     }
     
    /* Check contactors */
    if( g_u8StartingProcessCheck && ( CbBase_EscState == ESC_READY_STATE ) )
    {
        if( CMD_SF_ORDER & ORDER_FROM_SF_ACTIVATE_UP_CONTACTOR )
        {
            stat_RelayOutput |= OUTPUT_UP;
        }
        else
        {
            stat_RelayOutput &= ~OUTPUT_UP;
            g_u8StartingProcessCheck = 0u;
        }
    }
    /* not in run state */
    else if( CbBase_EscState != ESC_RUN_STATE )
    {
        stat_RelayOutput &= ~OUTPUT_UP;
        stat_RelayOutput &= ~OUTPUT_DOWN;
        stat_RelayOutput &= ~OUTPUT_DELTA;
        stat_RelayOutput &= ~OUTPUT_STAR;
        stat_u8StarDeltaChangeFinish = 0u;
        stat_u16StarDeltaChange = 0u;
        stat_u16TimerStarDelta = 0u;               
    }
    else
    { 
        if( CbBase_EscState == ESC_RUN_STATE )
        {    
            
            stat_RelayOutput &= ~OUTPUT_UP;
            stat_RelayOutput &= ~OUTPUT_DOWN;
            stat_RelayOutput &= ~OUTPUT_DELTA;
            stat_RelayOutput &= ~OUTPUT_STAR;

            if( stat_u16StarDeltaChange == 1u )
            {
                if( CMD_SF_ORDER & ORDER_FROM_SF_UP_DOWN ) 
                {
                    stat_RelayOutput |= OUTPUT_UP;
                }
                else
                {
                    stat_RelayOutput |= OUTPUT_DOWN;
                }
            }
     
            /* up direction, delta */
            if(( CMD_SF_ORDER & ORDER_FROM_SF_UP_DOWN ) 
               && (( CMD_ESC_RUN & ESC_MOTOR_MASK ) == ESC_MOTOR_DELTA ))
            {
                stat_RelayOutput |= OUTPUT_UP;
                stat_RelayOutput |= OUTPUT_DELTA;
            }
            /* down direction, delta */
            else if( (!(CMD_SF_ORDER & ORDER_FROM_SF_UP_DOWN)) 
                    && (( CMD_ESC_RUN & ESC_MOTOR_MASK ) == ESC_MOTOR_DELTA ))
            {
                stat_RelayOutput |= OUTPUT_DOWN;
                stat_RelayOutput |= OUTPUT_DELTA;
            }
            /* up direction, star */
            else if(( CMD_SF_ORDER & ORDER_FROM_SF_UP_DOWN ) 
                    && (( CMD_ESC_RUN & ESC_MOTOR_MASK ) == ESC_MOTOR_STAR ))
            {
                stat_RelayOutput |= OUTPUT_UP;
                stat_RelayOutput |= OUTPUT_STAR;
            }
            /* down direction, star */
            else if( (!(CMD_SF_ORDER & ORDER_FROM_SF_UP_DOWN))  
                    && (( CMD_ESC_RUN & ESC_MOTOR_MASK ) == ESC_MOTOR_STAR ))
            {
                stat_RelayOutput |= OUTPUT_DOWN;
                stat_RelayOutput |= OUTPUT_STAR;
            }
            else
            {
                /* fault */
                
            }  
            
            /* Star delta switch */
            if( STAR_DELTA_DELAY && ( stat_u16StarDeltaChange == 0u ) )
            {
                stat_u16TimerStarDelta++;
                /* star delta delay */
                if( ( stat_u16TimerStarDelta * 5 ) > ( STAR_DELTA_DELAY * 1000u ))
                {
                    stat_RelayOutput &= ~OUTPUT_DELTA;
                    stat_RelayOutput &= ~OUTPUT_STAR;    
                    stat_u16StarDeltaChange = 1u;
                    stat_u16TimerStarDelta = 0u;
                }
            }
            else if( stat_u16StarDeltaChange == 1u )
            {
                stat_u16TimerStarDelta++;
                /* star open and delay 100ms */
                if( ( stat_u16TimerStarDelta * 5 ) > 100u )
                {  
                    stat_u16StarDeltaChange = 2u;
                    stat_u8StarDeltaChangeFinish = 1u;
                    stat_u16TimerStarDelta = 0u;
                }        
            }
            else
            {}
        }
    } 
    
    /* Relay output */
    /* up relay */
    if(!( stat_RelayOutput & OUTPUT_UP ))
    {
        CMD_OUTPUT_PORT2 &= ~OUTPUT_UP;
        if( stat_u8UpRelayOnFlag ) 
        {
            stat_u8TimeUpRelay = 0u;    
        }
        stat_u8UpRelayOnFlag = 0u;    
    }
    else
    {
        stat_u8UpRelayOnFlag = 1u;
        
        /* delay 100ms */
        if( stat_u8TimeUpRelay > 20u )
        {  
            CMD_OUTPUT_PORT2 |= OUTPUT_UP;
        }        
    }
    
    /* down relay */
    if(!( stat_RelayOutput & OUTPUT_DOWN ))
    {
        CMD_OUTPUT_PORT2 &= ~OUTPUT_DOWN;    
        if( stat_u8DownRelayOnFlag ) 
        {
            stat_u8TimeDownRelay = 0u;    
        }
        stat_u8DownRelayOnFlag = 0u;  
    }
    else
    {
        stat_u8DownRelayOnFlag = 1u;
        
        if( stat_u8TimeDownRelay > 20u )
        {  
            CMD_OUTPUT_PORT2 |= OUTPUT_DOWN;
        }        
    }   

    /* star relay */
    if(!( stat_RelayOutput & OUTPUT_STAR ))
    {
        CMD_OUTPUT_PORT2 &= ~OUTPUT_STAR;       
        if( stat_u8StarRelayOnFlag ) 
        {
            stat_u8TimeStarRelay = 0u;    
        }
        stat_u8StarRelayOnFlag = 0u;  
    }
    else
    {
        stat_u8StarRelayOnFlag = 1u;
        
        if( stat_u8TimeStarRelay > 20u )
        {  
            CMD_OUTPUT_PORT2 |= OUTPUT_STAR;
        }        
    }
    
    /* delta relay */
    if(!( stat_RelayOutput & OUTPUT_DELTA ))
    {
        CMD_OUTPUT_PORT2 &= ~OUTPUT_DELTA;       
        if( stat_u8DeltaRelayOnFlag ) 
        {
            stat_u8TimeDeltaRelay = 0u;    
        }
        stat_u8DeltaRelayOnFlag = 0u;  
    }
    else
    {
        stat_u8DeltaRelayOnFlag = 1u;
        
        if( stat_u8TimeDeltaRelay > 20u )
        {  
            CMD_OUTPUT_PORT2 |= OUTPUT_DELTA;
        }        
    }    
}


/*******************************************************************************
* Function Name  : esc_task
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void esc_task(void *arg)
{	     
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 5; 
    static u8 Tms10Counter = 0u,Tms20Counter = 0u;
    
    xLastWakeTime = xTaskGetTickCount();
    
    while(1)
    {        
        /*********************** 5ms *******************************/
        Get_GpioInput(&CBEscData.ControlInputData[0]);
        Input_Check();
        led_display();
        output_driver(&CBEscData.ControlOutputData[0]);  
        
        if( testmode == 0u )
        {
            Esc_State_Machine();
            
            esc_run_process();
            
            ParametersLoading();
        }

        /*********************** 10ms *******************************/
        if( ++Tms10Counter >= 2u )
        { 
            Tms10Counter = 0u; 
            
            fault_code_decode(ErrorCodeControl);                  
            
            /* error display */
            esc_error_display();
        }
        /*********************** 20ms *******************************/
        if( ++Tms20Counter >= 4u )
        {                   
            Tms20Counter = 0u;

        }               
        
        xEventGroupSetBits(xCreatedEventGroup, TASK_BIT_0);
        /*vTaskDelay( 5 );*/
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


/*******************************************************************************
* Function Name  : esc_task_init
* Description    : None
*                  
* Input          : None
*                  None                 
* Output         : None
* Return         : None
*******************************************************************************/ 
void esc_task_init(void)
{
	xTaskCreate(esc_task, "esc", configMINIMAL_STACK_SIZE * 2, NULL, ESC_TASK_PRIO, NULL);
}


/******************************  END OF FILE  *********************************/