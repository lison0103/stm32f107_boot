/*******************************************************************************
* File Name          : esc_eeprom_process.c
* Author             : lison
* Version            : V1.0
* Date               : 06/13/2016
* Description        : This file contains esc eeprom process.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "esc_eeprom_process.h"
#include "lsys.h"
#include "includes.h"
#include "esc.h"
#include "24cxx.h"
#include "rtc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


u16 errorRecordFirstItem_old = 0;
u8 *const pcCurrentRecordErrorBuff = &Modbuff[506];//8

/*******************************************************************************
* Function Name  : get_error_fault_from_eeprom
* Description    : Get error record from eeprom.
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
void error_record_incode(void)
{
    u16 i=0,j=0;    

    (*ptErrorRecordCounter)++; 
    if((*ptErrorRecordCounter)>300) (*ptErrorRecordCounter)=300;   
    
    pcCurrentRecordErrorBuff[0] = 0x7F; 
    pcCurrentRecordErrorBuff[1] = ESC_ERROR_CODE[0];
    pcCurrentRecordErrorBuff[2] = pcMbRtccBuff[0];
    pcCurrentRecordErrorBuff[3] = pcMbRtccBuff[1];
    pcCurrentRecordErrorBuff[4] = pcMbRtccBuff[2];
    pcCurrentRecordErrorBuff[5] = pcMbRtccBuff[3];
    pcCurrentRecordErrorBuff[6] = pcMbRtccBuff[4];
    pcCurrentRecordErrorBuff[7] = pcMbRtccBuff[5];
    
    (*ptErrorRecordFirstItem)++;                                           
    if(((*ptErrorRecordFirstItem)>300) || (!(*ptErrorRecordFirstItem))) 
    {
        (*ptErrorRecordFirstItem) = 1;
    }
    
    i = ((*ptErrorRecordFirstItem)-1)*8;                          
    
    for(j=0;j<8;j++)
    {	
        EscErrorBuff[i+j] = pcCurrentRecordErrorBuff[j];
    } 
	
}

/*******************************************************************************
* Function Name  : get_error_fault_from_eeprom
* Description    : Get error record from eeprom.
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
void error_record_check(void)
{
    static u8 esc_stop_record_en=0,error_record_en=0,esc_run_to_stop_tms=0,error_tms=0;
    static u8 esc_error_code_old=0;
    
    /* escalator running*/
    if(ESC_STATE1&0x01) 
    {
        esc_stop_record_en = 1;
        esc_run_to_stop_tms = 0; 
    }
    /* escalator stop */
    else
    {
        if(esc_run_to_stop_tms<250) esc_run_to_stop_tms++;
        
        if((esc_run_to_stop_tms<100) && (ESC_ERROR_CODE[0]) && (esc_stop_record_en))
        {
            esc_stop_record_en = 0;	
            error_record_en=1;	   					
        }
    }
    
    //记录所有故障模式，同时非检修
//    if( !(ESC_STATE1&0x01))   
//    {
        //有故障
        if(ESC_ERROR_CODE[0])            
        {
            if(error_tms<200) error_tms++;
        }
        else
        {
            error_tms=0;
        }
        
        if((ESC_ERROR_CODE[0]) && (ESC_ERROR_CODE[0]!=esc_error_code_old))
        {
            error_tms = 0;	
        }			
        
        if((error_tms==5) && (esc_run_to_stop_tms>200))   		
        {
            if(((ESC_STATE1&0x01)) && (POWER_ON_TMS>500)) error_record_en = 1; // && (!(CMD_FLAG4&0x80))             	
        }	          
//    }		        
    
    if(error_record_en)	 			
    {
        error_record_incode();			
        error_record_en = 0;	
    }
    
    esc_error_code_old = ESC_ERROR_CODE[0];
}


/*******************************************************************************
* Function Name  : get_error_fault_from_eeprom
* Description    : Get error record from eeprom.
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/ 
void get_error_record_from_eeprom(void)
{
    /* error records */

    if(AT24CXX_Read(EEP_ERROR_RECORD_ITEM_ADR, 4, &Modbuff[500])) 
    {
        /* error */
        EN_ERROR1 |= 0x01;
    } 	
    
    errorRecordFirstItem_old = *ptErrorRecordFirstItem;
    
    if(AT24CXX_Read(EEP_ERROR_RECORD_ADR, EEP_ERROR_RECORD_NUM, EscErrorBuff)) 
    {
        /* error */
        EN_ERROR1 |= 0x01;
    } 			  
}

/*******************************************************************************
* Function Name  : write_error_record_to_eeprom
* Description    : Write error record to eeprom.
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/ 
void write_error_record_to_eeprom(void)
{
    u16 i;
    
    
    /* fault record */

    if((errorRecordFirstItem_old != *ptErrorRecordFirstItem) && (*ptErrorRecordFirstItem))
    {
        i = ((*ptErrorRecordFirstItem)-1)*8;                          
        if(AT24CXX_Write(EEP_ERROR_RECORD_ADR + i, 8, &EscErrorBuff[i]))
        {
            /* error */
            EN_ERROR1 |= 0x01;        
        }
        
        if(AT24CXX_Write(EEP_ERROR_RECORD_ITEM_ADR, 4, &Modbuff[500]))
        {
            /* error */
            EN_ERROR1 |= 0x01;        
        }        
    }
    
    errorRecordFirstItem_old = *ptErrorRecordFirstItem;	
			  
}

/******************************  END OF FILE  *********************************/



