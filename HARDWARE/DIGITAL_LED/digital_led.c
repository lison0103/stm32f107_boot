#include "digital_led.h"
#include "lsys.h"
#include "delay.h"
#include "ewdt.h"

const u8 bcd[11] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff}; //0~9£¬null

u8 dis_data[3]={0,0,0};

/*******************************************************************************
*******************************************************************************/
void digital_led_gpio_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE );
  
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

/******************************************************************************* 
*******************************************************************************/
void txbyte(u8 dat)
{
    u8 i,j,k;
    
    LED_OE = 0;
    j = dat;
    for(i = 0; i < 8; i++)
    {
        LED_SCLK = 0;
      
        k = (j&0x80)>>7;
        
        LED_SDI = k;
                                  
        j <<= 1;
        
        LED_SCLK = 1;
    }
    
    LED_RCLK = 0;
    delay_us(1);
    LED_RCLK = 1;
}

/******************************************************************************* 
*******************************************************************************/
void led_display1(void)
{
  static u32 dis_cnt=0;
  
  dis_cnt++;
  if(dis_cnt>2) dis_cnt=0;
  
  LED_NUM1 = 1;
  LED_NUM2 = 1;
  LED_NUM3 = 1;

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
void digital_led_check(void)
{
    u8 count = 0;
  
    for(u8 i = 0; i < 11; i++)
    {
        count = 100;
        
        for(u8 j = 0; j < 3; j++)
        {
          dis_data[j] = i;
        }
        
        while(count)
        {
          led_display();
          delay_ms(2);              
          count--;
        }  
        
        EWDT_TOOGLE();
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
