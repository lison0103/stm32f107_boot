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

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

extern u8 dis_data[3];

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
		printf("FATFS OK!\n");	
		printf("U Disk Total Size:  %d   MB\n",total>>10);//显示U盘总容量 MB	 
		printf("U Disk  Free Size:  %d   MB\n",free>>10); 	    	
	} 
        
        if(isFileExist("0:123.txt"))
        {
            printf("文件不存在\n");
        }
        else
        {
              printf("文件存在\n");
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
        EWDT_Drv_pin_config();
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
#if 1        
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
        HW_TEST();

#else        
        //can测试
        can_test();
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

	 	
   
	CAN_Mode_Init(USE_CAN,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN初始化环回模式,波特率500Kbps    

	
 	while(1)
	{

		if(mode==CAN_Mode_LoopBack)//KEY0按下,发送一次数据
		{
			for(i=0;i<8;i++)
			{
				canbuf_send[i]=cnt+i;//填充发送缓冲区

				printf("%s",canbuf_send[i]);	//显示数据
 			}
			res=Can_Send_Msg(USE_CAN,canbuf_send,8);//发送8个字节 
			if(res)printf("Failed");		//提示发送失败
			else printf("OK    ");	 		//提示发送成功								   
		}else if(mode==CAN_Mode_Normal)//WK_UP按下，改变CAN的工作模式
		{	   
//			mode=!mode;
  			CAN_Mode_Init(USE_CAN,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN普通模式初始化, 波特率500Kbps 

			if(mode==0)//普通模式，需要2个开发板
			{
				printf("Nnormal Mode ");	    
			}else //回环模式,一个开发板就可以测试了.
			{
 				printf("LoopBack Mode");
			}

		}		 
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


