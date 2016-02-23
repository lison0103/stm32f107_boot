#include "freertos_lwip.h" 
#include "led.h"  

xSemaphoreHandle gTouchxSem;



void led0_task(void *pvParam)
{
        LED0 = 0;
        
	for( ; ; )
	{
                LED0 =!LED0;
		vTaskDelay( 500 );
	}
}
void led1_task(void *pvParam)
{
        LED1 = 1;
        
	for( ; ; )
	{
                LED1 =!LED1;
		vTaskDelay( 500 );
	}
}

typedef struct _XTCPCLIENTSOCK{
	int s;						/*socket ��ʶ�� -1��Ч��>= 0 ��Ч*/
	int bconnect;				/*socket �Ƿ����ӳɹ���0 δ���ӣ�>= 1 ����*/
	xSemaphoreHandle	sxMutex;/*��������socket�����̰߳�ȫ�ģ�Ϊ��ʵ��socket
									��һ���߳���������ݣ����������̷߳��ͣ�
									���������������������*/
}XTCPCLIENTSOCK;

XTCPCLIENTSOCK xSeverSocket;

#define BUF_SIZE		1024

#if 1

#define MYPORT 8088    // the port users will be connecting to  
#define BACKLOG 5     // how many pending connections queue will hold  
int fd_A[BACKLOG];    // accepted connection fd  
int conn_amount;      // current connection amount 


void TCPClient(void *arg)
{
      int sock_fd, new_fd;             // listen on sock_fd, new connection on new_fd  
      struct sockaddr_in server_addr;  // server address information  
      struct sockaddr_in client_addr;  // connector's address information  
      socklen_t sin_size;  
      int yes = 1;  
      char buf[BUF_SIZE];  
      int ret;  
      int i;  
      
      if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  

      }  
      
      if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {  

      }  
      server_addr.sin_family = AF_INET;         // host byte order  
      server_addr.sin_port = htons(MYPORT);     // short, network byte order  
      server_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP  
      memset(server_addr.sin_zero, '/0', sizeof(server_addr.sin_zero));  
      
      
      if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {  

      }  
      
      
      if (listen(sock_fd, BACKLOG) == -1) {  
 
      }  
      
  
  
      fd_set fdsr;  
      int maxsock;  
      struct timeval tv;  
      conn_amount = 0;  
      sin_size = sizeof(client_addr);  
      maxsock = sock_fd;  
      
      
      while (1)   
      {  
        // initialize file descriptor set  
        FD_ZERO(&fdsr);  
        FD_SET(sock_fd, &fdsr);  // add fd  
        
        
        // timeout setting  
        tv.tv_sec = 10;  
        tv.tv_usec = 0;  
        
        
        // add active connection to fd set  
        for (i = 0; i < BACKLOG; i++) {  
          if (fd_A[i] != 0) {  
            FD_SET(fd_A[i], &fdsr);  
          }  
        }  
        
        
        ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);  
        if (ret < 0) {          // error   
          break;  
        } else if (ret == 0) {  // time out  
//          printf("timeout/n");  
          continue;  
        }  
        
        
        // check every fd in the set  
        for (i = 0; i < conn_amount; i++)   
        {  
          if (FD_ISSET(fd_A[i], &fdsr)) // check which fd is ready  
          {  
            ret = recv(fd_A[i], buf, sizeof(buf), 0);  
            if (ret <= 0)   
            {        // client close  
//              printf("ret : %d and client[%d] close/n", ret, i);  
              close(fd_A[i]);  
              FD_CLR(fd_A[i], &fdsr);  // delete fd   
              fd_A[i] = 0;  
              conn_amount--;  
            }  
            else   
            {        // receive data  
              if (ret < BUF_SIZE)  
//                memset(&buf[ret], '/0', 1); // add NULL('/0')  
              
//              printf("client[%d] send:%s/n", i, buf);  
              
              //send
              send(fd_A[i],buf,ret,0);
            }  
          }  
        }  
        
        // check whether a new connection comes  
        if (FD_ISSET(sock_fd, &fdsr))   
        {  
            // accept new connection  
            new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &sin_size);  
            if (new_fd <= 0)   
            {  
                continue;  
            }  
          
          
          // add to fd queue  
          if (conn_amount < BACKLOG)   
          {  
            fd_A[conn_amount++] = new_fd;  
//            printf("new connection client[%d] %s:%d/n", conn_amount,  
//                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));  
            if (new_fd > maxsock)  // update the maxsock fd for select function  
              maxsock = new_fd;  
          }  
          else   
          {  
//              printf("max connections arrive, exit/n");  
              send(new_fd, "bye", 4, 0);  
              close(new_fd);  
              break;     
          }  
        }  
        
      }  
      
      
      // close other connections  
      for (i = 0; i < BACKLOG; i++)   
      {  
        if (fd_A[i] != 0)   
        {   
          close(fd_A[i]);  
        }  
      } 
  
}


#else
char ClientRevBuf[BUF_SIZE];
void TCPClient(void *arg)
{
	struct sockaddr_in ServerAddr;
	int optval = 1;
	fd_set fdsr;
	struct timeval tv;
	
	(void)arg;
	xClientSocket.bconnect = 0;
	//����������
	xClientSocket.sxMutex = xSemaphoreCreateMutex();
	
	if(xClientSocket.sxMutex == NULL )
	{
		while(1);
	}
	while(1)
	{
		//�����ͻ���
		xClientSocket.s = lwip_socket( AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(xClientSocket.s == -1 ){
			continue;
		}
		//�����ɹ�
		optval = 1;
		
		lwip_setsockopt(xClientSocket.s,SOL_SOCKET,SO_KEEPALIVE,&optval,sizeof(optval));
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_addr.s_addr = inet_addr("10.129.200.79");//inet_addr("122.224.200.89");//
		ServerAddr.sin_port = htons(8080);
		
		xClientSocket.bconnect = 0;
		//���ӷ�����
		if( lwip_connect(xClientSocket.s,(struct sockaddr*)&ServerAddr,sizeof(ServerAddr) ) == - 1)
		{
			//connect error!
			lwip_close(xClientSocket.s);
			continue;
		}
		xClientSocket.bconnect = 1;
		
		//�Ƿ���յ�����
		while(1)
		{
			FD_ZERO(&fdsr);
			FD_SET(xClientSocket.s,&fdsr);
			tv.tv_sec = 10;
			tv.tv_usec = 0 ;

			if( lwip_select(xClientSocket.s + 1,&fdsr,NULL,0,&tv) == 0 )
				continue;
			
			if( FD_ISSET(xClientSocket.s,& fdsr) )
			{
				int datalen;
				int ret;
				//�����ݣ��������
				if( xSemaphoreTake( xClientSocket.sxMutex, portMAX_DELAY ) != pdTRUE ){
					while(1);
				}

				datalen = lwip_recv(xClientSocket.s,ClientRevBuf,BUF_SIZE,0);
				if(datalen > 0)
				{
					//���յ�����,���ԣ����͸�������
					ret = lwip_send(xClientSocket.s,ClientRevBuf,datalen,0);
					
				}else{
					//�������رյ��쳣
					xClientSocket.bconnect = 0 ;
					lwip_close(xClientSocket.s);
					xSemaphoreGive( xClientSocket.sxMutex );
					break;	//�������ӷ�����	
				}
				xSemaphoreGive( xClientSocket.sxMutex );
			}			
		}	
	}
}
#endif