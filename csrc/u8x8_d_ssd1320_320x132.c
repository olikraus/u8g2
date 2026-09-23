/*

  u8x8_d_ssd1320.c
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2020, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  


  https://github.com/olikraus/u8g2/issues/1351
  SSD1320: 
    160 x 160 dot matrix
    16 gray scale
  
  Adapted from u8x8_d_ssd1322.c with the command set of the SSD1320 controller
  "official" procedure is described here: https://github.com/olikraus/u8g2/wiki/internal
  
  NOTE: U8x8 does NOT work!
  
*/

#include "u8x8.h"
#include "u8g2.h"
#include "u8x8_d_ssd1320_320x132.h"

extern uint8_t u8x8_d_ssd1320_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#if 0
static const uint8_t u8x8_d_ssd1320_cs1_320x132_nhd_powersave0_seq[] = {
    U8X8_START_TRANSFER(),           /* enable chip, delay is part of the transfer start */
    U8X8_C(0x0af),                   /* ssd1320: display on */
    U8X8_END_TRANSFER(),             /* disable chip */
    U8X8_END()                       /* end of sequence */
};

static const uint8_t u8x8_d_ssd1320_cs1_320x132_nhd_powersave1_seq[] = {
    U8X8_START_TRANSFER(),           /* enable chip, delay is part of the transfer start */
    U8X8_C(0x0ae),                   /* ssd1320: display off */
    U8X8_END_TRANSFER(),             /* disable chip */
    U8X8_END()                       /* end of sequence */
};
#endif

/*=========================================================*/
/* 320x132 */

static const uint8_t u8x8_d_ssd1320_cs1_320x132_nhd_flip0_seq[] = {
    U8X8_START_TRANSFER(),           /* enable chip, delay is part of the transfer start */
    U8X8_C(0x0a0),                   /* remap : page 0  */
    U8X8_C(0xc8),                    /* Set COM Output Scan Direction: normal mode CS1 */
    U8X8_CA(0xd3, 0x0e),             /* CS1 */
    U8X8_END_TRANSFER(),             /* disable chip */
    U8X8_END()                       /* end of sequence */
};

static const uint8_t u8x8_d_ssd1320_cs1_320x132_nhd_flip1_seq[] = {
    U8X8_START_TRANSFER(),           /* enable chip, delay is part of the transfer start */
    U8X8_C(0x0a1),                   /* remap : page 1 */
    U8X8_C(0xc0),                    /* Set COM Output Scan Direction: normal mode CS1 */
    U8X8_CA(0xd3, 0x92),             /* CS1 */
    U8X8_END_TRANSFER(),             /* disable chip */
    U8X8_END()                       /* end of sequence */
};

static const u8x8_display_info_t u8x8_d_ssd1320_cs1_320x132_display_info =
    {
        .chip_enable_level = 0,
        .chip_disable_level = 1,
        .post_chip_enable_wait_ns = 20,
        .pre_chip_disable_wait_ns = 10,
        .reset_pulse_width_ms = 100,       /* ssd1320: 2 us */
        .post_reset_wait_ms = 100,         /* far east OLEDs need much longer setup time */
        .sda_setup_time_ns = 50,           /* ssd1320: 15ns, but cycle time is 100ns, so use 100/2 */
        .sck_pulse_width_ns = 50,          /* ssd1320: 20ns, but cycle time is 100ns, so use 100/2, AVR: below 70: 8 MHz, >= 70 --> 4MHz clock */
        .sck_clock_hz = 10000000UL,        /* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns, increased to 8MHz (issue 215), 10 MHz (issue 301) */
        .spi_mode = 0,                     /* active high, rising edge */
        .i2c_bus_clock_100kHz = 4,
        .data_setup_time_ns = 10,
        .write_pulse_width_ns = 150,       /* ssd1320: cycle time is 300ns, so use 300/2 = 150 */
        .tile_width = 40,                  /* 320 pixels, so we require 40 bytes for this (4bpp * 320 => 160 bytes) / 40 => 4 tiles/line */
        .tile_height = 17,                 /* 132 pixels, 17 pages ???
        .default_x_offset = 0,             /* this is the byte offset (there are two pixel per byte with 4 bit per pixel) */
        .flipmode_x_offset = 0,
        .pixel_width = 320,
        .pixel_height = 132
    };

#if 0
//----------------------------------------------------------------------
//EASTRISING TECHNOLOGY CO,.LTD.//
// Module    : ER-OLED0383-1  3.83"   320*132
// Lanuage   : C51 Code
// Create    : JAVEN
// Date      : Apr-08-2020
// Drive IC  : SSD1320*2
// INTERFACE : 4 wire SPI
// MCU 		 : STC12LE5A60S2
// VDD		 : 3.3V
//----------------------------------------------------------------------

void OLED_Init(void)
{
    OLED_RST_Clr();
    delay_ms(200);
    OLED_RST_Set();

    OLED_WR_Byte(0xae,OLED_CMD);//Display OFF
    OLED_WR_Byte(0xfd,OLED_CMD);//Set Command Lock
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0x20,OLED_CMD);//Set Memory Addressing Mode
    OLED_WR_Byte(0x00,OLED_CMD);
    OLED_WR_Byte(0x25,OLED_CMD);//Set Portrait Addressing Mode
    OLED_WR_Byte(0x00,OLED_CMD);//Normal Addressing Mode
    OLED_WR_Byte(0x81,OLED_CMD);//Set Contrast Control
    OLED_WR_Byte(0x6b,OLED_CMD);
    OLED_WR_Byte1(0xa0,OLED_CMD,1);//Set Seg Remap
    OLED_WR_Byte1(0xa1,OLED_CMD,2);
    OLED_WR_Byte(0xa2,OLED_CMD);//Set Display Start Line
    OLED_WR_Byte(0x00,OLED_CMD);
    OLED_WR_Byte(0xa4,OLED_CMD);//Resume to RAM content display
    OLED_WR_Byte(0xa6,OLED_CMD);//Set Normal Display
    OLED_WR_Byte(0xa8,OLED_CMD);//Set MUX Ratio
    OLED_WR_Byte(0x83,OLED_CMD);//1/132 duty
    OLED_WR_Byte(0xad,OLED_CMD);//Select external or internal IREF
    OLED_WR_Byte(0x10,OLED_CMD);
    OLED_WR_Byte(0xbc,OLED_CMD);//Set Pre-charge voltage
    OLED_WR_Byte(0x1e,OLED_CMD);//
    OLED_WR_Byte(0xbf,OLED_CMD);//Linear LUT
    OLED_WR_Byte1(0xc8,OLED_CMD,1);//Set COM Output Scan Direction
    OLED_WR_Byte1(0xc0,OLED_CMD,2);
    OLED_WR_Byte(0xd3,OLED_CMD);//Set Display Offset
    OLED_WR_Byte1(0x0e,OLED_CMD,1);
    OLED_WR_Byte1(0x92,OLED_CMD,2);
    OLED_WR_Byte(0xd5,OLED_CMD);//Set Display Clock Divide Ratio/Oscillator Frequency
    OLED_WR_Byte(0xc2,OLED_CMD);//85Hz
    OLED_WR_Byte(0xd9,OLED_CMD);//Set Pre-charge Period
    OLED_WR_Byte(0x72,OLED_CMD);//
    OLED_WR_Byte(0xda,OLED_CMD);//Set SEG Pins Hardware Configuration
    OLED_WR_Byte(0x32,OLED_CMD);
    OLED_WR_Byte(0xbd,OLED_CMD);//Set VP
    OLED_WR_Byte(0x03,OLED_CMD);
    OLED_WR_Byte(0xdb,OLED_CMD);//Set VCOMH
    OLED_WR_Byte(0x30,OLED_CMD);
    OLED_Clear(0,0,320,132,0x00);
    OLED_WR_Byte(0xaf,OLED_CMD);//Display on
}
#endif

static const uint8_t u8x8_d_ssd1320_320x132_init_seq[] = {
    U8X8_DLY(1),
    U8X8_START_TRANSFER(),           /* enable chip, delay is part of the transfer start */
    U8X8_DLY(1),

    U8X8_C(0xae),                    /* display off */
    U8X8_CA(0xd5, 0xC2),             /* set display clock divide ratio/oscillator frequency (set clock as 80 frames/sec)  */
    U8X8_CA(0xa8, 0x83),             /* multiplex ratio 1/132 Duty  */
    U8X8_CA(0xa2, 0x00),             /* display start line */

    U8X8_C(0xa0),                    /* Set Segment Re-Map: column address 0 mapped to SEG0  CS1 */
    // U8X8_C(0xa1),	      	        /* Set Segment Re-Map: column address 0 mapped to SEG0  CS2 */

    U8X8_C(0xc8),                    /* Set COM Output Scan Direction: normal mode CS1 */
    // U8X8_C(0xc0),		            /* Set COM Output Scan Direction: normal mode CS2 */

    U8X8_CA(0xad, 0x10),             /* select Iref: 0x00 external (reset default), 0x10 internal */
    U8X8_CA(0xbc, 0x1e),             /* pre-charge voltage level 0x00..0x1f, reset default: 0x1e */
    U8X8_C(0xbf),                    /* select linear LUT */
    U8X8_CA(0xd5, 0xc2),             /* Bit 0..3: clock ratio 1, 2, 4, 8, ...256, reset=0x1, Bit 4..7: F_osc 0..15 */
    U8X8_CA(0xd9, 0x72),             /* Set Phase 1&2 Length, Bit 0..3: Phase 1, Bit 4..7: Phase 2, reset default 0x72 */
    U8X8_CA(0xbd, 0x03),             /* from the vendor init sequence */
    U8X8_CA(0xdb, 0x30),             /* VCOMH Deselect Level */

    U8X8_CA(0xd3, 0x0e),             /* CS1 */
    // U8X8_CA(0xd3, 0x92),             /* CS2 */

    U8X8_CA(0xda, 0x12),             /* Set SEG Pins Hardware Configuration:  */
    U8X8_CA(0x81, 0x6b),             /* contrast */
    // U8X8_CA(0xd9, 0x22),			    /* Set Phase Length */
    // U8X8_CA(0xdb, 0x30),		        /* VCOMH Deselect Level */
    // U8X8_CA(0xad, 0x10),			    /* Internal IREF Enable */
    U8X8_CA(0x20, 0x00),             /* Memory Addressing Mode: Horizontal */
    // U8X8_CA(0x8d, 0x01),			    /* unknown in SSD1320 datasheet, disable internal charge pump 1 */
    // U8X8_CA(0xac, 0x00),			    /* unknown in SSD1320 datasheet, disable internal charge pump 2 */
    U8X8_C(0xa4),                    /* display RAM on */
    U8X8_C(0xa6),                    /* normal display */

    U8X8_DLY(1),                     /* delay 2ms */
    U8X8_END_TRANSFER(),             /* disable chip */
    U8X8_END()                       /* end of sequence */
};

uint8_t u8x8_d_ssd1320_320x132(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_DISPLAY_SETUP_MEMORY:
            u8x8_d_helper_display_setup_memory(u8x8, &u8x8_d_ssd1320_cs1_320x132_display_info);
            break;

        case U8X8_MSG_DISPLAY_INIT:
            u8x8_d_helper_display_init(u8x8);
            // u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1320_cs1_320x132_init_seq);
            u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1320_320x132_init_seq);
            break;

        case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
            if (arg_int == 0)
            {
                u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1320_cs1_320x132_nhd_flip0_seq);
                u8x8->x_offset = u8x8->display_info->default_x_offset;
            }
            else
            {
                u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1320_cs1_320x132_nhd_flip1_seq);
                u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
            }
            break;

        default:
            return u8x8_d_ssd1320_common(u8x8, msg, arg_int, arg_ptr);
    }
    return 1;
}

uint8_t *u8g2_m_40_17_1(uint8_t *page_cnt)
{
#ifdef U8G2_USE_DYNAMIC_ALLOC
    *page_cnt = 1;
  return 0;
#else
    static uint8_t buf[320];  // 1 * 320
    *page_cnt = 1;
    return buf;
#endif
}
uint8_t *u8g2_m_40_17_2(uint8_t *page_cnt)
{
#ifdef U8G2_USE_DYNAMIC_ALLOC
    *page_cnt = 2;
  return 0;
#else
    static uint8_t buf[640];  // 2 * 320
    *page_cnt = 2;
    return buf;
#endif
}
uint8_t *u8g2_m_40_17_f(uint8_t *page_cnt)
{
#ifdef U8G2_USE_DYNAMIC_ALLOC
    *page_cnt = 17;
  return 0;
#else
    static uint8_t buf[5440];  // 17 * 320
    *page_cnt = 17;
    return buf;
#endif
}


#include <zephyr.h>
#include <device.h>
#include <drivers/spi.h>
#include "hardware/spi.h"
#include "hardware/gpio.h"


/*=============================================*/
/*=== ARDUINO GPIO & DELAY ===*/

#ifdef U8X8_USE_PINS
uint8_t u8x8_gpio_and_delay_picosdk(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    uint8_t clk_pin, dat_pin;
    switch (msg)
    {
        // Using at least CS and DC:
        // U8X8_PIN_SPI_CLOCK,
        // U8X8_PIN_SDA_CLOCK, // MOSI
        // U8X8_PIN_CS,
        // U8X8_PIN_DC,
        // U8X8_PIN_RESET = U8X8_PIN_NONE

        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            //
            clk_pin = u8x8->pins[U8X8_MSG_GPIO_D0]; // SPI_CLOCK == SCK
            dat_pin = u8x8->pins[U8X8_MSG_GPIO_D1]; // SPI_DATA == MOSI/TX
            // NB: MISO/RX is not used.
            spi_inst_t *spi_inst;

            if ((dat_pin == 3 || dat_pin == 7 || dat_pin == 19 || dat_pin == 23)
                && (clk_pin == 2 || clk_pin == 6 || clk_pin == 18 || clk_pin == 22))
            {
                spi_inst = spi0;
            }
            else if ((dat_pin == 11 || dat_pin == 15 || dat_pin == 27)
                && (clk_pin == 10 || clk_pin == 14 || clk_pin == 26))
            {
                spi_inst = spi1;
            }
            else
            {
                //printk("SPI device %s is not ready\n", spi->name);
                return 0;
            }
            
            gpio_init(u8x8->pins[U8X8_PIN_DC]);
            gpio_set_function(u8x8->pins[U8X8_MSG_GPIO_D0], GPIO_FUNC_SPI);
            gpio_set_function(u8x8->pins[U8X8_MSG_GPIO_D0], GPIO_FUNC_SPI);
            gpio_set_function(u8x8->pins[U8X8_MSG_GPIO_D1], GPIO_FUNC_SPI);
            gpio_set_function(u8x8->pins[U8X8_PIN_CS], GPIO_FUNC_SPI);
            gpio_set_function(u8x8->pins[U8X8_PIN_DC], GPIO_FUNC_SIO);
            gpio_set_dir(u8x8->pins[U8X8_PIN_DC], true);
            gpio_set_dir(u8x8->pins[U8X8_PIN_CS], true);
            gpio_set_drive_strength(u8x8->pins[U8X8_PIN_DC], GPIO_DRIVE_STRENGTH_4MA);
            gpio_set_drive_strength(u8x8->pins[U8X8_PIN_CS], GPIO_DRIVE_STRENGTH_4MA);

            int actual_baud = spi_init(spi_inst, u8x8->bus_clock);
            spi_set_slave(spi_inst, 0);
            spi_set_format(spi_inst, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
            break;

        case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
            gpio_put(u8x8->pins[U8X8_PIN_DC, arg_int);
            break;

        case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
            gpio_put(u8x8->pins[U8X8_PIN_CS, arg_int);
            break;

        case U8X8_MSG_DELAY_NANO:
            busy_wait_us_32(arg_int << 10); /* should be *1000, *1024 is good enough */
            break;

        case U8X8_MSG_DELAY_10MICRO:
            /* not used at the moment */
            busy_wait_us_32(arg_int * 10);
            break;

        case U8X8_MSG_DELAY_100NANO:
            /* not used at the moment */
            busy_wait_us_32((arg_int < 12) ? 1 : (arg_int / 10));
            break;

        case U8X8_MSG_DELAY_MILLI:
            // when in milliseconds, we can finally suspend, rather than busy-wait.
            busy_wait_ms(arg_int);
            break;

        case U8X8_MSG_DELAY_I2C:
            /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
            busy_wait_us_32(arg_int <= 2 ? 5 : 2);
            break;

        default:
            u8x8_SetGPIOResult(u8x8, 1);
            return 0;
    }
    return 1;
}


uint8_t u8x8_gpio_and_delay_zephyr(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    uint32_t t;
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            // gpio not supported here.
            break;

        case U8X8_MSG_DELAY_NANO:
            /* wait 0us for any ns delay (range 0..255ns) */
            /*
             * No platform code for (ns) delays: use (us) delay instead.
             *
             * At 133MHz, 1 clock cycle is 7.5ns. busy_wait_us_32 is:
             *      5 setup instrs + 4 per timer_hw loop + 1 return.
             *      10 instrs => ~75ns min delay.
             *
             * Longest delay in quick check of sources is 100ns, tho one commented
             * out was 200ns. So if requested for >80ns do a full 1us delay, else
             * skip through with '0' delay = ~80ns including this fn too.
             */
            if (arg_int > 80/*ns*/)
                busy_wait_us_32(1/*us*/);
            else
                busy_wait_us_32(0/*us, ~80ns eff */);
            break;

        case U8X8_MSG_DELAY_100NANO:
            /* not used at the moment... range 100ns to 25500ns = 25.5us */
            /* Wait 1us for any delay from 0..1.1us, and /10 for 1.2us to 25.5us */
            busy_wait_us_32((arg_int < 12) ? 1 : (arg_int / 10));
            break;

        case U8X8_MSG_DELAY_10MICRO:
            /* not used at the moment... range from 10us to 2550us = 2.55ms */
            busy_wait_us_32(arg_int * 10);
            break;

        case U8X8_MSG_DELAY_MILLI:
            // when in milliseconds, we can finally suspend, rather than busy-wait.
            k_msleep(arg_int);
            break;

        case U8X8_MSG_DELAY_I2C:
            /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
            busy_wait_us_32(arg_int <= 2 ? 5 : 2);
            break;

        case U8X8_MSG_GPIO_I2C_CLOCK:
        case U8X8_MSG_GPIO_I2C_DATA:
            // i2c not supported here.
            break;

        default:
            return 0;
    }
    return 1;
}
#endif // U8X8_USE_PINS

/*=============================================*/
/*=== 4 WIRE HARDWARE SPI ===*/

#ifdef U8X8_USE_PINS

struct spi_config u8x8_byte_zephyr_spi_cfg = { 0 };

static int u8x8_byte_zephyr_spi_write(const struct device *spi, void *data, size_t len)
{
    struct spi_buf bufs[] = {
        {
            .buf = data,
            .len = len
        }
    };
    struct spi_buf_set tx = {
        .buffers = bufs
    };

    return spi_write(spi, &u8x8_byte_zephyr_spi_cfg, &tx);
}
static int u8x8_byte_zephyr_spi_read(const struct device *spi, void *data, size_t len)
{
    struct spi_buf bufs[] = {
        {
            .buf = data,
            .len = len
        }
    };
    struct spi_buf_set tx = {
        .buffers = bufs,
        .count = 1
    };

    return spi_read(spi, &u8x8_byte_zephyr_spi_cfg, &tx);
}

inline uint8_t u8x8_byte_zephyr_spi_ll(u8x8_t *u8x8, uint8_t unit, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
#ifdef U8X8_HAVE_HW_SPI
    uint8_t *data;
    struct device *spi = NULL;
    spi_inst_t *spi_inst = NULL;

    if (unit == 0)
        spi = DEVICE_DT_GET(DT_ALIAS(spi_0));
#ifdef U8X8_HAVE_2ND_HW_SPI
    elseif (unit == 1)
        spi = DEVICE_DT_GET(DT_ALIAS(spi_1));
#endif
    if (!device_is_ready(spi))
    {
        //printk("SPI device %s is not ready\n", spi->name);
        return 0;
    }

    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND:
            u8x8_byte_zephyr_spi_write(spi, (uint8_t *) arg_ptr, arg_int);
            break;

        case U8X8_MSG_BYTE_INIT:
            if (u8x8->bus_clock == 0)    /* issue 769 */
                u8x8->bus_clock = u8x8->display_info->sck_clock_hz;

            // Set SPI configuration. Most of these are default values.
            u8x8_byte_zephyr_spi_cfg.operation
                = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_LINES_SINGLE;
            u8x8_byte_zephyr_spi_cfg.frequency = u8x8->bus_clock;

            /* disable chipselect */
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);

            /* setup hardware with SPI.begin() instead of previous digitalWrite() and pinMode() calls */
            spi_init();
            break;

        case U8X8_MSG_BYTE_SET_DC:
            u8x8_gpio_SetDC(u8x8, arg_int);
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            /* SPI mode has to be mapped to the mode of the current controller */
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
            break;

        default:
            return 0;
    }
#endif    /* U8X8_HAVE_HW_SPI */
    return 1;
}


inline uint8_t u8x8_byte_picosdk_spi_ll(u8x8_t *u8x8, uint8_t unit, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
#ifdef U8X8_HAVE_HW_SPI
    uint8_t *data;
    spi_inst_t *spi_inst = NULL;

    if (unit == 0)
        spi_inst = spi0;
#ifdef U8X8_HAVE_2ND_HW_SPI
    elseif (unit == 1)
        spi_inst = spi1;
#endif
    if (spi_is_busy(spi_inst))
    {
        printk("SPI device %c is not ready\n", (spi_inst == spi0) ? '0' : '1');
        return 0;
    }

    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND:
            spi_write_blocking(spi_inst, (uint8_t *) arg_ptr, arg_int);
            break;

        case U8X8_MSG_BYTE_INIT:
            if (u8x8->bus_clock == 0)
                u8x8->bus_clock = u8x8->display_info->sck_clock_hz;

            /* disable chipselect */
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);

            spi_init(spi_inst, u8x8->bus_clock);
            /* these are the defaults: */
            //spi_set_slave(spi_inst, 0);
            //spi_set_format(spi_inst, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
            break;

        case U8X8_MSG_BYTE_SET_DC:
            u8x8_gpio_SetDC(u8x8, arg_int);
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
            break;

        default:
            return 0;
    }
#endif    /* U8X8_HAVE_HW_SPI */
    return 1;
}
#endif

uint8_t u8x8_byte_picosdk_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
#ifdef U8X8_HAVE_HW_SPI
    return u8x8_byte_zephyr_spi_ll(u8x8, 0, msg, arg_int, arg_ptr);
#endif    /* U8X8_HAVE_HW_SPI */
}

uint8_t u8x8_byte_picosdk_2nd_hw_spi(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
#ifdef U8X8_HAVE_2ND_HW_SPI
    return u8x8_byte_zephyr_spi_ll(u8x8, 1, msg, arg_int, arg_ptr);
#endif    /* U8X8_HAVE_2ND_HW_SPI */
}

/* ssd1320 1 */
void u8g2_Setup_ssd1320_320x132_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
    uint8_t tile_buf_height;
    uint8_t *buf;
    u8g2_SetupDisplay(u8g2, u8x8_d_ssd1320_320x132, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
    buf = u8g2_m_40_17_1(&tile_buf_height);
    u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}

/* ssd1320 2 */
void u8g2_Setup_ssd1320_320x132_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
    uint8_t tile_buf_height;
    uint8_t *buf;
    u8g2_SetupDisplay(u8g2, u8x8_d_ssd1320_320x132, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
    buf = u8g2_m_40_17_2(&tile_buf_height);
    u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}

/* ssd1320 f */
void u8g2_Setup_ssd1320_320x132_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
    uint8_t tile_buf_height;
    uint8_t *buf;
    u8g2_SetupDisplay(u8g2, u8x8_d_ssd1320_320x132, u8x8_cad_001, byte_cb, gpio_and_delay_cb);
    buf = u8g2_m_40_17_f(&tile_buf_height);
    u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}
