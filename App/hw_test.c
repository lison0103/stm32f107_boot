#include "sys.h"
#include "delay.h"
#include "hw_test.h"
#include "led.h"
#include "digital_led.h"

extern u8 dis_data[3];

void HW_TEST_INIT(void)
{
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE );
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  
  
      GPIO_InitTypeDef GPIO_InitStruct;

      //input
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;             
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;       
      GPIO_Init(GPIOA , &GPIO_InitStruct);    
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;                  
      GPIO_Init(GPIOB , &GPIO_InitStruct);        

      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;                  
      GPIO_Init(GPIOC , &GPIO_InitStruct);   

      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);   
      
      
      //output
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15; 
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_Init(GPIOA , &GPIO_InitStruct);  
            
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_8; 
      GPIO_Init(GPIOB , &GPIO_InitStruct);  
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12; 
      GPIO_Init(GPIOC , &GPIO_InitStruct);        
  
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7; 
      GPIO_Init(GPIOD , &GPIO_InitStruct);  
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; 
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

}

void HW_TEST(void)
{
    u8 sflag,t,inputnum = 0;
  
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
    
    dis_data[0] = 0;
    dis_data[1] = 0;
    dis_data[2] = 0;
    
    while(1)
    {
        sflag = 0;
        
        if ( IN1 )
        {
            GRL1 = 1;
            GRL2 = 0;
            GRL3 = 0;
            GRL4 = 0;
            GRL5 = 0;
            
            inputnum = 1;
            sflag++;
        }
        if ( IN2 ) 
        {
            GRL1 = 0;
            GRL2 = 1;
            GRL3 = 0;
            GRL4 = 0;
            GRL5 = 0;          

            inputnum = 2;
            sflag++;
        }
        if ( IN3 ) 
        {
            GRL1 = 1;
            GRL2 = 1;
            GRL3 = 0;
            GRL4 = 0;
            GRL5 = 0;          
            
            inputnum = 3;
            sflag++;

        }
        if ( IN4 ) 
        {
            GRL1 = 0;
            GRL2 = 0;
            GRL3 = 1;
            GRL4 = 0;
            GRL5 = 0;          

            inputnum = 4;
            sflag++;
        } 
        if ( IN5 ) 
        {
            GRL1 = 1;
            GRL2 = 0;
            GRL3 = 1;
            GRL4 = 0;
            GRL5 = 0;     
            
            inputnum = 5;
            sflag++;
        }
        if ( IN6 ) 
        {
            GRL1 = 0;
            GRL2 = 1;
            GRL3 = 1;
            GRL4 = 0;
            GRL5 = 0;        
            
            inputnum = 6;
            sflag++;

        }
        if ( IN7 ) 
        {
            GRL1 = 1;
            GRL2 = 1;
            GRL3 = 1;
            GRL4 = 0;
            GRL5 = 0;          
            
            inputnum = 7;
            sflag++;

        }        
        if ( IN8 ) 
        {
            GRL1 = 0;
            GRL2 = 0;
            GRL3 = 0;
            GRL4 = 1;
            GRL5 = 0;        
            
            inputnum = 8;
            sflag++;

        }
        if ( IN9 ) 
        {
            GRL1 = 1;
            GRL2 = 0;
            GRL3 = 0;
            GRL4 = 1;
            GRL5 = 0;   
            
            inputnum = 9;
            sflag++;

        }
        if ( IN10 ) 
        {
            GRL1 = 0;
            GRL2 = 1;
            GRL3 = 0;
            GRL4 = 1;
            GRL5 = 0;      
            
            inputnum = 10;
            sflag++;

        } 
        if ( IN11 ) 
        {
            GRL1 = 1;
            GRL2 = 1;
            GRL3 = 0;
            GRL4 = 1;
            GRL5 = 0;     
            
            inputnum = 11;
            sflag++;

        }
        if ( IN12 ) 
        {
            GRL1 = 0;
            GRL2 = 0;
            GRL3 = 1;
            GRL4 = 1;
            GRL5 = 0;        
            
            inputnum = 12;
            sflag++;

        }
        if ( IN13 ) 
        {
            GRL1 = 1;
            GRL2 = 0;
            GRL3 = 1;
            GRL4 = 1;
            GRL5 = 0;      
            
            inputnum = 13;
            sflag++;

        }         
        if ( IN14 ) 
        {
            GRL1 = 0;
            GRL2 = 1;
            GRL3 = 1;
            GRL4 = 1;
            GRL5 = 0;       
            
            inputnum = 14;
            sflag++;

        }
        if ( IN15 ) 
        {
            GRL1 = 1;
            GRL2 = 1;
            GRL3 = 1;
            GRL4 = 1;
            GRL5 = 0;       
            
            inputnum = 15;
            sflag++;

        }
        if ( IN16 ) 
        {
            GRL1 = 0;
            GRL2 = 0;
            GRL3 = 0;
            GRL4 = 0;
            GRL5 = 1;      
            
            inputnum = 16;
            sflag++;

        }    
        
        if(sflag > 1)
        {
            GRL6 = 1; 
            dis_data[0] = 0;
            dis_data[1] = 0;
            dis_data[2] = 0;
        }
        else
        {
            dis_data[0] = 0;
            dis_data[1] = inputnum/10;
            dis_data[2] = inputnum%10;
        }
        
        led_display();
        
        delay_ms(1);
        t++;
        if(t==200)
        {
            LED0=!LED0;
            LED1=!LED1;
            t=0;
        }        
        
    }
}