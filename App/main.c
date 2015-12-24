#include "sys.h"
#include "delay.h"  
//#include "usart.h"   
#include "led.h"  
#include "malloc.h" 
#include "ff.h"  
#include "exfuns.h"     
#include "usbh_usr.h" 
#include "24cxx.h"
#include "mb85rcxx.h"
#include "digital_led.h"



USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

extern u8 dis_data[3];

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
 
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//设备连接成功
	{	
		LED1=!LED1;
		delay_ms(200);
	}
   
	printf("设备连接中...\n");

	return res;
} 


int main(void)
{        
	u8 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
        delay_init();
//	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();				//初始化与LED连接的硬件接口
//	usmart_dev.init(84); 	//初始化USMART	 
	mem_init();	//初始化内部内存池	
               
        digital_led_gpio_init();
        for(u8 i = 0; i < 9; i++)
        {
              for(u8 j = 0; j < 3; j++)
              {
                  dis_data[j] = i;
                  led_display();                 
              }
              
              delay_ms(1000);
          
        }
        
        eep_init();
        if(MB85RCXX_Check())
        {
            printf("MB85RCXX_Check失败\n");
          
        }        
               
        AT24CXX_Init();        
        if(AT24CXX_Check())
        {
            printf("AT24CXX_Check失败\n");
          
        }
        
 	if(exfuns_init())			//为fatfs相关变量申请内存 
        {
            printf("fatfs内存申请失败\n");
        
        }
        
        
  	f_mount(fs[0],"0:",1); 	//挂载U盘  
     	       
	//初始化USB主机
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);  
	while(1)
	{
		USBH_Process(&USB_OTG_Core, &USB_Host);
		delay_ms(1);
		t++;
		if(t==200)
		{
			LED0=!LED0;
			t=0;
		}
	}	
}










