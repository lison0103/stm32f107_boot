#include "bsp_iocfg.h"


void Input_Output_PinInit(void)
{
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE );
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  
  
      GPIO_InitTypeDef GPIO_InitStruct;

      //input
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;             
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;       
      GPIO_Init(GPIOA , &GPIO_InitStruct);    
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;                  
      GPIO_Init(GPIOB , &GPIO_InitStruct);        

      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;                  
      GPIO_Init(GPIOC , &GPIO_InitStruct);   

      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);   
      
      
      //output
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15; 
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_ResetBits(GPIOA, GPIO_Pin_15);
      GPIO_Init(GPIOA , &GPIO_InitStruct);  
      
            
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_8; 
      GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_8);
      GPIO_Init(GPIOB , &GPIO_InitStruct);  
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12; 
      GPIO_ResetBits(GPIOC, GPIO_Pin_12);
      GPIO_Init(GPIOC , &GPIO_InitStruct);        
  
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7; 
      GPIO_ResetBits(GPIOD, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
      GPIO_Init(GPIOD , &GPIO_InitStruct);  
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; 
      GPIO_ResetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

}