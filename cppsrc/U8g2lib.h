/*
  
  U8g2lib.h
  
  C++ Arduino wrapper for the u8g2 struct and c functions for the u8g2 library


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
      
    void clearScreen(void) {
      u8g2_ClearDisplay(&u8g2); }
      
    void setPowerSave(uint8_t is_enable) {
      u8g2_SetPowerSave(&u8g2, is_enable); }
    
    void begin(void) {
      initDisplay(); clearScreen(); setPowerSave(0); }
    
    /* u8g2  */
    void firstPage(void) { u8g2_FirstPage(&u8g2); }
    uint8_t nextPage(void) { return u8g2_NextPage(&u8g2); }

    void setFont(const uint8_t  *font) {u8g2_SetFont(&u8g2, font); }
    
    u8g2_uint_t drawStr(u8g2_uint_t x, u8g2_uint_t y, const char *s) { return u8g2_DrawStr(&u8g2, x, y, s); }

    size_t write(uint8_t v) {
      tx += u8g2_DrawGlyph(&u8g2, tx, ty, v);
      return 1;
     }

     /* LiquidCrystal compatible functions */
    void home(void) { tx = 0; ty = 0; }
    void clear(void) { clearScreen(); home(); }
    void noDisplay(void) { u8g2_SetPowerSave(&u8g2, 1); }
    void display(void) { u8g2_SetPowerSave(&u8g2, 0); }
    void setCursor(uint8_t x, uint8_t y) { tx = x; ty = y; }
 
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
class U8G2_SSD1306_128X64_NONAME_1_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_1_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_1_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_2_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_2_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_ssd1306_128x64_noname_2(&u8g2, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_SW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_SSD1306_128X64_NONAME_F_HW_SPI : public U8G2 {
  public: U8G2_SSD1306_128X64_NONAME_F_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_1_SW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_1_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_uc1701_dogs102_1(&u8g2, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_1_HW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_1_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_uc1701_dogs102_1(&u8g2, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_2_SW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_2_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_uc1701_dogs102_2(&u8g2, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_2_HW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_2_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_uc1701_dogs102_2(&u8g2, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_F_SW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_F_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_uc1701_dogs102_f(&u8g2, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
class U8G2_UC1701_DOGS102_F_HW_SPI : public U8G2 {
  public: U8G2_UC1701_DOGS102_F_HW_SPI(const u8g2_cb_t *rotation, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) {
    u8g2_Setup_uc1701_dogs102_f(&u8g2, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino, rotation);
    u8x8_SetPin_4Wire_HW_SPI(getU8x8(), cs, dc, reset);
  }
};

/* Arduino constructor list end */

#endif /* _U8G2LIB_HH */