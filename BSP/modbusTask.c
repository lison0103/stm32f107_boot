/*******************************************************************************
* File Name          : modbusTask.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains modbus task functions.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "mb.h"
#include "crc16.h"
#include "lsys.h"
#include "usart.h"
#include "esc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define MODBUS_THREAD_PRIO    ( tskIDLE_PRIORITY + 3 )

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


extern unsigned char TmpBuf[256];
extern unsigned short mbFramBuflen;

#if defined(MULTIPLE_MODBUS_TCP_CONNECT)

#define BACKLOG 3
int fd_A[BACKLOG] = {0};    // accepted connection fd  
int tcp_fd_num = 0;

#else

int newconn;

#endif

u8 trx_buff[200];
u8 txbuff_size = 0;
u8 timeset = 0;

/*******************************************************************************
* Function Name  : ddu_rx_decode
* Description    : decode rx ddu data.
*                  
* Input          : buff: receive date  len: the length of receive data                       
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void ddu_rx_decode(u8 *buff, u16 len) 
{
    u16 i=0,j=0,k=0;
    
    i = buff[2]<<8 | buff[3];//address
    j = buff[4]<<8 | buff[5];//length
    
    if((len < 98))
    {
        
        if((buff[0] == 1) && (buff[1] == 16) && (i < 15))
        { 
            /* DDU set time */
            if( i == 4 && j == 3 )
            {
                timeset = 1;
                for( k = 0; k < 6; k++ )
                {
                    ESC_RTCC[k] = buff[7 + k];
                }
            }
        } 
        else
        {
             /* DDU remote set time */
            if( i >= 25 && i <= 27 && j == 1 )
            {
                timeset = 1;
                ESC_RTCC[0] = buff[7];
                ESC_RTCC[1] = buff[8];
            }            
        }                           
        
        //return    
        if(buff[1] == 16)
        {  
            txbuff_size = 8;         
        }
        else
        {
            txbuff_size = 0;
        }  
    }  
    else
    {
        //return error code
        trx_buff[1] |= 0x80;
        trx_buff[2] = 1;
        txbuff_size = 5;
    }
    
    //CRC
    if(txbuff_size)
    {  
        i = MB_CRC16(trx_buff,txbuff_size - 2);
        trx_buff[txbuff_size - 1] = i>>8;
        trx_buff[txbuff_size - 2] = i; 
    }      
}


/*******************************************************************************
* Function Name  : modbus_local_ddu_thread
* Description    : modbus local ddu task.
*                  
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void modbus_local_ddu_thread(void *arg)
{
      
    u8 len = 0;
    u16 uint1 = 0;
    u8 counttms = 0;
    
    while(1)
    {
        
        counttms++;        
        if( counttms == 4 )
        {
            
            counttms = 0;
            
            trx_buff[0] = 3;    
            trx_buff[1] = 16; 
            trx_buff[2] = 0; 
            trx_buff[3] = 0; 
            trx_buff[4] = 0; 
            trx_buff[5] = 50; 
            trx_buff[6] = 100; 
            
            for( u8 i = 0; i < 100; i++ )
            {
                trx_buff[i+7] = 0;    
            }  
            /* send date to ddu */
//            for( u8 i = 20; i < 26; i++ )
//            {
//                trx_buff[i+7] = ESC_RTCC[i-20];    
//            }          
            
            len = 109;
            
            
            uint1 = MB_CRC16( trx_buff, len-2);	        
            trx_buff[len-2] = uint1;
            trx_buff[len-1] = uint1>>8;
            
            BSP_USART_Send(USART3,trx_buff,len);
            
        }
        
        len = BSP_USART_Receive(USART3,trx_buff,0);
        if(!(MB_CRC16(trx_buff,len))) 
        {
            ddu_rx_decode(trx_buff, len);
            BSP_USART_Send(USART3,trx_buff,txbuff_size);
        }
        
        
        
        vTaskDelay( 500 );
    }										 
}

/*******************************************************************************
* Function Name  : modbus_local_ddu_init
* Description    : Create modbus local ddu task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void modbus_local_ddu_init(void)
{
	sys_thread_new("Modbus", modbus_local_ddu_thread, NULL, DEFAULT_THREAD_STACKSIZE, MODBUS_THREAD_PRIO);	
}

/*******************************************************************************
* Function Name  : modbus_rtu_thread
* Description    : modbus rtu task.
*                  
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void modbus_rtu_thread(void *arg)
{
  
        eMBInit(MB_RTU, 0x0A, 0, 115200, MB_PAR_NONE);
	eMBEnable();
        
	while(1)
	{
		(void)eMBPoll();
		vTaskDelay( 100 );
	}										 
}	


/*******************************************************************************
* Function Name  : modbus_rtu_init
* Description    : Create modbus rtu task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void modbus_rtu_init(void)
{
	sys_thread_new("Modbus", modbus_rtu_thread, NULL, DEFAULT_THREAD_STACKSIZE * 2, MODBUS_THREAD_PRIO);	
}



#if defined(MULTIPLE_MODBUS_TCP_CONNECT)


/*******************************************************************************
* Function Name  : modbus_socket_thread
* Description    : modbus tcp task demo.
*                  
* Input          : arg:  a pointer to an optional data area which can be used 
*                       to pass parameters to the task when the task first executes.
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void modbus_socket_thread(void *arg)
{
      int sock_fd, new_fd;             // listen on sock_fd, new connection on new_fd  
      struct sockaddr_in server_addr;  // server address information  
      struct sockaddr_in client_addr;  // connector's address information  
      socklen_t sin_size;  
      int yes = 1;   

      int conn_amount;      // current connection amount 
      int ret;  
      int i;  
      
      if ((sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {  

      }  
      
      if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1) {  
         
      
      }  
      
      /** don't support this Protocol **/
      if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {  

      }  
      server_addr.sin_family = AF_INET;         // host byte order  
      server_addr.sin_port = htons(502);     // short, network byte order  
      server_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP  
      
      
      if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {  

      }  
      
      /* listen for incoming connections (TCP listen backlog = 5) */
      if (listen(sock_fd, BACKLOG) == -1) {  
 
      }
      
      eMBTCPInit(502);
      eMBEnable(); 
  
      
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
                    
                      ret = recv(fd_A[i], TmpBuf, sizeof(TmpBuf), 0);  
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
                          tcp_fd_num = i;
                          mbFramBuflen  = ret;
                          ( void )xMBPortEventPost( EV_FRAME_RECEIVED );  
                          for(i=0;i<2;i++)
                          {
                              eMBPoll();
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
                close(new_fd);       
            }  
        }  
        
      }  
  
}


#else



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
    
    /* listen for incoming connections (TCP listen backlog = 1) */
    listen(sock, 1);
    
    size = sizeof(remotehost);
    eMBTCPInit(502);
    eMBEnable();
    while(1) 
    {
        newconn = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);
        while(1)
        {
            /* Read in the request */
            ret = recv(newconn, TmpBuf, sizeof(TmpBuf),0); 
            if( ret == 0 )	  //disconnect
            {
                close(newconn);
                break;	  //break the loop,wait for a connection
            }
            else if( ret > 0 )
            {
                mbFramBuflen  = ret;
                ( void )xMBPortEventPost( EV_FRAME_RECEIVED );  //has new data
                for( i = 0; i < 2; i++ )
                {
                    eMBPoll();
                }	
            }
            else		 //No connection
            {
                break;
            }
            
            vTaskDelay( 100 );
        }
        
        vTaskDelay( 500 );
    }		
}

#endif


/*******************************************************************************
* Function Name  : modbus_socket_init
* Description    : Create modbus tcp task.
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void modbus_socket_init(void)
{
	sys_thread_new("Modbus", modbus_socket_thread, NULL, DEFAULT_THREAD_STACKSIZE, MODBUS_THREAD_PRIO);	
}



/******************************  END OF FILE  *********************************/


