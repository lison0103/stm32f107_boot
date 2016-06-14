/*******************************************************************************
* File Name          : 24cxx.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains at24cxx driver functions.
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


#define SDA_IN()  {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=8<<4;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFFFF0F;GPIOC->CRH|=3<<4;}


#define IIC_SCL    PAout(8) 
#define IIC_SDA    PCout(9) 
#define READ_SDA   PCin(9)  

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* IIC all operating functions */
void IIC_Init(void);                                     				 
void IIC_Start(void);				        
void IIC_Stop(void);	  			        
void IIC_Send_Byte(u8 txd);			      
u8 IIC_Read_Byte(unsigned char ack);                 
u8 IIC_Wait_Ack(void); 				
void IIC_Ack(void);					
void IIC_NAck(void);				        
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	



/*******************************************************************************
* Function Name  : AT24CXX_Init
* Description    : Initialize AT24CXX IIC.
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
* Description    : a read-out data in the specified address AT24CXX
*                  
* Input          : ReadAddr: Start reading address  
*                  
* Output         : None
* Return         : 0: success  1: error
*******************************************************************************/
u8 AT24CXX_ReadOneByte(u16 ReadAddr, u8 *dat)
{				  	
    u8 err=0;
    
    IIC_Start();  
    if(EE_TYPE>AT24C16)
    {
        IIC_Send_Byte(0XA0);	 
        err = IIC_Wait_Ack();
        IIC_Send_Byte(ReadAddr>>8);	 
    }
    else 
    {   
        IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   	 
    }
    err = IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   
    err = IIC_Wait_Ack();	    
    IIC_Start();  	 	   
    IIC_Send_Byte(0XA1);          			   
    err = IIC_Wait_Ack();	 
    *dat=IIC_Read_Byte(0);		   
    IIC_Stop();  
    
    return (err);
}


/*******************************************************************************
* Function Name  : AT24CXX_WriteOneByte
* Description    : Data written to a specified address in AT24CXX
*                  
* Input          : WriteAddr : writing data to the destination address
*                  DataToWrite: data to be written
* Output         : None
* Return         : 0: success  1: error
*******************************************************************************/
u8 AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{		
    u8 err=0;
    
    IIC_Start();  
    if(EE_TYPE>AT24C16)
    {
        IIC_Send_Byte(0XA0);	    
        err = IIC_Wait_Ack();
        IIC_Send_Byte(WriteAddr>>8);
    }
    else
    {
        IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));  
    }	 
    err = IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   
    err = IIC_Wait_Ack(); 	 										  		   
    IIC_Send_Byte(DataToWrite);    							   
    err = IIC_Wait_Ack();  		    	   
    IIC_Stop(); 
    delay_ms(10);	 
    
    return (err);
}



/*******************************************************************************
* Function Name  : AT24CXX_Check
* Description    : Check whether normal AT24CXX
*                  Here with 24XX last word address to store the flag. If other 24C series, the address to be modified
* Input          : None
*                  
* Output         : None
* Return         : 0: success  1: error
*******************************************************************************/
u8 AT24CXX_Check(void)
{
    u8 temp;
    AT24CXX_ReadOneByte(32767,&temp);		   
    if(temp==0X55)return 0;		   
    else
    {
        AT24CXX_WriteOneByte(32767,0X55);
        AT24CXX_ReadOneByte(32767,&temp);	  
        if(temp==0X55)return 0;
    }
    return 1;											  
}



/*******************************************************************************
* Function Name  : AT24CXX_Init
* Description    : In the specified address AT24CXX inside starts reading a specified number of data
*                  
* Input          : ReadAddr : Start address read out from 0 to 32767 for 24c256
*                  NumToRead: the number of data to be read
*                  pBuffer  : the first address of the data array
* Output         : None
* Return         : 0: success  1: error
*******************************************************************************/
u8 AT24CXX_Read(u16 ReadAddr,u16 NumToRead,u8 *pBuffer)
{
    u8 ucCounter=0,err=0;
    
    while(ucCounter < 3)
    {
        
        while(NumToRead)
        {
            err = AT24CXX_ReadOneByte(ReadAddr, pBuffer);
            if(err) break;
            ReadAddr++;
            pBuffer++;
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
* Description    : In the specified address AT24CXX inside to start writing the specified number of data
*                  
* Input          : WriteAddr : start address written on 24c256 from 0 to 32767
*                  NumToWrite: the number of data to be written
*                  pBuffer   : the first address of the data array  
* Output         : None
* Return         : 0: success  1: error
*******************************************************************************/
u8 AT24CXX_Write(u16 WriteAddr,u16 NumToWrite,u8 *pBuffer)
{
    u8 ucCounter=0,err=0;
    
    while(ucCounter < 3)
    {
        
        while(NumToWrite)
        {
            err = AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
            if(err) break;
            WriteAddr++;
            pBuffer++;
            NumToWrite--;
        }
        if(!err) break;
        
        ucCounter++;
        delay_ms( 50 );            
        
    }
    return(err);        
    
}





/*******************************************************************************
* Function Name  : IIC_Init
* Description    : Initialize simulate IIC.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/
void IIC_Init(void)
{					     
    GPIO_InitTypeDef GPIO_InitStructure;	
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    IIC_SCL=1;
    IIC_SDA=1;
    
}

/*******************************************************************************
* Function Name  : IIC_Start
* Description    : Start signal generating IIC.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/
void IIC_Start(void)
{
    SDA_OUT();     
    IIC_SDA=1;	  	  
    IIC_SCL=1;
    delay_us(4);
    IIC_SDA=0;
    delay_us(4);
    IIC_SCL=0;
}	  


/*******************************************************************************
* Function Name  : IIC_Stop
* Description    : Stop signal generating IIC.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/
void IIC_Stop(void)
{
    SDA_OUT();
    IIC_SCL=0;
    IIC_SDA=0;
    delay_us(4);
    IIC_SCL=1; 
    IIC_SDA=1;
    delay_us(4);							   	
}


/*******************************************************************************
* Function Name  : IIC_Wait_Ack
* Description    : Waiting for a response signal.
*                  
* Input          : None
*                  
* Output         : None
* Return         : 0: success  1: error
*******************************************************************************/
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    
    SDA_IN();       
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
    IIC_SCL=0;   
    return 0;  
} 


/*******************************************************************************
* Function Name  : IIC_Ack
* Description    : An ACK.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/
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

/*******************************************************************************
* Function Name  : IIC_NAck
* Description    : No ACK.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/
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


/*******************************************************************************
* Function Name  : IIC_Send_Byte
* Description    : IIC send a byte.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
    
    SDA_OUT(); 	    
    IIC_SCL=0;
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
        delay_us(2);  
        IIC_SCL=1;
        delay_us(2); 
        IIC_SCL=0;	
        delay_us(2);
    }	 
} 	    

/*******************************************************************************
* Function Name  : IIC_Send_Byte
* Description    : IIC read a byte.
*                  
* Input          : ack: ack = 1, transmits ACK, ack = 0, transmit nACK
*                  
* Output         : None
* Return         : Read data
*******************************************************************************/
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    
    SDA_IN();
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
        IIC_NAck();
    else
        IIC_Ack();  
    return receive;
}






/******************************  END OF FILE  *********************************/






