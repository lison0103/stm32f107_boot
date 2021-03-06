/*******************************************************************************
* File Name          : hw_test.c
* Author             : lison
* Version            : V1.0
* Date               : 09/30/2016
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



void Input_Check(void)
{
#if 0    
    u8 i,sflag,inputnum;    
    u16 *ulPt_Input,*ulPt_Output;
    
    if( testmode == 1 )
    {
        sflag = 0;
        inputnum = 0;
        ulPt_Input = (u16*)&CBEscData.ControlInputData[0];
        ulPt_Output = (u16*)&CBEscData.ControlOutputData[0];
        
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
    else if( testmode == 2 )
    {
        
        inputnum = CAN1_RX_Data[0];
        sflag = CAN1_RX_Data[1];
        ulPt_Output = (u16*)&CBEscData.ControlOutputData[0];
        
   
        
        if(( inputnum == 0 ) || ( sflag > 1 ))
        {
            
            *ulPt_Output = 0;       
            
            dis_data[0] = 0;
            dis_data[1] = 0;
            dis_data[2] = 0;        
            
        }
        else
        {
            if( inputnum >= 16 && inputnum <= 46 )
            {
                if( inputnum & 0x10 )
                {
                    *ulPt_Output |= 0x0001;
                }
                else if( inputnum & 0x20 ) 
                {
                    *ulPt_Output |= 0x0002;;
                }
            }
            dis_data[0] = 0;
            dis_data[1] = inputnum/10;
            dis_data[2] = inputnum%10;
        }        
    
    }
#endif    
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
#if 0    
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
        if( waittms > 8000 )
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
        for( u8 i = 2; i < 10 ; i++ )
        {
            CAN1_TX_Data[i] = CAN1_RX_Data[i];
        }
        BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1_TEST_ID, CAN1_TX_Data, 10);
        
        if( len2 == 10 && CAN2_RX_Data[0] == 0xf1 )
        {
            waittms = 0;
            for( u8 i = 2; i < 10 ; i++ )
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
            for( u8 i = 2; i < 10 ; i++ )
            {
                if( CAN1_RX_Data[i] != testdata1[i] )
                {
                    testerror++;
                    break;
                }
            }

            if( len2 == 10 && CAN2_RX_Data[0] == 0xf1 )
            {
                for( u8 i = 2; i < 10 ; i++ )
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
                if( CAN1_RX_Data[1] == 0xe1 )
                {
                    testmode = 2;
                }
                else if( CAN1_RX_Data[1] == 0xa1 || CAN1_RX_Data[1] == 0xc1 )
                {
                    testmode = 1;
                }
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
#endif    
}

/******************************  END OF FILE  *********************************/




