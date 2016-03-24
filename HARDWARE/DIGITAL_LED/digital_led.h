/*******************************************************************************
* File Name          : digital_led.h
* Author             : lison
* Version            : V1.0
* Date               : 03/23/2016
* Description        : 
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIGITAL_LED_H
#define __DIGITAL_LED_H	

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void led_display(void);
void digital_led_gpio_init(void);
void digital_led_check(void);

extern u8 dis_data[3];



#endif /* __DIGITAL_LED_H */


/******************************  END OF FILE  *********************************/





