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

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "i2c.h"

struct i2c_handle {
    int fd;

    struct {
        int c_errno;
        char errmsg[96];
    } error;
};

static int _i2c_error(i2c_t *i2c, int code, int c_errno, const char *fmt, ...) {
    va_list ap;

    i2c->error.c_errno = c_errno;

    va_start(ap, fmt);
    vsnprintf(i2c->error.errmsg, sizeof(i2c->error.errmsg), fmt, ap);
    va_end(ap);

    /* Tack on strerror() and errno */
    if (c_errno) {
        char buf[64];
        strerror_r(c_errno, buf, sizeof(buf));
        snprintf(i2c->error.errmsg+strlen(i2c->error.errmsg), sizeof(i2c->error.errmsg)-strlen(i2c->error.errmsg), ": %s [errno %d]", buf, c_errno);
    }

    return code;
}

i2c_t *i2c_new(void) {
    i2c_t *i2c = calloc(1, sizeof(i2c_t));
    if (i2c == NULL)
        return NULL;

    i2c->fd = -1;

    return i2c;
}

void i2c_free(i2c_t *i2c) {
    free(i2c);
}

int i2c_open(i2c_t *i2c, const char *path) {
    unsigned long supported_funcs;

    memset(i2c, 0, sizeof(i2c_t));

    /* Open device */
    if ((i2c->fd = open(path, O_RDWR)) < 0)
        return _i2c_error(i2c, I2C_ERROR_OPEN, errno, "Opening I2C device \"%s\"", path);

    /* Query supported functions */
    if (ioctl(i2c->fd, I2C_FUNCS, &supported_funcs) < 0) {
        int errsv = errno;
        close(i2c->fd);
        i2c->fd = -1;
        return _i2c_error(i2c, I2C_ERROR_QUERY, errsv, "Querying I2C functions");
    }

    if (!(supported_funcs & I2C_FUNC_I2C)) {
        close(i2c->fd);
        i2c->fd = -1;
        return _i2c_error(i2c, I2C_ERROR_NOT_SUPPORTED, 0, "I2C not supported on %s", path);
    }

    return 0;
}

int i2c_transfer(i2c_t *i2c, struct i2c_msg *msgs, size_t count) {
    struct i2c_rdwr_ioctl_data i2c_rdwr_data;

    /* Prepare I2C transfer structure */
    memset(&i2c_rdwr_data, 0, sizeof(struct i2c_rdwr_ioctl_data));
    i2c_rdwr_data.msgs = msgs;
    i2c_rdwr_data.nmsgs = count;

    /* Transfer */
    if (ioctl(i2c->fd, I2C_RDWR, &i2c_rdwr_data) < 0)
        return _i2c_error(i2c, I2C_ERROR_TRANSFER, errno, "I2C transfer");

    return 0;
}

int i2c_close(i2c_t *i2c) {
    if (i2c->fd < 0)
        return 0;

    /* Close fd */
    if (close(i2c->fd) < 0)
        return _i2c_error(i2c, I2C_ERROR_CLOSE, errno, "Closing I2C device");

    i2c->fd = -1;

    return 0;
}

int i2c_tostring(i2c_t *i2c, char *str, size_t len) {
    return snprintf(str, len, "I2C (fd=%d)", i2c->fd);
}

const char *i2c_errmsg(i2c_t *i2c) {
    return i2c->error.errmsg;
}

int i2c_errno(i2c_t *i2c) {
    return i2c->error.c_errno;
}

int i2c_fd(i2c_t *i2c) {
    return i2c->fd;
}

