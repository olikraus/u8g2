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
    u8x8_char_cb cpp_next_cb; /*  the cpp interface has its own decoding function for the Arduino print command */
  public:
    u8g2_uint_t tx, ty;
  
    U8G2(void) { cpp_next_cb = u8x8_ascii_next; home(); }
    u8x8_t *getU8x8(void) { return u8g2_GetU8x8(&u8g2); }
    u8g2_t *getU8g2(void) { return &u8g2; }
    
    void enableUTF8Print(void) { cpp_next_cb = u8x8_utf8_next; }
    void disableUTF8Print(void) { cpp_next_cb = u8x8_ascii_next; }

    /* u8x8 interface */
    uint8_t getCols(void) { return u8x8_GetCols(u8g2_GetU8x8(&u8g2)); }
    uint8_t getRows(void) { return u8x8_GetRows(u8g2_GetU8x8(&u8g2)); }

#ifdef U8X8_USE_PINS 
    /* set the menu pins before calling begin() or initDisplay() */
    void setMenuSelectPin(uint8_t val) {
      u8g2_SetMenuSelectPin(&u8g2, val); }
    void setMenuPrevPin(uint8_t val) {
      u8g2_SetMenuPrevPin(&u8g2, val); }
    void setMenuNextPin(uint8_t val) {
      u8g2_SetMenuNextPin(&u8g2, val); }
    void setMenuUpPin(uint8_t val) {
      u8g2_SetMenuUpPin(&u8g2, val); }
    void setMenuDownPin(uint8_t val) {
      u8g2_SetMenuDownPin(&u8g2, val); }
    void setMenuHomePin(uint8_t val) {
      u8g2_SetMenuHomePin(&u8g2, val); }
#endif

    /* return 0 for no event or U8X8_MSG_GPIO_MENU_SELECT, */
    /* U8X8_MSG_GPIO_MENU_NEXT, U8X8_MSG_GPIO_MENU_PREV, */
    /* U8X8_MSG_GPIO_MENU_HOME */
    uint8_t getMenuEvent(void) { return u8x8_GetMenuEvent(u8g2_GetU8x8(&u8g2)); }

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
      
    void setDisplayRotation(const u8g2_cb_t *u8g2_cb) {
      u8g2_SetDisplayRotation(&u8g2, u8g2_cb); }

    
    void begin(void) {
      /* note: call to u8x8_utf8_init is not required here, this is done in the setup procedures before */
      initDisplay(); clearDisplay(); setPowerSave(0); }

    void beginSimple(void) {
      /* does not clear the display and does not wake up the display */
      /* user is responsible for calling clearDisplay() and setPowerSave(0) */
      initDisplay();  }
      
#ifdef U8X8_USE_PINS 
    /* use U8X8_PIN_NONE if a pin is not required */
    void begin(uint8_t menu_select_pin, uint8_t menu_next_pin, uint8_t menu_prev_pin, uint8_t menu_up_pin = U8X8_PIN_NONE, uint8_t menu_down_pin = U8X8_PIN_NONE, uint8_t menu_home_pin = U8X8_PIN_NONE) {
      setMenuSelectPin(menu_select_pin);
      setMenuNextPin(menu_next_pin);
      setMenuPrevPin(menu_prev_pin);
      setMenuUpPin(menu_up_pin);
      setMenuDownPin(menu_down_pin);
      setMenuHomePin(menu_home_pin);
      begin(); }
#endif

    /* u8g2  */

      
    u8g2_uint_t getDisplayHeight() { return u8g2_GetDisplayHeight(&u8g2); }
    u8g2_uint_t getDisplayWidth() { return u8g2_GetDisplayWidth(&u8g2); }

    
    /* u8g2_buffer.c */
    void sendBuffer(void) { u8g2_SendBuffer(&u8g2); }
    void clearBuffer(void) { u8g2_ClearBuffer(&u8g2); }    
    
    void firstPage(void) { u8g2_FirstPage(&u8g2); }
    uint8_t nextPage(void) { return u8g2_NextPage(&u8g2); }
    

    /* u8g2_hvline.c */
    void setDrawColor(uint8_t color_index) { u8g2_SetDrawColor(&u8g2, color_index); }
    uint8_t getDrawColor(void) { return u8g2_GetDrawColor(&u8g2); }
    void drawPixel(u8g2_uint_t x, u8g2_uint_t y) { u8g2_DrawPixel(&u8g2, x, y); }
    void drawHLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w) { u8g2_DrawHLine(&u8g2, x, y, w); }
    void drawVLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t h) { u8g2_DrawVLine(&u8g2, x, y, h); }
    void drawHVLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir) {
      u8g2_DrawHVLine(&u8g2, x, y, len, dir); }
    
    /* u8g2_box.c */
    void drawFrame(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) { u8g2_DrawFrame(&u8g2, x, y, w, h); }
    void drawRFrame(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r) { u8g2_DrawRFrame(&u8g2, x, y, w, h,r); }
    void drawBox(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) { u8g2_DrawBox(&u8g2, x, y, w, h); }
    void drawRBox(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r) { u8g2_DrawRBox(&u8g2, x, y, w, h,r); }
    
    /* u8g2_circle.c */
    void drawCircle(u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t opt = U8G2_DRAW_ALL) { u8g2_DrawCircle(&u8g2, x0, y0, rad, opt); }
    void drawDisc(u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t opt = U8G2_DRAW_ALL) { u8g2_DrawDisc(&u8g2, x0, y0, rad, opt); }     
    void drawEllipse(u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rx, u8g2_uint_t ry, uint8_t opt = U8G2_DRAW_ALL) { u8g2_DrawEllipse(&u8g2, x0, y0, rx, ry, opt); }
    void drawFilledEllipse(u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rx, u8g2_uint_t ry, uint8_t opt = U8G2_DRAW_ALL) { u8g2_DrawFilledEllipse(&u8g2, x0, y0, rx, ry, opt); }    

    /* u8g2_line.c */
    void drawLine(u8g2_uint_t x1, u8g2_uint_t y1, u8g2_uint_t x2, u8g2_uint_t y2) 
      { u8g2_DrawLine(&u8g2, x1, y1, x2, y2); }

    /* u8g2_bitmap.c */
    void drawBitmap(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t cnt, u8g2_uint_t h, const uint8_t *bitmap)
      { u8g2_DrawBitmap(&u8g2, x, y, cnt, h, bitmap); }
    void drawXBM(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, const uint8_t *bitmap)
      { u8g2_DrawXBM(&u8g2, x, y, w, h, bitmap); }
    
    
    /* u8g2_polygon.c */
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) 
      { u8g2_DrawTriangle(&u8g2, x0, y0, x1, y1, x2, y2); }
      
    
    /* u8g2_font.c */

    void setFont(const uint8_t  *font) {u8g2_SetFont(&u8g2, font); }
    void setFontMode(uint8_t  is_transparent) {u8g2_SetFontMode(&u8g2, is_transparent); }
    void setFontDirection(uint8_t dir) {u8g2_SetFontDirection(&u8g2, dir); }

    int8_t getAscent(void) { return u8g2_GetAscent(&u8g2); }
    int8_t getDescent(void) { return u8g2_GetDescent(&u8g2); }
    
    void setFontPosBaseline(void) { u8g2_SetFontPosBaseline(&u8g2); }
    void setFontPosBottom(void) { u8g2_SetFontPosBottom(&u8g2); }
    void setFontPosTop(void) { u8g2_SetFontPosTop(&u8g2); }
    void setFontPosCenter(void) { u8g2_SetFontPosCenter(&u8g2); }

    void setFontRefHeightText(void) { u8g2_SetFontRefHeightText(&u8g2); }
    void setFontRefHeightExtendedText(void) { u8g2_SetFontRefHeightExtendedText(&u8g2); }
    void setFontRefHeightAll(void) { u8g2_SetFontRefHeightAll(&u8g2); }
    

/*
uint8_t u8g2_IsGlyph(u8g2_t *u8g2, uint16_t requested_encoding);
int8_t u8g2_GetGlyphWidth(u8g2_t *u8g2, uint16_t requested_encoding);
u8g2_uint_t u8g2_GetStrWidth(u8g2_t *u8g2, const char *s);
u8g2_uint_t u8g2_GetUTF8Width(u8g2_t *u8g2, const char *str);
*/
    
    u8g2_uint_t drawGlyph(u8g2_uint_t x, u8g2_uint_t y, uint16_t encoding) { return u8g2_DrawGlyph(&u8g2, x, y, encoding); }    
    u8g2_uint_t drawStr(u8g2_uint_t x, u8g2_uint_t y, const char *s) { return u8g2_DrawStr(&u8g2, x, y, s); }
    u8g2_uint_t drawUTF8(u8g2_uint_t x, u8g2_uint_t y, const char *s) { return u8g2_DrawUTF8(&u8g2, x, y, s); }
    u8g2_uint_t drawExtUTF8(u8g2_uint_t x, u8g2_uint_t y, uint8_t to_left, const uint16_t *kerning_table, const char *s) 
      { return u8g2_DrawExtUTF8(&u8g2, x, y, to_left, kerning_table, s); }

      
    u8g2_uint_t getStrWidth(const char *s) { return u8g2_GetStrWidth(&u8g2, s); }
    u8g2_uint_t getUTF8Width(const char *s) { return u8g2_GetUTF8Width(&u8g2, s); }
    
    // not required any more, enable UTF8 for print 
    //void printUTF8(const char *s) { tx += u8g2_DrawUTF8(&u8g2, tx, ty, s); }
	
    
    /* virtual function for print base class */    
    size_t write(uint8_t v) {
      uint16_t e = cpp_next_cb(&(u8g2.u8x8), v);
      
      if ( e < 0x0fffe )
	tx += u8g2_DrawGlyph(&u8g2, tx, ty, e);
      return 1;
     }

    size_t write(const uint8_t *buffer, size_t size) {
      size_t cnt = 0;
      while( size > 0 ) {
	cnt += write(*buffer++); 
	size--;
      }
      return cnt;
    }
 

     /* user interface */
/*
uint8_t u8g2_UserInterfaceSelectionList(u8g2_t *u8g2, const char *title, uint8_t start_pos, const char *sl);
uint8_t u8g2_UserInterfaceMessage(u8g2_t *u8g2, const char *title1, const char *title2, const char *title3, const char *buttons);
uint8_t u8g2_UserInterfaceInputValue(u8g2_t *u8g2, const char *title, const char *pre, uint8_t *value, uint8_t lo, uint8_t hi, uint8_t digits, const char *post);
*/

    uint8_t userInterfaceSelectionList(const char *title, uint8_t start_pos, const char *sl) {
      return u8g2_UserInterfaceSelectionList(&u8g2, title, start_pos, sl); }
    uint8_t userInterfaceMessage(const char *title1, const char *title2, const char *title3, const char *buttons) {
      return u8g2_UserInterfaceMessage(&u8g2, title1, title2, title3, buttons); }
    uint8_t userInterfaceInputValue(const char *title, const char *pre, uint8_t *value, uint8_t lo, uint8_t hi, uint8_t digits, const char *post) {
      return u8g2_UserInterfaceInputValue(&u8g2, title, pre, value, lo, hi, digits, post); }
    

     /* LiquidCrystal compatible functions */
    void home(void) { tx = 0; ty = 0;  u8x8_utf8_init(u8g2_GetU8x8(&u8g2)); }
    void clear(void) { home(); clearDisplay(); clearBuffer();  }
    void noDisplay(void) { u8g2_SetPowerSave(&u8g2, 1); }
    void display(void) { u8g2_SetPowerSave(&u8g2, 0); }
    void setCursor(u8g2_uint_t x, u8g2_uint_t y) { tx = x; ty = y; }
 
    /* u8glib compatible functions */
    void sleepOn(void) { u8g2_SetPowerSave(&u8g2, 1); }
    void sleepOff(void) { u8g2_SetPowerSave(&u8g2, 0); }    
    void setColorIndex(uint8_t color_index) { u8g2_SetDrawColor(&u8g2, color_index); }
    uint8_t getColorIndex(void) { return u8g2_GetDrawColor(&u8g2); }
    int8_t getFontAscent(void) { return u8g2_GetAscent(&u8g2); }
    int8_t getFontDescent(void) { return u8g2_GetDescent(&u8g2); }
    u8g2_uint_t getHeight() { return u8g2_GetDisplayHeight(&u8g2); }
    u8g2_uint_t getWidth() { return u8g2_GetDisplayWidth(&u8g2); }
    
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
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
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
class U8G2_SSD1306_128X64_NONAME_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
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
class U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
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
class U8G2_SSD1306_128X64_NONAME_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_1_6800 : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_1_8080 : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_2_6800 : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_2_8080 : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_F_6800 : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_F_8080 : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_NONAME_1_SW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1106_128X64_NONAME_1_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1106_128X64_NONAME_2_SW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1106_128X64_NONAME_2_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1106_128X64_NONAME_F_SW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1106_128X64_NONAME_F_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_1_6800 : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_1_8080 : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_2_6800 : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_2_8080 : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_F_6800 : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_F_8080 : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_LD7032_60X32_1_4W_SW_SPI : public U8G2 {
  public: U8G2_LD7032_60X32_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_60x32_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_LD7032_60X32_1_4W_HW_SPI : public U8G2 {
  public: U8G2_LD7032_60X32_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_60x32_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_LD7032_60X32_2_4W_SW_SPI : public U8G2 {
  public: U8G2_LD7032_60X32_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_60x32_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_LD7032_60X32_2_4W_HW_SPI : public U8G2 {
  public: U8G2_LD7032_60X32_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_60x32_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_LD7032_60X32_F_4W_SW_SPI : public U8G2 {
  public: U8G2_LD7032_60X32_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_60x32_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_LD7032_60X32_F_4W_HW_SPI : public U8G2 {
  public: U8G2_LD7032_60X32_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_60x32_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_LD7032_60X32_1_SW_I2C : public U8G2 {
  public: U8G2_LD7032_60X32_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_LD7032_60X32_1_HW_I2C : public U8G2 {
  public: U8G2_LD7032_60X32_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_LD7032_60X32_2_SW_I2C : public U8G2 {
  public: U8G2_LD7032_60X32_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_LD7032_60X32_2_HW_I2C : public U8G2 {
  public: U8G2_LD7032_60X32_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_LD7032_60X32_F_SW_I2C : public U8G2 {
  public: U8G2_LD7032_60X32_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_LD7032_60X32_F_HW_I2C : public U8G2 {
  public: U8G2_LD7032_60X32_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST7920_192X32_1_8080 : public U8G2 {
  public: U8G2_ST7920_192X32_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_192x32_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_2_8080 : public U8G2 {
  public: U8G2_ST7920_192X32_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_192x32_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_F_8080 : public U8G2 {
  public: U8G2_ST7920_192X32_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_192x32_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_1_SW_SPI : public U8G2 {
  public: U8G2_ST7920_192X32_1_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_192x32_1(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7920_192X32_1_HW_SPI : public U8G2 {
  public: U8G2_ST7920_192X32_1_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_192x32_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_ST7920_HW_SPI(getU8x8(), cs, reset);
  }
};
class U8G2_ST7920_192X32_2_SW_SPI : public U8G2 {
  public: U8G2_ST7920_192X32_2_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_192x32_2(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7920_192X32_2_HW_SPI : public U8G2 {
  public: U8G2_ST7920_192X32_2_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_192x32_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_ST7920_HW_SPI(getU8x8(), cs, reset);
  }
};
class U8G2_ST7920_192X32_F_SW_SPI : public U8G2 {
  public: U8G2_ST7920_192X32_F_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_192x32_f(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7920_192X32_F_HW_SPI : public U8G2 {
  public: U8G2_ST7920_192X32_F_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_192x32_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_ST7920_HW_SPI(getU8x8(), cs, reset);
  }
};
class U8G2_ST7920_128X64_1_8080 : public U8G2 {
  public: U8G2_ST7920_128X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_128x64_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_128X64_2_8080 : public U8G2 {
  public: U8G2_ST7920_128X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_128x64_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_128X64_F_8080 : public U8G2 {
  public: U8G2_ST7920_128X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_128x64_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_128X64_1_SW_SPI : public U8G2 {
  public: U8G2_ST7920_128X64_1_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_128x64_1(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7920_128X64_1_HW_SPI : public U8G2 {
  public: U8G2_ST7920_128X64_1_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_128x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_ST7920_HW_SPI(getU8x8(), cs, reset);
  }
};
class U8G2_ST7920_128X64_2_SW_SPI : public U8G2 {
  public: U8G2_ST7920_128X64_2_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_128x64_2(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7920_128X64_2_HW_SPI : public U8G2 {
  public: U8G2_ST7920_128X64_2_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_128x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_ST7920_HW_SPI(getU8x8(), cs, reset);
  }
};
class U8G2_ST7920_128X64_F_SW_SPI : public U8G2 {
  public: U8G2_ST7920_128X64_F_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_128x64_f(&u8g2, rotation, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7920_128X64_F_HW_SPI : public U8G2 {
  public: U8G2_ST7920_128X64_F_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_128x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_ST7920_HW_SPI(getU8x8(), cs, reset);
  }
};
class U8G2_LS013B7DH03_128X128_1_4W_SW_SPI : public U8G2 {
  public: U8G2_LS013B7DH03_128X128_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ls013b7dh03_128x128_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_LS013B7DH03_128X128_1_4W_HW_SPI : public U8G2 {
  public: U8G2_LS013B7DH03_128X128_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ls013b7dh03_128x128_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_LS013B7DH03_128X128_2_4W_SW_SPI : public U8G2 {
  public: U8G2_LS013B7DH03_128X128_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ls013b7dh03_128x128_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_LS013B7DH03_128X128_2_4W_HW_SPI : public U8G2 {
  public: U8G2_LS013B7DH03_128X128_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ls013b7dh03_128x128_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_LS013B7DH03_128X128_F_4W_SW_SPI : public U8G2 {
  public: U8G2_LS013B7DH03_128X128_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ls013b7dh03_128x128_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_LS013B7DH03_128X128_F_4W_HW_SPI : public U8G2 {
  public: U8G2_LS013B7DH03_128X128_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ls013b7dh03_128x128_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_1_6800 : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_1_8080 : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_2_6800 : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_2_8080 : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_F_6800 : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_EA_DOGS102_F_8080 : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1701_MINI12864_1_6800 : public U8G2 {
  public: U8G2_UC1701_MINI12864_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_1_8080 : public U8G2 {
  public: U8G2_UC1701_MINI12864_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1701_MINI12864_2_6800 : public U8G2 {
  public: U8G2_UC1701_MINI12864_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_2_8080 : public U8G2 {
  public: U8G2_UC1701_MINI12864_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1701_MINI12864_F_6800 : public U8G2 {
  public: U8G2_UC1701_MINI12864_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1701_MINI12864_F_8080 : public U8G2 {
  public: U8G2_UC1701_MINI12864_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_1_6800 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_1_8080 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_2_6800 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_2_8080 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_F_6800 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM128_F_8080 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_NHD_C12832_1_6800 : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_1_8080 : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_NHD_C12832_2_6800 : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_2_8080 : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_NHD_C12832_F_6800 : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12832_F_8080 : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_240X128_1_8080 : public U8G2 {
  public: U8G2_T6963_240X128_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_240x128_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_240X128_2_8080 : public U8G2 {
  public: U8G2_T6963_240X128_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_240x128_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_240X128_F_8080 : public U8G2 {
  public: U8G2_T6963_240X128_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_240x128_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_256X64_1_8080 : public U8G2 {
  public: U8G2_T6963_256X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_256x64_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_256X64_2_8080 : public U8G2 {
  public: U8G2_T6963_256X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_256x64_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_256X64_F_8080 : public U8G2 {
  public: U8G2_T6963_256X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_256x64_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_128X64_1_8080 : public U8G2 {
  public: U8G2_T6963_128X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_128x64_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_128X64_2_8080 : public U8G2 {
  public: U8G2_T6963_128X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_128x64_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_128X64_F_8080 : public U8G2 {
  public: U8G2_T6963_128X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_128x64_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_256X64_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_256X64_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_256X64_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1322_256X64_1_6800 : public U8G2 {
  public: U8G2_SSD1322_256X64_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_1_8080 : public U8G2 {
  public: U8G2_SSD1322_256X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_256X64_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_256X64_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_256X64_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1322_256X64_2_6800 : public U8G2 {
  public: U8G2_SSD1322_256X64_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_2_8080 : public U8G2 {
  public: U8G2_SSD1322_256X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_256X64_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_256X64_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_256X64_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1322_256X64_F_6800 : public U8G2 {
  public: U8G2_SSD1322_256X64_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_256X64_F_8080 : public U8G2 {
  public: U8G2_SSD1322_256X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_256x64_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SED1330_240X128_1_6800 : public U8G2 {
  public: U8G2_SED1330_240X128_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sed1330_240x128_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SED1330_240X128_1_8080 : public U8G2 {
  public: U8G2_SED1330_240X128_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sed1330_240x128_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SED1330_240X128_2_6800 : public U8G2 {
  public: U8G2_SED1330_240X128_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sed1330_240x128_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SED1330_240X128_2_8080 : public U8G2 {
  public: U8G2_SED1330_240X128_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sed1330_240x128_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SED1330_240X128_F_6800 : public U8G2 {
  public: U8G2_SED1330_240X128_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sed1330_240x128_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SED1330_240X128_F_8080 : public U8G2 {
  public: U8G2_SED1330_240X128_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sed1330_240x128_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_NHD_240X128_1_6800 : public U8G2 {
  public: U8G2_RA8835_NHD_240X128_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_nhd_240x128_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_NHD_240X128_1_8080 : public U8G2 {
  public: U8G2_RA8835_NHD_240X128_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_nhd_240x128_1(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_NHD_240X128_2_6800 : public U8G2 {
  public: U8G2_RA8835_NHD_240X128_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_nhd_240x128_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_NHD_240X128_2_8080 : public U8G2 {
  public: U8G2_RA8835_NHD_240X128_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_nhd_240x128_2(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_NHD_240X128_F_6800 : public U8G2 {
  public: U8G2_RA8835_NHD_240X128_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_nhd_240x128_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_NHD_240X128_F_8080 : public U8G2 {
  public: U8G2_RA8835_NHD_240X128_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_nhd_240x128_f(&u8g2, rotation, u8x8_byte_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};

/* Arduino constructor list end */

#endif /* _U8G2LIB_HH */

