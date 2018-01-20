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

    void setI2CAddress(uint8_t adr) { u8g2_SetI2CAddress(&u8g2, adr); }
    
    
    void enableUTF8Print(void) { cpp_next_cb = u8x8_utf8_next; }
    void disableUTF8Print(void) { cpp_next_cb = u8x8_ascii_next; }

    /* u8x8 interface */
    uint8_t getCols(void) { return u8x8_GetCols(u8g2_GetU8x8(&u8g2)); }
    uint8_t getRows(void) { return u8x8_GetRows(u8g2_GetU8x8(&u8g2)); }
    void drawTile(uint8_t x, uint8_t y, uint8_t cnt, uint8_t *tile_ptr) {
      u8x8_DrawTile(u8g2_GetU8x8(&u8g2), x, y, cnt, tile_ptr); }

#ifdef U8X8_WITH_USER_PTR
      void *getUserPtr() { return u8g2_GetUserPtr(&u8g2); }
      void setUserPtr(void *p) { u8g2_SetUserPtr(&u8g2, p); }
#endif
      

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
    
    uint8_t *getBufferPtr(void) { return u8g2_GetBufferPtr(&u8g2); }
    uint8_t getBufferTileHeight(void) { return u8g2_GetBufferTileHeight(&u8g2); }
    uint8_t getBufferTileWidth(void) { return u8g2_GetBufferTileWidth(&u8g2); }
    uint8_t getPageCurrTileRow(void) { return u8g2_GetBufferCurrTileRow(&u8g2); }	// obsolete
    void setPageCurrTileRow(uint8_t row) { u8g2_SetBufferCurrTileRow(&u8g2, row); }	// obsolete
    uint8_t getBufferCurrTileRow(void) { return u8g2_GetBufferCurrTileRow(&u8g2); }
    void setBufferCurrTileRow(uint8_t row) { u8g2_SetBufferCurrTileRow(&u8g2, row); }
    
    // this should be renamed to setBufferAutoClear
    void setAutoPageClear(uint8_t mode)  { u8g2_SetAutoPageClear(&u8g2, mode); }

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
    void setBitmapMode(uint8_t is_transparent) 
      { u8g2_SetBitmapMode(&u8g2, is_transparent); }
    void drawBitmap(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t cnt, u8g2_uint_t h, const uint8_t *bitmap)
      { u8g2_DrawBitmap(&u8g2, x, y, cnt, h, bitmap); }
    void drawXBM(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, const uint8_t *bitmap)
      { u8g2_DrawXBM(&u8g2, x, y, w, h, bitmap); }
    void drawXBMP(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, const uint8_t *bitmap)
      { u8g2_DrawXBMP(&u8g2, x, y, w, h, bitmap); }
    
    
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
    int8_t getMaxCharHeight(void) { return u8g2_GetMaxCharHeight(&u8g2); }
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


#ifdef U8X8_USE_PINS

/* Arduino constructor list start */
/* generated code (codebuild), u8g2 project */
class U8G2_SSD1305_128X32_NONAME_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_1_6800 : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_1_8080 : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_2_6800 : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_2_8080 : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_F_6800 : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_F_8080 : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_128x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_i2c_128x32_noname_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_i2c_128x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1305_128X32_NONAME_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_i2c_128x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_i2c_128x32_noname_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_i2c_128x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1305_128X32_NONAME_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_i2c_128x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_i2c_128x32_noname_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1305_128X32_NONAME_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_i2c_128x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1305_128X32_NONAME_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1305_128X32_NONAME_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1305_i2c_128x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
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
class U8G2_SSD1306_128X64_NONAME_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_1_6800 : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_1_8080 : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_1_6800 : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_1_8080 : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_SSD1306_128X64_NONAME_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_2_6800 : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_2_8080 : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_2_6800 : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_2_8080 : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_SSD1306_128X64_NONAME_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_F_6800 : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_F_8080 : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_F_6800 : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_F_8080 : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x64_alt0_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_SSD1306_128X64_NONAME_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X64_NONAME_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_alt0_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_alt0_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X64_ALT0_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_alt0_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_SSD1306_128X64_NONAME_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_alt0_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_alt0_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X64_ALT0_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_alt0_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_SSD1306_128X64_NONAME_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X64_VCOMH0_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_VCOMH0_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_alt0_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_128X64_ALT0_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_alt0_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X64_ALT0_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X64_ALT0_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x64_alt0_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
class U8G2_SH1106_128X64_NONAME_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_sh1106_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_1_6800 : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_1_8080 : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_1_6800 : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_1_8080 : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_SH1106_128X64_NONAME_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_sh1106_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_2_6800 : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_2_8080 : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_2_6800 : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_2_8080 : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_SH1106_128X64_NONAME_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_F_6800 : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_F_8080 : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_F_6800 : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_F_8080 : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_winstar_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_SH1106_128X64_NONAME_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1106_128X64_NONAME_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_1_SW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_1_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1106_128X64_VCOMH0_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_vcomh0_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_1_SW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_winstar_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_1_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_winstar_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1106_128X64_WINSTAR_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_winstar_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_SH1106_128X64_NONAME_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1106_128X64_NONAME_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_2_SW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_2_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1106_128X64_VCOMH0_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_vcomh0_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_2_SW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_winstar_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_2_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_winstar_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1106_128X64_WINSTAR_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_winstar_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_SH1106_128X64_NONAME_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1106_128X64_NONAME_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_F_SW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1106_128X64_VCOMH0_F_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1106_128X64_VCOMH0_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_VCOMH0_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_vcomh0_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_F_SW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_winstar_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1106_128X64_WINSTAR_F_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_winstar_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1106_128X64_WINSTAR_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1106_128X64_WINSTAR_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_i2c_128x64_winstar_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1107_128X64_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1107_128X64_1_6800 : public U8G2 {
  public: U8G2_SH1107_128X64_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_1_8080 : public U8G2 {
  public: U8G2_SH1107_128X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1107_128X64_2_6800 : public U8G2 {
  public: U8G2_SH1107_128X64_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_2_8080 : public U8G2 {
  public: U8G2_SH1107_128X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_128X64_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1107_128X64_F_6800 : public U8G2 {
  public: U8G2_SH1107_128X64_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_F_8080 : public U8G2 {
  public: U8G2_SH1107_128X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_128x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_128X64_1_SW_I2C : public U8G2 {
  public: U8G2_SH1107_128X64_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_128x64_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1107_128X64_1_HW_I2C : public U8G2 {
  public: U8G2_SH1107_128X64_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_128x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1107_128X64_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1107_128X64_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_128x64_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1107_128X64_2_SW_I2C : public U8G2 {
  public: U8G2_SH1107_128X64_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_128x64_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1107_128X64_2_HW_I2C : public U8G2 {
  public: U8G2_SH1107_128X64_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_128x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1107_128X64_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1107_128X64_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_128x64_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1107_128X64_F_SW_I2C : public U8G2 {
  public: U8G2_SH1107_128X64_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_128x64_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1107_128X64_F_HW_I2C : public U8G2 {
  public: U8G2_SH1107_128X64_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_128x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1107_128X64_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1107_128X64_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_128x64_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1107_SEEED_96X96_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_1_6800 : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_1_8080 : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_2_6800 : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_2_8080 : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_F_6800 : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_F_8080 : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SH1107_SEEED_96X96_1_SW_I2C : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_seeed_96x96_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1107_SEEED_96X96_1_HW_I2C : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1107_SEEED_96X96_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1107_SEEED_96X96_2_SW_I2C : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_seeed_96x96_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1107_SEEED_96X96_2_HW_I2C : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1107_SEEED_96X96_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SH1107_SEEED_96X96_F_SW_I2C : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_seeed_96x96_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SH1107_SEEED_96X96_F_HW_I2C : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SH1107_SEEED_96X96_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SH1107_SEEED_96X96_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1107_i2c_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
class U8G2_SSD1306_128X32_UNIVISION_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_ssd1306_128x32_univision_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_SSD1306_128X32_UNIVISION_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_ssd1306_128x32_univision_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_SSD1306_128X32_UNIVISION_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_128x32_univision_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_ssd1306_128x32_univision_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_SSD1306_128X32_UNIVISION_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_128X32_UNIVISION_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_128X32_UNIVISION_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_128x32_univision_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_64X48_ER_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_64X48_ER_1_6800 : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_1_8080 : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_64X48_ER_2_6800 : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_2_8080 : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_64X48_ER_F_6800 : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_F_8080 : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x48_er_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X48_ER_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x48_er_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_64X48_ER_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x48_er_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_64X48_ER_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x48_er_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_64X48_ER_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x48_er_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_64X48_ER_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x48_er_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_64X48_ER_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x48_er_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_64X48_ER_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x48_er_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_64X48_ER_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x48_er_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_64X48_ER_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X48_ER_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x48_er_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_1_6800 : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_1_8080 : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_64X32_1F_1_6800 : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_1_8080 : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_2_6800 : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_2_8080 : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_64X32_1F_2_6800 : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_2_8080 : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_F_6800 : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_F_8080 : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_64X32_1F_F_6800 : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_1F_F_8080 : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_64x32_1f_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_noname_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_64X32_NONAME_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_noname_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_64X32_1F_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_1f_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_64X32_1F_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_1f_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_64X32_1F_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_1f_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_noname_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_64X32_NONAME_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_noname_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_64X32_1F_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_1f_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_64X32_1F_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_1f_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_64X32_1F_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_1f_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_noname_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_64X32_NONAME_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_64X32_NONAME_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_NONAME_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_noname_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_64X32_1F_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_1f_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_64X32_1F_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_1f_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_64X32_1F_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_64X32_1F_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_64x32_1f_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_96X16_ER_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_96X16_ER_1_6800 : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_1_8080 : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_96X16_ER_2_6800 : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_2_8080 : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1306_96X16_ER_F_6800 : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_F_8080 : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_96x16_er_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1306_96X16_ER_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_96x16_er_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_96X16_ER_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_96x16_er_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_96X16_ER_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_96x16_er_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_96X16_ER_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_96x16_er_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_96X16_ER_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_96x16_er_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_96X16_ER_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_96x16_er_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1306_96X16_ER_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_96x16_er_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1306_96X16_ER_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_96x16_er_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1306_96X16_ER_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1306_96X16_ER_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1306_i2c_96x16_er_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_1_6800 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_1_8080 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_2_6800 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_2_8080 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_F_6800 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_F_8080 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname2_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname2_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname2_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1309_128X64_NONAME2_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname2_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname2_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname2_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1309_128X64_NONAME2_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname2_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname2_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1309_128X64_NONAME2_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname2_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1309_128X64_NONAME2_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME2_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname2_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_1_6800 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_1_8080 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_2_6800 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_2_8080 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_F_6800 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_F_8080 : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_128x64_noname0_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname0_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname0_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1309_128X64_NONAME0_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname0_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname0_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname0_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1309_128X64_NONAME0_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname0_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname0_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1309_128X64_NONAME0_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname0_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1309_128X64_NONAME0_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1309_128X64_NONAME0_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1309_i2c_128x64_noname0_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1325_NHD_128X64_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_1_6800 : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_1_8080 : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_2_6800 : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_2_8080 : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_F_6800 : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_F_8080 : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1325_NHD_128X64_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_i2c_nhd_128x64_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1325_NHD_128X64_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_i2c_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1325_NHD_128X64_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_i2c_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1325_NHD_128X64_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_i2c_nhd_128x64_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1325_NHD_128X64_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_i2c_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1325_NHD_128X64_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_i2c_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1325_NHD_128X64_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_i2c_nhd_128x64_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1325_NHD_128X64_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_i2c_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1325_NHD_128X64_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1325_NHD_128X64_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1325_i2c_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1326_ER_256X32_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1326_ER_256X32_1_6800 : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_1_8080 : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1326_ER_256X32_2_6800 : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_2_8080 : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1326_ER_256X32_F_6800 : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_F_8080 : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_er_256x32_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1326_ER_256X32_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_i2c_er_256x32_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1326_ER_256X32_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_i2c_er_256x32_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1326_ER_256X32_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_i2c_er_256x32_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1326_ER_256X32_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_i2c_er_256x32_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1326_ER_256X32_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_i2c_er_256x32_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1326_ER_256X32_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_i2c_er_256x32_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1326_ER_256X32_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_i2c_er_256x32_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1326_ER_256X32_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_i2c_er_256x32_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1326_ER_256X32_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1326_ER_256X32_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1326_i2c_er_256x32_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_1_6800 : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_1_8080 : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_2_6800 : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_2_8080 : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_F_6800 : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_F_8080 : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_seeed_96x96_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1327_SEEED_96X96_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_seeed_96x96_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_seeed_96x96_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1327_SEEED_96X96_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_seeed_96x96_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_seeed_96x96_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1327_SEEED_96X96_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1327_SEEED_96X96_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_SEEED_96X96_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_seeed_96x96_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_1_6800 : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_1_8080 : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_2_6800 : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_2_8080 : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_F_6800 : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_F_8080 : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_midas_128x128_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_1_SW_I2C : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_midas_128x128_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_1_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_midas_128x128_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1327_MIDAS_128X128_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_midas_128x128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_2_SW_I2C : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_midas_128x128_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_2_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_midas_128x128_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1327_MIDAS_128X128_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_midas_128x128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_F_SW_I2C : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_midas_128x128_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_SSD1327_MIDAS_128X128_F_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_midas_128x128_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_SSD1327_MIDAS_128X128_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_SSD1327_MIDAS_128X128_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1327_i2c_midas_128x128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_1_6800 : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_1_8080 : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_2_6800 : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_2_8080 : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_F_6800 : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1329_128X96_NONAME_F_8080 : public U8G2 {
  public: U8G2_SSD1329_128X96_NONAME_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1329_128x96_noname_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
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
class U8G2_LD7032_60X32_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_LD7032_60X32_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_60x32_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
class U8G2_LD7032_60X32_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_LD7032_60X32_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_60x32_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
class U8G2_LD7032_60X32_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_LD7032_60X32_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_60x32_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_LD7032_60X32_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_LD7032_60X32_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_LD7032_60X32_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_LD7032_60X32_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_LD7032_60X32_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_LD7032_60X32_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
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
  public: U8G2_LD7032_60X32_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_LD7032_60X32_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_LD7032_60X32_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ld7032_i2c_60x32_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST7920_192X32_1_8080 : public U8G2 {
  public: U8G2_ST7920_192X32_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_192x32_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_2_8080 : public U8G2 {
  public: U8G2_ST7920_192X32_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_192x32_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_F_8080 : public U8G2 {
  public: U8G2_ST7920_192X32_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_192x32_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_1_6800 : public U8G2 {
  public: U8G2_ST7920_192X32_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_192x32_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_2_6800 : public U8G2 {
  public: U8G2_ST7920_192X32_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_192x32_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_F_6800 : public U8G2 {
  public: U8G2_ST7920_192X32_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_192x32_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_192X32_1_SW_SPI : public U8G2 {
  public: U8G2_ST7920_192X32_1_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_192x32_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7920_s_192x32_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7920_s_192x32_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7920_p_128x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_128X64_2_8080 : public U8G2 {
  public: U8G2_ST7920_128X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_128x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_128X64_F_8080 : public U8G2 {
  public: U8G2_ST7920_128X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_p_128x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_128X64_1_6800 : public U8G2 {
  public: U8G2_ST7920_128X64_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_128x64_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_128X64_2_6800 : public U8G2 {
  public: U8G2_ST7920_128X64_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_128x64_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_128X64_F_6800 : public U8G2 {
  public: U8G2_ST7920_128X64_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_128x64_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7920_128X64_1_SW_SPI : public U8G2 {
  public: U8G2_ST7920_128X64_1_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7920_s_128x64_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7920_s_128x64_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7920_s_128x64_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
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
class U8G2_LS013B7DH03_128X128_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_LS013B7DH03_128X128_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ls013b7dh03_128x128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
class U8G2_LS013B7DH03_128X128_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_LS013B7DH03_128X128_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ls013b7dh03_128x128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
class U8G2_LS013B7DH03_128X128_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_LS013B7DH03_128X128_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ls013b7dh03_128x128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
class U8G2_UC1701_EA_DOGS102_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_uc1701_ea_dogs102_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_UC1701_EA_DOGS102_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_uc1701_ea_dogs102_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_UC1701_EA_DOGS102_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_EA_DOGS102_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_ea_dogs102_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_uc1701_ea_dogs102_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_UC1701_MINI12864_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_uc1701_mini12864_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_UC1701_MINI12864_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_uc1701_mini12864_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_UC1701_MINI12864_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1701_MINI12864_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1701_mini12864_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_uc1701_mini12864_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_1_4W_SW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_1_4W_HW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_1_3W_SW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_PCD8544_84X48_2_4W_SW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_2_4W_HW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_2_3W_SW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_PCD8544_84X48_F_4W_SW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_F_4W_HW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCD8544_84X48_F_3W_SW_SPI : public U8G2 {
  public: U8G2_PCD8544_84X48_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcd8544_84x48_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_PCF8812_96X65_1_4W_SW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_PCF8812_96X65_1_4W_HW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCF8812_96X65_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCF8812_96X65_1_3W_SW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_PCF8812_96X65_2_4W_SW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_PCF8812_96X65_2_4W_HW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCF8812_96X65_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCF8812_96X65_2_3W_SW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_PCF8812_96X65_F_4W_SW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_PCF8812_96X65_F_4W_HW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCF8812_96X65_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_PCF8812_96X65_F_3W_SW_SPI : public U8G2 {
  public: U8G2_PCF8812_96X65_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_pcf8812_96x65_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1604_JLX19264_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1604_JLX19264_1_6800 : public U8G2 {
  public: U8G2_UC1604_JLX19264_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_1_8080 : public U8G2 {
  public: U8G2_UC1604_JLX19264_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1604_JLX19264_2_6800 : public U8G2 {
  public: U8G2_UC1604_JLX19264_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_2_8080 : public U8G2 {
  public: U8G2_UC1604_JLX19264_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1604_JLX19264_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1604_JLX19264_F_6800 : public U8G2 {
  public: U8G2_UC1604_JLX19264_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_F_8080 : public U8G2 {
  public: U8G2_UC1604_JLX19264_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_jlx19264_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1604_JLX19264_1_SW_I2C : public U8G2 {
  public: U8G2_UC1604_JLX19264_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_i2c_jlx19264_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1604_JLX19264_1_HW_I2C : public U8G2 {
  public: U8G2_UC1604_JLX19264_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_i2c_jlx19264_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1604_JLX19264_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1604_JLX19264_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_i2c_jlx19264_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1604_JLX19264_2_SW_I2C : public U8G2 {
  public: U8G2_UC1604_JLX19264_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_i2c_jlx19264_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1604_JLX19264_2_HW_I2C : public U8G2 {
  public: U8G2_UC1604_JLX19264_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_i2c_jlx19264_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1604_JLX19264_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1604_JLX19264_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_i2c_jlx19264_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1604_JLX19264_F_SW_I2C : public U8G2 {
  public: U8G2_UC1604_JLX19264_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_i2c_jlx19264_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1604_JLX19264_F_HW_I2C : public U8G2 {
  public: U8G2_UC1604_JLX19264_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_i2c_jlx19264_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1604_JLX19264_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1604_JLX19264_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1604_i2c_jlx19264_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1608_ERC24064_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1608_ERC24064_1_6800 : public U8G2 {
  public: U8G2_UC1608_ERC24064_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_1_8080 : public U8G2 {
  public: U8G2_UC1608_ERC24064_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1608_ERC24064_2_6800 : public U8G2 {
  public: U8G2_UC1608_ERC24064_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_2_8080 : public U8G2 {
  public: U8G2_UC1608_ERC24064_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC24064_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1608_ERC24064_F_6800 : public U8G2 {
  public: U8G2_UC1608_ERC24064_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_F_8080 : public U8G2 {
  public: U8G2_UC1608_ERC24064_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc24064_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC24064_1_SW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC24064_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc24064_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1608_ERC24064_1_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC24064_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc24064_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1608_ERC24064_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC24064_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc24064_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1608_ERC24064_2_SW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC24064_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc24064_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1608_ERC24064_2_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC24064_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc24064_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1608_ERC24064_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC24064_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc24064_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1608_ERC24064_F_SW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC24064_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc24064_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1608_ERC24064_F_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC24064_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc24064_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1608_ERC24064_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC24064_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc24064_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1608_ERC240120_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1608_ERC240120_1_6800 : public U8G2 {
  public: U8G2_UC1608_ERC240120_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_1_8080 : public U8G2 {
  public: U8G2_UC1608_ERC240120_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1608_ERC240120_2_6800 : public U8G2 {
  public: U8G2_UC1608_ERC240120_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_2_8080 : public U8G2 {
  public: U8G2_UC1608_ERC240120_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_ERC240120_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1608_ERC240120_F_6800 : public U8G2 {
  public: U8G2_UC1608_ERC240120_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_F_8080 : public U8G2 {
  public: U8G2_UC1608_ERC240120_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_erc240120_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_ERC240120_1_SW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC240120_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc240120_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1608_ERC240120_1_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC240120_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc240120_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1608_ERC240120_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC240120_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc240120_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1608_ERC240120_2_SW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC240120_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc240120_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1608_ERC240120_2_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC240120_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc240120_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1608_ERC240120_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC240120_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc240120_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1608_ERC240120_F_SW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC240120_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc240120_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1608_ERC240120_F_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC240120_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc240120_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1608_ERC240120_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1608_ERC240120_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_erc240120_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1608_240X128_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1608_240X128_1_6800 : public U8G2 {
  public: U8G2_UC1608_240X128_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_1_8080 : public U8G2 {
  public: U8G2_UC1608_240X128_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1608_240X128_2_6800 : public U8G2 {
  public: U8G2_UC1608_240X128_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_2_8080 : public U8G2 {
  public: U8G2_UC1608_240X128_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1608_240X128_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1608_240X128_F_6800 : public U8G2 {
  public: U8G2_UC1608_240X128_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_F_8080 : public U8G2 {
  public: U8G2_UC1608_240X128_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_240x128_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1608_240X128_1_SW_I2C : public U8G2 {
  public: U8G2_UC1608_240X128_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_240x128_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1608_240X128_1_HW_I2C : public U8G2 {
  public: U8G2_UC1608_240X128_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_240x128_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1608_240X128_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1608_240X128_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_240x128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1608_240X128_2_SW_I2C : public U8G2 {
  public: U8G2_UC1608_240X128_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_240x128_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1608_240X128_2_HW_I2C : public U8G2 {
  public: U8G2_UC1608_240X128_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_240x128_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1608_240X128_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1608_240X128_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_240x128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1608_240X128_F_SW_I2C : public U8G2 {
  public: U8G2_UC1608_240X128_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_240x128_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1608_240X128_F_HW_I2C : public U8G2 {
  public: U8G2_UC1608_240X128_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_240x128_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1608_240X128_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1608_240X128_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1608_i2c_240x128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1638_160X128_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1638_160X128_1_6800 : public U8G2 {
  public: U8G2_UC1638_160X128_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_1_8080 : public U8G2 {
  public: U8G2_UC1638_160X128_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1638_160X128_2_6800 : public U8G2 {
  public: U8G2_UC1638_160X128_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_2_8080 : public U8G2 {
  public: U8G2_UC1638_160X128_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1638_160X128_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1638_160X128_F_6800 : public U8G2 {
  public: U8G2_UC1638_160X128_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1638_160X128_F_8080 : public U8G2 {
  public: U8G2_UC1638_160X128_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1638_160x128_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_1_6800 : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_1_8080 : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_2_6800 : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_2_8080 : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_F_6800 : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_F_8080 : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_ea_dogxl160_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_1_SW_I2C : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_i2c_ea_dogxl160_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_1_HW_I2C : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_i2c_ea_dogxl160_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1610_EA_DOGXL160_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_i2c_ea_dogxl160_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_2_SW_I2C : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_i2c_ea_dogxl160_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_2_HW_I2C : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_i2c_ea_dogxl160_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1610_EA_DOGXL160_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_i2c_ea_dogxl160_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_F_SW_I2C : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_i2c_ea_dogxl160_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1610_EA_DOGXL160_F_HW_I2C : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_i2c_ea_dogxl160_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1610_EA_DOGXL160_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1610_EA_DOGXL160_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1610_i2c_ea_dogxl160_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1611_EA_DOGM240_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_1_6800 : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_1_8080 : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_2_6800 : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_2_8080 : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_F_6800 : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_F_8080 : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogm240_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGM240_1_SW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogm240_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1611_EA_DOGM240_1_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogm240_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1611_EA_DOGM240_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogm240_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1611_EA_DOGM240_2_SW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogm240_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1611_EA_DOGM240_2_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogm240_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1611_EA_DOGM240_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogm240_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1611_EA_DOGM240_F_SW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogm240_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1611_EA_DOGM240_F_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogm240_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1611_EA_DOGM240_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGM240_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogm240_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_1_6800 : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_1_8080 : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_2_6800 : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_2_8080 : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_F_6800 : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_F_8080 : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ea_dogxl240_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_1_SW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogxl240_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_1_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogxl240_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1611_EA_DOGXL240_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogxl240_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_2_SW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogxl240_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_2_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogxl240_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1611_EA_DOGXL240_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogxl240_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_F_SW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogxl240_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1611_EA_DOGXL240_F_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogxl240_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1611_EA_DOGXL240_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EA_DOGXL240_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ea_dogxl240_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1611_EW50850_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1611_EW50850_1_6800 : public U8G2 {
  public: U8G2_UC1611_EW50850_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_1_8080 : public U8G2 {
  public: U8G2_UC1611_EW50850_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1611_EW50850_2_6800 : public U8G2 {
  public: U8G2_UC1611_EW50850_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_2_8080 : public U8G2 {
  public: U8G2_UC1611_EW50850_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1611_EW50850_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1611_EW50850_F_6800 : public U8G2 {
  public: U8G2_UC1611_EW50850_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_F_8080 : public U8G2 {
  public: U8G2_UC1611_EW50850_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_ew50850_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1611_EW50850_1_SW_I2C : public U8G2 {
  public: U8G2_UC1611_EW50850_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ew50850_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1611_EW50850_1_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EW50850_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ew50850_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1611_EW50850_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EW50850_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ew50850_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1611_EW50850_2_SW_I2C : public U8G2 {
  public: U8G2_UC1611_EW50850_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ew50850_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1611_EW50850_2_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EW50850_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ew50850_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1611_EW50850_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EW50850_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ew50850_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1611_EW50850_F_SW_I2C : public U8G2 {
  public: U8G2_UC1611_EW50850_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ew50850_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1611_EW50850_F_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EW50850_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ew50850_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1611_EW50850_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1611_EW50850_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1611_i2c_ew50850_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
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
class U8G2_ST7565_EA_DOGM128_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7565_ea_dogm128_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_64128N_1_6800 : public U8G2 {
  public: U8G2_ST7565_64128N_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_1_8080 : public U8G2 {
  public: U8G2_ST7565_64128N_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_1_6800 : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_1_8080 : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_LM6059_1_6800 : public U8G2 {
  public: U8G2_ST7565_LM6059_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_1_8080 : public U8G2 {
  public: U8G2_ST7565_LM6059_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_ERC12864_1_6800 : public U8G2 {
  public: U8G2_ST7565_ERC12864_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_1_8080 : public U8G2 {
  public: U8G2_ST7565_ERC12864_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_NHD_C12864_1_6800 : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_1_8080 : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_ST7565_EA_DOGM128_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7565_ea_dogm128_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_64128N_2_6800 : public U8G2 {
  public: U8G2_ST7565_64128N_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_2_8080 : public U8G2 {
  public: U8G2_ST7565_64128N_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_2_6800 : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_2_8080 : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_LM6059_2_6800 : public U8G2 {
  public: U8G2_ST7565_LM6059_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_2_8080 : public U8G2 {
  public: U8G2_ST7565_LM6059_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_ERC12864_2_6800 : public U8G2 {
  public: U8G2_ST7565_ERC12864_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_2_8080 : public U8G2 {
  public: U8G2_ST7565_ERC12864_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_NHD_C12864_2_6800 : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_2_8080 : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_ST7565_EA_DOGM128_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM128_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7565_ea_dogm128_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_64128N_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_64128N_F_6800 : public U8G2 {
  public: U8G2_ST7565_64128N_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_64128N_F_8080 : public U8G2 {
  public: U8G2_ST7565_64128N_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_64128n_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_F_6800 : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ZOLEN_128X64_F_8080 : public U8G2 {
  public: U8G2_ST7565_ZOLEN_128X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_zolen_128x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_LM6059_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_LM6059_F_6800 : public U8G2 {
  public: U8G2_ST7565_LM6059_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_LM6059_F_8080 : public U8G2 {
  public: U8G2_ST7565_LM6059_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_lm6059_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_ERC12864_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_ERC12864_F_6800 : public U8G2 {
  public: U8G2_ST7565_ERC12864_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_ERC12864_F_8080 : public U8G2 {
  public: U8G2_ST7565_ERC12864_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_erc12864_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_NHD_C12864_F_6800 : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_NHD_C12864_F_8080 : public U8G2 {
  public: U8G2_ST7565_NHD_C12864_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12864_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_ST7565_NHD_C12832_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7565_nhd_c12832_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_ST7565_NHD_C12832_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7565_nhd_c12832_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
class U8G2_ST7565_NHD_C12832_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_NHD_C12832_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_nhd_c12832_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_st7565_nhd_c12832_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_1_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_1_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_1_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1601_128X32_1_6800 : public U8G2 {
  public: U8G2_UC1601_128X32_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_1_8080 : public U8G2 {
  public: U8G2_UC1601_128X32_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_2_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_2_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_2_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1601_128X32_2_6800 : public U8G2 {
  public: U8G2_UC1601_128X32_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_2_8080 : public U8G2 {
  public: U8G2_UC1601_128X32_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_F_4W_SW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_F_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_F_3W_SW_SPI : public U8G2 {
  public: U8G2_UC1601_128X32_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_UC1601_128X32_F_6800 : public U8G2 {
  public: U8G2_UC1601_128X32_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_F_8080 : public U8G2 {
  public: U8G2_UC1601_128X32_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_128x32_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_UC1601_128X32_1_SW_I2C : public U8G2 {
  public: U8G2_UC1601_128X32_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_i2c_128x32_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1601_128X32_1_HW_I2C : public U8G2 {
  public: U8G2_UC1601_128X32_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_i2c_128x32_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1601_128X32_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1601_128X32_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_i2c_128x32_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1601_128X32_2_SW_I2C : public U8G2 {
  public: U8G2_UC1601_128X32_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_i2c_128x32_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1601_128X32_2_HW_I2C : public U8G2 {
  public: U8G2_UC1601_128X32_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_i2c_128x32_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1601_128X32_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1601_128X32_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_i2c_128x32_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_UC1601_128X32_F_SW_I2C : public U8G2 {
  public: U8G2_UC1601_128X32_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_i2c_128x32_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_UC1601_128X32_F_HW_I2C : public U8G2 {
  public: U8G2_UC1601_128X32_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_i2c_128x32_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_UC1601_128X32_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_UC1601_128X32_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_uc1601_i2c_128x32_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST7565_EA_DOGM132_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_1_6800 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_1_8080 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_2_6800 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_2_8080 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_F_6800 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7565_EA_DOGM132_F_8080 : public U8G2 {
  public: U8G2_ST7565_EA_DOGM132_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7565_ea_dogm132_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7567_PI_132X64_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_PI_132X64_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_PI_132X64_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_1_6800 : public U8G2 {
  public: U8G2_ST7567_PI_132X64_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_1_8080 : public U8G2 {
  public: U8G2_ST7567_PI_132X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7567_PI_132X64_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_PI_132X64_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_PI_132X64_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_2_6800 : public U8G2 {
  public: U8G2_ST7567_PI_132X64_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_2_8080 : public U8G2 {
  public: U8G2_ST7567_PI_132X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7567_PI_132X64_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_PI_132X64_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_PI_132X64_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_F_6800 : public U8G2 {
  public: U8G2_ST7567_PI_132X64_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_PI_132X64_F_8080 : public U8G2 {
  public: U8G2_ST7567_PI_132X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_pi_132x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7567_JLX12864_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_JLX12864_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_JLX12864_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_1_6800 : public U8G2 {
  public: U8G2_ST7567_JLX12864_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_1_8080 : public U8G2 {
  public: U8G2_ST7567_JLX12864_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7567_JLX12864_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_JLX12864_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_JLX12864_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_2_6800 : public U8G2 {
  public: U8G2_ST7567_JLX12864_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_2_8080 : public U8G2 {
  public: U8G2_ST7567_JLX12864_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7567_JLX12864_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_JLX12864_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7567_JLX12864_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_F_6800 : public U8G2 {
  public: U8G2_ST7567_JLX12864_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7567_JLX12864_F_8080 : public U8G2 {
  public: U8G2_ST7567_JLX12864_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7567_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7588_JLX12864_1_6800 : public U8G2 {
  public: U8G2_ST7588_JLX12864_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_1_8080 : public U8G2 {
  public: U8G2_ST7588_JLX12864_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7588_JLX12864_2_6800 : public U8G2 {
  public: U8G2_ST7588_JLX12864_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_2_8080 : public U8G2 {
  public: U8G2_ST7588_JLX12864_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST7588_JLX12864_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST7588_JLX12864_F_6800 : public U8G2 {
  public: U8G2_ST7588_JLX12864_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_F_8080 : public U8G2 {
  public: U8G2_ST7588_JLX12864_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST7588_JLX12864_1_SW_I2C : public U8G2 {
  public: U8G2_ST7588_JLX12864_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_i2c_jlx12864_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST7588_JLX12864_1_HW_I2C : public U8G2 {
  public: U8G2_ST7588_JLX12864_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_i2c_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST7588_JLX12864_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST7588_JLX12864_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_i2c_jlx12864_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST7588_JLX12864_2_SW_I2C : public U8G2 {
  public: U8G2_ST7588_JLX12864_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_i2c_jlx12864_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST7588_JLX12864_2_HW_I2C : public U8G2 {
  public: U8G2_ST7588_JLX12864_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_i2c_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST7588_JLX12864_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST7588_JLX12864_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_i2c_jlx12864_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST7588_JLX12864_F_SW_I2C : public U8G2 {
  public: U8G2_ST7588_JLX12864_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_i2c_jlx12864_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST7588_JLX12864_F_HW_I2C : public U8G2 {
  public: U8G2_ST7588_JLX12864_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_i2c_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST7588_JLX12864_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST7588_JLX12864_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st7588_i2c_jlx12864_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST75256_JLX256128_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST75256_JLX256128_1_6800 : public U8G2 {
  public: U8G2_ST75256_JLX256128_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_1_8080 : public U8G2 {
  public: U8G2_ST75256_JLX256128_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST75256_JLX256128_2_6800 : public U8G2 {
  public: U8G2_ST75256_JLX256128_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_2_8080 : public U8G2 {
  public: U8G2_ST75256_JLX256128_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX256128_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST75256_JLX256128_F_6800 : public U8G2 {
  public: U8G2_ST75256_JLX256128_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_F_8080 : public U8G2 {
  public: U8G2_ST75256_JLX256128_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx256128_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX256128_1_SW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX256128_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx256128_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST75256_JLX256128_1_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX256128_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx256128_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST75256_JLX256128_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX256128_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx256128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST75256_JLX256128_2_SW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX256128_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx256128_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST75256_JLX256128_2_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX256128_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx256128_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST75256_JLX256128_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX256128_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx256128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST75256_JLX256128_F_SW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX256128_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx256128_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST75256_JLX256128_F_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX256128_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx256128_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST75256_JLX256128_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX256128_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx256128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST75256_JLX25664_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST75256_JLX25664_1_6800 : public U8G2 {
  public: U8G2_ST75256_JLX25664_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_1_8080 : public U8G2 {
  public: U8G2_ST75256_JLX25664_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST75256_JLX25664_2_6800 : public U8G2 {
  public: U8G2_ST75256_JLX25664_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_2_8080 : public U8G2 {
  public: U8G2_ST75256_JLX25664_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX25664_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST75256_JLX25664_F_6800 : public U8G2 {
  public: U8G2_ST75256_JLX25664_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_F_8080 : public U8G2 {
  public: U8G2_ST75256_JLX25664_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx25664_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX25664_1_SW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX25664_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx25664_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST75256_JLX25664_1_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX25664_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx25664_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST75256_JLX25664_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX25664_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx25664_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST75256_JLX25664_2_SW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX25664_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx25664_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST75256_JLX25664_2_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX25664_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx25664_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST75256_JLX25664_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX25664_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx25664_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST75256_JLX25664_F_SW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX25664_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx25664_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST75256_JLX25664_F_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX25664_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx25664_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST75256_JLX25664_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX25664_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx25664_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST75256_JLX172104_1_4W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_1_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_1_3W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST75256_JLX172104_1_6800 : public U8G2 {
  public: U8G2_ST75256_JLX172104_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_1_8080 : public U8G2 {
  public: U8G2_ST75256_JLX172104_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_2_4W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_2_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_2_3W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST75256_JLX172104_2_6800 : public U8G2 {
  public: U8G2_ST75256_JLX172104_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_2_8080 : public U8G2 {
  public: U8G2_ST75256_JLX172104_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_F_4W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_F_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_F_3W_SW_SPI : public U8G2 {
  public: U8G2_ST75256_JLX172104_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_ST75256_JLX172104_F_6800 : public U8G2 {
  public: U8G2_ST75256_JLX172104_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_F_8080 : public U8G2 {
  public: U8G2_ST75256_JLX172104_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_jlx172104_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_ST75256_JLX172104_1_SW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX172104_1_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx172104_1(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST75256_JLX172104_1_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX172104_1_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx172104_1(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST75256_JLX172104_1_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX172104_1_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx172104_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST75256_JLX172104_2_SW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX172104_2_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx172104_2(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST75256_JLX172104_2_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX172104_2_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx172104_2(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST75256_JLX172104_2_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX172104_2_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx172104_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_ST75256_JLX172104_F_SW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX172104_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx172104_f(&u8g2, rotation, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);
  }
};
class U8G2_ST75256_JLX172104_F_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX172104_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx172104_f(&u8g2, rotation, u8x8_byte_arduino_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset, clock, data);
  }
};
class U8G2_ST75256_JLX172104_F_2ND_HW_I2C : public U8G2 {
  public: U8G2_ST75256_JLX172104_F_2ND_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_st75256_i2c_jlx172104_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_i2c, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_HW_I2C(getU8x8(), reset);
  }
};
class U8G2_NT7534_TG12864R_1_4W_SW_SPI : public U8G2 {
  public: U8G2_NT7534_TG12864R_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_1_4W_HW_SPI : public U8G2 {
  public: U8G2_NT7534_TG12864R_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_NT7534_TG12864R_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_1_6800 : public U8G2 {
  public: U8G2_NT7534_TG12864R_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_1_8080 : public U8G2 {
  public: U8G2_NT7534_TG12864R_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_2_4W_SW_SPI : public U8G2 {
  public: U8G2_NT7534_TG12864R_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_2_4W_HW_SPI : public U8G2 {
  public: U8G2_NT7534_TG12864R_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_NT7534_TG12864R_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_2_6800 : public U8G2 {
  public: U8G2_NT7534_TG12864R_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_2_8080 : public U8G2 {
  public: U8G2_NT7534_TG12864R_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_F_4W_SW_SPI : public U8G2 {
  public: U8G2_NT7534_TG12864R_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_F_4W_HW_SPI : public U8G2 {
  public: U8G2_NT7534_TG12864R_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_NT7534_TG12864R_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_F_6800 : public U8G2 {
  public: U8G2_NT7534_TG12864R_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_NT7534_TG12864R_F_8080 : public U8G2 {
  public: U8G2_NT7534_TG12864R_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_nt7534_tg12864r_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_1_4W_SW_SPI : public U8G2 {
  public: U8G2_IST3020_ERC19264_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_1_4W_HW_SPI : public U8G2 {
  public: U8G2_IST3020_ERC19264_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_IST3020_ERC19264_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_1_6800 : public U8G2 {
  public: U8G2_IST3020_ERC19264_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_1_8080 : public U8G2 {
  public: U8G2_IST3020_ERC19264_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_2_4W_SW_SPI : public U8G2 {
  public: U8G2_IST3020_ERC19264_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_2_4W_HW_SPI : public U8G2 {
  public: U8G2_IST3020_ERC19264_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_IST3020_ERC19264_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_2_6800 : public U8G2 {
  public: U8G2_IST3020_ERC19264_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_2_8080 : public U8G2 {
  public: U8G2_IST3020_ERC19264_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_F_4W_SW_SPI : public U8G2 {
  public: U8G2_IST3020_ERC19264_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_F_4W_HW_SPI : public U8G2 {
  public: U8G2_IST3020_ERC19264_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_IST3020_ERC19264_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_F_6800 : public U8G2 {
  public: U8G2_IST3020_ERC19264_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_IST3020_ERC19264_F_8080 : public U8G2 {
  public: U8G2_IST3020_ERC19264_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ist3020_erc19264_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SBN1661_122X32_1 : public U8G2 {
  public: U8G2_SBN1661_122X32_1(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t dc, uint8_t e1, uint8_t e2, uint8_t reset) : U8G2() {
    u8g2_Setup_sbn1661_122x32_1(&u8g2, rotation, u8x8_byte_sed1520, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SED1520(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, dc, e1, e2, reset);
  }
};
class U8G2_SBN1661_122X32_2 : public U8G2 {
  public: U8G2_SBN1661_122X32_2(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t dc, uint8_t e1, uint8_t e2, uint8_t reset) : U8G2() {
    u8g2_Setup_sbn1661_122x32_2(&u8g2, rotation, u8x8_byte_sed1520, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SED1520(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, dc, e1, e2, reset);
  }
};
class U8G2_SBN1661_122X32_F : public U8G2 {
  public: U8G2_SBN1661_122X32_F(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t dc, uint8_t e1, uint8_t e2, uint8_t reset) : U8G2() {
    u8g2_Setup_sbn1661_122x32_f(&u8g2, rotation, u8x8_byte_sed1520, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SED1520(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, dc, e1, e2, reset);
  }
};
class U8G2_SED1520_122X32_1 : public U8G2 {
  public: U8G2_SED1520_122X32_1(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t dc, uint8_t e1, uint8_t e2, uint8_t reset) : U8G2() {
    u8g2_Setup_sed1520_122x32_1(&u8g2, rotation, u8x8_byte_sed1520, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SED1520(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, dc, e1, e2, reset);
  }
};
class U8G2_SED1520_122X32_2 : public U8G2 {
  public: U8G2_SED1520_122X32_2(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t dc, uint8_t e1, uint8_t e2, uint8_t reset) : U8G2() {
    u8g2_Setup_sed1520_122x32_2(&u8g2, rotation, u8x8_byte_sed1520, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SED1520(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, dc, e1, e2, reset);
  }
};
class U8G2_SED1520_122X32_F : public U8G2 {
  public: U8G2_SED1520_122X32_F(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t dc, uint8_t e1, uint8_t e2, uint8_t reset) : U8G2() {
    u8g2_Setup_sed1520_122x32_f(&u8g2, rotation, u8x8_byte_sed1520, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_SED1520(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, dc, e1, e2, reset);
  }
};
class U8G2_KS0108_128X64_1 : public U8G2 {
  public: U8G2_KS0108_128X64_1(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t dc, uint8_t cs0, uint8_t cs1, uint8_t cs2, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ks0108_128x64_1(&u8g2, rotation, u8x8_byte_arduino_ks0108, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_KS0108(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, dc, cs0, cs1, cs2, reset);
  }
};
class U8G2_KS0108_128X64_2 : public U8G2 {
  public: U8G2_KS0108_128X64_2(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t dc, uint8_t cs0, uint8_t cs1, uint8_t cs2, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ks0108_128x64_2(&u8g2, rotation, u8x8_byte_arduino_ks0108, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_KS0108(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, dc, cs0, cs1, cs2, reset);
  }
};
class U8G2_KS0108_128X64_F : public U8G2 {
  public: U8G2_KS0108_128X64_F(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t dc, uint8_t cs0, uint8_t cs1, uint8_t cs2, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ks0108_128x64_f(&u8g2, rotation, u8x8_byte_arduino_ks0108, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_KS0108(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, dc, cs0, cs1, cs2, reset);
  }
};
class U8G2_KS0108_ERM19264_1 : public U8G2 {
  public: U8G2_KS0108_ERM19264_1(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t dc, uint8_t cs0, uint8_t cs1, uint8_t cs2, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ks0108_erm19264_1(&u8g2, rotation, u8x8_byte_arduino_ks0108, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_KS0108(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, dc, cs0, cs1, cs2, reset);
  }
};
class U8G2_KS0108_ERM19264_2 : public U8G2 {
  public: U8G2_KS0108_ERM19264_2(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t dc, uint8_t cs0, uint8_t cs1, uint8_t cs2, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ks0108_erm19264_2(&u8g2, rotation, u8x8_byte_arduino_ks0108, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_KS0108(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, dc, cs0, cs1, cs2, reset);
  }
};
class U8G2_KS0108_ERM19264_F : public U8G2 {
  public: U8G2_KS0108_ERM19264_F(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t dc, uint8_t cs0, uint8_t cs1, uint8_t cs2, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ks0108_erm19264_f(&u8g2, rotation, u8x8_byte_arduino_ks0108, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_KS0108(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, dc, cs0, cs1, cs2, reset);
  }
};
class U8G2_LC7981_160X80_1_6800 : public U8G2 {
  public: U8G2_LC7981_160X80_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_lc7981_160x80_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_LC7981_160X80_2_6800 : public U8G2 {
  public: U8G2_LC7981_160X80_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_lc7981_160x80_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_LC7981_160X80_F_6800 : public U8G2 {
  public: U8G2_LC7981_160X80_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_lc7981_160x80_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_LC7981_160X160_1_6800 : public U8G2 {
  public: U8G2_LC7981_160X160_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_lc7981_160x160_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_LC7981_160X160_2_6800 : public U8G2 {
  public: U8G2_LC7981_160X160_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_lc7981_160x160_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_LC7981_160X160_F_6800 : public U8G2 {
  public: U8G2_LC7981_160X160_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_lc7981_160x160_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_LC7981_240X128_1_6800 : public U8G2 {
  public: U8G2_LC7981_240X128_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_lc7981_240x128_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_LC7981_240X128_2_6800 : public U8G2 {
  public: U8G2_LC7981_240X128_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_lc7981_240x128_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_LC7981_240X128_F_6800 : public U8G2 {
  public: U8G2_LC7981_240X128_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_lc7981_240x128_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_240X128_1_8080 : public U8G2 {
  public: U8G2_T6963_240X128_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_240x128_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_240X128_2_8080 : public U8G2 {
  public: U8G2_T6963_240X128_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_240x128_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_240X128_F_8080 : public U8G2 {
  public: U8G2_T6963_240X128_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_240x128_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_240X64_1_8080 : public U8G2 {
  public: U8G2_T6963_240X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_240x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_240X64_2_8080 : public U8G2 {
  public: U8G2_T6963_240X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_240x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_240X64_F_8080 : public U8G2 {
  public: U8G2_T6963_240X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_240x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_256X64_1_8080 : public U8G2 {
  public: U8G2_T6963_256X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_256x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_256X64_2_8080 : public U8G2 {
  public: U8G2_T6963_256X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_256x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_256X64_F_8080 : public U8G2 {
  public: U8G2_T6963_256X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_256x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_128X64_1_8080 : public U8G2 {
  public: U8G2_T6963_128X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_128x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_128X64_2_8080 : public U8G2 {
  public: U8G2_T6963_128X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_128x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_T6963_128X64_F_8080 : public U8G2 {
  public: U8G2_T6963_128X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_t6963_128x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_1_6800 : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_1_8080 : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_2_6800 : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_2_8080 : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_F_6800 : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_256X64_F_8080 : public U8G2 {
  public: U8G2_SSD1322_NHD_256X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_256x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_1_6800 : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_1_8080 : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_2_6800 : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_2_8080 : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_F_6800 : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1322_NHD_128X64_F_8080 : public U8G2 {
  public: U8G2_SSD1322_NHD_128X64_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1322_nhd_128x64_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1606_172X72_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1606_172X72_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1606_172X72_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1606_172X72_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1606_172x72_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1607_200X200_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1607_200X200_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_200X200_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_200X200_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1607_GD_200X200_1_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1607_GD_200X200_1_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_GD_200X200_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_GD_200X200_1_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1607_200X200_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1607_200X200_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_200X200_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_200X200_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1607_GD_200X200_2_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1607_GD_200X200_2_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_GD_200X200_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_GD_200X200_2_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1607_200X200_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1607_200X200_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_200X200_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_200X200_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_200X200_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_200x200_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_SSD1607_GD_200X200_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1607_GD_200X200_F_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_GD_200X200_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1607_GD_200X200_F_3W_SW_SPI : public U8G2 {
  public: U8G2_SSD1607_GD_200X200_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ssd1607_gd_200x200_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_IL3820_296X128_1_4W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_IL3820_296X128_1_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_296X128_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_296X128_1_3W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_IL3820_V2_296X128_1_4W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_IL3820_V2_296X128_1_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_V2_296X128_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_V2_296X128_1_3W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_1_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_1(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_IL3820_296X128_2_4W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_IL3820_296X128_2_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_296X128_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_296X128_2_3W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_IL3820_V2_296X128_2_4W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_IL3820_V2_296X128_2_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_V2_296X128_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_V2_296X128_2_3W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_2_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_2(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_IL3820_296X128_F_4W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_IL3820_296X128_F_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_296X128_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_296X128_F_3W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_296X128_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_296x128_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
  }
};
class U8G2_IL3820_V2_296X128_F_4W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_IL3820_V2_296X128_F_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_V2_296X128_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_IL3820_V2_296X128_F_3W_SW_SPI : public U8G2 {
  public: U8G2_IL3820_V2_296X128_F_3W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_il3820_v2_296x128_f(&u8g2, rotation, u8x8_byte_3wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
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
    u8g2_Setup_sed1330_240x128_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_sed1330_240x128_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_sed1330_240x128_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_ra8835_nhd_240x128_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_ra8835_nhd_240x128_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
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
    u8g2_Setup_ra8835_nhd_240x128_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_320X240_1_6800 : public U8G2 {
  public: U8G2_RA8835_320X240_1_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_320x240_1(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_320X240_1_8080 : public U8G2 {
  public: U8G2_RA8835_320X240_1_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_320x240_1(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_320X240_2_6800 : public U8G2 {
  public: U8G2_RA8835_320X240_2_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_320x240_2(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_320X240_2_8080 : public U8G2 {
  public: U8G2_RA8835_320X240_2_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_320x240_2(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_320X240_F_6800 : public U8G2 {
  public: U8G2_RA8835_320X240_F_6800(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_320x240_f(&u8g2, rotation, u8x8_byte_8bit_6800mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_6800(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_RA8835_320X240_F_8080 : public U8G2 {
  public: U8G2_RA8835_320X240_F_8080(const u8g2_cb_t *rotation, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_ra8835_320x240_f(&u8g2, rotation, u8x8_byte_arduino_8bit_8080mode, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_8Bit_8080(getU8x8(), d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset);
  }
};
class U8G2_MAX7219_32X8_1_4W_SW_SPI : public U8G2 {
  public: U8G2_MAX7219_32X8_1_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_max7219_32x8_1(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_MAX7219_32X8_1_4W_HW_SPI : public U8G2 {
  public: U8G2_MAX7219_32X8_1_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_max7219_32x8_1(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_MAX7219_32X8_1_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_MAX7219_32X8_1_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_max7219_32x8_1(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_MAX7219_32X8_2_4W_SW_SPI : public U8G2 {
  public: U8G2_MAX7219_32X8_2_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_max7219_32x8_2(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_MAX7219_32X8_2_4W_HW_SPI : public U8G2 {
  public: U8G2_MAX7219_32X8_2_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_max7219_32x8_2(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_MAX7219_32X8_2_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_MAX7219_32X8_2_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_max7219_32x8_2(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_MAX7219_32X8_F_4W_SW_SPI : public U8G2 {
  public: U8G2_MAX7219_32X8_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_max7219_32x8_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_MAX7219_32X8_F_4W_HW_SPI : public U8G2 {
  public: U8G2_MAX7219_32X8_F_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_max7219_32x8_f(&u8g2, rotation, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_MAX7219_32X8_F_2ND_4W_HW_SPI : public U8G2 {
  public: U8G2_MAX7219_32X8_F_2ND_4W_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_max7219_32x8_f(&u8g2, rotation, u8x8_byte_arduino_2nd_hw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};

/* Arduino constructor list end */

#endif // U8X8_USE_PINS

class U8G2_BITMAP : public U8G2 {
  public: U8G2_BITMAP(uint16_t pixel_width, uint16_t pixel_height, const u8g2_cb_t *rotation) {
    u8g2_SetupBitmap(getU8g2(), rotation, pixel_width, pixel_height);
  }

  // This completely resets various settings, such as the
  // font, so be sure to re-initialize things
  void changeSize(uint16_t pixel_width, uint16_t pixel_height) {
    u8g2_SetupBitmap(getU8g2(), getU8g2()->cb, pixel_width, pixel_height);
  }
};

#endif /* _U8G2LIB_HH */

