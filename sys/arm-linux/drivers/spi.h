#ifndef SPI_H
#define SPI_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

void pabort(const char *s);

int openSPIDevice(const char* device, uint8_t mode, uint8_t bits, uint32_t speed);
int SPITransfer(int fd, struct spi_ioc_transfer* tr);
void closeSPIDevice(int fd);

#endif
