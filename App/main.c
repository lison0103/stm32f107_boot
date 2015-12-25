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
#include "ewdt.h"
#include "update.h"


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
        
        if(isFileExist("0:123.txt"))
        {
            printf("文件不存在\n");
        }
        else
        {
//            UpdateApp("0:stm32f107_cb.bin");
//            INTX_DISABLE();
//            iap_load_app(FLASH_APP1_ADDR);
        }
 
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//设备连接成功
	{	
		LED1=!LED1;
		delay_ms(200);
                
                EWDT_TOOGLE();
	}
   
	printf("设备连接中...\n");

	return res;
} 


int main(void)
{        
	u8 t;
        
        //设置系统中断优先级分组2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        delay_init();
        
        //外部看门狗初始化
        EWDT_Drv_pin_config();
//        power_on_bsp_check();
        
        //初始化串口波特率为115200
//	uart_init(115200);		
        
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
        
        //为fatfs相关变量申请内存 
 	if(exfuns_init())			
        {
            printf("fatfs内存申请失败\n");
        
        }
        
        //挂载U盘  
  	f_mount(fs[0],"0:",1); 	
     	       
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
                        
                        EWDT_TOOGLE();
		}
	}	
}










