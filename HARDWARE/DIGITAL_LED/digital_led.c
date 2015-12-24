#include "digital_led.h"
#include "sys.h"
#include "delay.h"

const u8 led_dm[3] = {0x04,0x3d,0x00}; //a,s,f 
const u8 bcd[10] = {0x04,0x3d,0x41,0x11,0x38,0x12,0x02,0x35,0x00,0x10}; 

u8 dis_data[3]={0,0,0};

/*******************************************************************************
*******************************************************************************/
void digital_led_gpio_init(void)
{
      GPIO_InitTypeDef GPIO_InitStruct;
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;             
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;       
      GPIO_Init(GPIOC , &GPIO_InitStruct);    
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
      GPIO_Init(GPIOD , &GPIO_InitStruct);  
      
      LED_OE = 0;
      delay_ms(2);
      LED_OE = 1;
}

void txbyte(u8 dat)
{
    u8 i,j;
    
    j = dat;
    for(i = 0; i < 8; i++)
    {
        LED_SDI = (j&0x80)>>7;
        
        LED_SCLK = 0;
        
        LED_SCLK = 1;
        
        j <<= 1;
    }
    
    LED_RCLK = 0;
    
    LED_RCLK = 1;
}

void led_display1(void)
{
  static u32 dis_cnt=0;
  
  dis_cnt++;
  if(dis_cnt>2) dis_cnt=0;

  txbyte(bcd[dis_data[dis_cnt]]);
  
  if(dis_cnt==0) 
  {
    LED_NUM1 = 0; 
  } 
  else if(dis_cnt==1) 
  {
    LED_NUM2 = 0; 
  }  
  else if(dis_cnt==2) 
  {
    LED_NUM3 = 0; 
  }    
}

/*******************************************************************************
*******************************************************************************/
void led_display(void)
{  
  led_display1();                                                                                    
}

/*******************************************************************************
*******************************************************************************/
