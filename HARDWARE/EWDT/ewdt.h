
#ifndef __BSP_CHECK_H
#define __BSP_CHECK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define EWDT_TOOGLE() 	GPIOC->ODR ^= 0X0001

void power_on_bsp_check(void);
void EWDT_Drv_pin_config(void);


#endif //__BSP_CHECK_H