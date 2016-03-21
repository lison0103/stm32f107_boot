#ifndef __INITIAL_DEVICES_H
#define __INITIAL_DEVICES_H

#include "lsys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"  
#include "malloc.h" 
#include "ff.h"  
#include "exfuns.h"     
#include "usbh_usr.h" 
#include "24cxx.h"
#include "mb85rcxx.h"
#include "digital_led.h"
#include "ewdt.h"
#include "update.h"
#include "iap.h"
#include "rtc.h"

extern USBH_HOST  USB_Host;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;

void Bsp_Init(void);

#ifdef GEC_CB_MAIN

#include "hw_test.h"
#include "freertos_lwip.h" 
#include "modbusTask.h"
#include "self_test.h"

#define LED_TASK_PRIO			( tskIDLE_PRIORITY + 1 )
#define TCP_TASK_PRIO			( tskIDLE_PRIORITY + 3 )
#define DHCP_TASK_PRIO                  ( tskIDLE_PRIORITY + 4 ) 

#else


#endif



#endif