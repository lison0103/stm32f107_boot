/*******************************************************************************
* File Name          : esc_error_process.c
* Author             : lison
* Version            : V1.0
* Date               : 10/12/2016
* Description        : This file contains esc error process functions.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "esc.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* fault type: Failure lock / standard fault / recovery fault */
u8 EscFaultType[512] = 
{
    /* 1~30,F00~F29 */                      
    EF,EU,EU,EU,EU,EU,EU,EF,EF,ES,  EU,EF,EF,EU,ES,ES,EU,EU,EU,EU,  EU,ES,ES,ES,ES,EU,ER,ER,ER,ER,
    /* 31~60,F30~F59 */                      
    EU,EU,ES,ES,EU,ES,ES,EU,ES,ES,  ES,ES,ES,ES,EF,EF,EF,EF,ES,EU,  EF,EF,EU,EU,EU,EU,EU,EU,EU,EU,  
    /* 61~90 */                      
    EU,EU,ES,EU,EU,EU,EU,ES,EU,EU,  EU,EU,EU,ES,ES,EU,EU,EU,EU,EU,  EU,EU,EU,EU,EU,EU,EU,EU,EU,EU, 
    /* 91~120 */                      
    EU,EU,EU,EU,EU,ES,ES,ES,ES,EU,  EU,EU,EU,EU,EU,EU,EU,EU,ES,EU,  EU,EU,EU,EU,EU,EU,EU,EU,ES,ES, 
    /* 121~150 */                      
    ES,ES,ES,ES,EF,ES,ES,ES,ES,EF,  EF,ES,ES,EU,EU,EU,EU,ES,EU,EU,  EU,EU,EU,EU,EU,EU,EU,EU,EU,EU, 
    /* 151~180 */                      
    EU,EU,EU,EU,EU,EU,EU,EU,EU,EU,  EU,EU,EU,EU,EU,EU,EU,EU,EU,EU,  EU,EU,EU,EU,EU,EU,EU,EU,EU,EF, 
    /* 181~210 */                      
    EU,EU,EU,EU,EU,EU,EU,EU,EU,EU,  EU,EU,EU,EU,EU,EU,EU,EU,EU,EU,  ES,ES,ES,EU,EU,EU,EU,EU,EF,EF, 
    /* 211~240 */                      
    EU,EU,EU,EU,EU,EU,EU,EU,EU,EU,  EU,EU,EU,EU,EU,EU,EU,EU,EU,EU,  EU,EU,EU,EU,EU,EU,EU,EU,EU,EU, 
    /* 241~270 */                      
    EU,EU,EU,EU,EU,EU,EU,EU,EU,EU,  EU,EU,EU,EU,EU,EU,EF,EF,EF,EF,  ES,ES,ES,EF,EF,EF,EF,EF,EF,EF, 
    /* 271~300 */                      
    ES,ES,ES,ES,ES,ES,ES,ES,EF,EF,  EF,EF,EF,EF,EF,EF,EF,EF,EF,EF,  ES,EF,EF,ER,ER,ES,ES,ES,ES,EF,  
    /* 301~330 */                      
    EF,ES,ES,ES,ES,EF,EF,EF,EF,ES,  ES,ES,ES,ES,ES,ES,ES,ES,ES,ES,  ES,ES,ES,ES,ES,ES,EF,EF,ES,ES, 
    /* 331~360 */                      
    ES,ES,ES,ES,ES,EF,EF,EF,ES,ES,  ES,ES,ES,EF,EF,EF,EU,EU,EU,EU,  EU,EU,EU,EU,EU,EU,EU,EU,EU,EU, 
    /* 361~390 */                      
    EU,EU,ES,ES,ES,ES,ES,ES,ES,ES,  ES,ES,ES,ES,ES,ES,ES,ES,ES,ES,  ES,ES,ES,ES,ES,ES,ES,ES,ES,ES, 
    /* 391~420 */                      
    ES,ES,ES,ES,ES,ES,ES,ES,ES,ES,  ES,ES,ES,ES,ES,ES,ES,EF,EF,EF,  EF,EF,EF,EF,EF,EF,EF,EF,EF,EF, 
    /* 421~450 */                      
    EF,EF,EF,EF,ES,ES,ES,ES,ES,ES,  ES,ES,ES,ES,ES,ES,ES,ES,ES,ES,  ES,ES,ES,ES,ES,ES,ES,ES,ES,ES, 
    /* 451~480 */                      
    ES,ES,ES,ES,ES,ES,ES,ES,ES,ES,  ES,ES,ES,ES,ES,ES,ES,ES,ES,ES,  ES,ES,ES,ES,ES,ES,ES,ES,ES,ES, 
    /* 481~512 */                      
    ES,ES,ES,ES,ES,ES,ES,ES,ES,ES,  ES,ES,ES,ES,ES,ES,ES,EU,EU,EU,  EU,EU,EU,EU,EU,EU,EU,EU,EU,EU, EU,EU 
};

/*******************************************************************************
* Function Name  : esc_error_display
* Description    : esc error display
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void esc_error_display(void)
{
    if( ESC_ERROR_CODE[0] || CBEscData.WarnCodeSafety[0] )
    {
        if( ESC_ERROR_CODE[0] )
        {
            /* error display */
            if( ESC_ERROR_CODE[0] <= 100u )
            {
                dis_data[0] = 10u;
                dis_data[1] = (ESC_ERROR_CODE[0] - 1u)%100/10;
                dis_data[2] = (ESC_ERROR_CODE[0] - 1u)%10;
            }
            else
            {
                dis_data[0] = (ESC_ERROR_CODE[0] - 1u)/100;
                dis_data[1] = (ESC_ERROR_CODE[0] - 1u)%100/10;
                dis_data[2] = (ESC_ERROR_CODE[0] - 1u)%10;                    
            }
        }
        else
        {
            /* warnning display */
            dis_data[0] = 11u;
            dis_data[1] = (CBEscData.WarnCodeSafety[0] - 1u)%100/10;
            dis_data[2] = (CBEscData.WarnCodeSafety[0] - 1u)%10;
        }
        
    }
    else
    {
        /* no fault */
        dis_data[0] = 0u;
        dis_data[1] = 0u;
        dis_data[2] = 0u;                
    }
}


/*******************************************************************************
* Function Name  : fault_code_reset
* Description    : 
* Input          : None            
* Output         : None
* Return         : None
*******************************************************************************/
void fault_code_reset(void)
{  
    u8 i,j;
    static u8 stat_u8LastReset = 0u;
    
    
    if(( stat_u8LastReset == 0u ) && ( CMD_SF_ORDER & ORDER_FROM_SF_RESET ))
    {
        if( ErrorCodeControl[0] )
        {
            i = (ErrorCodeControl[0]-1u)/8u;
            j = (ErrorCodeControl[0]-1u)%8u;
            
            CBEscData.ErrorBuff[i] &= ~( 1u << j );
            
            ErrorCodeControl[4] = 0u;
            for( i = 0u; i < 4u; i++ )
            {
                ErrorCodeControl[i] = ErrorCodeControl[i + 1u];
            }
        }
    }
    
    if( CMD_SF_ORDER & ORDER_FROM_SF_RESET )
    {
        stat_u8LastReset = 1u;
    }
    else
    {
        stat_u8LastReset = 0u;
    }
}

/*******************************************************************************
* Function Name  : fault_code_manual_reset
* Description    : 
* Input          : ResetType: 1: statand fault reset, 2: failure lock reset            
* Output         : None
* Return         : None
*******************************************************************************/
void fault_code_manual_reset(u8 ResetType)
{  
    u16 i,j;
    
    if(( ErrorCodeControl[0] ) && ( EscFaultType[ErrorCodeControl[0]] <= ResetType ))
    {
        i = (ErrorCodeControl[0]-1u)/8u;
        j = (ErrorCodeControl[0]-1u)%8u;
        
        CBEscData.ErrorBuff[i] &= ~( 1u << j );
        
        ErrorCodeControl[4] = 0u;
        for( i = 0u; i < 4u; i++ )
        {
            ErrorCodeControl[i] = ErrorCodeControl[i + 1u];
        }
    }
}

/*******************************************************************************
* Function Name  : CheckReset
* Description    : Check reset.
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void CheckReset(void)
{
    static u16 stat_u16TimerResetPress = 0u;
   
    /* reset Button */
    if(( CBEscData.SafetyInputDataCHA[1] & 0x01u ) && ( CBEscData.SafetyInputDataCHB[1] & 0x01u ))
    {
        stat_u16TimerResetPress++;
    }
    else
    {       
        if(((stat_u16TimerResetPress * 5u) > KEY_MINIMUM_TIME ) && ((stat_u16TimerResetPress * 5u) < 5000u ))
        {            
            /* Reset a fault */
            if( CbBase_EscState == ESC_FAULT_STATE )
            {
                fault_code_manual_reset(2u);                
            }                    
        }
        stat_u16TimerResetPress = 0u;
    }  
}

/*******************************************************************************
* Function Name  : fault_code_decode
* Description    : The maxium number of decode is 5.
* Input          : code_buff : recode fault parameter.            
* Output         : None
* Return         : None
*******************************************************************************/
void fault_code_decode(u16 code_buff[])
{  
    
    u16 i = 0u,j = 0u,error_counter = 0u,error_code_temp = 0u;
    u16 error_temp[5]={0,0,0,0,0};
    
       
    for( i = 0u; i < 64u; i++ ) 
    {
        EscErrorBuff[i] = CBEscData.ErrorBuff[i];				
    }  
    
    error_counter = 0u;
    error_code_temp = 0u;
    
    for( i = 0u; i < 64u; i++ )
    {
        error_code_temp = i*8u;
        
        if(EscErrorBuff[i])
        {
            for( j = 0u; j < 8u; j++ )
            {
                error_code_temp++;
                if(EscErrorBuff[i] & (1u << j)) 
                {
                    error_temp[error_counter] = error_code_temp; 
                    error_counter++;
                    
                    if(error_counter >= 5u) 
                    {
                        break;
                    }
                }  
            } 
            
            g_u16LedFreq = FREQ_3HZ;
        } 
        
        if(error_counter >= 5u) 
        {
            break;
        }
    }    
    
    if( error_counter > 0u )
    {
        for( i = 0u; i < error_counter; i++ )
        {
            if(error_temp[i])
            {
                for( j = 0u; j < 5u; j++ )
                {
                    if( ((code_buff[j]) && ( code_buff[j] == error_temp[i] )) || ( code_buff[j] == 0u ))
                    {
                        if( code_buff[j] == 0u )
                        {
                            code_buff[j] = error_temp[i];
                        }
                        break;
                    }
                }
            }
        }
    }
    else
    {
        code_buff[0] = 0u;	
        code_buff[1] = 0u;	
        code_buff[2] = 0u;	
        code_buff[3] = 0u;	
        code_buff[4] = 0u;	
        
        g_u16LedFreq = FREQ_1HZ;
    }  
    
    /* communicate error */
    if( EN_ERROR48 & 0x04u )
    {
        ESC_ERROR_CODE[0] = 379u;
    }
    else
    {
        /* Combine Safety and Control fault */
        for( i = 0u; i < 5u; i++ )
        {
            if( (CBEscData.ErrorCodeSafetyCHA[i]) || (CBEscData.ErrorCodeSafetyCHB[i]) )
            {
                if( CBEscData.ErrorCodeSafetyCHA[i] )
                {
                    ESC_ERROR_CODE[i] = CBEscData.ErrorCodeSafetyCHA[i];
                }
                else
                {
                    ESC_ERROR_CODE[i] = CBEscData.ErrorCodeSafetyCHB[i];
                }
            }
            else
            {
                break;
            }
        }
        for( j = 0u; j < ( 5u - i ); j++ )
        {
            if( ErrorCodeControl[j] )
            {
                ESC_ERROR_CODE[i+j] = ErrorCodeControl[j];
            }
            else
            {
                ESC_ERROR_CODE[i+j] = 0u;
            }
        }  
    }
}


/******************************  END OF FILE  *********************************/

