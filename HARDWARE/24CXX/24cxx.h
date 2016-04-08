/*******************************************************************************
* File Name          : 24cxx.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
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
u8 AT24CXX_ReadOneByte(u16 ReadAddr, u8 *dat);					//指定地址读取一个字节
u8 AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
//void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
//u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
u8 AT24CXX_Write(u16 WriteAddr,u16 NumToWrite,u8 *pBuffer);	//从指定地址开始写入指定长度的数据
u8 AT24CXX_Read(u16 ReadAddr,u16 NumToRead,u8 *pBuffer);   	//从指定地址开始读出指定长度的数据
u8 AT24CXX_Check(void);                                         //检查器件
void AT24CXX_Init(void);                                        //初始化IIC


#endif /* __24CXX_H */


/******************************  END OF FILE  *********************************/



   


					  


















