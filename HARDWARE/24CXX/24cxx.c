/*******************************************************************************
* File Name          : 24cxx.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "24cxx.h" 
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	        8191
#define AT24C128	16383
#define AT24C256	32767  

#define EE_TYPE         AT24C256

//IO��������
#define SDA_IN()  {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=8<<4;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=3<<4;}

//IO��������	 
#define IIC_SCL    PAout(8) //SCL
#define IIC_SDA    PCout(9) //SDA	 
#define READ_SDA   PCin(9)  //����SDA 

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//IIC���в�������
void IIC_Init(void);                                     //��ʼ��IIC��IO��				 
void IIC_Start(void);				          //����IIC��ʼ�ź�
void IIC_Stop(void);	  			          //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			         //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);                   //IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				 //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					 //IIC����ACK�ź�
void IIC_NAck(void);				        //IIC������ACK�ź�
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	



/*******************************************************************************
* Function Name  : AT24CXX_Init
* Description    : ��ʼ��IIC�ӿ�
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/
void AT24CXX_Init(void)
{
	IIC_Init();
}

/*******************************************************************************
* Function Name  : AT24CXX_ReadOneByte
* Description    : ��AT24CXXָ����ַ����һ������
*                  
* Input          : ReadAddr:��ʼ�����ĵ�ַ  
*                  
* Output         : None
* Return         : 
*******************************************************************************/
u8 AT24CXX_ReadOneByte(u16 ReadAddr, u8 *dat)
{				  	
        u8 err=0;
        
        IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		err = IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
//		IIC_Wait_Ack();		 
	}
        else 
        {   
            IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	 
        }
	err = IIC_Wait_Ack(); 
        IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	err = IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	err = IIC_Wait_Ack();	 
        *dat=IIC_Read_Byte(0);		   
        IIC_Stop();//����һ��ֹͣ����	  
        
	return (err);
}


/*******************************************************************************
* Function Name  : AT24CXX_WriteOneByte
* Description    : ��AT24CXXָ����ַд��һ������
*                  
* Input          : WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
*                  DataToWrite:Ҫд�������
* Output         : None
* Return         : None
*******************************************************************************/
u8 AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{		
        u8 err=0;
        
        IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		err = IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
 	}else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 
	err = IIC_Wait_Ack();	   
        IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	err = IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	err = IIC_Wait_Ack();  		    	   
        IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
        
        return (err);
}


/*******************************************************************************
* Function Name  : AT24CXX_WriteLenByte
* Description    : ��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
*                  �ú�������д��16bit����32bit������
* Input          : WriteAddr  :��ʼд��ĵ�ַ  
*                  DataToWrite:���������׵�ַ
*                  Len        :Ҫд�����ݵĳ���2,4
* Output         : None
* Return         : None
*******************************************************************************/
//void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
//{  	
//	u8 t;
//	for(t=0;t<Len;t++)
//	{
//		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
//	}												    
//}



/*******************************************************************************
* Function Name  : AT24CXX_ReadLenByte
* Description    : ��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
*                  �ú������ڶ���16bit����32bit������
* Input          : ReadAddr   :��ʼ�����ĵ�ַ 
*                  Len        :Ҫ�������ݵĳ���2,4
* Output         : None
* Return         : ����
*******************************************************************************/
//u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
//{  	
//	u8 t;
//	u32 temp=0;
//	for(t=0;t<Len;t++)
//	{
//		temp<<=8;
//		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
//	}
//	return temp;												    
//}



/*******************************************************************************
* Function Name  : AT24CXX_Check
* Description    : ���AT24CXX�Ƿ�����
*                  ��������24XX�����һ����ַ(255)���洢��־��.���������24Cϵ��,�����ַҪ�޸�
* Input          : None
*                  
* Output         : None
* Return         : 1:���ʧ��  0:���ɹ�
*******************************************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	AT24CXX_ReadOneByte(32767,&temp);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(32767,0X55);
                AT24CXX_ReadOneByte(32767,&temp);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}



/*******************************************************************************
* Function Name  : AT24CXX_Init
* Description    : ��AT24CXX�����ָ����ַ��ʼ����ָ������������
*                  
* Input          : ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
*                  NumToRead:Ҫ�������ݵĸ���
*                  pBuffer  :���������׵�ַ
* Output         : None
* Return         : None
*******************************************************************************/
u8 AT24CXX_Read(u16 ReadAddr,u16 NumToRead,u8 *pBuffer)
{
        u8 ucCounter=0,err=0;
        
        while(ucCounter<3)
	{
            
            while(NumToRead)
            {
		err = AT24CXX_ReadOneByte(ReadAddr++, pBuffer++);
                if(err) break;
		NumToRead--;
            }
            if(!err) break;
            
            ucCounter++;
            delay_ms( 50 );            
            
        }
        return(err);
}  



/*******************************************************************************
* Function Name  : AT24CXX_Write
* Description    : ��AT24CXX�����ָ����ַ��ʼд��ָ������������
*                  
* Input          : WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
*                  NumToWrite:Ҫд�����ݵĸ���
*                  pBuffer   :���������׵�ַ     
* Output         : None
* Return         : None
*******************************************************************************/
u8 AT24CXX_Write(u16 WriteAddr,u16 NumToWrite,u8 *pBuffer)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
        u8 ucCounter=0,err=0;
        
        while(ucCounter<3)
	{
            
            while(NumToWrite--)
            {
		err = AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
                WriteAddr++;
		pBuffer++;
                if(err) break;
            }
            if(!err) break;
            
            ucCounter++;
            delay_ms( 50 );            
            
        }
        return(err);        
        
}
 



//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	IIC_SCL=1;
	IIC_SDA=1;

}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}






/******************************  END OF FILE  *********************************/






