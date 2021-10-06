#include "i2c.h"

int openI2CDevice(const char* device)
{
    int i2c_fd;
    char filename[40];
    sprintf(filename, device);
    if ((i2c_fd = open(filename,O_RDWR)) < 0) 
	{
        printf("Failed to open the bus.");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        return -1;
    }
    return i2c_fd;
}

void setI2CSlave(int i2c_fd,int addr)
{    
    if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0) 
	{
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        // exit(1);
    }
}

void I2CWriteBytes(int i2c_fd, uint8_t* data, uint8_t length)
{
    if (write(i2c_fd, data, length) != length) 
	{
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
    }
}

void sleep_ms(unsigned long milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void sleep_us(unsigned long microseconds)
{
    struct timespec ts;
    ts.tv_sec = microseconds / 1000 / 1000;
    ts.tv_nsec = (microseconds % 1000000) * 1000;
    nanosleep(&ts, NULL);
}

void sleep_ns(unsigned long nanoseconds)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = nanoseconds;
    nanosleep(&ts, NULL);
}
