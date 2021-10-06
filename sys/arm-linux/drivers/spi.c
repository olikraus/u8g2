#include "spi.h"

void pabort(const char *s)
{
    perror(s);
    abort();
}

int openSPIDevice(const char* device, uint8_t mode, uint8_t bits, uint32_t speed)
{
    int ret;
    int fd = open(device, O_RDWR);
    if (fd < 0)
    {
        pabort("can't open device");
    }

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
    {
        pabort("can't set spi mode");
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
    {
        pabort("can't get spi mode");
    }

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        pabort("can't set bits per word");
    }

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        pabort("can't get bits per word");
    }

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        pabort("can't set max speed hz");
    }

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        pabort("can't get max speed hz");
    }

    // printf("spi mode: %d\n", mode);
    // printf("bits per word: %d\n", bits);
    // printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

    return fd;
}

int SPITransfer(int fd, struct spi_ioc_transfer* tr)
{
    int ret;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), tr);
    if (ret < 1)
    {
        pabort("can't send spi message");
    }

    return 0;
}

void closeSPIDevice(int fd)
{
    close(fd);
}
