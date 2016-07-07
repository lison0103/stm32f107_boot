/*******************************************************************************
* File Name          : rtc.c
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains stm rtc funcitons.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "lsys.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Month correction data table */										 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};   
/* Non-leap year month date table */
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

_calendar_obj calendar;



/*******************************************************************************
* Function Name  : RTC_NVIC_Config
* Description    : Configuring rtc interrupt group.      
*                  
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/
static void RTC_NVIC_Config(void)
{	
        NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);		
}


/*******************************************************************************
* Function Name  : RTC_Init
* Description    : Real Time Clock Configuration
*                  Initialize the RTC clock, while clock is working properly detected
*                  BKP-> DR1 for the first time whether to save the configuration settings
* Input          : None
*                  
* Output         : None
* Return         : 0: Normal Other: Error code
*******************************************************************************/
u8 RTC_Init(void)
{
	
	u8 temp=0;
 
	if (BKP_ReadBackupRegister(BKP_DR42) != 0x5050)		
        {	 			
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	
            PWR_BackupAccessCmd(ENABLE);	
            BKP_DeInit();		
            RCC_LSEConfig(RCC_LSE_ON);	
            while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
            {
                temp++;
                delay_ms(10);
            }
            if(temp>=250)return 1;   
            RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		 
            RCC_RTCCLKCmd(ENABLE);	 
            RTC_WaitForLastTask();	
            RTC_WaitForSynchro();		
            RTC_ITConfig(RTC_IT_SEC, ENABLE);		
            RTC_WaitForLastTask();	
            RTC_EnterConfigMode();
            RTC_SetPrescaler(32776);//32767
            RTC_WaitForLastTask();	
            RTC_Set(2016,7,7,9,10,00); 
            RTC_ExitConfigMode();  
            BKP_WriteBackupRegister(BKP_DR42, 0x5050);
        }
	else
        {
          
            RTC_WaitForSynchro();	
            RTC_ITConfig(RTC_IT_SEC, ENABLE);	
            RTC_WaitForLastTask();	
        }
	RTC_NVIC_Config();	    				     
	RTC_Get();	
        
	return 0; 

}	


/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : RTC clock interrupt
*                  Trigger once per second
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{							
		RTC_Get();  
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		 	   
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		
	RTC_WaitForLastTask();	  	    						 	   	 
}


/*******************************************************************************
* Function Name  : Is_Leap_Year
* Description    : determine whether it is a leap year
*                  Month   1  2  3  4  5  6  7  8  9  10 11 12
*                  Leap year   31 29 31 30 31 30 31 31 30 31 30 31
*                  Non-leap year 31 28 31 30 31 30 31 31 30 31 30 31                
* Input          : Year
*                  
* Output         : None
* Return         : The year is not a leap year  1: yes  0: not
*******************************************************************************/
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) 
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 


/*******************************************************************************
* Function Name  : RTC_Set
* Description    : Setting the clock
*                  The input clock to the second to January 1, 1970 as the base, from 1970 to 2099 as a legitimate Year
* Input          : None
*                  
* Output         : None
* Return         : 0: successful; Others: error code.
*******************************************************************************/
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	
	{
		if(Is_Leap_Year(t))seccount+=31622400;
		else seccount+=31536000;			 
	}
	smon-=1;
	for(t=0;t<smon;t++)	 
	{
		seccount+=(u32)mon_table[t]*86400;
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;	   
	}
	seccount+=(u32)(sday-1)*86400;
	seccount+=(u32)hour*3600;
        seccount+=(u32)min*60;	 
	seccount+=sec;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	 
	PWR_BackupAccessCmd(ENABLE);	
	RTC_SetCounter(seccount);	

	RTC_WaitForLastTask();	
	RTC_Get();
        
	return 0;	    
}



/*******************************************************************************
* Function Name  : RTC_Get
* Description    : Get the current clock
*                  
* Input          : None
*                  
* Output         : None
* Return         : 0: successful; Others: error code.
*******************************************************************************/
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	
        
        timecount=RTC_GetCounter();	 
 	temp=timecount/86400;  
	if(daycnt!=temp)
	{	  
		daycnt=temp;
		temp1=1970;	
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))
			{
				if(temp>=366)temp-=366;
				else {temp1++;break;}  
			}
			else temp-=365;	 
			temp1++;  
		}   
		calendar.w_year=temp1;
		temp1=0;
		while(temp>=28)
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)
			{
				if(temp>=29)temp-=29;
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	
		calendar.w_date=temp+1;  	
	}
	temp=timecount%86400;     		   
	calendar.hour=temp/3600;     
	calendar.min=(temp%3600)/60; 	
	calendar.sec=(temp%3600)%60; 	
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);
        
	return 0;
}	



/*******************************************************************************
* Function Name  : RTC_Get_Week
* Description    : Now is the day of the week to get
*                  Enter Gregorian date obtained week (only allowed 1901- 2099)
* Input          : calendar date
*                  
* Output         : None
* Return         : Week No.
*******************************************************************************/																					 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 

	if (yearH>19)yearL+=100;

	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
        
	return(temp2%7);
}	


/*******************************************************************************
* Function Name  : RTC_GetTime
* Description    : Get the current time
*                  
* Input          : None
*                  
* Output         : None
* Return         : 0: successful; Others: error code.
*******************************************************************************/
u8 RTC_GetTime( unsigned char *pcBuff )
{
      static u8 read_cnt=0,second_old=0xff,rtcc_valid=0; 
    
      RTC_Get();
      pcBuff[0] = (calendar.w_year)%100;
      pcBuff[1] = calendar.w_month;
      pcBuff[2] = calendar.w_date;
      pcBuff[3] = calendar.hour;
      pcBuff[4] = calendar.min;
      pcBuff[5] = calendar.sec;
      
      if(read_cnt++>15)
      {
          read_cnt = 0;
          
          if((second_old != pcBuff[5]) && (pcBuff[0]>=16))
          {
              rtcc_valid = 1;
          }
          else
          {
              rtcc_valid = 0;
          }  
          
          second_old = pcBuff[5]; 
      }   
      
      return(rtcc_valid);
}

/*******************************************************************************
* Function Name  : RTC_SetTime
* Description    : Set the time
*                  The input clock to the second to January 1, 1970 as the base, from 1970 to 2099 as a legitimate Year
* Input          : None
*                  
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_SetTime( unsigned char *pcBuff )
{
    RTC_Set( pcBuff[0], pcBuff[1], pcBuff[2], pcBuff[3], pcBuff[4], pcBuff[5] );    
}

/******************************  END OF FILE  *********************************/














