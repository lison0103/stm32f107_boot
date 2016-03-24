/*******************************************************************************
* File Name          : led.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "led.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : LED_Init
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void LED_Init(void)
{

     GPIO_InitTypeDef  GPIO_InitStructure;            
            
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //LED1-->PB.15 端口配置
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
     GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.15
     GPIO_SetBits(GPIOB,GPIO_Pin_15);						 //PB.15 输出高

     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //LED0-->PD.11 端口配置, 推挽输出
     GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
     GPIO_SetBits(GPIOD,GPIO_Pin_11); 
}

/******************************  END OF FILE  *********************************/




 
