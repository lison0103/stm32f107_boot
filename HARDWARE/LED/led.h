#ifndef _LED_H
#define _LED_H
#include "sys.h"


//LED�˿ڶ���
#define LED0 PDout(11)	// PD11
#define LED1 PBout(15)	// PB15	

void LED_Init(void); //��ʼ��
 
#endif
