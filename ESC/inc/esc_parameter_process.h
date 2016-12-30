/*******************************************************************************
* File Name          : esc_parameter_process.h
* Author             : lison
* Version            : V1.0
* Date               : 07/15/2016
* Description        : This file contains prototype of esc parameter process.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ESC_PARAMETER_PROCESS_H
#define __ESC_PARAMETER_PROCESS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
#pragma pack(1)
typedef struct sfparameter
{
    u8 p_MANUFACTURING_ORDER[20];
    u8 p_CODE;
    u8 p_NOMINAL_SPEED;
    u16 p_MOTOR_RPM;
    u8 p_MOTOR_PULSE_PER_REV;
    u16 p_MAIN_SHAFT_RPM;
    u16 p_MAIN_SHAFT_PULSE_PER_REV;
    u8 p_ROLLER_HR_RADIUS;
    u8 p_HR_PULSES_PER_REV;
    u8 p_HR_FAULT_TIME;
    u16 p_STEP_WIDTH; 
    u16 p_DELAY_NO_PULSE_CHECKING;
    u8 p_SPEED_SENSOR_INSTALLATION;
    u16 p_UNDERSPEED_TIME;
    u8 p_END_SAFETY_STRING_FAULT_TYPE;
    u16 p_CONTACTOR_FEEDBACK_FILTER;
    u8 p_CONTACTORS_TIMEOUT;
    u8 p_CONTACTOR_FB1_MASK;
    u8 p_CONTACTOR_FB2_MASK;
    u8 p_CONTACTOR_FB3_MASK;
    u8 p_CONTACTOR_FB4_MASK;
    u8 p_CONTACTOR_FB5_MASK;
    u8 p_CONTACTOR_FB6_MASK;    
    u8 p_CONTACTOR_FB7_MASK;
    u8 p_CONTACTOR_FB8_MASK;    
    u16 p_KEY_MINIMUM_TIME;
    u8 p_UP_DOWN_ALLOWED;
    u8 p_AUTCONT_OPTIONS;
    u8 p_DIAGNOSTIC_BOARD_L2_QUANTITY;
    u8 p_TANDEM;
    u8 p_INSPECTION_CATCH_THE_MOTOR;
    u8 p_RESET_FROM_INSPECTION_CONTROL;
    u8 p_AUX_BRAKE_SUPERVISION_TIME;
    u8 p_AUX_BRAKE_ENABLE;
    u8 p_CAPACITOR_TIME_MEASUREMENT;
    u8 p_RADAR_TYPE;
    u8 p_LIGHT_BARRIER_COMBS_AREA_TYPE;
    u8 p_LIGHT_BARRIER_ENTRY_TYPE;
    u16 p_TIME_TRANSPORTATION;
    u16 p_TIME_DIRECTION_INDICATION;
    u16 p_TIME_REVERSING;
    u8 p_SAFETY_CURTAIN_LONG_TIME;
    u16 p_PULSE_SIGNALS_MINIMUM_LAG;
    u16 p_DRIVE_CHAIN_DELAY;    
    u8 p_SPARE_PARAMETER_1;
    u8 p_SPARE_PARAMETER_2;
    u8 p_SPARE_PARAMETER_3;
    u8 p_SPARE_PARAMETER_4;
    u16 p_SPARE_PARAMETER_5;
    u16 p_SPARE_PARAMETER_6;
    u16 p_SPARE_PARAMETER_7;
    u16 p_SPARE_PARAMETER_8;
    u16 p_SPARE_PARAMETER_9;
    u16 p_SPARE_PARAMETER_10;
    u8 p_SAFETY_INPUT_CONFIGURABLE_1_TYPE;
    u8 p_SAFETY_INPUT_CONFIGURABLE_2_TYPE;
    u8 p_SAFETY_INPUT_CONFIGURABLE_3_TYPE;
    u8 p_SAFETY_INPUT_CONFIGURABLE_4_TYPE;
    u8 p_SAFETY_INPUT_CONFIGURABLE_5_TYPE;
    u8 p_SAFETY_INPUT_CONFIGURABLE_6_TYPE;
    u8 p_SAFETY_INPUT_CONFIGURABLE_7_TYPE;
    u8 p_SAFETY_INPUT_CONFIGURABLE_8_TYPE;
    u8 p_SAFETY_INPUT_CONFIGURABLE_9_TYPE;
    u8 p_SAFETY_INPUT_CONFIGURABLE_10_TYPE;
    
    /* 186 INPUT */
    u16 p_IO_PARAMETER_SAFETY_INPUT[46];
    u16 p_IO_PARAMETER_DBL2_UPPER_INPUT[35];
    u16 p_IO_PARAMETER_DBL2_LOWER_INPUT[35];
    u16 p_IO_PARAMETER_DBL2_INTERM1_INPUT[35];
    u16 p_IO_PARAMETER_DBL2_INTERM2_INPUT[35];
      
    u32 CRC32;
    
}SFPara;
#pragma pack()

#pragma pack(1)
typedef struct cb_para
{  
    u8 p_MOTOR_CONNECTION;
    u16 p_TIME_LOW_SPEED;
    u16 p_ADDITIONAL_DIRECTION_INDICATION_TIME;
    u16 p_ADDITIONAL_TRANSPORTATION_TIME;
    u16 p_ADDITIONAL_REVERSING_TIME;  
    u8 p_OIL_PUMP_CONTROL;
    u16 p_OIL_PUMP_TIMER_ON;
    u16 p_OIL_PUMP_TIMER_OFF;
    u8 p_STAR_DELTA_DELAY;
    u8 p_TIMETABLE_ENABLE;
    u8 p_TIME_CHANGE_TO_MODE1_HOUR;
    u8 p_TIME_CHANGE_TO_MODE1_MIN;
    u8 p_TIME_CHANGE_TO_MODE2_HOUR;
    u8 p_TIME_CHANGE_TO_MODE2_MIN;
    u8 p_LANGUAGE;
    u8 p_CHANGE_SUMERWINTER_TIME;
    u8 p_SPEED_MEASUREMENT_UNIT;
    u8 p_TIMETABLE_MODE1;
    u8 p_TIMETABLE_MODE2;
    u8 p_HEATER_LOW_LEVEL;
    u8 p_HEATER_HIGH_LEVEL;
    u8 p_IP_ADDRESS_BYTE1;
    u8 p_IP_ADDRESS_BYTE2;
    u8 p_IP_ADDRESS_BYTE3;
    u8 p_IP_ADDRESS_BYTE4;
    u8 p_SUBNET_MASK_BYTE1;
    u8 p_SUBNET_MASK_BYTE2;
    u8 p_SUBNET_MASK_BYTE3;
    u8 p_SUBNET_MASK_BYTE4;
    u8 p_GATEWAY_BYTE1;
    u8 p_GATEWAY_BYTE2;
    u8 p_GATEWAY_BYTE3;
    u8 p_GATEWAY_BYTE4;
    u8 p_NODE_NUMBER;
    u8 p_RGB;
    u8 p_RGB_movinlight;
    u8 p_DIAGNOSTIC_BOARD_L1_QUANTITY;
    u8 p_UPPER_DIAG_SS_LENGTH;
    u8 p_LOWER_DIAG_SS_LENGTH;
    u8 p_INTERM1_DIAG_SS_LENGTH;
    u8 p_INTERM2_DIAG_SS_LENGTH;
    u8 p_DIAG3_ENABLE;
    u8 p_LABEL_FAULT1[40];
    u8 p_LABEL_FAULT2[40];
    u8 p_LABEL_FAULT3[40];
    u8 p_LABEL_FAULT4[40];
    u8 p_LABEL_FAULT5[40];
    u8 p_LABEL_FAULT6[40];
    u8 p_LABEL_FAULT7[40];
    u8 p_LABEL_FAULT8[40];
    u8 p_LABEL_FAULT9[40];
    u8 p_LABEL_FAULT10[40];
    u8 p_SPARE_PARAMETER_11;
    u8 p_SPARE_PARAMETER_12;
    u8 p_SPARE_PARAMETER_13;
    u8 p_SPARE_PARAMETER_14;
    u8 p_SPARE_PARAMETER_15;
    u16 p_SPARE_PARAMETER_16;
    u16 p_SPARE_PARAMETER_17;
    u16 p_SPARE_PARAMETER_18;
    u16 p_SPARE_PARAMETER_19;
    u16 p_SPARE_PARAMETER_20;
    
    u8 p_ModBus[120];    
    u8 p_FT12DISPLAY[401];    
    
    u16 p_IO_PARAMETER_CONTROL_OUTPUT[15];
    u16 p_IO_PARAMETER_CONTROL_INPUT[16];
    u16 p_IO_PARAMETER_DBL1_UPPER_OUTPUT[8];
    u16 p_IO_PARAMETER_DBL1_UPPER_INPUT[32];
    u16 p_IO_PARAMETER_DBL1_LOWER_OUTPUT[8];
    u16 p_IO_PARAMETER_DBL1_LOWER_INPUT[32];
    u16 p_IO_PARAMETER_DBL1_INTERM1_OUTPUT[8];
    u16 p_IO_PARAMETER_DBL1_INTERM1_INPUT[32];
    u16 p_IO_PARAMETER_DBL1_INTERM2_OUTPUT[8];
    u16 p_IO_PARAMETER_DBL1_INTERM2_INPUT[32];
    u16 p_IO_PARAMETER_DBL2_UPPER_OUTPUT[8];
    u16 p_IO_PARAMETER_DBL2_LOWER_OUTPUT[8];
    u16 p_IO_PARAMETER_DBL2_INTERM1_OUTPUT[8];
    u16 p_IO_PARAMETER_DBL2_INTERM2_OUTPUT[8];
    
    u32 CRC32;
    
}CBPara;
#pragma pack()

#pragma pack(1)
typedef struct cb_para_in_sf
{  
    u16 p_TIME_LOW_SPEED;
    u16 p_ADDITIONAL_DIRECTION_INDICATION_TIME;
    u16 p_ADDITIONAL_TRANSPORTATION_TIME;
    u16 p_ADDITIONAL_REVERSING_TIME;        
    u8 p_DIAGNOSTIC_BOARD_L1_QUANTITY;
    u8 p_UPPER_DIAG_SS_LENGTH;
    u8 p_LOWER_DIAG_SS_LENGTH;
    u8 p_INTERM1_DIAG_SS_LENGTH;
    u8 p_INTERM2_DIAG_SS_LENGTH;  
    u8 p_SPARE_PARAMETER_11;
    u8 p_SPARE_PARAMETER_12;
    u16 p_SPARE_PARAMETER_16;
    u16 p_SPARE_PARAMETER_17;
    
    u16 p_IO_PARAMETER_CONTROL_INPUT[16];
    u16 p_IO_PARAMETER_DBL1_UPPER_INPUT[32];
    u16 p_IO_PARAMETER_DBL1_LOWER_INPUT[32];
    u16 p_IO_PARAMETER_DBL1_INTERM1_INPUT[32];
    u16 p_IO_PARAMETER_DBL1_INTERM2_INPUT[32];

    u32 CRC32;
    
}CBParaInSF;
#pragma pack() 

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define MESSAGE_TO_CPU      0x11
#define MESSAGE_TO_CONTROL      0x22
#define USB_DETECTED        0x01
#define USB_NOT_DETECTED        0x02
#define SAFETY_PARAMETER_LOADED        0x10
#define CONTROL_PARAMETER_LOADED        0x20
#define PARAMETER_ERROR        0x03
#define PARAMETER_CORRECT        0x04
#define SAFETY_PARAMETER_NOT_EXIST        0x05
#define CONTROL_PARAMETER_NOT_EXIST        0x06
#define SAFETY_PARAMETER_EXIST        0x07
#define CONTROL_PARAMETER_EXIST        0x08
#define SEND_PARAMETER        0x09
#define RECEIVE_PARAMETER        0x0A
#define PARAMETER_LOADED_FINSH        0x0B


/* parameters safety */
#define KEY_MINIMUM_TIME                        SFParameterData.p_KEY_MINIMUM_TIME
#define DIAGNOSTIC_BOARD_L2_QUANTITY            SFParameterData.p_DIAGNOSTIC_BOARD_L2_QUANTITY
/* safety IO parameter */
#define IO_PARAMETER_SAFETY_INPUT               SFParameterData.p_IO_PARAMETER_SAFETY_INPUT
#define IO_PARAMETER_DBL2_UPPER_INPUT           SFParameterData.p_IO_PARAMETER_DBL2_UPPER_INPUT
#define IO_PARAMETER_DBL2_LOWER_INPUT           SFParameterData.p_IO_PARAMETER_DBL2_LOWER_INPUT
#define IO_PARAMETER_DBL2_INTERM1_INPUT         SFParameterData.p_IO_PARAMETER_DBL2_INTERM1_INPUT
#define IO_PARAMETER_DBL2_INTERM2_INPUT         SFParameterData.p_IO_PARAMETER_DBL2_INTERM2_INPUT


/* parameters control */
#define MOTOR_CONNECTION                                CBParameterData.p_MOTOR_CONNECTION
#define TIME_LOW_SPEED                                  CBParameterData.p_TIME_LOW_SPEED

#define ADDITIONAL_DIRECTION_INDICATION_TIME            CBParameterData.p_ADDITIONAL_DIRECTION_INDICATION_TIME
#define ADDITIONAL_TRANSPORTATION_TIME                  CBParameterData.p_ADDITIONAL_TRANSPORTATION_TIME
#define ADDITIONAL_REVERSING_TIME                       CBParameterData.p_ADDITIONAL_REVERSING_TIME

#define OIL_PUMP_CONTROL                                CBParameterData.p_OIL_PUMP_CONTROL
#define OIL_PUMP_TIMER_ON                               CBParameterData.p_OIL_PUMP_TIMER_ON
#define OIL_PUMP_TIMER_OFF                              CBParameterData.p_OIL_PUMP_TIMER_OFF

#define STAR_DELTA_DELAY                                CBParameterData.p_STAR_DELTA_DELAY

#define TIMETABLE_ENABLE                                CBParameterData.p_TIMETABLE_ENABLE
#define TIME_CHANGE_TO_MODE1_HOUR                       CBParameterData.p_TIME_CHANGE_TO_MODE1_HOUR
#define TIME_CHANGE_TO_MODE1_MIN                        CBParameterData.p_TIME_CHANGE_TO_MODE1_MIN
#define TIME_CHANGE_TO_MODE2_HOUR                       CBParameterData.p_TIME_CHANGE_TO_MODE2_HOUR
#define TIME_CHANGE_TO_MODE2_MIN                        CBParameterData.p_TIME_CHANGE_TO_MODE2_MIN

#define LANGUAGE                                        CBParameterData.p_LANGUAGE
#define CHANGE_SUMERWINTER_TIME                         CBParameterData.p_CHANGE_SUMERWINTER_TIME
#define SPEED_MEASUREMENT_UNIT                          CBParameterData.p_SPEED_MEASUREMENT_UNIT

#define TIMETABLE_MODE1                                 CBParameterData.p_TIMETABLE_MODE1
#define TIMETABLE_MODE2                                 CBParameterData.p_TIMETABLE_MODE2

#define HEATER_LOW_LEVEL                                CBParameterData.p_HEATER_LOW_LEVEL
#define HEATER_HIGH_LEVEL                               CBParameterData.p_HEATER_HIGH_LEVEL

#define IP_ADDRESS_BYTE1                                   CBParameterData.p_IP_ADDRESS_BYTE1
#define IP_ADDRESS_BYTE2                                   CBParameterData.p_IP_ADDRESS_BYTE2
#define IP_ADDRESS_BYTE3                                   CBParameterData.p_IP_ADDRESS_BYTE3
#define IP_ADDRESS_BYTE4                                   CBParameterData.p_IP_ADDRESS_BYTE4
#define SUBNET_MASK_BYTE1                                  CBParameterData.p_SUBNET_MASK_BYTE1
#define SUBNET_MASK_BYTE2                                  CBParameterData.p_SUBNET_MASK_BYTE2
#define SUBNET_MASK_BYTE3                                  CBParameterData.p_SUBNET_MASK_BYTE3
#define SUBNET_MASK_BYTE4                                  CBParameterData.p_SUBNET_MASK_BYTE4
#define GATEWAY_BYTE1                                      CBParameterData.p_GATEWAY_BYTE1
#define GATEWAY_BYTE2                                      CBParameterData.p_GATEWAY_BYTE2
#define GATEWAY_BYTE3                                      CBParameterData.p_GATEWAY_BYTE3
#define GATEWAY_BYTE4                                      CBParameterData.p_GATEWAY_BYTE4

#define NODE_NUMBER                                      CBParameterData.p_NODE_NUMBER
#define RGB                                              CBParameterData.p_RGB
#define RGB_movinlight                                   CBParameterData.p_RGB_movinlight

#define DIAGNOSTIC_BOARD_L1_QUANTITY                    CBParameterData.p_DIAGNOSTIC_BOARD_L1_QUANTITY
#define UPPER_DIAG_SS_LENGTH                            CBParameterData.p_UPPER_DIAG_SS_LENGTH
#define LOWER_DIAG_SS_LENGTH                            CBParameterData.p_LOWER_DIAG_SS_LENGTH
#define INTERM1_DIAG_SS_LENGTH                          CBParameterData.p_INTERM1_DIAG_SS_LENGTH
#define INTERM2_DIAG_SS_LENGTH                          CBParameterData.p_INTERM2_DIAG_SS_LENGTH

#define DIAG3_ENABLE                                    CBParameterData.p_DIAG3_ENABLE

#define LABEL_FAULT1                                    CBParameterData.p_LABEL_FAULT1
#define LABEL_FAULT2                                    CBParameterData.p_LABEL_FAULT2
#define LABEL_FAULT3                                    CBParameterData.p_LABEL_FAULT3
#define LABEL_FAULT4                                    CBParameterData.p_LABEL_FAULT4
#define LABEL_FAULT5                                    CBParameterData.p_LABEL_FAULT5
#define LABEL_FAULT6                                    CBParameterData.p_LABEL_FAULT6
#define LABEL_FAULT7                                    CBParameterData.p_LABEL_FAULT7
#define LABEL_FAULT8                                    CBParameterData.p_LABEL_FAULT8
#define LABEL_FAULT9                                    CBParameterData.p_LABEL_FAULT9
#define LABEL_FAULT10                                   CBParameterData.p_LABEL_FAULT10

#define SPARE_PARAMETER_11                              CBParameterData.p_SPARE_PARAMETER_11
#define SPARE_PARAMETER_12                              CBParameterData.p_SPARE_PARAMETER_12
#define SPARE_PARAMETER_13                              CBParameterData.p_SPARE_PARAMETER_13
#define SPARE_PARAMETER_14                              CBParameterData.p_SPARE_PARAMETER_14
#define SPARE_PARAMETER_15                              CBParameterData.p_SPARE_PARAMETER_15
#define SPARE_PARAMETER_16                              CBParameterData.p_SPARE_PARAMETER_16
#define SPARE_PARAMETER_17                              CBParameterData.p_SPARE_PARAMETER_17
#define SPARE_PARAMETER_18                              CBParameterData.p_SPARE_PARAMETER_18
#define SPARE_PARAMETER_19                              CBParameterData.p_SPARE_PARAMETER_19
#define SPARE_PARAMETER_20                              CBParameterData.p_SPARE_PARAMETER_20

#define ModBus                                          CBParameterData.p_ModBus
#define FT12DISPLAY                                     CBParameterData.p_FT12DISPLAY

/* non safety IO parameter */
#define IO_PARAMETER_CONTROL_OUTPUT              CBParameterData.p_IO_PARAMETER_CONTROL_OUTPUT
#define IO_PARAMETER_CONTROL_INPUT               CBParameterData.p_IO_PARAMETER_CONTROL_INPUT

#define IO_PARAMETER_DBL1_UPPER_OUTPUT           CBParameterData.p_IO_PARAMETER_DBL1_UPPER_OUTPUT
#define IO_PARAMETER_DBL1_UPPER_INPUT            CBParameterData.p_IO_PARAMETER_DBL1_UPPER_INPUT

#define IO_PARAMETER_DBL1_LOWER_OUTPUT           CBParameterData.p_IO_PARAMETER_DBL1_LOWER_OUTPUT
#define IO_PARAMETER_DBL1_LOWER_INPUT            CBParameterData.p_IO_PARAMETER_DBL1_LOWER_INPUT

#define IO_PARAMETER_DBL1_INTERM1_OUTPUT         CBParameterData.p_IO_PARAMETER_DBL1_INTERM1_OUTPUT
#define IO_PARAMETER_DBL1_INTERM1_INPUT          CBParameterData.p_IO_PARAMETER_DBL1_INTERM1_INPUT

#define IO_PARAMETER_DBL1_INTERM2_OUTPUT         CBParameterData.p_IO_PARAMETER_DBL1_INTERM2_OUTPUT
#define IO_PARAMETER_DBL1_INTERM2_INPUT          CBParameterData.p_IO_PARAMETER_DBL1_INTERM2_INPUT

#define IO_PARAMETER_DBL2_UPPER_OUTPUT           CBParameterData.p_IO_PARAMETER_DBL2_UPPER_OUTPUT
#define IO_PARAMETER_DBL2_LOWER_OUTPUT           CBParameterData.p_IO_PARAMETER_DBL2_LOWER_OUTPUT
#define IO_PARAMETER_DBL2_INTERM1_OUTPUT         CBParameterData.p_IO_PARAMETER_DBL2_INTERM1_OUTPUT
#define IO_PARAMETER_DBL2_INTERM2_OUTPUT         CBParameterData.p_IO_PARAMETER_DBL2_INTERM2_OUTPUT


#define NO_DIAGNOSTIC_BOARD     0u
#define DIAGNOSTIC_BOARD_1      1u
#define DIAGNOSTIC_BOARD_2      2u


/* parameter size */
#define ESC_SF_PARAMETER_DATA_LEN         sizeof(SFPara)
#define ESC_CB_PARAMETER_DATA_LEN         sizeof(CBPara)
#define ESC_CB_PARAMETER_IN_SF_DATA_LEN         sizeof(CBParaInSF)

/* Exported functions ------------------------------------------------------- */
void ParametersLoading(void);
void EscParameterInit(void);
 





#endif  /* __ESC_PARAMETER_PROCESS_H */


/******************************  END OF FILE  *********************************/



