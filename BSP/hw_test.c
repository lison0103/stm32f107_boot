/*******************************************************************************
* File Name          : hw_test.c
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : 
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "delay.h"
#include "hw_test.h"
#include "led.h"
#include "digital_led.h"
#include "usart.h"
#include "ewdt.h"
#include "can.h"
#include "bsp_iocfg.h"
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern u8 dis_data[3];
u8 sflag,t,inputnum = 0;
u8 canbuf_send[8];
u8 passflag = 1;



#if 1

/*******************************************************************************
* Function Name  : Input_Check
* Description    : None
*                  
* Input          : None
*                  None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/ 
void Input_Check(void)
{
    
        canbuf_send[0] = 0;
        canbuf_send[1] = 0;
    
//        sflag = 0;
//        inputnum = 0;
        
        
        
        if(passflag && ( IN1 && IN2 && IN3 && IN4 && IN5 && IN6 && IN7 && IN8 && 
                        IN9 && IN10 && IN11 && IN12 && IN13 && IN14 && IN15 && IN16 ))
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

void Input_Check(void)
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
* Function Name  : can_test
* Description    : None
*                  
* Input          : None
*                  None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void can_test(void)
 {	 
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf_send[8],canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_LoopBack;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ

	 	
        CAN_Mode_Init(CAN1,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,10,mode);
	CAN_Mode_Init(USE_CAN,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,10,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������500Kbps    

	
 	while(1)
	{

	
                for(i=0;i<8;i++)
                {
                  canbuf_send[i]=cnt+i;//��䷢�ͻ�����
                  
//                  printf("%s",canbuf_send[i]);	//��ʾ����
                }
                
                res=Can_Send_Msg(USE_CAN,canbuf_send,8);//����8���ֽ� 
                if(res)printf("Failed");		//��ʾ����ʧ��
                else printf("OK    ");	 		//��ʾ���ͳɹ�								   
                
                can_rcv=Can_Receive_Msg(USE_CAN,canbuf_recv);
		if(can_rcv)//���յ�������
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
//                              printf("%s",canbuf_recv[i]);	//��ʾ����
 			}
		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED1=!LED1;//��ʾϵͳ��������	
			t=0;
			cnt++;
			printf("%d",cnt);	//��ʾ����
		}		   
	}
}

/*******************************************************************************
* Function Name  : can1_can2_test
* Description    : None
*                  
* Input          : None
*                  None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void can1_can2_test(void)
 {	 
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf_send[8],canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ

	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN��ʼ������ģʽ,������200Kbps    
        CAN_Mode_Init(CAN2,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN��ʼ������ģʽ,������200Kbps    

	
 	while(1)
	{
                //CAN1����
		if(mode==CAN_Mode_Normal)
		{
			for(i=0;i<8;i++)
			{
				canbuf_send[i]=cnt+i;//��䷢�ͻ�����

//				printf("%s",canbuf_send[i]);	//��ʾ����
 			}
			res=Can_Send_Msg(CAN1,canbuf_send,8);//����8���ֽ� 
			if(res)
                          printf("Failed");		//��ʾ����ʧ��
			else 
                          printf("OK    ");	 		//��ʾ���ͳɹ�								   
		}

                //CAN2����  
		can_rcv=Can_Receive_Msg(CAN2,canbuf_recv);
		if(can_rcv)//���յ�������
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
//                              printf("%s",canbuf_recv[i]);	//��ʾ����
 			}
		}
                
                
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED1=!LED1;//��ʾϵͳ��������	
			t=0;
			cnt++;
			printf("%d",cnt);	//��ʾ����
		}		   
	}
}


/*******************************************************************************
* Function Name  : input_can_task
* Description    : None
*                  
* Input          : None
*                  None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void input_test_task(void *arg)
 {	 
	u8 i=0,t=0;
	u8 canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
        u32 aa = 0;
	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,mode);//CAN��ʼ������ģʽ,������250Kbps    
//        CAN_Mode_Init(CAN2,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,mode);//CAN��ʼ������ģʽ,������250Kbps    

	
 	while(1)
	{
          
                aa++;
          
                  //CAN1����
          
                  Input_Check();
                  
                  if(aa == 100)
                  {
                  
                        res=Can_Send_Msg(CAN1,canbuf_send,2);//����2���ֽ� 
                        
                        if(res)
                          printf("Failed");		//��ʾ����ʧ��
                        else 
                          printf("OK    ");	 		//��ʾ���ͳɹ�	
                        
                        LED0=!LED0;
                        aa = 0;
                  }

                //CAN2����  
		can_rcv=Can_Receive_Msg(CAN1,canbuf_recv);
		if(can_rcv)//���յ�������
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
//                              printf("%s",canbuf_recv[i]);	//��ʾ����
 			}
		}
                
                
		t++; 
		delay_ms(10);
		if(t==50)
		{
			LED1=!LED1;//��ʾϵͳ��������	
                        EWDT_TOOGLE();
			t=0;
		}		   
	}
}


/*******************************************************************************
* Function Name  : rtc_clock_init
* Description    : None
*                  
* Input          : None
*                  None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void input_test_init(void)
{
	xTaskCreate(input_test_task, "INPUT_TEST", configMINIMAL_STACK_SIZE * 2, NULL, INPUT_TASK_PRIO, NULL);
}


/******************************  END OF FILE  *********************************/




