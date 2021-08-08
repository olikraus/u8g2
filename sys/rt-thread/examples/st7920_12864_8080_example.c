#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <u8g2_port.h>

// You may reference Drivers/drv_gpio.c for pinout
// In u8x8.h #define U8X8_USE_PINS 

#define ST7920_8080_PIN_D0                    31  // PB15
#define ST7920_8080_PIN_D1                    30  // PB14
#define ST7920_8080_PIN_D2                    29  // PB13
#define ST7920_8080_PIN_D3                    28  // PB12
#define ST7920_8080_PIN_D4                    38  // PC6
#define ST7920_8080_PIN_D5                    39  // PC7
#define ST7920_8080_PIN_D6                    40  // PC8
#define ST7920_8080_PIN_D7                    41  // PC9
#define ST7920_8080_PIN_EN                    15  // PA15
#define ST7920_8080_PIN_CS                    U8X8_PIN_NONE
#define ST7920_8080_PIN_DC                    11  // PA11
#define ST7920_8080_PIN_RST                   12  // PA12

void u8x8_SetPin_8Bit_8080(u8x8_t *u8x8, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t wr, uint8_t cs, uint8_t dc, uint8_t reset)
{
    u8x8_SetPin(u8x8, U8X8_PIN_D0, d0);
    u8x8_SetPin(u8x8, U8X8_PIN_D1, d1);
    u8x8_SetPin(u8x8, U8X8_PIN_D2, d2);
    u8x8_SetPin(u8x8, U8X8_PIN_D3, d3);
    u8x8_SetPin(u8x8, U8X8_PIN_D4, d4);
    u8x8_SetPin(u8x8, U8X8_PIN_D5, d5);
    u8x8_SetPin(u8x8, U8X8_PIN_D6, d6);
    u8x8_SetPin(u8x8, U8X8_PIN_D7, d7);
    u8x8_SetPin(u8x8, U8X8_PIN_E, wr);
    u8x8_SetPin(u8x8, U8X8_PIN_CS, cs);
    u8x8_SetPin(u8x8, U8X8_PIN_DC, dc);
    u8x8_SetPin(u8x8, U8X8_PIN_RESET, reset);
}

static void u8g2_st7920_12864_8080_example(int argc,char *argv[])
{
    u8g2_t u8g2;

    // Initialization
    u8g2_Setup_st7920_p_128x64_f(&u8g2, U8G2_R0, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_rtthread);
    u8x8_SetPin_8Bit_8080(u8g2_GetU8x8(&u8g2), 
                          ST7920_8080_PIN_D0, ST7920_8080_PIN_D1, 
                          ST7920_8080_PIN_D2, ST7920_8080_PIN_D3, 
                          ST7920_8080_PIN_D4, ST7920_8080_PIN_D5, 
                          ST7920_8080_PIN_D6, ST7920_8080_PIN_D7, 
                          ST7920_8080_PIN_EN, ST7920_8080_PIN_CS, 
                          ST7920_8080_PIN_DC, ST7920_8080_PIN_RST);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    // Draw Graphics
    /* full buffer example, setup procedure ends in _f */
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_baby_tf);
    u8g2_DrawStr(&u8g2, 1, 18, "U8g2 on RT-Thread");
    u8g2_SendBuffer(&u8g2);

    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603 );
    u8g2_SendBuffer(&u8g2);
}
MSH_CMD_EXPORT(u8g2_st7920_12864_8080_example, st7920 12864 LCD sample);
