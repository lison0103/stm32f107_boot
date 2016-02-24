
#include "FreeRTOS.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

#include "mb.h"


#define MODBUS_THREAD_PRIO    ( tskIDLE_PRIORITY + 3 )

extern char TmpBuf[256];
extern unsigned short mbFramBuflen;
extern int newconn;

void modbus_socket_thread(void *arg)
{
  int sock, size,ret,opt,i;
  struct sockaddr_in address, remotehost;

 /* create a TCP socket */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  {
    return;
  }
  
  /* bind to port 80 at any interface */
  address.sin_family = AF_INET;
  address.sin_port = htons(502);
  address.sin_addr.s_addr = INADDR_ANY;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  bind(sock, (struct sockaddr *)&address, sizeof (address));
  
  /* listen for incoming connections (TCP listen backlog = 5) */
  listen(sock, 1);
  
  size = sizeof(remotehost);
  eMBTCPInit(502);
  eMBEnable();
  while (1) 
  {
    newconn = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);
    while(1)
	{
		  /* Read in the request */
	  	ret = recv(newconn, TmpBuf, sizeof(TmpBuf),0); 
	  	if(ret == 0)	  //断开连接
		{
			close(newconn);
			break;	  //跳出，去等待连接
		}
		else if(ret>0)
		{
			mbFramBuflen  = ret;
			( void )xMBPortEventPost( EV_FRAME_RECEIVED );  //告诉Mbpoll有新的数据帧
		   	for(i=0;i<2;i++)
			{
				eMBPoll();
			}	
		}
		else		 //无连接存在
		{
			break;
		}
	 }
  }		
}

void modbus_socket_init(void)
{
	sys_thread_new("Modbus", modbus_socket_thread, NULL, DEFAULT_THREAD_STACKSIZE * 2, MODBUS_THREAD_PRIO);	
}