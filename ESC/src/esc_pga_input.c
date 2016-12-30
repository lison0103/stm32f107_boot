/*******************************************************************************
* File Name          : esc_pga_input.c
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
static void pga_input_decode_item(u16 ucPgaNum, u8 ucLevel);  /* pga iput decode sub */



/*******************************************************************************
* Function Name  : pga_input_decode_item
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void pga_input_decode_item(u16 ucPgaNum, u8 ucLevel)
{
  u16 temp1,ucByte,ucBit;
  
  temp1 = ucPgaNum-1u;
  
  ucByte = temp1>>3u;    /* byte address */
  ucBit = temp1%8u;      /* bit */
  
  CBEscData.Cfg_Input_Mask[ucByte] |= (1u<<ucBit); /* Function enable  */
            
  if(ucLevel) 
  {
    CBEscData. Cfg_Input_Level[ucByte] |= (1u<<ucBit); /* input 1 */
  }
}


/*******************************************************************************
* Function Name  : pga_input_decode
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void pga_input_decode(void)
{
    u8 i;
    u32 *pConfigInputA;
    u32 *pConfigInputB;
    
    for(i=0u;i<25u;i++)
    {
        CBEscData. Cfg_Input_Mask[i] = 0u; 
        CBEscData. Cfg_Input_Level[i] = 0u;
    }  
    
    /* Safety board input */
    pConfigInputA = (u32*)&CBEscData.SafetyInputDataCHA[0];
    pConfigInputB = (u32*)&CBEscData.SafetyInputDataCHB[0];
    for( i = 0u; i < 29u; i++ )
    {
        if( IO_PARAMETER_SAFETY_INPUT[i] )
        {
            pga_input_decode_item(IO_PARAMETER_SAFETY_INPUT[i], (((u8)(*pConfigInputA >> i) & 0x01u) & ((u8)(*pConfigInputB >> i) & 0x01u)));
        }
    }
    
    pConfigInputA = (u32*)&CBEscData.SafetyInputDataCHA[4];
    pConfigInputB = (u32*)&CBEscData.SafetyInputDataCHB[4];
    for( i = 29u; i < 46u; i++ )
    {
        if( IO_PARAMETER_SAFETY_INPUT[i] )
        {
            pga_input_decode_item(IO_PARAMETER_SAFETY_INPUT[i], (((u8)(*pConfigInputA >> (i-29u)) & 0x01u) & ((u8)(*pConfigInputB >> (i-29u)) & 0x01u)));
        }
    }  

    /* Control board input */
    pConfigInputA = (u32*)&CBEscData.ControlInputData[0];
    for( i = 0u; i < 16u; i++ )
    {
        if( IO_PARAMETER_CONTROL_INPUT[i] )
        {
            pga_input_decode_item(IO_PARAMETER_CONTROL_INPUT[i], ((u8)(*pConfigInputA >> i) & 0x01u));
        }
    }   
}

/******************************  END OF FILE  *********************************/