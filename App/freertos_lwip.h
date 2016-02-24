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

typedef struct _XTCPCLIENTSOCK{
	int s;						/*socket 标识符 -1无效，>= 0 有效*/
	int bconnect;				/*socket 是否连接成功，0 未连接，>= 1 连接*/
	xSemaphoreHandle	sxMutex;/*互斥量，socket不是线程安全的，为了实现socket
									在一个线程里接收数据，而在其他线程发送，
									建立互斥量来做互斥操作*/
}XTCPCLIENTSOCK;


#define BUF_SIZE	1024
#define MYPORT 8088    // the port users will be connecting to  
#define BACKLOG 3     // how many pending connections queue will hold  


void led0_task(void *arg);
void led1_task(void *arg);
void TCPServer(void *arg);
void TCPClient(void *arg);

#endif