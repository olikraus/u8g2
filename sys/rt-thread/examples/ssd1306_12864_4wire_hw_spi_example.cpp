#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <U8g2lib.h>

// You may reference Drivers/drv_gpio.c for pinout
// In u8x8.h #define U8X8_USE_PINS 

#define OLED_SPI_PIN_RES                     2  // PA2
#define OLED_SPI_PIN_DC                      1  // PA1
#define OLED_SPI_PIN_CS                      0  // PA0

// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for all supported devices
static U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0,
                                          /* cs=*/ OLED_SPI_PIN_CS,
                                          /* dc=*/ OLED_SPI_PIN_DC,
                                          /* reset=*/ OLED_SPI_PIN_RES);
                                          // same as the NONAME variant, but may solve the "every 2nd line skipped" problem

static void u8g2_ssd1306_12864_4wire_hw_spi_example(int argc,char *argv[])
{
    u8g2.begin();
    u8g2.clearBuffer();                         // clear the internal memory
    u8g2.setFont(u8g2_font_6x13_tr);            // choose a suitable font
    u8g2.drawStr(1, 18, "U8g2 on RT-Thread");   // write something to the internal memory
    u8g2.setFont(u8g2_font_5x8_tr);            // choose a suitable font
    u8g2.drawStr(1, 56, "2.29 Milestone");   // write something to the internal memory
    u8g2.sendBuffer();                          // transfer internal memory to the display
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawGlyph(112, 56, 0x2603 );
    u8g2.sendBuffer();
}
MSH_CMD_EXPORT(u8g2_ssd1306_12864_4wire_hw_spi_example, hw 4wire spi ssd1306 sample);
