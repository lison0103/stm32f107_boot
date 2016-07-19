/*******************************************************************************
* File Name          : esc_parameter_process.c
* Author             : lison
* Version            : V1.0
* Date               : 07/15/2016
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


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : esc_para_init
* Description    : esc parameter initialization, set the default value.                 
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void esc_para_init(u8 *para)
{
       
    LANGUAGE = para[0];

}


/*******************************************************************************
* Function Name  : get_para_from_usb
* Description    : get esc parameter from usb.                 
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void get_para_from_usb(void)
{
    u8 paradata[200];
    u8 len = 0;
    u16 waittms = 0;
    
    /* 1. Waiting for message from CPU1 to start parameter loading process */
    do
    {
        len = BSP_CAN_Receive(CAN1, &CAN1_RX_Normal, paradata, 0);
        delay_ms(1);
        EWDT_TOOGLE();
        waittms++;
        if( waittms > 5000 )
        {
            waittms = 0;
            break;
        }
    }
    while( len != 2 && paradata[0] != 0x22 );
        

    
    if( paradata[1] == 0x01 )
    {
        /* 2. wait sf load usb-stick, recv para from sf CPU1 */
        do
        {
            len = BSP_CAN_Receive(CAN1, &CAN1_RX_Normal, paradata, 0);
            delay_ms(1);
            EWDT_TOOGLE();
            waittms++;
            if( waittms > 15000 )
            {
                waittms = 0;
                break;
            }
        }
        while( len == 0 );
        
    }
        
    if( len > 0 )
    {
        /* 3. Check crc16 is it ok */
        if( MB_CRC16( paradata, len ))
        {
            /* Error message. Abort parameter loading. System remains in Init Fault. */

        }
        else
        {
            /* 4. Store the parameters in the fram */
            AT24CXX_Write(EEP_PARA_RECORD_ADR , len, paradata);       
        }
        
    }
    else
    {
        /* 3. CPU1 Read parameters from non volatile memory */
        AT24CXX_Read(EEP_ERROR_RECORD_ADR, EEP_PARA_RECORD_NUM, paradata);
    
        /* 4. Check crc16 is it ok */
        if( MB_CRC16( paradata, EEP_PARA_RECORD_NUM ))
        {
            /* Error message. Abort parameter loading. System remains in Init Fault. */

        }
        else
        {
            /* 5. Save parameters into variables */
            esc_para_init(paradata);    
        }    
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
      get_para_from_usb();
      
}



/******************************  END OF FILE  *********************************/






