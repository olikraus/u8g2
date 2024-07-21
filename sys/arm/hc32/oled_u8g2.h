/*!
    \file  oled_u8g2.h
    \brief the header file of u8g2 oled
    \author Ken
    \version 2024-06-11, V1.0.0
*/
#ifndef __MD_OLED_U8G2_H
#define __MD_OLED_U8G2_H

#include "u8g2.h"
#include "u8x8.h"
#include "stdlib.h"

// OLED控制用函数
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
void draw(u8g2_t *u8g2);

void test_oled_u8g2(uint8_t num);

#endif
