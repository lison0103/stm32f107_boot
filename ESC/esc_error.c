/*******************************************************************************
* File Name          : esc_error.c
* Author             : lison
* Version            : V1.0
* Date               : 04/08/2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "esc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

u8 *const pcCurrentRecordErrorBuff = &Modbuff[446];




/*******************************************************************************
* Function Name  : error_record_incode
* Description    : //²¶×½Í£ÌÝ/¹ÊÕÏ´úÂë
*                  
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
void error_record_incode(void)
{
	u16 i=0,j=0; 
        u16 *ptErrorRecordCounter = (u16*)&pcStatDataBuff[28];

    
        if((*ptErrorRecordCounter)>300) (*ptErrorRecordCounter)=300;   

	pcCurrentRecordErrorBuff[0] = 0xFA; 
	pcCurrentRecordErrorBuff[1] = ESC_ERROR_CODE[0];
	pcCurrentRecordErrorBuff[2] = pcMbRtccBuff[0];
	pcCurrentRecordErrorBuff[3] = pcMbRtccBuff[1];
	pcCurrentRecordErrorBuff[4] = pcMbRtccBuff[2];
	pcCurrentRecordErrorBuff[5] = pcMbRtccBuff[3];
	pcCurrentRecordErrorBuff[6] = pcMbRtccBuff[4];
	pcCurrentRecordErrorBuff[7] = pcMbRtccBuff[5];
   
	(*ptErrorRecordFirstItem)++;                                           
	if(((*ptErrorRecordFirstItem)>300) || (!(*ptErrorRecordFirstItem))) 
        {
            (*ptErrorRecordFirstItem) = 1;
	}
  
	i = ((*ptErrorRecordFirstItem)-1)*8;                          
	
	for(j=0;j<8;j++)
	{	
		pcErrorRecordBuff[i+j] = pcCurrentRecordErrorBuff[j];
	} 
	
}


/******************************  END OF FILE  *********************************/


