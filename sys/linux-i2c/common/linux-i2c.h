#ifndef _LINUX_I2C_H
#define _LINUX_I2C_H	1




#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <u8x8.h>
#include <unistd.h>


uint8_t u8x8_byte_linux_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

uint8_t u8x8_linux_i2c_delay (u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) ;


#endif // LINUXi2c
