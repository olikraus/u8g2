/*
 * c-periphery
 * https://github.com/vsergeev/c-periphery
 * License: MIT
 */

#ifndef _PERIPHERY_GPIO_H
#define _PERIPHERY_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

enum gpio_error_code {
    GPIO_ERROR_ARG                  = -1,   /* Invalid arguments */
    GPIO_ERROR_OPEN                 = -2,   /* Opening GPIO */
    GPIO_ERROR_NOT_FOUND            = -3,   /* Line name not found */
    GPIO_ERROR_QUERY                = -4,   /* Querying GPIO attributes */
    GPIO_ERROR_CONFIGURE            = -5,   /* Configuring GPIO attributes */
    GPIO_ERROR_UNSUPPORTED          = -6,   /* Unsupported attribute or operation */
    GPIO_ERROR_INVALID_OPERATION    = -7,   /* Invalid operation */
    GPIO_ERROR_IO                   = -8,   /* Reading/writing GPIO */
    GPIO_ERROR_CLOSE                = -9,   /* Closing GPIO */
};

typedef enum gpio_direction {
    GPIO_DIR_IN,        /* Input */
    GPIO_DIR_OUT,       /* Output, initialized to low */
    GPIO_DIR_OUT_LOW,   /* Output, initialized to low */
    GPIO_DIR_OUT_HIGH,  /* Output, initialized to high */
} gpio_direction_t;

typedef enum gpio_edge {
    GPIO_EDGE_NONE,     /* No interrupt edge */
    GPIO_EDGE_RISING,   /* Rising edge 0 -> 1 */
    GPIO_EDGE_FALLING,  /* Falling edge 1 -> 0 */
    GPIO_EDGE_BOTH      /* Both edges X -> !X */
} gpio_edge_t;

typedef enum gpio_bias {
    GPIO_BIAS_DEFAULT,      /* Default line bias */
    GPIO_BIAS_PULL_UP,      /* Pull-up */
    GPIO_BIAS_PULL_DOWN,    /* Pull-down */
    GPIO_BIAS_DISABLE,      /* Disable line bias */
} gpio_bias_t;

typedef enum gpio_drive {
    GPIO_DRIVE_DEFAULT,     /* Default line drive (push-pull) */
    GPIO_DRIVE_OPEN_DRAIN,  /* Open drain */
    GPIO_DRIVE_OPEN_SOURCE, /* Open source */
} gpio_drive_t;

/* Configuration structure for gpio_open_*advanced() functions */
typedef struct gpio_config {
    gpio_direction_t direction;
    gpio_edge_t edge;
    gpio_bias_t bias;
    gpio_drive_t drive;
    bool inverted;
    const char *label; /* Can be NULL for default consumer label */
} gpio_config_t;

typedef struct gpio_handle gpio_t;

/* Primary Functions */
gpio_t *gpio_new(void);
int gpio_open(gpio_t *gpio, const char *path, unsigned int line, gpio_direction_t direction);
int gpio_open_name(gpio_t *gpio, const char *path, const char *name, gpio_direction_t direction);
int gpio_open_advanced(gpio_t *gpio, const char *path, unsigned int line, const gpio_config_t *config);
int gpio_open_name_advanced(gpio_t *gpio, const char *path, const char *name, const gpio_config_t *config);
int gpio_open_sysfs(gpio_t *gpio, unsigned int line, gpio_direction_t direction);
int gpio_read(gpio_t *gpio, bool *value);
int gpio_write(gpio_t *gpio, bool value);
int gpio_poll(gpio_t *gpio, int timeout_ms);
int gpio_close(gpio_t *gpio);
void gpio_free(gpio_t *gpio);

/* Read Event (for character device GPIOs) */
int gpio_read_event(gpio_t *gpio, gpio_edge_t *edge, uint64_t *timestamp);

/* Poll Multiple */
int gpio_poll_multiple(gpio_t **gpios, size_t count, int timeout_ms, bool *gpios_ready);

/* Getters */
int gpio_get_direction(gpio_t *gpio, gpio_direction_t *direction);
int gpio_get_edge(gpio_t *gpio, gpio_edge_t *edge);
int gpio_get_bias(gpio_t *gpio, gpio_bias_t *bias);
int gpio_get_drive(gpio_t *gpio, gpio_drive_t *drive);
int gpio_get_inverted(gpio_t *gpio, bool *inverted);

/* Setters */
int gpio_set_direction(gpio_t *gpio, gpio_direction_t direction);
int gpio_set_edge(gpio_t *gpio, gpio_edge_t edge);
int gpio_set_bias(gpio_t *gpio, gpio_bias_t bias);
int gpio_set_drive(gpio_t *gpio, gpio_drive_t drive);
int gpio_set_inverted(gpio_t *gpio, bool inverted);

/* Miscellaneous Properties */
unsigned int gpio_line(gpio_t *gpio);
int gpio_fd(gpio_t *gpio);
int gpio_name(gpio_t *gpio, char *str, size_t len);
int gpio_label(gpio_t *gpio, char *str, size_t len);
int gpio_chip_fd(gpio_t *gpio);
int gpio_chip_name(gpio_t *gpio, char *str, size_t len);
int gpio_chip_label(gpio_t *gpio, char *str, size_t len);
int gpio_tostring(gpio_t *gpio, char *str, size_t len);

/* Error Handling */
int gpio_errno(gpio_t *gpio);
const char *gpio_errmsg(gpio_t *gpio);

#ifdef __cplusplus
}
#endif

#endif

