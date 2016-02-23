#include "lsys.h"
#include "delay.h"
#include "hw_test.h"
#include "led.h"
#include "digital_led.h"
#include "usart.h"
#include "ewdt.h"
#include "can.h"
#include "spi.h"

extern u8 dis_data[3];

/******************************************************************************* 
*******************************************************************************/
void HW_TEST_INIT(void)
{
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE );
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  
  
      GPIO_InitTypeDef GPIO_InitStruct;

      //input
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;             
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;       
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
      GPIO_ResetBits(GPIOA, GPIO_Pin_15);
      GPIO_Init(GPIOA , &GPIO_InitStruct);  
      
            
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_8; 
      GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_8);
      GPIO_Init(GPIOB , &GPIO_InitStruct);  
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12; 
      GPIO_ResetBits(GPIOC, GPIO_Pin_12);
      GPIO_Init(GPIOC , &GPIO_InitStruct);        
  
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7; 
      GPIO_ResetBits(GPIOD, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
      GPIO_Init(GPIOD , &GPIO_InitStruct);  
      
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; 
      GPIO_ResetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
      GPIO_Init(GPIOE , &GPIO_InitStruct);  

}

u8 sflag,t,inputnum = 0;
extern u8 canbuf_send[8];
u8 passflag = 1;

#if 1

void HW_TEST(void)
{
    
        canbuf_send[0] = 0;
        canbuf_send[1] = 0;
    
//        sflag = 0;
//        inputnum = 0;
        
        
        
        if(passflag /*&& ( IN1 && IN2 && IN3 && IN4 && IN5 && IN6 && IN7 && IN8 && 
                        IN9 && IN10 && IN11 && IN12 && IN13 && IN14 && IN15 && IN16 )*/)
        {
                GRL1 = 1;
                GRL2 = 1;
                GRL3 = 1;
                GRL4 = 1;
                GRL5 = 1;
                GRL6 = 1;
                GRL7 = 1;
                GRL8 = 1;
                GRL9 = 1;
                
                GSFR1 = 1;
                GSFR2 = 1;
                GSFR3 = 1;
                GSFR4 = 1;
                
                TRANS_CTRL1 = 1;
                TRANS_CTRL2 = 1;
                
                canbuf_send[0] = 0xff;
                canbuf_send[1] = 0xff;
          
        }
        else   
        {
                passflag = 0;
                
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
                
//                canbuf_send[0] = 0xff;
//                canbuf_send[1] = 0xfc;
        
                if ( IN1 )
                {       
                    canbuf_send[0] |= 1 << 0;
                }
                if ( IN2 ) 
                {
                    canbuf_send[0] |= 1 << 1;
                }
                if ( IN3 ) 
                {           
                    canbuf_send[0] |= 1 << 2;

                }
                if ( IN4 ) 
                {
                    canbuf_send[0] |= 1 << 3;
                } 
                if ( IN5 ) 
                {         
                    canbuf_send[0] |= 1 << 4;
                }
                if ( IN6 ) 
                {        
                    canbuf_send[0] |= 1 << 5;

                }
                if ( IN7 ) 
                {      
                    canbuf_send[0] |= 1 << 6;

                }        
                if ( IN8 ) 
                {         
                    canbuf_send[0] |= 1 << 7;

                }
                if ( IN9 ) 
                {        
                    canbuf_send[1] |= 1 << 0;

                }
                if ( IN10 ) 
                {        
                    canbuf_send[1] |= 1 << 1;

                } 
                if ( IN11 ) 
                {         
                    canbuf_send[1] |= 1 << 2;

                }
                if ( IN12 ) 
                {         
                    canbuf_send[1] |= 1 << 3;

                }
                if ( IN13 ) 
                {           
                    canbuf_send[1] |= 1 << 4;

                }         
                if ( IN14 ) 
                {           
                    canbuf_send[1] |= 1 << 5;

                }
                if ( IN15 ) 
                {
                    canbuf_send[1] |= 1 << 6;

                }
                if ( IN16 ) 
                {               
                    canbuf_send[1] |= 1 << 7;

                }                   
                
        }
                    
    
}

#else
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
        inputnum = 0;
        
        if ( IN1 )
        {       
            inputnum = 1;
            sflag++;
        }
        if ( IN2 ) 
        {
            inputnum = 2;
            sflag++;
        }
        if ( IN3 ) 
        {           
            inputnum = 3;
            sflag++;

        }
        if ( IN4 ) 
        {
            inputnum = 4;
            sflag++;
        } 
        if ( IN5 ) 
        {         
            inputnum = 5;
            sflag++;
        }
        if ( IN6 ) 
        {        
            inputnum = 6;
            sflag++;

        }
        if ( IN7 ) 
        {      
            inputnum = 7;
            sflag++;

        }        
        if ( IN8 ) 
        {         
            inputnum = 8;
            sflag++;

        }
        if ( IN9 ) 
        {        
            inputnum = 9;
            sflag++;

        }
        if ( IN10 ) 
        {        
            inputnum = 10;
            sflag++;

        } 
        if ( IN11 ) 
        {         
            inputnum = 11;
            sflag++;

        }
        if ( IN12 ) 
        {         
            inputnum = 12;
            sflag++;

        }
        if ( IN13 ) 
        {           
            inputnum = 13;
            sflag++;

        }         
        if ( IN14 ) 
        {           
            inputnum = 14;
            sflag++;

        }
        if ( IN15 ) 
        {
            inputnum = 15;
            sflag++;

        }
        if ( IN16 ) 
        {               
            inputnum = 16;
            sflag++;

        }    
        
        if(inputnum == 0)
        {
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
        
        }
        
        else if(sflag > 1)
        {
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
        }
        else
        {
            switch(inputnum)
            {
                case 1:
                  GRL1 = 1;break;
                case 2:
                  GRL2 = 1;break;
                case 3:
                  GRL3 = 1;break;
                case 4:
                  GRL4 = 1;break;
                case 5:
                  GRL5 = 1;break;
                case 6:
                  GRL6 = 1;break; 
                case 7:
                  GRL7 = 1;break;
                case 8:
                  GRL8 = 1;break;
                case 9:
                  GRL9 = 1;break;
                case 10:
                  GSFR1 = 1;break;
                case 11:
                  GSFR2 = 1;
                  break;
                case 12:
                  GSFR3 = 1;   
                  break;
                case 13:
                  GSFR4 = 1;
                  break;
                case 14:
                  TRANS_CTRL1 = 1;
                  break;
                case 15:
                  TRANS_CTRL2 = 1;
                  break;
                case 16:
                  GRL1 = 1; 
                  break;
                default:
                  break;
            }
            
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
#endif




#define USE_CAN CAN1

/******************************************************************************* 
*******************************************************************************/
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
                  
//                  printf("%s",canbuf_send[i]);	//显示数据
                }
                
                res=Can_Send_Msg(USE_CAN,canbuf_send,8);//发送8个字节 
                if(res)printf("Failed");		//提示发送失败
                else printf("OK    ");	 		//提示发送成功								   
                
                can_rcv=Can_Receive_Msg(USE_CAN,canbuf_recv);
		if(can_rcv)//接收到有数据
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
//                              printf("%s",canbuf_recv[i]);	//显示数据
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

/******************************************************************************* 
*******************************************************************************/
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

//				printf("%s",canbuf_send[i]);	//显示数据
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
//                              printf("%s",canbuf_recv[i]);	//显示数据
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


/******************************************************************************* 
*******************************************************************************/
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
//                              printf("%s",canbuf_recv[i]);	//显示数据
 			}
		}
                
                
		t++; 
		delay_ms(10);
		if(t==50)
		{
			LED1=!LED1;//提示系统正在运行	
                        EWDT_TOOGLE();
			t=0;
		}		   
	}
}


/******************************************************************************* 
*******************************************************************************/
u8 Master_Temp =0;
void spi1_test(void)
{  
  
    u8 t;
    
    SPI1_Init();

//    SPI1_SetSpeed(SPI_BaudRatePrescaler_256);

   while(1)
   { 


     
#if 0
       SPI1_ReadWriteByte(0x55); 
       Master_Temp = SPI1_ReadWriteByte(0x00);
#else
       SPI1_WriteByte(0x66); 
//       delay_ms(1);
       Master_Temp = SPI1_ReadByte(0x00);
#endif
       
       delay_ms(10); 
       
       t++;
       if(t == 50)
       {
             t = 0;
             LED0 =! LED0;
                             
         
       }
       

   }

}