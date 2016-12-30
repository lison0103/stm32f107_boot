/*******************************************************************************
* File Name          : esc.h
* Author             : lison
* Version            : V1.0
* Date               : 09/30/2016
* Last modify date   : 10/26/2016
* Description        : This file contains esc parameters.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ESC_H
#define __ESC_H	

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"
#include "esc_parameter_process.h"
#include "initial_devices.h"

/* Exported types ------------------------------------------------------------*/
/* ESC STATE */
enum EscState
{
    ESC_INIT_STATE = 0u,
    ESC_RUN_STATE = 1u,    
    ESC_READY_STATE = 4u,  
    ESC_STOPPING_PROCESS_STATE = 5u,
    ESC_FAULT_STATE = 7u  
};

typedef struct  
{
    /* Header code and comm times */
    u8 HeaderCode[6];
    
    /* Safety Input */
    u8 SafetyInputDataCHA[8];
    u8 SafetyInputDataCHB[8];
    
    /* Control Input */
    u8 ControlInputData[2];
    
    /* DBL1 Input */
    u8 DBL1UperInputData[4];
    u8 DBL1LowerInputData[4];
    u8 DBL1Interm1InputData[4];
    u8 DBL1Interm2InputData[4];
    
    /* DBL2 Input */
    u8 DBL2UperInputDataCHA[4];
    u8 DBL2UperInputDataCHB[4];
    u8 DBL2LowerInputDataCHA[4];
    u8 DBL2LowerInputDataCHB[4];
    u8 DBL2Interm1InputDataCHA[4];
    u8 DBL2Interm1InputDataCHB[4];
    u8 DBL2Interm2InputDataCHA[4];
    u8 DBL2Interm2InputDataCHB[4];
    
    /* ANALOG/PT100 */
    u16 DBL2UpperAnalogData[3];
    u16 DBL2LowerAnalogData[3];
    u16 DBL2Interm1AnalogData[3];
    u16 DBL2Interm2AnalogData[3];    
    
    /* Output */
    u8 ControlOutputData[2];
    u8 DBLOutputData[4];
    
    /* Rtc, */
    u8 RtccData[6];

    /* Command data */
    u8 CommandData[8];
    
    /* Fault data */
    u8 ErrorBuff[64];

    /* Sensor Data */
    u16 SensorDataCHA[14];
    u16 SensorDataCHB[14];

    /* Configuration func */
    u8 CFG_FUNC_MASK[20];
    u8 CFG_FUNC_INPUT[20];

    /* Error code */
    u16 ErrorCode[5];
    u16 ErrorCodeSafetyCHA[5];
    u16 ErrorCodeSafetyCHB[5];
    
    /* warn code */
    u8 WarnCodeSafety[4];
    
    /* Esc parameter crc value */
    u8 ParaCRC[4];
    
    /* Configurable input buff */
    u8 Cfg_Input_Mask[25]; 
    u8 Cfg_Input_Level[25];     
    
    /* Configurable output buff */
    u8 Cfg_Output_Mask[25]; 
    u8 Cfg_Output_Level[25];      
    
    /* Esc Timer */
    u16 EscTimer[4];
	
}
ControlEscData;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/ 
/* led flash freq */
#define FREQ_0_5HZ      1000u
#define FREQ_1HZ        500u
#define FREQ_2HZ        250u
#define FREQ_3HZ        167u

/* failure lock */
#define EF       2u
/* standard fault */
#define ES       1u
/* recovery fault */
#define ER       3u
/* Undefined */
#define EU       0x0u

#define ESC_ERROR_CODE          ErrorCodeBuff
#define ErrorCodeControl        CBEscData.ErrorCode
#define ESC_RTCC                CBEscData.RtccData

#define INPUT_PORT1_8       CBEscData.ControlInputData[0]
#define INPUT_PORT9_16      CBEscData.ControlInputData[1]

#define INPUT_PORT1_MASK        (u8)( 1 << 0 )
#define INPUT_PORT2_MASK        (u8)( 1 << 1 )
#define INPUT_PORT3_MASK        (u8)( 1 << 2 )
#define INPUT_PORT4_MASK        (u8)( 1 << 3 )
#define INPUT_PORT5_MASK        (u8)( 1 << 4 )
#define INPUT_PORT6_MASK        (u8)( 1 << 5 )
#define INPUT_PORT7_MASK        (u8)( 1 << 6 )
#define INPUT_PORT8_MASK        (u8)( 1 << 7 )

#define INPUT_PORT9_MASK         (u8)( 1 << 0 )
#define INPUT_PORT10_MASK        (u8)( 1 << 1 )
#define INPUT_PORT11_MASK        (u8)( 1 << 2 )
#define INPUT_PORT12_MASK        (u8)( 1 << 3 )
#define INPUT_PORT13_MASK        (u8)( 1 << 4 )
#define INPUT_PORT14_MASK        (u8)( 1 << 5 )
#define INPUT_PORT15_MASK        (u8)( 1 << 6 )
#define INPUT_PORT16_MASK        (u8)( 1 << 7 )


/* Command data */

#define CbBase_EscState        CBEscData.CommandData[0]
#define CMD_ESC_RUN             CBEscData.CommandData[1]
#define CMD_SF_ORDER           CBEscData.CommandData[2]
#define CMD_SF_STATE_MODE      CBEscData.CommandData[3]
#define CMD_SF_STOP            CBEscData.CommandData[4]

#define CMD_OUTPUT_PORT         CBEscData.ControlOutputData[0]
#define CMD_OUTPUT_PORT2        CBEscData.ControlOutputData[1]


#define OUTPUT_GRL1        (u8)( 1 << 0 )
#define OUTPUT_GRL2        (u8)( 1 << 1 )
#define OUTPUT_GRL3        (u8)( 1 << 2 )
#define OUTPUT_GRL4        (u8)( 1 << 3 )
#define OUTPUT_GRL5        (u8)( 1 << 4 )
#define OUTPUT_GRL6        (u8)( 1 << 5 )
#define OUTPUT_GRL7        (u8)( 1 << 6 )
#define OUTPUT_GRL8        (u8)( 1 << 7 )

#define OUTPUT_GRL9               (u8)( 1 << 0 )
#define OUTPUT_UP                 (u8)( 1 << 1 )
#define OUTPUT_DOWN               (u8)( 1 << 2 )
#define OUTPUT_STAR               (u8)( 1 << 3 )
#define OUTPUT_DELTA              (u8)( 1 << 4 )
#define OUTPUT_TRANS_CTRL1        (u8)( 1 << 5 )
#define OUTPUT_TRANS_CTRL2        (u8)( 1 << 6 )

/* Fault data, 64*8 = 512 */
#define         EN_ERROR1   	CBEscData.ErrorBuff[0]
#define  	EN_ERROR2   	CBEscData.ErrorBuff[1]
#define 	EN_ERROR3   	CBEscData.ErrorBuff[2]
#define 	EN_ERROR4   	CBEscData.ErrorBuff[3]
#define 	EN_ERROR5   	CBEscData.ErrorBuff[4]
#define 	EN_ERROR6   	CBEscData.ErrorBuff[5]
#define 	EN_ERROR7   	CBEscData.ErrorBuff[6]
#define 	EN_ERROR8   	CBEscData.ErrorBuff[7]
#define 	EN_ERROR9   	CBEscData.ErrorBuff[8]
#define 	EN_ERROR10   	CBEscData.ErrorBuff[9]
#define         EN_ERROR11   	CBEscData.ErrorBuff[10]
#define  	EN_ERROR12   	CBEscData.ErrorBuff[11]
#define 	EN_ERROR13   	CBEscData.ErrorBuff[12]
#define 	EN_ERROR14   	CBEscData.ErrorBuff[13]
#define 	EN_ERROR15   	CBEscData.ErrorBuff[14]
#define 	EN_ERROR16   	CBEscData.ErrorBuff[15]
#define 	EN_ERROR17   	CBEscData.ErrorBuff[16]
#define 	EN_ERROR18   	CBEscData.ErrorBuff[17]
#define 	EN_ERROR19   	CBEscData.ErrorBuff[18]
#define 	EN_ERROR20   	CBEscData.ErrorBuff[19]
#define         EN_ERROR21   	CBEscData.ErrorBuff[20]
#define  	EN_ERROR22   	CBEscData.ErrorBuff[21]
#define 	EN_ERROR23   	CBEscData.ErrorBuff[22]
#define 	EN_ERROR24   	CBEscData.ErrorBuff[23]
#define 	EN_ERROR25   	CBEscData.ErrorBuff[24]
#define 	EN_ERROR26   	CBEscData.ErrorBuff[25]
#define 	EN_ERROR27   	CBEscData.ErrorBuff[26]
#define 	EN_ERROR28   	CBEscData.ErrorBuff[27]
#define 	EN_ERROR29   	CBEscData.ErrorBuff[28]
#define 	EN_ERROR30   	CBEscData.ErrorBuff[29]
#define         EN_ERROR31   	CBEscData.ErrorBuff[30]
#define  	EN_ERROR32   	CBEscData.ErrorBuff[31]
#define         EN_ERROR33   	CBEscData.ErrorBuff[32]
#define  	EN_ERROR34   	CBEscData.ErrorBuff[33]
#define 	EN_ERROR35   	CBEscData.ErrorBuff[34]
#define 	EN_ERROR36   	CBEscData.ErrorBuff[35]
#define 	EN_ERROR37   	CBEscData.ErrorBuff[36]
#define 	EN_ERROR38   	CBEscData.ErrorBuff[37]
#define 	EN_ERROR39   	CBEscData.ErrorBuff[38]
#define 	EN_ERROR40   	CBEscData.ErrorBuff[39]
#define 	EN_ERROR41   	CBEscData.ErrorBuff[40]
#define 	EN_ERROR42   	CBEscData.ErrorBuff[41]
#define         EN_ERROR43   	CBEscData.ErrorBuff[42]
#define  	EN_ERROR44   	CBEscData.ErrorBuff[43]
#define 	EN_ERROR45   	CBEscData.ErrorBuff[44]
#define 	EN_ERROR46   	CBEscData.ErrorBuff[45]
#define 	EN_ERROR47   	CBEscData.ErrorBuff[46]
#define 	EN_ERROR48   	CBEscData.ErrorBuff[47]
#define 	EN_ERROR49   	CBEscData.ErrorBuff[48]
#define 	EN_ERROR50   	CBEscData.ErrorBuff[49]
#define 	EN_ERROR51   	CBEscData.ErrorBuff[50]
#define 	EN_ERROR52   	CBEscData.ErrorBuff[51]
#define         EN_ERROR53   	CBEscData.ErrorBuff[52]
#define  	EN_ERROR54   	CBEscData.ErrorBuff[53]
#define 	EN_ERROR55   	CBEscData.ErrorBuff[54]
#define 	EN_ERROR56   	CBEscData.ErrorBuff[55]
#define 	EN_ERROR57   	CBEscData.ErrorBuff[56]
#define 	EN_ERROR58   	CBEscData.ErrorBuff[57]
#define 	EN_ERROR59   	CBEscData.ErrorBuff[58]
#define 	EN_ERROR60   	CBEscData.ErrorBuff[59]
#define 	EN_ERROR61   	CBEscData.ErrorBuff[60]
#define 	EN_ERROR62   	CBEscData.ErrorBuff[61]
#define         EN_ERROR63   	CBEscData.ErrorBuff[62]
#define  	EN_ERROR64   	CBEscData.ErrorBuff[63]


#define         ESC_TRANSPORTATION_TIMER                CBEscData.EscTimer[0]
#define         ESC_DIRECTION_INDICATION_TIMER          CBEscData.EscTimer[1]
#define         ESC_LOW_SPEED_TIMER                     CBEscData.EscTimer[2]
#define         ESC_REVERSING_TIMER                     CBEscData.EscTimer[3]

/* esc communicaiton, from safety interface data */
/* Message ID: 100h */
/* byte 1 */
/* order */
#define ORDER_FROM_SF_RUN                       (u8)( 1 << 0 )
#define ORDER_FROM_SF_STOP                      (u8)( 1 << 1 )
#define ORDER_FROM_SF_FAULT                     (u8)( 1 << 2 )
#define ORDER_FROM_SF_RESET                     (u8)( 1 << 3 )
#define ORDER_FROM_SF_ACTIVATE_UP_CONTACTOR     (u8)( 1 << 4 )
#define ORDER_FROM_SF_SLOW_FAST                 (u8)( 1 << 5 )
#define ORDER_FROM_SF_UP_DOWN                   (u8)( 1 << 6 )
#define ORDER_FROM_SF_CAPACITOR                 (u8)( 1 << 7 )

/* byte 2 */
/* state */
#define ESC_FROM_SF_STATE_MASK                  (u8)0x07
#define ESC_FROM_SF_STATE_INIT                  (u8)0x00
#define ESC_FROM_SF_STATE_READY                 (u8)0x04
#define ESC_FROM_SF_STATE_STARTING_PROCESS      (u8)0x02
#define ESC_FROM_SF_STATE_RUN                   (u8)0x01
#define ESC_FROM_SF_STATE_INTERM                (u8)0x06
#define ESC_FROM_SF_STATE_STOPPING_PROCESS      (u8)0x05
#define ESC_FROM_SF_STATE_FAULT                 (u8)0x07

/* mode1 */
#define ESC_FROM_SF_MODE1_MASK                  (u8)0x18
#define ESC_FROM_SF_MODE1_NORMAL                (u8)0x10
#define ESC_FROM_SF_MODE1_INSPECTION            (u8)0x08

/* mode2 */
#define ESC_FROM_SF_MODE2_MASK                   (u8)0xE0
#define ESC_FROM_SF_MODE2_CONTINOUS              (u8)0x00
#define ESC_FROM_SF_MODE2_INTERMITTEND           (u8)0x20
#define ESC_FROM_SF_MODE2_STBY                   (u8)0x40
#define ESC_FROM_SF_MODE2_INTERMITEND_STBY       (u8)0x80
#define ESC_FROM_SF_MODE2_2DIRECTION             (u8)0x60
#define ESC_FROM_SF_MODE2_Y_D_SWITCHBACK         (u8)0xA0


/* byte 3 */
#define ESC_FROM_SF_INTERM_STOP                 (u8)( 1 << 0 )
#define ESC_FROM_SF_INSP_BUTTONS_RELEASED       (u8)( 1 << 1 )
#define ESC_FROM_SF_REMOTE_STOP                 (u8)( 1 << 2 )
#define ESC_FROM_SF_SAFETY_CURTAIN_STOP         (u8)( 1 << 3 )
#define ESC_FROM_SF_STOPPING_FINISH             (u8)( 1 << 4 )
#define ESC_FROM_SF_SWITCH_ON_ELEM_UPPER        (u8)( 1 << 5 )
#define ESC_FROM_SF_SWITCH_ON_ELEM_LOWER        (u8)( 1 << 6 )
#define ESC_FROM_SF_HANDRAIL_ASME               (u8)( 1 << 7 )


/* esc communicaiton, control data to safety */
/* Message ID: 200h */
/* byte 1 */
/* order */
#define ORDER_REMOTE_KEY_SWITCH_MASK              (u8)0x03
#define ORDER_REMOTE_KEY_SWITCH_ON                (u8)0x02
#define ORDER_REMOTE_KEY_SWITCH_OFF               (u8)0x01
#define ORDER_REMOTE_RUN_MASK                     (u8)0x0C
#define ORDER_REMOTE_RUN_ON                       (u8)0x08
#define ORDER_REMOTE_RUN_OFF                      (u8)0x04
#define ORDER_STOP                                (u8)( 1 << 4 )
#define ORDER_FAULT                               (u8)( 1 << 5 )
#define ORDER_RESET                               (u8)( 1 << 6 )

/* byte 2 */
/* state */
#define ESC_STATE_MASK                          (u8)0x07
#define ESC_STATE_INIT                          (u8)0x00
#define ESC_STATE_READY                         (u8)0x04
#define ESC_SF_STATE_RUNNING                    (u8)0x01
#define ESC_SF_STATE_STOPPING_PROCESS           (u8)0x05
#define ESC_SF_STATE_FAULT                      (u8)0x07

/* mode */
#define ESC_MODE_LOCAL                  (u8)0x10
#define ESC_MODE_REMOTE                 (u8)0x08
#define ESC_MODE_SLOW                   (u8)0x20
#define ESC_MODE_INTERMITTEND           (u8)0x40
#define ESC_MODE_2DIRECTION             (u8)0x80


/* byte 3 */
#define ESC_DIRECTION_MASK               (u8)0x03
#define ESC_DIRECTION_UP                 (u8)0x02
#define ESC_DIRECTION_DOWN               (u8)0x01
#define ESC_MOTOR_MASK                  (u8)0x0c
#define ESC_MOTOR_DELTA                 (u8)0x08
#define ESC_MOTOR_STAR                  (u8)0x04
#define ESC_TEST_AUX_BRAKE              (u8)0x10
#define ESC_TYPE_OF_CONTROL_FAULT       (u8)0x60
#define ESC_FAULT_FAILURE_LOCK          (u8)0x60
#define ESC_FAULT_STANDARD_FAULT        (u8)0x20
#define ESC_FAULT_RECOVERY_STOP         (u8)0x40


/* byte 4 */
#define ESC_CONTACTORS_OUTPUTS_K1_1             (u8)( 1 << 0 )
#define ESC_CONTACTORS_OUTPUTS_K1_2             (u8)( 1 << 1 )
#define ESC_CONTACTORS_OUTPUTS_K2_1_1           (u8)( 1 << 2 )
#define ESC_CONTACTORS_OUTPUTS_K2_2_1           (u8)( 1 << 3 )
#define ESC_CONTACTORS_OUTPUTS_K10_1            (u8)( 1 << 4 )
#define ESC_CONTACTORS_OUTPUTS_K10_5            (u8)( 1 << 5 )
#define ESC_CONTACTORS_OUTPUTS_K2_1_2           (u8)( 1 << 6 )
#define ESC_CONTACTORS_OUTPUTS_K2_2_2           (u8)( 1 << 7 )


#define CB_TO_SAFETY_DATA_LEN    3u
#define DATA_FROM_SAFETY_LEN    21u

/* Exported functions ------------------------------------------------------- */
void fault_code_reset(void);
void fault_code_decode(u16 code_buff[]);
void esc_error_display(void);
void CheckReset(void);

extern u8 EscDataFromSafety[DATA_FROM_SAFETY_LEN][8];
extern u8 EscDataToSafety[CB_TO_SAFETY_DATA_LEN][8];

/* Parameter data */
extern u8 ParaDataToSafety[8];
extern u8 ParaDataFromSafety[5][8];

extern ControlEscData CBEscData;

extern u16 ErrorCodeBuff[5];
extern u8 EscErrorBuff[2400];
extern u16 ErrorRecordFirstItem;
extern u16 ErrorRecordCounter;
extern u8 pcCurrentRecordErrorBuff[8];

extern u32 g_u32InitTestError;
extern u16 g_u16ParameterLoadingError;
extern u8 g_u8ParameterLoadingOK;
extern u8 g_u8ParameterRead;
extern u8 g_u8ParameterLoadingFinish;
extern u8 g_u8StartingProcessCheck;

extern CBPara CBParameterData;
extern SFPara SFParameterData;
extern CBParaInSF CBParameterInSafety;
extern u8 DIAGNOSTIC;

extern u16 g_u16LedFreq;
extern u8 EscFaultType[512];






#endif  /* __ESC_H */


/******************************  END OF FILE  *********************************/







