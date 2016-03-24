/*******************************************************************************
* File Name          : malloc.h
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MALLOC_H
#define __MALLOC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(void);				//��ʼ��
	u8 (*perused)(void);		  	//�ڴ�ʹ����
	u8 	*membase;					//�ڴ�� 
	u16 *memmap; 					//�ڴ����״̬��
	u8  memrdy; 					//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	//��mallco.c���涨��

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifndef NULL
#define NULL 0
#endif

/* Exported functions ------------------------------------------------------- */
void mymemset(void *s,u8 c,u32 count);	//�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
void mmem_init(void);					 //�ڴ�����ʼ������(��/�ڲ�����)
u32 mmem_malloc(u32 size);		 		//�ڴ����(�ڲ�����)
u8 mmem_free(u32 offset);		 		//�ڴ��ͷ�(�ڲ�����)
u8 mem_perused(void);					//���ڴ�ʹ����(��/�ڲ�����)
//�û����ú���
void myfree(void *ptr);  				//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u32 size);				//�ڴ����(�ⲿ����)
void *myrealloc(void *ptr,u32 size);	//���·����ڴ�(�ⲿ����)



#endif /* __IAP_H */


/******************************  END OF FILE  *********************************/

















