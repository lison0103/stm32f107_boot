/*******************************************************************************
* File Name          : esc_eeprom_process.c
* Author             : lison
* Version            : V1.0
* Date               : 06/13/2016
* Last modify date   : 10/12/2016
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
#include "crc16.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ERROR_BACKUP_STORE_TIME 1000u

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
u8 eep_write_data(u16 Adr, u16 len, u8 WriteData[], u32 Polynomials);
u8 eep_read_data(u16 Adr, u16 len, u8 ReadData[]);

u16 errorRecordFirstItem_old = 0;
u8 pcCurrentRecordErrorBuff[8];

u8 FramWriteBuffer[100];

/*******************************************************************************
* Function Name  : Check_Error_Present_Memory
* Description    : esc check error in memory.                 
* Input          : None          
* Output         : None
* Return         : return last power off esc state
*******************************************************************************/
u8 Check_Error_Present_Memory(void)
{
    u8 readbuffer[100];
    u8 result = 0u;
    u8 i;
    u16 temp;
    u8 state = 0u;
    
    result = eep_read_data(ESC_ERROR_ADR, ESC_ERROR_NUM, readbuffer);
    if( !result )
    {
        if( (readbuffer[0] == 0xfau) && (readbuffer[1] == 0xedu))
        {
            /* 5 Error Code */
            for( i = 0u; i < 5u; i++ )
            {
                CBEscData.ErrorCode[i] |= readbuffer[i*2u + 2u];
                temp = (u16)readbuffer[i*2u + 3u] << 8u;
                CBEscData.ErrorCode[i] |= temp;
            }
            
            /* Error Buffer */
            for( i = 0u; i < 64u; i++ )
            {
                CBEscData.ErrorBuff[i] = readbuffer[i + 12u];
                if( CBEscData.ErrorBuff[i] )
                {
                    result = 1u;
                }
            }     
        }
    }
    else
    {
        /* epprom read error */
        EN_ERROR34 |= 0x01; 
    }
    
    if(( CBEscData.ErrorCode[0] ) || ( result ))
    {
        state = ESC_FAULT_STATE;
    }
    else
    {
        state = ESC_READY_STATE;
    }
    
    return state;
}

/*******************************************************************************
* Function Name  : StoreFaultInMemory
* Description    : Store Fault code In Memory.
* Input          : None            
* Output         : None
* Return         : None
*******************************************************************************/
void StoreFaultInMemory(void)
{
    static u16 stat_u16TimerStoreFault = 0u;
    static u16 ErrorBuffCRC = 0u;
    static u8 g_u8FaultCodeStore = 0u;
    u8 errorbuff[64];
    u8 u8DataStore = 0u;
    u16 i;
    
    if(( CbBase_EscState == ESC_FAULT_STATE ) || ( CbBase_EscState == ESC_READY_STATE ))
    {
        /* check error code change */
        for( i = 0u; i < 64u; i++ ) 
        {
            errorbuff[i] = CBEscData.ErrorBuff[i];
        }     
        
        i = MB_CRC16( errorbuff, 64u );
        
        if( i != ErrorBuffCRC )
        {
            ErrorBuffCRC = i;
            g_u8FaultCodeStore = 1u;
        }
        
        
        
        if( g_u8FaultCodeStore == 1u )
        {
            u8DataStore = 1u;
            g_u8FaultCodeStore = 2u;
            stat_u16TimerStoreFault = 0u;
        }
        else
        {
            if( g_u8FaultCodeStore == 2u )
            {
                stat_u16TimerStoreFault++;
                if( stat_u16TimerStoreFault * 10u > ERROR_BACKUP_STORE_TIME )
                {
                    u8DataStore = 1u;
                    g_u8FaultCodeStore = 0u;
                }
            } 
        }
        
        if( u8DataStore == 1u )
        {
            /* first, clear buffer */
            for( i = 0u; i < ESC_ERROR_NUM; i++ )
            {
                FramWriteBuffer[i] = 0u;
            }
            
            /* Header */
            FramWriteBuffer[0] = 0xfau;
            FramWriteBuffer[1] = 0xedu;
            
            /* 5 Error Code */
            for( i = 0u; i < 5u; i++ )
            {
                FramWriteBuffer[i*2u + 2u] = (u8)(CBEscData.ErrorCode[i] & 0xffu);
                FramWriteBuffer[i*2u + 3u] = (u8)((CBEscData.ErrorCode[i] >> 8u) & 0xffu);
            }
            
            /* Error FramWriteBuffer */
            for( i = 0u; i < 64u; i++ )
            {
                FramWriteBuffer[i + 12u] = CBEscData.ErrorBuff[i];
            }
            
            /* CRC16 */
            /*
            i = MB_CRC16( FramWriteBuffer, ESC_ERROR_NUM - 2u );
            FramWriteBuffer[ESC_ERROR_NUM - 2u] = (u8)i;
            FramWriteBuffer[ESC_ERROR_NUM - 1u] = (u8)(i >> 8u);
            */
            
            /* record in eeprom */
            if( g_u8FaultCodeStore )
            {
                eep_write_data(ESC_ERROR_ADR, ESC_ERROR_NUM, FramWriteBuffer, 1u);
            }
            else
            {
                /* backup */
                eep_write_data(ESC_ERROR_BACKUP_ADR, ESC_ERROR_NUM, FramWriteBuffer, 1u);
            }
            
            u8DataStore = 0u;
        } 
       
    }
}

/*******************************************************************************
* Function Name  : StoreParaInMemory
* Description    : Store Parameter code In Memory.
* Input          : None            
* Output         : None
* Return         : None
*******************************************************************************/
void StoreParaInMemory(void)
{
    if( CbBase_EscState == ESC_INIT_STATE )
    {
        /* record in eeprom */
        if(( g_u8ParameterLoadingOK == 1u ) && ( g_u16ParameterLoadingError == 0u ))
        {
            if(!eep_write_data(ESC_PARA_ADR, ESC_PARA_NUM, (u8*)&CBParameterData, 0u))
            {
                vTaskDelay( 500 );
                
                /* backup */
                if(!eep_write_data(ESC_PARA_BACKUP_ADR, ESC_PARA_NUM, (u8*)&CBParameterData, 0u))
                {
                    g_u8ParameterLoadingOK = 2u;
                }
                else
                {
                    g_u16ParameterLoadingError = 2u;
                }
            }
            else
            {
                g_u16ParameterLoadingError = 2u;
            }
        }  
    }
}

/*******************************************************************************
* Function Name  : ReadParaFromMemory
* Description    : 
* Input          : None            
* Output         : None
* Return         : None
*******************************************************************************/
void ReadParaFromMemory(void)
{
    u8 result = 0u;
    
    if( CbBase_EscState == ESC_INIT_STATE )
    {
        if( g_u8ParameterLoadingOK == 2u )
        {
            if(( g_u8ParameterRead == 1u ) && (!( g_u16ParameterLoadingError & 0x02u )))
            {
                /* read cb para */
                result = eep_read_data(ESC_PARA_ADR, ESC_CB_PARAMETER_DATA_LEN, (u8*)&CBParameterData);
                if( !result )
                {                       
                    g_u8ParameterRead = 2u;
                }
                else
                {
                    g_u16ParameterLoadingError |= 0x02u;
                    
                    /* epprom read error */
                    EN_ERROR34 |= 0x01; 
                }
            } 
        }
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
void error_record_incode(void)
{
    u16 i=0,j=0;    

    ErrorRecordCounter++; 
    if( ErrorRecordCounter > 300 ) 
    {
        ErrorRecordCounter = 300;   
    }
    
    pcCurrentRecordErrorBuff[0] = 0x7F; 
    pcCurrentRecordErrorBuff[1] = ESC_ERROR_CODE[0];
    pcCurrentRecordErrorBuff[2] = ESC_RTCC[0];
    pcCurrentRecordErrorBuff[3] = ESC_RTCC[1];
    pcCurrentRecordErrorBuff[4] = ESC_RTCC[2];
    pcCurrentRecordErrorBuff[5] = ESC_RTCC[3];
    pcCurrentRecordErrorBuff[6] = ESC_RTCC[4];
    pcCurrentRecordErrorBuff[7] = ESC_RTCC[5];
    
    ErrorRecordFirstItem++;                                           
    if((ErrorRecordFirstItem > 300) || (!ErrorRecordFirstItem)) 
    {
        ErrorRecordFirstItem = 1;
    }
    
    i = (ErrorRecordFirstItem - 1)*8;                          
    
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
    if( CbBase_EscState & ESC_RUN_STATE ) 
    {
        esc_stop_record_en = 1;
        esc_run_to_stop_tms = 0; 
    }
    /* escalator stop */
    else
    {
        if(esc_run_to_stop_tms < 250) esc_run_to_stop_tms++;
        
        if((esc_run_to_stop_tms < 100) && (ESC_ERROR_CODE[0]) && (esc_stop_record_en))
        {
            esc_stop_record_en = 0;	
            error_record_en = 1;	   					
        }
    }
    

    /* A fault */
    if(ESC_ERROR_CODE[0])            
    {
        if(error_tms < 200) error_tms++;
    }
    else
    {
        error_tms = 0;
    }
    
    if((ESC_ERROR_CODE[0]) && (ESC_ERROR_CODE[0]!=esc_error_code_old))
    {
        error_tms = 0;	
    }			
    
    if((error_tms == 5) && (esc_run_to_stop_tms > 200))   		
    {
        if((( CbBase_EscState & ESC_RUN_STATE )) /*&& (POWER_ON_TMS > 500)*/) 
        {
            error_record_en = 1;     	
        }
    }	          
	        
    
    if(error_record_en)	 			
    {
        error_record_incode();			
        error_record_en = 0;	
    }
    
    esc_error_code_old = ESC_ERROR_CODE[0];
}


/*******************************************************************************
* Function Name  : eep_read_data
* Description    : Get error record from eeprom.
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/ 
u8 eep_read_data(u16 Adr, u16 len, u8 ReadData[])
{      
    u16 i;
    u8 errorflag = 0u;
        
    if( Adr == ESC_ERROR_ADR )
    {
        if(!AT24CXX_Read(Adr, len, ReadData))
        {
            if(MB_CRC16(ReadData, len))
            {
                errorflag = 1u;
            }
        }
        else
        {
            errorflag = 1u;
        }
        
        if( errorflag == 1u )
        {
            if(!AT24CXX_Read(ESC_ERROR_BACKUP_ADR, len, ReadData))
            {
                if(!MB_CRC16(ReadData, len))
                {
                    errorflag = 0u;
                }
            }
        }
    }
    else if( Adr == ESC_PARA_ADR )
    {
        if(!AT24CXX_Read(Adr, len, ReadData))
        {
            if(MB_CRC32(ReadData, len, PARAMETER_POLYNOMIALS))
            {
                errorflag = 1u;
            }
        }   
        else
        {
            errorflag = 1u;
        }  
        
        if( errorflag == 1u )
        {
            if(!AT24CXX_Read(ESC_PARA_BACKUP_ADR, len, ReadData))
            {
                if(!MB_CRC32(ReadData, len, PARAMETER_POLYNOMIALS))
                {
                    errorflag = 0u;
                }
            }
        }        
    }
    else
    {}
    
    if( errorflag != 0u )
    {
        for(i = 0u; i < len; i++)
        {
            ReadData[i] = 0u;
        } 

        if(Adr == ESC_ERROR_ADR)
        {
            eep_write_data(Adr, len, ReadData, 1u);
            eep_write_data(ESC_ERROR_BACKUP_ADR, len, ReadData, 1u);
        }
        else if(Adr == ESC_PARA_ADR)
        {
            eep_write_data(Adr, len, ReadData, PARAMETER_POLYNOMIALS);
            eep_write_data(ESC_PARA_BACKUP_ADR, len, ReadData, PARAMETER_POLYNOMIALS);
        }
        else
        {} 
    }
    
    return errorflag;    

}

/*******************************************************************************
* Function Name  : eep_write_data
* Description    : 
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/ 
u8 eep_write_data(u16 Adr, u16 len, u8 WriteData[], u32 Polynomials)
{
    u32 i;
    u8 result = 0u;
    
    if( Polynomials == 1u )
    {
        /* CRC16 */
        i = MB_CRC16( WriteData, len - 2u );
        WriteData[len - 2u] = (u8)i;
        WriteData[len - 1u] = (u8)(i >> 8u);
    }
    else if( Polynomials > 1u )
    {
        /* CRC32 */
        i = MB_CRC32( WriteData, (len - 4u) , Polynomials);
        WriteData[len - 4u] = (u8)(i >> 24u);
        WriteData[len - 3u] = (u8)(i >> 16u);     
        WriteData[len - 2u] = (u8)(i >> 8u);
        WriteData[len - 1u] = (u8)i;         
    }
    else
    {}

    
    if( AT24CXX_Write(Adr, len, WriteData))
    {
        /* error */
        EN_ERROR34 |= 0x04;   
        result = 1u;
    }
    
    return result;
}

/*******************************************************************************
* Function Name  : get_error_fault_from_eeprom
* Description    : Get error record from eeprom.
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/ 
//#define TESTNUM  200

void get_error_record_from_eeprom(void)
{
    /* error records */
//    u8 writebuff[TESTNUM] = {0};
//    u8 readbuff[TESTNUM] = {0};
//    
//    for(u16 a = 0; a < TESTNUM; a++)
//    {
//        writebuff[a] = 0x0a;
//    }

    
    
//    eep_read_data(EEP_ERROR_RECORD_ADR, EEP_ERROR_RECORD_NUM, (u8*)&ErrorRecordFirstItem[0]);
//    AT24CXX_Read(EEP_ERROR_RECORD_ADR, EEP_ERROR_RECORD_NUM, (u8*)&ErrorRecordFirstItem);
    
    errorRecordFirstItem_old = ErrorRecordFirstItem;

    
//    eep_write_data(EEP_ERROR_RECORD_ADR, TESTNUM, &writebuff[0]);
//    eep_read_data(EEP_ERROR_RECORD_ADR, TESTNUM, readbuff);
    
        
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

    if((errorRecordFirstItem_old != ErrorRecordFirstItem) && (ErrorRecordFirstItem))
    {
//        i = (ErrorRecordFirstItem - 1)*8;      
//        AT24CXX_Write(EEP_ERROR_RECORD_ADR, 50, (u8*)&ErrorRecordFirstItem);
//        AT24CXX_Write(EEP_ERROR_RECORD_ADR + 50 + i, 8, &EscErrorBuff[i]);
//        eep_write_data(EEP_ERROR_RECORD_ADR, EEP_ERROR_RECORD_NUM, (u8*)&ErrorRecordFirstItem);
      
    }
    
    errorRecordFirstItem_old = ErrorRecordFirstItem;	
			  
}


/*******************************************************************************
* Function Name  : eeprom_thread
* Description    : EEPROM task.
* Input          : None  
* Output         : None
* Return         : None
*******************************************************************************/
void eeprom_thread(void *arg)
{

    while(1)
    {               

        StoreParaInMemory();
        ReadParaFromMemory();
        StoreFaultInMemory();
        
        xEventGroupSetBits(xCreatedEventGroup, TASK_BIT_2);
        vTaskDelay( 10 );
    }
}
	


/*******************************************************************************
* Function Name  : eeprom_task_init
* Description    : None               
* Input          : None               
* Output         : None
* Return         : None
*******************************************************************************/ 
void eeprom_task_init(void)
{	
    xTaskCreate(eeprom_thread, "EEPROM", configMINIMAL_STACK_SIZE * 4, NULL, EEPROM_THREAD_PRIO, NULL);      
}

/******************************  END OF FILE  *********************************/



