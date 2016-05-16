/*******************************************************************************
* File Name          : ewdt.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains the external watchdog functions.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"
#include "delay.h"
#include "ewdt.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : EWDT_Drv_pin_config
* Description    : Configure the pin.
*                  the external watchdog reset  min:1.12 typ:1.6 max:2.4
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void EWDT_Drv_pin_config(void)
{
  
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOC , &GPIO_InitStruct);

}

/*******************************************************************************
* Function Name  : write_bkp
* Description    : This function write the cpu backup register to store data.
*                  
* Input          : adr: the address of backup register
*                  dat: data to be written
* Output         : None
* Return         : None
*******************************************************************************/
void write_bkp(u16 adr,u16 dat)
{
  PWR_BackupAccessCmd(ENABLE);
  BKP_WriteBackupRegister(adr, dat);   
  PWR_BackupAccessCmd(DISABLE);
}

/*******************************************************************************
* Function Name  : ExtWdtCheck
* Description    : Check the external watchdog.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void ExtWdtCheck(void)
{
  u16 bkr_rst_flag=0;

  delay_ms(10); 
  EWDT_TOOGLE();
  delay_ms(10);    
  EWDT_TOOGLE();
  delay_ms(10);    
  EWDT_TOOGLE();
    
  bkr_rst_flag = BKP_ReadBackupRegister(BKP_DR1);
  if(bkr_rst_flag == 0xfa01)
  {
    /** soft reset **/
//    if(RCC_GetFlagStatus(RCC_FLAG_SFTRST) == SET)
//    {
//        
//    }
    /** pin reset **/
    if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != SET)
    {
      write_bkp(BKP_DR1, 0);  
      RCC_ClearFlag();   
//      EN_ERROR_SYS1 |= 0x01;
    }   
  }  
  else
  {

    write_bkp(BKP_DR1, 0xfa01);   
    RCC_ClearFlag();
    
    delay_ms(2000);    
    
//    EN_ERROR_SYS1 |= 0x01;
  }  
  
  write_bkp(BKP_DR1, 0);  
  
//  if( EN_ERROR_SYS1&0x01 ) 
//  {
//      ESC_EWDT_Error_Process();
//  }
  

}  




/******************************  END OF FILE  *********************************/


