/*

  U8x8lib.h
  
  C++ Arduino wrapper for the u8x8 struct and c functions.

*/

#ifndef _U8X8LIB_HH
#define _U8X8LIB_HH

#include <Arduino.h>
#include <Print.h>

#include "u8x8.h"

extern "C" uint8_t u8x8_gpio_and_delay_arduino(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern "C" uint8_t u8x8_byte_arduino_hw_spi(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void u8x8_SetPin_4Wire_SW_SPI(u8x8_t *u8x8, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset);
void u8x8_SetPin_3Wire_SW_SPI(u8x8_t *u8x8, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset);
void u8x8_SetPin_4Wire_HW_SPI(u8x8_t *u8x8, uint8_t cs, uint8_t dc, uint8_t reset);
void u8x8_SetPin_SSD13xx_SW_I2C(u8x8_t *u8x8, uint8_t clock, uint8_t data, uint8_t reset);
void u8x8_SetPin_8Bit_6800(u8x8_t *u8x8, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset);
void u8x8_SetPin_8Bit_8080(u8x8_t *u8x8, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t wr, uint8_t cs, uint8_t dc, uint8_t reset);

void u8x8_Setup_4Wire_SW_SPI(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset);
void u8x8_Setup_3Wire_SW_SPI(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset);
void u8x8_Setup_4Wire_HW_SPI(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t cs, uint8_t dc, uint8_t reset);
void u8x8_Setup_SSD13xx_SW_I2C(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t clock, uint8_t data, uint8_t reset);
void u8x8_Setup_8Bit_6800(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset);
void u8x8_Setup_8Bit_8080(u8x8_t *u8x8, u8x8_msg_cb display_cb, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t wr, uint8_t cs, uint8_t dc, uint8_t reset);

class U8X8 : public Print
{
  protected:
    u8x8_t u8x8;
  public:
    uint8_t tx, ty;
  
    U8X8(void) { home(); }
    u8x8_t *getU8x8(void) { return &u8x8; }

    uint8_t getCols(void) { return u8x8_GetCols(&u8x8); }
    uint8_t getRows(void) { return u8x8_GetRows(&u8x8); }
    
    void drawTile(uint8_t x, uint8_t y, uint8_t cnt, uint8_t *tile_ptr) {
      u8x8_DrawTile(&u8x8, x, y, cnt, tile_ptr); }
      
    void initDisplay(void) {
      u8x8_InitDisplay(&u8x8); }
      
    void clearDisplay(void) {
      u8x8_ClearDisplay(&u8x8); }
      
    void setPowerSave(uint8_t is_enable) {
      u8x8_SetPowerSave(&u8x8, is_enable); }

    void begin(void) {
      initDisplay(); clearDisplay(); setPowerSave(0); }
      
    void setFlipMode(uint8_t mode) {
      u8x8_SetFlipMode(&u8x8, mode); }

    void setContrast(uint8_t value) {
      u8x8_SetContrast(&u8x8, value); }

    void setInverseFont(uint8_t value) {
      u8x8_SetInverseFont(&u8x8, value); }

    void setFont(const uint8_t *font_8x8) {
      u8x8_SetFont(&u8x8, font_8x8); }

    void drawGlyph(uint8_t x, uint8_t y, uint8_t encoding) {
      u8x8_DrawGlyph(&u8x8, x, y, encoding); }

    void drawString(uint8_t x, uint8_t y, const char *s) {
      u8x8_DrawString(&u8x8, x, y, s); }
      
    void drawUTF8(uint8_t x, uint8_t y, const char *s) {
      u8x8_DrawUTF8(&u8x8, x, y, s); }
      
    uint8_t getUTF8Len(const char *s) {
      return u8x8_GetUTF8Len(&u8x8, s); }
    
    size_t write(uint8_t v) {
      u8x8_DrawGlyph(&u8x8, tx, ty, v);
      tx++;
      return 1;
     }
     
     void inverse(void) { setInverseFont(1); }
     void noInverse(void) { setInverseFont(0); }
     
     /* LiquidCrystal compatible functions */
    void home(void) { tx = 0; ty = 0; }
    void clear(void) { clearDisplay(); home(); }
    void noDisplay(void) { u8x8_SetPowerSave(&u8x8, 1); }
    void display(void) { u8x8_SetPowerSave(&u8x8, 0); }
    void setCursor(uint8_t x, uint8_t y) { tx = x; ty = y; }
 
};


class U8X8_SSD1306_128X64_4W_SW_SPI : public U8X8 {
  public: U8X8_SSD1306_128X64_4W_SW_SPI(uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8X8() {
    u8x8_Setup(getU8x8(), u8x8_d_ssd1306_128x64_noname, u8x8_cad_001, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
}

// constructor list start
class U8X8_SSD1306_128X64_4W_SW_SPI : public U8X8 {
  public: U8X8_SSD1306_128X64_4W_SW_SPI(uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8X8() {
    u8x8_Setup_4Wire_SW_SPI(getU8x8(), u8x8_d_ssd1306_128x64_noname, clock, data, cs, dc, reset);
  }
};
class U8X8_SSD1306_128X64_4W_HW_SPI : public U8X8 {
  public: U8X8_SSD1306_128X64_4W_HW_SPI(uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8X8() {
    u8x8_Setup_4Wire_HW_SPI(getU8x8(), u8x8_d_ssd1306_128x64_noname, cs, dc, reset);
  }
};
class U8X8_SSD1306_128X64_3W_SW_SPI : public U8X8 {
  public: U8X8_SSD1306_128X64_3W_SW_SPI(uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE) : U8X8() {
    u8x8_Setup_3Wire_SW_SPI(getU8x8(), u8x8_d_ssd1306_128x64_noname, clock, data, cs, reset);
  }
};
class U8X8_SSD1306_128X64_SW_I2C : public U8X8 {
  public: U8X8_SSD1306_128X64_SW_I2C(uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8X8() {
    u8x8_Setup_SSD13xx_SW_I2C(getU8x8(), u8x8_d_ssd1306_128x64_noname, clock,  data,  reset);
  }
};
class U8X8_SSD1306_128X64_6800 : public U8X8 {
  public: U8X8_SSD1306_128X64_6800(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8X8() {
    u8x8_Setup_8Bit_6800(getU8x8(), u8x8_d_ssd1306_128x64_noname, d0,  d1,  d2,  d3,  d4,  d5,  d6,  d7,  enable,  cs,  dc,  reset);
  }
};
class U8X8_SSD1306_128X64_8080 : public U8X8 {
  public: U8X8_SSD1306_128X64_8080(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8X8() {
    u8x8_Setup_8Bit_8080(getU8x8(), u8x8_d_ssd1306_128x64_noname, d0,  d1,  d2,  d3,  d4,  d5,  d6,  d7,  enable,  cs,  dc,  reset);
  }
};

// constructor list end
  


#endif /* _U8X8LIB_HH */


