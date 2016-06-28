/*******************************************************************************
* File Name          : hw_test.h
* Author             : lison
* Version            : V1.0
* Date               : 03/24/2016
* Description        : Contains prototype of some test function.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_TEST_H
#define __HW_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "lsys.h"
#include "bsp_iocfg.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Input_Check(void);
void input_test_init(void);
void can_comm_test_init(void);

extern u8 canbuf_send[8];


#endif /* __HW_TEST_H */


/******************************  END OF FILE  *********************************/
	

