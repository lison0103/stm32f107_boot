#include "freertos_lwip.h" 
#include "led.h"  

#include <string.h>

void led0_task(void *arg)
{
        LED0 = 0;
        
	for( ; ; )
	{
                LED0 =!LED0;
		vTaskDelay( 500 );
	}
}
void led1_task(void *arg)
{
        LED1 = 1;
        
	for( ; ; )
	{
                LED1 =!LED1;
		vTaskDelay( 500 );
	}
}




char *SERVER_ADDRESS = "10.129.200.77";
char show_buf[] = "The most number of connections reached! bye!";

#if 1

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
      
      //设为非阻塞  
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
    //建立socket，类型为TCP流  
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (server_sockfd == -1) {  
  
    }  
  
    //设为非阻塞  
    if (fcntl(server_sockfd, F_SETFL, O_NONBLOCK) == -1) {  
 
    }  
  
    struct sockaddr_in server_sockaddr;  
    memset(&server_sockaddr, 0, sizeof(server_sockaddr));  
    server_sockaddr.sin_family = AF_INET;  
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    //设置监听端口  
    server_sockaddr.sin_port = htons(MYPORT);  
    server_len = sizeof(server_sockaddr);  
    //绑定  
    rcd = bind(server_sockfd, (struct sockaddr *) &server_sockaddr, server_len);  
    if (rcd == -1) {  
  
    }  
    //监听  
    rcd = listen(server_sockfd, BACKLOG);  
    if (rcd == -1) {  
 
    }  
 
  
    watch_fd_list[0] = server_sockfd;  
    maxfd = server_sockfd;  
  
    //初始化监听集合  
    fd_set watchset;  
    FD_ZERO(&watchset);  
    FD_SET(server_sockfd, &watchset);  
  
    struct timeval tv; /* 声明一个时间变量来保存时间 */  
    struct sockaddr_in cli_sockaddr;  
    while (1) {  
  
        tv.tv_sec = 20;  
        tv.tv_usec = 0; /* 设置select等待的最大时间为20秒*/  
        //每次都要重新设置集合才能激发事件  
        FD_ZERO(&watchset);  
        FD_SET(server_sockfd, &watchset);  
        //对已存在到socket重新设置  
        for (ci = 0; ci <= BACKLOG; ci++)  
            if (watch_fd_list[ci] != -1) {  
                FD_SET(watch_fd_list[ci], &watchset);  
            }  
  
        rcd = select(maxfd + 1, &watchset, NULL, NULL, &tv);  
        switch (rcd) {  
        case -1:  
 
        case 0:  
  
            //超时则清理掉所有集合元素并关闭所有与客户端的socket  
            FD_ZERO(&watchset);  
            for (ci = 1; ci <= BACKLOG; ci++){  
                shutdown(watch_fd_list[ci],2);  
                watch_fd_list[ci] = -1;  
            }  
            //重新设置监听socket，等待链接  
            FD_CLR(server_sockfd, &watchset);  
            FD_SET(server_sockfd, &watchset);  
            continue;  
        default:  
            //检测是否有新连接建立  
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
            } else {//已有连接的数据通信  
                //遍历每个设置过的集合元素  
                for (ci = 1; ci <= BACKLOG; ci++) { //data  
                    if (watch_fd_list[ci] == -1)  
                        continue;  
                    if (!FD_ISSET(watch_fd_list[ci], &watchset)) {  
                        continue;  
                    }  
                    char buffer[128];  
                    //接收  
                    int len = recv(watch_fd_list[ci], buffer, 128, 0);  
                    if (len < 0) {  
 
                    }  
                    buffer[len] = 0;  
  

                    //发送接收到到数据  
                    len = send(watch_fd_list[ci], buffer, strlen(buffer), 0);  
                    if (len < 0) {  
 
                    }   
  
                    //接收到的是关闭命令  
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
	//建立互斥量
	xClientSocket.sxMutex = xSemaphoreCreateMutex();
	
	if(xClientSocket.sxMutex == NULL )
	{
		while(1);
	}
	while(1)
	{
		//创建客户端
		xClientSocket.s = lwip_socket( AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(xClientSocket.s == -1 ){
			continue;
		}
		//创建成功
		optval = 1;
		
		lwip_setsockopt(xClientSocket.s,SOL_SOCKET,SO_KEEPALIVE,&optval,sizeof(optval));
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
		ServerAddr.sin_port = htons(MYPORT);
		
		xClientSocket.bconnect = 0;
		//连接服务器
		if( lwip_connect(xClientSocket.s,(struct sockaddr*)&ServerAddr,sizeof(ServerAddr) ) == - 1)
		{
			//connect error!
			lwip_close(xClientSocket.s);
			continue;
		}
		xClientSocket.bconnect = 1;
		
		//是否接收到数据
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
				//有数据，互斥操作
				if( xSemaphoreTake( xClientSocket.sxMutex, portMAX_DELAY ) != pdTRUE ){
					while(1);
				}

				datalen = lwip_recv(xClientSocket.s,ClientRevBuf,BUF_SIZE,0);
				if(datalen > 0)
				{
					//接收的数据,测试，回送给服务器
					ret = lwip_send(xClientSocket.s,ClientRevBuf,datalen,0);
                                        if(ret < 0)
                                        {
                                            /** send error **/
                                        }
					
				}else{
					//服务器关闭等异常
					xClientSocket.bconnect = 0 ;
					lwip_close(xClientSocket.s);
					xSemaphoreGive( xClientSocket.sxMutex );
					break;	//重新连接服务器	
				}
				xSemaphoreGive( xClientSocket.sxMutex );
			}			
		}	
	}
}