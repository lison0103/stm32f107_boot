#include "sys.h"
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
#include "can.h"
#include "hw_test.h"

void can_test(void);
void can1_can2_test(void);
void hw_can_test(void);

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

extern u8 dis_data[3];

u8 canbuf_send[8];

//void EXTI1_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
//  {
//      USB_Host.usr_cb->OverCurrentDetected();
//      EXTI_ClearITPendingBit(EXTI_Line1);
//  }
//}

//用户测试主程序
//返回值:0,正常
//       1,有问题
u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	printf("设备连接成功!.\n");	 
	res=exf_getfree("0:",&total,&free);
	if(res==0)
	{	   
                total = total>>10;
                free  = free>>10;
		printf("FATFS OK!\n");	
		printf("U Disk Total Size:  %d   MB\n",total);//显示U盘总容量 MB	 
		printf("U Disk  Free Size:  %d   MB\n",free); 	    	
	} 
        
        if(isFileExist("0:123.txt"))
        {
            printf("文件不存在\n");
        }
        else
        {
              printf("文件存在\n");
              
              DeleteFile("0:abc.txt");
              
//              if(isFileExist("0:abc.txt"))
//              {
              
                  CreateFile("0:123.txt", "0:abc.txt");
//              }
              
//              DeleteFile("0:123.txt");
              
//            UpdateApp("0:stm32f107_cb.bin");
//            INTX_DISABLE();
//            iap_load_app(FLASH_APP1_ADDR);
        }
        
        LED0=1;
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//设备连接成功
	{	
		LED1=!LED1;
		delay_ms(200);
                
                EWDT_TOOGLE();
                res=exf_getfree("0:",&total,&free);
                if(res==0)
                {	   
                  printf("FATFS OK!\n");	
                  printf("U Disk Total Size:  %d   MB\n",total>>10);//显示U盘总容量 MB	 
                  printf("U Disk  Free Size:  %d   MB\n",free>>10); 	    	
                }
	}
   
	printf("设备连接中...\n");

	return res;
} 


int main(void)
{        
	u8 t;
        RCC_ClocksTypeDef RCC_Clocks;
        //设置系统中断优先级分组2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        delay_init();
        
        //外部看门狗初始化
//        EWDT_Drv_pin_config();
//        power_on_bsp_check(); 
        
        //初始化串口波特率为115200
	uart_init(115200);		
        
        //初始化与LED连接的硬件接口
	LED_Init();		
        
        //初始化USMART	
//	usmart_dev.init(84); 	 
               
        //初始化内部内存池	
	mem_init();	
               
        //数码管初始化
        digital_led_gpio_init();               
        digital_led_check();


        //MB85RCXX初始化
        eep_init();
        if(MB85RCXX_Check())
        {
            printf("MB85RCXX_Check失败\n");
          
        }        

        //AT24CXX初始化
        AT24CXX_Init();        
        if(AT24CXX_Check())
        {
            printf("AT24CXX_Check失败\n");
          
        }
//RCC_GetClocksFreq(&RCC_Clocks); 
#if 0        
        //为fatfs相关变量申请内存 
 	if(exfuns_init())			
        {
            printf("fatfs内存申请失败\n");
        
        }
        
//        //挂载U盘  
//  	f_mount(fs[0],"0:",1); 	
//     	 RCC_GetClocksFreq(&RCC_Clocks);      
	//初始化USB主机
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);  
	while(1)
	{
		USBH_Process(&USB_OTG_Core, &USB_Host);
		delay_ms(1);
		t++;
		if(t==200)
		{
//                        RCC_GetClocksFreq(&RCC_Clocks); 
			LED0=!LED0;
			t=0;
                        
                        EWDT_TOOGLE();
		}
	}	
#else

#if 1
        HW_TEST_INIT();
//        HW_TEST();
        
        GRL1 = 0;
        GRL2 = 0;
        GRL3 = 0;
        GRL4 = 0;
        GRL5 = 0;
        GRL6 = 0;
        GRL7 = 0;
        GRL8 = 0;
        GRL9 = 0;
        
        GSFR1 = 0;
        GSFR2 = 0;
        GSFR3 = 0;
        GSFR4 = 0;
        
        TRANS_CTRL1 = 0;
        TRANS_CTRL2 = 0;
              
        hw_can_test();
               

#else        
        //can测试
//        can_test();
        can1_can2_test();
        
        //485test
        while(1){
//        USART3_SEND("ABCDE",6);
          if( (USART_GetITStatus(USART3, USART_IT_RXNE) == RESET) && (USART_RX_STA != 0))
          {
//                USART3->SR &= ~0x00000040;
                USART3_SEND(USART_RX_BUF,USART_RX_STA);
                USART_RX_STA = 0;
          }
        }
#endif   
        
#endif
        
}




#define USE_CAN CAN1


void can_test(void)
 {	 
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf_send[8],canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_LoopBack;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式

	 	
        CAN_Mode_Init(CAN1,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,10,mode);
	CAN_Mode_Init(USE_CAN,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,10,CAN_Mode_LoopBack);//CAN初始化环回模式,波特率500Kbps    

	
 	while(1)
	{

	
                for(i=0;i<8;i++)
                {
                  canbuf_send[i]=cnt+i;//填充发送缓冲区
                  
                  printf("%s",canbuf_send[i]);	//显示数据
                }
                
                res=Can_Send_Msg(USE_CAN,canbuf_send,8);//发送8个字节 
                if(res)printf("Failed");		//提示发送失败
                else printf("OK    ");	 		//提示发送成功								   
                
                can_rcv=Can_Receive_Msg(USE_CAN,canbuf_recv);
		if(can_rcv)//接收到有数据
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
                              printf("%s",canbuf_recv[i]);	//显示数据
 			}
		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED1=!LED1;//提示系统正在运行	
			t=0;
			cnt++;
			printf("%d",cnt);	//显示数据
		}		   
	}
}

void can1_can2_test(void)
 {	 
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf_send[8],canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_Normal;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式

	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN初始化环回模式,波特率200Kbps    
        CAN_Mode_Init(CAN2,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN初始化环回模式,波特率200Kbps    

	
 	while(1)
	{
                //CAN1发送
		if(mode==CAN_Mode_Normal)
		{
			for(i=0;i<8;i++)
			{
				canbuf_send[i]=cnt+i;//填充发送缓冲区

				printf("%s",canbuf_send[i]);	//显示数据
 			}
			res=Can_Send_Msg(CAN1,canbuf_send,8);//发送8个字节 
			if(res)
                          printf("Failed");		//提示发送失败
			else 
                          printf("OK    ");	 		//提示发送成功								   
		}

                //CAN2接收  
		can_rcv=Can_Receive_Msg(CAN2,canbuf_recv);
		if(can_rcv)//接收到有数据
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
                              printf("%s",canbuf_recv[i]);	//显示数据
 			}
		}
                
                
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED1=!LED1;//提示系统正在运行	
			t=0;
			cnt++;
			printf("%d",cnt);	//显示数据
		}		   
	}
}

void hw_can_test(void)
 {	 
	u8 i=0,t=0;
	u8 canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_Normal;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式
        u32 aa = 0;
	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,mode);//CAN初始化环回模式,波特率250Kbps    
//        CAN_Mode_Init(CAN2,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,mode);//CAN初始化环回模式,波特率250Kbps    

	
 	while(1)
	{
          
                aa++;
          
                  //CAN1发送
          
                  HW_TEST();
                  
                  if(aa == 100)
                  {
                  
                        res=Can_Send_Msg(CAN1,canbuf_send,2);//发送2个字节 
                        
                        if(res)
                          printf("Failed");		//提示发送失败
                        else 
                          printf("OK    ");	 		//提示发送成功	
                        
                        LED0=!LED0;
                        aa = 0;
                  }

                //CAN2接收  
		can_rcv=Can_Receive_Msg(CAN1,canbuf_recv);
		if(can_rcv)//接收到有数据
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
                              printf("%s",canbuf_recv[i]);	//显示数据
 			}
		}
                
                
		t++; 
		delay_ms(10);
		if(t==50)
		{
			LED1=!LED1;//提示系统正在运行	
			t=0;
		}		   
	}
}
