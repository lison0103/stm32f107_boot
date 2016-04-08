/*******************************************************************************
* File Name          : esc.h
* Author             : lison
* Version            : V1.0
* Date               : 04/08/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ESC_H
#define __ESC_H	

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define PARA_CONTROL_MODE     ParaBuff[3]
#define ESC_CONTROL_MODE      ParaBuff[3]
#define PARA_DRIVER_MODE      ParaBuff[5]

#define INPUT_PORT1   EscRTBuff[4]
#define INPUT_PORT2   EscRTBuff[5]
#define INPUT_PORT3   EscRTBuff[6]
#define INPUT_PORT4   EscRTBuff[7]
#define INPUT_PORT5   EscRTBuff[8]
#define INPUT_PORT6   EscRTBuff[9]
#define INPUT_PORT7   EscRTBuff[10]
#define INPUT_PORT8   EscRTBuff[11]

#define UPPER_INPUT_PORT1 	EscRTBuff[12]
#define UPPER_INPUT_PORT2  	EscRTBuff[13]
#define UPPER_INPUT_PORT3  	EscRTBuff[14]
#define UPPER_INPUT_PORT4  	EscRTBuff[15]

#define LOWER_INPUT_PORT1  	EscRTBuff[16]
#define LOWER_INPUT_PORT2  	EscRTBuff[17]
#define LOWER_INPUT_PORT3  	EscRTBuff[18]
#define LOWER_INPUT_PORT4  	EscRTBuff[19]

#define CMD_FLAG1 	EscRTBuff[20]
#define CMD_FLAG2 	EscRTBuff[21]
#define CMD_FLAG3 	EscRTBuff[22]
#define CMD_FLAG4 	EscRTBuff[23]
#define CMD_FLAG5 	EscRTBuff[24]
#define CMD_FLAG6 	EscRTBuff[25]
#define CMD_FLAG7 	EscRTBuff[26]
#define CMD_FLAG8 	EscRTBuff[27]
#define CMD_FLAG9 	EscRTBuff[28]
#define CMD_FLAG10 	EscRTBuff[29]

#define CMD_OMC_FLAG1 	pcOMC_EscRTBuff[20]
#define CMD_OMC_FLAG2 	pcOMC_EscRTBuff[21]
#define CMD_OMC_FLAG3 	pcOMC_EscRTBuff[22]
#define CMD_OMC_FLAG4 	pcOMC_EscRTBuff[23]
#define CMD_OMC_FLAG5 	pcOMC_EscRTBuff[24]
#define CMD_OMC_FLAG6 	pcOMC_EscRTBuff[25]
#define CMD_OMC_FLAG7 	pcOMC_EscRTBuff[26]
#define CMD_OMC_FLAG8 	pcOMC_EscRTBuff[27]
#define CMD_OMC_FLAG9 	pcOMC_EscRTBuff[28]
#define CMD_OMC_FLAG10 	pcOMC_EscRTBuff[29]

#define   RELAY_OUTPUT_CMD        EscRTBuff[30]
#define   RELAY_OUTPUT_CMD_AUX    EscRTBuff[30]
#define   RELAY_OUTPUT_Q5         EscRTBuff[32]
#define   RELAY_OUTPUT_MB         EscRTBuff[33]
#define   RELAY_OUTPUT_UPPER      EscRTBuff[34]
#define   RELAY_OUTPUT_LOWER      EscRTBuff[35]

#define 	STATE1        EscRTBuff[60]
#define 	STATE2        EscRTBuff[61]
#define 	STATE3        EscRTBuff[62]
#define 	STATE4        EscRTBuff[63]

#define 	ALARM1        EscRTBuff[64]
#define 	ALARM2        EscRTBuff[65]
#define 	ALARM3        EscRTBuff[66]
#define 	ALARM4        EscRTBuff[67]

#define 	POWER_ON_TMS  plPowerOnTms[0]

#define   EN_ERROR1   	pcErrorBuff[6] 
#define  	EN_ERROR2   	pcErrorBuff[7] 
#define 	EN_ERROR3   	pcErrorBuff[8] 
#define 	EN_ERROR4   	pcErrorBuff[9]

#define 	EN_ERROR5   	pcErrorBuff[16] 
#define 	EN_ERROR6   	pcErrorBuff[17] 
#define 	EN_ERROR7   	pcErrorBuff[18] 
#define 	EN_ERROR8   	pcErrorBuff[19] 
#define 	EN_ERROR9   	pcErrorBuff[20] 
#define 	EN_ERROR10   	pcErrorBuff[21] 
#define 	EN_ERROR11   	pcErrorBuff[22] 
#define 	EN_ERROR12   	pcErrorBuff[23] 
#define 	EN_ERROR13   	pcErrorBuff[24] 
#define 	EN_ERROR14   	pcErrorBuff[25] 
#define 	EN_ERROR15   	pcErrorBuff[26] 
#define 	EN_ERROR16   	pcErrorBuff[27] 
#define 	EN_ERROR17   	pcErrorBuff[28] 
#define 	EN_ERROR18   	pcErrorBuff[29] 
#define 	EN_ERROR19   	pcErrorBuff[30] 
#define 	EN_ERROR20   	pcErrorBuff[31]

#define 	EN_ERROR_SYS1 EscRTBuff[104]
#define 	EN_ERROR_SYS2 EscRTBuff[105]

//#define 	ESC_SPEED_RT_PST      EscRTBuff[106]
//#define 	ESC_SPEED_PST         EscRTBuff[107]

//MODBUS 
#define 	ESC_ERROR_CODE  pcEscErrorCodeBuff
#define 	ESC_ALARM_CODE  Modbuff[459]

#define 	ESC_STATE1    pcEscStateBuff[0]
#define 	ESC_STATE2    pcEscStateBuff[1]
#define 	ESC_STATE3    pcEscStateBuff[2]
#define 	ESC_STATE4    pcEscStateBuff[3]

#define 	ESC_ALARM1    pcEscAlarmBuff[0]
#define 	ESC_ALARM2    pcEscAlarmBuff[1]
#define 	ESC_ALARM3    pcEscAlarmBuff[2]
#define 	ESC_ALARM4    pcEscAlarmBuff[3]

#define 	ESC_ERROR     pcEscErrorBuff

//
#define 	CMD_OUTPUT_PORT			EscRTBuff[30]
#define 	PORT_SF     	0X01
#define 	PORT_AUX    	0X02
#define 	PORT_UDC    	0X04
#define 	PORT_UP     	0X08
#define 	PORT_DOWN   	0X10
#define 	PORT_DYC    	0X20
#define 	PORT_D      	0X40
#define 	PORT_Y      	0X80

#define 	CMD_OUTPUT_PORT2			EscRTBuff[31]
#define 	PORT_SF_COMM  				0X01

#define 	PGA_INPUT_MASK        pcPgaInputMask
#define 	PGA_INPUT_BUFF        pcPgaInputBuff

#define   CONTACT_MASK          EscRTBuff[148]		 
#define   CONTACT_FEEDBACK      EscRTBuff[149]	   

#define   PGAOUTDATA            pcPgaOutputBuff     //10字节

//#define   FAULT_KEEP_TS         ptEscRTBuff[55]

#define ESC_MODE_RELAY	 		    pcOutputBuff[2]
#define RELAY_BIT_DS	 			    (1<<0)
#define RELAY_BIT_VI	 			    (1<<1)
#define RELAY_BIT_VO	 			    (1<<2)


////////////////////////////////////////////////////////////////////////////////
#define CF2_1	                      ((u8)0x01)
#define CF2_2	                      ((u8)0x02)
#define CF2_3	                      ((u8)0x04)
#define CF2_4	                      ((u8)0x08)
#define CF2_5	                      ((u8)0x10)
#define CF2_6	                      ((u8)0x20)
#define CF2_Slow_Disable	          ((u8)0x40)
#define CF2_Intt_Disable	          ((u8)0x80)

#define CF4_SC_State_On	            ((u8)0x01)
#define CF4_EN_SC_Error	            ((u8)0x02)
#define CF4_SC_SRL_Enable	          ((u8)0x04)
#define CF4_EN_Error	              ((u8)0x08)
#define CF4_EN_SRL_Enable	          ((u8)0x10)
#define CF4_SRL_State_Off	          ((u8)0x20)
#define CF4_SRL_State_On	          ((u8)0x40)
#define CF4_EN_Record_Error	        ((u8)0x80)

#define CF7_1	        ((u8)0x01)
#define CF7_2	        ((u8)0x02)
#define CF7_3	        ((u8)0x04)
#define CF7_Aux_Output_Enable         ((u8)0x08)
#define CF7_Aux_CmdOff_Enable	        ((u8)0x10)
#define CF7_6	        ((u8)0x20)
#define CF7_Aux_Delay_State         ((u8)0x40)
#define CF7_Aux_State_Off	          ((u8)0x80)

#define CF8_ESC_Driver_On           ((u8)0x01)
#define CF8_ESC_Driver_Feedback	    ((u8)0x02)
#define CF8_3	                      ((u8)0x04)
#define CF8_4	                      ((u8)0x08)
#define CF8_DY_State_Star	          ((u8)0x10)
#define CF8_DY_State_Delta	        ((u8)0x20)
#define CF8_Auto_Fast_Mode1	        ((u8)0x40)
#define CF8_Auto_Fast_Mode2	        ((u8)0x80)

////////////////////////////////////////////////////////////////////////////////
typedef enum {RELAY_ON = 1, RELAY_OFF = !RELAY_ON} RelayCMD;


typedef struct sdfsdfs
{
  u8 ch;
  u8 level;
  u16 tcnt;
}TypeDef_PlsSeq;

////////////////////////////////////////////////////////////////////////////////

extern u16 System_Sequence,System_Check_Fault_Flag;

	
extern u8 Modbuff[5400];

extern u16 *const ptModbuff;

extern u8 McRxBuff[1000];

extern u8 EscRTBuff[200];     

extern u8 *const ParaBuff; //240字节
extern u16 *const ptParaBuff;

extern u8 *const pcMbRtccBuff; //6字节 //modbus 50

//modbus 
extern u8 *const pcEscErrorCodeBuff;  //5字节
extern u8 *const pcEscStateBuff;      //4字节 
extern u8 *const pcEscAlarmBuff;  	  //4字节
extern u8 *const pcEscErrorBuff;      //32字节

extern u8 *const pcRtRecordErrorCodeBuff;

//extern u8 Esc_RelayOutBuff[6];  
//extern u8 *pcParaBuff; 
//extern u16 *ptMcRxBuff;

extern u8 *const pcStatDataBuff; //modbus 500
extern u8 *const pcParaMultiSpeedBuff; //modbus 1600
extern u8 *const pcParaERB_DecodeBuff;
extern u8 *const pcParaErrorResetBuff; //modbus 1700

extern u8 *const pcRtDataBuff; //modbus 2700
extern u8 *const pcErrorRecordBuff; //modbus 3000

//实时数据区指针
extern u16 *const ptEscRTBuff;

extern u8 *const pcInputBuff;
extern u8 *const pcCMDBuff;
extern u8 *const pcOutputBuff;
//extern u8 *const pcStateBuff;
extern u8 *const pcAlarmBuff;
extern u8 *const pcErrorBuff;

//记录故障
extern u16 *const ptErrorRecordFirstItem;

extern u32 *const plPowerOnTms;

extern u8 *const pcPgaInputMask;
extern u8 *const pcPgaInputBuff;
extern u8 *const pcPgaOutputBuff;

extern u8 *const pcOMC_EscRTBuff;




#endif  /* __ESC_H */

/******************************  END OF FILE  *********************************/


