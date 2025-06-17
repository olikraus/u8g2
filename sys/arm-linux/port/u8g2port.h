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
#include <stdlib.h>

#define MAX_I2C_HANDLES 8
#define MAX_SPI_HANDLES 256

/*
 * User data passed in user_ptr of u8x8_struct.
 */
struct user_data_struct {
	// GPIO chip /dev/gpiochip%d
	uint8_t gpio_chip;
	// c-periphery GPIO handles
	gpio_t *pins[U8X8_PIN_CNT];
	// For I2C /dev/i2c-%d and for SPI /dev/spidev%d.%d using high and low 4 bits
	uint8_t bus;
	// Index into buffer
	uint8_t index;
	// Callback buffer, I2C should send 32 bytes max and SPI 128 bytes max
	uint8_t buffer[256];  // issue 2666
	// Nanosecond delay for U8X8_MSG_DELAY_I2C
	unsigned long delay;
	// SPI mode
	unsigned int spi_mode;
	// SPI max speed
	uint32_t max_speed;
	// Internal buffer
	uint8_t *int_buf;
};

typedef struct user_data_struct user_data_t;

void sleep_ms(unsigned long milliseconds);
void sleep_us(unsigned long microseconds);
void sleep_ns(unsigned long nanoseconds);
user_data_t *init_user_data(u8g2_t *u8g2);
void init_i2c_hw(u8g2_t *u8g2, uint8_t bus);
void init_i2c_sw(u8g2_t *u8g2, uint8_t gpio_chip, uint8_t scl, uint8_t sda,
		uint8_t res, unsigned long delay);
void init_spi_hw_advanced(u8g2_t *u8g2, uint8_t gpio_chip, uint8_t bus, uint8_t dc,
		uint8_t res, uint8_t cs, unsigned int spi_mode, uint32_t max_speed);
void init_spi_hw(u8g2_t *u8g2, uint8_t gpio_chip, uint8_t bus, uint8_t dc,
		uint8_t res, uint8_t cs);
void init_spi_sw(u8g2_t *u8g2, uint8_t gpio_chip, uint8_t dc, uint8_t res,
		uint8_t mosi, uint8_t sck, uint8_t cs, unsigned long delay);
void done_user_data(u8g2_t *u8g2);
void init_pin(u8x8_t *u8x8, uint8_t pin);
void write_pin(u8x8_t *u8x8, uint8_t pin, uint8_t value);
void init_i2c(u8x8_t *u8x8);
void done_i2c();
void init_spi(u8x8_t *u8x8);
void done_spi();
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
