/*******************************************************************************
* File Name          : esc_pga_output.c
* Author             : Lison/Tu
* Version            : V1.0
* Date               : 12/19/2016
* Last modify date   : 12/19/2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "bsp_iocfg.h"
#include "esc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void pga_output_decode_item(u16 ucPgaNum, u8 *output);  /* pga output decode sub */




/*******************************************************************************
* Function Name  : pga_output_decode_item
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void pga_output_decode_item(u16 ucPgaNum, u8 *output)
{
    u16 temp1,ucByte,ucBit;
    
    temp1 = ucPgaNum-1u;
    
    ucByte = temp1>>3u;    /* byte address */
    ucBit = temp1%8u;      /* bit */
    
    CBEscData.Cfg_Output_Mask[ucByte] |= (1u<<ucBit); /* Function enable  */
    

}

/*******************************************************************************
* Function Name  : pga_input_decode
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void pga_output_decode(void)
{
    u8 i;
    
    for(i=0u;i<25u;i++)
    {
        CBEscData. Cfg_Output_Mask[i] = 0u; 
        CBEscData. Cfg_Output_Level[i] = 0u;
    }  
    
    
    /* Control board output */
    for( i = 0u; i < 15u; i++ )
    {
        if( IO_PARAMETER_CONTROL_OUTPUT[i] )
        {
            pga_output_decode_item(IO_PARAMETER_CONTROL_OUTPUT[i], &CBEscData.ControlOutputData[0]);
        }
    } 
    
    
    if( DIAGNOSTIC == DIAGNOSTIC_BOARD_1 )
    {
        /* DBL1 upper output */
        for( i = 0u; i < 8u; i++ )
        {
            if( IO_PARAMETER_DBL1_UPPER_OUTPUT[i] )
            {
                pga_output_decode_item(IO_PARAMETER_CONTROL_OUTPUT[i], &CBEscData.ControlOutputData[0]);
            }
        }     
        
        /* DBL1 lower output */
        for( i = 0u; i < 8u; i++ )
        {
            if( IO_PARAMETER_DBL1_LOWER_OUTPUT[i] )
            {
                pga_output_decode_item(IO_PARAMETER_CONTROL_OUTPUT[i], &CBEscData.ControlOutputData[0]);
            }
        }    
        
        /* DBL1 interm1 output */
        for( i = 0u; i < 8u; i++ )
        {
            if( IO_PARAMETER_DBL1_INTERM1_OUTPUT[i] )
            {
                pga_output_decode_item(IO_PARAMETER_CONTROL_OUTPUT[i], &CBEscData.ControlOutputData[0]);
            }
        }      
        
        /* DBL1 interm2 output */
        for( i = 0u; i < 8u; i++ )
        {
            if( IO_PARAMETER_DBL1_INTERM2_OUTPUT[i] )
            {
                pga_output_decode_item(IO_PARAMETER_CONTROL_OUTPUT[i], &CBEscData.ControlOutputData[0]);
            }
        } 
    }
    else if( DIAGNOSTIC == DIAGNOSTIC_BOARD_2 )
    {
        /* DBL2 upper output */
        for( i = 0u; i < 8u; i++ )
        {
            if( IO_PARAMETER_DBL2_UPPER_OUTPUT[i] )
            {
                pga_output_decode_item(IO_PARAMETER_CONTROL_OUTPUT[i], &CBEscData.ControlOutputData[0]);
            }
        }     
        
        /* DBL2 lower output */
        for( i = 0u; i < 8u; i++ )
        {
            if( IO_PARAMETER_DBL2_LOWER_OUTPUT[i] )
            {
                pga_output_decode_item(IO_PARAMETER_CONTROL_OUTPUT[i], &CBEscData.ControlOutputData[0]);
            }
        }    
        
        /* DBL2 interm1 output */
        for( i = 0u; i < 8u; i++ )
        {
            if( IO_PARAMETER_DBL2_INTERM1_OUTPUT[i] )
            {
                pga_output_decode_item(IO_PARAMETER_CONTROL_OUTPUT[i], &CBEscData.ControlOutputData[0]);
            }
        }      
        
        /* DBL2 interm2 output */
        for( i = 0u; i < 8u; i++ )
        {
            if( IO_PARAMETER_DBL2_INTERM2_OUTPUT[i] )
            {
                pga_output_decode_item(IO_PARAMETER_CONTROL_OUTPUT[i], &CBEscData.ControlOutputData[0]);
            }
        }         
    }
    else
    {}
    
}

/******************************  END OF FILE  *********************************/