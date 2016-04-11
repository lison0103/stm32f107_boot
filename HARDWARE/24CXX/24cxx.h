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
u8 AT24CXX_ReadOneByte(u16 ReadAddr, u8 *dat);					//ָ����ַ��ȡһ���ֽ�
u8 AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
//void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
//u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
u8 AT24CXX_Write(u16 WriteAddr,u16 NumToWrite,u8 *pBuffer);	//��ָ����ַ��ʼд��ָ�����ȵ�����
u8 AT24CXX_Read(u16 ReadAddr,u16 NumToRead,u8 *pBuffer);   	//��ָ����ַ��ʼ����ָ�����ȵ�����
u8 AT24CXX_Check(void);                                         //�������
void AT24CXX_Init(void);                                        //��ʼ��IIC


#endif /* __24CXX_H */


/******************************  END OF FILE  *********************************/



   


					  


















