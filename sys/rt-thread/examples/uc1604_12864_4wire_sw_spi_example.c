#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <u8g2_port.h>

// You may reference Drivers/drv_gpio.c for pinout
// In u8x8.h #define U8X8_USE_PINS 

#define LCD_SPI_PIN_CLK                    26  // PB12
#define LCD_SPI_PIN_MOSI                   28  // PB15
#define LCD_SPI_PIN_RES                    33  // PB12
#define LCD_SPI_PIN_DC                     27  // PB14
#define LCD_SPI_PIN_CS                     32  // PB11

static void u8g2_uc1604_12864_4wire_sw_spi_example(int argc,char *argv[])
{
    u8g2_t u8g2;

    // Initialization
    u8g2_Setup_uc1604_jlx12864_f( &u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_rtthread);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_SPI_CLOCK, LCD_SPI_PIN_CLK);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_SPI_DATA, LCD_SPI_PIN_MOSI);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_CS, LCD_SPI_PIN_CS);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_DC, LCD_SPI_PIN_DC);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_RESET, LCD_SPI_PIN_RES);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    // Draw Graphics
    /* full buffer example, setup procedure ends in _f */
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&u8g2, 1, 18, "U8g2 on RT-Thread");
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);                     // choose a suitable font
    u8g2_DrawStr(&u8g2, 1, 36, "github.com/olikraus/u8g2");    // write something to the internal memory
    u8g2_SendBuffer(&u8g2);

    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603 );
    u8g2_SendBuffer(&u8g2);
}
MSH_CMD_EXPORT(u8g2_uc1604_12864_4wire_sw_spi_example, sw 4wire spi uc1604 sample);
