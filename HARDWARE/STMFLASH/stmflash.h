/*******************************************************************************
* File Name          : stmflash.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains prototype of stm flash functions.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STMFLASH_H
#define __STMFLASH_H

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u16 STMFLASH_ReadHalfWord(u32 faddr);		                        
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);				
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		
void Test_Write(u32 WriteAddr,u16 WriteData);                         


#endif /* __STMFLASH_H */


/******************************  END OF FILE  *********************************/




















