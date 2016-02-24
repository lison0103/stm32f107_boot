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
	int s;						/*socket ��ʶ�� -1��Ч��>= 0 ��Ч*/
	int bconnect;				/*socket �Ƿ����ӳɹ���0 δ���ӣ�>= 1 ����*/
	xSemaphoreHandle	sxMutex;/*��������socket�����̰߳�ȫ�ģ�Ϊ��ʵ��socket
									��һ���߳���������ݣ����������̷߳��ͣ�
									���������������������*/
}XTCPCLIENTSOCK;


#define BUF_SIZE	1024
#define MYPORT 8088    // the port users will be connecting to  
#define BACKLOG 3     // how many pending connections queue will hold  


void led0_task(void *arg);
void led1_task(void *arg);
void TCPServer(void *arg);
void TCPClient(void *arg);

#endif