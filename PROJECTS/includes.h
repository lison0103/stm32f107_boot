/*******************************************************************************
* File Name          : includes.h
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Last modify date   : 10/09/2016
* Description        : This file contains includes file and define.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __INCLUDES_H
#define  __INCLUDES_H

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"
#include "delay.h"  
#include "usart.h"   
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
#include "iap.h"
#include "rtc.h"
#include "usb_task.h"
#include "sys_task.h"
#include "comm_display_board.h"
#ifdef GEC_CB_MAIN
#include "hw_test.h"
#include "can.h"
#include "freertos_lwip.h" 
#include "event_groups.h"
#include "modbusTask.h"
#include "esc_eeprom_process.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifdef GEC_CB_MAIN

#define USE_FREERTOS_OS        1

/* number is big, priority is big */
#define LED_TASK_PRIO			( tskIDLE_PRIORITY + 9 )
#define USB_THREAD_PRIO                 ( tskIDLE_PRIORITY + 7 )
#define RX485_THREAD_PRIO               ( tskIDLE_PRIORITY + 2 )
#define RTC_THREAD_PRIO                 ( tskIDLE_PRIORITY + 2 )
#define TCP_TASK_PRIO			( tskIDLE_PRIORITY + 5 )
#define DHCP_TASK_PRIO                  ( tskIDLE_PRIORITY + 8 ) 
#define ESC_TASK_PRIO                   ( tskIDLE_PRIORITY + 7 )
#define CAN_THREAD_PRIO                 ( tskIDLE_PRIORITY + 6 )
#define EEPROM_THREAD_PRIO              ( tskIDLE_PRIORITY + 5 )
#define COMM_DB_THREAD_PRIO             ( tskIDLE_PRIORITY + 4 )

/*************************  TEST    *******************************************/
#define ETH_LWIP_ENABLE    0

#define TCP_CLIENT_TEST    0

#define TCP_SERVER_TEST    0

#define MODBUS_TCP_ENABLE    1

#define MODBUS_RTU_ENABLE    0

#define USB_HOST_MASS_STORAGE_TEST      0

#define RX485_TEST      0

#define RTC_CLOCK_ENABLE          1

#define MODBUS_LOCAL_DDU_ENABLE   0

#define COMM_DISPLAY_BOARD_ENABLE   0

/*************************  TEST    *******************************************/
#define DEBUG_PRINTF                0


#define TASK_BIT_0      (1u << 0u)
#define TASK_BIT_1      (1u << 1u)
#define TASK_BIT_2      (1u << 2u)
#define TASK_BIT_3      (1u << 3u)
#define TASK_BIT_4      (1u << 4u)
#define TASK_BIT_5      (1u << 5u)
#define TASK_BIT_ALL    (TASK_BIT_0|TASK_BIT_1|TASK_BIT_2)

#endif /* GEC_CB_MAIN */

/* Exported functions ------------------------------------------------------- */

#ifdef GEC_CB_MAIN
extern QueueHandle_t xQueue;
extern u8 testmode;
extern u8 timeset;
extern EventGroupHandle_t xCreatedEventGroup;
#endif

#endif /* __INCLUDES_H */


/******************************  END OF FILE  *********************************/   

