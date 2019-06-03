#ifndef U8X8_AVR_H_
#define U8X8_AVR_H_

#include <u8g2.h>
#include <stdint.h>

uint8_t u8x8_byte_avr_hw_spi(u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_avr_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_avr_delay(u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif /* U8X8_AVR_H_ */
