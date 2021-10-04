#ifndef I2C_H
#define I2C_H

#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int openI2CDevice(const char* device);
void setI2CSlave(int i2c_fd,int addr);			
void I2CWriteBytes(int i2c_fd, uint8_t* data, uint8_t length);

void sleep_ms(unsigned long milliseconds);
void sleep_us(unsigned long microseconds);
void sleep_ns(unsigned long nanoseconds);

#endif
