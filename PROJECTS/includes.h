#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#ifdef GEC_CB_MAIN


/*************************  TEST    *******************************************/
#define TCP_SERVER_TEST    0

#define MODBUS_TCP_TEST    0

#define MODBUS_RTU_TEST    0

#define USB_HOST_MASS_STORAGE_TEST      0

#define INPUT_RELAY_OUTPUT_AND_CAN_TEST    0

#define RX485_TEST      0

#define RTC_CLOCK_TEST          1
/*************************  TEST    *******************************************/

#if RTC_CLOCK_TEST
#define DEBUG_PRINTF                1
#else
#define DEBUG_PRINTF                0
#endif

#else



#endif



#endif