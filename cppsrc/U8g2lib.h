/*
  
  U8g2lib.h
  
  C++ Arduino wrapper for the u8g2 struct and c functions for the u8g2 library

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
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


  Note:
  U8x8lib.h is included for the declaration of the helper functions in U8x8lib.cpp.
  U8g2 class is based on the u8g2 struct from u8g2.h, the U8x8 class from U8x8lib.h is not used.



*/


#ifndef _U8G2LIB_HH
#define _U8G2LIB_HH

#include <Arduino.h>
#include <Print.h>
#include <U8x8lib.h>

#include "u8g2.h"

class U8G2 : public Print
{
  protected:
    u8g2_t u8g2;
  public:
    u8g2_uint_t tx, ty;
  
    U8G2(void) { home(); }
    u8x8_t *getU8x8(void) { return u8g2_GetU8x8(&u8g2); }
    u8g2_t *getU8g2(void) { return &u8g2; }

    /* u8x8 interface */
    uint8_t getCols(void) { return u8x8_GetCols(u8g2_GetU8x8(&u8g2)); }
    uint8_t getRows(void) { return u8x8_GetRows(u8g2_GetU8x8(&u8g2)); }

    void initDisplay(void) {
      u8g2_InitDisplay(&u8g2); }
      
    void clearDisplay(void) {
      u8g2_ClearDisplay(&u8g2); }
      
    void setPowerSave(uint8_t is_enable) {
      u8g2_SetPowerSave(&u8g2, is_enable); }
      
    void setFlipMode(uint8_t mode) {
      u8g2_SetFlipMode(&u8g2, mode); }

    void setContrast(uint8_t value) {
      u8g2_SetContrast(&u8g2, value); }
    
    void begin(void) {
      initDisplay(); clearDisplay(); setPowerSave(0); }
      

    /* u8g2  */

      
    u8g2_uint_t getDisplayHeight() { return u8g2_GetDisplayHeight(&u8g2); }
    u8g2_uint_t getDisplayWidth() { return u8g2_GetDisplayWidth(&u8g2); }

    
    
    void firstPage(void) { u8g2_FirstPage(&u8g2); }
    uint8_t nextPage(void) { return u8g2_NextPage(&u8g2); }
    
    void sendBuffer(void) { u8g2_SendBuffer(&u8g2); }
    void clearBuffer(void) { u8g2_ClearBuffer(&u8g2); }    

    void setFont(const uint8_t  *font) {u8g2_SetFont(&u8g2, font); }
    void setFontMode(uint8_t  is_transparent) {u8g2_SetFontMode(&u8g2, is_transparent); }
    void setFontDirection(uint8_t dir) {u8g2_SetFontDirection(&u8g2, dir); }


/*
uint8_t u8g2_IsGlyph(u8g2_t *u8g2, uint16_t requested_encoding);
int8_t u8g2_GetGlyphWidth(u8g2_t *u8g2, uint16_t requested_encoding);
u8g2_uint_t u8g2_GetStrWidth(u8g2_t *u8g2, const char *s);
u8g2_uint_t u8g2_GetUTF8Width(u8g2_t *u8g2, const char *str);
*/
    
    u8g2_uint_t drawGlyph(u8g2_uint_t x, u8g2_uint_t y, uint16_t encoding) { return u8g2_DrawGlyph(&u8g2, x, y, encoding); }    
    u8g2_uint_t drawStr(u8g2_uint_t x, u8g2_uint_t y, const char *s) { return u8g2_DrawStr(&u8g2, x, y, s); }
    u8g2_uint_t drawUTF8(u8g2_uint_t x, u8g2_uint_t y, const char *s) { return u8g2_DrawUTF8(&u8g2, x, y, s); }
    
    u8g2_uint_t getStrWidth(const char *s) { return u8g2_GetStrWidth(&u8g2, s); }
    u8g2_uint_t getUTF8Width(const char *s) { return u8g2_GetUTF8Width(&u8g2, s); }
    
    
    void drawHVLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir) {
      u8g2_DrawHVLine(&u8g2, x, y, len, dir); }
    void drawHLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w) { u8g2_DrawHLine(&u8g2, x, y, w); }
    void drawVLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t h) { u8g2_DrawVLine(&u8g2, x, y, h); }
    
    /* virtual function for print base class */    
    size_t write(uint8_t v) {
      tx += u8g2_DrawGlyph(&u8g2, tx, ty, v);
      return 1;
     }

     /* LiquidCrystal compatible functions */
    void home(void) { tx = 0; ty = 0; }
    void clear(void) { clearBuffer(); home(); }
    void noDisplay(void) { u8g2_SetPowerSave(&u8g2, 1); }
    void display(void) { u8g2_SetPowerSave(&u8g2, 0); }
    void setCursor(u8g2_uint_t x, u8g2_uint_t y) { tx = x; ty = y; }
 
};

/* 
  U8G2_<controller>_<display>_<memory>_<communication> 
  memory
    "1"	one page
    "2"	two pages
    "f"	full frame buffer
  communication
    "SW SPI"

*/



/* Arduino constructor list start */
/* generated code (codebuild), u8g2 project */
class U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_1_6800 : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_1_8080 : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, rotation, u8x8_byte_ssd13xx_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SSD13xx_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_6800 : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_8080 : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, rotation, u8x8_byte_ssd13xx_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SSD13xx_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_6800 : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_8080 : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_ssd13xx_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SSD13xx_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST7920_192X32_1_6800 : public U8G2 {
  public: U8G2_ST7920_192X32_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_192x32_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_1_8080 : public U8G2 {
  public: U8G2_ST7920_192X32_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_192x32_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_2_6800 : public U8G2 {
  public: U8G2_ST7920_192X32_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_192x32_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_2_8080 : public U8G2 {
  public: U8G2_ST7920_192X32_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_192x32_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_F_6800 : public U8G2 {
  public: U8G2_ST7920_192X32_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_192x32_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_F_8080 : public U8G2 {
  public: U8G2_ST7920_192X32_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_192x32_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_1(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1701_DOGS102_1_6800 : public U8G2 {
  public: U8G2_UC1701_DOGS102_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_1_8080 : public U8G2 {
  public: U8G2_UC1701_DOGS102_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_2(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1701_DOGS102_2_6800 : public U8G2 {
  public: U8G2_UC1701_DOGS102_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_2_8080 : public U8G2 {
  public: U8G2_UC1701_DOGS102_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_f(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1701_DOGS102_F_6800 : public U8G2 {
  public: U8G2_UC1701_DOGS102_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_F_8080 : public U8G2 {
  public: U8G2_UC1701_DOGS102_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_dogs102_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};

/* Arduino constructor list end */

#endif /* _U8G2LIB_HH */

