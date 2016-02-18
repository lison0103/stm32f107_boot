#include "sys.h"
#include "delay.h"
#include "stmflash.h"
#include "iap.h"
	

iapfun jump2app; 
u16 iapbuf[1024];   
//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�).
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u16 t;
	u16 i=0;
	u16 temp;
	u32 fwaddr=appxaddr;//��ǰд��ĵ�ַ
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=2)
	{						    
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];	  
		dfu+=2;//ƫ��2���ֽ�
		iapbuf[i++]=temp;	    
		if(i==1024)
		{
			i=0;
			STMFLASH_Write(fwaddr,iapbuf,1024);	
			fwaddr+=2048;//ƫ��2048  16=2*8.����Ҫ����2.
		}
	}
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  
}

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
        //���ջ����ַ�Ƿ�Ϸ�.
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	
	{ 
#if 0          
                 INTX_DISABLE(); 
                 
                //�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		
                //��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)		
		MSR_MSP(*(vu32*)appxaddr);		
                //��ת��APP.
		jump2app();	
                
#endif

		jump2app=(iapfun)*(vu32*)(appxaddr+4);		               
                
                INTX_DISABLE();                                                
                               
                /* MCU peripherals re-initial. */              
                {
                  
                    GPIO_InitTypeDef GPIO_InitStruct;
                                                    
                    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                 
                    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;                  
                                    
                    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;                 
                    GPIO_InitStruct.GPIO_Pin &= ~(GPIO_Pin_13 | GPIO_Pin_14); /* SWDIO/SWCLK */                  
                    GPIO_Init(GPIOA, &GPIO_InitStruct);
                                                       
                    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;                  
                    GPIO_Init(GPIOB, &GPIO_InitStruct);                  
                    GPIO_Init(GPIOC, &GPIO_InitStruct);                  
                    GPIO_Init(GPIOD, &GPIO_InitStruct);                 
                    GPIO_Init(GPIOE, &GPIO_InitStruct);                  
                    GPIO_Init(GPIOF, &GPIO_InitStruct);                 
                    GPIO_Init(GPIOG, &GPIO_InitStruct);                 
  //                  GPIO_Init(GPIOH, &GPIO_InitStruct);                 
  //                  GPIO_Init(GPIOI, &GPIO_InitStruct);

                  /* reset systick */
                  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

                  /* disable all peripherals clock. */
                  RCC->AHBENR = 0;
//                  RCC->APB1ENR = 0;
//                  RCC->APB2ENR = 0;
                  
  //                USART2->BRR = 0;
                  
                  /* Switch to default cpu clock. */
                  RCC->CFGR = 0;
              } /* MCU peripherals re-initial. */
              
              /* Disable MPU */
//              MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;



                /* Set new vector table pointer */
                SCB->VTOR = appxaddr;
                
                /* reset register values */
                __set_BASEPRI(0);
                __set_FAULTMASK(0);
                
                /* set up MSP and switch to it */
                __set_PSP(*(volatile unsigned int*) appxaddr);                  //�������ý���PSP��ջ��ַ
                __set_CONTROL(0);                                               //����CONTROL�Ĵ�����ֵ��������ʲôδ֪???
                __set_MSP(*(volatile unsigned int*) appxaddr);                  //�������ý���MSP��ջ��ַ
                
                /* ensure what we have done could take effect */
                __ISB();
                
                
                INTX_DISABLE();
                
                jump2app();							//��ת��APP.               
                
	}
}		 














