/*******************************************************************************
* File Name          : stmflash.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains stm flash functions.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stmflash.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Selected the STM32 FLASH capacity size (K) */
#define STM32_FLASH_SIZE 256 	 
/* FLASH write enable (0, not enabled; 1, to enable) */
#define STM32_FLASH_WREN 1                     

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 
#else 
#define STM_SECTOR_SIZE	2048
#endif		 

/* STM32 FLASH start address */
#define STM32_FLASH_BASE 0x08000000 	        

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/*******************************************************************************
* Function Name  : STMFLASH_ReadHalfWord
* Description    : read the specified address half-word (16-bit data)
*                  
* Input          : faddr: the read address (This address must be a multiple of 2 !!)
*                  
* Output         : None
* Return         : None
*******************************************************************************/
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}


#if STM32_FLASH_WREN	

/*******************************************************************************
* Function Name  : STMFLASH_Write_NoCheck
* Description    : Do not check writing
*                  
* Input          : WriteAddr: starting address
*                  pBuffer: Data Pointer
*                  NumToWrite: half-word (16-bit) number   
* Output         : None
* Return         : None
*******************************************************************************/ 
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
            FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;
	}  
} 


/*******************************************************************************
* Function Name  : STMFLASH_Write
* Description    : Specifies the length of the start writing data from the specified address
*                  
* Input          : WriteAddr: start address (This address must be a multiple of 2 !!)
*                  pBuffer: Data Pointer
*                  NumToWrite: half-word (16-bit) (the number is to write 16-bit data.) Number
* Output         : None
* Return         : None
*******************************************************************************/
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	  
	u16 secoff;	  
	u16 secremain; 	   
 	u16 i;    
	u32 offaddr;  
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;
	FLASH_Unlock();					
	offaddr=WriteAddr-STM32_FLASH_BASE;		
	secpos=offaddr/STM_SECTOR_SIZE;			
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		
	secremain=STM_SECTOR_SIZE/2-secoff;		 
	if(NumToWrite<=secremain)secremain=NumToWrite;
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);
		for(i=0;i<secremain;i++)
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break; 	  
		}
		if(i<secremain)
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);
			for(i=0;i<secremain;i++)
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);				   
		if(NumToWrite==secremain)break;
		else
		{
			secpos++;				
			secoff=0;					 
		   	pBuffer+=secremain;  	
			WriteAddr+=secremain;		   
		   	NumToWrite-=secremain;	
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;
			else secremain=NumToWrite;
		}	 
	};	
	FLASH_Lock();
}
#endif



/*******************************************************************************
* Function Name  : STMFLASH_Read
* Description    : Start reading a designated length of the data from the specified address
*                  
* Input          : ReadAddr: starting address
*                  pBuffer: Data Pointer
*                  NumToWrite: half-word (16-bit) number
* Output         : None
* Return         : None
*******************************************************************************/
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);
		ReadAddr+=2;	
	}
}

/*******************************************************************************
* Function Name  : Test_Write
* Description    : None
*                  
* Input          : WriteAddr: starting address
*                  WriteData: data to be written
* Output         : None
* Return         : None
*******************************************************************************/
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1); 
}





/******************************  END OF FILE  *********************************/















