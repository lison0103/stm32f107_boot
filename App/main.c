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

//�û�����������
//����ֵ:0,����
//       1,������
u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	printf("�豸���ӳɹ�!.\n");	 
	res=exf_getfree("0:",&total,&free);
	if(res==0)
	{	   
		printf("FATFS OK!\n");	
		printf("U Disk Total Size:  %d   MB\n",total>>10);//��ʾU�������� MB	 
		printf("U Disk  Free Size:  %d   MB\n",free>>10); 	    	
	} 
        
        if(isFileExist("0:123.txt"))
        {
            printf("�ļ�������\n");
        }
        else
        {
//            UpdateApp("0:stm32f107_cb.bin");
//            INTX_DISABLE();
//            iap_load_app(FLASH_APP1_ADDR);
        }
 
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//�豸���ӳɹ�
	{	
		LED1=!LED1;
		delay_ms(200);
                
                EWDT_TOOGLE();
	}
   
	printf("�豸������...\n");

	return res;
} 


int main(void)
{        
	u8 t;
        
        //����ϵͳ�ж����ȼ�����2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        delay_init();
        
        //�ⲿ���Ź���ʼ��
        EWDT_Drv_pin_config();
//        power_on_bsp_check();
        
        //��ʼ�����ڲ�����Ϊ115200
//	uart_init(115200);		
        
        //��ʼ����LED���ӵ�Ӳ���ӿ�
	LED_Init();		
        
        //��ʼ��USMART	
//	usmart_dev.init(84); 	 
               
        //��ʼ���ڲ��ڴ��	
	mem_init();	
               
        //����ܳ�ʼ��
        digital_led_gpio_init();               
        digital_led_check();

        
        //MB85RCXX��ʼ��
        eep_init();
        if(MB85RCXX_Check())
        {
            printf("MB85RCXX_Checkʧ��\n");
          
        }        
        
        //AT24CXX��ʼ��
        AT24CXX_Init();        
        if(AT24CXX_Check())
        {
            printf("AT24CXX_Checkʧ��\n");
          
        }
        
        //Ϊfatfs��ر��������ڴ� 
 	if(exfuns_init())			
        {
            printf("fatfs�ڴ�����ʧ��\n");
        
        }
        
        //����U��  
  	f_mount(fs[0],"0:",1); 	
     	       
	//��ʼ��USB����
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










