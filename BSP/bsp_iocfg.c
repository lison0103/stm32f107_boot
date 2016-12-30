/*******************************************************************************
* File Name          : bsp_iocfg.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : Contains input and output pin Initializes.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "bsp_iocfg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : Get_GpioInput
* Description    : Get GPIO input pin value.
*                  
* Input          : inBuff: read the gpio pin value of data
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void Get_GpioInput(u8 inBuff[])
{     
    static u16 PinValue[3][5],ByteAnd[5],ByteOr[5],read_pin_cnt = 0;
    u16 i;     
    
    read_pin_cnt++;  
    if(read_pin_cnt > 2) read_pin_cnt = 0;  
    
    /* Read I/O port */                      
    PinValue[read_pin_cnt][0] = GPIO_ReadInputData(GPIOA);
    PinValue[read_pin_cnt][1] = GPIO_ReadInputData(GPIOB);
    PinValue[read_pin_cnt][2] = GPIO_ReadInputData(GPIOC);
    PinValue[read_pin_cnt][3] = GPIO_ReadInputData(GPIOD);
    PinValue[read_pin_cnt][4] = GPIO_ReadInputData(GPIOE);
    
    for( i = 0; i < 5; i++ )
    {   
        ByteAnd[i] = PinValue[0][i] & PinValue[1][i] & PinValue[2][i];
        ByteOr[i] = PinValue[0][i] | PinValue[1][i] | PinValue[2][i]; 
    }
    

    
    /* IN1 ~ IN8 */
    if(ByteAnd[0] & 0x0008) inBuff[0] |= 0x01; else if(!(ByteOr[0] & 0x0008)) inBuff[0] &= ~0x01; 
    if(ByteAnd[0] & 0x0010) inBuff[0] |= 0x02; else if(!(ByteOr[0] & 0x0010)) inBuff[0] &= ~0x02; 
    if(ByteAnd[2] & 0x0010) inBuff[0] |= 0x04; else if(!(ByteOr[2] & 0x0010)) inBuff[0] &= ~0x04; 
    if(ByteAnd[2] & 0x0020) inBuff[0] |= 0x08; else if(!(ByteOr[2] & 0x0020)) inBuff[0] &= ~0x08; 
    if(ByteAnd[1] & 0x0001) inBuff[0] |= 0x10; else if(!(ByteOr[1] & 0x0001)) inBuff[0] &= ~0x10; 
    if(ByteAnd[1] & 0x0002) inBuff[0] |= 0x20; else if(!(ByteOr[1] & 0x0002)) inBuff[0] &= ~0x20; 
    if(ByteAnd[1] & 0x0004) inBuff[0] |= 0x40; else if(!(ByteOr[1] & 0x0004)) inBuff[0] &= ~0x40; 
    if(ByteAnd[4] & 0x0080) inBuff[0] |= 0x80; else if(!(ByteOr[4] & 0x0080)) inBuff[0] &= ~0x80; 
    
    /* IN9 ~ IN16 */
    if(ByteAnd[4] & 0x0100) inBuff[1] |= 0x01; else if(!(ByteOr[4] & 0x0100)) inBuff[1] &= ~0x01; 
    if(ByteAnd[4] & 0x0200) inBuff[1] |= 0x02; else if(!(ByteOr[4] & 0x0200)) inBuff[1] &= ~0x02; 
    if(ByteAnd[4] & 0x0400) inBuff[1] |= 0x04; else if(!(ByteOr[4] & 0x0400)) inBuff[1] &= ~0x04; 
    if(ByteAnd[4] & 0x0800) inBuff[1] |= 0x08; else if(!(ByteOr[4] & 0x0800)) inBuff[1] &= ~0x08; 
    if(ByteAnd[4] & 0x1000) inBuff[1] |= 0x10; else if(!(ByteOr[4] & 0x1000)) inBuff[1] &= ~0x10; 
    if(ByteAnd[4] & 0x2000) inBuff[1] |= 0x20; else if(!(ByteOr[4] & 0x2000)) inBuff[1] &= ~0x20; 
    if(ByteAnd[4] & 0x4000) inBuff[1] |= 0x40; else if(!(ByteOr[4] & 0x4000)) inBuff[1] &= ~0x40; 
    if(ByteAnd[4] & 0x8000) inBuff[1] |= 0x80; else if(!(ByteOr[4] & 0x8000)) inBuff[1] &= ~0x80; 
    
}


/*******************************************************************************
* Function Name  : output_driver
* Description    : Enable or disable relay output.
*                  
* Input          : out_buff: set the gpio pin value of data
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void output_driver(u8 out_buff[])
{
	if(out_buff[0] & 0x01) 		GRL1 = 1; 		else GRL1 = 0; 					  
	if(out_buff[0] & 0x02) 		GRL2 = 1; 		else GRL2 = 0; 	 					  
	if(out_buff[0] & 0x04) 		GRL3 = 1; 		else GRL3 = 0;	 					  
	if(out_buff[0] & 0x08)  	GRL4 = 1; 		else GRL4 = 0; 						  
	if(out_buff[0] & 0x10)  	GRL5 = 1; 		else GRL5 = 0; 					  
	if(out_buff[0] & 0x20)  	GRL6 = 1; 		else GRL6 = 0;				  
	if(out_buff[0] & 0x40)  	GRL7 = 1; 		else GRL7 = 0;				  
	if(out_buff[0] & 0x80)		GRL8 = 1; 		else GRL8 = 0; 	

	if(out_buff[1] & 0x01) 	        GRL9 = 1; 		else GRL9 = 0; 	
	if(out_buff[1] & 0x02) 		UP_RELAY_ON(); 		else UP_RELAY_OFF(); 	 					  
	if(out_buff[1] & 0x04) 		DOWN_RELAY_ON(); 	else DOWN_RELAY_OFF();	 					  
	if(out_buff[1] & 0x08)  	Y_RELAY_ON(); 		else Y_RELAY_OFF(); 						  
	if(out_buff[1] & 0x10)  	D_RELAY_ON(); 		else D_RELAY_OFF(); 					  
	if(out_buff[1] & 0x20)  	TRANS_CTRL1 = 1; 	else TRANS_CTRL1 = 0;				  
	if(out_buff[1] & 0x40)  	TRANS_CTRL2 = 1; 	else TRANS_CTRL2 = 0;	 
}

/*******************************************************************************
* Function Name  : Input_Output_PinInit
* Description    : Initializes the Input and Output GPIO
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void Input_Output_PinInit(void)
{
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  
  
      GPIO_InitTypeDef GPIO_InitStruct;
      
      /** input gpio ----------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; 
      
      /* IN1 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;                   
      GPIO_Init(GPIOA , &GPIO_InitStruct);    

      /* IN2 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;                   
      GPIO_Init(GPIOA , &GPIO_InitStruct);  

      /* IN3 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;                   
      GPIO_Init(GPIOC , &GPIO_InitStruct);  

      /* IN4 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;                   
      GPIO_Init(GPIOC , &GPIO_InitStruct);  

      /* IN5 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;                   
      GPIO_Init(GPIOB , &GPIO_InitStruct);  

      /* IN6 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;                   
      GPIO_Init(GPIOB , &GPIO_InitStruct);  

      /* IN7 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;                   
      GPIO_Init(GPIOB , &GPIO_InitStruct);  

      /* IN8 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;                   
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      /* IN9 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;                   
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      /* IN10 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;                   
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      /* IN11 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;                   
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      /* IN12 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;                   
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      /* IN13 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;                   
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      /* IN14 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;                   
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      /* IN15 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;                   
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      /* IN16 */      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;                   
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      
      
      /** output gpio ---------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
      
      /* GRL1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
      GPIO_ResetBits(GPIOE, GPIO_Pin_2);
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

      /* GRL2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_ResetBits(GPIOE, GPIO_Pin_3);
      GPIO_Init(GPIOE , &GPIO_InitStruct); 

      /* GRL3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_ResetBits(GPIOE, GPIO_Pin_4);
      GPIO_Init(GPIOE , &GPIO_InitStruct); 

      /* GRL4 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
      GPIO_ResetBits(GPIOE, GPIO_Pin_5);
      GPIO_Init(GPIOE , &GPIO_InitStruct); 

      /* GRL5 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
      GPIO_ResetBits(GPIOE, GPIO_Pin_6);
      GPIO_Init(GPIOE , &GPIO_InitStruct); 

      /* GRL6 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
      GPIO_ResetBits(GPIOB, GPIO_Pin_8);
      GPIO_Init(GPIOB , &GPIO_InitStruct); 

      /* GRL7 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
      GPIO_ResetBits(GPIOB, GPIO_Pin_7);
      GPIO_Init(GPIOB , &GPIO_InitStruct); 

      /* GRL8 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_ResetBits(GPIOB, GPIO_Pin_4);
      GPIO_Init(GPIOB , &GPIO_InitStruct); 

      /* GRL9 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_ResetBits(GPIOB, GPIO_Pin_3);
      GPIO_Init(GPIOB , &GPIO_InitStruct); 

      /* GSFR1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      GPIO_Init(GPIOD , &GPIO_InitStruct); 

      /* GSFR2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_ResetBits(GPIOD, GPIO_Pin_4);
      GPIO_Init(GPIOD , &GPIO_InitStruct);       

      /* GSFR3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_ResetBits(GPIOD, GPIO_Pin_3);
      GPIO_Init(GPIOD , &GPIO_InitStruct); 

      /* GSFR4 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
      GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      GPIO_Init(GPIOD , &GPIO_InitStruct);
      
      /* TRANS_CTRL1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
      GPIO_ResetBits(GPIOA, GPIO_Pin_15);
      GPIO_Init(GPIOA , &GPIO_InitStruct); 

      /* TRANS_CTRL2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
      GPIO_ResetBits(GPIOC, GPIO_Pin_12);
      GPIO_Init(GPIOC , &GPIO_InitStruct);

      
}

/******************************  END OF FILE  *********************************/


