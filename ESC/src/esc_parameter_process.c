/*******************************************************************************
* File Name          : esc_parameter_process.c
* Author             : lison
* Version            : V1.0
* Date               : 07/15/2016
* Last modify date   : 10/12/2016
* Description        : This file contains esc parameter process.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "esc_parameter_process.h"
#include "crc16.h"
#include "delay.h"
#include "esc.h"
#include "can.h"
#include "24cxx.h"
#include "ewdt.h"
#include "includes.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

SFPara SFParameterData;
CBPara CBParameterData;
CBParaInSF CBParameterInSafety;

u8 DIAGNOSTIC = 0u;
u8 g_u8ParameterLoadingOK = 0u;
u8 g_u8ParameterRead = 0u;

/*******************************************************************************
* Function Name  : esc_para_init
* Description    : esc parameter initialization, set the default value.                 
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void EscParameterInit(void)
{  
    /* SF para */
    KEY_MINIMUM_TIME = 800u; 
    DIAGNOSTIC_BOARD_L2_QUANTITY = 0u;
    
    /* CB para */
    STAR_DELTA_DELAY = 3u;
    DIAGNOSTIC_BOARD_L1_QUANTITY = 0u;

    /*Static IP ADDRESS*/
    IP_ADDRESS_BYTE1 = 10u;
    IP_ADDRESS_BYTE2 = 129u;
    IP_ADDRESS_BYTE3 = 199u;
    IP_ADDRESS_BYTE4 = 200u;
    
    /*NETMASK*/
    SUBNET_MASK_BYTE1 = 255u;
    SUBNET_MASK_BYTE2 = 255u;
    SUBNET_MASK_BYTE3 = 255u;
    SUBNET_MASK_BYTE4 = 0u;
    
    /*Gateway Address*/
    GATEWAY_BYTE1 = 10u;
    GATEWAY_BYTE2 = 129u;
    GATEWAY_BYTE3 = 199u;
    GATEWAY_BYTE4 = 2u;    
    
    if(( DIAGNOSTIC_BOARD_L1_QUANTITY != 0u ) && ( DIAGNOSTIC_BOARD_L2_QUANTITY == 0u ))
    {
        DIAGNOSTIC = DIAGNOSTIC_BOARD_1;
    }
    else if(( DIAGNOSTIC_BOARD_L1_QUANTITY == 0u ) && ( DIAGNOSTIC_BOARD_L2_QUANTITY != 0u ))
    {
        DIAGNOSTIC = DIAGNOSTIC_BOARD_2;
    }    
    else
    {
        DIAGNOSTIC = NO_DIAGNOSTIC_BOARD;
    }     
    
}



/*******************************************************************************
* Function Name  : ParametersLoading
* Description    : esc get parameter.                 
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void ParametersLoading(void)
{
    u16 *pCBparalen = (u16*)&ParaDataFromSafety[0][4];
    
    
    if( CbBase_EscState == ESC_INIT_STATE )
    {
        
        if( ParaDataFromSafety[0][0] == 0x31u )
        {
            if(( g_u8ParameterLoadingOK == 0u ) && ( g_u16ParameterLoadingError == 0u ))
            {
                /* receive cb para */
                if(( ParaDataFromSafety[0][3] == 1u ) && ( *pCBparalen == ESC_CB_PARAMETER_DATA_LEN ))
                {
                    if( MB_CRC32( (u8*)&CBParameterData, ESC_CB_PARAMETER_DATA_LEN, PARAMETER_POLYNOMIALS ))
                    {
                        /* Error message. Abort parameter loading. System remains in Init Fault. */
                        EN_ERROR49 |= 0x02u;   
                        
                        g_u16ParameterLoadingError = 1u;  
                    }
                    else
                    {         
                        g_u8ParameterLoadingOK = 1u;
                    }
                }
            }
        }
        else if( ParaDataFromSafety[0][0] == 0x02u )
        {
            if( g_u8ParameterRead == 0u )
            {
                g_u8ParameterRead = 1u;
            }
            
            if(( g_u8ParameterLoadingOK == 0u ) && ( g_u16ParameterLoadingError == 0u ))
            {
                /* receive sf para */
                if(( ParaDataFromSafety[0][3] == 1u ) && ( *pCBparalen == ESC_SF_PARAMETER_DATA_LEN ))
                {
                    if( MB_CRC32( (u8*)&SFParameterData, ESC_SF_PARAMETER_DATA_LEN, PARAMETER_POLYNOMIALS ))
                    {
                        /* Error message. Abort parameter loading. System remains in Init Fault. */
                        EN_ERROR49 |= 0x02u;   
                        
                        g_u16ParameterLoadingError |= 0x01u;  
                    }
                    else
                    {         
                        g_u8ParameterLoadingOK = 2u;
                    }
                }  
            }
            
            if((( g_u8ParameterLoadingOK == 2u ) && ( g_u8ParameterRead == 4u )) || (( g_u16ParameterLoadingError == 0x01u ) && ( g_u8ParameterRead == 4u ))
               || (( g_u16ParameterLoadingError == 0x02u ) && ( g_u8ParameterLoadingOK == 2u )))
            {
                g_u8ParameterLoadingFinish = 1u;
            }
        }
        else
        {}
        
    }
    else if(( CbBase_EscState == ESC_READY_STATE ) || ( CbBase_EscState == ESC_FAULT_STATE ))
    {
        /* Parameter changed from the DDU */
        
    }
    else
    {}
}



/******************************  END OF FILE  *********************************/






