
#include "u8x8.h"


void u8x8_gpio_call(u8x8_t *u8x8, uint8_t msg, uint8_t arg)
{
  u8x8->gpio_and_delay_cb(u8x8, msg, arg, NULL);
}

/*
void u8x8_gpio_Delay(u8x8_t *u8x8, uint8_t msg, uint8_t dly)
{
  u8x8->gpio_and_delay_cb(u8x8, msg, dly, NULL);
}
*/