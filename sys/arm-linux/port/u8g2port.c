/*
 * This code supports multiple displays since GPIO pin handles have been
 * moved into user_data_struct. I2C and SPI handles are global since they can be
 * shared by multiple devices (think I2C with different address sharing bus).
 *
 * So far I have tested this with 3 software I2C displays. See examples.
 */

#include "u8g2port.h"

// c-periphery I2C handles
static i2c_t *i2c_handles[MAX_I2C_HANDLES] = { NULL };

// c-periphery SPI handles
static spi_t *spi_handles[MAX_SPI_HANDLES] = { NULL };

/*
 * Sleep milliseconds.
 */
void sleep_ms(unsigned long milliseconds) {
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

/*
 * Sleep microseconds.
 */
void sleep_us(unsigned long microseconds) {
	struct timespec ts;
	ts.tv_sec = microseconds / 1000 / 1000;
	ts.tv_nsec = (microseconds % 1000000) * 1000;
	nanosleep(&ts, NULL);
}

/**
 * Sleep nanoseconds.
 */
void sleep_ns(unsigned long nanoseconds) {
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = nanoseconds;
	nanosleep(&ts, NULL);
}

/*
 * Allocate user_data_struct, set common values and set user_ptr.
 */
user_data_t* init_user_data(u8g2_t *u8g2) {
	// Dynamically allocate user data_struct
	user_data_t *user_data = (user_data_t*) malloc(sizeof(user_data_t));
	// Dynamically allocate internal buffer
	user_data->int_buf = (uint8_t*)malloc(u8g2_GetBufferSize(u8g2));
	// We need a unique buffer for each display in order to be thread friendly
	u8g2_SetBufferPtr(u8g2, user_data->int_buf);
	for (int i = 0; i < U8X8_PIN_CNT; ++i) {
		user_data->pins[i] = NULL;
	}
	u8g2_SetUserPtr(u8g2, user_data);
	return user_data;
}

/*
 * Allocate user_data_struct for I2C hardware.
 */
void init_i2c_hw(u8g2_t *u8g2, uint8_t bus) {
	user_data_t *user_data = init_user_data(u8g2);
	user_data->bus = bus;
}

/*
 * Allocate user_data_struct for I2C software.
 */
void init_i2c_sw(u8g2_t *u8g2, uint8_t gpio_chip, uint8_t scl, uint8_t sda,
		uint8_t res, unsigned long delay) {
	user_data_t *user_data = init_user_data(u8g2);
	user_data->gpio_chip = gpio_chip;
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_I2C_CLOCK, scl);
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_I2C_DATA, sda);
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_RESET, res);
	user_data->delay = delay;
}

/*
 * Allocate user_data_struct for hardware SPI advanced.
 */
void init_spi_hw_advanced(u8g2_t *u8g2, uint8_t gpio_chip, uint8_t bus,
		uint8_t dc, uint8_t res, uint8_t cs, unsigned int spi_mode,
		uint32_t max_speed) {
	user_data_t *user_data = init_user_data(u8g2);
	user_data->gpio_chip = gpio_chip;
	user_data->bus = bus;
	user_data->spi_mode = spi_mode;
	user_data->max_speed = max_speed;
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_DC, dc);
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_RESET, res);
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_CS, cs);
}

/*
 * Allocate user_data_struct for hardware SPI.
 */
void init_spi_hw(u8g2_t *u8g2, uint8_t gpio_chip, uint8_t bus, uint8_t dc,
		uint8_t res, uint8_t cs) {
	init_spi_hw_advanced(u8g2, gpio_chip, bus, dc, res, cs,
			u8g2_GetU8x8(u8g2)->display_info->spi_mode, 500000);
}

/*
 * Allocate user_data_struct for hardware SPI.
 */
void init_spi_sw(u8g2_t *u8g2, uint8_t gpio_chip, uint8_t dc, uint8_t res,
		uint8_t mosi, uint8_t sck, uint8_t cs, unsigned long delay) {
	user_data_t *user_data = init_user_data(u8g2);
	user_data->gpio_chip = gpio_chip;
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_DC, dc);
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_RESET, res);
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_SPI_DATA, mosi);
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_SPI_CLOCK, sck);
	u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_CS, cs);
	user_data->delay = delay;
}

/*
 * Close GPIO pins and free user_data_struct.
 */
void done_user_data(u8g2_t *u8g2) {
	user_data_t *user_data = u8g2_GetUserPtr(u8g2);
	if (user_data != NULL) {
		// Close all GPIO pins
		for (int i = 0; i < U8X8_PIN_CNT; ++i) {
			if (user_data->pins[i] != NULL) {
				gpio_close(user_data->pins[i]);
				gpio_free(user_data->pins[i]);
			}
		}
		// Free internal buffer
		free(user_data->int_buf);
		// Free user data struct
		free(user_data);
		u8g2_SetUserPtr(u8g2, NULL);
	}
}

/**
 * Initialize pin if not set to U8X8_PIN_NONE and NULL.
 */
#if PERIPHERY_GPIO_CDEV_SUPPORT
void init_pin(u8x8_t *u8x8, uint8_t pin) {
	user_data_t *user_data = u8x8_GetUserPtr(u8x8);
	char filename[16];
	if (u8x8->pins[pin] != U8X8_PIN_NONE && user_data -> pins[pin] == NULL) {
		snprintf(filename, sizeof(filename), "/dev/gpiochip%d",	user_data ->gpio_chip);
		user_data -> pins[pin] = gpio_new();
		int error = gpio_open(user_data -> pins[pin], filename, u8x8->pins[pin], GPIO_DIR_OUT_HIGH);
		if (error < 0) {
			fprintf(stderr, "gpio_open(): pin %d, %s\n", u8x8->pins[pin],
					gpio_errmsg(user_data -> pins[pin]));
			gpio_free(user_data->pins[pin]);
		}
	}
}
#else
void init_pin(u8x8_t *u8x8, uint8_t pin) {
	user_data_t *user_data = u8x8_GetUserPtr(u8x8);
	if (u8x8->pins[pin] != U8X8_PIN_NONE && user_data->pins[pin] == NULL) {
		user_data->pins[pin] = gpio_new();
		int error = gpio_open_sysfs(user_data->pins[pin], u8x8->pins[pin],
				GPIO_DIR_OUT_HIGH);
		if (error < 0) {
			fprintf(stderr, "gpio_open_sysfs(): pin %d, %s\n", u8x8->pins[pin],
					gpio_errmsg(user_data->pins[pin]));
			gpio_free(user_data->pins[pin]);
		}
	}
}
#endif

/**
 * Write pin if not set to U8X8_PIN_NONE.
 */
void write_pin(u8x8_t *u8x8, uint8_t pin, uint8_t value) {
	user_data_t *user_data = u8x8_GetUserPtr(u8x8);
	if (u8x8->pins[pin] != U8X8_PIN_NONE) {
		gpio_write(user_data->pins[pin], value);
	}
}

/*
 * Initialize I2C bus.
 */
void init_i2c(u8x8_t *u8x8) {
	char filename[11];
	user_data_t *user_data = u8x8_GetUserPtr(u8x8);
	// Only open bus once
	if (i2c_handles[user_data->bus] == NULL) {
		snprintf(filename, sizeof(filename), "/dev/i2c-%d", user_data->bus);
		i2c_handles[user_data->bus] = i2c_new();
		int error = i2c_open(i2c_handles[user_data->bus], filename);
		if (error) {
			fprintf(stderr, "i2c_open(): %s\n",
					i2c_errmsg(i2c_handles[user_data->bus]));
			i2c_free(i2c_handles[user_data->bus]);
			i2c_handles[user_data->bus] = NULL;
		}
	}
}

/*
 * Close and free all i2c_t.
 */
void done_i2c() {
	for (int i = 0; i < MAX_I2C_HANDLES; ++i) {
		if (i2c_handles[i] != NULL) {
			i2c_close(i2c_handles[i]);
			i2c_free(i2c_handles[i]);
			i2c_handles[i] = NULL;
		}
	}
}

/*
 * Initialize SPI bus.
 */
void init_spi(u8x8_t *u8x8) {
	char filename[15];
	user_data_t *user_data = u8x8_GetUserPtr(u8x8);
	// Only open bus once
	if (spi_handles[user_data->bus] == NULL) {
		// user_data->bus should be in the format 0x12 for /dev/spidev1.2
		snprintf(filename, sizeof(filename), "/dev/spidev%d.%d",
				user_data->bus >> 4, user_data->bus & 0x0f);
		spi_handles[user_data->bus] = spi_new();
		/* SPI mode has to be mapped to the mode of the current controller, at least Uno, Due, 101 have different SPI_MODEx values */
		/*   0: clock active high, data out on falling edge, clock default value is zero, takover on rising edge */
		/*   1: clock active high, data out on rising edge, clock default value is zero, takover on falling edge */
		/*   2: clock active low, data out on rising edge */
		/*   3: clock active low, data out on falling edge */
		int error = spi_open(spi_handles[user_data->bus], filename,
				user_data->spi_mode, user_data->max_speed);
		if (error < 0) {
			fprintf(stderr, "spi_open(): %s\n",
					spi_errmsg(spi_handles[user_data->bus]));
			spi_free(spi_handles[user_data->bus]);
			spi_handles[user_data->bus] = NULL;
		}
	}
}

/*
 * Close and free all spi_t.
 */
void done_spi() {
	for (int i = 0; i < MAX_SPI_HANDLES; ++i) {
		if (spi_handles[i] != NULL) {
			spi_close(spi_handles[i]);
			spi_free(spi_handles[i]);
			spi_handles[i] = NULL;
		}
	}
}

/**
 * GPIO callback.
 */
uint8_t u8x8_arm_linux_gpio_and_delay(u8x8_t *u8x8, uint8_t msg,
		uint8_t arg_int, void *arg_ptr) {
	user_data_t *user_data;

	(void) arg_ptr; /* suppress unused parameter warning */
	switch (msg) {
	case U8X8_MSG_DELAY_NANO:
		// delay arg_int * 1 nano second or 0 for none
		user_data = u8x8_GetUserPtr(u8x8);
		if (user_data->delay != 0) {
			sleep_ns(user_data->delay);
		}
		break;

	case U8X8_MSG_DELAY_100NANO:
		// delay arg_int * 100 nano seconds
		sleep_ns(arg_int * 100);
		break;

	case U8X8_MSG_DELAY_10MICRO:
		// delay arg_int * 10 micro seconds
		sleep_us(arg_int * 10);
		break;

	case U8X8_MSG_DELAY_MILLI:
		// delay arg_int * 1 milli second
		sleep_ms(arg_int);
		break;

	case U8X8_MSG_DELAY_I2C:
		// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz, but we ignore
		// that and use user_data->delay
		user_data = u8x8_GetUserPtr(u8x8);
		if (user_data->delay != 0) {
			sleep_ns(user_data->delay);
		}
		break;

	case U8X8_MSG_GPIO_AND_DELAY_INIT:
		// Function which implements a delay, arg_int contains the amount of ms

		// SPI Pins
		init_pin(u8x8, U8X8_PIN_SPI_CLOCK);
		init_pin(u8x8, U8X8_PIN_SPI_DATA);
		init_pin(u8x8, U8X8_PIN_CS);
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

	case U8X8_MSG_GPIO_D2:
		// D2 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D2, arg_int);
		break;

	case U8X8_MSG_GPIO_D3:
		// D3 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D3, arg_int);
		break;

	case U8X8_MSG_GPIO_D4:
		// D4 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D4, arg_int);
		break;

	case U8X8_MSG_GPIO_D5:
		// D5 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D5, arg_int);
		break;

	case U8X8_MSG_GPIO_D6:
		// D6 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D6, arg_int);
		break;

	case U8X8_MSG_GPIO_D7:
		// D7 pin: Output level in arg_int
		write_pin(u8x8, U8X8_PIN_D7, arg_int);
		break;

	case U8X8_MSG_GPIO_E:
		// E/WR pin: Output level in arg_int
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

	case U8X8_MSG_GPIO_SPI_CLOCK: // Same as U8X8_MSG_GPIO_D0
		//Function to define the logic level of the clockline
		write_pin(u8x8, U8X8_PIN_SPI_CLOCK, arg_int);
		break;

	case U8X8_MSG_GPIO_SPI_DATA: // Same as U8X8_MSG_GPIO_D1
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

/*
 * I2c callback.
 */
uint8_t u8x8_byte_arm_linux_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr) {
	user_data_t *user_data;
	uint8_t *data;
	struct i2c_msg msgs[1];

	switch (msg) {
	case U8X8_MSG_BYTE_SEND:
		user_data = u8x8_GetUserPtr(u8x8);
		data = (uint8_t*) arg_ptr;
		while (arg_int > 0) {
			user_data->buffer[user_data->index++] = *data;
			data++;
			arg_int--;
		}
		break;

	case U8X8_MSG_BYTE_INIT:
		init_i2c(u8x8);
		break;

	case U8X8_MSG_BYTE_START_TRANSFER:
		user_data = u8x8_GetUserPtr(u8x8);
		user_data->index = 0;
		break;

	case U8X8_MSG_BYTE_END_TRANSFER:
		user_data = u8x8_GetUserPtr(u8x8);
		msgs[0].addr = u8x8_GetI2CAddress(u8x8) >> 1;
		msgs[0].flags = 0; // Write
		msgs[0].len = user_data->index;
		msgs[0].buf = user_data->buffer;
		i2c_transfer(i2c_handles[user_data->bus], msgs, 1);
		break;

	default:
		return 0;
	}
	return 1;
}

/*
 * SPI callback.
 */
uint8_t u8x8_byte_arm_linux_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr) {
	user_data_t *user_data;
	uint8_t *data;

	switch (msg) {
	case U8X8_MSG_BYTE_SEND:
		user_data = u8x8_GetUserPtr(u8x8);
		user_data->index = 0;
		data = (uint8_t*) arg_ptr;
		while (arg_int > 0) {
			user_data->buffer[user_data->index++] = *data;
			data++;
			arg_int--;
		}
		spi_transfer(spi_handles[user_data->bus], user_data->buffer,
				user_data->buffer, user_data->index);
		break;

	case U8X8_MSG_BYTE_INIT:
		init_spi(u8x8);
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
