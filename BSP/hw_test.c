/*******************************************************************************
* File Name          : hw_test.c
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : Contains some test function.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "delay.h"
#include "hw_test.h"
#include "led.h"
#include "digital_led.h"
#include "usart.h"
#include "ewdt.h"
#include "can.h"
#include "bsp_iocfg.h"
#include "includes.h"
#include "crc16.h"
#include "esc.h"
#include "esc_eeprom_process.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern u8 dis_data[3];
u8 sflag,t,inputnum = 0;
u8 passflag = 1;



#if 0

/*******************************************************************************
* Function Name  : Input_Check
* Description    : Monitor the input pin status test.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/ 
void Input_Check(void)
{
    
        u16 *ulPt_Input,*ulPt_Output,*pc_can_tx;
        
        ulPt_Input = (u16*)&EscRTBuff[4];
        ulPt_Output = (u16*)&EscRTBuff[30];
        pc_can_tx = (u16*)&CAN1_TX_Data[0];
                
        
        CAN1_TX_Data[0] = 0;
        CAN1_TX_Data[1] = 0;
    
        
        if(passflag && ( IN1 && IN2 && IN3 && IN4 && IN5 && IN6 && IN7 && IN8 && 
                        IN9 && IN10 && IN11 && IN12 && IN13 && IN14 && IN15 && IN16 ))
        {
            
                *ulPt_Output = 0xffff;
                    
                CAN1_TX_Data[0] = 0xff;
                CAN1_TX_Data[1] = 0xff;        
        }
        else   
        {                
                *ulPt_Output = 0;              
                
                for( i = 0; i < 16; i++ )
                {
                    if( ulPt_Input[0] & ( 1 << i ))
                    {
                       *pc_can_tx |= 1 << i;
                    }
                }           
        }
                    
    
}

#else

void Input_Check(void)
{
    u8 i,sflag,inputnum;    
    u16 *ulPt_Input,*ulPt_Output;
    
    sflag = 0;
    inputnum = 0;
    ulPt_Input = (u16*)&EscRTBuff[4];
    ulPt_Output = (u16*)&EscRTBuff[30];
    
    for( i = 0; i < 16; i++ )
    {
        if( ulPt_Input[0] & ((u16)( 1 << i )))
        {
            sflag++;
            inputnum = i + 1;
        }
    }    
    
    if(( inputnum == 0 ) || ( sflag > 1 ))
    {
        
        *ulPt_Output = 0;       
        
        dis_data[0] = 0;
        dis_data[1] = 0;
        dis_data[2] = 0;        
        
    }
    else
    {
        if( inputnum == 16 )
        {
            *ulPt_Output |= ((u16)( 1 << ( inputnum - 16 )));
        }
        else
        {
            *ulPt_Output |= ((u16)( 1 << ( inputnum - 1 )));
        }
        
        dis_data[0] = 0;
        dis_data[1] = inputnum/10;
        dis_data[2] = inputnum%10;
    }
    
    led_display();
 
}
#endif


/*******************************************************************************
* Function Name  : CAN_Comm
* Description    : CAN Communication test.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_Comm(void)
{
    static u8 can1_comm_timeout,can2_comm_timeout = 0;
    u8 len = 0;
    
    if( can1_receive == 1 )
    {
        can1_receive = 0;
        can1_comm_timeout = 0;
    }
    else if( ++can1_comm_timeout >= 3 )
    {
        /*  can communication timeout process */
    }  
    
    if( can2_receive == 1 )
    {
        can2_receive = 0;
        can2_comm_timeout = 0;
    }
    else if( ++can2_comm_timeout >= 3 )
    {
        /*  can communication timeout process */
    }    

    CAN1_TX_Data[0] = EscRTBuff[4];
    CAN1_TX_Data[1] = EscRTBuff[5];
    
    len = BSP_CAN_Receive(CAN1, &CAN1_RX_Normal, CAN1_RX_Data, 0);
    len = BSP_CAN_Receive(CAN1, &CAN1_RX_Urge, CAN2_RX_Data, 0);
    
    BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1TX_NORMAL_ID, CAN1_TX_Data, 100);
  
}

/*******************************************************************************
* Function Name  : can_task
* Description    : None
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void can_task(void *arg)
{	 
    
 	while(1)
	{

                CAN_Comm();
                
                /* for test ----------------------*/
                ESC_STATE1 = CAN2_RX_Data[0];
                ESC_ERROR_CODE[0] = CAN2_RX_Data[1];                
                
                if( ESC_STATE1 & ( 1 << 1 ))
                {
                    CMD_OUTPUT_PORT &= ~0x04;
                    POWER_ON_TMS++;
                }
                else
                {
                    CMD_OUTPUT_PORT |= 0x04;
                    POWER_ON_TMS = 0;
                }
                
                error_record_check();
                write_error_record_to_eeprom();
                              
                vTaskDelay( 20 );		   
	}
}

/*******************************************************************************
* Function Name  : input_can_task
* Description    : None
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void input_test_task(void *arg)
{	 
    
 	while(1)
	{
                Get_GpioInput(&EscRTBuff[4]);
//                Input_Check();
                output_driver(&EscRTBuff[30]);
                              
                vTaskDelay( 5 );		   
	}
}


/*******************************************************************************
* Function Name  : input_test_init
* Description    : None
*                  
* Input          : None
*                  None                 
* Output         : None
* Return         : None
*******************************************************************************/ 
void input_test_init(void)
{
	xTaskCreate(input_test_task, "INPUT_TEST", configMINIMAL_STACK_SIZE * 2, NULL, INPUT_TASK_PRIO, NULL);
}

/*******************************************************************************
* Function Name  : can_comm_test_init
* Description    : None
*                  
* Input          : None
*                  None                 
* Output         : None
* Return         : None
*******************************************************************************/ 
void can_comm_test_init(void)
{
	xTaskCreate(can_task, "CAN_COMM", configMINIMAL_STACK_SIZE * 2, NULL, INPUT_TASK_PRIO, NULL);
}

/******************************  END OF FILE  *********************************/




