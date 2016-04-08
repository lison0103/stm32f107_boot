/*******************************************************************************
* File Name          : esc_eeprom_process.c
* Author             : lison
* Version            : V1.0
* Date               : 04/08/2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "esc.h"
#include "crc16.h"
#include "eeprom_process.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define EEP_ERROR_RECORD_ADR  5000
#define EEP_START_ADDR1	10
#define EEP_START_ADDR2	2010

void null_sub(void);

EEP_TypeDef EEP_Struct_StatBuff=   {0,0, 		 EEP_START_ADDR1,       EEP_START_ADDR2,  100,  &Modbuff[500],   EEP_StatBuff_Init,             null_sub,  0, 0};  // 
EEP_TypeDef EEP_Struct_BasePara=   {0,0, EEP_START_ADDR1+150,   EEP_START_ADDR2+150,  100, &Modbuff[1000],   EEP_BasePara_Init,   EEP_BasePara_Check,  0, 0};  // 
EEP_TypeDef EEP_Struct_SysPara=    {0,0, EEP_START_ADDR1+300,   EEP_START_ADDR2+300,  240, &Modbuff[1100],    EEP_SysPara_Init,    EEP_SysPara_Check,  0, 0};  // 
EEP_TypeDef EEP_Struct_ComPara=    {0,0, EEP_START_ADDR1+600,   EEP_START_ADDR2+600,  242, &Modbuff[1350],    EEP_ComPara_Init,    EEP_ComPara_Check,  0, 0};  // 
EEP_TypeDef EEP_Struct_MspPara=    {0,0, EEP_START_ADDR1+900,   EEP_START_ADDR2+900,   50, &Modbuff[1600],    EEP_MspPara_Init,             null_sub,  0, 0};  // 
EEP_TypeDef EEP_Struct_ErrRstPara= {0,0,EEP_START_ADDR1+1000,  EEP_START_ADDR2+1000,  252, &Modbuff[1700], EEP_ErrRstPara_Init, EEP_ErrRstPara_Check,  0, 0};  // 
EEP_TypeDef EEP_Struct_RtBuff=     {0,1,EEP_START_ADDR1+1500,  EEP_START_ADDR2+1500,  100, &Modbuff[2700],     EEP_RtBuff_Init,     EEP_RtBuff_Check,  0, 0};  // 

static u8 EepTempBuff[300];


u8 FramWrite_Flag=0;

u16 errorRecordFirstItem_old = 0;

extern u16 PC_DataWriteWaitms;


/***************************************************************************************************
***************************************************************************************************/
void null_sub(void) 
{
  
}

/*******************************************************************************
* Function Name  : eep_read_data
* Description    : 
*                  
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
void eep_read_data(EEP_TypeDef *eepItem)
{
	u8 en_error=0,fram_err1=0,fram_err2=0;
  u16 uint1=0,para_crc1=0,para_crc2=0;

  EWDT_TOOGLE();
  
	eepItem->flag = 0;   
	fram_err1 = AT24CXX_Read(eepItem->adr1, eepItem->len, eepItem->buff);
  if( fram_err1 )
  {
    delay_ms( 500 ); 	
  }
  else
  {
    delay_ms( 20 ); 	
  }
  fram_err2 = AT24CXX_Read(eepItem->adr2, eepItem->len, EepTempBuff);

  //2次读Fram错误，故障锁定 
	if(fram_err1 && fram_err2)        
	{
    en_error = 1;
    EN_ERROR_SYS2 |= 0x01;
	}

  para_crc1 = MB_CRC16( eepItem->buff, eepItem->len );
  para_crc2 = MB_CRC16( EepTempBuff, eepItem->len );
	
  //数据校验CRC异常
	if(para_crc1 && para_crc2)
	{
		eepItem->InitSub();  // if(!en_error) 
    
    en_error |= 2;
		eepItem->flag = 0xf1;
	}	
  else
  {
    if((!para_crc1) && (para_crc2))
    {
      en_error |= 0x02; 
      eepItem->flag = 0xf1; 
    } 
    else if((para_crc1) && (!para_crc2))
    {
      for(uint1=0; uint1<eepItem->len; uint1++)
      {
        eepItem->buff[uint1] = EepTempBuff[uint1]; 
      }   
      
      en_error |= 0x02;                      
      eepItem->flag = 0xf1; 
    }
  } 
  
  eepItem->CheckSub(); 
  
	uint1 = MB_CRC16(eepItem->buff,eepItem->len-2);
	
  eepItem->crc = uint1;  
  
	eepItem->buff[eepItem->len-1] = uint1>>8;
	eepItem->buff[eepItem->len-2] = uint1;                    
  
  EN_ERROR5 |= en_error; 
}

//------------------------------------------------------------------------------
void get_para_from_eeprom(void) //开机读取参数
{  
	eep_read_data(&EEP_Struct_SysPara);	
  
	eep_read_data(&EEP_Struct_ComPara); 
	
  eep_read_data(&EEP_Struct_BasePara);
  
  eep_read_data(&EEP_Struct_StatBuff);
  
  eep_read_data(&EEP_Struct_MspPara);
  
  eep_read_data(&EEP_Struct_ErrRstPara);
  
  //必须放置最后
  eep_read_data(&EEP_Struct_RtBuff);
  
  //故障记录
  errorRecordFirstItem_old = *ptErrorRecordFirstItem;
  
  if(AT24CXX_Read(EEP_ERROR_RECORD_ADR,2400,pcErrorRecordBuff)) 
  {
    EN_ERROR5 |= 0x01;
  }     
}

/***************************************************************************************************
***************************************************************************************************/
void eep_write_data(EEP_TypeDef *eepItem)
{
	u16 i;
	
  if(((CMD_FLAG1&0x0c) || (CMD_FLAG3&0x0c)) && (eepItem->rtw_en==0))
  {
    eepItem->flag = 0;
    eepItem->delay = 0;
    return;
  }    
  
  if((eepItem->flag != 0xf1) && (eepItem->flag != 0xf2))
  {
    eepItem->flag = 0;
    eepItem->delay = 0;
    return;
  }  
  
  eepItem->delay++;  
  if((eepItem->flag == 0xf1) && (eepItem->delay>200) && (FramWrite_Flag==0))
  {
		i = MB_CRC16(eepItem->buff, eepItem->len-2);
																																																			                                                              
		eepItem->buff[eepItem->len-1] = i>>8; 
		eepItem->buff[eepItem->len-2] = i; 
    eepItem->crc = i;
         
		AT24CXX_Write(eepItem->adr1, eepItem->len, eepItem->buff);

		eepItem->flag = 0xf2; 
    eepItem->delay = 0; 
    FramWrite_Flag = 1;
  }	
  else if((eepItem->flag == 0xf2) && (eepItem->delay>50) && (FramWrite_Flag==0)) 
  {
		i = MB_CRC16(eepItem->buff, eepItem->len-2);
																																																			                                                              
		eepItem->buff[eepItem->len-1] = i>>8; 
		eepItem->buff[eepItem->len-2] = i; 
    eepItem->crc = i;
      
		AT24CXX_Write(eepItem->adr2, eepItem->len, eepItem->buff);

		eepItem->flag = 0;
    eepItem->delay = 0;
    FramWrite_Flag = 1;
  }	
}

//------------------------------------------------------------------------------
void write_para_to_eeprom(void) //写实时记录
{
  u16 i;
  
  if(PC_DataWriteWaitms && (PC_DataWriteWaitms++>60000))
  {
    EEP_Struct_RtBuff.flag = 0xf1; 
    PC_DataWriteWaitms = 0; 
  }  
  
  eep_write_data(&EEP_Struct_SysPara);	
  eep_write_data(&EEP_Struct_ComPara);	
  eep_write_data(&EEP_Struct_BasePara);	
  eep_write_data(&EEP_Struct_RtBuff);	
  eep_write_data(&EEP_Struct_StatBuff);	
  eep_write_data(&EEP_Struct_MspPara);	
  
  eep_write_data(&EEP_Struct_ErrRstPara);

  //故障记录
  if(FramWrite_Flag==0)
  {  
      if((errorRecordFirstItem_old != *ptErrorRecordFirstItem) && (*ptErrorRecordFirstItem))
      {
          i = ((*ptErrorRecordFirstItem)-1)*8;                          
          AT24CXX_Write(EEP_ERROR_RECORD_ADR+i, 8, &pcErrorRecordBuff[i]);
      }
                  
      errorRecordFirstItem_old = *ptErrorRecordFirstItem;	
  }
  
  FramWrite_Flag = 0;
}

/******************************  END OF FILE  *********************************/

