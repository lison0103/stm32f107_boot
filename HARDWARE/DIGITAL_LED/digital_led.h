#ifndef __DIGITAL_LED_H
#define __DIGITAL_LED_H	

#include "lsys.h"

#define LED_NUM1        PCout(8)
#define LED_NUM2        PCout(7)
#define LED_NUM3        PCout(6)

#define LED_SCLK        PDout(12)
#define LED_RCLK        PDout(13)
#define LED_OE          PDout(14)
#define LED_SDI         PDout(15)

extern u8 dis_data[3];

void led_display(void);
void digital_led_gpio_init(void);
void digital_led_check(void);

#endif