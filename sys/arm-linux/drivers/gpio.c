/*
 * c-periphery
 * https://github.com/vsergeev/c-periphery
 * License: MIT
 */

#define _XOPEN_SOURCE   600 /* for POLLRDNORM */

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <errno.h>

#include <sys/ioctl.h>

#if PERIPHERY_GPIO_CDEV_SUPPORT
#include <linux/gpio.h>

/* Disable cdev support when building with older kernel headers that don't yet
 * support line events in the gpio-cdev abi */
#ifndef GPIO_GET_LINEEVENT_IOCTL
#undef PERIPHERY_GPIO_CDEV_SUPPORT
#endif
#endif

#include "gpio.h"

/*********************************************************************************/
/* Operations table and handle structure */
/*********************************************************************************/

struct gpio_ops {
    int (*read)(gpio_t *gpio, bool *value);
    int (*write)(gpio_t *gpio, bool value);
    int (*read_event)(gpio_t *gpio, gpio_edge_t *edge, uint64_t *timestamp);
    int (*poll)(gpio_t *gpio, int timeout_ms);
    int (*close)(gpio_t *gpio);
    int (*get_direction)(gpio_t *gpio, gpio_direction_t *direction);
    int (*get_edge)(gpio_t *gpio, gpio_edge_t *edge);
    int (*get_bias)(gpio_t *gpio, gpio_bias_t *bias);
    int (*get_drive)(gpio_t *gpio, gpio_drive_t *drive);
    int (*get_inverted)(gpio_t *gpio, bool *inverted);
    int (*set_direction)(gpio_t *gpio, gpio_direction_t direction);
    int (*set_edge)(gpio_t *gpio, gpio_edge_t edge);
    int (*set_bias)(gpio_t *gpio, gpio_bias_t bias);
    int (*set_drive)(gpio_t *gpio, gpio_drive_t drive);
    int (*set_inverted)(gpio_t *gpio, bool inverted);
    unsigned int (*line)(gpio_t *gpio);
    int (*fd)(gpio_t *gpio);
    int (*name)(gpio_t *gpio, char *str, size_t len);
    int (*label)(gpio_t *gpio, char *str, size_t len);
    int (*chip_fd)(gpio_t *gpio);
    int (*chip_name)(gpio_t *gpio, char *str, size_t len);
    int (*chip_label)(gpio_t *gpio, char *str, size_t len);
    int (*tostring)(gpio_t *gpio, char *str, size_t len);
};

struct gpio_handle {
    const struct gpio_ops *ops;

    union {
        struct {
            unsigned int line;
            int line_fd;
            int chip_fd;
            gpio_direction_t direction;
            gpio_edge_t edge;
            gpio_bias_t bias;
            gpio_drive_t drive;
            bool inverted;
            char label[32];
        } cdev;
        struct {
            unsigned int line;
            int line_fd;
            bool exported;
        } sysfs;
    } u;

    /* error state */
    struct {
        int c_errno;
        char errmsg[96];
    } error;
};

static const struct gpio_ops gpio_cdev_ops;
static const struct gpio_ops gpio_sysfs_ops;

/*********************************************************************************/
/* Public interface, except for open()s */
/*********************************************************************************/

gpio_t *gpio_new(void) {
    gpio_t *gpio = calloc(1, sizeof(gpio_t));
    if (gpio == NULL)
        return NULL;

    gpio->ops = &gpio_cdev_ops;
    gpio->u.cdev.line_fd = -1;
    gpio->u.cdev.chip_fd = -1;

    return gpio;
}

void gpio_free(gpio_t *gpio) {
    free(gpio);
}

const char *gpio_errmsg(gpio_t *gpio) {
    return gpio->error.errmsg;
}

int gpio_errno(gpio_t *gpio) {
    return gpio->error.c_errno;
}

int gpio_read(gpio_t *gpio, bool *value) {
    return gpio->ops->read(gpio, value);
}

int gpio_write(gpio_t *gpio, bool value) {
    return gpio->ops->write(gpio, value);
}

int gpio_read_event(gpio_t *gpio, gpio_edge_t *edge, uint64_t *timestamp) {
    return gpio->ops->read_event(gpio, edge, timestamp);
}

int gpio_poll(gpio_t *gpio, int timeout_ms) {
    return gpio->ops->poll(gpio, timeout_ms);
}

int gpio_close(gpio_t *gpio) {
    return gpio->ops->close(gpio);
}

int gpio_get_direction(gpio_t *gpio, gpio_direction_t *direction) {
    return gpio->ops->get_direction(gpio, direction);
}

int gpio_get_edge(gpio_t *gpio, gpio_edge_t *edge) {
    return gpio->ops->get_edge(gpio, edge);
}

int gpio_get_bias(gpio_t *gpio, gpio_bias_t *bias) {
    return gpio->ops->get_bias(gpio, bias);
}

int gpio_get_drive(gpio_t *gpio, gpio_drive_t *drive) {
    return gpio->ops->get_drive(gpio, drive);
}

int gpio_get_inverted(gpio_t *gpio, bool *inverted) {
    return gpio->ops->get_inverted(gpio, inverted);
}

int gpio_set_direction(gpio_t *gpio, gpio_direction_t direction) {
    return gpio->ops->set_direction(gpio, direction);
}

int gpio_set_edge(gpio_t *gpio, gpio_edge_t edge) {
    return gpio->ops->set_edge(gpio, edge);
}

int gpio_set_bias(gpio_t *gpio, gpio_bias_t bias) {
    return gpio->ops->set_bias(gpio, bias);
}

int gpio_set_drive(gpio_t *gpio, gpio_drive_t drive) {
    return gpio->ops->set_drive(gpio, drive);
}

int gpio_set_inverted(gpio_t *gpio, bool inverted) {
    return gpio->ops->set_inverted(gpio, inverted);
}

unsigned int gpio_line(gpio_t *gpio) {
    return gpio->ops->line(gpio);
}

int gpio_fd(gpio_t *gpio) {
    return gpio->ops->fd(gpio);
}

int gpio_name(gpio_t *gpio, char *str, size_t len) {
    return gpio->ops->name(gpio, str, len);
}

int gpio_label(gpio_t *gpio, char *str, size_t len) {
    return gpio->ops->label(gpio, str, len);
}

int gpio_chip_fd(gpio_t *gpio) {
    return gpio->ops->chip_fd(gpio);
}

int gpio_chip_name(gpio_t *gpio, char *str, size_t len) {
    return gpio->ops->chip_name(gpio, str, len);
}

int gpio_chip_label(gpio_t *gpio, char *str, size_t len) {
    return gpio->ops->chip_label(gpio, str, len);
}

int gpio_tostring(gpio_t *gpio, char *str, size_t len) {
    return gpio->ops->tostring(gpio, str, len);
}

/*********************************************************************************/
/* Common error formatting function */
/*********************************************************************************/

static int _gpio_error(gpio_t *gpio, int code, int c_errno, const char *fmt, ...) {
    va_list ap;

    gpio->error.c_errno = c_errno;

    va_start(ap, fmt);
    vsnprintf(gpio->error.errmsg, sizeof(gpio->error.errmsg), fmt, ap);
    va_end(ap);

    /* Tack on strerror() and errno */
    if (c_errno) {
        char buf[64];
        strerror_r(c_errno, buf, sizeof(buf));
        snprintf(gpio->error.errmsg+strlen(gpio->error.errmsg), sizeof(gpio->error.errmsg)-strlen(gpio->error.errmsg), ": %s [errno %d]", buf, c_errno);
    }

    return code;
}

/*********************************************************************************/
/* GPIO Poll Multiple Implementation */
/*********************************************************************************/

int gpio_poll_multiple(gpio_t **gpios, size_t count, int timeout_ms, bool *gpios_ready) {
    struct pollfd fds[count];
    int ret;

    /* Setup pollfd structs */
    for (size_t i = 0; i < count; i++) {
        fds[i].fd = gpio_fd(gpios[i]);
        fds[i].events = (gpios[i]->ops == &gpio_sysfs_ops) ?
                            (POLLPRI | POLLERR) : (POLLIN | POLLRDNORM);
        if (gpios_ready)
            gpios_ready[i] = false;
    }

    /* Poll */
    if ((ret = poll(fds, count, timeout_ms)) < 0)
        return GPIO_ERROR_IO;

    /* Event occurred */
    if (ret) {
        for (size_t i = 0; i < count; i++) {
            /* Set ready GPIOs */
            if (gpios_ready)
                gpios_ready[i] = fds[i].revents != 0;

            /* Rewind GPIO if it is a sysfs GPIO */
            if (gpios[i]->ops == &gpio_sysfs_ops) {
                if (lseek(gpios[i]->u.sysfs.line_fd, 0, SEEK_SET) < 0)
                    return GPIO_ERROR_IO;
            }
        }

        return ret;
    }

    /* Timed out */
    return 0;
}

/*********************************************************************************/
/* sysfs implementation */
/*********************************************************************************/

#define P_PATH_MAX  256

/* Delay between checks for successful GPIO export (100ms) */
#define GPIO_SYSFS_OPEN_DELAY      100000
/* Number of retries to check for successful GPIO exports */
#define GPIO_SYSFS_OPEN_RETRIES    10

static int gpio_sysfs_close(gpio_t *gpio) {
    char buf[16];
    int len, fd;

    if (gpio->u.sysfs.line_fd < 0)
        return 0;

    /* Close fd */
    if (close(gpio->u.sysfs.line_fd) < 0)
        return _gpio_error(gpio, GPIO_ERROR_CLOSE, errno, "Closing GPIO 'value'");

    gpio->u.sysfs.line_fd = -1;

    /* Unexport the GPIO, if we exported it */
    if (gpio->u.sysfs.exported) {
        len = snprintf(buf, sizeof(buf), "%u\n", gpio->u.sysfs.line);

        if ((fd = open("/sys/class/gpio/unexport", O_WRONLY)) < 0)
            return _gpio_error(gpio, GPIO_ERROR_CLOSE, errno, "Closing GPIO: opening 'unexport'");

        if (write(fd, buf, len) < 0) {
            int errsv = errno;
            close(fd);
            return _gpio_error(gpio, GPIO_ERROR_CLOSE, errsv, "Closing GPIO: writing 'unexport'");
        }

        if (close(fd) < 0)
            return _gpio_error(gpio, GPIO_ERROR_CLOSE, errno, "Closing GPIO: closing 'unexport'");

        gpio->u.sysfs.exported = false;
    }

    return 0;
}

static int gpio_sysfs_read(gpio_t *gpio, bool *value) {
    char buf[2];

    /* Read fd */
    if (read(gpio->u.sysfs.line_fd, buf, 2) < 0)
        return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Reading GPIO 'value'");

    /* Rewind */
    if (lseek(gpio->u.sysfs.line_fd, 0, SEEK_SET) < 0)
        return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Rewinding GPIO 'value'");

    if (buf[0] == '0')
        *value = false;
    else if (buf[0] == '1')
        *value = true;
    else
        return _gpio_error(gpio, GPIO_ERROR_IO, 0, "Unknown GPIO value");

    return 0;
}

static int gpio_sysfs_write(gpio_t *gpio, bool value) {
    static const char *value_str[2] = {"0\n", "1\n"};

    /* Write fd */
    if (write(gpio->u.sysfs.line_fd, value_str[value], 2) < 0)
        return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Writing GPIO 'value'");

    /* Rewind */
    if (lseek(gpio->u.sysfs.line_fd, 0, SEEK_SET) < 0)
        return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Rewinding GPIO 'value'");

    return 0;
}

static int gpio_sysfs_read_event(gpio_t *gpio, gpio_edge_t *edge, uint64_t *timestamp) {
    (void)edge;
    (void)timestamp;
    return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "GPIO of type sysfs does not support read event");
}

static int gpio_sysfs_poll(gpio_t *gpio, int timeout_ms) {
    struct pollfd fds[1];
    int ret;

    /* Poll */
    fds[0].fd = gpio->u.sysfs.line_fd;
    fds[0].events = POLLPRI | POLLERR;
    if ((ret = poll(fds, 1, timeout_ms)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Polling GPIO 'value'");

    /* GPIO edge interrupt occurred */
    if (ret) {
        /* Rewind */
        if (lseek(gpio->u.sysfs.line_fd, 0, SEEK_SET) < 0)
            return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Rewinding GPIO 'value'");

        return 1;
    }

    /* Timed out */
    return 0;
}

static int gpio_sysfs_set_direction(gpio_t *gpio, gpio_direction_t direction) {
    char gpio_path[P_PATH_MAX];
    const char *buf;
    int fd;

    if (direction == GPIO_DIR_IN)
        buf = "in\n";
    else if (direction == GPIO_DIR_OUT)
        buf = "out\n";
    else if (direction == GPIO_DIR_OUT_LOW)
        buf = "low\n";
    else if (direction == GPIO_DIR_OUT_HIGH)
        buf = "high\n";
    else
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO direction (can be in, out, low, high)");

    /* Write direction */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u/direction", gpio->u.sysfs.line);

    if ((fd = open(gpio_path, O_WRONLY)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errno, "Opening GPIO 'direction'");

    if (write(fd, buf, strlen(buf)) < 0) {
        int errsv = errno;
        close(fd);
        return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errsv, "Writing GPIO 'direction'");
    }

    if (close(fd) < 0)
        return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errno, "Closing GPIO 'direction'");

    return 0;
}

static int gpio_sysfs_get_direction(gpio_t *gpio, gpio_direction_t *direction) {
    char gpio_path[P_PATH_MAX];
    char buf[8];
    int fd, ret;

    /* Read direction */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u/direction", gpio->u.sysfs.line);

    if ((fd = open(gpio_path, O_RDONLY)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Opening GPIO 'direction'");

    if ((ret = read(fd, buf, sizeof(buf))) < 0) {
        int errsv = errno;
        close(fd);
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errsv, "Reading GPIO 'direction'");
    }

    if (close(fd) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Closing GPIO 'direction'");

    buf[ret] = '\0';

    if (strcmp(buf, "in\n") == 0)
        *direction = GPIO_DIR_IN;
    else if (strcmp(buf, "out\n") == 0)
        *direction = GPIO_DIR_OUT;
    else
        return _gpio_error(gpio, GPIO_ERROR_QUERY, 0, "Unknown GPIO direction");

    return 0;
}

static int gpio_sysfs_set_edge(gpio_t *gpio, gpio_edge_t edge) {
    char gpio_path[P_PATH_MAX];
    const char *buf;
    int fd;

    if (edge == GPIO_EDGE_NONE)
        buf = "none\n";
    else if (edge == GPIO_EDGE_RISING)
        buf = "rising\n";
    else if (edge == GPIO_EDGE_FALLING)
        buf = "falling\n";
    else if (edge == GPIO_EDGE_BOTH)
        buf = "both\n";
    else
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO interrupt edge (can be none, rising, falling, both)");

    /* Write edge */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u/edge", gpio->u.sysfs.line);

    if ((fd = open(gpio_path, O_WRONLY)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errno, "Opening GPIO 'edge'");

    if (write(fd, buf, strlen(buf)) < 0) {
        int errsv = errno;
        close(fd);
        return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errsv, "Writing GPIO 'edge'");
    }

    if (close(fd) < 0)
        return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errno, "Closing GPIO 'edge'");

    return 0;
}

static int gpio_sysfs_get_edge(gpio_t *gpio, gpio_edge_t *edge) {
    char gpio_path[P_PATH_MAX];
    char buf[16];
    int fd, ret;

    /* Read edge */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u/edge", gpio->u.sysfs.line);

    if ((fd = open(gpio_path, O_RDONLY)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Opening GPIO 'edge'");

    if ((ret = read(fd, buf, sizeof(buf))) < 0) {
        int errsv = errno;
        close(fd);
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errsv, "Reading GPIO 'edge'");
    }

    if (close(fd) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Closing GPIO 'edge'");

    buf[ret] = '\0';

    if (strcmp(buf, "none\n") == 0)
        *edge = GPIO_EDGE_NONE;
    else if (strcmp(buf, "rising\n") == 0)
        *edge = GPIO_EDGE_RISING;
    else if (strcmp(buf, "falling\n") == 0)
        *edge = GPIO_EDGE_FALLING;
    else if (strcmp(buf, "both\n") == 0)
        *edge = GPIO_EDGE_BOTH;
    else
        return _gpio_error(gpio, GPIO_ERROR_QUERY, 0, "Unknown GPIO edge");

    return 0;
}

static int gpio_sysfs_set_bias(gpio_t *gpio, gpio_bias_t bias) {
    (void)bias;
    return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "GPIO of type sysfs does not support line bias attribute");
}

static int gpio_sysfs_get_bias(gpio_t *gpio, gpio_bias_t *bias) {
    (void)bias;
    return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "GPIO of type sysfs does not support line bias attribute");
}

static int gpio_sysfs_set_drive(gpio_t *gpio, gpio_drive_t drive) {
    (void)drive;
    return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "GPIO of type sysfs does not support line drive attribute");
}

static int gpio_sysfs_get_drive(gpio_t *gpio, gpio_drive_t *drive) {
    (void)drive;
    return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "GPIO of type sysfs does not support line drive attribute");
}

static int gpio_sysfs_set_inverted(gpio_t *gpio, bool inverted) {
    char gpio_path[P_PATH_MAX];
    static const char *inverted_str[2] = {"0\n", "1\n"};
    int fd;

    /* Write active_low */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u/active_low", gpio->u.sysfs.line);

    if ((fd = open(gpio_path, O_WRONLY)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errno, "Opening GPIO 'active_low'");

    if (write(fd, inverted_str[inverted], strlen(inverted_str[inverted])) < 0) {
        int errsv = errno;
        close(fd);
        return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errsv, "Writing GPIO 'active_low'");
    }

    if (close(fd) < 0)
        return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errno, "Closing GPIO 'active_low'");

    return 0;
}

static int gpio_sysfs_get_inverted(gpio_t *gpio, bool *inverted) {
    char gpio_path[P_PATH_MAX];
    char buf[4];
    int fd, ret;

    /* Read active_low */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u/active_low", gpio->u.sysfs.line);

    if ((fd = open(gpio_path, O_RDONLY)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Opening GPIO 'active_low'");

    if ((ret = read(fd, buf, sizeof(buf))) < 0) {
        int errsv = errno;
        close(fd);
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errsv, "Reading GPIO 'active_low'");
    }

    if (close(fd) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Closing GPIO 'active_low'");

    buf[ret] = '\0';

    if (buf[0] == '0')
        *inverted = false;
    else if (buf[0] == '1')
        *inverted = true;
    else
        return _gpio_error(gpio, GPIO_ERROR_QUERY, 0, "Unknown GPIO active_low value");

    return 0;
}

static unsigned int gpio_sysfs_line(gpio_t *gpio) {
    return gpio->u.sysfs.line;
}

static int gpio_sysfs_fd(gpio_t *gpio) {
    return gpio->u.sysfs.line_fd;
}

static int gpio_sysfs_name(gpio_t *gpio, char *str, size_t len) {
    (void)gpio;
    if (len)
        str[0] = '\0';

    return 0;
}

static int gpio_sysfs_label(gpio_t *gpio, char *str, size_t len) {
    (void)gpio;
    if (len)
        str[0] = '\0';

    return 0;
}

static int gpio_sysfs_chip_fd(gpio_t *gpio) {
    return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "GPIO of type sysfs has no chip fd");
}

static int gpio_sysfs_chip_name(gpio_t *gpio, char *str, size_t len) {
    int ret;
    char gpio_path[P_PATH_MAX];
    char gpiochip_path[P_PATH_MAX];

    if (!len)
        return 0;

    /* Form path to device */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u/device", gpio->u.sysfs.line);

    /* Resolve symlink to gpiochip */
    if ((ret = readlink(gpio_path, gpiochip_path, sizeof(gpiochip_path))) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Reading GPIO chip symlink");

    /* Null-terminate symlink path */
    gpiochip_path[(ret < P_PATH_MAX) ? ret : (P_PATH_MAX - 1)] = '\0';

    /* Find last / in symlink path */
    const char *sep = strrchr(gpiochip_path, '/');
    if (!sep)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, 0, "Invalid GPIO chip symlink");

    strncpy(str, sep + 1, len - 1);
    str[len - 1] = '\0';

    return 0;
}

static int gpio_sysfs_chip_label(gpio_t *gpio, char *str, size_t len) {
    char gpio_path[P_PATH_MAX];
    char chip_name[32];
    int fd, ret;

    if (!len)
        return 0;

    if ((ret = gpio_sysfs_chip_name(gpio, chip_name, sizeof(chip_name))) < 0)
        return ret;

    /* Read gpiochip label */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/%s/label", chip_name);

    if ((fd = open(gpio_path, O_RDONLY)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Opening GPIO chip 'label'");

    if ((ret = read(fd, str, len)) < 0) {
        int errsv = errno;
        close(fd);
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errsv, "Reading GPIO chip 'label'");
    }

    if (close(fd) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Closing GPIO 'label'");

    str[ret - 1] = '\0';

    return 0;
}

static int gpio_sysfs_tostring(gpio_t *gpio, char *str, size_t len) {
    gpio_direction_t direction;
    const char *direction_str;
    gpio_edge_t edge;
    const char *edge_str;
    bool inverted;
    const char *inverted_str;
    char chip_name[32];
    const char *chip_name_str;
    char chip_label[32];
    const char *chip_label_str;

    if (gpio_sysfs_get_direction(gpio, &direction) < 0)
        direction_str = "<error>";
    else
        direction_str = (direction == GPIO_DIR_IN) ? "in" :
                        (direction == GPIO_DIR_OUT) ? "out" : "unknown";

    if (gpio_sysfs_get_edge(gpio, &edge) < 0)
        edge_str = "<error>";
    else
        edge_str = (edge == GPIO_EDGE_NONE) ? "none" :
                   (edge == GPIO_EDGE_RISING) ? "rising" :
                   (edge == GPIO_EDGE_FALLING) ? "falling" :
                   (edge == GPIO_EDGE_BOTH) ? "both" : "unknown";

    if (gpio_sysfs_get_inverted(gpio, &inverted) < 0)
        inverted_str = "<error>";
    else
        inverted_str = inverted ? "true" : "false";

    if (gpio_sysfs_chip_name(gpio, chip_name, sizeof(chip_name)) < 0)
        chip_name_str = "<error>";
    else
        chip_name_str = chip_name;

    if (gpio_sysfs_chip_label(gpio, chip_label, sizeof(chip_label)) < 0)
        chip_label_str = "<error>";
    else
        chip_label_str = chip_label;

    return snprintf(str, len, "GPIO %u (fd=%d, direction=%s, edge=%s, inverted=%s, chip_name=\"%s\", chip_label=\"%s\", type=sysfs)",
                    gpio->u.sysfs.line, gpio->u.sysfs.line_fd, direction_str, edge_str, inverted_str, chip_name_str, chip_label_str);
}

static const struct gpio_ops gpio_sysfs_ops = {
    .read = gpio_sysfs_read,
    .write = gpio_sysfs_write,
    .read_event = gpio_sysfs_read_event,
    .poll = gpio_sysfs_poll,
    .close = gpio_sysfs_close,
    .get_direction = gpio_sysfs_get_direction,
    .get_edge = gpio_sysfs_get_edge,
    .get_bias = gpio_sysfs_get_bias,
    .get_drive = gpio_sysfs_get_drive,
    .get_inverted = gpio_sysfs_get_inverted,
    .set_direction = gpio_sysfs_set_direction,
    .set_edge = gpio_sysfs_set_edge,
    .set_bias = gpio_sysfs_set_bias,
    .set_drive = gpio_sysfs_set_drive,
    .set_inverted = gpio_sysfs_set_inverted,
    .line = gpio_sysfs_line,
    .fd = gpio_sysfs_fd,
    .name = gpio_sysfs_name,
    .label = gpio_sysfs_label,
    .chip_fd = gpio_sysfs_chip_fd,
    .chip_name = gpio_sysfs_chip_name,
    .chip_label = gpio_sysfs_chip_label,
    .tostring = gpio_sysfs_tostring,
};

int gpio_open_sysfs(gpio_t *gpio, unsigned int line, gpio_direction_t direction) {
    char gpio_path[P_PATH_MAX];
    struct stat stat_buf;
    char buf[16];
    int len, fd, ret;
    bool exported = false;

    if (direction != GPIO_DIR_IN && direction != GPIO_DIR_OUT && direction != GPIO_DIR_OUT_LOW && direction != GPIO_DIR_OUT_HIGH)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO direction (can be in, out, low, high)");

    /* Check if GPIO directory exists */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u", line);
    if (stat(gpio_path, &stat_buf) < 0) {
        /* Write line number to export file */
        len = snprintf(buf, sizeof(buf), "%u\n", line);

        if ((fd = open("/sys/class/gpio/export", O_WRONLY)) < 0)
            return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening GPIO: opening 'export'");

        if (write(fd, buf, len) < 0) {
            int errsv = errno;
            close(fd);
            return _gpio_error(gpio, GPIO_ERROR_OPEN, errsv, "Opening GPIO: writing 'export'");
        }

        if (close(fd) < 0)
            return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening GPIO: closing 'export'");

        /* Wait until GPIO directory appears */
        unsigned int retry_count;
        for (retry_count = 0; retry_count < GPIO_SYSFS_OPEN_RETRIES; retry_count++) {
            int ret = stat(gpio_path, &stat_buf);
            if (ret == 0) {
                exported = true;
                break;
            } else if (ret < 0 && errno != ENOENT) {
                return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening GPIO: stat 'gpio%u/' after export", line);
            }

            usleep(GPIO_SYSFS_OPEN_DELAY);
        }

        if (retry_count == GPIO_SYSFS_OPEN_RETRIES)
            return _gpio_error(gpio, GPIO_ERROR_OPEN, 0, "Opening GPIO: waiting for 'gpio%u/' timed out", line);

        /* Write direction, looping in case of EACCES errors due to delayed
         * udev permission rule application after export */
        const char *dir = (direction == GPIO_DIR_OUT) ? "out\n" :
                          (direction == GPIO_DIR_OUT_HIGH) ? "high\n" :
                          (direction == GPIO_DIR_OUT_LOW) ? "low\n" : "in\n";

        snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u/direction", line);

        for (retry_count = 0; retry_count < GPIO_SYSFS_OPEN_RETRIES; retry_count++) {
            if ((fd = open(gpio_path, O_WRONLY)) >= 0) {
                if (write(fd, dir, strlen(dir)) < 0) {
                    int errsv = errno;
                    close(fd);
                    return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errsv, "Writing GPIO 'direction'");
                }

                if (close(fd) < 0)
                    return _gpio_error(gpio, GPIO_ERROR_CONFIGURE, errno, "Closing GPIO 'direction'");

                break;
            } else if (errno != EACCES) {
                return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening GPIO: opening 'gpio%u/direction'", line);
            }

            usleep(GPIO_SYSFS_OPEN_DELAY);
        }

        if (retry_count == GPIO_SYSFS_OPEN_RETRIES)
            return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening GPIO: opening 'gpio%u/direction'", line);
    }

    /* Open value */
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%u/value", line);
    if ((fd = open(gpio_path, O_RDWR)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening GPIO 'gpio%u/value'", line);

    memset(gpio, 0, sizeof(gpio_t));
    gpio->ops = &gpio_sysfs_ops;
    gpio->u.sysfs.line = line;
    gpio->u.sysfs.line_fd = fd;
    gpio->u.sysfs.exported = exported;

    if (!exported) {
        ret = gpio_sysfs_set_direction(gpio, direction);
        if (ret < 0)
            return ret;
    }

    ret = gpio_sysfs_set_inverted(gpio, false);
    if (ret < 0)
        return ret;

    return 0;
}

/*********************************************************************************/
/* cdev implementation */
/*********************************************************************************/

#if PERIPHERY_GPIO_CDEV_SUPPORT

static int _gpio_cdev_reopen(gpio_t *gpio, gpio_direction_t direction, gpio_edge_t edge, gpio_bias_t bias, gpio_drive_t drive, bool inverted) {
    uint32_t flags = 0;

    #ifdef GPIOHANDLE_REQUEST_BIAS_PULL_UP
    if (bias == GPIO_BIAS_PULL_UP)
        flags |= GPIOHANDLE_REQUEST_BIAS_PULL_UP;
    else if (bias == GPIO_BIAS_PULL_DOWN)
        flags |= GPIOHANDLE_REQUEST_BIAS_PULL_DOWN;
    else if (bias == GPIO_BIAS_DISABLE)
        flags |= GPIOHANDLE_REQUEST_BIAS_DISABLE;
    #else
    if (bias != GPIO_BIAS_DEFAULT)
        return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "Kernel version does not support configuring GPIO line bias");
    #endif

    #ifdef GPIOHANDLE_REQUEST_OPEN_DRAIN
    if (drive == GPIO_DRIVE_OPEN_DRAIN)
        flags |= GPIOHANDLE_REQUEST_OPEN_DRAIN;
    else if (drive == GPIO_DRIVE_OPEN_SOURCE)
        flags |= GPIOHANDLE_REQUEST_OPEN_SOURCE;
    #else
    if (drive != GPIO_DRIVE_DEFAULT)
        return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "Kernel version does not support configuring GPIO line drive");
    #endif

    if (inverted)
        flags |= GPIOHANDLE_REQUEST_ACTIVE_LOW;

    /* FIXME this should really use GPIOHANDLE_SET_CONFIG_IOCTL instead of
     * closing and reopening, especially to preserve output value on
     * configuration changes */

    if (gpio->u.cdev.line_fd >= 0) {
        if (close(gpio->u.cdev.line_fd) < 0)
            return _gpio_error(gpio, GPIO_ERROR_CLOSE, errno, "Closing GPIO line");

        gpio->u.cdev.line_fd = -1;
    }

    if (direction == GPIO_DIR_IN) {
        if (edge == GPIO_EDGE_NONE) {
            struct gpiohandle_request request = {0};

            request.lineoffsets[0] = gpio->u.cdev.line;
            request.flags = flags | GPIOHANDLE_REQUEST_INPUT;
            strncpy(request.consumer_label, gpio->u.cdev.label, sizeof(request.consumer_label) - 1);
            request.consumer_label[sizeof(request.consumer_label) - 1] = '\0';
            request.lines = 1;

            if (ioctl(gpio->u.cdev.chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &request) < 0)
                return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening input line handle");

            gpio->u.cdev.line_fd = request.fd;
        } else {
            struct gpioevent_request request = {0};

            request.lineoffset = gpio->u.cdev.line;
            request.handleflags = flags | GPIOHANDLE_REQUEST_INPUT;
            request.eventflags = (edge == GPIO_EDGE_RISING) ? GPIOEVENT_REQUEST_RISING_EDGE :
                                 (edge == GPIO_EDGE_FALLING) ? GPIOEVENT_REQUEST_FALLING_EDGE :
                                                               GPIOEVENT_REQUEST_BOTH_EDGES;
            strncpy(request.consumer_label, gpio->u.cdev.label, sizeof(request.consumer_label) - 1);
            request.consumer_label[sizeof(request.consumer_label) - 1] = '\0';

            if (ioctl(gpio->u.cdev.chip_fd, GPIO_GET_LINEEVENT_IOCTL, &request) < 0)
                return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening input event line handle");

            gpio->u.cdev.line_fd = request.fd;
        }
    } else {
        struct gpiohandle_request request = {0};
        bool initial_value = (direction == GPIO_DIR_OUT_HIGH) ? true : false;
        initial_value ^= inverted;

        request.lineoffsets[0] = gpio->u.cdev.line;
        request.flags = flags | GPIOHANDLE_REQUEST_OUTPUT;
        request.default_values[0] = initial_value;
        strncpy(request.consumer_label, gpio->u.cdev.label, sizeof(request.consumer_label) - 1);
        request.consumer_label[sizeof(request.consumer_label) - 1] = '\0';
        request.lines = 1;

        if (ioctl(gpio->u.cdev.chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &request) < 0)
            return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening output line handle");

        gpio->u.cdev.line_fd = request.fd;
    }

    gpio->u.cdev.direction = (direction == GPIO_DIR_IN) ? GPIO_DIR_IN : GPIO_DIR_OUT;
    gpio->u.cdev.edge = edge;
    gpio->u.cdev.bias = bias;
    gpio->u.cdev.drive = drive;
    gpio->u.cdev.inverted = inverted;

    return 0;
}

static int gpio_cdev_read(gpio_t *gpio, bool *value) {
    struct gpiohandle_data data = {0};

    if (ioctl(gpio->u.cdev.line_fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) < 0)
        return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Getting line value");

    *value = data.values[0];

    return 0;
}

static int gpio_cdev_write(gpio_t *gpio, bool value) {
    struct gpiohandle_data data = {0};

    if (gpio->u.cdev.direction != GPIO_DIR_OUT)
        return _gpio_error(gpio, GPIO_ERROR_INVALID_OPERATION, 0, "Invalid operation: cannot write to input GPIO");

    data.values[0] = value;

    if (ioctl(gpio->u.cdev.line_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0)
        return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Setting line value");

    return 0;
}

static int gpio_cdev_read_event(gpio_t *gpio, gpio_edge_t *edge, uint64_t *timestamp) {
    struct gpioevent_data event_data = {0};

    if (gpio->u.cdev.direction != GPIO_DIR_IN)
        return _gpio_error(gpio, GPIO_ERROR_INVALID_OPERATION, 0, "Invalid operation: cannot read event of output GPIO");
    else if (gpio->u.cdev.edge == GPIO_EDGE_NONE)
        return _gpio_error(gpio, GPIO_ERROR_INVALID_OPERATION, 0, "Invalid operation: GPIO edge not set");

    if (read(gpio->u.cdev.line_fd, &event_data, sizeof(event_data)) < (ssize_t)sizeof(event_data))
        return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Reading GPIO event");

    if (edge)
        *edge = (event_data.id == GPIOEVENT_EVENT_RISING_EDGE) ? GPIO_EDGE_RISING :
                (event_data.id == GPIOEVENT_EVENT_FALLING_EDGE) ? GPIO_EDGE_FALLING : GPIO_EDGE_NONE;
    if (timestamp)
        *timestamp = event_data.timestamp;

    return 0;
}

static int gpio_cdev_poll(gpio_t *gpio, int timeout_ms) {
    struct pollfd fds[1];
    int ret;

    if (gpio->u.cdev.direction != GPIO_DIR_IN)
        return _gpio_error(gpio, GPIO_ERROR_INVALID_OPERATION, 0, "Invalid operation: cannot poll output GPIO");

    fds[0].fd = gpio->u.cdev.line_fd;
    fds[0].events = POLLIN | POLLPRI | POLLERR;
    if ((ret = poll(fds, 1, timeout_ms)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_IO, errno, "Polling GPIO line");

    return ret > 0;
}

static int gpio_cdev_close(gpio_t *gpio) {
    /* Close line fd */
    if (gpio->u.cdev.line_fd >= 0) {
        if (close(gpio->u.cdev.line_fd) < 0)
            return _gpio_error(gpio, GPIO_ERROR_CLOSE, errno, "Closing GPIO line");

        gpio->u.cdev.line_fd = -1;
    }

    /* Close chip fd */
    if (gpio->u.cdev.chip_fd >= 0) {
        if (close(gpio->u.cdev.chip_fd) < 0)
            return _gpio_error(gpio, GPIO_ERROR_CLOSE, errno, "Closing GPIO chip");

        gpio->u.cdev.chip_fd = -1;
    }

    gpio->u.cdev.edge = GPIO_EDGE_NONE;
    gpio->u.cdev.direction = GPIO_DIR_IN;

    return 0;
}

static int gpio_cdev_get_direction(gpio_t *gpio, gpio_direction_t *direction) {
    *direction = gpio->u.cdev.direction;
    return 0;
}

static int gpio_cdev_get_edge(gpio_t *gpio, gpio_edge_t *edge) {
    *edge = gpio->u.cdev.edge;
    return 0;
}

static int gpio_cdev_get_bias(gpio_t *gpio, gpio_bias_t *bias) {
    *bias = gpio->u.cdev.bias;
    return 0;
}

static int gpio_cdev_get_drive(gpio_t *gpio, gpio_drive_t *drive) {
    *drive = gpio->u.cdev.drive;
    return 0;
}

static int gpio_cdev_get_inverted(gpio_t *gpio, bool *inverted) {
    *inverted = gpio->u.cdev.inverted;
    return 0;
}

static int gpio_cdev_set_direction(gpio_t *gpio, gpio_direction_t direction) {
    if (direction != GPIO_DIR_IN && direction != GPIO_DIR_OUT && direction != GPIO_DIR_OUT_LOW && direction != GPIO_DIR_OUT_HIGH)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO direction (can be in, out, low, high)");

    if (gpio->u.cdev.direction == direction)
        return 0;

    return _gpio_cdev_reopen(gpio, direction, GPIO_EDGE_NONE, gpio->u.cdev.bias, gpio->u.cdev.drive, gpio->u.cdev.inverted);
}

static int gpio_cdev_set_edge(gpio_t *gpio, gpio_edge_t edge) {
    if (edge != GPIO_EDGE_NONE && edge != GPIO_EDGE_RISING && edge != GPIO_EDGE_FALLING && edge != GPIO_EDGE_BOTH)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO interrupt edge (can be none, rising, falling, both)");

    if (gpio->u.cdev.direction != GPIO_DIR_IN)
        return _gpio_error(gpio, GPIO_ERROR_INVALID_OPERATION, 0, "Invalid operation: cannot set edge on output GPIO");

    if (gpio->u.cdev.edge == edge)
        return 0;

    return _gpio_cdev_reopen(gpio, gpio->u.cdev.direction, edge, gpio->u.cdev.bias, gpio->u.cdev.drive, gpio->u.cdev.inverted);
}

static int gpio_cdev_set_bias(gpio_t *gpio, gpio_bias_t bias) {
    if (bias != GPIO_BIAS_DEFAULT && bias != GPIO_BIAS_PULL_UP && bias != GPIO_BIAS_PULL_DOWN && bias != GPIO_BIAS_DISABLE)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO line bias (can be default, pull_up, pull_down, disable)");

    if (gpio->u.cdev.bias == bias)
        return 0;

    return _gpio_cdev_reopen(gpio, gpio->u.cdev.direction, gpio->u.cdev.edge, bias, gpio->u.cdev.drive, gpio->u.cdev.inverted);
}

static int gpio_cdev_set_drive(gpio_t *gpio, gpio_drive_t drive) {
    if (drive != GPIO_DRIVE_DEFAULT && drive != GPIO_DRIVE_OPEN_DRAIN && drive != GPIO_DRIVE_OPEN_SOURCE)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO line drive (can be default, open_drain, open_source)");

    if (gpio->u.cdev.direction != GPIO_DIR_OUT && drive != GPIO_DRIVE_DEFAULT)
        return _gpio_error(gpio, GPIO_ERROR_INVALID_OPERATION, 0, "Invalid operation: cannot set line drive on input GPIO");

    if (gpio->u.cdev.drive == drive)
        return 0;

    return _gpio_cdev_reopen(gpio, gpio->u.cdev.direction, gpio->u.cdev.edge, gpio->u.cdev.bias, drive, gpio->u.cdev.inverted);
}

static int gpio_cdev_set_inverted(gpio_t *gpio, bool inverted) {
    if (gpio->u.cdev.inverted == inverted)
        return 0;

    return _gpio_cdev_reopen(gpio, gpio->u.cdev.direction, gpio->u.cdev.edge, gpio->u.cdev.bias, gpio->u.cdev.drive, inverted);
}

static unsigned int gpio_cdev_line(gpio_t *gpio) {
    return gpio->u.cdev.line;
}

static int gpio_cdev_fd(gpio_t *gpio) {
    return gpio->u.cdev.line_fd;
}

static int gpio_cdev_name(gpio_t *gpio, char *str, size_t len) {
    struct gpioline_info line_info = {0};

    if (!len)
        return 0;

    line_info.line_offset = gpio->u.cdev.line;

    if (ioctl(gpio->u.cdev.chip_fd, GPIO_GET_LINEINFO_IOCTL, &line_info) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Querying GPIO line info for line %u", gpio->u.cdev.line);

    strncpy(str, line_info.name, len - 1);
    str[len - 1] = '\0';

    return 0;
}

static int gpio_cdev_label(gpio_t *gpio, char *str, size_t len) {
    struct gpioline_info line_info = {0};

    if (!len)
        return 0;

    line_info.line_offset = gpio->u.cdev.line;

    if (ioctl(gpio->u.cdev.chip_fd, GPIO_GET_LINEINFO_IOCTL, &line_info) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Querying GPIO line info for line %u", gpio->u.cdev.line);

    strncpy(str, line_info.consumer, len - 1);
    str[len - 1] = '\0';

    return 0;
}

static int gpio_cdev_chip_fd(gpio_t *gpio) {
    return gpio->u.cdev.chip_fd;
}

static int gpio_cdev_chip_name(gpio_t *gpio, char *str, size_t len) {
    struct gpiochip_info chip_info = {0};

    if (!len)
        return 0;

    if (ioctl(gpio->u.cdev.chip_fd, GPIO_GET_CHIPINFO_IOCTL, &chip_info) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Querying GPIO chip info");

    strncpy(str, chip_info.name, len - 1);
    str[len - 1] = '\0';

    return 0;
}

static int gpio_cdev_chip_label(gpio_t *gpio, char *str, size_t len) {
    struct gpiochip_info chip_info = {0};

    if (!len)
        return 0;

    if (ioctl(gpio->u.cdev.chip_fd, GPIO_GET_CHIPINFO_IOCTL, &chip_info) < 0)
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errno, "Querying GPIO chip info");

    strncpy(str, chip_info.label, len - 1);
    str[len - 1] = '\0';

    return 0;
}

static int gpio_cdev_tostring(gpio_t *gpio, char *str, size_t len) {
    gpio_direction_t direction;
    const char *direction_str;
    gpio_edge_t edge;
    const char *edge_str;
    gpio_bias_t bias;
    const char *bias_str;
    gpio_drive_t drive;
    const char *drive_str;
    bool inverted;
    const char *inverted_str;
    char line_name[32];
    const char *line_name_str;
    char line_label[32];
    const char *line_label_str;
    char chip_name[32];
    const char *chip_name_str;
    char chip_label[32];
    const char *chip_label_str;

    if (gpio_cdev_get_direction(gpio, &direction) < 0)
        direction_str = "<error>";
    else
        direction_str = (direction == GPIO_DIR_IN) ? "in" :
                        (direction == GPIO_DIR_OUT) ? "out" : "unknown";

    if (gpio_cdev_get_edge(gpio, &edge) < 0)
        edge_str = "<error>";
    else
        edge_str = (edge == GPIO_EDGE_NONE) ? "none" :
                   (edge == GPIO_EDGE_RISING) ? "rising" :
                   (edge == GPIO_EDGE_FALLING) ? "falling" :
                   (edge == GPIO_EDGE_BOTH) ? "both" : "unknown";

    if (gpio_cdev_get_bias(gpio, &bias) < 0)
        bias_str = "<error>";
    else
        bias_str = (bias == GPIO_BIAS_DEFAULT) ? "default" :
                   (bias == GPIO_BIAS_PULL_UP) ? "pull_up" :
                   (bias == GPIO_BIAS_PULL_DOWN) ? "pull_down" :
                   (bias == GPIO_BIAS_DISABLE) ? "disable" : "unknown";

    if (gpio_cdev_get_drive(gpio, &drive) < 0)
        drive_str = "<error>";
    else
        drive_str = (drive == GPIO_DRIVE_DEFAULT) ? "default" :
                    (drive == GPIO_DRIVE_OPEN_DRAIN) ? "open_drain" :
                    (drive == GPIO_DRIVE_OPEN_SOURCE) ? "open_source" : "unknown";

    if (gpio_cdev_get_inverted(gpio, &inverted) < 0)
        inverted_str = "<error>";
    else
        inverted_str = inverted ? "true" : "false";

    if (gpio_cdev_name(gpio, line_name, sizeof(line_name)) < 0)
        line_name_str = "<error>";
    else
        line_name_str = line_name;

    if (gpio_cdev_label(gpio, line_label, sizeof(line_label)) < 0)
        line_label_str = "<error>";
    else
        line_label_str = line_label;

    if (gpio_cdev_chip_name(gpio, chip_name, sizeof(chip_name)) < 0)
        chip_name_str = "<error>";
    else
        chip_name_str = chip_name;

    if (gpio_cdev_chip_label(gpio, chip_label, sizeof(chip_label)) < 0)
        chip_label_str = "<error>";
    else
        chip_label_str = chip_label;

    return snprintf(str, len, "GPIO %u (name=\"%s\", label=\"%s\", line_fd=%d, chip_fd=%d, direction=%s, edge=%s, bias=%s, drive=%s, inverted=%s, chip_name=\"%s\", chip_label=\"%s\", type=cdev)",
                    gpio->u.cdev.line, line_name_str, line_label_str, gpio->u.cdev.line_fd, gpio->u.cdev.chip_fd, direction_str, edge_str, bias_str, drive_str, inverted_str, chip_name_str, chip_label_str);
}

static const struct gpio_ops gpio_cdev_ops = {
    .read = gpio_cdev_read,
    .write = gpio_cdev_write,
    .read_event = gpio_cdev_read_event,
    .poll = gpio_cdev_poll,
    .close = gpio_cdev_close,
    .get_direction = gpio_cdev_get_direction,
    .get_edge = gpio_cdev_get_edge,
    .get_bias = gpio_cdev_get_bias,
    .get_drive = gpio_cdev_get_drive,
    .get_inverted = gpio_cdev_get_inverted,
    .set_direction = gpio_cdev_set_direction,
    .set_edge = gpio_cdev_set_edge,
    .set_bias = gpio_cdev_set_bias,
    .set_drive = gpio_cdev_set_drive,
    .set_inverted = gpio_cdev_set_inverted,
    .line = gpio_cdev_line,
    .fd = gpio_cdev_fd,
    .name = gpio_cdev_name,
    .label = gpio_cdev_label,
    .chip_fd = gpio_cdev_chip_fd,
    .chip_name = gpio_cdev_chip_name,
    .chip_label = gpio_cdev_chip_label,
    .tostring = gpio_cdev_tostring,
};

int gpio_open_advanced(gpio_t *gpio, const char *path, unsigned int line, const gpio_config_t *config) {
    int ret, fd;

    if (config->direction != GPIO_DIR_IN && config->direction != GPIO_DIR_OUT && config->direction != GPIO_DIR_OUT_LOW && config->direction != GPIO_DIR_OUT_HIGH)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO direction (can be in, out, low, high)");

    if (config->edge != GPIO_EDGE_NONE && config->edge != GPIO_EDGE_RISING && config->edge != GPIO_EDGE_FALLING && config->edge != GPIO_EDGE_BOTH)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO interrupt edge (can be none, rising, falling, both)");

    if (config->direction != GPIO_DIR_IN && config->edge != GPIO_EDGE_NONE)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO edge for output GPIO");

    if (config->bias != GPIO_BIAS_DEFAULT && config->bias != GPIO_BIAS_PULL_UP && config->bias != GPIO_BIAS_PULL_DOWN && config->bias != GPIO_BIAS_DISABLE)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO line bias (can be default, pull_up, pull_down, disable)");

    if (config->drive != GPIO_DRIVE_DEFAULT && config->drive != GPIO_DRIVE_OPEN_DRAIN && config->drive != GPIO_DRIVE_OPEN_SOURCE)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO line drive (can be default, open_drain, open_source)");

    if (config->direction == GPIO_DIR_IN && config->drive != GPIO_DRIVE_DEFAULT)
        return _gpio_error(gpio, GPIO_ERROR_ARG, 0, "Invalid GPIO line drive for input GPIO");

    /* Open GPIO chip */
    if ((fd = open(path, 0)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening GPIO chip");

    memset(gpio, 0, sizeof(gpio_t));
    gpio->ops = &gpio_cdev_ops;
    gpio->u.cdev.line = line;
    gpio->u.cdev.line_fd = -1;
    gpio->u.cdev.chip_fd = fd;
    strncpy(gpio->u.cdev.label, config->label ? config->label : "periphery", sizeof(gpio->u.cdev.label) - 1);
    gpio->u.cdev.label[sizeof(gpio->u.cdev.label) - 1] = '\0';

    /* Open GPIO line */
    ret = _gpio_cdev_reopen(gpio, config->direction, config->edge, config->bias, config->drive, config->inverted);
    if (ret < 0) {
        close(gpio->u.cdev.chip_fd);
        gpio->u.cdev.chip_fd = -1;
        return ret;
    }

    return 0;
}

int gpio_open_name_advanced(gpio_t *gpio, const char *path, const char *name, const gpio_config_t *config) {
    int fd;

    /* Open GPIO chip */
    if ((fd = open(path, 0)) < 0)
        return _gpio_error(gpio, GPIO_ERROR_OPEN, errno, "Opening GPIO chip");

    /* Get chip info for number of lines */
    struct gpiochip_info chip_info = {0};
    if (ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &chip_info) < 0) {
        int errsv = errno;
        close(fd);
        return _gpio_error(gpio, GPIO_ERROR_QUERY, errsv, "Querying GPIO chip info");
    }

    /* Loop through every line */
    struct gpioline_info line_info = {0};
    unsigned int line;
    for (line = 0; line < chip_info.lines; line++) {
        line_info.line_offset = line;

        /* Get the line info */
        if (ioctl(fd, GPIO_GET_LINEINFO_IOCTL, &line_info) < 0) {
            int errsv = errno;
            close(fd);
            return _gpio_error(gpio, GPIO_ERROR_QUERY, errsv, "Querying GPIO line info for line %u", line);
        }

        /* Compare the name */
        if (strcmp(line_info.name, name) == 0)
            break;
    }

    /* If no matching line name was found */
    if (line == chip_info.lines) {
        close(fd);
        return _gpio_error(gpio, GPIO_ERROR_NOT_FOUND, 0, "GPIO line \"%s\" not found by name", name);
    }

    if (close(fd) < 0)
        return _gpio_error(gpio, GPIO_ERROR_CLOSE, errno, "Closing GPIO chip");

    return gpio_open_advanced(gpio, path, line, config);
}

int gpio_open(gpio_t *gpio, const char *path, unsigned int line, gpio_direction_t direction) {
    gpio_config_t config = {
        .direction = direction,
        .edge = GPIO_EDGE_NONE,
        .bias = GPIO_BIAS_DEFAULT,
        .drive = GPIO_DRIVE_DEFAULT,
        .inverted = false,
        .label = NULL,
    };

    return gpio_open_advanced(gpio, path, line, &config);
}

int gpio_open_name(gpio_t *gpio, const char *path, const char *name, gpio_direction_t direction) {
    gpio_config_t config = {
        .direction = direction,
        .edge = GPIO_EDGE_NONE,
        .bias = GPIO_BIAS_DEFAULT,
        .drive = GPIO_DRIVE_DEFAULT,
        .inverted = false,
        .label = NULL,
    };

    return gpio_open_name_advanced(gpio, path, name, &config);
}

#else /* PERIPHERY_GPIO_CDEV_SUPPORT */

int gpio_open_advanced(gpio_t *gpio, const char *path, unsigned int line, const gpio_config_t *config) {
    (void)path;
    (void)line;
    (void)config;
    return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "c-periphery library built without character device GPIO support.");
}

int gpio_open_name_advanced(gpio_t *gpio, const char *path, const char *name, const gpio_config_t *config) {
    (void)path;
    (void)name;
    (void)config;
    return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "c-periphery library built without character device GPIO support.");
}

int gpio_open(gpio_t *gpio, const char *path, unsigned int line, gpio_direction_t direction) {
    (void)path;
    (void)line;
    (void)direction;
    return _gpio_error(gpio, GPIO_ERROR_UNSUPPORTED, 0, "c-periphery library built without character device GPIO support.");
}

#endif
