
/*
 * RP2040/RP2350 HAL example for u8g2
 * Added in response to https://github.com/olikraus/u8g2/issues/2159
 * This is a reference implementation intended for adaptation.
 */


#ifndef U8G2_PICO_HAL_H
#define U8G2_PICO_HAL_H

#include <stdint.h>
#include "u8g2.h"
#include "hardware/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 *  Interface selection
 * ============================================================ */

typedef enum {
    U8G2_PICO_IF_HW_SPI = 0,
    U8G2_PICO_IF_SW_SPI      /* reserved for future */
} u8g2_pico_interface_t;


/* ============================================================
 *  Hardware SPI configuration
 * ============================================================ */

typedef struct {
    spi_inst_t *spi;      /* spi0 or spi1 */
    uint32_t    baudrate;

    uint8_t pin_sck;
    uint8_t pin_mosi;
    uint8_t pin_cs;
    uint8_t pin_dc;
    uint8_t pin_rst;
} u8g2_pico_spi_hw_config_t;


/* ============================================================
 *  Unified configuration object
 * ============================================================ */

typedef struct {
    u8g2_pico_interface_t interface;
    u8g2_pico_spi_hw_config_t hw_spi;
} u8g2_pico_config_t;


/* ============================================================
 *  Display setup function type
 *
 *  This matches u8g2_Setup_xxx() signatures exactly.
 * ============================================================ */

typedef void (*u8g2_pico_setup_fn)(
    u8g2_t *u8g2,
    const u8g2_cb_t *rotation,
    u8x8_msg_cb byte_cb,
    u8x8_msg_cb gpio_cb
);


/* ============================================================
 *  Public HAL API
 * ============================================================ */

/*
 * Initialize U8g2 on RP2040 using the Pico SDK.
 *
 * Responsibilities:
 *  - configure SPI and GPIO
 *  - install U8g2 callbacks
 *  - initialize and wake the display
 *
 * After this call:
 *  - u8g2 is fully ready for drawing
 *  - no further hardware setup is required
 *
 * @param u8g2      Pointer to u8g2 instance
 * @param setup_fn Display-specific u8g2_Setup_xxx() function
 * @param cfg       Pico hardware configuration
 */
void u8g2_pico_init(
    u8g2_t *u8g2,
    u8g2_pico_setup_fn setup_fn,
    const u8g2_pico_config_t *cfg
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* U8G2_PICO_HAL_H */
