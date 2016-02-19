#ifndef __FREERTOS_LWIP_H
#define __FREERTOS_LWIP_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32_eth_bsp.h"
#include "netconf.h"
#include "lwip/sockets.h"

#define LED0_TASK_PRIO			( tskIDLE_PRIORITY + 1 )
#define LED1_TASK_PRIO			( tskIDLE_PRIORITY + 2 )
#define TCP_TASK_PRIO			( tskIDLE_PRIORITY + 3 )

#define configTCP_STACK_SIZE	        ( ( unsigned short ) 1024 )

void led0_task(void *pvParam);
void led1_task(void *pvParam);
void TCPClient(void *arg);

#endif