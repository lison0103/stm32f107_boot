/*******************************************************************************
* File Name          : stmflash.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stmflash.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 256 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1                      //ʹ��FLASHд��(0��������;1��ʹ��)

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	        //STM32 FLASH����ʼ��ַ

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/*******************************************************************************
* Function Name  : STMFLASH_ReadHalfWord
* Description    : ��ȡָ����ַ�İ���(16λ����)
*                  
* Input          : faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
*                  
* Output         : None
* Return         : None
*******************************************************************************/
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}


#if STM32_FLASH_WREN	//���ʹ����д   

/*******************************************************************************
* Function Name  : STMFLASH_Write_NoCheck
* Description    : ������д��
*                  
* Input          : WriteAddr:��ʼ��ַ
*                  pBuffer:����ָ��
*                  NumToWrite:����(16λ)��   
* Output         : None
* Return         : None
*******************************************************************************/ 
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 


/*******************************************************************************
* Function Name  : STMFLASH_Write
* Description    : ��ָ����ַ��ʼд��ָ�����ȵ�����
*                  
* Input          : WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
*                  pBuffer:����ָ��
*                  NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
* Output         : None
* Return         : None
*******************************************************************************/
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain;  //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();					//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
#endif



/*******************************************************************************
* Function Name  : STMFLASH_Read
* Description    : ��ָ����ַ��ʼ����ָ�����ȵ�����
*                  
* Input          : ReadAddr:��ʼ��ַ
*                  pBuffer:����ָ��
*                  NumToWrite:����(16λ)��
* Output         : None
* Return         : None
*******************************************************************************/
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

/*******************************************************************************
* Function Name  : Test_Write
* Description    : None
*                  
* Input          : WriteAddr:��ʼ��ַ
*                  WriteData:Ҫд�������
* Output         : None
* Return         : None
*******************************************************************************/
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}





/******************************  END OF FILE  *********************************/















