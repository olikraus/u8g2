#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <U8g2lib.h>

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

// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for all supported devices
static U8G2_ST7920_128X64_F_8080 u8g2(U8G2_R0, 
                                      ST7920_8080_PIN_D0, 
                                      ST7920_8080_PIN_D1, 
                                      ST7920_8080_PIN_D2, 
                                      ST7920_8080_PIN_D3, 
                                      ST7920_8080_PIN_D4, 
                                      ST7920_8080_PIN_D5, 
                                      ST7920_8080_PIN_D6, 
                                      ST7920_8080_PIN_D7, 
                                      /*enable=*/ ST7920_8080_PIN_EN, 
                                      /*cs=*/ ST7920_8080_PIN_CS, 
                                      /*dc=*/ ST7920_8080_PIN_DC, 
                                      /*reset=*/ ST7920_8080_PIN_RST);

static void u8g2_st7920_12864_8080_example(int argc,char *argv[])
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
MSH_CMD_EXPORT(u8g2_st7920_12864_8080_example, st7920 12864 LCD sample);
