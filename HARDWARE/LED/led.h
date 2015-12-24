#ifndef _LED_H
#define _LED_H
#include "sys.h"


//LED端口定义
#define LED0 PDout(11)	// PD11
#define LED1 PBout(15)	// PB15	

void LED_Init(void); //初始化
 
#endif
