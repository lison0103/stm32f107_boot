/*******************************************************************************
* File Name          : 24cxx.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains prototype of 24cxx driver functions.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __24CXX_H
#define __24CXX_H

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 AT24CXX_ReadOneByte(u16 ReadAddr, u8 *dat, u16 len);					
u8 AT24CXX_WriteOneByte(u16 WriteAddr,u8 *DataToWrite,u8 len);				
u8 AT24CXX_Write(u16 WriteAddr,u16 NumToWrite,u8 *pBuffer);	
u8 AT24CXX_Read(u16 ReadAddr,u16 NumToRead,u8 *pBuffer);   	
u8 AT24CXX_Check(void);                                         
void AT24CXX_Init(void);                                       


#endif /* __24CXX_H */


/******************************  END OF FILE  *********************************/



   


					  


















