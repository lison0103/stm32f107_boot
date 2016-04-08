/*******************************************************************************
* File Name          : eeprim_process.h
* Author             : lison
* Version            : V1.0
* Date               : 04/08/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_PROCESS_H
#define __EEPROM_PROCESS_H	

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/


typedef struct
{
	u16 flag;
  
  u16 rtw_en;
  
	u16 adr1;
	u16 adr2;
	u16 len;

 	u8 *buff;	
 
  void (*InitSub)(void);
  void (*CheckSub)(void);
  
  u16 crc;
  
  u16 delay;
}EEP_TypeDef;

/*
typedef struct
{
  u16 Fault_Number;
  //u16 ;
}aaaa;
*/
void get_para_from_eeprom(void);
void write_para_to_eeprom(void);

//参数检查
void EEP_SysPara_Check(void);
void EEP_ComPara_Check(void);
void EEP_BasePara_Check(void);
void EEP_RtBuff_Check(void);
void EEP_ErrRstPara_Check(void);

//参数初始化
void EEP_SysPara_Init(void);
void EEP_ComPara_Init(void);
void EEP_BasePara_Init(void);
void EEP_RtBuff_Init(void);
void EEP_StatBuff_Init(void);
void EEP_MspPara_Init(void);

void EEP_ErrRstPara_Init(void);

extern EEP_TypeDef EEP_Struct_SysPara;
extern EEP_TypeDef EEP_Struct_ComPara;
extern EEP_TypeDef EEP_Struct_BasePara;
extern EEP_TypeDef EEP_Struct_MspPara;
extern EEP_TypeDef EEP_Struct_RtBuff;
extern EEP_TypeDef EEP_Struct_StatBuff;

extern EEP_TypeDef EEP_Struct_ErrRstPara;

#endif //__EEPROM_PROCESS_H 


/******************************  END OF FILE  *********************************/

