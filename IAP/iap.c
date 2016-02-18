#include "sys.h"
#include "delay.h"
#include "stmflash.h"
#include "iap.h"
	

iapfun jump2app; 
u16 iapbuf[1024];   
//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u16 t;
	u16 i=0;
	u16 temp;
	u32 fwaddr=appxaddr;//当前写入的地址
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=2)
	{						    
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];	  
		dfu+=2;//偏移2个字节
		iapbuf[i++]=temp;	    
		if(i==1024)
		{
			i=0;
			STMFLASH_Write(fwaddr,iapbuf,1024);	
			fwaddr+=2048;//偏移2048  16=2*8.所以要乘以2.
		}
	}
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
}

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
        //检查栈顶地址是否合法.
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	
	{ 
#if 0          
                 INTX_DISABLE(); 
                 
                //用户代码区第二个字为程序开始地址(复位地址)
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		
                //初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)		
		MSR_MSP(*(vu32*)appxaddr);		
                //跳转到APP.
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
                __set_PSP(*(volatile unsigned int*) appxaddr);                  //重新设置进程PSP堆栈地址
                __set_CONTROL(0);                                               //设置CONTROL寄存器的值，作用是什么未知???
                __set_MSP(*(volatile unsigned int*) appxaddr);                  //重新设置进程MSP堆栈地址
                
                /* ensure what we have done could take effect */
                __ISB();
                
                
                INTX_DISABLE();
                
                jump2app();							//跳转到APP.               
                
	}
}		 














