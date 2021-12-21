#ifndef __U8G_PORT_H__
#define __U8G_PORT_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include <rtthread.h>
#include <rtdevice.h>
#if defined U8G2_USE_HW_SPI
    #include <drv_spi.h>
#endif

#include <string.h>
#include <u8g2.h>
#include <u8x8.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#if defined U8G2_USE_HW_SPI

    #ifndef U8G2_SPI_BUS_NAME
        #define U8G2_SPI_BUS_NAME            "spi1" 
    #endif

    #ifndef U8G2_SPI_DEVICE_NAME
        #define U8G2_SPI_DEVICE_NAME         "spi10"
    #endif

    uint8_t u8x8_byte_rtthread_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif

#if defined U8G2_USE_HW_I2C

    #ifndef U8G2_I2C_DEVICE_NAME
        #define U8G2_I2C_DEVICE_NAME         "i2c2"
    #endif

    uint8_t u8x8_byte_rtthread_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif

uint8_t u8x8_gpio_and_delay_rtthread(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#ifdef __cplusplus
    }
#endif

#endif /* __U8G_PORT_H__ */
