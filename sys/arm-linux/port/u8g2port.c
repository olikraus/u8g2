/*
 * This code is not thread safe, however you should be able to use one IC2 and
 * one SPI device at the same time. This should be reworked to make multiple
 * displays work in a thread safe way.
 */

#include "u8g2port.h"

// c-periphery i2c handle
static i2c_t *i2c_device;
static const char i2c_bus[] = "/dev/i2c-0";

static spi_t *spi_device;
static const char spi_bus[] = "/dev/spidev1.0";

#if PERIPHERY_GPIO_CDEV_SUPPORT
static const char gpio_device[] = "/dev/gpiochip0";
#endif

// c-periphery GPIO pins
gpio_t *pins[U8X8_PIN_CNT] = { };

void sleep_ms(unsigned long milliseconds) {
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

void sleep_us(unsigned long microseconds) {
	struct timespec ts;
	ts.tv_sec = microseconds / 1000 / 1000;
	ts.tv_nsec = (microseconds % 1000000) * 1000;
	nanosleep(&ts, NULL);
}

void sleep_ns(unsigned long nanoseconds) {
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = nanoseconds;
	nanosleep(&ts, NULL);
}

/**
 * Initialize pin if not set to U8X8_PIN_NONE.
 */
void init_pin(u8x8_t *u8x8, int pin) {
	if (u8x8->pins[pin] != U8X8_PIN_NONE) {
		pins[pin] = gpio_new();
// Support character device
#if PERIPHERY_GPIO_CDEV_SUPPORT
	if (gpio_open(pins[pin], gpio_device, u8x8->pins[pin], GPIO_DIR_OUT_HIGH)
			< 0) {
		fprintf(stderr, "gpio_open(): pin %d, %s\n", u8x8->pins[pin], gpio_errmsg(pins[pin]));
	}
// Support deprecated sysfs
#else
		if (gpio_open_sysfs(pins[pin], u8x8->pins[pin], GPIO_DIR_OUT_HIGH)
				< 0) {
			fprintf(stderr, "gpio_open_sysfs(): pin %d, %s\n", u8x8->pins[pin],
					gpio_errmsg(pins[pin]));
		}
#endif
	}
}

/*
 * Close and free gpio_t.
 */
void done_pins() {
	for (int i = 0; i < U8X8_PIN_CNT; ++i) {
		if (pins[i] != NULL) {
			gpio_close(pins[i]);
			gpio_free(pins[i]);
		}
	}
}

void done_i2c() {
	i2c_close(i2c_device);
	i2c_free(i2c_device);
}

void done_spi() {
	spi_close(spi_device);
	spi_free(spi_device);
}

/**
 * Write pin if not set to U8X8_PIN_NONE.
 */
void write_pin(u8x8_t *u8x8, int pin, int value) {
	if (u8x8->pins[pin] != U8X8_PIN_NONE) {
		gpio_write(pins[pin], value);
	}
}

uint8_t u8x8_arm_linux_gpio_and_delay(u8x8_t *u8x8, uint8_t msg,
		uint8_t arg_int, void *arg_ptr) {
	(void) arg_ptr; /* suppress unused parameter warning */
	switch (msg) {
	case U8X8_MSG_DELAY_NANO:            // delay arg_int * 1 nano second
		sleep_ns(arg_int);
		break;

	case U8X8_MSG_DELAY_100NANO:        // delay arg_int * 100 nano seconds
		sleep_ns(arg_int * 100);
		break;

	case U8X8_MSG_DELAY_10MICRO:        // delay arg_int * 10 micro seconds
		sleep_us(arg_int * 10);
		break;

	case U8X8_MSG_DELAY_MILLI:            // delay arg_int * 1 milli second
		sleep_ms(arg_int);
		break;

	case U8X8_MSG_DELAY_I2C:
		// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
		// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
		if (arg_int == 1) {
			sleep_us(5);
		} else if (arg_int == 4) {
			sleep_ns(1250);
		}
		break;

	case U8X8_MSG_GPIO_AND_DELAY_INIT:
		// Function which implements a delay, arg_int contains the amount of ms

		// SPI Pins
		init_pin(u8x8, U8X8_PIN_SPI_CLOCK);
		init_pin(u8x8, U8X8_PIN_SPI_DATA);
		init_pin(u8x8, U8X8_PIN_CS);

		// 8080 mode
		// D0 --> spi clock
		// D1 --> spi data
		init_pin(u8x8, U8X8_PIN_D2);
		init_pin(u8x8, U8X8_PIN_D3);
		init_pin(u8x8, U8X8_PIN_D4);
		init_pin(u8x8, U8X8_PIN_D5);
		init_pin(u8x8, U8X8_PIN_D6);
		init_pin(u8x8, U8X8_PIN_D7);
		init_pin(u8x8, U8X8_PIN_E);
		init_pin(u8x8, U8X8_PIN_RESET);
		init_pin(u8x8, U8X8_PIN_DC);

		// I2c pins
		init_pin(u8x8, U8X8_PIN_I2C_DATA);
		init_pin(u8x8, U8X8_PIN_I2C_CLOCK);

		break;

		//case U8X8_MSG_GPIO_D0:                // D0 or SPI clock pin: Output level in arg_int
		//case U8X8_MSG_GPIO_SPI_CLOCK:

		//case U8X8_MSG_GPIO_D1:                // D1 or SPI data pin: Output level in arg_int
		//case U8X8_MSG_GPIO_SPI_DATA:

	case U8X8_MSG_GPIO_D2:                  // D2 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D2, arg_int);
		break;

	case U8X8_MSG_GPIO_D3:                  // D3 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D3, arg_int);
		break;

	case U8X8_MSG_GPIO_D4:                  // D4 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D4, arg_int);
		break;

	case U8X8_MSG_GPIO_D5:                  // D5 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D5, arg_int);
		break;

	case U8X8_MSG_GPIO_D6:                  // D6 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D6, arg_int);
		break;

	case U8X8_MSG_GPIO_D7:                  // D7 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D7, arg_int);
		break;

	case U8X8_MSG_GPIO_E:                   // E/WR pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_E, arg_int);
		break;

	case U8X8_MSG_GPIO_I2C_CLOCK:
		// arg_int=0: Output low at I2C clock pin
		// arg_int=1: Input dir with pullup high for I2C clock pin
		write_pin(u8x8, U8X8_PIN_I2C_CLOCK, arg_int);
		break;

	case U8X8_MSG_GPIO_I2C_DATA:
		// arg_int=0: Output low at I2C data pin
		// arg_int=1: Input dir with pullup high for I2C data pin
		write_pin(u8x8, U8X8_PIN_I2C_DATA, arg_int);
		break;

	case U8X8_MSG_GPIO_SPI_CLOCK:
		//Function to define the logic level of the clockline
		write_pin(u8x8, U8X8_PIN_SPI_CLOCK, arg_int);
		break;

	case U8X8_MSG_GPIO_SPI_DATA:
		//Function to define the logic level of the data line to the display
		write_pin(u8x8, U8X8_PIN_SPI_DATA, arg_int);
		break;

	case U8X8_MSG_GPIO_CS:
		// Function to define the logic level of the CS line
		write_pin(u8x8, U8X8_PIN_CS, arg_int);
		break;

	case U8X8_MSG_GPIO_DC:
		//Function to define the logic level of the Data/ Command line
		write_pin(u8x8, U8X8_PIN_DC, arg_int);
		break;

	case U8X8_MSG_GPIO_RESET:
		//Function to define the logic level of the RESET line
		write_pin(u8x8, U8X8_PIN_RESET, arg_int);
		break;

	default:
		return 0;
	}
	return 1;
}

uint8_t u8x8_byte_arm_linux_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr) {
	/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
	static uint8_t buffer[32];
	static uint8_t buf_idx;
	uint8_t *data;
	struct i2c_msg msgs[1];

	switch (msg) {
	case U8X8_MSG_BYTE_SEND:
		data = (uint8_t*) arg_ptr;
		while (arg_int > 0) {
			buffer[buf_idx++] = *data;
			data++;
			arg_int--;
		}
		break;

	case U8X8_MSG_BYTE_INIT:
		i2c_device = i2c_new();
		if (i2c_open(i2c_device, i2c_bus) < 0) {
			fprintf(stderr, "i2c_open(): %s\n", i2c_errmsg(i2c_device));
		}
		break;

	case U8X8_MSG_BYTE_START_TRANSFER:
		buf_idx = 0;
		break;

	case U8X8_MSG_BYTE_END_TRANSFER:
		msgs[0].addr = u8x8_GetI2CAddress(u8x8) >> 1;
		;
		msgs[0].flags = 0; // Write
		msgs[0].len = buf_idx;
		msgs[0].buf = buffer;
		i2c_transfer(i2c_device, msgs, 1);
		break;

	default:
		return 0;
	}
	return 1;
}

uint8_t u8x8_byte_arm_linux_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr) {
	// u8g2/u8x8 will never send more than 128 bytes
	static uint8_t buffer[132];
	uint8_t buf_idx;
	uint8_t *data;

	switch (msg) {
	case U8X8_MSG_BYTE_SEND:
		buf_idx = 0;
		data = (uint8_t*) arg_ptr;
		while (arg_int > 0) {
			buffer[buf_idx++] = *data;
			data++;
			arg_int--;
		}
		spi_transfer(spi_device, buffer, buffer, buf_idx);
		break;

	case U8X8_MSG_BYTE_INIT:
		spi_device = spi_new();
		//u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
		/* SPI mode has to be mapped to the mode of the current controller, at least Uno, Due, 101 have different SPI_MODEx values */
		/*   0: clock active high, data out on falling edge, clock default value is zero, takover on rising edge */
		/*   1: clock active high, data out on rising edge, clock default value is zero, takover on falling edge */
		/*   2: clock active low, data out on rising edge */
		/*   3: clock active low, data out on falling edge */
		if (spi_open(spi_device, spi_bus, u8x8->display_info->spi_mode, 500000)
				< 0) {
			fprintf(stderr, "spi_open(): %s\n", spi_errmsg(spi_device));
		}
		break;

	case U8X8_MSG_BYTE_SET_DC:
		u8x8_gpio_SetDC(u8x8, arg_int);
		break;

	case U8X8_MSG_BYTE_START_TRANSFER:
		break;

	case U8X8_MSG_BYTE_END_TRANSFER:
		break;

	default:
		return 0;
	}
	return 1;
}
