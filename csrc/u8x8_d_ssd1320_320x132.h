//
// Created by Ruth Ivimey-Cook on 30/04/2022.
//

#ifndef U8X8_D_SSD1320_320X132_H
#define U8X8_D_SSD1320_320X132_H

#include "u8x8.h"
#include "u8g2.h"

#define U8X8_USE_PINS
#define U8X8_HAVE_HW_SPI

extern "C" {
void u8g2_Setup_ssd1320_320x132_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1320_320x132_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1320_320x132_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
uint8_t u8x8_gpio_and_delay_zephyr(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
uint8_t u8x8_byte_zephyr_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
}

#ifdef __cplusplus
#include "U8g2lib.h"

class U8G2_SSD1320_320X132_F_4W_HW_SPI: public U8G2
{
public:
    inline U8G2_SSD1320_320X132_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE)
        : U8G2()
    {
        u8g2_Setup_ssd1320_320x132_f(&u8g2, rotation, u8x8_byte_zephyr_hw_spi, u8x8_gpio_and_delay_zephyr);
        u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
    }
};

class U8G2_SSD1320_320X132_1_4W_HW_SPI: public U8G2
{
public:
    U8G2_SSD1320_320X132_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE)
        : U8G2()
    {
        u8g2_Setup_ssd1320_320x132_1(&u8g2, rotation, u8x8_byte_zephyr_hw_spi, u8x8_gpio_and_delay_zephyr);
        u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
    }
};

class U8G2_SSD1320_320X132_2_4W_HW_SPI: public U8G2
{
public:
    U8G2_SSD1320_320X132_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE)
        : U8G2()
    {
        u8g2_Setup_ssd1320_320x132_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_zephyr);
        u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
    }
};

#endif
#endif //U8X8_D_SSD1320_320X132_H
