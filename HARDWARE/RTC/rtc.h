/*******************************************************************************
* File Name          : rtc.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : This file contains prototype of stm rtc funcitons.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H	

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Calendar structure*/
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	

/* Exported constants --------------------------------------------------------*/
extern u8 const mon_table[12];	

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Disp_Time(u8 x,u8 y,u8 size);
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);
u8 RTC_Init(void);        
u8 Is_Leap_Year(u16 year);
u8 RTC_Get(void);          
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_GetTime( unsigned char *pcBuff );
void RTC_SetTime( unsigned char *pcBuff );

#endif /* __RTC_H */


/******************************  END OF FILE  *********************************/

    




