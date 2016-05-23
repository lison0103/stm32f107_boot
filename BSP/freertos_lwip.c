/*******************************************************************************
* File Name          : freertos_lwip.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains lwip tcp demo.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "freertos_lwip.h" 
#include "led.h"  
#include "ewdt.h"
#include <string.h>
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct _XTCPCLIENTSOCK{
    
        /* Socket identifier -1 invalid,> = 0 valid */
	int s;						
        /* Socket whether the connection is successful, 0 is not connected,> = 1 connection */
	int bconnect;				
        /* Mutex, socket not thread-safe, in order to achieve socket receiving 
        data in a thread while another thread is sent to do build mutual exclusion mutex */
	xSemaphoreHandle	sxMutex;
}
XTCPCLIENTSOCK;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define BUF_SIZE	1024
#define MYPORT 8088    // the port users will be connecting to  
#define BACKLOG 3     // how many pending connections queue will hold 

/* Private variables ---------------------------------------------------------*/
char *SERVER_ADDRESS = "10.129.200.77";
char show_buf[] = "The most number of connections reached! bye!";

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void TCPServer(void *arg);
void TCPClient(void *arg);





/*******************************************************************************
* Function Name  : tcp_client_init
* Description    : Creat tcp client task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void tcp_client_init(void)
{
	xTaskCreate(TCPClient, "TCPClient",  DEFAULT_THREAD_STACKSIZE * 2, NULL, TCP_TASK_PRIO, NULL);
}


/*******************************************************************************
* Function Name  : tcp_server_init
* Description    : Creat tcp server task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void tcp_server_init(void)
{
	xTaskCreate(TCPServer, "TCPServer",  DEFAULT_THREAD_STACKSIZE * 2, NULL, TCP_TASK_PRIO, NULL);
}


/*******************************************************************************
* Function Name  : lwip_dhcp_init
* Description    : Creat lwip dhcp task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void lwip_dhcp_init(void)
{
	xTaskCreate(LwIP_DHCP_task, "DHCP", configMINIMAL_STACK_SIZE * 2, NULL,DHCP_TASK_PRIO, NULL);
}





#if 1

/*******************************************************************************
* Function Name  : TCPServer
* Description    : Tcp server demo.
*                  
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.  
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void TCPServer(void *arg)
{
      int sock_fd, new_fd;             // listen on sock_fd, new connection on new_fd  
      struct sockaddr_in server_addr;  // server address information  
      struct sockaddr_in client_addr;  // connector's address information  
      socklen_t sin_size;  
      int yes = 1;  
      char buf[BUF_SIZE];  
      int fd_A[BACKLOG] = {0};    // accepted connection fd  
      int conn_amount;      // current connection amount 
      int ret;  
      int i;  
      
      if ((sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {  

      }  
      
      //Unblocked
      if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1) {  
         
      
      }  
      
      /** don't support this Protocol **/
      if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {  

      }  
      server_addr.sin_family = AF_INET;         // host byte order  
      server_addr.sin_port = htons(MYPORT);     // short, network byte order  
      server_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP  
      memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));  
      
      
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
        if (ret < 0) 
        {      
          // error   
          break;  
        } else if (ret == 0) 
        {  
          // time out   
          continue;  
        }  
        
        
        // check every fd in the set  
        for (i = 0; i < BACKLOG; i++)   
        {  
            if((fd_A[i] != 0))
            { 
                  // check which fd is ready 
                  if (FD_ISSET(fd_A[i], &fdsr))  
                  {  
                    
                      ret = recv(fd_A[i], buf, sizeof(buf), 0);  
                      if (ret <= 0)   
                      {        
                          // client close   
                          close(fd_A[i]);  
                          // delete fd  
                          FD_CLR(fd_A[i], &fdsr);   
                          fd_A[i] = 0;  
                          conn_amount--;  
                      }  
                      else   
                      {        
                          // receive data  
                          if (ret < BUF_SIZE)  
                          {              
                              //send
                              send(fd_A[i],buf,ret,0);
                          }
                      }  
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
                for (i = 0; i < BACKLOG; i++) {  
                  if (fd_A[i] == 0) {  
                    fd_A[i] = new_fd;  
                    conn_amount++;
                    break;
                  }  
                }   
     
                // update the maxsock fd for select function  
                if (new_fd > maxsock)  
                {
                    maxsock = new_fd;  
                }
            }  
            else   
            {   
              ret = sizeof(show_buf);
                send(new_fd, show_buf, sizeof(show_buf), 0);
//                send(new_fd, "bye!", 5, 0);  
                close(new_fd);  
//                break;     
            }  
        }  
        
      }  
      
      
      // close other connections  
//      for (i = 0; i < BACKLOG; i++)   
//      {  
//          if (fd_A[i] != 0)   
//          {   
//              close(fd_A[i]);  
//                fd_A[i] = 0;
//          }  
//      } 
  
}


#else


void TCPServer(void *arg)
{
    int rcd;  
    int new_cli_fd;  
    int maxfd;  
    int socklen, server_len;  
    int ci;  
    int watch_fd_list[BACKLOG + 1];  
    for (ci = 0; ci <= BACKLOG; ci++)  
        watch_fd_list[ci] = -1;  
  
    int server_sockfd;  

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (server_sockfd == -1) {  
  
    }  
  
 
    if (fcntl(server_sockfd, F_SETFL, O_NONBLOCK) == -1) {  
 
    }  
  
    struct sockaddr_in server_sockaddr;  
    memset(&server_sockaddr, 0, sizeof(server_sockaddr));  
    server_sockaddr.sin_family = AF_INET;  
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  

    server_sockaddr.sin_port = htons(MYPORT);  
    server_len = sizeof(server_sockaddr);  
 
    rcd = bind(server_sockfd, (struct sockaddr *) &server_sockaddr, server_len);  
    if (rcd == -1) {  
  
    }  

    rcd = listen(server_sockfd, BACKLOG);  
    if (rcd == -1) {  
 
    }  
 
  
    watch_fd_list[0] = server_sockfd;  
    maxfd = server_sockfd;  
  

    fd_set watchset;  
    FD_ZERO(&watchset);  
    FD_SET(server_sockfd, &watchset);  
  
    struct timeval tv; 
    struct sockaddr_in cli_sockaddr;  
    while (1) {  
  
        tv.tv_sec = 20;  
        tv.tv_usec = 0; 

        FD_ZERO(&watchset);  
        FD_SET(server_sockfd, &watchset);  

        for (ci = 0; ci <= BACKLOG; ci++)  
            if (watch_fd_list[ci] != -1) {  
                FD_SET(watch_fd_list[ci], &watchset);  
            }  
  
        rcd = select(maxfd + 1, &watchset, NULL, NULL, &tv);  
        switch (rcd) {  
        case -1:  
 
        case 0:  
  

            FD_ZERO(&watchset);  
            for (ci = 1; ci <= BACKLOG; ci++){  
                shutdown(watch_fd_list[ci],2);  
                watch_fd_list[ci] = -1;  
            }  

            FD_CLR(server_sockfd, &watchset);  
            FD_SET(server_sockfd, &watchset);  
            continue;  
        default:  

            if (FD_ISSET(server_sockfd, &watchset)) { //new connection  
                socklen = sizeof(cli_sockaddr);  
                new_cli_fd = accept(server_sockfd, (struct sockaddr *) &cli_sockaddr, &socklen);  
                if (new_cli_fd < 0) {  

                }  

  
                for (ci = 1; ci <= BACKLOG; ci++) {  
                    if (watch_fd_list[ci] == -1) {  
                        watch_fd_list[ci] = new_cli_fd;  
                        break;  
                    }  
                }  
  
                FD_SET(new_cli_fd, &watchset);  
                if (maxfd < new_cli_fd) {  
                    maxfd = new_cli_fd;  
                }  
  
                continue;  
            } else {  
                for (ci = 1; ci <= BACKLOG; ci++) { //data  
                    if (watch_fd_list[ci] == -1)  
                        continue;  
                    if (!FD_ISSET(watch_fd_list[ci], &watchset)) {  
                        continue;  
                    }  
                    char buffer[128];  
 
                    int len = recv(watch_fd_list[ci], buffer, 128, 0);  
                    if (len < 0) {  
 
                    }  
                    buffer[len] = 0;  
  
 
                    len = send(watch_fd_list[ci], buffer, strlen(buffer), 0);  
                    if (len < 0) {  
 
                    }   
  

                    if (strcmp(buffer, "quit") == 0) {  
                        for (ci = 0; ci <= BACKLOG; ci++)  
                            if (watch_fd_list[ci] != -1) {  
                                shutdown(watch_fd_list[ci],2);  
                            }  
                    }  
                }  
            }  
            break;  
        }  
    }  
}
#endif


/*******************************************************************************
* Function Name  : TCPClient
* Description    : Tcp client demo.
*                  
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void TCPClient(void *arg)
{
	struct sockaddr_in ServerAddr;
	int optval = 1;
	fd_set fdsr;
	struct timeval tv;
        char ClientRevBuf[BUF_SIZE];
        XTCPCLIENTSOCK xClientSocket;
	
	(void)arg;
	xClientSocket.bconnect = 0;
	//set mutex
	xClientSocket.sxMutex = xSemaphoreCreateMutex();
	
	if(xClientSocket.sxMutex == NULL )
	{
		while(1);
	}
	while(1)
	{
		//create a client
		xClientSocket.s = lwip_socket( AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(xClientSocket.s == -1 ){
			continue;
		}
		//success
		optval = 1;
		
		lwip_setsockopt(xClientSocket.s,SOL_SOCKET,SO_KEEPALIVE,&optval,sizeof(optval));
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
		ServerAddr.sin_port = htons(MYPORT);
		
		xClientSocket.bconnect = 0;
		//connect to server
		if( lwip_connect(xClientSocket.s,(struct sockaddr*)&ServerAddr,sizeof(ServerAddr) ) == - 1)
		{
			//connect error!
			lwip_close(xClientSocket.s);
			continue;
		}
		xClientSocket.bconnect = 1;
		
		//Whether it has received data
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
				//Data, mutual exclusion
				if( xSemaphoreTake( xClientSocket.sxMutex, portMAX_DELAY ) != pdTRUE ){
					while(1);
				}

				datalen = lwip_recv(xClientSocket.s,ClientRevBuf,BUF_SIZE,0);
				if(datalen > 0)
				{
					//The received data back to the server
					ret = lwip_send(xClientSocket.s,ClientRevBuf,datalen,0);
                                        if(ret < 0)
                                        {
                                            /** send error **/
                                        }
					
				}else{
					//close server error,reconnect to server
					xClientSocket.bconnect = 0 ;
					lwip_close(xClientSocket.s);
					xSemaphoreGive( xClientSocket.sxMutex );
					break;	
				}
				xSemaphoreGive( xClientSocket.sxMutex );
			}			
		}	
	}
}




/******************************  END OF FILE  *********************************/


