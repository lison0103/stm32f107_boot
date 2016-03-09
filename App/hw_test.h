#ifndef __HW_TEST_H
#define __HW_TEST_H	

#include "lsys.h"
#include "bsp_iocfg.h"

extern u8 canbuf_send[8];

void HW_TEST(void);
void can_test(void);
void can1_can2_test(void);
void hw_can_test(void);



#endif