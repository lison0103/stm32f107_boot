#include "led.h"



void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PB,PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //LED1-->PB.15 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.15
 GPIO_SetBits(GPIOB,GPIO_Pin_15);						 //PB.15 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    		 //LED0-->PD.11 �˿�����, �������
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOD,GPIO_Pin_11); 						 //PD.11 ����� 
}

 
