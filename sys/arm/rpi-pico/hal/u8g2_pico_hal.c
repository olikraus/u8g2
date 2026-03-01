/*
 * RP2040/RP2350 HAL example for u8g2
 * Added in response to https://github.com/olikraus/u8g2/issues/2159
 * This is a reference implementation intended for adaptation.
 */




#include "u8g2_pico_hal.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

static u8g2_pico_spi_hw_config_t g_hw_cfg;

/* ------------------------------------------------------------
 * Forward declarations of u8g2 callbacks
 * ------------------------------------------------------------ */

static uint8_t u8x8_byte_pico_hw_spi(
    u8x8_t *u8x8,
    uint8_t msg,
    uint8_t arg_int,
    void *arg_ptr);

static uint8_t u8x8_gpio_and_delay_pico(
    u8x8_t *u8x8,
    uint8_t msg,
    uint8_t arg_int,
    void *arg_ptr);

/* ------------------------------------------------------------
 * Public HAL API
 * ------------------------------------------------------------ */

void u8g2_pico_init(
    u8g2_t *u8g2,
    u8g2_pico_setup_fn setup_fn,
    const u8g2_pico_config_t *cfg)
{
    if (cfg->interface != U8G2_PICO_IF_HW_SPI)
        return;

    g_hw_cfg = cfg->hw_spi;

    setup_fn(
        u8g2,
        U8G2_R3,
        u8x8_byte_pico_hw_spi,
        u8x8_gpio_and_delay_pico
    );

    
    u8g2->u8x8.user_ptr = &g_hw_cfg;

    u8g2_InitDisplay(u8g2);
    u8g2_SetPowerSave(u8g2, 0);
}

static uint8_t u8x8_gpio_and_delay_pico(
    u8x8_t *u8x8,
    uint8_t msg,
    uint8_t arg_int,
    void *arg_ptr)
{

    const u8g2_pico_spi_hw_config_t *cfg =
        (const u8g2_pico_spi_hw_config_t *)u8x8->user_ptr;

    switch (msg)
    {

    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        spi_init(cfg->spi, cfg->baudrate);
        spi_set_format(cfg->spi, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

        gpio_set_function(cfg->pin_sck, GPIO_FUNC_SPI);
        gpio_set_function(cfg->pin_mosi, GPIO_FUNC_SPI);

        gpio_init(cfg->pin_cs);
        gpio_init(cfg->pin_dc);
        gpio_init(cfg->pin_rst);

        gpio_set_dir(cfg->pin_cs, GPIO_OUT);
        gpio_set_dir(cfg->pin_dc, GPIO_OUT);
        gpio_set_dir(cfg->pin_rst, GPIO_OUT);

        gpio_put(cfg->pin_cs, 1);
        gpio_put(cfg->pin_rst, 1);
        break;

    case U8X8_MSG_DELAY_NANO: // delay arg_int * 1 nano second
        sleep_us(arg_int);    // 1000 times slower, though generally fine in practice given rp2040 has no `sleep_ns()`
        break;
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        sleep_us(arg_int);
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        sleep_us(arg_int * 10);
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        sleep_ms(arg_int);
        break;
    case U8X8_MSG_GPIO_CS: // CS (chip select) pin: Output level in arg_int
        gpio_put(cfg->pin_cs, arg_int);
        break;
    case U8X8_MSG_GPIO_DC: // DC (data/cmd, A0, register select) pin: Output level
        gpio_put(cfg->pin_dc, arg_int);
        break;
    case U8X8_MSG_GPIO_RESET:            // Reset pin: Output level in arg_int
        gpio_put(cfg->pin_rst, arg_int); // printf("U8X8_MSG_GPIO_RESET %d\n", arg_int);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }

    return 1;
}

static uint8_t u8x8_byte_pico_hw_spi(
    u8x8_t *u8x8,
    uint8_t msg,
    uint8_t arg_int,
    void *arg_ptr)
{
    const u8g2_pico_spi_hw_config_t *cfg =
        (const u8g2_pico_spi_hw_config_t *)u8x8->user_ptr;

    switch (msg)
    {
    case U8X8_MSG_BYTE_INIT:
        // Ensure CS is inactive
        u8x8_gpio_SetCS(u8x8,
                        u8x8->display_info->chip_disable_level);
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        // Assert CS
        u8x8_gpio_SetCS(u8x8,
                        u8x8->display_info->chip_enable_level);

        // Respect display timing
        u8x8->gpio_and_delay_cb(
            u8x8,
            U8X8_MSG_DELAY_NANO,
            u8x8->display_info->post_chip_enable_wait_ns,
            NULL);
        break;

    case U8X8_MSG_BYTE_SEND:
        spi_write_blocking(
            cfg->spi,
            (const uint8_t *)arg_ptr,
            arg_int);
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        // Respect display timing
        u8x8->gpio_and_delay_cb(
            u8x8,
            U8X8_MSG_DELAY_NANO,
            u8x8->display_info->pre_chip_disable_wait_ns,
            NULL);

        // Deassert CS
        u8x8_gpio_SetCS(u8x8,
                        u8x8->display_info->chip_disable_level);
        break;

    case U8X8_MSG_BYTE_SET_DC:
        u8x8_gpio_SetDC(u8x8, arg_int);
        break;

    default:
        return 0;
    }

    return 1;
}
