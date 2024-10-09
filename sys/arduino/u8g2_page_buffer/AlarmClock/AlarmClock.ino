/*

  AlarmClock.ino

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2017, olikraus@gmail.com
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

*/

#include <Arduino.h>
#include <RTCZero.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Picture Loop Page Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
//U8G2_NULL u8g2(U8G2_R0);	// null device, a 8x8 pixel display which does nothing
//U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6);	// Arduboy (Production, Kickstarter Edition)
//U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1306_128X64_NONAME_1_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* reset=*/ 8);
//U8G2_SSD1306_128X64_NONAME_1_3W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* reset=*/ 8);
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_ALT0_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // same as the NONAME variant, but may solve the "every 2nd line skipped" problem
//U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* reset=*/ 8);
//U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
//U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 16, /* data=*/ 17, /* reset=*/ U8X8_PIN_NONE);   // ESP32 Thing, pure SW emulated I2C
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17);   // ESP32 Thing, HW I2C with pin remapping
//U8G2_SSD1306_128X64_NONAME_1_6800 u8g2(U8G2_R0, 13, 11, 2, 3, 4, 5, 6, A4, /*enable=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8);
//U8G2_SSD1306_128X64_NONAME_1_8080 u8g2(U8G2_R0, 13, 11, 2, 3, 4, 5, 6, A4, /*enable=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8);
//U8G2_SSD1306_128X64_VCOMH0_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// same as the NONAME variant, but maximizes setContrast() range
//U8G2_SSD1306_128X64_ALT0_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// same as the NONAME variant, but may solve the "every 2nd line skipped" problem
//U8G2_SSD1306_102X64_EA_OLEDS102_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// same as the NONAME variant, but may solve the "every 2nd line skipped" problem
//U8G2_SSD1312_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1312_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ 8);
//U8G2_SSD1312_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
//U8G2_SSD1312_120X28_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1312_120X28_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ 8);
//U8G2_SSD1312_120X28_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
//U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SH1106_128X64_VCOMH0_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// same as the NONAME variant, but maximizes setContrast() range
//U8G2_SH1106_128X64_WINSTAR_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// same as the NONAME variant, but uses updated SH1106 init sequence
//U8G2_SH1106_128X32_VISIONOX_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
//U8G2_SH1106_128X32_VISIONOX_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1106_72X40_WISE_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1107_64X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1107_128X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1107_128X128_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
//U8G2_SH1107_128X80_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
//U8G2_SH1107_PIMORONI_128X128_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
//U8G2_SH1107_SEEED_128X128_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SH1107_SEEED_128X128_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SH1107_SEEED_96X96_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1107_HJR_OEL1M0201_96X96_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1107_TK078F288_80X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1108_128X160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_CH1120_128X160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1108_160X160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SH1122_256X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);				// Enable U8G2_16BIT in u8g2.h
//U8G2_SH1122_256X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);        // Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 21, /* data=*/ 20, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather M0 Basic Proto + FeatherWing OLED
//U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
//U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C
//U8G2_SSD1306_128X32_WINSTAR_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C
//U8G2_SSD1306_64X48_ER_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered
//U8G2_SSD1306_48X64_WINSTAR_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   
//U8G2_SSD1306_64X32_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_64X32_1F_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_96X16_ER_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.69" OLED 
//U8G2_SSD1306_72X40_ER_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.42" OLED 
//U8G2_SSD1306_96X40_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_96X39_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1322_240X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1322_TOPWIN_240X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1322_NHD_256X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1322_ZJY_256X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1322_ZJY_256X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1362_256X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1362_256X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1362_206X36_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1362_206X36_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1363_256X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1363_256X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1322_NHD_128X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1322_NHD_128X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1325_NHD_128X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD0323_OS128064_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD0323_OS128064_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1326_ER_256X32_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);         // experimental driver for ER-OLED018-1
//U8G2_SSD1327_SEEED_96X96_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);	// Seeedstudio Grove OLED 96x96
//U8G2_SSD1327_SEEED_96X96_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);	// Seeedstudio Grove OLED 96x96
//U8G2_SSD1327_EA_W128128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1327_EA_W128128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1327_EA_W128128_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1327_EA_W128128_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); /* Uno: A4=SDA, A5=SCL, add "u8g2.setBusClock(400000);" into setup() for speedup if possible */
//U8G2_SSD1327_MIDAS_128X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1327_MIDAS_128X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1327_MIDAS_128X128_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); /* Uno: A4=SDA, A5=SCL, add "u8g2.setBusClock(400000);" into setup() for speedup if possible */
//U8G2_SSD1327_ZJY_128X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1327_ZJY_128X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1327_ZJY_128X128_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); /* Uno: A4=SDA, A5=SCL, add "u8g2.setBusClock(400000);" into setup() for speedup if possible */
//U8G2_SSD1327_WS_128X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1327_WS_128X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1327_VISIONOX_128X96_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1327_VISIONOX_128X96_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1329_128X96_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1329_128X96_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1329_96X96_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1329_96X96_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1329_96X96_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ A4, /* dc=*/ A2, /* reset=*/ U8X8_PIN_NONE); //  Adafruit Feather 32u4 Basic Proto
//U8G2_SSD1305_128X32_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1305_128X32_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1305_128X32_ADAFRUIT_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1305_128X32_ADAFRUIT_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1305_128X64_ADAFRUIT_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1305_128X64_ADAFRUIT_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1305_128X64_RAYSTAR_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1305_128X64_RAYSTAR_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1309_128X64_NONAME0_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_SSD1309_128X64_NONAME0_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_SSD1309_128X64_NONAME2_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_SSD1309_128X64_NONAME2_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_SSD1316_128X32_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_SSD1316_128X32_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 	
//U8G2_SSD1316_96X32_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_SSD1316_96X32_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 	
//U8G2_SSD1317_96X96_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // not tested, not confirmed
//U8G2_SSD1317_96X96_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 	// not tested, not confirmed
//U8G2_SSD1318_128X96_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_SSD1318_128X96_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 	
//U8G2_SSD1320_160X132_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_SSD1320_160X132_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 	
//U8G2_SSD1320_160X80_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_SSD1320_160X80_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 	
//U8G2_LD7032_60X32_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 9, /* dc=*/ 10, /* reset=*/ 8);	// SW SPI Nano Board
//U8G2_LD7032_60X32_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* reset=*/ U8X8_PIN_NONE);	// NOT TESTED!
//U8G2_LD7032_60X32_ALT_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 9, /* dc=*/ 10, /* reset=*/ 8);	// SW SPI Nano Board
//U8G2_LD7032_60X32_ALT_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* reset=*/ U8X8_PIN_NONE);	// NOT TESTED!
//U8G2_LD7032_128X36_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 9, /* dc=*/ 10, /* reset=*/ 8);	// SW SPI Nano Board
//U8G2_LD7032_128X36_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* reset=*/ U8X8_PIN_NONE);	// NOT TESTED!
//U8G2_UC1701_EA_DOGS102_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1701_EA_DOGS102_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Nokia 5110 Display
//U8G2_PCD8544_84X48_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);						// Nokia 5110 Display
//U8G2_PCF8812_96X65_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Could be also PCF8814
//U8G2_PCF8812_96X65_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);						// Could be also PCF8814
//U8G2_PCF8812_101X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Could be also PCF8814
//U8G2_PCF8812_101X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);						// Could be also PCF8814
//U8G2_HX1230_96X68_1_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* reset=*/ 8);
//U8G2_HX1230_96X68_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_KS0108_128X64_1 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*dc=*/ 17, /*cs0=*/ 14, /*cs1=*/ 15, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE); 	// Set R/W to low!
//U8G2_KS0108_ERM19264_1 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*dc=*/ 17, /*cs0=*/ 14, /*cs1=*/ 15, /*cs2=*/ 16, /* reset=*/  U8X8_PIN_NONE); 	// Set R/W to low!
//U8G2_HD44102_100X64_1 u8g2(U8G2_R0, 4, 5, 6, 7, 8, 9, 10, 11, /*enable=*/ 2, /*dc=*/ 3, /*cs0=*/ A0, /*cs1=*/ A1, /*cs2=*/ A2, /* reset=*/  U8X8_PIN_NONE); 	// Set R/W to low!
//U8G2_T7932_150X32_1 u8g2(U8G2_R0, 4, 5, 6, 7, 8, 9, 10, 11, /*enable=*/ 2, /*dc=*/ 3, /*cs0=*/ A0, /*cs1=*/ A1, /*cs2=*/ A2, /* reset=*/  U8X8_PIN_NONE); 	// Set R/W to low!
//U8G2_ST7920_256X32_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ U8X8_PIN_NONE, /*dc=*/ 17, /*reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_256X32_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 16 /* A2 */, /* CS=*/ 17 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_192X32_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ U8X8_PIN_NONE, /*dc=*/ 17, /*reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_192X32_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 16 /* A2 */, /* CS=*/ 17 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_128X32_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ U8X8_PIN_NONE, /*dc=*/ 17, /*reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_128X32_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 16 /* A2 */, /* CS=*/ 17 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_144X32_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ U8X8_PIN_NONE, /*dc=*/ 17, /*reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_144X32_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 16 /* A2 */, /* CS=*/ 17 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_160X32_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ U8X8_PIN_NONE, /*dc=*/ 17, /*reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_160X32_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 16 /* A2 */, /* CS=*/ 17 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_128X64_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18 /* A4 */, /*cs=*/ U8X8_PIN_NONE, /*dc/rs=*/ 17 /* A3 */, /*reset=*/ 15 /* A1 */);	// Remember to set R/W to 0 
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 16 /* A2 */, /* CS=*/ 17 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8);
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* CS=*/ 15, /* reset=*/ 16); // Feather HUZZAH ESP8266, E=clock=14, RW=data=13, RS=CS
//U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);
//U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 15, /* reset=*/ 16); // Feather HUZZAH ESP8266, E=clock=14, RW=data=13, RS=CS
//U8G2_ST7528_ERC16064_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7528_ERC16064_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7539_192X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7539_192X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_EA_DOGM128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_EA_DOGM128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_64128N_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_64128N_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_EA_DOGM132_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ U8X8_PIN_NONE);	// DOGM132 Shield
//U8G2_ST7565_EA_DOGM132_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ U8X8_PIN_NONE);	// DOGM132 Shield
//U8G2_ST7565_ZOLEN_128X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_ZOLEN_128X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_LM6059_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// Adafruit ST7565 GLCD
//U8G2_ST7565_LM6059_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);			// Adafruit ST7565 GLCD
//U8G2_ST7565_KS0713_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// KS0713 controller
//U8G2_ST7565_KS0713_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);			// KS0713 controller
//U8G2_ST7565_LX12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_LX12864_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_ERC12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_ERC12864_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_ERC12864_ALT_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); // contrast improved version for ERC12864
//U8G2_ST7565_ERC12864_ALT_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); // contrast improved version for ERC12864
//U8G2_ST7565_NHD_C12832_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_NHD_C12832_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_NHD_C12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_NHD_C12864_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_JLX12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_JLX12864_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7567_PI_132X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  // Pax Instruments Shield, LCD_BL=6
//U8G2_ST7567_PI_132X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  // Pax Instruments Shield, LCD_BL=6
//U8G2_ST7567_JLX12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_ST7567_JLX12864_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_ST7567_122X32_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_ST7567_122X32_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_ST7567_OS12864_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_ST7567_OS12864_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_ST7567_ENH_DG128064_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_ST7567_ENH_DG128064_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_ST7567_ENH_DG128064I_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_ST7567_ENH_DG128064I_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_ST7567_64X32_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
//U8G2_ST7567_HEM6432_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
//U8G2_ST7567_ERC13232_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_ST7567_ERC13232_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_ST7567_96X65_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_ST7567_96X65_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_ST7571_128X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7571_128X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7571_128X96_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7571_128X96_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7302_122X250_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7586S_JLX384160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);             // Uno: Enable U8g2 16 bit mode for this display
//U8G2_ST7586S_ERC240160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7586S_YMC240160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7586S_JLX320160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);             // Uno: Enable U8g2 16 bit mode for this display
//U8G2_ST75160_JM16096_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75160_JM16096_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75161_JLX160160_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75161_JLX160160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75256_JLX172104_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75256_JLX172104_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75256_JLX19296_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75256_JLX19296_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75256_JLX16080_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75256_JLX16080_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75256_JLX256128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX256128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);				// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_WO256X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_WO256X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);				// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX256128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 9, /* data=*/ 8, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 5);  // MKR Zero, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX256128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 5);  // MKR Zero, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX256160_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX256160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);				// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX256160M_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX256160M_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);				// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX256160_ALT_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX256160_ALT_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);				// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75256_JLX240160_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75256_JLX240160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	
//U8G2_ST75256_JLX25664_1_2ND_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);	// Due, 2nd I2C, enable U8g2 16 bit mode for this display
//U8G2_ST75320_JLX320240_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Uno, Enable U8g2 16 bit mode for this display
//U8G2_ST75320_JLX320240_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);				// Uno, Enable U8g2 16 bit mode for this display
//U8G2_NT7534_TG12864R_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_NT7534_TG12864R_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_ST7588_JLX12864_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ 5);  
//U8G2_ST7588_JLX12864_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ 5);
//U8G2_IST3020_ERC19264_1_6800 u8g2(U8G2_R0, 44, 43, 42, 41, 40, 39, 38, 37,  /*enable=*/ 28, /*cs=*/ 32, /*dc=*/ 30, /*reset=*/ 31); // Connect WR pin with GND
//U8G2_IST3020_ERC19264_1_8080 u8g2(U8G2_R0, 44, 43, 42, 41, 40, 39, 38, 37,  /*enable=*/ 29, /*cs=*/ 32, /*dc=*/ 30, /*reset=*/ 31); // Connect RD pin with 3.3V
//U8G2_IST3020_ERC19264_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_IST3088_320X240_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_IST3088_320X240_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_IST7920_128X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // Round display
//U8G2_IST7920_128X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // Round display
//U8G2_LC7981_160X80_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RW with GND
//U8G2_LC7981_160X160_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RW with GND
//U8G2_LC7981_240X128_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RW with GND
//U8G2_LC7981_240X64_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RW with GND
//U8G2_LC7981_128X128_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RW with GND
//U8G2_SED1520_122X32_1 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*dc=*/ A0, /*e1=*/ A3, /*e2=*/ A2, /* reset=*/  A4); 	// Set R/W to low!
//U8G2_T6963_240X128_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//U8G2_T6963_256X64_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//U8G2_T6963_160X80_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//U8G2_T6963_128X64_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//U8G2_T6963_128X64_ALT_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//U8G2_SED1330_240X64_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect /RD = E with +5V, enable is /WR = RW, FG with GND, 14=Uno Pin A0
//U8G2_SED1330_240X128_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect /RD = E with +5V, enable is /WR = RW, FG with GND, 14=Uno Pin A0
//U8G2_SED1330_240X128_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // A0 is dc pin, /WR = RW = GND, enable is /RD = E
//U8G2_SED1330_256X128_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect /RD = E with +5V, enable is /WR = RW, FG with GND, 14=Uno Pin A0, enable U8g2 16Bit Mode 
//U8G2_SED1330_256X128_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // A0 is dc pin, /WR = RW = GND, enable is /RD = E, enable U8g2 16Bit Mode 
//U8G2_SED1330_320X200_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect /RD = E with +5V, enable is /WR = RW, FG with GND, 14=Uno Pin A0, enable U8g2 16Bit Mode 
//U8G2_SED1330_320X200_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // A0 is dc pin, /WR = RW = GND, enable is /RD = E, enable U8g2 16Bit Mode 
//U8G2_RA8835_NHD_240X128_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect /RD = E with +5V, enable is /WR = RW, FG with GND, 14=Uno Pin A0
//U8G2_RA8835_NHD_240X128_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // A0 is dc pin, /WR = RW = GND, enable is /RD = E
//U8G2_UC1601_128X32_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_UC1601_128X32_1_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* reset=*/ 8);
//U8G2_UC1601_128X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_UC1604_JLX19264_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_UC1604_JLX19264_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_UC1609_SLG19264_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_UC1609_SLG19264_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//U8G2_UC1608_ERC24064_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // SW SPI, Due ERC24064-1 Test Setup
//U8G2_UC1608_DEM240064_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // SW SPI, Due ERC24064-1 Test Setup
//U8G2_UC1608_ERC240120_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//U8G2_UC1608_240X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // SW SPI, Due ERC24064-1 Test Setup
//U8G2_UC1610_EA_DOGXL160_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/  U8X8_PIN_NONE);
//U8G2_UC1610_EA_DOGXL160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/  U8X8_PIN_NONE);
//U8G2_UC1611_EA_DOGM240_1_2ND_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);	// Due, 2nd I2C, DOGM240 Test Board
//U8G2_UC1611_EA_DOGM240_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);   // SW SPI, Due DOGXL240 Test Board
//U8G2_UC1611_EA_DOGXL240_1_2ND_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);	// Due, 2nd I2C, DOGXL240 Test Board
//U8G2_UC1611_EA_DOGXL240_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);   // SW SPI, Due DOGXL240 Test Board
//U8G2_UC1611_EW50850_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 18, /*cs=*/ 3, /*dc=*/ 16, /*reset=*/ 17); // 240x160, Connect RD/WR1 pin with 3.3V, CS is aktive high
//U8G2_UC1611_CG160160_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 18, /*cs=*/ 3, /*dc=*/ 16, /*reset=*/ 17); // Connect WR1 and CD1 pin with 3.3V, connect CS0 with cs, WR0 with enable, CD with dc
//U8G2_UC1611_IDS4073_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8g2 16Bit Mode 
//U8G2_UC1611_IDS4073_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// Enable U8g2 16Bit Mode 
//U8G2_UC1617_JLX128128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1617_JLX128128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1628_128X64_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 18, /*cs=*/ 3, /*dc=*/ 16, /*reset=*/ 17); // UC1628 pin names: enable: /WR, dc: CD
//U8G2_UC1628_128X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1628_128X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1628_256X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1628_256X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1628_256X32_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1628_256X32_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1638_192X96_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1638_192X96_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1638_192X96_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* reset=*/ 8);  // u8g2 test board: I2C clock/data is same as SPI, I2C default address is 0x78
//U8G2_UC1638_240X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1638_240X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1638_240X128_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* reset=*/ 8);  // u8g2 test board: I2C clock/data is same as SPI, I2C default address is 0x78
//U8G2_SSD1606_172X72_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);		// eInk/ePaper Display
//U8G2_SSD1607_200X200_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// eInk/ePaper Display, original LUT from embedded artists
//U8G2_SSD1607_GD_200X200_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Good Display
//U8G2_SSD1607_WS_200X200_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Waveshare
//U8G2_IL3820_296X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// WaveShare 2.9 inch eInk/ePaper Display, enable 16 bit mode for this display!
//U8G2_IL3820_V2_296X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// ePaper Display, lesser flickering and faster speed, enable 16 bit mode for this display!
//U8G2_LS013B7DH03_128X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display
//U8G2_LS027B7DH01_400X240_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display, SPI Mode 2
//U8G2_LS027B7DH01_M0_400X240_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display, SPI Mode 0
//U8G2_LS013B7DH05_144X168_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);	// there is no DC line for this display
//U8G2_ST7511_AVD_320X240_1_8080 u8g2(U8G2_R0, 13, 11, 2, 3, 4, 5, 6, A4, /*enable/WR=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8); // Enable U8g2 16Bit Mode and connect RD pin with 3.3V/5V
//U8G2_S1D15300_LM6023_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_S1D15300_LM6023_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_S1D15300_100X32_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_S1D15300_100X32_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_S1D15300_100X32I_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_S1D15300_100X32I_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_S1D15721_240X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_S1D15721_240X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_GU800_128X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_GU800_128X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_GU800_160X16_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_GU800_160X16_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_GP1294AI_256X48_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/10, /* dc=*/U8X8_PIN_NONE, /* reset=*/8);




// End of constructor list

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

RTCZero rtc;


/*===================================================*/

uint8_t is_leap_year(uint16_t y);
uint16_t get_year_day_number(uint16_t y, uint8_t m, uint8_t d);
uint8_t get_month_by_year_day_number(uint16_t y, uint16_t ydn);
uint8_t get_day_by_year_day_number(uint16_t y, uint16_t ydn);
uint8_t get_weekday_by_year_day_number(uint16_t y, uint16_t ydn); /* returns day within a week: 0..6 with 0 = Sunday, 1 = Monday, ... */
uint16_t to_century_day_number(uint16_t y, uint16_t ydn);
void from_century_day_number(uint16_t cdn, uint16_t *year, uint16_t *ydn);
uint32_t to_time(uint16_t cdn, uint8_t h, uint8_t m, uint8_t s);
void from_time(uint32_t t, uint16_t *cdn, uint8_t *h, uint8_t *m, uint8_t *s);

uint32_t to_sec_since_2000(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi, uint8_t s);

uint32_t to_minutes(uint16_t cdn, uint8_t h, uint8_t m);
void from_minutes(uint32_t t, uint16_t *cdn, uint8_t *h, uint8_t *m);
uint32_t to_minutes_since_2000(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi);


/*===================================================*/

#define MENU_SMALL_FONT u8g2_font_baby_tr
#define MENU_NORMAL_FONT u8g2_font_ncenR08_tf
#define MENU_LARGE_FONT u8g2_font_ncenR10_tf
#define MENU_BIG_NUM u8g2_font_ncenR24_tn
#define MENU_BIG_NUM_FOCUS_XO 0
#define MENU_BIG_NUM_FOCUS_EXTRAX 1

typedef struct _menu_struct menu_t;
typedef struct _me_struct me_t;

typedef int (*me_cb)(menu_t *menu, const me_t *me, uint8_t msg);



struct _me_struct
{
  me_cb cb;
  void *val;
  void *arg;
  u8g2_uint_t x;
  u8g2_uint_t y;  
};


/* return 1, if this element can have focus */
#define ME_MSG_IS_FOCUS 1

/* draw focus graphics for the element */
#define ME_MSG_DRAW_FOCUS 2

/* user has pressed the select key */
#define ME_MSG_SELECT 3

/* advice for drawing */
#define ME_MSG_DRAW 4


struct _menu_struct
{
  u8g2_t *u8g2;
  
  volatile uint16_t current_index;	/* element which is processed right now */
  uint16_t focus_index;	/* element which has the focus at the moment */
  uint16_t radio_index;	/* if elements for a radio selection, then this is set by the cb */
  uint16_t me_count;	/* total number of elements in the list */
    
  /* pointer to the list of menu elements */
  const me_t *me_list;  
};

void menu_SetEdgePixel(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) U8G2_NOINLINE;
void menu_ClearEdgePixel(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) U8G2_NOINLINE;
void menu_DrawBoxFocus(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) U8G2_NOINLINE;
void menu_DrawFrameFocus(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) U8G2_NOINLINE;

void menu_Init(menu_t *menu, u8g2_t *u8g2);
void menu_SetMEList(menu_t *menu, const me_t *me_list, uint16_t initial_focus);
void menu_Draw(menu_t *menu);
void menu_NextFocus(menu_t *menu);
void menu_Select(menu_t *menu);


int me_cb_null(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_big_toggle(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_wd_toggle(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_5(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_9(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_9_ro(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_23(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_23_ro(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_9_small_ro(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_0_55(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_1_12(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_1_31(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_num_label(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_button_full_line(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_button_half_line(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_button_empty(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_scale_1_7(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_label(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_inv_label(menu_t *menu, const me_t *me, uint8_t msg);
int me_cb_16x16_bitmap_button(menu_t *menu, const me_t *me, uint8_t msg);

/*===================================================*/

u8g2_t *u8g2_ptr;

#define GUI_STATE_STOP 0
#define GUI_STATE_SIGNAL_ALARM 1
#define GUI_STATE_DISPLAY_TIME 2
#define GUI_STATE_MENU 3

#define GUI_ALARM_CNT 4
#define SNOOZE_MINUTES 5
#define ALLOW_SKIP_HOURS 10

struct _gui_data
{
  uint16_t uptime;				/* uptime in days, 10 bits, counts from 0 to 999, this value will be stored in the backup register */
  
  
  uint16_t week_time;			/* calculated: derived from h, mt, mo and weekday */
  uint8_t gui_state;				/* global running state, see guistate.c, defaults to 0 (GUI_STATE_STOP) */
  uint8_t h, mt, mo, st, so;		/* input: current time */
  uint8_t day; 					/* input: 1 .. 31 current day in month */
  uint8_t last_day;				/* last day. This is used to check, whether the day has changed. Required for uptime calc. This is also stored in the backup register. */
  uint8_t month;				/* input: 1..12 */
  uint8_t year_t, year_o;			/* input: current year */
  uint8_t weekday; 				/* calculated: 0 = Monday */
  
  uint8_t next_alarm_index;	/* calculated: index for the next alarm or GUI_ALARM_CNT if there is no next alarm */
  uint8_t is_skip_possible;		/* calculated: whether the next alarm (next_alarm_index) can be skipped */
  
  uint8_t is_equal;			/* calculated: whether the current time matches any alarm, will be set to 0 automatically */
  uint8_t equal_h;
  uint8_t equal_mt;
  uint8_t equal_mo;
  
  uint8_t is_alarm;			/* input/calculated: set by the software, has to be reset by the user */
  uint8_t active_alarm_idx;	/* input/calculated: set by the software, has to be reset by the user */
  uint8_t contrast;			/* value 1..7, 0 is default (do not set) */
  uint8_t display_voltage;		
  
  char s[16];				/* string buffer */
};
typedef struct _gui_data gui_data_t;


struct _gui_alarm_struct
{
  /* next alarm, all na_ fields are derived from the alarm information */
  uint16_t na_week_time_in_minutes;
  uint16_t na_minutes_diff;		/* calculated: time in minutes until next alarm, 0x0ffff = no alarm */
  uint8_t na_h;					/* calculated */
  uint8_t na_m;					/* calculated */
  uint8_t na_wd;				/* calculated: 0...7, 0=monday, 7=no alarm */
  
  /* alarm information */
  uint8_t snooze_count;	/* input, 1 bit*/
  volatile uint8_t enable;		/* input, 1 bit */
  uint8_t skip_wd;		/* input 0 = no skip, 1 = Monday, ... 3 bits*/
  uint8_t h;			/* input 5 bits */
  uint8_t m;			/* input 6 bits */
  uint8_t wd[7];		/* input: 0 or 1, 0=weekday not selected, 7 bits */
};
typedef struct _gui_alarm_struct gui_alarm_t;




extern const me_t melist_setup_time[];
extern const me_t melist_display_time[];
extern const me_t melist_setup_date[];
extern const me_t melist_setup_alarm[];
extern const me_t melist_alarm_menu[];
extern const me_t melist_setup_menu[];
extern const me_t melist_active_alarm_menu[];
extern const me_t melist_top_menu[];

extern gui_alarm_t gui_alarm_list[GUI_ALARM_CNT];
extern char gui_alarm_str[GUI_ALARM_CNT][8];
extern gui_data_t gui_data;
extern menu_t gui_menu;

void gui_date_adjust(void);


void gui_LoadData(void);
void gui_Recalculate(void);
void gui_SignalTimeChange(void);
void gui_Init(u8g2_t *u8g2, uint8_t is_por);
void gui_Draw(void);
void gui_Next(void);
void gui_Select(void);

void do_reset(void);
void store_gui_data(uint32_t *data);
void load_gui_data(uint32_t *data);
int is_por_reset(void);
int is_button_reset(void);
uint32_t get_boot_status_register(void);
uint32_t get_pwr_status_register(void);
uint32_t get_reset_reason(void);
uint32_t get_wakeup_count(void);
uint32_t get_dst_by_date(void);
uint32_t get_dst_by_RTC(void);

void enable_alarm(void);
void disable_alarm(void);
void set_time(uint8_t ht, uint8_t ho, uint8_t mt, uint8_t mo, uint8_t st, uint8_t so);
void set_date(uint8_t yt, uint8_t yo, uint8_t mt, uint8_t mo, uint8_t dayt, uint8_t dayo, uint8_t weekday);
void set_contrast(void);  /* set contrast to gui_data.contrast, value 1..7, 0 is default (do not set) */


/*===================================================*/

/*
  Prototype:
    uint8_t is_leap_year(uint16_t y)
  Description:
    Calculate leap year
  Arguments:
    y           year, e.g. 2011 for year 2011
  Result:
    0           not a leap year
    1           leap year
*/
uint8_t is_leap_year(uint16_t y)
{
   if ( 
          ((y % 4 == 0) && (y % 100 != 0)) || 
          (y % 400 == 0) 
      )
      return 1;
   return 0;
}

/*
  Prototype:
    uint16_t get_year_day_number(uint16_t y, uint8_t m, uint8_t d)
  Description:
    Calculate the day number within a year. 1st of Jan has the number 1.
    "Robertson" Algorithm IDAY (CACM Vol 15/#10/Oct 1972)
  Arguments:
    y           year, e.g. 2011 for year 2011
    m           month with 1 = january to 12 = december
    d           day starting with 1
  Result:
    The "day number" within the year: 1 for the 1st of Jan.
  See also:
    get_month_by_day_number()

*/
uint16_t get_year_day_number(uint16_t y, uint8_t m, uint8_t d)
{
  uint8_t tmp1; 
  uint16_t tmp2;
  tmp1 = 0;
  if ( m >= 3 )
    tmp1++;
  tmp2 = m;
  tmp2 +=2;
  tmp2 *=611;
  tmp2 /= 20;
  tmp2 += d;
  tmp2 -= 91;
  tmp1 <<=1;
  tmp2 -= tmp1;
  if ( tmp1 != 0 )
    tmp2 += is_leap_year(y);
  return tmp2;
}

/*
  Prototype:
    uint8_t get_month_by_year_day_number(uint16_t y, uint16_t ydn)
  Description:
    Get the month from year and day number within a year.
    "R. A. Stone" Algorithm (CACM Vol 13/#10/Oct 1970)
  Arguments:
    y           year, e.g. 2011 for year 2011
    ydn	year day number (1st of Jan has the number 1)
  Result:
    The month within the year: 1 for January.
  See also:
    get_year_day_number()
*/

static uint16_t corrected_year_day_number(uint16_t y, uint16_t ydn)
{
   uint8_t a;
   a = is_leap_year(y);
   if ( ydn > 59+a )
   {
      ydn += 2;
      ydn -= a;
   }
   ydn += 91;
   return ydn;
}

uint8_t get_month_by_year_day_number(uint16_t y, uint16_t ydn)
{
  uint8_t a;
  ydn = corrected_year_day_number(y, ydn);
  ydn *= 20;
  ydn /= 611;
  a = ydn;
  a -= 2;
  return a;  
}

/*
  Prototype:
    uint8_t get_day_by_year_day_number(uint16_t y, uint16_t ydn)
  Description:
    Get the day within month from year and day number within a year.
    "R. A. Stone" Algorithm (CACM Vol 13/#10/Oct 1970)
  Arguments:
    y           year, e.g. 2011 for year 2011
    ydn	year day number (1st of Jan has the number 1)
  Result:
    The day within a month: 1 for the first day of a month.
  See also:
    get_year_day_number()
*/
uint8_t get_day_by_year_day_number(uint16_t y, uint16_t ydn)
{
  uint8_t m;
  uint16_t tmp;
  m = get_month_by_year_day_number(y, ydn);
  m += 2;
  ydn = corrected_year_day_number(y, ydn);
  tmp = 611;
  tmp *= m;
  tmp /= 20;
  ydn -= tmp;
  return ydn;
}

/*
  Prototype:
    uint8_t get_weekday_by_year_day_number(uint16_t y, uint16_t ydn)
  Description:
    Get the day within week from year and day number within a year.
    "Zeller" Algorithm
    https://de.wikisource.org/wiki/Index:Acta_Mathematica_vol._009_(1886)
    https://ia801407.us.archive.org/8/items/actamathematica09upps/actamathematica09upps.pdf
  Arguments:
    y           year, e.g. 2011 for year 2011
    ydn	year day number (1st of Jan has the number 1)
  Result:
    The day within a week: 0..6 with 0 = Sunday, 1 = Monday, ...
  See also:
    get_year_day_number()
*/
uint8_t get_weekday_by_year_day_number(uint16_t y, uint16_t ydn)
{
  uint8_t j, c, tmp8;
  uint16_t tmp16;
  y--;
  j = y % 100;
  c = y / 100;
  tmp16 = c;
  tmp16 *= 5;
  tmp16 += ydn;
  tmp8 = j;
  j >>= 2;
  c >>= 2;
  tmp8 += j;
  tmp8 += c;
  tmp8 += 28;
  tmp16 += tmp8;
  tmp16 %= 7;
  return tmp16;  
}

/*
  Prototype:
    uint16_t to_century_day_number(uint16_t y, uint16_t ydn)
  Description:
    Calculate days since January, 1st, 2000
  Arguments:
    y           year, e.g. 2011 for year 2011
    ydn	year day number (1st of Jan has the number 1)
*/
uint16_t to_century_day_number(uint16_t y, uint16_t ydn)
{
  uint16_t cdn;
  cdn = ydn;
  cdn--;
  while( y > 2000 )
  {
    y--;
    cdn += 365;
    cdn += is_leap_year(y);
  }
  return cdn;
}

void from_century_day_number(uint16_t cdn, uint16_t *year, uint16_t *ydn)
{
  uint16_t y, days_per_year;
  y = 2000;
  for(;;)
  {
    days_per_year = 365;
    days_per_year += is_leap_year(y);
    if ( cdn >= days_per_year )
    {
      cdn -= days_per_year;
      y++;
    }
    else
      break;
  }
  cdn++;
  *year = y;
  *ydn = cdn;
}

/*
  Calculate the seconds after 2000-01-01 00:00. The largest possible
  time is 2136-02-07 06:28:15
*/
uint32_t to_time(uint16_t cdn, uint8_t h, uint8_t m, uint8_t s)
{
  uint32_t t;
  t = cdn;
  t *= 24;
  t += h;
  t *= 60;
  t += m;
  t *= 60;
  t += s;
  return t;
}


void from_time(uint32_t t, uint16_t *cdn, uint8_t *h, uint8_t *m, uint8_t *s)
{
  *s = t % 60;
  t /= 60;
  *m = t % 60;
  t /= 60;
  *h = t % 24;
  t /= 24;
  *cdn = t;
}

uint32_t to_sec_since_2000(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi, uint8_t s)
{
  uint16_t ydn = get_year_day_number(y, mo, d);
  uint16_t cdn = to_century_day_number(y, ydn);
  return to_time(cdn, h, mi, s);
}

/*
  Calculate the minutes after 2000-01-01 00:00. 
*/
uint32_t to_minutes(uint16_t cdn, uint8_t h, uint8_t m)
{
  uint32_t t;
  t = cdn;
  t *= 24;
  t += h;
  t *= 60;
  t += m;
  return t;
}


void from_minutes(uint32_t t, uint16_t *cdn, uint8_t *h, uint8_t *m)
{
  *m = t % 60;
  t /= 60;
  *h = t % 24;
  t /= 24;
  *cdn = t;
}

uint32_t to_minutes_since_2000(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi)
{
  uint16_t ydn = get_year_day_number(y, mo, d);
  uint16_t cdn = to_century_day_number(y, ydn);
  return to_minutes(cdn, h, mi);
}

/*===================================================*/

/* common menu functions */

void menu_DrawEdgePixel(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
{
  w--;
  h--;
  u8g2_DrawPixel(menu->u8g2, x,y);
  u8g2_DrawPixel(menu->u8g2, x+w,y);
  u8g2_DrawPixel(menu->u8g2, x,y+h);
  u8g2_DrawPixel(menu->u8g2, x+w,y+h);
}

void menu_ClearEdgePixel(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
{
  u8g2_SetDrawColor(menu->u8g2, 0);
  menu_DrawEdgePixel(menu, x, y, w, h);
  u8g2_SetDrawColor(menu->u8g2, 1);
}

void menu_DrawBoxFocus(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
{
  u8g2_SetDrawColor(menu->u8g2, 2);
  u8g2_DrawBox(menu->u8g2, x, y, w, h);
  menu_ClearEdgePixel(menu, x, y, w, h);
}

void menu_DrawFrameFocus(menu_t *menu, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
{
  menu_DrawEdgePixel(menu, x, y, w, h);
  x--;
  y--;
  w+=2;
  h+=2;
  u8g2_DrawFrame(menu->u8g2, x, y, w, h);
  menu_ClearEdgePixel(menu, x, y, w, h);
}


/*===================================================*/
// menu callbacks

/* this function must be the last function in the list. it also marks the end of a list */
int me_cb_null(menu_t *menu, const me_t *me, uint8_t msg)
{
  return 0;
}


/*
  Name: 	me_cb_big_toggle
  Val:	uint8_t *
  Arg:	Not used
*/
int me_cb_big_toggle(menu_t *menu, const me_t *me, uint8_t msg)
{
  uint8_t val = *(uint8_t *)(me->val);
  u8g2_uint_t x, y, w, h, w2;

  w = 16;
  w2 = 6;
  h = 10;
  x = me->x;
  y = me->y;
  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_DRAW_FOCUS:
      //menu_DrawFrameFocus(menu, x-1, y-1, w+2, h+2);
      menu_DrawFrameFocus(menu, x, y, w, h);
      return 1;
    case ME_MSG_SELECT:
      {
	val++;
	if ( val > 1 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
    case ME_MSG_DRAW:
      menu_DrawFrameFocus(menu, x+1,y+1,w-2,h-2);
      if ( val == 0 )
      {
	menu_DrawFrameFocus(menu, x+3,y+3,w2-2,h-6);
      }
      else
      {
	menu_DrawBoxFocus(menu, x+w/2,y+2,w2,h-4);
      }
      return 1;
    }  
    return 0;
}


/*
  Name: 	me_cb_wd_toggle
  Val:	uint8_t *
  Arg:	char *
*/
int me_cb_wd_toggle(menu_t *menu, const me_t *me, uint8_t msg)
{  
  uint8_t val = *(uint8_t *)(me->val);
  u8g2_uint_t x, y, w, h;

  u8g2_SetFont(menu->u8g2, MENU_SMALL_FONT);
  
  w = 13;
  h = u8g2_GetAscent(menu->u8g2)+2;
  x = me->x-2;
  y = me->y - u8g2_GetAscent(menu->u8g2)-1;
  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_DRAW_FOCUS:
      menu_DrawFrameFocus(menu, x, y, w, h);
      return 1;
    case ME_MSG_SELECT:
      {
	val++;
	if ( val > 1 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
    case ME_MSG_DRAW:
      u8g2_DrawUTF8(menu->u8g2, me->x, me->y, (const char *)(me->arg));
      if ( val > 0 )
      {
	menu_DrawBoxFocus(menu, x,y,w,h);
      }
	//u8g2_DrawRFrame(menu->u8g2, x, y, w, h, 1);
      return 1;
  }
  return 0;
}

/*
  Name: 	me_cb_0_9
  Val:	uint8_t *
*/
int me_cb_0_9(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_DRAW_FOCUS:
      u8g2_SetFont(menu->u8g2, MENU_BIG_NUM);
      menu_DrawBoxFocus(menu, 
	  me->x+MENU_BIG_NUM_FOCUS_XO, 
	  me->y - u8g2_GetAscent(menu->u8g2)-1, 
	  u8g2_GetGlyphWidth(menu->u8g2, '0')+MENU_BIG_NUM_FOCUS_EXTRAX, 
	  u8g2_GetAscent(menu->u8g2) + 2);
      return 1;
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val++;
	if ( val > 9 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_BIG_NUM);
      u8g2_DrawGlyph(menu->u8g2, me->x, me->y, *(uint8_t *)(me->val) + '0');
      return 1;
  }
  return 0;
}

int me_cb_0_9_ro(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_IS_FOCUS )
    return 0;
  return me_cb_0_9(menu, me, msg);
}


/*
  Name: 	me_cb_0_5
  Val:	uint8_t *
*/
int me_cb_0_5(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val++;
	if ( val > 5 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
  }
  return me_cb_0_9(menu, me, msg);
}

/*
  Name: 	me_cb_0_23
  Val:	uint8_t *
*/
int me_cb_0_23(menu_t *menu, const me_t *me, uint8_t msg)
{  
  char s[4];
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_DRAW_FOCUS:
      u8g2_SetFont(menu->u8g2, MENU_BIG_NUM);
      menu_DrawBoxFocus(menu, 
	  me->x+MENU_BIG_NUM_FOCUS_XO, 
	  me->y - u8g2_GetAscent(menu->u8g2)-1, 
	  u8g2_GetGlyphWidth(menu->u8g2, '0')*2+MENU_BIG_NUM_FOCUS_EXTRAX, 
	  u8g2_GetAscent(menu->u8g2) + 2);
      return 1;
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val++;
	if ( val > 23 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_BIG_NUM);
      s[0] = *(uint8_t *)(me->val);
      s[1] = s[0];
      s[1] %= 10;
      s[1] += '0';
      s[0] /= 10;
      s[0] += '0';
      s[2] = '\0';
	u8g2_DrawUTF8(menu->u8g2, me->x, me->y, s);
      return 1;
  }
  return 0;
}

int me_cb_0_23_ro(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_IS_FOCUS )
    return 0;
  return me_cb_0_23(menu, me, msg);
}


int me_cb_0_9_small_ro(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 0;
    case ME_MSG_DRAW_FOCUS:
      return 1;
    case ME_MSG_SELECT:
      return 1;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_SMALL_FONT);
      u8g2_DrawGlyph(menu->u8g2, me->x, me->y, *(uint8_t *)(me->val) + '0');
      return 1;
  }
  return 0;
}


/*
  Name: 	me_cb_0_55
  Val:	uint8_t *
*/
int me_cb_0_55(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val+=5;
	if ( val > 55 )
	  val = 0;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
  }
  return me_cb_0_23(menu, me, msg);
}

/*
  Name: 	me_cb_1_12
  Val:	uint8_t *
*/
int me_cb_1_12(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val++;
	if ( val > 12 )
	  val = 1;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
  }
  return me_cb_0_23(menu, me, msg);
}

/*
  Name: 	me_cb_1_31
  Val:	uint8_t *
*/
int me_cb_1_31(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_SELECT:
      {
	uint8_t val = *(uint8_t *)(me->val);
	val++;
	if ( val > 31 )
	  val = 1;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
  }
  return me_cb_0_23(menu, me, msg);
}

/*
  Name: 	me_cb_num_label
  can not get focus
  Arg:	char *
*/
int me_cb_num_label(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
    case ME_MSG_DRAW_FOCUS:
    case ME_MSG_SELECT:
      break;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_BIG_NUM);
      u8g2_DrawUTF8(menu->u8g2, me->x, me->y, (char *)(me->arg) );
      return 1;
  }
  return 0;
}

/*
  Name: 	me_cb_button_full_line
  Val:	callback function
  Arg:	char *
*/
int me_cb_button_full_line(menu_t *menu, const me_t *me, uint8_t msg)
{  
  int r = 0;
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_DRAW_FOCUS:
      menu_DrawBoxFocus(menu, 
	  0, 
	  me->y - u8g2_GetAscent(menu->u8g2)-1, 
	  u8g2_GetDisplayWidth(menu->u8g2) , 
	  u8g2_GetAscent(menu->u8g2) - u8g2_GetDescent(menu->u8g2) +1);
      r = 1;
      break;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
      u8g2_DrawUTF8(menu->u8g2, me->x, me->y, (char *)(me->arg) );
      r = 1;
      break;
  }
  /* pass all messages except for the IS_FOCUS also to the callback function */
  if ( me->val != NULL )
    return ((me_cb)(me->val))(menu, me, msg) | r;
  return r;
}

/*
  Name: 	me_cb_button_full_line
  Val:	callback function
  Arg:	char *
*/
int me_cb_button_half_line(menu_t *menu, const me_t *me, uint8_t msg)
{  
  int r = 0;
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_DRAW_FOCUS:
      menu_DrawBoxFocus(menu, 
	  me->x, 
	  me->y - u8g2_GetAscent(menu->u8g2)-1, 
	  u8g2_GetDisplayWidth(menu->u8g2)/2, 
	  u8g2_GetAscent(menu->u8g2) - u8g2_GetDescent(menu->u8g2) +1);
      r = 1;
      break;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
      u8g2_DrawUTF8(menu->u8g2, me->x+4, me->y, (char *)(me->arg) );
      r = 1;
      break;    
  }
  /* pass all messages except for the IS_FOCUS also to the callback function */
  if ( me->val != NULL )
    return ((me_cb)(me->val))(menu, me, msg) | r;
  return r;
}

/*
  Name: 	me_cb_button_empty
  Val:	callback function
  Arg:	not used
*/
int me_cb_button_empty(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_IS_FOCUS )
    return 1;
  if ( me->val != NULL )
    return ((me_cb)(me->val))(menu, me, msg);
  return 0;
}

/*
  Name: 	me_cb_scale_1_7
  Val:	uint8_t *
*/

void set_contrast(void);	


int me_cb_scale_1_7(menu_t *menu, const me_t *me, uint8_t msg)
{  
  u8g2_uint_t x;
  uint8_t val = *(uint8_t *)(me->val);
  
  if ( val <= 0 )
    val = 1;
  x = me->x+(val-1)*5;
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_DRAW_FOCUS:
      /*
      u8g2_SetFont(menu->u8g2, MENU_BIG_NUM);
      menu_DrawBoxFocus(menu, 
	  me->x+MENU_BIG_NUM_FOCUS_XO, 
	  me->y - u8g2_GetAscent(menu->u8g2)-1, 
	  u8g2_GetGlyphWidth(menu->u8g2, '0')+MENU_BIG_NUM_FOCUS_EXTRAX, 
	  u8g2_GetAscent(menu->u8g2) + 2);
    */
      u8g2_DrawBox(menu->u8g2,  x-2 , me->y-2, 5, 5);
      return 1;
    case ME_MSG_SELECT:
      {
	val++;
	if ( val > 7 )
	  val = 1;
	*(uint8_t *)(me->val) = val;
      }
      return 1;
    case ME_MSG_DRAW:
      set_contrast();		/* give user feedback... not so nice: We assume *(uint8_t *)(me->val) points to gui_data.contrast */

      //u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
      //u8g2_DrawGlyph(menu->u8g2, me->x, me->y-2, *(uint8_t *)(me->val) + '0');
    
      u8g2_DrawHLine(menu->u8g2,  me->x, me->y, 6*5+1);
    
      u8g2_DrawVLine(menu->u8g2,  me->x, me->y-2, 5);
      //u8g2_DrawVLine(menu->u8g2,  me->x+1*5, me->y-1, 3);
      //u8g2_DrawVLine(menu->u8g2,  me->x+2*5, me->y-1, 3);
      u8g2_DrawVLine(menu->u8g2,  me->x+3*5, me->y-2, 5);
      //u8g2_DrawVLine(menu->u8g2,  me->x+4*5, me->y-1, 3);
      //u8g2_DrawVLine(menu->u8g2,  me->x+5*5, me->y-1, 3);
      u8g2_DrawVLine(menu->u8g2,  me->x+6*5, me->y-2, 5);
    
      u8g2_DrawFrame(menu->u8g2,  x-3 , me->y-3, 7, 7);
      u8g2_SetDrawColor(menu->u8g2, 0);
      u8g2_DrawBox(menu->u8g2,  x-2 , me->y-2, 5, 5);
      /* draw color is set to 1 in the following function */
      menu_ClearEdgePixel(menu,  x-3 , me->y-3, 7, 7);
      menu_DrawEdgePixel(menu,  x-2 , me->y-3+1, 5, 5);
      return 1;
  }
  return 0;
}


/*
  Name: 	me_cb_label
  can not get focus
  Arg:	char *
*/
int me_cb_label(menu_t *menu, const me_t *me, uint8_t msg)
{  
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
    case ME_MSG_DRAW_FOCUS:
    case ME_MSG_SELECT:
      break;
    case ME_MSG_DRAW:
      u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
      u8g2_DrawUTF8(menu->u8g2, me->x, me->y, (char *)(me->arg) );
      return 1;
  }
  return 0;
}

int me_cb_inv_label(menu_t *menu, const me_t *me, uint8_t msg)
{  
  int r = me_cb_label(menu, me, msg);
  if ( msg == ME_MSG_DRAW )
  {
      menu_DrawBoxFocus(menu, 
	  me->x-1, 
	  me->y - u8g2_GetAscent(menu->u8g2)-1, 
	  u8g2_GetUTF8Width(menu->u8g2, (char *)(me->arg))+2, 
	  u8g2_GetAscent(menu->u8g2) + 2);
  }
  return r;
}

/*
  Name: 	me_cb_button_full_line
  Val:	callback function
  Arg:	bitmap
*/
int me_cb_16x16_bitmap_button(menu_t *menu, const me_t *me, uint8_t msg)
{  
  int r = 0;
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return 1;
    case ME_MSG_DRAW_FOCUS:
      /*
      menu_DrawFrameFocus(menu, 
	  me->x-1, 
	  me->y-1, 
	  16+2, 
	  16+2);
    */
      menu_DrawFrameFocus(menu, 
	  me->x, 
	  me->y, 
	  16, 
	  16);
    
      r = 1;
      break;
    case ME_MSG_DRAW:
      u8g2_DrawXBM(menu->u8g2, me->x, me->y, 16, 16, (const uint8_t *)(me->arg));
      r = 1;
      break;
  }
  /* pass all messages except for the IS_FOCUS also to the callback function */
  if ( me->val != NULL )
    return ((me_cb)(me->val))(menu, me, msg) | r;
  return r;
}

/*===================================================*/
// menu API

/* call menu element from menu->current_index */
int menu_CallME(menu_t *menu, uint8_t msg)
{
  const me_t *me;
  me = menu->me_list+menu->current_index;
  return me->cb(menu, me, msg);
}

/* stay on current focus if valid, move to next valid focus */
static void menu_CalcNextValidFocus(menu_t *menu) U8G2_NOINLINE;
static void menu_CalcNextValidFocus(menu_t *menu)
{
  for(;;)
  {
    menu->current_index = menu->focus_index;
    if ( menu->current_index >= menu->me_count )
      break;    
    if ( menu_CallME(menu, ME_MSG_IS_FOCUS) != 0 )
      break;
    menu->focus_index++;
  }
}

/* advance current focus to the next element */
void menu_NextFocus(menu_t *menu)
{
  menu->focus_index++;
  if ( menu->focus_index >= menu->me_count )
    menu->focus_index = 0;
  menu_CalcNextValidFocus(menu);
}

/* send select message to the element which has the current focus */
void menu_Select(menu_t *menu)
{
  menu->current_index = menu->focus_index;
  menu_CallME(menu, ME_MSG_SELECT);
}



void menu_SetMEList(menu_t *menu, const me_t *me_list, uint16_t initial_focus)
{
  menu->me_list = me_list;
  
  menu->me_count = 0;
  while( me_list[menu->me_count].cb !=  me_cb_null )
    menu->me_count++;
  
  menu->focus_index = 0;
  menu_CalcNextValidFocus(menu);
  
  while( initial_focus > 0 )
  {
    menu_NextFocus(menu);
    initial_focus--;
  }
  
  menu->radio_index = menu->me_count;
}

me_t melist_emty[] = 
{
  { me_cb_null, NULL, 0, 0 }
};


void menu_Init(menu_t *menu, u8g2_t *u8g2)
{
  menu->u8g2 = u8g2;
  menu_SetMEList(menu, melist_emty, 0);
}



void menu_Draw(menu_t *menu)
{
  for( menu->current_index = 0; menu->current_index < menu->me_count; menu->current_index++ )
  {
    menu_CallME(menu, ME_MSG_DRAW);
    if ( menu->current_index == menu->focus_index )
    {
      menu_CallME(menu, ME_MSG_DRAW_FOCUS);
    }
  }
   // u8g2_DrawHLine(menu->u8g2, 0, 32, 128);
}

/*===================================================*/

/*============================================*/
/* reset */
void do_reset(void)
{
}

/*============================================*/
/* load & store from/to permanent memory */


/* 5x 32bit */

void store_gui_data(uint32_t *data)
{
  /*
  FILE *fp;
  fp = fopen("clock.dat", "w");
  if ( fp != NULL )
  {
    fwrite(data, sizeof(uint32_t), 5, fp);
    fclose(fp);
  }
  */
}

void load_gui_data(uint32_t *data)
{
  /*
  FILE *fp;

  int i;
  
  for( i = 0; i < GUI_ALARM_CNT; i++ )
    data[i] = 0;
  
  fp = fopen("clock.dat", "r");
  if ( fp != NULL )
  {
    fread(data, sizeof(uint32_t), 5, fp);
    fclose(fp);
  }
  */
}


/*============================================*/
/* input */

int is_por_reset(void)
{
  return 1;
}

int is_button_reset(void)
{
  return 0;
}

uint32_t get_boot_status_register(void)
{
  return 0;
}

uint32_t get_pwr_status_register(void)
{
  return 0;
}

uint32_t get_reset_reason(void)
{
  return 0;
}

uint32_t get_wakeup_count(void)
{
  return 0;
}

uint32_t get_dst_by_date(void)
{
  return -1;
}

uint32_t get_dst_by_RTC(void)
{
  return 0;
}

/*============================================*/
/* output */

void enable_alarm(void)
{
}

void disable_alarm(void)
{
}

void set_time(uint8_t ht, uint8_t ho, uint8_t mt, uint8_t mo, uint8_t st, uint8_t so)
{
}

void set_date(uint8_t yt, uint8_t yo, uint8_t mt, uint8_t mo, uint8_t dayt, uint8_t dayo, uint8_t weekday)
{
}

/* set contrast to gui_data.contrast, value 1..7, 0 is default (do not set) */
void set_contrast(void)
{
}

/*===================================================*/


/*============================================*/

/* global variable for the gui menues */

uint8_t gui_alarm_index = 0;
gui_alarm_t gui_alarm_current;

const char weekdaystr[7][4] = {
  "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"
};




/*============================================*/

const static uint8_t ok_xbm[] = {	/* 16x16 */
   0xfe, 0x7f, 0x03, 0xc0, 0x01, 0x80, 0x01, 0xb8, 0x01, 0x9c, 0x01, 0x8e,
   0x01, 0x87, 0x01, 0x87, 0x9d, 0x83, 0xb9, 0x83, 0xf1, 0x81, 0xe1, 0x81,
   0xc1, 0x80, 0x01, 0x80, 0x03, 0xc0, 0xfe, 0x7f };

const static uint8_t alarm_xbm[] = {  /* 12x12 */
0x00, 0x00, 0x0c, 0x06, 0xf6, 0x0d, 0x1a, 0x0b, 0x4c, 0x06, 0x44, 0x04,
0xc4, 0x05, 0x04, 0x04, 0x0c, 0x06, 0x18, 0x03, 0xf0, 0x01, 0x00, 0x00 };

   
int me_action_to_top_menu(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_top_menu, 0);
    return 1;
  }
  return 0;
}

int me_action_to_setup_menu(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_setup_menu, 0);
    return 1;
  }
  return 0;
}



int me_action_save_time(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    set_time(gui_data.h / 10, gui_data.h % 10, gui_data.mt, gui_data.mo, gui_data.st, gui_data.so);
    //menu_SetMEList(menu, melist_top_menu, 0);	/* first set the normal menu */
    menu_SetMEList(menu, melist_setup_menu, 0);
    gui_Recalculate();							/* because it might be overwritten with the alarm menu */
    return 1;
  }
  return 0;
}

#define ME_TIME_Y 26


#define ME_TIME_XO 11
const me_t melist_setup_time[] = 
{
  { me_cb_0_23, &gui_data.h, NULL, 		1,ME_TIME_Y },
  { me_cb_num_label, NULL, (void *)":", 			37,ME_TIME_Y-3 },
  { me_cb_0_5, &gui_data.mt, NULL, 		45,ME_TIME_Y },
  { me_cb_0_9, &gui_data.mo, NULL, 		63,ME_TIME_Y },
  { me_cb_num_label, NULL, (void *)":", 			81,ME_TIME_Y-3 },
  { me_cb_0_5, &gui_data.st, NULL, 		89,ME_TIME_Y },
  { me_cb_0_9, &gui_data.so, NULL, 		107,ME_TIME_Y },
  { me_cb_button_half_line, (void *)me_action_to_setup_menu, (void *)"Abbrechen", 0,42 },
  { me_cb_button_half_line, (void *)me_action_save_time, (void *)"Speichern", 64,42 },
  
  //{ me_cb_button_full_line, (void *)me_action_save_time, "Speichern", 40,42 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


/*============================================*/
/* Display Time */

void gui_alarm_to_str(uint8_t idx)
{
      strcpy(gui_data.s, weekdaystr[gui_alarm_list[gui_data.next_alarm_index].na_wd]);
      gui_data.s[2] = ',';
      gui_data.s[3] = ' ';
      strcpy(gui_data.s+4, u8x8_u8toa(gui_alarm_list[gui_data.next_alarm_index].na_h, 2));
      gui_data.s[6] = ':';
      strcpy(gui_data.s+7, u8x8_u8toa(gui_alarm_list[gui_data.next_alarm_index].na_m, 2));
      if ( gui_alarm_list[gui_data.next_alarm_index].snooze_count != 0 )
      {
	gui_data.s[9] = '+';
	gui_data.s[10] = '\0';
      }
      else
      {
	gui_data.s[9] = '\0';      
      }
}

int me_action_handle_display_time(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_DRAW )
  {
    char s[14];
    u8g2_uint_t w;


    u8g2_uint_t x = 27;
    u8g2_uint_t y = 61;
    u8g2_SetFont(menu->u8g2, MENU_LARGE_FONT);

    if ( gui_data.next_alarm_index < GUI_ALARM_CNT )
    {
      gui_alarm_to_str(gui_data.next_alarm_index);
      w = u8g2_GetUTF8Width(menu->u8g2, gui_data.s)+14;
      //u8g2_DrawUTF8(menu->u8g2, x+14, y, gui_data.s);
      u8g2_DrawXBM(menu->u8g2, (128-w)/2, y-11, 12, 12, (const uint8_t *)(alarm_xbm));
      u8g2_DrawUTF8(menu->u8g2, (128-w)/2+14, y, gui_data.s);
    }
    
    y -= 17;
    x -= 3;
    
    strcpy(s, weekdaystr[gui_data.weekday]);
    s[2] = ',';
    s[3] = ' ';
    strcpy(s+4, u8x8_u8toa(gui_data.day, 2));
    s[6] = '.';
    strcpy(s+7, u8x8_u8toa(gui_data.month, 2));
    s[9] = '.';
    s[10] = gui_data.year_t+'0';
    s[11] = gui_data.year_o+'0';
    s[12] = '\0';

    w = u8g2_GetUTF8Width(menu->u8g2, s);

    u8g2_DrawUTF8(menu->u8g2, (128-w)/2, y, s);
    
    return 1;
  }
  else if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_top_menu, 0);
    return 1;
  }
  return 0;
}


#define ME_TIME_DXO 29
const me_t melist_display_time[] = 
{
  { me_cb_0_23_ro, 	&gui_data.h, NULL, 		ME_TIME_DXO-7,ME_TIME_Y },
  { me_cb_num_label, NULL, (void *)":", 			ME_TIME_DXO+30,ME_TIME_Y-3 },
  { me_cb_0_9_ro, &gui_data.mt, NULL, 		ME_TIME_DXO+39,ME_TIME_Y },
  { me_cb_0_9_ro, &gui_data.mo, NULL, 		ME_TIME_DXO+57,ME_TIME_Y },

  { me_cb_0_9_small_ro, &gui_data.st, NULL, 		118,ME_TIME_Y },
  { me_cb_0_9_small_ro, &gui_data.so, NULL, 		123,ME_TIME_Y },

    
  { me_cb_button_empty, (void *)me_action_handle_display_time, NULL, 0, 0 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Date Edit Dialog */

int me_action_save_date(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    gui_date_adjust();	/* calculate the weekday */
    
    set_date(gui_data.year_t, gui_data.year_o, gui_data.month / 10, gui_data.month % 10, gui_data.day / 10 , gui_data.day % 10, gui_data.weekday);
    
    menu_SetMEList(menu, melist_setup_menu, 0);	/* first set the normal menu */
    gui_Recalculate();							/* because it might be overwritten with the alarm menu */
    return 1;
  }
  return 0;
}



const me_t melist_setup_date[] = 
{
  { me_cb_1_31, &gui_data.day, NULL, 		1,ME_TIME_Y },
  { me_cb_num_label, NULL, (void *)".", 			37,ME_TIME_Y },
  { me_cb_1_12, &gui_data.month, NULL, 	45,ME_TIME_Y },
  { me_cb_num_label, NULL, (void *)".", 			81,ME_TIME_Y },
  { me_cb_0_9, &gui_data.year_t, NULL, 		89,ME_TIME_Y },
  { me_cb_0_9, &gui_data.year_o, NULL, 	107,ME_TIME_Y },
  //{ me_cb_button_full_line, (void *)me_action_save_date, (void *)"Speichern", 40,42 },

  { me_cb_button_half_line, (void *)me_action_to_setup_menu, (void *)"Abbrechen", 0,42 },
  { me_cb_button_half_line, (void *)me_action_save_date, (void *)"Speichern", 64,42 },

  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Alarm Edit Dialog */



int me_action_alarm_done(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    gui_alarm_list[gui_alarm_index] = gui_alarm_current;
    gui_alarm_list[gui_alarm_index].skip_wd = 0;		/* clear the skip alarm (if any) */
    gui_alarm_list[gui_alarm_index].snooze_count = 0;		/* clear snooze (if any) */
    //gui_alarm_calc_str_time(gui_alarm_index);
    menu_SetMEList(menu, melist_alarm_menu, gui_alarm_index);	/* first set the normal menu */
    gui_Recalculate();							/* because it might be overwritten with the alarm menu */
   return 1;
  }
  return 0;
}

#define ME_ALARM_TIME_XO 28
#define ME_ALARM_TIME_Y 26
#define ME_ALARM_WD_Y 36
#define ME_ALARM_WD_XO 8
const me_t melist_setup_alarm[] = 
{

  { me_cb_0_23, &(gui_alarm_current.h), NULL, 		ME_ALARM_TIME_XO-7,ME_ALARM_TIME_Y },
  { me_cb_num_label, NULL, (void *)":", 					ME_ALARM_TIME_XO+30,ME_ALARM_TIME_Y-3 },
  { me_cb_0_55, &(gui_alarm_current.m), NULL, 		ME_ALARM_TIME_XO+39,ME_ALARM_TIME_Y },
    
  { me_cb_wd_toggle, &(gui_alarm_current.wd[0]), (void *)weekdaystr[0], 	ME_ALARM_WD_XO+17*0, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[1]), (void *)weekdaystr[1], 	ME_ALARM_WD_XO+17*1, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[2]), (void *)weekdaystr[2], 	ME_ALARM_WD_XO+17*2, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[3]), (void *)weekdaystr[3], 	ME_ALARM_WD_XO+17*3, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[4]), (void *)weekdaystr[4], 	ME_ALARM_WD_XO+17*4, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[5]), (void *)weekdaystr[5], 	ME_ALARM_WD_XO+17*5, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[6]), (void *)weekdaystr[6], 	ME_ALARM_WD_XO+17*6, ME_ALARM_WD_Y},

  { me_cb_big_toggle, (void *)&(gui_alarm_current.enable), NULL,  28, 47},

  { me_cb_16x16_bitmap_button, (void *)me_action_alarm_done, (void *)ok_xbm, 80, 44 },

  { me_cb_null, NULL, NULL, 0, 0 },
};


/*============================================*/
/* Alarm Setup Menu */


static int me_action_alarm_common(menu_t *menu, const me_t *me, uint8_t msg) U8G2_NOINLINE;
static int me_action_alarm_common(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    gui_alarm_current = gui_alarm_list[gui_alarm_index];
    menu_SetMEList(menu, melist_setup_alarm, 0);
    return 1;
  }
  return 0;
}

int me_action_alarm1(menu_t *menu, const me_t *me, uint8_t msg)
{
  gui_alarm_index = 0;
  return me_action_alarm_common(menu, me, msg);
}

int me_action_alarm2(menu_t *menu, const me_t *me, uint8_t msg)
{
  gui_alarm_index = 1;
  return me_action_alarm_common(menu, me, msg);
}
int me_action_alarm3(menu_t *menu, const me_t *me, uint8_t msg)
{
  gui_alarm_index = 2;
  return me_action_alarm_common(menu, me, msg);
}
int me_action_alarm4(menu_t *menu, const me_t *me, uint8_t msg)
{
  gui_alarm_index = 3;
  return me_action_alarm_common(menu, me, msg);
}

int me_action_handle_alarm_list(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_DRAW )
  {
    uint8_t y, x;
    uint8_t ym;
    u8g2_SetFont(menu->u8g2, MENU_SMALL_FONT);
    for( y = 0; y < 4; y++ )
    {
      ym = y*8+7;
      for( x = 0; x < 7; x++ )
      {
	u8g2_DrawGlyph(menu->u8g2, 0, ym, y+'1');
	u8g2_DrawGlyph(menu->u8g2, 5, ym, ':');
	u8g2_DrawStr(menu->u8g2, 9, ym, gui_alarm_str[y]);
	if ( gui_alarm_list[y].wd[x] )
	{
	  u8g2_DrawStr(menu->u8g2, 40+x*12, ym, weekdaystr[x]);
	}
      }
    }
  }
  else if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_alarm_menu, 4);
    return 1;
  }
  return 0;
}

const me_t melist_alarm_list_menu[] = 
{
  { me_cb_button_empty, (void *)me_action_handle_alarm_list, NULL, 0, 0 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

int me_action_goto_alarm_list(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_alarm_list_menu, 0);
    return 1;
  }
  return 0;
}

const me_t melist_alarm_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_alarm1, gui_alarm_str[0], 0,12 },
  { me_cb_button_half_line, (void *)me_action_alarm2, gui_alarm_str[1], 64,12 },
  { me_cb_button_half_line, (void *)me_action_alarm3, gui_alarm_str[2], 0,24 },
  { me_cb_button_half_line, (void *)me_action_alarm4, gui_alarm_str[3], 64,24 },
  { me_cb_button_half_line, (void *)me_action_goto_alarm_list, (void *)"Liste", 0,36 },
  { me_cb_button_half_line, (void *)me_action_to_top_menu, (void *)"Zurück", 64,36 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


/*============================================*/
/* Reset Menu */

int me_action_reset_no(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_setup_menu, 0);
    return 1;
  }
  return 0;
}

int me_action_reset_yes(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    do_reset();
    return 1;
  }
  return 0;
}

const me_t melist_reset_menu[] = 
{
  { me_cb_label, NULL, (void *)"Reset?",			44, 13},
  { me_cb_button_half_line, (void *)me_action_reset_no, (void *)"Nein", 0,30 },
  { me_cb_button_half_line, (void *)me_action_reset_yes, (void *)"Ja", 64,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Boot Info */

const char *bitstr(uint32_t v, int bitcnt)
{
  static char s[34];
  char *t = s;
  uint32_t mask;
  
  mask = 1<<(bitcnt-1);
  while( mask > 0 )
  {
    *t = ((v & mask) == 0 ? '0' : '1');
    t++;
    mask >>= 1;
  }
  *t = '\0';
  return s;
}

static const char *reset_reason_str[] = { "POR", "NVIC", "T2", "T3", "WUF" };

int me_action_handle_boot_info(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_DRAW )
  {
    u8g2_SetFont(menu->u8g2, MENU_SMALL_FONT);
    u8g2_DrawStr(menu->u8g2, 0, 8-1, "RCC_CSR 31..24");
    u8g2_DrawStr(menu->u8g2, 64, 8-1, bitstr(get_boot_status_register(), 8));

    u8g2_DrawStr(menu->u8g2, 0, 16-1, "PWR_CSR 7..0");
    u8g2_DrawStr(menu->u8g2, 64, 16-1, bitstr(get_pwr_status_register(), 8));

    u8g2_DrawStr(menu->u8g2, 0, 24-1, "ResetReason");
    u8g2_DrawStr(menu->u8g2, 64, 24-1, reset_reason_str[get_reset_reason()]);

    u8g2_DrawStr(menu->u8g2, 0, 32-1, "Uptime");
    u8g2_DrawStr(menu->u8g2, 64, 32-1, u8x8_u16toa(gui_data.uptime, 3));

    u8g2_DrawStr(menu->u8g2, 0, 40-1, "Wakeups");
    u8g2_DrawStr(menu->u8g2, 64, 40-1, u8x8_u16toa(get_wakeup_count(), 5));

    u8g2_DrawStr(menu->u8g2, 0, 48-1, "DST by Date:");
    u8g2_DrawStr(menu->u8g2, 50, 48-1, u8x8_u16toa(get_dst_by_date(), 2));
    u8g2_DrawStr(menu->u8g2, 64, 48-1, "by RTC:");
    u8g2_DrawStr(menu->u8g2, 95, 48-1, u8x8_u16toa(get_dst_by_RTC(), 1));

    
    
  }
  else if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_display_time, 4);
    return 1;
  }
  return 0;
}


const me_t melist_boot_info_menu[] = 
{
  { me_cb_button_empty, (void *)me_action_handle_boot_info, NULL, 0, 0 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* System Menu */


const me_t melist_system_menu[] = 
{
  { me_cb_label, NULL, (void *)"Helligkeit:",	 0, 10},  
  { me_cb_scale_1_7, &(gui_data.contrast), NULL, 103-10, 8},
  { me_cb_label, NULL, (void *)"Batteriespannung:",	 0, 10+12},  
  { me_cb_big_toggle, &(gui_data.display_voltage), NULL, 100-7, 10+12-8},

  { me_cb_button_full_line, (void *)me_action_to_setup_menu, (void *)"Speichern", 40,10+2*12 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* System 2 Menu */

int me_action_goto_boot_info(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_boot_info_menu, 0);
    return 1;
  }
  return 0;
}

int me_action_goto_reset(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_reset_menu, 0);
    return 1;
  }
  return 0;
}

const me_t melist_system_2_menu[] = 
{
  //{ me_cb_button_half_line, (void *)me_action_setup_time, "Uhrzeit", 0,10 },
  //{ me_cb_button_half_line, (void *)me_action_setup_date, "Datum", 64,10 },
  { me_cb_button_half_line, (void *)me_action_goto_boot_info, (void *)"Info", 0,20 },
  { me_cb_button_half_line, (void *)me_action_goto_reset, (void *)"Reset", 64,20 },
  { me_cb_button_full_line, (void *)me_action_to_setup_menu, (void *)"Zurück", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


/*============================================*/
/* Setup Menu */

int me_action_setup_time(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_setup_time, 0);
    return 1;
  }
  return 0;
}

int me_action_setup_date(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_setup_date, 0);
    return 1;
  }
  return 0;
}


int me_action_goto_system(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_system_menu, 0);
    return 1;
  }
  return 0;
}

int me_action_goto_system_2(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    //menu_SetMEList(menu, melist_reset_menu, 0);
    menu_SetMEList(menu, melist_system_2_menu, 0);
    return 1;
  }
  return 0;
}


const me_t melist_setup_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_setup_time, (void *)"Uhrzeit", 0,10 },
  { me_cb_button_half_line, (void *)me_action_setup_date, (void *)"Datum", 64,10 },
  { me_cb_button_half_line, (void *)me_action_goto_system, (void *)"Anzeige", 0,20 },
  { me_cb_button_half_line, (void *)me_action_goto_system_2, (void *)"System", 64,20 },
  { me_cb_button_full_line, (void *)me_action_to_top_menu, (void *)"Zurück", 40,31 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Alarm Menu */


int me_action_deactivate_alarm(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_display_time, 0);
    return 1;
  }
  return 0;
}

int me_action_do_snooze(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_display_time, 0);
    return 1;
  }
  return 0;
}

int me_cb_cond_inv_label(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( gui_data.is_alarm == 0 )
    return me_cb_inv_label(menu, me, msg);
  if ( gui_alarm_list[gui_data.active_alarm_idx].snooze_count == 0 )
    return me_cb_inv_label(menu, me, msg);
  return 0;    
  
}

const me_t melist_active_alarm_menu[] = 
{
  { me_cb_label, NULL, (void *)"Alarm",			2, 13},
  
  { me_cb_0_23_ro, &gui_data.h, NULL, 		ME_TIME_DXO+2+12,ME_TIME_Y },
  { me_cb_num_label, NULL, (void *)":", 			ME_TIME_DXO+30+20,ME_TIME_Y-3 },
  { me_cb_0_9_ro, &gui_data.mt, NULL, 		ME_TIME_DXO+39+19,ME_TIME_Y },
  { me_cb_0_9_ro, &gui_data.mo, NULL, 		ME_TIME_DXO+52+24,ME_TIME_Y },
  
  { me_cb_button_empty, (void *)me_action_deactivate_alarm, NULL, 0,0 },
  //{ me_cb_button_half_line, (void *)me_action_deactivate_alarm, (void *)"Alarm aus", 0,30 },
  //{ me_cb_button_half_line, (void *)me_action_do_snooze, (void *)"+5 Min ", 64,30 },

  { me_cb_inv_label, NULL, (void *)"Alarm aus", 4,40 },
  { me_cb_cond_inv_label, NULL, (void *)"+5 Min", 76,40 },

  { me_cb_null, NULL, NULL, 0, 0 },
};



/*============================================*/
/* toplevel menu */


int me_action_to_display_time(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_display_time, 0);
    return 1;
  }
  return 0;
}


int me_action_to_alarm_menu(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_alarm_menu, 0);
    return 1;
  }
  return 0;
}

int me_cb_button_skip_alarm(menu_t *menu, const me_t *me, uint8_t msg)
{  
  int r = 0;
  u8g2_uint_t x;
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return gui_data.is_skip_possible;
    case ME_MSG_DRAW_FOCUS:
      u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
      menu_DrawBoxFocus(menu, 
	  0, 
	  me->y - u8g2_GetAscent(menu->u8g2)-1, 
	  u8g2_GetDisplayWidth(menu->u8g2) , 
	  u8g2_GetAscent(menu->u8g2) - u8g2_GetDescent(menu->u8g2) +1);
      r = 1;
      break;
    case ME_MSG_DRAW:
      if ( gui_data.is_skip_possible )
      {
	u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
	gui_alarm_to_str(gui_data.next_alarm_index);
      	x = u8g2_DrawUTF8(menu->u8g2, me->x, me->y, gui_data.s );
      	u8g2_DrawUTF8(menu->u8g2, me->x+x, me->y, " deaktvieren" );
      }
      r = 1;
      break;
    case ME_MSG_SELECT:
      //printf("me_cb_button_skip_alarm ME_MSG_SELECT\n");
      gui_alarm_list[gui_data.next_alarm_index].skip_wd = 
	gui_alarm_list[gui_data.next_alarm_index].na_wd + 1;       
      gui_alarm_list[gui_data.next_alarm_index].snooze_count = 0;		/* clear snooze (if any) */
      menu_SetMEList(menu, melist_display_time, 0);		/* first set the normal menu */
      gui_Recalculate();							/* it might be changed here to the alarm menu */
      r = 1;
      break;
  }
  return r;
}


const me_t melist_top_menu[] = 
{
  { me_cb_button_full_line, (void *)me_action_to_display_time, (void *)"Zurück", 3,12 },
  { me_cb_button_full_line, (void *)me_action_to_alarm_menu, (void *)"Alarm", 3,24 },
  { me_cb_button_skip_alarm, NULL, NULL, 3,36 },
  { me_cb_button_full_line, (void *)me_action_to_setup_menu, (void *)"Weitere Funktionen", 3,48 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


/*===================================================*/


/*============================================*/

gui_alarm_t gui_alarm_list[GUI_ALARM_CNT];
char gui_alarm_str[GUI_ALARM_CNT][8];
uint32_t gui_backup_array[5];


gui_data_t gui_data;

menu_t gui_menu;

/*============================================*/
/* local functions */

uint32_t get_u32_by_alarm_data(gui_alarm_t *alarm);
void set_alarm_data_by_u32(gui_alarm_t *alarm, uint32_t u);
void gui_alarm_calc_next_wd_alarm(uint8_t idx, uint16_t current_week_time_in_minutes);
void gui_alarm_calc_str_time(uint8_t idx) U8G2_NOINLINE;
void gui_date_adjust(void) U8G2_NOINLINE;
void gui_calc_week_time(void);
void gui_calc_next_alarm(void);


/*============================================*/


uint32_t get_u32_by_alarm_data(gui_alarm_t *alarm)
{
  uint32_t u;
  int i;
  u = 0;
  for( i = 0; i < 7; i++ )
  {
    if ( alarm->wd[i] )
    {
      u |= 1<<i;
    }
  }
  u |= ((uint32_t)alarm->m&63) << (7);
  u |= ((uint32_t)alarm->h&31) << (7+6);
  u |= ((uint32_t)alarm->skip_wd&7) << (7+6+5);
  u |= ((uint32_t)alarm->enable&1) << (7+6+5+3);
  u |= ((uint32_t)alarm->snooze_count&1) << (7+6+5+3+1);
  return u;
}

void set_alarm_data_by_u32(gui_alarm_t *alarm, uint32_t u)
{
  int i;
  for( i = 0; i < 7; i++ )
  {
    if ( (u & (1<<i)) != 0 )
      alarm->wd[i] = 1;
    else
      alarm->wd[i] = 0;
  }
  u>>=7;
  alarm->m = u & 63;
  u>>=6;
  alarm->h = u & 31;
  u>>=5;
  alarm->skip_wd =  u & 7;
  u>>=3;
  alarm->enable =  u & 1;
  u>>=1;
  alarm->snooze_count =  u & 1;  
}


/*============================================*/

void gui_alarm_calc_next_wd_alarm(uint8_t idx, uint16_t current_week_time_in_minutes)
{
  uint8_t i;
  uint16_t week_time_abs;
  uint16_t week_time_diff;	/* difference to current_week_time_in_minutes */
  uint16_t best_diff = 0x0ffff;
  gui_alarm_list[idx].na_week_time_in_minutes = 0x0ffff;		/* not found */
  gui_alarm_list[idx].na_minutes_diff = 0x0ffff;				/* not found */
  gui_alarm_list[idx].na_wd = 7;						/* not found */
  
  //printf("gui_alarm_calc_next_wd_alarm: %d\n", idx);
  
  if ( gui_alarm_list[idx].enable != 0 )
  {
    //printf("gui_alarm_calc_next_wd_alarm: %d enabled\n", idx);
    for( i = 0; i < 7; i++ )
    {
      if ( gui_alarm_list[idx].wd[i] != 0 )
      {
	//printf("gui_alarm_calc_next_wd_alarm: %d i=%d gui_alarm_list[idx].skip_wd=%d \n", idx, i, gui_alarm_list[idx].skip_wd);
	if ( gui_alarm_list[idx].skip_wd != i+1 )
	{
	  week_time_abs = i; 
	  week_time_abs *= 24; 
	  week_time_abs += gui_alarm_list[idx].h;
	  week_time_abs *= 60;
	  week_time_abs += gui_alarm_list[idx].m;
	  week_time_abs += gui_alarm_list[idx].snooze_count*(uint16_t)SNOOZE_MINUTES;
	  
	  
	  if ( current_week_time_in_minutes <= week_time_abs )
	    week_time_diff = week_time_abs - current_week_time_in_minutes;
	  else
	    week_time_diff = week_time_abs + 7*24*60 - current_week_time_in_minutes;

	  //printf("gui_alarm_calc_next_wd_alarm: %d week_time_abs=%d current_week_time_in_minutes=%d week_time_diff=%d\n", idx, week_time_abs, current_week_time_in_minutes,week_time_diff);
	  
	  if (  best_diff > week_time_diff )
	  {
	    best_diff = week_time_diff;
	    /* found for this alarm */
	    gui_alarm_list[idx].na_minutes_diff = week_time_diff;
	    gui_alarm_list[idx].na_week_time_in_minutes = week_time_abs;
	    gui_alarm_list[idx].na_h = gui_alarm_list[idx].h;
	    gui_alarm_list[idx].na_m = gui_alarm_list[idx].m;
	    gui_alarm_list[idx].na_wd = i;
	  }
	}
      }
    }
  }
  //printf("gui_alarm_calc_next_wd_alarm: %d na_minutes_diff=%d\n", idx, gui_alarm_list[idx].na_minutes_diff);
  //printf("gui_alarm_calc_next_wd_alarm: %d na_wd=%d\n", idx, gui_alarm_list[idx].na_wd);
}


void gui_alarm_calc_str_time(uint8_t idx)
{
  gui_alarm_str[idx][0] = ' ';
  strcpy(gui_alarm_str[idx]+1, u8x8_u8toa(gui_alarm_list[idx].h, 2));
  strcpy(gui_alarm_str[idx]+4, u8x8_u8toa(gui_alarm_list[idx].m, 2));
  gui_alarm_str[idx][3] = ':';
  if ( gui_alarm_list[idx].enable == 0 )
  {
    gui_alarm_str[idx][0] = '(';
    gui_alarm_str[idx][6] = ')';
    gui_alarm_str[idx][7] = '\0';
  }    
}

/* adjust day/month and calculates the weekday */
/* this function must be called after reading from RTC or after setting the input vars by the user */
void gui_date_adjust(void)
{
    uint16_t ydn;
    uint16_t year;
    //uint16_t cdn;
    //uint32_t minutes_since_2000;
  
    if ( gui_data.month == 0 )
      gui_data.month++;
    if ( gui_data.day == 0 )
      gui_data.day++;
    year = 2000+gui_data.year_t*10 + gui_data.year_o;
    ydn = get_year_day_number(year, gui_data.month, gui_data.day);
    //cdn = to_century_day_number(year, ydn);
    //minutes_since_2000 = to_minutes(cdn, gui_data.h, gui_data.mt*10+gui_data.mo);
  
    // maybe adjust time by +/- 1h based on argument given to gui_date_adjust... but then it should be renamed also
  
    gui_data.month = get_month_by_year_day_number(year, ydn);
    gui_data.day = get_day_by_year_day_number(year, ydn);
    gui_data.weekday = get_weekday_by_year_day_number(year, ydn);	/* 0 = Sunday */
    /* adjust the weekday so that 0 will be Monday */
    gui_data.weekday += 6;
    if ( gui_data.weekday >= 7 ) 
      gui_data.weekday -= 7;
    
    if ( gui_data.day != gui_data.last_day )
    {
      gui_data.uptime++;
      gui_data.last_day = gui_data.day;
    }
    
    
}


/*
  calculate the minute within the week.
  this must be called after gui_date_adjust(), because the weekday is used here
*/
void gui_calc_week_time(void)
{
  gui_data.week_time = gui_data.weekday;
  gui_data.week_time  *= 24;
  gui_data.week_time += gui_data.h;
  gui_data.week_time  *= 60;
  gui_data.week_time  += gui_data.mt * 10 + gui_data.mo;
}



/*
  calculate the next alarm.
  this must be called after gui_calc_week_time() because, we need week_time
*/
void gui_calc_next_alarm(void)
{
  uint8_t i;
  uint8_t lowest_i;
  uint16_t lowest_diff;
  uint8_t redo;
  
  do
  {
    redo = 0;
    
    /* step 1: Calculate the difference to current weektime for each alarm */
    /* result is stored in gui_alarm_list[i].na_minutes_diff */
    for( i = 0; i < GUI_ALARM_CNT; i++ )
      gui_alarm_calc_next_wd_alarm(i, gui_data.week_time+(uint16_t)gui_data.is_equal);	/* is_equal flag is used as a offset */
    
    /* step 2: find the index with the lowest difference */
    lowest_diff = 0x0ffff;
    lowest_i = GUI_ALARM_CNT;
    for( i = 0; i < GUI_ALARM_CNT; i++ )
    {
      if ( lowest_diff > gui_alarm_list[i].na_minutes_diff )
      {
	lowest_diff = gui_alarm_list[i].na_minutes_diff;
	lowest_i = i;
      }
    }
    
    
    /* step 3: store the result */
    gui_data.next_alarm_index = lowest_i;  /* this can be GUI_ALARM_CNT */
    //printf("gui_calc_next_alarm gui_data.next_alarm_index=%d\n", gui_data.next_alarm_index);
    
    /* calculate the is_skip_possible and the is_alarm flag */
    gui_data.is_skip_possible = 0;
    if ( lowest_i < GUI_ALARM_CNT )
    {
      
      if ( gui_alarm_list[lowest_i].na_minutes_diff == 0 )
      {
	if ( gui_data.is_equal == 0 )
	{
	  gui_data.is_alarm = 1;
	  gui_data.is_equal = 1;
	  gui_data.active_alarm_idx = lowest_i;
	  //gui_data.active_equal_idx = lowest_i;
	  
	  gui_data.equal_h = gui_data.h;
	  gui_data.equal_mt = gui_data.mt;
	  gui_data.equal_mo = gui_data.mo;
	  
	  redo = 1;
	}
      }
      else
      {
	
	/* valid next alarm time */
	if ( gui_alarm_list[lowest_i].skip_wd == 0 )
	{
	  /* skip flag not yet set */
	  if ( gui_alarm_list[lowest_i].na_minutes_diff <= (uint16_t)60*(uint16_t)ALLOW_SKIP_HOURS )
	  {
	    /* within the limit before alarm */
	    gui_data.is_skip_possible = 1;
	  }
	}
      }
    }
  } while( redo );
  
  /* reset the equal flag */
  if ( gui_data.is_equal != 0 )
  {
    if ( gui_data.equal_h != gui_data.h ||
	  gui_data.equal_mt != gui_data.mt ||
	  gui_data.equal_mo != gui_data.mo )
    {
      gui_data.is_equal = 0;
    }
  }
}


/*============================================*/

void gui_LoadData(void)
{
  uint32_t data[5];
  int i;
  
  //printf("Load Data\n");
  
  load_gui_data(data);
  for( i = 0; i < GUI_ALARM_CNT; i++ )
  {
    set_alarm_data_by_u32(gui_alarm_list+i, data[i]);
  }
  gui_data.uptime = data[4] & (uint32_t)0x03ff;
  gui_data.last_day =  (data[4]>>10) & (uint32_t)31;
  gui_data.contrast = (data[4]>>15) & (uint32_t)7;
  gui_data.display_voltage = (data[4]>>16) & (uint32_t)1;
}

void gui_StoreData(void)
{
  uint32_t data[5];
  int i;
  for( i = 0; i < GUI_ALARM_CNT; i++ )
  {
    data[i] = get_u32_by_alarm_data(gui_alarm_list+i);
    //printf("%d: %08lx\n", i, data[i]);
  }
  data[4] = 0;
  data[4] |= gui_data.uptime & (uint32_t)0x03ff;	/* 0...1023 */
  data[4] |= (gui_data.last_day & (uint32_t)31)<<10;
  data[4] |= (gui_data.contrast & (uint32_t)7)<<15;
  data[4] |= (gui_data.display_voltage & (uint32_t)1)<<16;
  
  store_gui_data(data);
}



/* recalculate all internal data */
void gui_Recalculate(void)
{
  int i;
  
  gui_date_adjust();
  gui_calc_week_time();
  gui_calc_next_alarm();
  for ( i = 0; i < GUI_ALARM_CNT; i++ )
  {
    gui_alarm_calc_str_time(i);
  }
  gui_StoreData();
}

/* minute and/or hour has changed */
/* additionally the active alarm menu might be set by this function */
void gui_SignalTimeChange(void)
{
  /* recalculate dependent values */
  gui_Recalculate();
  
  /* setup menu */
  menu_SetMEList(&gui_menu, melist_display_time, 0);
  
  /* enable alarm */
  if ( gui_data.is_alarm != 0 )
  {
    menu_SetMEList(&gui_menu, melist_active_alarm_menu, 0);
    enable_alarm();
  }
}

void gui_Init(u8g2_t *u8g2, uint8_t is_por)
{
  if ( is_por == 0 )
  {
    /* not a POR reset, so load current values */
    gui_LoadData();
    /* do NOT init the display, otherwise there will be some flicker visible */
    /* however, the GPIO subsystem still has to be setup, so call the other init procedures */
    /* this acts like a warm start for the display */
    /* the display setup code for the display is NOT send */
    u8x8_gpio_Init(u8g2_GetU8x8(u8g2));
    u8x8_cad_Init(u8g2_GetU8x8(u8g2));
    u8x8_gpio_SetReset(u8g2_GetU8x8(u8g2), 1);

    //u8g2_InitDisplay(u8g2);
    //u8x8_d_helper_display_init(u8g2_GetU8x8(u8g2));

    // u8g2_SetPowerSave(u8g2, 0);   // this will be done later
  }
  else
  {
    /* POR reset, so do NOT load any values (they will be 0 in the best case) */
    /* instead do a proper reset of the display */
    // u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8g2_InitDisplay(u8g2);
    
    // u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
    u8g2_SetPowerSave(u8g2, 0);
  }
  
  menu_Init(&gui_menu, u8g2);
  
  gui_SignalTimeChange();
}


void gui_Draw(void)
{
    menu_Draw(&gui_menu);
}

void gui_Next(void)
{
  if ( gui_menu.me_list == melist_active_alarm_menu )
  {
    disable_alarm();
    if ( gui_alarm_list[gui_data.active_alarm_idx].snooze_count != 0 )
    {
      int i;
      /* this is already the snooze alarm, so clear all and behave like the normal alarm off */
      for( i = 0; i < GUI_ALARM_CNT; i++ )
	gui_alarm_list[i].snooze_count = 0;
    }
    else
    {
      /* enable snooze */
      gui_alarm_list[gui_data.active_alarm_idx].snooze_count = 1;
    }
    gui_data.is_alarm = 0;
    gui_Recalculate();
    menu_SetMEList(&gui_menu, melist_display_time, 0);
  }
  else
  {
    menu_NextFocus(&gui_menu);
  }
}

void gui_Select(void)
{
  if ( gui_menu.me_list == melist_active_alarm_menu )
  {
    int i;
    disable_alarm();
    for( i = 0; i < GUI_ALARM_CNT; i++ )
      gui_alarm_list[i].snooze_count = 0;
    gui_data.is_alarm = 0;
    gui_Recalculate();
    menu_SetMEList(&gui_menu, melist_display_time, 0);
  }
  else
  {
    menu_Select(&gui_menu);
  }
}



/*===================================================*/

void read_rtc(void)
{
  gui_data.h = rtc.getHours();
  gui_data.mt = rtc.getMinutes()/10;
  gui_data.mo = rtc.getMinutes()%10;
  gui_data.st = rtc.getSeconds()/10;
  gui_data.so = rtc.getSeconds()%10;

  gui_data.day = rtc.getDay();
  gui_data.month = rtc.getMonth();
  gui_data.year_o = rtc.getYear()/10;
  gui_data.year_t = rtc.getYear()%10;

  //gui_date_adjust();	/* calculate weekday */
  gui_Recalculate();
  
}


void setup(void) {

  rtc.begin();

  // U8g2 SH1106 Proto-Shield
  //u8g2.begin(/* menu_select_pin= */ 2, /* menu_next_pin= */ 4, /* menu_prev_pin= */ 7, /* menu_up_pin= */ 6, /* menu_down_pin= */ 5, /* menu_home_pin= */ 3);

  // DOGS102 Shield (http://shieldlist.org/controlconnection/dogs102)
  // u8g2.begin(/* menu_select_pin= */ 5, /* menu_next_pin= */ 4, /* menu_prev_pin= */ 2, /* menu_up_pin= */ U8X8_PIN_NONE, /* menu_down_pin= */ U8X8_PIN_NONE, /* menu_home_pin= */ 3);
  
  // DOGM128 Shield (http://shieldlist.org/schmelle2/dogm128) + DOGXL160 Shield
  //u8g2.begin(/* menu_select_pin= */ 2, /* menu_next_pin= */ 3, /* menu_prev_pin= */ 7, /* menu_up_pin= */ U8X8_PIN_NONE, /* menu_down_pin= */ U8X8_PIN_NONE, /* menu_home_pin= */ 8);
  
  // Arduboy
  //u8g2.begin(/*Select=*/ A0, /*Right/Next=*/ 5, /*Left/Prev=*/ 9, /*Up=*/ 8, /*Down=*/ 10, /*Home/Cancel=*/ A1); // Arduboy DevKit
  //u8g2.begin(/*Select=*/ 7, /*Right/Next=*/ A1, /*Left/Prev=*/ A2, /*Up=*/ A0, /*Down=*/ A3, /*Home/Cancel=*/ 8); // Arduboy 10 (Production)

  u8g2.begin(/* menu_select_pin= */ 6, /* menu_next_pin= */ 7, /* menu_prev_pin= */ U8X8_PIN_NONE, /* menu_up_pin= */ U8X8_PIN_NONE, /* menu_down_pin= */ U8X8_PIN_NONE, /* menu_home_pin= */ U8X8_PIN_NONE);
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.setFlipMode(1);
  
  u8g2_ptr = u8g2.getU8g2();

  gui_Init(u8g2_ptr, 0);
  
}



void loop(void) {
  uint8_t event;
  unsigned long m;
  
  for(;;)
  {
    read_rtc();
    u8g2.firstPage();
    do
    {
      gui_Draw();
    } while(u8g2.nextPage()); 
    
    m = millis() + 400UL;
    for(;;)
    {
      event = u8g2.getMenuEvent();
      if ( event == U8X8_MSG_GPIO_MENU_SELECT )
      {
	gui_Select();
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_NEXT )
      {
	gui_Next();
	break;
      }
      if ( millis() > m )
        break;
      
    }
  }
}



