#ifndef U8G2LIB_H
#define U8G2LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <u8g2.h>
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include <time.h>
#include <stdio.h>

void sleep_ms(unsigned long milliseconds);
void sleep_us(unsigned long microseconds);
void sleep_ns(unsigned long nanoseconds);
void init_pin(u8x8_t *u8x8, int pin);
void done_pins();
void done_i2c();
void done_spi();
void write_pin(u8x8_t *u8x8, int pin, int value);
uint8_t u8x8_arm_linux_gpio_and_delay(u8x8_t *u8x8, uint8_t msg,
		uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_arm_linux_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr);
uint8_t u8x8_byte_arm_linux_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr);

#ifdef __cplusplus
}
#endif

#endif
