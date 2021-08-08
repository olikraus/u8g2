#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <U8g2lib.h>

// You may reference Drivers/drv_gpio.c for pinout
// In u8x8.h #define U8X8_USE_PINS 
#define OLED_I2C_PIN_SCL                    22  // PB6
#define OLED_I2C_PIN_SDA                    23  // PB7

// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for all supported devices
static U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,
                                         /* clock=*/ OLED_I2C_PIN_SCL,
                                         /* data=*/ OLED_I2C_PIN_SDA,
                                         /* reset=*/ U8X8_PIN_NONE);
                                         // All Boards without Reset of the Display

static void u8g2_ssd1306_12864_sw_i2c_example(int argc,char *argv[])
{
    u8g2.begin();
    u8g2.clearBuffer();                         // clear the internal memory
    u8g2.setFont(u8g2_font_6x13_tr);            // choose a suitable font
    u8g2.drawStr(1, 18, "U8g2 on RT-Thread");   // write something to the internal memory
    u8g2.sendBuffer();                          // transfer internal memory to the display
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawGlyph(112, 56, 0x2603 );
    u8g2.sendBuffer();
}
MSH_CMD_EXPORT(u8g2_ssd1306_12864_sw_i2c_example, i2c ssd1306 software i2c sample);
