/*
 * c-periphery
 * https://github.com/vsergeev/c-periphery
 * License: MIT
 */

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"

struct spi_handle {
    int fd;

    struct {
        int c_errno;
        char errmsg[96];
    } error;
};

static int _spi_error(spi_t *spi, int code, int c_errno, const char *fmt, ...) {
    va_list ap;

    spi->error.c_errno = c_errno;

    va_start(ap, fmt);
    vsnprintf(spi->error.errmsg, sizeof(spi->error.errmsg), fmt, ap);
    va_end(ap);

    /* Tack on strerror() and errno */
    if (c_errno) {
        char buf[64];
        strerror_r(c_errno, buf, sizeof(buf));
        snprintf(spi->error.errmsg+strlen(spi->error.errmsg), sizeof(spi->error.errmsg)-strlen(spi->error.errmsg), ": %s [errno %d]", buf, c_errno);
    }

    return code;
}

spi_t *spi_new(void) {
    spi_t *spi = calloc(1, sizeof(spi_t));
    if (spi == NULL)
        return NULL;

    spi->fd = -1;

    return spi;
}

void spi_free(spi_t *spi) {
    free(spi);
}

int spi_open(spi_t *spi, const char *path, unsigned int mode, uint32_t max_speed) {
    return spi_open_advanced(spi, path, mode, max_speed, MSB_FIRST, 8, 0);
}

int spi_open_advanced(spi_t *spi, const char *path, unsigned int mode, uint32_t max_speed, spi_bit_order_t bit_order, uint8_t bits_per_word, uint8_t extra_flags) {
    return spi_open_advanced2(spi, path, mode, max_speed, bit_order, bits_per_word, extra_flags);
}

int spi_open_advanced2(spi_t *spi, const char *path, unsigned int mode, uint32_t max_speed, spi_bit_order_t bit_order, uint8_t bits_per_word, uint32_t extra_flags) {
    uint32_t data32;
    uint8_t data8;

    /* Validate arguments */
    if (mode & ~0x3)
        return _spi_error(spi, SPI_ERROR_ARG, 0, "Invalid mode (can be 0,1,2,3)");
    if (bit_order != MSB_FIRST && bit_order != LSB_FIRST)
        return _spi_error(spi, SPI_ERROR_ARG, 0, "Invalid bit order (can be MSB_FIRST,LSB_FIRST)");
#ifndef SPI_IOC_WR_MODE32
    if (extra_flags > 0xff)
        return _spi_error(spi, SPI_ERROR_UNSUPPORTED, 0, "Kernel version does not support 32-bit SPI mode flags");
#endif

    memset(spi, 0, sizeof(spi_t));

    /* Open device */
    if ((spi->fd = open(path, O_RDWR)) < 0)
        return _spi_error(spi, SPI_ERROR_OPEN, errno, "Opening SPI device \"%s\"", path);

    /* Set mode, bit order, extra flags */
#ifndef SPI_IOC_WR_MODE32
    (void)data32;

    data8 = mode | ((bit_order == LSB_FIRST) ? SPI_LSB_FIRST : 0) | extra_flags;
    if (ioctl(spi->fd, SPI_IOC_WR_MODE, &data8) < 0) {
        int errsv = errno;
        close(spi->fd);
        spi->fd = -1;
        return _spi_error(spi, SPI_ERROR_CONFIGURE, errsv, "Setting SPI mode");
    }
#else
    if (extra_flags > 0xff) {
        /* Use 32-bit mode if extra_flags is wider than 8-bits */
        data32 = mode | ((bit_order == LSB_FIRST) ? SPI_LSB_FIRST : 0) | extra_flags;
        if (ioctl(spi->fd, SPI_IOC_WR_MODE32, &data32) < 0) {
            int errsv = errno;
            close(spi->fd);
            spi->fd = -1;
            return _spi_error(spi, SPI_ERROR_CONFIGURE, errsv, "Setting SPI mode");
        }
    } else {
        /* Prefer 8-bit mode, in case this library is inadvertently used on an
         * older kernel. */
        data8 = mode | ((bit_order == LSB_FIRST) ? SPI_LSB_FIRST : 0) | extra_flags;
        if (ioctl(spi->fd, SPI_IOC_WR_MODE, &data8) < 0) {
            int errsv = errno;
            close(spi->fd);
            spi->fd = -1;
            return _spi_error(spi, SPI_ERROR_CONFIGURE, errsv, "Setting SPI mode");
        }
    }
#endif

    /* Set max speed */
    if (ioctl(spi->fd, SPI_IOC_WR_MAX_SPEED_HZ, &max_speed) < 0) {
        int errsv = errno;
        close(spi->fd);
        spi->fd = -1;
        return _spi_error(spi, SPI_ERROR_CONFIGURE, errsv, "Setting SPI max speed");
    }

    /* Set bits per word */
    if (ioctl(spi->fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0) {
        int errsv = errno;
        close(spi->fd);
        spi->fd = -1;
        return _spi_error(spi, SPI_ERROR_CONFIGURE, errsv, "Setting SPI bits per word");
    }

    return 0;
}

int spi_transfer(spi_t *spi, const uint8_t *txbuf, uint8_t *rxbuf, size_t len) {
    struct spi_ioc_transfer spi_xfer;

    /* Prepare SPI transfer structure */
    memset(&spi_xfer, 0, sizeof(struct spi_ioc_transfer));
    spi_xfer.tx_buf = (uintptr_t)txbuf;
    spi_xfer.rx_buf = (uintptr_t)rxbuf;
    spi_xfer.len = len;
    spi_xfer.delay_usecs = 0;
    spi_xfer.speed_hz = 0;
    spi_xfer.bits_per_word = 0;
    spi_xfer.cs_change = 0;

    /* Transfer */
    if (ioctl(spi->fd, SPI_IOC_MESSAGE(1), &spi_xfer) < 1)
        return _spi_error(spi, SPI_ERROR_TRANSFER, errno, "SPI transfer");

    return 0;
}

int spi_close(spi_t *spi) {
    if (spi->fd < 0)
        return 0;

    /* Close fd */
    if (close(spi->fd) < 0)
        return _spi_error(spi, SPI_ERROR_CLOSE, errno, "Closing SPI device");

    spi->fd = -1;

    return 0;
}

int spi_get_mode(spi_t *spi, unsigned int *mode) {
    uint8_t data8;

    if (ioctl(spi->fd, SPI_IOC_RD_MODE, &data8) < 0)
        return _spi_error(spi, SPI_ERROR_QUERY, errno, "Getting SPI mode");

    *mode = data8 & (SPI_CPHA | SPI_CPOL);

    return 0;
}

int spi_get_max_speed(spi_t *spi, uint32_t *max_speed) {
    uint32_t data32;

    if (ioctl(spi->fd, SPI_IOC_RD_MAX_SPEED_HZ, &data32) < 0)
        return _spi_error(spi, SPI_ERROR_QUERY, errno, "Getting SPI max speed");

    *max_speed = data32;

    return 0;
}

int spi_get_bit_order(spi_t *spi, spi_bit_order_t *bit_order) {
    uint8_t data8;

    if (ioctl(spi->fd, SPI_IOC_RD_LSB_FIRST, &data8) < 0)
        return _spi_error(spi, SPI_ERROR_QUERY, errno, "Getting SPI bit order");

    if (data8)
        *bit_order = LSB_FIRST;
    else
        *bit_order = MSB_FIRST;

    return 0;
}

int spi_get_bits_per_word(spi_t *spi, uint8_t *bits_per_word) {
    uint8_t data8;

    if (ioctl(spi->fd, SPI_IOC_RD_BITS_PER_WORD, &data8) < 0)
        return _spi_error(spi, SPI_ERROR_QUERY, errno, "Getting SPI bits per word");

    *bits_per_word = data8;

    return 0;
}

int spi_get_extra_flags(spi_t *spi, uint8_t *extra_flags) {
    uint8_t data8;

    if (ioctl(spi->fd, SPI_IOC_RD_MODE, &data8) < 0)
        return _spi_error(spi, SPI_ERROR_QUERY, errno, "Getting SPI mode flags");

    /* Extra mode flags without mode 0-3 and bit order */
    *extra_flags = data8 & ~( SPI_CPOL | SPI_CPHA | SPI_LSB_FIRST );

    return 0;
}

int spi_get_extra_flags32(spi_t *spi, uint32_t *extra_flags) {
#ifdef SPI_IOC_RD_MODE32
    uint32_t mode32;

    if (ioctl(spi->fd, SPI_IOC_RD_MODE32, &mode32) < 0)
        return _spi_error(spi, SPI_ERROR_QUERY, errno, "Getting 32-bit SPI mode flags");

    /* Extra mode flags without mode 0-3 and bit order */
    *extra_flags = mode32 & ~(SPI_CPOL | SPI_CPHA | SPI_LSB_FIRST);

    return 0;
#else
    (void)extra_flags;

    return _spi_error(spi, SPI_ERROR_UNSUPPORTED, 0, "Kernel version does not support 32-bit SPI mode flags");
#endif
}

int spi_set_mode(spi_t *spi, unsigned int mode) {
    uint8_t data8;

    if (mode & ~0x3)
        return _spi_error(spi, SPI_ERROR_ARG, 0, "Invalid mode (can be 0,1,2,3)");

    if (ioctl(spi->fd, SPI_IOC_RD_MODE, &data8) < 0)
        return _spi_error(spi, SPI_ERROR_QUERY, errno, "Getting SPI mode");

    data8 &= ~(SPI_CPOL | SPI_CPHA);
    data8 |= mode;

    if (ioctl(spi->fd, SPI_IOC_WR_MODE, &data8) < 0)
        return _spi_error(spi, SPI_ERROR_CONFIGURE, errno, "Setting SPI mode");

    return 0;
}

int spi_set_bit_order(spi_t *spi, spi_bit_order_t bit_order) {
    uint8_t data8;

    if (bit_order != MSB_FIRST && bit_order != LSB_FIRST)
        return _spi_error(spi, SPI_ERROR_ARG, 0, "Invalid bit order (can be MSB_FIRST,LSB_FIRST)");

    if (bit_order == LSB_FIRST)
        data8 = 1;
    else
        data8 = 0;

    if (ioctl(spi->fd, SPI_IOC_WR_LSB_FIRST, &data8) < 0)
        return _spi_error(spi, SPI_ERROR_CONFIGURE, errno, "Setting SPI bit order");

    return 0;
}

int spi_set_extra_flags(spi_t *spi, uint8_t extra_flags) {
    uint8_t data8;

    if (ioctl(spi->fd, SPI_IOC_RD_MODE, &data8) < 0)
        return _spi_error(spi, SPI_ERROR_QUERY, errno, "Getting SPI mode flags");

    /* Keep mode 0-3 and bit order */
    data8 &= (SPI_CPOL | SPI_CPHA | SPI_LSB_FIRST);
    /* Set extra flags */
    data8 |= extra_flags;

    if (ioctl(spi->fd, SPI_IOC_WR_MODE, &data8) < 0)
        return _spi_error(spi, SPI_ERROR_CONFIGURE, errno, "Setting SPI mode flags");

    return 0;
}

int spi_set_extra_flags32(spi_t *spi, uint32_t extra_flags) {
#ifdef SPI_IOC_WR_MODE32
    uint32_t mode32;

    if (ioctl(spi->fd, SPI_IOC_RD_MODE32, &mode32) < 0)
        return _spi_error(spi, SPI_ERROR_QUERY, errno, "Getting 32-bit SPI mode flags");

    /* Keep mode 0-3 and bit order */
    mode32 &= (SPI_CPOL | SPI_CPHA | SPI_LSB_FIRST);
    /* Set extra flags */
    mode32 |= extra_flags;

    if (ioctl(spi->fd, SPI_IOC_WR_MODE32, &mode32) < 0)
        return _spi_error(spi, SPI_ERROR_CONFIGURE, errno, "Setting 32-bit SPI mode flags");

    return 0;
#else
    (void)extra_flags;

    return _spi_error(spi, SPI_ERROR_UNSUPPORTED, 0, "Kernel version does not support 32-bit SPI mode flags");
#endif
}

int spi_set_max_speed(spi_t *spi, uint32_t max_speed) {

    if (ioctl(spi->fd, SPI_IOC_WR_MAX_SPEED_HZ, &max_speed) < 0)
        return _spi_error(spi, SPI_ERROR_CONFIGURE, errno, "Setting SPI max speed");

    return 0;
}

int spi_set_bits_per_word(spi_t *spi, uint8_t bits_per_word) {

    if (ioctl(spi->fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0)
        return _spi_error(spi, SPI_ERROR_CONFIGURE, errno, "Setting SPI bits per word");

    return 0;
}

int spi_tostring(spi_t *spi, char *str, size_t len) {
    unsigned int mode;
    char mode_str[2];
    uint32_t max_speed;
    char max_speed_str[16];
    uint8_t bits_per_word;
    char bits_per_word_str[4];
    spi_bit_order_t bit_order;
    const char *bit_order_str;
    uint8_t extra_flags8;
    uint32_t extra_flags32;
    char extra_flags_str[11];

    if (spi_get_mode(spi, &mode) < 0)
        strcpy(mode_str, "?");
    else
        snprintf(mode_str, sizeof(mode_str), "%u", mode);

    if (spi_get_max_speed(spi, &max_speed) < 0)
        strcpy(max_speed_str, "?");
    else
        snprintf(max_speed_str, sizeof(max_speed_str), "%u", max_speed);

    if (spi_get_bit_order(spi, &bit_order) < 0)
        bit_order_str = "?";
    else
        bit_order_str = (bit_order == LSB_FIRST) ? "LSB_FIRST" : "MSB_FIRST";

    if (spi_get_bits_per_word(spi, &bits_per_word) < 0)
        strcpy(bits_per_word_str, "?");
    else
        snprintf(bits_per_word_str, sizeof(bits_per_word_str), "%u", bits_per_word);

    if (spi_get_extra_flags32(spi, &extra_flags32) < 0) {
        if (spi_get_extra_flags(spi, &extra_flags8) < 0)
            strcpy(extra_flags_str, "?");
        else
            snprintf(extra_flags_str, sizeof(extra_flags_str), "0x%02x", extra_flags8);
    } else {
        snprintf(extra_flags_str, sizeof(extra_flags_str), "0x%08x", extra_flags32);
    }

    return snprintf(str, len, "SPI (fd=%d, mode=%s, max_speed=%s, bit_order=%s, bits_per_word=%s, extra_flags=%s)", spi->fd, mode_str, max_speed_str, bit_order_str, bits_per_word_str, extra_flags_str);
}

const char *spi_errmsg(spi_t *spi) {
    return spi->error.errmsg;
}

int spi_errno(spi_t *spi) {
    return spi->error.c_errno;
}

int spi_fd(spi_t *spi) {
    return spi->fd;
}

