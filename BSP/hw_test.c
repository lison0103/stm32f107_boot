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
    
    if( testmode == 1 )
    {
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
                *ulPt_Output |= 0x6000;
            }
            else
            {
                *ulPt_Output |= ((u16)( 1 << ( inputnum - 1 )));
            }
            
            dis_data[0] = 0;
            dis_data[1] = inputnum/10;
            dis_data[2] = inputnum%10;
        }
        
        //    led_display();
    }
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
    len = BSP_CAN_Receive(CAN1, &CAN1_RX_Urge, CAN1_RX2_Data, 0);
    
    BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1TX_NORMAL_ID, CAN1_TX_Data, 100);
    
    if( testmode == 1 )
    {
        BSP_CAN_Send(CAN2, &CAN2_TX_Normal, CAN1_TEST_ID, CAN2_TX_Data, 10);
    }
  
}


/*******************************************************************************
* Function Name  : input_can_task
* Description    : input output and can communication.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void input_can_task(void *arg)
{	 
        static u8 can_comm_tms = 0;
    
 	while(1)
	{
            
                Get_GpioInput(&EscRTBuff[4]);
                Input_Check();
                led_display();
                output_driver(&EscRTBuff[30]);            
            
                if( ++can_comm_tms >= 4 )
                {                   
                        can_comm_tms = 0;
                        
                        CAN_Comm();
                        
                        /* for test ----------------------*/
                        SF_ESC_STATE = CAN1_RX2_Data[0];
                        ESC_ERROR_CODE[0] = CAN1_RX2_Data[2];                
                        
                        if( testmode != 1 )
                        {
                            if( SF_ESC_STATE & ( 1 << 2 ))
                            {
                                CMD_OUTPUT_PORT &= ~0x04;
                                POWER_ON_TMS++;
                            }
                            else
                            {
                                CMD_OUTPUT_PORT |= 0x04;
                                POWER_ON_TMS = 0;
                            }
                        }
                        
                        dis_data[0] = 0;
                        dis_data[1] = ESC_ERROR_CODE[0]/10;
                        dis_data[2] = ESC_ERROR_CODE[0]%10;
                        
                        error_record_check();
                        write_error_record_to_eeprom();               
                }               
                              
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
	xTaskCreate(input_can_task, "INPUT_TEST", configMINIMAL_STACK_SIZE * 2, NULL, INPUT_TASK_PRIO, NULL);
}


/*******************************************************************************
* Function Name  : HardwareTEST
* Description    : Test the board.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void HardwareTEST(void)
{
    u8 testdata1[10],testdata2[10];
    u8 testerror = 0;
    u8 len1 = 0, len2 = 0;
    u16 waittms = 0;
    
    CAN1_TX_Data[0] = 0xf1;
    CAN2_TX_Data[0] = 0xf1;
    testdata1[0] = 0xf1;
    testdata2[0] = 0xf1;
    for( u8 i = 1; i < 10 ; i++ )
    {
        CAN1_TX_Data[i] = i + 0xb0;
        testdata1[i] = i + 0xb0;
        CAN2_TX_Data[i] = i + 0xc0;
        testdata2[i] = i + 0xc0;        
    }
    BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1_TEST_ID, CAN1_TX_Data, 10);
    BSP_CAN_Send(CAN2, &CAN2_TX_Normal, CAN1_TEST_ID, CAN2_TX_Data, 10);
    
    do
    {
        len1 = BSP_CAN_Receive(CAN1, &CAN1_RX_Normal, CAN1_RX_Data, 0);
        delay_ms(1);
        EWDT_TOOGLE();
        waittms++;
        if( waittms > 2000 )
        {
            waittms = 0;
            break;
        }
    }
    while( len1 != 10 || CAN1_RX_Data[0] != 0xf1 );     

    waittms = 0;
    do
    {
        len2 = BSP_CAN_Receive(CAN2, &CAN2_RX_Normal, CAN2_RX_Data, 0);
        delay_ms(1);
        EWDT_TOOGLE();
        waittms++;
        if( waittms > 100 )
        {
            waittms = 0;
            break;
        }
    }
    while( len2 != 10 || CAN2_RX_Data[0] != 0xf1 );   
    
    if( len1 == 10 && CAN1_RX_Data[0] == 0xf1 )
    {
        waittms = 0;
        for( u8 i = 1; i < 10 ; i++ )
        {
            CAN1_TX_Data[i] = CAN1_RX_Data[i];
        }
        BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1_TEST_ID, CAN1_TX_Data, 10);
        
        if( len2 == 10 && CAN2_RX_Data[0] == 0xf1 )
        {
            waittms = 0;
            for( u8 i = 1; i < 10 ; i++ )
            {
                CAN2_TX_Data[i] = CAN2_RX_Data[i];
            }
            BSP_CAN_Send(CAN2, &CAN2_TX_Normal, CAN1_TEST_ID, CAN2_TX_Data, 10);
        }        
        
        waittms = 0;
        do
        {
            len1 = BSP_CAN_Receive(CAN1, &CAN1_RX_Normal, CAN1_RX_Data, 0);
            delay_ms(1);
            EWDT_TOOGLE();
            waittms++;
            if( waittms > 2000 )
            {
                waittms = 0;
                break;
            }
        }
        while( len1 != 10 || CAN1_RX_Data[0] != 0xf1 ); 
        
        waittms = 0;
        do
        {
            len2 = BSP_CAN_Receive(CAN2, &CAN2_RX_Normal, CAN2_RX_Data, 0);
            delay_ms(1);
            EWDT_TOOGLE();
            waittms++;
            if( waittms > 100 )
            {
                waittms = 0;
                break;
            }
        }
        while( len2 != 10 || CAN2_RX_Data[0] != 0xf1 );         
        
        
        if( len1 == 10 && CAN1_RX_Data[0] == 0xf1 )
        {
            for( u8 i = 1; i < 10 ; i++ )
            {
                if( CAN1_RX_Data[i] != testdata1[i] )
                {
                    testerror++;
                    break;
                }
            }

            if( len2 == 10 && CAN2_RX_Data[0] == 0xf1 )
            {
                for( u8 i = 1; i < 10 ; i++ )
                {
                    if( CAN2_RX_Data[i] != testdata2[i] )
                    {
                        testerror++;
                        break;
                    }
                }
            }  
            
            if( testerror == 0 )
            {
                testmode = 1;
            }
        } 
        
        
    }
    else
    {
        CAN_FilterInitTypeDef  	        CAN_FilterInitStructure;
        /* CAN1 filter init */
        CAN_FilterInitStructure.CAN_FilterNumber=0;	
        CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	
        CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	
        
        CAN_FilterInitStructure.CAN_FilterIdHigh=(((u32)0x0064<<3)&0xFFFF0000)>>16;	
        CAN_FilterInitStructure.CAN_FilterIdLow=(((u32)0x0064<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;//32-bit MASK 
        CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xff87; 
        
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
        CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
        CAN_FilterInit(&CAN_FilterInitStructure);          
    }
}

/******************************  END OF FILE  *********************************/




