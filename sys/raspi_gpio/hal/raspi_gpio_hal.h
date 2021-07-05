/*

  raspi_gpio_hal.h

  This could needs to be compiled with -DU8X8_USE_PINS 

*/


#ifndef raspi_gpio_hal_h
#define raspi_gpio_hal_h

#include "u8x8.h"

void delaynanoseconds(unsigned long ns);
int gpio_export(int pin);
int gpio_direction(int pin, int dir);
int gpio_write(int pin, int value);
uint8_t u8x8_gpio_and_delay_raspi_gpio_hal(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif /* raspi_gpio_hal_h */
