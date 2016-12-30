/*******************************************************************************
* File Name          : esc_comm.c
* Author             : lison
* Version            : V1.0
* Date               : 09/30/2016
* Last modify date   : 10/26/2016
* Description        : Contains the control board esc communication.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "esc.h"
#include "esc_comm.h"
#include "includes.h"
#include "crc16.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CAN_DATA_QUEUE_LEN      40u
#define CAN_COMM_HAND_TIME      5000u

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Can_Receive_Data(void);
static void Can_Send_Data_Process(void);
static void Esc_Comm_Error_Process(void);
static void Can_Receive_Data_Process(void);

u32 g_u32SafetyCommData = 0u;
u8 g_u8SafetyCommDataValidate = 0u;

u32 const g_SafetyCommCycle20ms  = 0x00067831u;//0b00000000000000000000000001100111100000110001;    
u32 const g_SafetyCommCycle50ms  = 0x0001800Eu;//0b00000000000000000000000000011000000000001110;
u32 const g_SafetyCommCycle100ms = 0x001807c0u;//0b00000000000000000000000110000000011111000000;


/*******************************************************************************
* Function Name  : Esc_Comm_Error_Process
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
static void Esc_Comm_Error_Process(void)
{	
    u8 i,j;
    
    /* Clear the data when communicaiton timeout or error */
    for( j = 0u; j < DATA_FROM_SAFETY_LEN; j++ )
    {
        for( i = 0u; i < 8u; i++ )
        {
            EscDataFromSafety[j][i] = 0u;
        }
    }
}

/*******************************************************************************
* Function Name  : Can_Receive_Data_Process
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
static void Can_Receive_Data_Process(void)
{	
    static u8 stat_u8ReceiveTime1 = 0u,stat_u8ReceiveTime2 = 0u;
    static u8 stat_u8SEQN = 0u;
    
    
    if(( g_u32SafetyCommData & g_SafetyCommCycle20ms ) == g_SafetyCommCycle20ms )
    {
        g_u8SafetyCommDataValidate |= 0x01u;
        g_u32SafetyCommData &= ~g_SafetyCommCycle20ms;
    }
    else
    {
        g_u8SafetyCommDataValidate &= ~0x01u;
    }
    
    if( stat_u8ReceiveTime1  == 3u )
    {
        if(( g_u32SafetyCommData & g_SafetyCommCycle50ms ) == g_SafetyCommCycle50ms )
        {
            stat_u8ReceiveTime1 = 0u;
            g_u8SafetyCommDataValidate |= 0x02u;
            g_u32SafetyCommData &= ~g_SafetyCommCycle50ms;
        }
        else
        {
            g_u8SafetyCommDataValidate &= ~0x02u;
        }
    }
    else
    {
        stat_u8ReceiveTime1++;
    }

    if( stat_u8ReceiveTime2 == 5u )
    {
        if(( g_u32SafetyCommData & g_SafetyCommCycle100ms ) == g_SafetyCommCycle100ms )
        {
            stat_u8ReceiveTime2 = 0u;
            g_u8SafetyCommDataValidate |= 0x04u;
            g_u32SafetyCommData &= ~g_SafetyCommCycle100ms;
        }    
        else
        {
            g_u8SafetyCommDataValidate &= ~0x04u;
        } 
    } 
    else
    {
        stat_u8ReceiveTime2++;
    }
    

    
/*
Message ID: 100h.
Message cycle time: 20ms.
BYTE	BIT	DESCRIPTION		VALUE	
0	0-7	LIFE SIGNAL	
1	0	ORDER_RUN		1: Signal activated / 0: Signal no activated (Safety activates the order (=1) when changes to RUN state and removes the order (=0) when checks that the control board is in RUN state)
	1	ORDER_STOP		1: Signal activated / 0: Signal no activated (This order is to indicate to the Control that the system is going to stop due to a commanded stop not due to a fault, the order remains actived during the STOPPING PROCESS state)
	2	ORDER_FAULT		1: Signal activated / 0: Signal no activated  (Safety activates the order (=1) when a fault is detected and removes the order (=0) when checks that the control board is in FAULT state) 
	3	ORDER_RESET		1: Signal activated / 0: Signal no activated (Safety board activates reset order when the last active fault is reset, removes the order (=0) when checks that the control board is in READY state, maximum 200ms)
	4	ORDER_CONTACTOR		1: ACIVATE UP CONTACTOR / 0: DEACTIVATE UP CONT. (Control board only checks this order in READY state)
	5 	ORDER_SLOW_FAST		0: NOMINAL SPEED or DELTA / 1: LOW SPEED or STAR  (Control board checks this value when goes to RUN)
	6	ORDER_UP_DOWN		1: UP / 0: DOWN (Control board checks this value when goes to RUN)
	7	ORDER_CAPACITOR		1: Activation of main shaft brake capacitator Contactor (K4.2) / 0: Deactivation

2	0-2	STATE			0 0 0 : INIT / 1 0 0 : READY / 0 1 0 : STARTING PROCESS 
					0 0 1: RUN / 1 1 0 : INTERM / 1 0 1:  STOPPING PROCESS / 1 1 1: FAULT
        3-4	MODE 1			1 0: NORMAL / 0 1: INSPECTION / 0 0: NO VALID / 1 1: NO VALID
        5-7	MODE 2			0 0 0: CONTINOUS / 0 0 1: INTERMITTEND / 0 1 0: STBY / 1 0 0: INTERMITEND +STBY
					0 1 1: 2 DIRECTION / 1 0 1: Y/D SWITCHBACK		

3	0	TYPE OF STOP		1: INTERM STOP
	1	TYPE OF STOP		1: INSP BUTTONS RELEASED
	2	TYPE OF STOP		1: REMOTE
	3	TYPE OF STOP		1: SAFETY CURTAIN 
	4	STOPPING PROCESS	1: FINISH	0: STOPPING      (=0 during the STOPPING PROCESS state)			
	5	SWITCH ON ELEM.	        1: PEOPLE DETECTED UPPER
	6	SWITCH ON ELEM.	        1: PEOPLE DETECTED LOWER 	
	7	HANDRAIL ASME		1: HANDRAIL NOT SYNCRO (FOR ASME CODE)

4	0-7	WARNING		WARNING 1 = WARNING 1 CHA OR WARNING 1 CHB
5	0-7	WARNING		WARNING 2 = WARNING 2 CHA OR WARNING 2 CHB
6	0-7	WARNING		WARNING 3 = WARNING 3 CHA OR WARNING 3 CHB
7	0-7	WARNING		WARNING 4 = WARNING 4 CHA OR WARNING 4 CHB

*/    
    
    /* LIFE SIGNAL check */
    if( stat_u8SEQN != EscDataFromSafety[0][0] )
    {
        stat_u8SEQN = EscDataFromSafety[0][0];
    }
    else
    {
        /* error */
    }
    
    
    CMD_SF_ORDER = EscDataFromSafety[0][1];
    CMD_SF_STATE_MODE = EscDataFromSafety[0][2];    
    CMD_SF_STOP = EscDataFromSafety[0][3];
    
    /* WARNNING */
    CBEscData.WarnCodeSafety[0] = EscDataFromSafety[0][4];
    CBEscData.WarnCodeSafety[1] = EscDataFromSafety[0][5];
    CBEscData.WarnCodeSafety[2] = EscDataFromSafety[0][6];
    CBEscData.WarnCodeSafety[3] = EscDataFromSafety[0][7];
    
    /* FAULT CHA*/
    CBEscData.ErrorCodeSafetyCHA[0] = *(u16*)&EscDataFromSafety[1][0];
    CBEscData.ErrorCodeSafetyCHA[1] = *(u16*)&EscDataFromSafety[1][2];
    CBEscData.ErrorCodeSafetyCHA[2] = *(u16*)&EscDataFromSafety[2][0];
    CBEscData.ErrorCodeSafetyCHA[3] = *(u16*)&EscDataFromSafety[2][2];
    CBEscData.ErrorCodeSafetyCHA[4] = *(u16*)&EscDataFromSafety[3][0];

    /* FAULT CHB*/
    CBEscData.ErrorCodeSafetyCHB[0] = *(u16*)&EscDataFromSafety[1][4];
    CBEscData.ErrorCodeSafetyCHB[1] = *(u16*)&EscDataFromSafety[1][6];
    CBEscData.ErrorCodeSafetyCHB[2] = *(u16*)&EscDataFromSafety[2][4];
    CBEscData.ErrorCodeSafetyCHB[3] = *(u16*)&EscDataFromSafety[2][6];
    CBEscData.ErrorCodeSafetyCHB[4] = *(u16*)&EscDataFromSafety[3][2];
    
    /* SAFETY INPUT CHA */
    CBEscData.SafetyInputDataCHA[0] = EscDataFromSafety[4][0];
    CBEscData.SafetyInputDataCHA[1] = EscDataFromSafety[4][1];
    CBEscData.SafetyInputDataCHA[2] = EscDataFromSafety[4][2];
    CBEscData.SafetyInputDataCHA[3] = EscDataFromSafety[4][3];
    CBEscData.SafetyInputDataCHA[4] = EscDataFromSafety[4][4];
    CBEscData.SafetyInputDataCHA[5] = EscDataFromSafety[4][5];
    
    /* SAFETY INPUT CHB */
    CBEscData.SafetyInputDataCHB[0] = EscDataFromSafety[5][0];
    CBEscData.SafetyInputDataCHB[1] = EscDataFromSafety[5][1];
    CBEscData.SafetyInputDataCHB[2] = EscDataFromSafety[5][2];
    CBEscData.SafetyInputDataCHB[3] = EscDataFromSafety[5][3];
    CBEscData.SafetyInputDataCHB[4] = EscDataFromSafety[5][4];
    CBEscData.SafetyInputDataCHB[5] = EscDataFromSafety[5][5]; 
    
    /* MOTOR SPEED */
    CBEscData.SensorDataCHA[0] = *(u16*)&EscDataFromSafety[6][0];
    CBEscData.SensorDataCHA[1] = *(u16*)&EscDataFromSafety[6][2];
    CBEscData.SensorDataCHB[0] = *(u16*)&EscDataFromSafety[6][4];
    CBEscData.SensorDataCHB[1] = *(u16*)&EscDataFromSafety[6][6];    

    /* MAIN SHAFT SPEED */
    CBEscData.SensorDataCHA[2] = *(u16*)&EscDataFromSafety[7][0];
    CBEscData.SensorDataCHA[3] = *(u16*)&EscDataFromSafety[7][2];
    CBEscData.SensorDataCHB[2] = *(u16*)&EscDataFromSafety[7][4];
    CBEscData.SensorDataCHB[3] = *(u16*)&EscDataFromSafety[7][6];

    /* HANDRAIL SPEED */
    CBEscData.SensorDataCHA[4] = *(u16*)&EscDataFromSafety[8][0];
    CBEscData.SensorDataCHA[5] = *(u16*)&EscDataFromSafety[8][2];
    CBEscData.SensorDataCHB[4] = *(u16*)&EscDataFromSafety[8][4];
    CBEscData.SensorDataCHB[5] = *(u16*)&EscDataFromSafety[8][6];
    
    /* HANDRAIL (MOTOR PULSE) */
    CBEscData.SensorDataCHA[6] = *(u16*)&EscDataFromSafety[9][0];
    CBEscData.SensorDataCHA[7] = *(u16*)&EscDataFromSafety[9][2];
    CBEscData.SensorDataCHB[6] = *(u16*)&EscDataFromSafety[9][4];
    CBEscData.SensorDataCHB[7] = *(u16*)&EscDataFromSafety[9][6];
    
    /* MISSING STEP */
    CBEscData.SensorDataCHA[8] = *(u16*)&EscDataFromSafety[10][0];
    CBEscData.SensorDataCHA[9] = *(u16*)&EscDataFromSafety[10][2];
    CBEscData.SensorDataCHB[8] = *(u16*)&EscDataFromSafety[10][4];
    CBEscData.SensorDataCHB[9] = *(u16*)&EscDataFromSafety[10][6];  

    /* DBL2 UPPER INPUT CHA */
    CBEscData.DBL2UperInputDataCHA[0] = EscDataFromSafety[11][0];
    CBEscData.DBL2UperInputDataCHA[1] = EscDataFromSafety[11][1];
    CBEscData.DBL2UperInputDataCHA[2] = EscDataFromSafety[11][2];
    CBEscData.DBL2UperInputDataCHA[3] = EscDataFromSafety[11][3];
    
    /* DBL2 UPPER INPUT CHB */
    CBEscData.DBL2LowerInputDataCHB[0] = EscDataFromSafety[11][4];
    CBEscData.DBL2LowerInputDataCHB[1] = EscDataFromSafety[11][5];
    CBEscData.DBL2LowerInputDataCHB[2] = EscDataFromSafety[11][6];
    CBEscData.DBL2LowerInputDataCHB[3] = EscDataFromSafety[11][7];    
    
    /* DBL2 LOWER INPUT CHA */
    CBEscData.DBL2LowerInputDataCHA[0] = EscDataFromSafety[12][0];
    CBEscData.DBL2LowerInputDataCHA[1] = EscDataFromSafety[12][1];
    CBEscData.DBL2LowerInputDataCHA[2] = EscDataFromSafety[12][2];
    CBEscData.DBL2LowerInputDataCHA[3] = EscDataFromSafety[12][3];
    
    /* DBL2 LOWER INPUT CHB */
    CBEscData.DBL2LowerInputDataCHB[0] = EscDataFromSafety[12][4];
    CBEscData.DBL2LowerInputDataCHB[1] = EscDataFromSafety[12][5];
    CBEscData.DBL2LowerInputDataCHB[2] = EscDataFromSafety[12][6];
    CBEscData.DBL2LowerInputDataCHB[3] = EscDataFromSafety[12][7];
    
    /* DBL2 INTERM 1 INPUT CHA */
    CBEscData.DBL2Interm1InputDataCHA[0] = EscDataFromSafety[13][0];
    CBEscData.DBL2Interm1InputDataCHA[1] = EscDataFromSafety[13][1];
    CBEscData.DBL2Interm1InputDataCHA[2] = EscDataFromSafety[13][2];
    CBEscData.DBL2Interm1InputDataCHA[3] = EscDataFromSafety[13][3];
    
    /* DBL2 INTERM 1 INPUT CHB */
    CBEscData.DBL2Interm1InputDataCHB[0] = EscDataFromSafety[13][4];
    CBEscData.DBL2Interm1InputDataCHB[1] = EscDataFromSafety[13][5];
    CBEscData.DBL2Interm1InputDataCHB[2] = EscDataFromSafety[13][6];
    CBEscData.DBL2Interm1InputDataCHB[3] = EscDataFromSafety[13][7];   
    
    /* DBL2 INTERM 2 INPUT CHA */
    CBEscData.DBL2Interm2InputDataCHA[0] = EscDataFromSafety[14][0];
    CBEscData.DBL2Interm2InputDataCHA[1] = EscDataFromSafety[14][1];
    CBEscData.DBL2Interm2InputDataCHA[2] = EscDataFromSafety[14][2];
    CBEscData.DBL2Interm2InputDataCHA[3] = EscDataFromSafety[14][3];
    
    /* DBL2 INTERM 2 INPUT CHB */
    CBEscData.DBL2Interm2InputDataCHB[0] = EscDataFromSafety[14][4];
    CBEscData.DBL2Interm2InputDataCHB[1] = EscDataFromSafety[14][5];
    CBEscData.DBL2Interm2InputDataCHB[2] = EscDataFromSafety[14][6];
    CBEscData.DBL2Interm2InputDataCHB[3] = EscDataFromSafety[14][7];     
    
    /* ANALOG/PT100 UPPER */
    CBEscData.DBL2UpperAnalogData[0] |= (u8)EscDataFromSafety[15][0];
    CBEscData.DBL2UpperAnalogData[0] |= (u16)(((u16)EscDataFromSafety[15][1] << 8u) & 0x0f00u);
    CBEscData.DBL2UpperAnalogData[1] |= (u8)((EscDataFromSafety[15][1] >> 4u) & 0x0fu);
    CBEscData.DBL2UpperAnalogData[1] |= (u16)(((u16)EscDataFromSafety[15][2] << 4u) & 0x0ff0u);
    CBEscData.DBL2UpperAnalogData[2] = EscDataFromSafety[15][3];   
    
    /* ANALOG/PT100 LOWER */
    CBEscData.DBL2LowerAnalogData[0] |= (u8)EscDataFromSafety[15][4];
    CBEscData.DBL2LowerAnalogData[0] |= (u16)(((u16)EscDataFromSafety[15][5] << 8u) & 0x0f00u);
    CBEscData.DBL2LowerAnalogData[1] |= (u8)((EscDataFromSafety[15][5] >> 4u) & 0x0fu);
    CBEscData.DBL2LowerAnalogData[1] |= (u16)(((u16)EscDataFromSafety[15][6] << 4u) & 0x0ff0u);
    CBEscData.DBL2LowerAnalogData[2] = EscDataFromSafety[15][7];       
    
    /* ANALOG/PT100 INTERM 1 */
    CBEscData.DBL2Interm1AnalogData[0] |= (u8)EscDataFromSafety[16][0];
    CBEscData.DBL2Interm1AnalogData[0] |= (u16)(((u16)EscDataFromSafety[16][1] << 8u) & 0x0f00u);
    CBEscData.DBL2Interm1AnalogData[1] |= (u8)((EscDataFromSafety[16][1] >> 4u) & 0x0fu);
    CBEscData.DBL2Interm1AnalogData[1] |= (u16)(((u16)EscDataFromSafety[16][2] << 4u) & 0x0ff0u);
    CBEscData.DBL2Interm1AnalogData[2] = EscDataFromSafety[16][3];   
    
    /* ANALOG/PT100 INTERM 2 */
    CBEscData.DBL2Interm2AnalogData[0] |= (u8)EscDataFromSafety[16][4];
    CBEscData.DBL2Interm2AnalogData[0] |= (u16)(((u16)EscDataFromSafety[16][5] << 8u) & 0x0f00u);
    CBEscData.DBL2Interm2AnalogData[1] |= (u8)((EscDataFromSafety[16][5] >> 4u) & 0x0fu);
    CBEscData.DBL2Interm2AnalogData[1] |= (u16)(((u16)EscDataFromSafety[16][6] << 4u) & 0x0ff0u);
    CBEscData.DBL2Interm2AnalogData[2] = EscDataFromSafety[16][7];       
    
    /* DBL1 UPPER INPUT */
    CBEscData.DBL1UperInputData[0] = EscDataFromSafety[17][0];
    CBEscData.DBL1UperInputData[1] = EscDataFromSafety[17][1];
    CBEscData.DBL1UperInputData[2] = EscDataFromSafety[17][2];
    CBEscData.DBL1UperInputData[3] = EscDataFromSafety[17][3];
    
    /* DBL1 LOWER INPUT */
    CBEscData.DBL1LowerInputData[0] = EscDataFromSafety[17][4];
    CBEscData.DBL1LowerInputData[1] = EscDataFromSafety[17][5];
    CBEscData.DBL1LowerInputData[2] = EscDataFromSafety[17][6];
    CBEscData.DBL1LowerInputData[3] = EscDataFromSafety[17][7];  

    /* DBL1 INTERM 1 INPUT */
    CBEscData.DBL1Interm1InputData[0] = EscDataFromSafety[18][0];
    CBEscData.DBL1Interm1InputData[1] = EscDataFromSafety[18][1];
    CBEscData.DBL1Interm1InputData[2] = EscDataFromSafety[18][2];
    CBEscData.DBL1Interm1InputData[3] = EscDataFromSafety[18][3]; 
    
    /* DBL1 INTERM 2 INPUT */
    CBEscData.DBL1Interm2InputData[0] = EscDataFromSafety[18][4];
    CBEscData.DBL1Interm2InputData[1] = EscDataFromSafety[18][5];
    CBEscData.DBL1Interm2InputData[2] = EscDataFromSafety[18][6];
    CBEscData.DBL1Interm2InputData[3] = EscDataFromSafety[18][7];      
    
    /* Stopping Distace */
    CBEscData.SensorDataCHA[10] = *(u16*)&EscDataFromSafety[19][0];
    CBEscData.SensorDataCHB[10] = *(u16*)&EscDataFromSafety[19][2];
    
    /* end of safety string voltage */
    CBEscData.SensorDataCHA[11] = *(u16*)&EscDataFromSafety[19][4]; 
    
    /* run timer */
    ESC_TRANSPORTATION_TIMER = *(u16*)&EscDataFromSafety[20][0]; 
    ESC_DIRECTION_INDICATION_TIMER = *(u16*)&EscDataFromSafety[20][2];
    ESC_LOW_SPEED_TIMER = *(u16*)&EscDataFromSafety[20][4];
    ESC_REVERSING_TIMER = *(u16*)&EscDataFromSafety[20][6];
    
}

/*******************************************************************************
* Function Name  : Can_Send_Data_Process
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
static void Can_Send_Data_Process(void)
{	
    static u8 stat_u8SEQN = 1u;
    u16 crc;
    u8 result = 0;
    static u16 len = 0u;
    static u8 ParaSendFail = 0u;
    u8 j;  
    u8 *pCbInSafety = (u8*)&CBParameterInSafety;
    u32 i;
    u8 paracrc[4];    
    
/*
Message ID: 200h
Message cycle time: 20ms.
BYTE	BIT	DESCRIPTION		VALUE	
0	0-7	LIFE SIGNAL	
1	0-1	ORDER_REMOTE_KEY_SWITCH	1 0 : ON / 0 1: OFF  / 0 0: NO VALID / 1 1: NO VALID	(driver test is performed) (timing to generate a valid run order to the safety must be performed)
	2-3	ORDER_REMOTE_RUN 		1 0: ON / 0 1: OFF / 0 0: NO VALID / 1 1: NO VALID  	(driver test is not performed) (timing to generate a valid run order to the safety must be performed)
	4	ORDER_STOP			1: STOP (this order is to indicate to the Safety that the system is going to stop due to a commanded stop not due to a fault, the order remains actived during the STOPPING PROCESS state)	
	5	ORDER_FAULT			1: FAULT  (Control activates the order (=1) when a fault is detected and removes the order (=0) when checks that the safety board is in FAULT state)
        6	ORDER_RESET			1: RESET  (timing to generate a valid reset signal must be performed)	
        7	
2 	0-2	STATE				0 0 0 : INIT / 1 0 0 : READY / 0 0 1: RUN  / 1 0 1:  STOPPING PROCESS / 1 1 1: FAULT
	3-4	MODE				1 0: LOCAL / 0 1: REMOTE / 0 0: NO VALID / 1 1: NO VALID
	5	MODE				1: SLOW (TEC)
	6	MODE				1: STOP (Intermittend)
	7	MODE				1: 2 DIRECTION	
3	0-1	DIRECTION			1 0: UP / 0 1: DOWN / 0 0: NO VALID / 1 1: NO VALID   (Remote run direction order)			
	2-3	SPEED				1 0: NOMINAL SPEED / 0 1: LOW SPEED / 0 0: NO VALID / 1 1: NO VALID	(just feedback to the safety)		
	4	TEST AUX BRAKE		        1 : SWITCH AUX BRAKE TEST ON / 0 : SWITCH AUX BRAKE TEST OFF 		
	5-6	TYPE OF CONTROL FAULT	        11: FAILURE LOCK / 01: STANDARD FAULT / 10: RECOVERY STOP
	7			
4	0-7	CONTACTORS OUTPUTS		bit7	bit6	bit5	bit4	        bit3	   bit2	        bit1	bit0        
						K2.2.2	K2.1.2	K10.5	K10.1/K10.2     K2.2.1	   K2.1.1	K1.2	K1.1
5	0-7	

6-7	0-15	CRC

*/    
        
    if( CbBase_EscState != ESC_INIT_STATE )
    {    
        stat_u8SEQN++;
        if( stat_u8SEQN > 250u )
        {
            stat_u8SEQN = 1u;
        }
        
        /* LIFE SIGNAL */
        EscDataToSafety[0][0] = stat_u8SEQN;
        
        /* ORDER */
        EscDataToSafety[0][1] &= ~( ORDER_REMOTE_KEY_SWITCH_MASK | ORDER_FAULT );   
        EscDataToSafety[0][1] |= ORDER_REMOTE_KEY_SWITCH_OFF;
        EscDataToSafety[0][1] |= ORDER_REMOTE_RUN_OFF;
        
        if( CbBase_EscState == ESC_FAULT_STATE )
        {
            if(( CMD_SF_STATE_MODE & ESC_FROM_SF_STATE_MASK ) == ESC_FROM_SF_STATE_FAULT )
            {
                /* removes the order (=0) when checks that the control board is in FAULT state */
                EscDataToSafety[0][1] &= ~ORDER_FAULT;
            }
            else
            {
                /* Safety activates the order (=1) when a fault is detected  */
                /* ORDER_FAULT */ 
                EscDataToSafety[0][1] |= ORDER_FAULT;            
            }
        }   
        else
        {}
        
        /* STATE & MODE */    
        EscDataToSafety[0][2] &= ~( ESC_STATE_MASK | ESC_MODE_LOCAL );
        EscDataToSafety[0][2] |= (CbBase_EscState & ESC_STATE_MASK);
        EscDataToSafety[0][2] |= ESC_MODE_LOCAL;
        
        /* DIRECTION & SPEED & TEST AUX BRAKE & Fault type */
        EscDataToSafety[0][3] &= ~( ESC_MOTOR_MASK | ESC_TYPE_OF_CONTROL_FAULT );
        EscDataToSafety[0][3] |= (CMD_ESC_RUN & ESC_MOTOR_MASK);
        if( ErrorCodeControl[0] )
        {
            if( EscFaultType[ErrorCodeControl[0] - 1u] == EF )
            {
                EscDataToSafety[0][3] |= ESC_FAULT_FAILURE_LOCK;
            }
            else if( EscFaultType[ErrorCodeControl[0] - 1u] == ES )
            {
                EscDataToSafety[0][3] |= ESC_FAULT_STANDARD_FAULT;
            }
            else if( EscFaultType[ErrorCodeControl[0] - 1u] == ER )
            {
                EscDataToSafety[0][3] |= ESC_FAULT_RECOVERY_STOP;
            }            
            else
            {
                /* undefine fault type */
                EscDataToSafety[0][3] |= ESC_FAULT_STANDARD_FAULT;
            }
        }
        else
        {
            EscDataToSafety[0][3] &= ~ESC_TYPE_OF_CONTROL_FAULT;
        }
        
        /* CONTACTOR OUTPUTS */
        EscDataToSafety[0][4] = (CMD_OUTPUT_PORT2 >> 1u) & 0x0fu;
        
        /* NOT USED */
        EscDataToSafety[0][5] = 0u;
        
        /* CRC */
        len = 8u;
        crc = MB_CRC16( &EscDataToSafety[0][0], len - 2u );
        EscDataToSafety[0][len - 2u] = (u8)crc;
        EscDataToSafety[0][len - 1u] = (u8)(crc >> 8u); 
        
        /*
        Message ID: 201h
        Message cycle time: 20ms.
        BYTE	BIT	DESCRIPTION			VALUE	
        0	0-7	OUTPUTS (DBL UPPER)		1:ON / 0:OFF
        1	0-7	OUTPUTS (DBL LOWER)		1:ON / 0:OFF 
        2	0-7	OUTPUTS (DBL INTERM. 1)	        1:ON / 0:OFF 
        3	0-7	OUTPUTS (DBL INTERM. 2)	        1:ON / 0:OFF 
        4	0-7	
        5	0-7	
        6-7	0-7	CRC
        */ 
        
        /* OUTPUTS (DBL UPPER) */
        EscDataToSafety[1][0] = CBEscData.DBLOutputData[0];
        /* OUTPUTS (DBL LOWER) */
        EscDataToSafety[1][1] = CBEscData.DBLOutputData[1];
        /* OUTPUTS (DBL INTERM. 1) */
        EscDataToSafety[1][2] = CBEscData.DBLOutputData[2];
        /* OUTPUTS (DBL INTERM. 2) */
        EscDataToSafety[1][3] = CBEscData.DBLOutputData[3];
        
        /* CONTROL STANDARD INPUT (1-8) */ 
        EscDataToSafety[1][4] = CBEscData.ControlInputData[0];
        /* CONTROL STANDARD INPUT (9-16) */ 
        EscDataToSafety[1][5] = CBEscData.ControlInputData[1];    
        
        /* CRC */
        len = 8u;
        crc = MB_CRC16( &EscDataToSafety[1][0], len - 2u );
        EscDataToSafety[1][len - 2u] = (u8)crc;
        EscDataToSafety[1][len - 1u] = (u8)(crc >> 8u);     
        
        /* Send Message ID: 200h */
        result = Can_Send_Msg(CAN1, CAN1TX_CONTROL_DATA_ID, &EscDataToSafety[0][0], CAN_FRAME_LEN ); 
        if( result )
        {
            /* No mail box, send fail */
            g_u8CAN1SendFail |= 0x01u;
        }
        
        /* Send Message ID: 201h */
        result = Can_Send_Msg(CAN1, CAN1TX_CONTROL_DATA_ID+1u, &EscDataToSafety[1][0], CAN_FRAME_LEN ); 
        if( result )
        {
            /* No mail box, send fail */
            g_u8CAN1SendFail |= 0x02u;
        }         
        
#if 0    
        /* Send Message ID: 202h */
        result = Can_Send_Msg(CAN1, CAN1TX_CONTROL_DATA_ID+2u, &EscDataToSafety[2][0], CAN_FRAME_LEN ); 
        if( result )
        {
            /* No mail box, send fail */
            g_u8CAN1SendFail |= 0x04u;
        }
#endif    
    }
    else
    {
        if( g_u8ParameterRead == 2u )
        {
            /* packet the cs001 */
            CBParameterInSafety.p_TIME_LOW_SPEED = CBParameterData.p_TIME_LOW_SPEED;
            CBParameterInSafety.p_ADDITIONAL_DIRECTION_INDICATION_TIME = CBParameterData.p_ADDITIONAL_DIRECTION_INDICATION_TIME;
            CBParameterInSafety.p_ADDITIONAL_TRANSPORTATION_TIME = CBParameterData.p_ADDITIONAL_TRANSPORTATION_TIME;
            CBParameterInSafety.p_ADDITIONAL_REVERSING_TIME = CBParameterData.p_ADDITIONAL_REVERSING_TIME;
            CBParameterInSafety.p_DIAGNOSTIC_BOARD_L1_QUANTITY = CBParameterData.p_DIAGNOSTIC_BOARD_L1_QUANTITY;
            CBParameterInSafety.p_UPPER_DIAG_SS_LENGTH = CBParameterData.p_UPPER_DIAG_SS_LENGTH;
            CBParameterInSafety.p_LOWER_DIAG_SS_LENGTH = CBParameterData.p_LOWER_DIAG_SS_LENGTH;
            CBParameterInSafety.p_INTERM1_DIAG_SS_LENGTH = CBParameterData.p_INTERM1_DIAG_SS_LENGTH;
            CBParameterInSafety.p_INTERM2_DIAG_SS_LENGTH = CBParameterData.p_INTERM2_DIAG_SS_LENGTH;
            CBParameterInSafety.p_SPARE_PARAMETER_11 = CBParameterData.p_SPARE_PARAMETER_11;
            CBParameterInSafety.p_SPARE_PARAMETER_12 = CBParameterData.p_SPARE_PARAMETER_12;
            CBParameterInSafety.p_SPARE_PARAMETER_16 = CBParameterData.p_SPARE_PARAMETER_16;
            CBParameterInSafety.p_SPARE_PARAMETER_17 = CBParameterData.p_SPARE_PARAMETER_17;
            
            for( i = 0u; i < 16u; i++ )
            {
                CBParameterInSafety.p_IO_PARAMETER_CONTROL_INPUT[i] = CBParameterData.p_IO_PARAMETER_CONTROL_INPUT[i];
            }
            for( i = 0u; i < 32u; i++ )
            {
                CBParameterInSafety.p_IO_PARAMETER_DBL1_UPPER_INPUT[i] = CBParameterData.p_IO_PARAMETER_DBL1_UPPER_INPUT[i];
            }
            for( i = 0u; i < 32u; i++ )
            {
                CBParameterInSafety.p_IO_PARAMETER_DBL1_LOWER_INPUT[i] = CBParameterData.p_IO_PARAMETER_DBL1_LOWER_INPUT[i];
            }
            for( i = 0u; i < 32u; i++ )
            {
                CBParameterInSafety.p_IO_PARAMETER_DBL1_INTERM1_INPUT[i] = CBParameterData.p_IO_PARAMETER_DBL1_INTERM1_INPUT[i];
            }
            for( i = 0u; i < 32u; i++ )
            {
                CBParameterInSafety.p_IO_PARAMETER_DBL1_INTERM2_INPUT[i] = CBParameterData.p_IO_PARAMETER_DBL1_INTERM2_INPUT[i];
            }
            
            i = MB_CRC32( (u8*)&CBParameterInSafety, (ESC_CB_PARAMETER_IN_SF_DATA_LEN - 4u) , PARAMETER_POLYNOMIALS);
            
            paracrc[0] = (u8)(i >> 24u);
            paracrc[1] = (u8)(i >> 16u);     
            paracrc[2] = (u8)(i >> 8u);
            paracrc[3] = (u8)i;
            
            CBParameterInSafety.CRC32 = *(u32*)&paracrc; 
            
            g_u8ParameterRead = 3u;
        }
        /* Init state */
        /* Send CS0001.bin file to Safety board */                
        ParaDataToSafety[0] = g_u8ParameterRead;               
        ParaDataToSafety[1] = (u8)ESC_CB_PARAMETER_IN_SF_DATA_LEN & 0xffu;
        ParaDataToSafety[2] = (u8)( ESC_CB_PARAMETER_IN_SF_DATA_LEN >> 8u ) & 0xffu;
        if(( ESC_CB_PARAMETER_IN_SF_DATA_LEN > 0u ) && ( ESC_CB_PARAMETER_IN_SF_DATA_LEN == len ))
        {
            /* send finish */
            ParaDataToSafety[3] = 1u;
            g_u8ParameterRead = 4u;
        }
        else
        {
            ParaDataToSafety[3] = 0u;
        }
        ParaDataToSafety[4] = 0u;
        ParaDataToSafety[5] = 0u;
        ParaDataToSafety[6] = 0u;
        ParaDataToSafety[7] = 0u;
        result = Can_Send_Msg(CAN1, CAN1TX_PARA_STATUS_ID, ParaDataToSafety, CAN_FRAME_LEN ); 
        if( result )
        {
            /* No mail box, send fail */
        }       
        
        
        if( g_u8ParameterRead == 3u )
        {           
            for( j = 0u; j < 2u; j++ )
            {
                if(( ESC_CB_PARAMETER_IN_SF_DATA_LEN > len ) || ( ParaSendFail == 1u ))
                {    
                    if( ParaSendFail == 0u )
                    {
                        for( i = 0u; i < 8u; i++ )
                        {
                            ParaDataToSafety[i] = 0u;
                        }  
                        
                        if( ESC_CB_PARAMETER_IN_SF_DATA_LEN - len > 8u )
                        {
                            for( i = 0u; i < 8u; i++ )
                            {
                                ParaDataToSafety[i] = pCbInSafety[len + i];
                            }
                            len += 8u;
                        }
                        else
                        {
                            for( i = 0u; i < ESC_CB_PARAMETER_IN_SF_DATA_LEN - len; i++ )
                            {
                                ParaDataToSafety[i] = pCbInSafety[len + i];
                            }
                            len = ESC_CB_PARAMETER_IN_SF_DATA_LEN;
                        }
                    }
                    result = Can_Send_Msg(CAN1, (CAN1TX_PARA_CB_IN_SAFETY_ID1 + ((len - 1u)/ 8u)), ParaDataToSafety, CAN_FRAME_LEN ); 
                    if( result )
                    {
                        /* No mail box, send fail */
                        ParaSendFail = 1u;
                    }
                    else
                    {
                        ParaSendFail = 0u;
                    }
                }
            }
        }        
    }
}

/*******************************************************************************
* Function Name  : Can_Receive_Data
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
static void Can_Receive_Data(void)
{	
    BaseType_t xStatus;   
    CAN_MSG DataFromSF;
    static u8 receivelen = 0u;
    u8 *pCBPara = (u8*)&CBParameterData;
    u8 *pSFPara = (u8*)&SFParameterData;
    u8 i,j;
    
    receivelen = uxQueueMessagesWaiting(xQueue);
    for( j = 0u; j < receivelen; j++ )
    {
        xStatus = xQueueReceive( xQueue, &DataFromSF, 0 ); 
        if( xStatus != pdPASS ) 
        { 
            /* error */
            i = 0u;
            break;
        } 
        else
        {
            /* receive sf data */
            if(( DataFromSF.msg_id >= CAN1RX_SAFETY_DATA1_ID ) && ( DataFromSF.msg_id <= CAN1RX_SAFETY_DATA3_ID ))
            {
                g_u32SafetyCommData |= ( 1u << ( DataFromSF.msg_id - CAN1RX_SAFETY_DATA1_ID ));
                for( i = 0u; i < 8u; i++ )
                {
                    EscDataFromSafety[DataFromSF.msg_id - CAN1RX_SAFETY_DATA1_ID][i] = DataFromSF.msg_data[i];
                }                
            }
            /* receive cb para */
            else if(( DataFromSF.msg_id >= CAN1RX_PARA_STATUS_ID ) && ( DataFromSF.msg_id <= CAN1RX_PARA_CONTROL_ID2 ))
            {
                if( DataFromSF.msg_id == CAN1RX_PARA_STATUS_ID )
                {
                    for( i = 0u; i < 8u; i++ )
                    {
                        ParaDataFromSafety[0][i] = DataFromSF.msg_data[i];
                    } 
                }
                else
                {
                    for( i = 0u; i < 8u; i++ )
                    {
                        pCBPara[(DataFromSF.msg_id - CAN1RX_PARA_CONTROL_ID1)*8u + i] = DataFromSF.msg_data[i];
                    } 
                }
            }
            /* receive sf para */
            else if(( DataFromSF.msg_id >= CAN1RX_PARA_SAFETY_ID1 ) && ( DataFromSF.msg_id <= CAN1RX_PARA_SAFETY_ID2 ))
            {
                for( i = 0u; i < 8u; i++ )
                {
                    pSFPara[(DataFromSF.msg_id - CAN1RX_PARA_SAFETY_ID1)*8u + i] = DataFromSF.msg_data[i];
                }                
            }
            else
            {}
        }
    }
}

/*******************************************************************************
* Function Name  : can_comm_thread
* Description    : CAN Communication.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void can_comm_thread(void *arg)
{
    static u8 can1_comm_timeout = 0;
    static u16 stat_u16TimerCan1CommWait = 0u, stat_u16Can1HandshakeSuccess = 0u;
    static u16 stat_u16TimeCan1Error = 0u;
    
//    CAN_ITConfig(CAN1,CAN_IT_FMP0 , ENABLE); 		    
//    CAN_ITConfig(CAN1,CAN_IT_FF0, ENABLE); 		     
//    
//    CAN_ITConfig(CAN1,CAN_IT_FMP1 , ENABLE); 		    
//    CAN_ITConfig(CAN1,CAN_IT_FF1, ENABLE); 		         
    
    while(1)
    {               
        /* Receive data from SF */
        Can_Receive_Data();
        
        if( CbBase_EscState != ESC_INIT_STATE )
        {
            Can_Receive_Data_Process();        
            
            /* CAN error check */            
            if( can1_receive == 0u )
            {
                if( stat_u16TimeCan1Error < 0xffffu )
                {
                    stat_u16TimeCan1Error++;
                }
                
                /* 1000ms */
                if( stat_u16TimeCan1Error > 50u )
                {
                    stat_u16TimeCan1Error = 0u;
                    if( CAN1->ESR & CAN_ESR_BOFF )
                    {
                        CAN_Int_Init(CAN1);
                    }                    

                }
            }
            else
            {
                stat_u16TimeCan1Error = 0u;
                can1_receive = 0u;
            }
            
            /* Communicate timeout check */
#if 0
            if( can1_receive == 1u )
            {
                can1_receive = 0u;
                can1_comm_timeout = 0;
                stat_u16Can1HandshakeSuccess = 1u;
                EN_ERROR48 &= ~0x04u;
            }
#else        
            if( g_u8SafetyCommDataValidate == 0x07u )
            {
                can1_comm_timeout = 0;
                stat_u16Can1HandshakeSuccess = 1u;
            }
#endif        
            else if( stat_u16Can1HandshakeSuccess )
            {
                if( can1_comm_timeout < 0xffu )
                {
                    can1_comm_timeout++;
                }
                /* 200 ms */
                if( can1_comm_timeout > 10u )
                {
                    /*  Communication error Control F378 */
                    EN_ERROR48 |= 0x04u;         
                    Esc_Comm_Error_Process();
                    
                    /* unknow, for test */
                    /*
                    if( can1_comm_timeout > 250u )
                    {
                        can1_comm_timeout = 0u;
                        CAN_Int_Init(CAN1);
                    }
                    */
                }
            }
            else
            {
                if( stat_u16TimerCan1CommWait < 0xffffu )
                {
                    stat_u16TimerCan1CommWait++;
                }
                if( stat_u16TimerCan1CommWait * 20u > CAN_COMM_HAND_TIME )
                {
                    /*  Communication error Control F378 */
                    EN_ERROR48 |= 0x04u;         
                    Esc_Comm_Error_Process(); 
                }            
            }           
        }   
        
        /* Send data to SF */
        Can_Send_Data_Process();       
        
        
        xEventGroupSetBits(xCreatedEventGroup, TASK_BIT_1);
        vTaskDelay( 20 );
    }
}
	


/*******************************************************************************
* Function Name  : can_comm_init
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void can_comm_init(void)
{	
    xQueue = xQueueCreate(CAN_DATA_QUEUE_LEN, sizeof(CAN_MSG));
    if( xQueue != NULL)
    {
#if configQUEUE_REGISTRY_SIZE > 0
        vQueueAddToRegistry(xQueue, "comm");
#endif        
        xTaskCreate(can_comm_thread, "CAN", configMINIMAL_STACK_SIZE * 4, NULL, CAN_THREAD_PRIO, NULL);
    }       
}

/******************************  END OF FILE  *********************************/