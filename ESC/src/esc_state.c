/*******************************************************************************
* File Name          : esc_state.c
* Author             : lison
* Version            : V1.0
* Date               : 10/11/2016
* Last modify date   : 10/12/2016
* Description        : Esc state machine.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "esc_state.h"
#include "esc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

u32 g_u32InitTestError = 0u;
u16 g_u16ParameterLoadingError = 0u;
u8 g_u8ParameterLoadingFinish = 0u;
u8 g_u8StartingProcessCheck = 0u;

/*******************************************************************************
* Function Name  : Esc_State_Machine
* Description    :                
* Input          : None                
* Output         : None
* Return         : None
*******************************************************************************/
void Esc_State_Machine(void)
{     
    Esc_State_Check();
    
    switch (CbBase_EscState) 
    {
        
       case ESC_INIT_STATE:
        {  
            /*
            if( ParaDataFromSafety[0][0] == 0u )
            {
                break;
            }
            */
            
            /* for test */
            /*g_u8ParameterLoadingFinish = 1u;*/
                
            /* Initial and wait for parameters from Safety board */
            if( g_u8ParameterLoadingFinish )
            {
                if( g_u32InitTestError || g_u16ParameterLoadingError )
                {
                    /* init fault */
                    
                }
                else
                {
                    CbBase_EscState = Check_Error_Present_Memory();
                }
            }
            break;
        }
       case ESC_READY_STATE:
        {            
            /* Safety fault order / Control fault */
            if( ESC_ERROR_CODE[0] || ( CMD_SF_ORDER & ORDER_FROM_SF_FAULT )) 
            {  
                CbBase_EscState = ESC_FAULT_STATE;
                break;
            }   
            
            /* Check contactors */
            if( CMD_SF_ORDER & ORDER_FROM_SF_ACTIVATE_UP_CONTACTOR )
            {
                g_u8StartingProcessCheck = 1u;
            }
            
            /* normal mode, In local mode */
            /* Safety run order / Control run */
            if(( CMD_SF_ORDER & ORDER_FROM_SF_RUN ) && (( CMD_SF_STATE_MODE & ESC_FROM_SF_STATE_MASK ) == ESC_FROM_SF_STATE_RUN ))
            {
                CbBase_EscState = ESC_RUN_STATE;
            }
            
            /* normal mode, In Remote mode */
            
            
            /* inspection mode */
            
            
            
            break;      
        }
       case ESC_RUN_STATE:
        {
            /* Safety fault order / Control fault */
            if( ESC_ERROR_CODE[0] || ( CMD_SF_ORDER & ORDER_FROM_SF_FAULT )) 
            {  
                CbBase_EscState = ESC_STOPPING_PROCESS_STATE;
                break;
            } 
            
            /* Safety stop order / Control stop */
            if( ( CMD_SF_ORDER & ORDER_FROM_SF_STOP ) )
            {
                CbBase_EscState = ESC_STOPPING_PROCESS_STATE;
            }
            
            break;
        } 
       case ESC_FAULT_STATE:
        {
            /*CheckReset();*/
            fault_code_reset();
            if( !ESC_ERROR_CODE[0] ) 
            {
                CbBase_EscState = ESC_READY_STATE;
            }
            
            break;    
        }        
       case ESC_STOPPING_PROCESS_STATE: 
        {
            if( !ESC_ERROR_CODE[0] ) 
            {
                /* Safety run order */
                if( CMD_SF_ORDER & ORDER_FROM_SF_RUN )
                {
//                    CbBase_EscState = ESC_RUN_STATE;
                }
            }
            
            /* stopping process finishes */
            if(( CMD_SF_STOP & ESC_FROM_SF_STOPPING_FINISH ))
            {
                if( ESC_ERROR_CODE[0] ) 
                {  
                    CbBase_EscState = ESC_FAULT_STATE;
                }    
                else
                {
                    CbBase_EscState = ESC_READY_STATE;
                }
            }
            break;
        }     
       default:        
        /* set fault */
        break;   
    }
}


/*******************************************************************************
* Function Name  : Esc_State_Check
* Description    :                
* Input          : None                
* Output         : None
* Return         : None
*******************************************************************************/
void Esc_State_Check(void)
{ 
    static u16 stat_u16TimeFaultExternalState = 0u;
    u8 u8SafetyState = CMD_SF_STATE_MODE & ESC_FROM_SF_STATE_MASK;
       
    
    /* Safety board and control board state check */
    /* The states INTERM, READY and STARTING PROCESS of the safety board are equivalent to READY state in the Control Board. */
    if(( u8SafetyState  == ESC_FROM_SF_STATE_STARTING_PROCESS ) || ( u8SafetyState == ESC_FROM_SF_STATE_INTERM )) 
    {
        u8SafetyState = ESC_FROM_SF_STATE_READY;
    }

    if( u8SafetyState != CbBase_EscState )
    {
        if( stat_u16TimeFaultExternalState < 0xffffu )
        {
            stat_u16TimeFaultExternalState++;
        }
        
        /* if during 500ms they don't have the same state then the system goes to fault: INCOHERENCE STATE EXTERNAL. */
        if( stat_u16TimeFaultExternalState > 100u )
        {
            /* Incoherence state external F389 */
            EN_ERROR49 |= 0x40u; 
        }
    }
    else
    {
        stat_u16TimeFaultExternalState = 0u;
    }
}


/******************************  END OF FILE  *********************************/


