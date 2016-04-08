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

//IO方向设置
#define SDA_IN()  {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=8<<4;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=3<<4;}

//IO操作函数	 
#define IIC_SCL    PAout(8) //SCL
#define IIC_SDA    PCout(9) //SDA	 
#define READ_SDA   PCin(9)  //输入SDA 

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//IIC所有操作函数
void IIC_Init(void);                                     //初始化IIC的IO口				 
void IIC_Start(void);				          //发送IIC开始信号
void IIC_Stop(void);	  			          //发送IIC停止信号
void IIC_Send_Byte(u8 txd);			         //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);                   //IIC读取一个字节
u8 IIC_Wait_Ack(void); 				 //IIC等待ACK信号
void IIC_Ack(void);					 //IIC发送ACK信号
void IIC_NAck(void);				        //IIC不发送ACK信号
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	



/*******************************************************************************
* Function Name  : AT24CXX_Init
* Description    : 初始化IIC接口
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
* Description    : 在AT24CXX指定地址读出一个数据
*                  
* Input          : ReadAddr:开始读数的地址  
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
		IIC_Send_Byte(0XA0);	   //发送写命令
		err = IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址
//		IIC_Wait_Ack();		 
	}
        else 
        {   
            IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
        }
	err = IIC_Wait_Ack(); 
        IIC_Send_Byte(ReadAddr%256);   //发送低地址
	err = IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	err = IIC_Wait_Ack();	 
        *dat=IIC_Read_Byte(0);		   
        IIC_Stop();//产生一个停止条件	  
        
	return (err);
}


/*******************************************************************************
* Function Name  : AT24CXX_WriteOneByte
* Description    : 在AT24CXX指定地址写入一个数据
*                  
* Input          : WriteAddr  :写入数据的目的地址
*                  DataToWrite:要写入的数据
* Output         : None
* Return         : None
*******************************************************************************/
u8 AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{		
        u8 err=0;
        
        IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		err = IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
 	}else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}	 
	err = IIC_Wait_Ack();	   
        IIC_Send_Byte(WriteAddr%256);   //发送低地址
	err = IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	err = IIC_Wait_Ack();  		    	   
        IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
        
        return (err);
}


/*******************************************************************************
* Function Name  : AT24CXX_WriteLenByte
* Description    : 在AT24CXX里面的指定地址开始写入长度为Len的数据
*                  该函数用于写入16bit或者32bit的数据
* Input          : WriteAddr  :开始写入的地址  
*                  DataToWrite:数据数组首地址
*                  Len        :要写入数据的长度2,4
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
* Description    : 在AT24CXX里面的指定地址开始读出长度为Len的数据
*                  该函数用于读出16bit或者32bit的数据
* Input          : ReadAddr   :开始读出的地址 
*                  Len        :要读出数据的长度2,4
* Output         : None
* Return         : 数据
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
* Description    : 检查AT24CXX是否正常
*                  这里用了24XX的最后一个地址(255)来存储标志字.如果用其他24C系列,这个地址要修改
* Input          : None
*                  
* Output         : None
* Return         : 1:检测失败  0:检测成功
*******************************************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	AT24CXX_ReadOneByte(32767,&temp);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(32767,0X55);
                AT24CXX_ReadOneByte(32767,&temp);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}



/*******************************************************************************
* Function Name  : AT24CXX_Init
* Description    : 在AT24CXX里面的指定地址开始读出指定个数的数据
*                  
* Input          : ReadAddr :开始读出的地址 对24c02为0~255
*                  NumToRead:要读出数据的个数
*                  pBuffer  :数据数组首地址
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
* Description    : 在AT24CXX里面的指定地址开始写入指定个数的数据
*                  
* Input          : WriteAddr :开始写入的地址 对24c02为0~255
*                  NumToWrite:要写入数据的个数
*                  pBuffer   :数据数组首地址     
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
 



//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	IIC_SCL=1;
	IIC_SDA=1;

}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}






/******************************  END OF FILE  *********************************/






