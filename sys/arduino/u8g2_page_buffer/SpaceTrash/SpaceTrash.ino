/*

  SpaceTrash.ino

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

*/

#include <Arduino.h>
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
//U8G2_SSD1322_NHD_256X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1362_256X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1362_256X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Enable U8G2_16BIT in u8g2.h
//U8G2_SSD1362_206X36_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1362_206X36_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
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
//U8G2_LD7032_60X32_1_4W_SW_I2C u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* reset=*/ U8X8_PIN_NONE);	// NOT TESTED!
//U8G2_LD7032_60X32_ALT_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 9, /* dc=*/ 10, /* reset=*/ 8);	// SW SPI Nano Board
//U8G2_LD7032_60X32_ALT_1_4W_SW_I2C u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* reset=*/ U8X8_PIN_NONE);	// NOT TESTED!
//U8G2_UC1701_EA_DOGS102_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_UC1701_EA_DOGS102_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Nokia 5110 Display
//U8G2_PCD8544_84X48_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);						// Nokia 5110 Display
//U8G2_PCF8812_96X65_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// Could be also PCF8814
//U8G2_PCF8812_96X65_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);						// Could be also PCF8814
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
//U8G2_ST7571_128X128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7571_128X128_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7571_128X96_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7571_128X96_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7586S_JLX384160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7586S_ERC240160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7586S_YMC240160_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75160_JM16096_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST75160_JM16096_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
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
//U8G2_SED1330_240X128_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect /RD = E with +5V, enable is /WR = RW, FG with GND, 14=Uno Pin A0
//U8G2_SED1330_240X128_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // A0 is dc pin, /WR = RW = GND, enable is /RD = E
//U8G2_SED1330_256X128_1_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect /RD = E with +5V, enable is /WR = RW, FG with GND, 14=Uno Pin A0, enable U8g2 16Bit Mode 
//U8G2_SED1330_256X128_1_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // A0 is dc pin, /WR = RW = GND, enable is /RD = E, enable U8g2 16Bit Mode 
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
//U8G2_S1D15721_240X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_S1D15721_240X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_GU800_128X64_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_GU800_128X64_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_GU800_160X16_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_GU800_160X16_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);




// End of constructor list



/* button setup for Arduboy Production */
uint8_t pin_up = A0;
uint8_t pin_down = A3;
uint8_t pin_fire = 7;




#define ST_FP 4

/* object types */
struct _st_ot_struct
{
  /* 
    missle and hit: 
      bit 0: player missle and trash
      bit 1: trash, which might hit the player
  */
  
  uint8_t missle_mask;		/* this object is a missle: it might destroy something if the target is_hit_fn says so */ 
  uint8_t hit_mask;			/* if missle_mask & hit_mask is != 0  then the object can be destroyed */
  uint8_t points;
  uint8_t draw_fn;
  uint8_t move_fn;
      /* ST_MOVE_FN_NONE, ST_MOVE_FN_X_SLOW */
  uint8_t destroy_fn;		/* object can be destroyed by a missle (e.g. a missle from the space ship) */
      /* ST_DESTROY_FN_NONE, ST_DESTROY_FN_SPLIT */
  uint8_t is_hit_fn;		/* is hit procedure */
      /* ST_IS_HIT_FN_NONE, ST_IS_HIT_BBOX */
  uint8_t fire_fn;
    /* ST_FIRE_FN_NONE, ST_FIRE_FN_X_LEFT */
  
};
typedef struct _st_ot_struct st_ot;

/*
  objects, which are visible at the play area
*/
struct _st_obj_struct
{
  uint8_t ot; 			/* object type: zero means, object is not used */
  int8_t tmp;			/* generic value, used by ST_MOVE_IMPLODE */
  /* absolute position */
  /* LCD pixel position is x>>ST_FP and y>>ST_FP */
  int16_t x, y;
  int8_t x0,y0,x1,y1;	/* object outline in pixel, reference point is at 0,0 */
};
typedef struct _st_obj_struct st_obj;

#define ST_DRAW_NONE 0
#define ST_DRAW_BBOX 1
#define ST_DRAW_TRASH1 2
#define ST_DRAW_PLAYER1 3
#define ST_DRAW_TRASH2 4
#define ST_DRAW_PLAYER2 5
#define ST_DRAW_PLAYER3 6
#define ST_DRAW_GADGET 7
#define ST_DRAW_BACKSLASH 8
#define ST_DRAW_SLASH 9
#define ST_DRAW_BIG_TRASH 10

#define ST_MOVE_NONE 0
#define ST_MOVE_X_SLOW 1
#define ST_MOVE_PX_NORMAL 2
#define ST_MOVE_PX_FAST 3
#define ST_MOVE_PLAYER 4
#define ST_MOVE_PY 5
#define ST_MOVE_NY 6
#define ST_MOVE_IMPLODE 7
#define ST_MOVE_X_FAST 8
#define ST_MOVE_WALL 9
#define ST_MOVE_NXPY 10
#define ST_MOVE_NXNY 11

#define ST_IS_HIT_NONE 0
#define ST_IS_HIT_BBOX 1
#define ST_IS_HIT_WALL 2

#define ST_DESTROY_NONE 0
#define ST_DESTROY_DISAPPEAR 1
#define ST_DESTROY_TO_DUST 2
#define ST_DESTROY_GADGET 3
#define ST_DESTROY_PLAYER 4
#define ST_DESTROY_PLAYER_GADGETS 5
#define ST_DESTROY_BIG_TRASH 6

#define ST_FIRE_NONE 0
#define ST_FIRE_PLAYER1 1
#define ST_FIRE_PLAYER2 2
#define ST_FIRE_PLAYER3 3

#define ST_OT_WALL_SOLID 1
#define ST_OT_BIG_TRASH 2
#define ST_OT_MISSLE 3
#define ST_OT_TRASH1 4
#define ST_OT_PLAYER 5
#define ST_OT_DUST_PY 6
#define ST_OT_DUST_NY 7
#define ST_OT_TRASH_IMPLODE 8
#define ST_OT_TRASH2 9
#define ST_OT_PLAYER2 10
#define ST_OT_PLAYER3 11
#define ST_OT_GADGET 12
#define ST_OT_GADGET_IMPLODE 13
#define ST_OT_DUST_NXPY 14
#define ST_OT_DUST_NXNY 15


/*================================================================*/
/* graphics object */
/*================================================================*/

u8g2_t *st_u8g2;

u8g2_uint_t u8g_height_minus_one;


#define ST_AREA_HEIGHT (st_u8g2->height - 8)
#define ST_AREA_WIDTH (st_u8g2->width)


/*================================================================*/
/* object types */
/*================================================================*/


const st_ot st_object_types[] U8X8_PROGMEM =
{
    /* 0: empty object type */
    { 0, 0,  0, ST_DRAW_NONE, ST_MOVE_NONE, ST_DESTROY_DISAPPEAR, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* 1: wall, player will be destroyed */
    { 2, 1, 30, ST_DRAW_BBOX, ST_MOVE_WALL, ST_DESTROY_DISAPPEAR, ST_IS_HIT_WALL, ST_FIRE_NONE },
    /* ST_OT_BIG_TRASH (2) */
    { 2, 1,  0, ST_DRAW_BIG_TRASH, ST_MOVE_X_SLOW, ST_DESTROY_BIG_TRASH, ST_IS_HIT_BBOX, ST_FIRE_NONE },
    /* 3: simple space ship (player) missle */
    { 1, 0,  0, ST_DRAW_BBOX, ST_MOVE_PX_FAST, ST_DESTROY_DISAPPEAR, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_TRASH1 (4): trash */
    { 2, 1,  0, ST_DRAW_TRASH1, ST_MOVE_X_SLOW, ST_DESTROY_TO_DUST, ST_IS_HIT_BBOX, ST_FIRE_NONE },
    /* ST_OT_PLAYER (5): player space ship */
    { 0, 2,  0, ST_DRAW_PLAYER1, ST_MOVE_PLAYER, ST_DESTROY_PLAYER, ST_IS_HIT_BBOX, ST_FIRE_PLAYER1 },
    /* ST_OT_DUST_PY (6): Last part of trash  */
    { 0, 0,  0, ST_DRAW_BBOX, ST_MOVE_PY, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_DUST_NY (7): Last part of trash  */
    { 0, 0,  0, ST_DRAW_BBOX, ST_MOVE_NY, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_TRASH_IMPLODE (8): trash was hit */
    { 0, 0,  5, ST_DRAW_TRASH1, ST_MOVE_IMPLODE, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_TRASH2 (9): trash */
    { 2, 1,  0, ST_DRAW_TRASH2, ST_MOVE_X_SLOW, ST_DESTROY_TO_DUST, ST_IS_HIT_BBOX, ST_FIRE_NONE },
    /* ST_OT_PLAYER2 (10): player space ship+1x enhancement */
    { 0, 2,  0, ST_DRAW_PLAYER2, ST_MOVE_PLAYER, ST_DESTROY_PLAYER_GADGETS, ST_IS_HIT_BBOX, ST_FIRE_PLAYER2 },
    /* ST_OT_PLAYER3 (11): player space ship+2x enhancement */
    { 0, 2,  0, ST_DRAW_PLAYER3, ST_MOVE_PLAYER, ST_DESTROY_PLAYER_GADGETS, ST_IS_HIT_BBOX, ST_FIRE_PLAYER3 },
    /* ST_OT_GADGET (12): adds enhancements */
    { 0, 1,  0, ST_DRAW_GADGET, ST_MOVE_X_FAST, ST_DESTROY_GADGET, ST_IS_HIT_BBOX, ST_FIRE_NONE },
    /* ST_OT_GADGET_IMPLODE (13) */
    { 0, 0, 20, ST_DRAW_GADGET, ST_MOVE_IMPLODE, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_DUST_NXPY (14): Last part of trash  */
    { 0, 0,  0, ST_DRAW_BACKSLASH, ST_MOVE_NXPY, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    /* ST_OT_DUST_NXNY (15): Last part of trash  */
    { 0, 0,  0, ST_DRAW_SLASH, ST_MOVE_NXNY, ST_DESTROY_NONE, ST_IS_HIT_NONE, ST_FIRE_NONE },
    
};

/*================================================================*/
/* list of all objects on the screen */
/*================================================================*/

/* use AVR RAMEND constant to derive the number of allowed objects */

#if RAMEND < 0x300 
#define ST_OBJ_CNT 25
#else
//#define ST_OBJ_CNT 45
#define ST_OBJ_CNT 60
#endif 

st_obj st_objects[ST_OBJ_CNT];

/*================================================================*/
/* about players space ship*/
/*================================================================*/

/* player position */
uint8_t st_player_pos;

/* points */
#define ST_POINTS_PER_LEVEL 25
uint16_t st_player_points;
uint16_t st_player_points_delayed;
uint16_t st_highscore = 0;

/*================================================================*/
/* overall game state */
/*================================================================*/

#define ST_STATE_PREPARE 0
#define ST_STATE_IPREPARE 1
#define ST_STATE_GAME 2
#define ST_STATE_END 3
#define ST_STATE_IEND 4

uint8_t st_state = ST_STATE_PREPARE;

/*================================================================*/
/* game difficulty */
/*================================================================*/
uint8_t st_difficulty = 1;
#define ST_DIFF_VIS_LEN 30
#define ST_DIFF_FP 5
uint16_t st_to_diff_cnt = 0;

/*================================================================*/
/* bitmaps */
/*================================================================*/

const uint8_t st_bitmap_player1[]  = 
{ 
  /* 01100000 */ 0x060,
  /* 11111000 */ 0x0f8,
  /* 01111110 */ 0x07e,
  /* 11111000 */ 0x0f8,
  /* 01100000 */ 0x060
};

const uint8_t st_bitmap_player2[] = 
{   
  /* 01100000 */ 0x060,
  /* 01111100 */ 0x078,
  /* 01100000 */ 0x060,
  /* 11100000 */ 0x0e0,
  /* 11111000 */ 0x0f8,
  /* 01111110 */ 0x07e,
  /* 11111000 */ 0x0f8,
  /* 01100000 */ 0x060
};

const uint8_t st_bitmap_player3[] = 
{   
  /* 01100000 */ 0x060,
  /* 01111100 */ 0x078,
  /* 01100000 */ 0x060,
  /* 11100000 */ 0x0e0,
  /* 11111000 */ 0x0f8,
  /* 01111110 */ 0x07e,
  /* 11111000 */ 0x0f8,
  /* 11100000 */ 0x0e0,
  /* 01100000 */ 0x060,
  /* 01111100 */ 0x078,
  /* 01100000 */ 0x060
 };

const uint8_t st_bitmap_trash_5x5_1[] = 
{ 
  /* 01110000 */ 0x070,
  /* 11110000 */ 0x0f0,
  /* 11111000 */ 0x0f8,
  /* 01111000 */ 0x078,
  /* 00110000 */ 0x030,
};

const uint8_t st_bitmap_trash_5x5_2[] = 
{ 
  /* 00110000 */ 0x030,
  /* 11111000 */ 0x0f8,
  /* 11111000 */ 0x0f8,
  /* 11110000 */ 0x0f0,
  /* 01110000 */ 0x070,
};

const uint8_t st_bitmap_trash_7x7[] = 
{
  /* 00111000 */ 0x038,
  /* 01111100 */ 0x07c,
  /* 11111100 */ 0x0fc,
  /* 11111110 */ 0x0fe,
  /* 11111110 */ 0x0fe,
  /* 01111110 */ 0x07e,
  /* 01111000 */ 0x078,
};

const uint8_t st_bitmap_gadget[] = 
{ 
  /* 01110000 */ 0x070,
  /* 11011000 */ 0x0d8,
  /* 10001000 */ 0x088,
  /* 11011000 */ 0x0d8,
  /* 01110000 */ 0x070,
};

/*================================================================*/
/* forward definitions */
/*================================================================*/
uint8_t st_rnd(void) U8X8_NOINLINE;
static st_obj *st_GetObj(uint8_t objnr) U8X8_NOINLINE;
uint8_t st_GetMissleMask(uint8_t objnr);
uint8_t st_GetHitMask(uint8_t objnr);
int8_t st_FindObj(uint8_t ot) U8X8_NOINLINE;
void st_ClrObjs(void) U8X8_NOINLINE;
int8_t st_NewObj(void) U8X8_NOINLINE;
uint8_t st_CntObj(uint8_t ot);
uint8_t st_CalcXY(st_obj *o) U8X8_NOINLINE;
void st_SetXY(st_obj *o, uint8_t x, uint8_t y) U8X8_NOINLINE;

void st_FireStep(uint8_t is_auto_fire, uint8_t is_fire) U8X8_NOINLINE;

void st_InitTrash(uint8_t x, uint8_t y, int8_t dir);
void st_NewGadget(uint8_t x, uint8_t y);
void st_NewPlayerMissle(uint8_t x, uint8_t y) ;
void st_NewTrashDust(uint8_t x, uint8_t y, int ot);
void st_NewTrashDustAreaArgs(int16_t x, int16_t y, int ot); 
void st_SetupPlayer(uint8_t objnr, uint8_t ot);


/*================================================================*/
/* utility functions */
/*================================================================*/

char st_itoa_buf[12];
char *st_itoa(unsigned long v)
{
  volatile unsigned char i = 11;
  st_itoa_buf[11] = '\0';
  while( i > 0)
  {
      i--;
      st_itoa_buf[i] = (v % 10)+'0';
      v /= 10;
      if ( v == 0 )
	break;
  }
  return st_itoa_buf+i;
}


uint8_t st_rnd(void)
{
  return rand();
}

/*	
  for the specified index number, return the object
*/
static st_obj *st_GetObj(uint8_t objnr)
{
  return st_objects+objnr;
}


/*
  check, if this is a missle-like object (that is, can this object destroy something else)
*/ 
uint8_t st_GetMissleMask(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  return u8x8_pgm_read(&(st_object_types[o->ot].missle_mask));
}

/*
  check, if this is a missle-like object (that is, can this object destroy something else)
*/ 
uint8_t st_GetHitMask(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  return u8x8_pgm_read(&(st_object_types[o->ot].hit_mask));
}

/*
  search an empty object
*/
int8_t st_FindObj(uint8_t ot)
{
  int8_t i;
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    if ( st_objects[i].ot == ot )
      return i;
  }
  return -1;
}

/*
  delete all objects
*/

void st_ClrObjs(void)
{
  int8_t i;
  for( i = 0; i < ST_OBJ_CNT; i++ )
    st_objects[i].ot = 0;
}

/*
  search an empty object
*/
int8_t st_NewObj(void)
{
  int8_t i;
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    if ( st_objects[i].ot == 0 )
      return i;
  }
  return -1;
}

/*
  count number of objectes of the provided type
  st_CntObj(0) will return the number of empty objects, that means if
  st_CntObj(0) > 0 then st_NewObj() will return  a valid index
*/
uint8_t st_CntObj(uint8_t ot)
{
  uint8_t i;
  uint8_t cnt = 0;
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    if ( st_objects[i].ot == ot )
      cnt++;
  }
  return cnt;
}

/*
  calculate the pixel coordinates of the reference point of an object
  return rhe x value
*/
uint8_t st_px_x, st_px_y; /* pixel within area */
uint8_t st_CalcXY(st_obj *o)
{
  //st_obj *o = st_GetObj(objnr);
  st_px_y = o->y>>ST_FP;
  st_px_x = o->x>>ST_FP;
  return st_px_x;
}

void st_SetXY(st_obj *o, uint8_t x, uint8_t y)
{
  o->x = ((int16_t)x) << ST_FP;
  o->y = ((int16_t)y) << ST_FP;
}

/*
  calculate the object bounding box and place it into some global variables
*/
int16_t st_bbox_x0, st_bbox_y0, st_bbox_x1, st_bbox_y1;  

void st_CalcBBOX(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  
  st_bbox_x0 = (uint16_t)(o->x>>ST_FP);
  st_bbox_x1 = st_bbox_x0;
  st_bbox_x0 += o->x0;
  st_bbox_x1 += o->x1;

  st_bbox_y0 = (uint16_t)(o->y>>ST_FP);
  st_bbox_y1 = st_bbox_y0;
  st_bbox_y0 += o->y0;
  st_bbox_y1 += o->y1;
}

/*
  clip bbox with the view window. requires a call to st_CalcBBOX
  return 0, if the bbox is totally outside the window
*/
uint8_t st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1;
uint8_t st_ClipBBOX(void)
{
  if ( st_bbox_x0 >= ST_AREA_WIDTH )
    return 0;
  if ( st_bbox_x0 >= 0 )
    st_cbbox_x0  = (uint16_t)st_bbox_x0;
  else
    st_cbbox_x0 = 0;

  if ( st_bbox_x1 < 0 )
    return 0;
  if ( st_bbox_x1 < ST_AREA_WIDTH )
    st_cbbox_x1  = (uint16_t)st_bbox_x1;
  else
    st_cbbox_x1 = ST_AREA_WIDTH-1;

  if ( st_bbox_y0 >= ST_AREA_HEIGHT )
    return 0;
  if ( st_bbox_y0 >= 0 )
    st_cbbox_y0  = (uint16_t)st_bbox_y0;
  else
    st_cbbox_y0 = 0;

  if ( st_bbox_y1 < 0 )
    return 0;
  if ( st_bbox_y1 < ST_AREA_HEIGHT )
    st_cbbox_y1  = (uint16_t)st_bbox_y1;
  else
    st_cbbox_y1 = ST_AREA_HEIGHT-1;
  
  return 1;
}


/*================================================================*/
/* universal member functions */
/*================================================================*/


uint8_t st_IsOut(uint8_t objnr)
{
  st_CalcBBOX(objnr);
  if ( st_bbox_x0 >= ST_AREA_WIDTH )
    return 1;
  if ( st_bbox_x1 < 0 )
    return 1;
  if ( st_bbox_y0 >= ST_AREA_HEIGHT )
    return 1;
  if ( st_bbox_y1 < 0 )
    return 1;
  return 0;
}

void st_Disappear(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  st_player_points += u8x8_pgm_read(&(st_object_types[o->ot].points));
  o->ot = 0;
}

/*================================================================*/
/* type dependent member functions */
/*================================================================*/

void st_Move(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  switch(u8x8_pgm_read(&(st_object_types[o->ot].move_fn)))
  {
    case ST_MOVE_NONE:
      break;
    case ST_MOVE_X_SLOW:
      o->x -= (1<<ST_FP)/8;
      o->x -= st_difficulty;
      o->y += (int16_t)o->tmp;
      if ( o->y >= ((ST_AREA_HEIGHT-1) << ST_FP) || o->y <= 0 )
	o->tmp = - o->tmp;
      break;
    case ST_MOVE_X_FAST:
      o->x -= (1<<ST_FP)/2;
      o->y += (int16_t)o->tmp;
      if ( o->y >= ((ST_AREA_HEIGHT-1) << ST_FP) || o->y <= 0 )
	o->tmp = - o->tmp;
      break;
    case ST_MOVE_PX_NORMAL:
      o->x += (1<<ST_FP)/4;
      break;
    case ST_MOVE_PX_FAST:
      o->x += (1<<ST_FP);
      break;
    case ST_MOVE_PLAYER:
      o->y = st_player_pos<<ST_FP;
      break;
    case ST_MOVE_PY:
      o->y += 3*ST_FP;
      break;
    case ST_MOVE_NY:
      o->y -= 3*ST_FP;
      break;
    case ST_MOVE_NXPY:
      o->y += 3*ST_FP;
      o->x -= 3*ST_FP;
      break;
    case ST_MOVE_NXNY:
      o->y -= 3*ST_FP;
      o->x -= 3*ST_FP;
      break;
    case ST_MOVE_IMPLODE:
      o->tmp++;
      if ( (o->tmp & 0x03) == 0 )
      {
	if ( o->x0 != o->x1 )
	  o->x0++;
	else
	    st_Disappear(objnr);
      }
      break;
    case ST_MOVE_WALL:
      o->x -= 1;
      o->x -= (st_difficulty>>1);
      break;
  }
}

void st_DrawBBOX(uint8_t objnr)
{
  uint8_t y0, y1;
  /*st_obj *o = st_GetObj(objnr);*/
  st_CalcBBOX(objnr);
  if ( st_ClipBBOX() == 0 )
    return;
  /* st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1; */

  
  // w = st_cbbox_x1-st_cbbox_x0;
  // w++;
  // h = st_cbbox_y1-st_cbbox_y0;
  // h++;
  
  
  //dog_SetVLine(st_cbbox_x0, st_cbbox_y0, st_cbbox_y1);
  //dog_SetVLine(st_cbbox_x1, st_cbbox_y0, st_cbbox_y1);
  //dog_SetHLine(st_cbbox_x0, st_cbbox_x1, st_cbbox_y0);
  //dog_SetHLine(st_cbbox_x0, st_cbbox_x1, st_cbbox_y1);

  u8g2_SetDrawColor(st_u8g2, 1);
  y0 = u8g_height_minus_one - st_cbbox_y0;
  y1 = u8g_height_minus_one - st_cbbox_y1;
  
  u8g2_DrawFrame(st_u8g2, st_cbbox_x0, y1, st_cbbox_x1-st_cbbox_x0+1, y0-y1+1);
  
  //dog_SetBox(st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1);

  /*
  if ( o->ot == ST_OT_PLAYER )
  {
    dog_DrawStr(0, 26, font_4x6, st_itoa(st_cbbox_y0));
    dog_DrawStr(10, 26, font_4x6, st_itoa(st_cbbox_y1));
  }
  */
}

#ifdef FN_IS_NOT_IN_USE
void st_DrawFilledBox(uint8_t objnr)
{
  st_CalcBBOX(objnr);
  if ( st_ClipBBOX() == 0 )
    return;
  /* st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1; */
  dog_SetBox(st_cbbox_x0, st_cbbox_y0, st_cbbox_x1, st_cbbox_y1);
}
#endif

void st_DrawBitmap(uint8_t objnr, const uint8_t * bm, uint8_t w, uint8_t h)
{
  /* st_obj *o = st_GetObj(objnr); */
  st_CalcBBOX(objnr);
  /* result is here: int16_t st_bbox_x0, st_bbox_y0, st_bbox_x1, st_bbox_y1 */
  //dog_SetBitmapP(st_bbox_x0,st_bbox_y1,bm,w,h);
  
  u8g2_DrawBitmap(st_u8g2, st_bbox_x0, u8g_height_minus_one - st_bbox_y1, (w+7)/8, h, bm);
  
 }

void st_DrawObj(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  switch(u8x8_pgm_read(&(st_object_types[o->ot].draw_fn)))
  {
    case ST_DRAW_NONE:
      break;
    case ST_DRAW_BBOX:
      st_DrawBBOX(objnr);
      break;
    case ST_DRAW_TRASH1:
      st_DrawBitmap(objnr, st_bitmap_trash_5x5_1,o->x1-o->x0+1, 5);
      break;
    case ST_DRAW_TRASH2:
      st_DrawBitmap(objnr, st_bitmap_trash_5x5_2,o->x1-o->x0+1, 5);
      break;
    case ST_DRAW_BIG_TRASH:
      st_DrawBitmap(objnr, st_bitmap_trash_7x7,o->x1-o->x0+1, 7);
      break;
    case ST_DRAW_PLAYER1:
      st_DrawBitmap(objnr, st_bitmap_player1,7,5);
      break;
    case ST_DRAW_PLAYER2:
      st_DrawBitmap(objnr, st_bitmap_player2,7,8);
      break;
    case ST_DRAW_PLAYER3:
      st_DrawBitmap(objnr, st_bitmap_player3,7,11);
      break;
    case ST_DRAW_GADGET:
      /* could use this proc, but... */
      /* st_DrawBitmap(objnr, st_bitmap_gadget,o->x1-o->x0+1, 5); */
      /* ... this one looks also funny. */
      st_DrawBitmap(objnr, st_bitmap_gadget,5,5);
      break;
    case ST_DRAW_BACKSLASH:
      {
	uint8_t x;
	uint8_t y;
	x = st_CalcXY(o);
	y = st_px_y;

	
	// dog_SetPixel(x,y);
	// x++; y--;
	// dog_SetPixel(x,y);
	// x++; y--;
 	// dog_SetPixel(x,y);
	
	u8g2_SetDrawColor(st_u8g2, 1);  
	u8g2_DrawPixel(st_u8g2, x, u8g_height_minus_one - y);  
	x++; y--;
	u8g2_DrawPixel(st_u8g2, x, u8g_height_minus_one - y);  
	x++; y--;
	u8g2_DrawPixel(st_u8g2, x, u8g_height_minus_one - y);  	
     }      
     break;
    case ST_DRAW_SLASH:
      {
	uint8_t x;
	uint8_t y;
	x = st_CalcXY(o);
	y = st_px_y;
	
	// dog_SetPixel(x,y);
	// x++; y++;
	// dog_SetPixel(x,y);
	// x++; y++;
 	// dog_SetPixel(x,y);
	
	u8g2_SetDrawColor(st_u8g2, 1);  
	u8g2_DrawPixel(st_u8g2, x, u8g_height_minus_one - y);  
	x++; y++;
	u8g2_DrawPixel(st_u8g2, x, u8g_height_minus_one - y);  
	x++; y++;
	u8g2_DrawPixel(st_u8g2, x, u8g_height_minus_one - y);  	
     }      
     break;
  }
}

uint8_t st_IsHitBBOX(uint8_t objnr, uint8_t x, uint8_t y)
{
  st_CalcBBOX(objnr);
  if ( st_ClipBBOX() == 0 ) 
    return 0;	/* obj is outside (not visible) */
  if ( x < st_cbbox_x0 )
    return 0;
  if ( x > st_cbbox_x1 )
    return 0;
  if ( y < st_cbbox_y0 )
    return 0;
  if ( y > st_cbbox_y1 )
    return 0;
  return 1;
}

void st_Destroy(uint8_t objnr)
{
  int8_t nr;
  st_obj *o = st_GetObj(objnr);
  switch(u8x8_pgm_read(&(st_object_types[o->ot].destroy_fn)))
  {
    case ST_DESTROY_NONE:      /* only usefull for missels or walls which stay alife */
      break;
    case ST_DESTROY_DISAPPEAR:	/* this should be the default operation */
      st_Disappear(objnr);
      break;
    case ST_DESTROY_GADGET:
      nr = st_FindObj(ST_OT_PLAYER2);
      if ( nr >= 0 )
	st_SetupPlayer(nr, ST_OT_PLAYER3);
      else
      {
	nr = st_FindObj(ST_OT_PLAYER);
	if ( nr >= 0 )
	  st_SetupPlayer(nr, ST_OT_PLAYER2);
      }
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_PY);
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_NY);
      o->ot = ST_OT_GADGET_IMPLODE;
      o->tmp = 0;
      break;
    case ST_DESTROY_TO_DUST:
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_PY);
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_NY);
      o->ot = ST_OT_TRASH_IMPLODE;
      o->tmp = 0;
      break;
    case ST_DESTROY_BIG_TRASH:
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_PY);
      st_NewTrashDustAreaArgs(o->x, o->y, ST_OT_DUST_NY);
      st_InitTrash((o->x>>ST_FP)-1, (o->y>>ST_FP)+3, 2+(st_rnd()&3));
      st_InitTrash((o->x>>ST_FP)-2, (o->y>>ST_FP)-3, -2-(st_rnd()&3));
      st_Disappear(objnr);
      break;
    case ST_DESTROY_PLAYER:
      st_Disappear(objnr);
      st_state = ST_STATE_END;
      o->tmp = 0;
      break;
    case ST_DESTROY_PLAYER_GADGETS:
      /* o->ot = ST_OT_PLAYER; */
      st_SetupPlayer(objnr, ST_OT_PLAYER);
      break;
  }
}

/*
  check if the target (objnr) has been hit.
  st_IsHit() must also destroy the target.
  return value:
    0: do not destroy the missle
    1: destroy the missle
*/
uint8_t st_IsHit(uint8_t objnr, uint8_t x, uint8_t y, uint8_t missle_mask)
{
  uint8_t hit_mask = st_GetHitMask(objnr);
  st_obj *o;
  
  if ( (hit_mask & missle_mask) == 0 )
    return 0;
  
  o = st_GetObj(objnr);
  
  switch(u8x8_pgm_read(&(st_object_types[o->ot].is_hit_fn)))
  {
    case ST_IS_HIT_NONE:
      break;
    case ST_IS_HIT_BBOX:
      if ( st_IsHitBBOX(objnr, x, y) != 0 )
      {
	st_Destroy(objnr);
	return 1;
      }
      break;
    case ST_IS_HIT_WALL:
      if ( st_IsHitBBOX(objnr, x, y) != 0 )
      {
	o->x0++;
	if ( o->x0 < o->x1 )
	{
	  st_NewTrashDust(x, y, ST_OT_DUST_NXPY);
	  st_NewTrashDust(x, y, ST_OT_DUST_NXNY);
	}
	else
	{
	  st_Destroy(objnr);
	  st_NewTrashDust(x, y, ST_OT_DUST_NXPY);
	  st_NewTrashDust(x, y, ST_OT_DUST_NXNY);
	  st_NewTrashDust(x, y, ST_OT_DUST_NY);
	  st_NewTrashDust(x, y, ST_OT_DUST_PY);
	}
	return 1;
      }
      break;
  }
  return 0;
}



/* update all fire counters */
uint8_t st_fire_player = 0;
uint8_t st_fire_period = 51;
uint8_t st_manual_fire_delay = 20;
uint8_t st_is_fire_last_value = 0;

/*
  is_auto_fire == 1
    is_fire will be ignored, autofire enabled
  is_auto_fire == 0
    a transition from 1 to 0 on the is_fire variable will issue a missle
*/
void st_FireStep(uint8_t is_auto_fire, uint8_t is_fire)
{
  if ( is_auto_fire != 0 )
  {
    st_fire_player++;
    if ( st_fire_player >= st_fire_period )
      st_fire_player = 0;
  }
  else
  {
    if ( st_fire_player < st_manual_fire_delay )
    {
      st_fire_player++;
    }
    else
    {
      if ( st_is_fire_last_value == 0 )
	if ( is_fire != 0 )
	  st_fire_player = 0;
    }
    st_is_fire_last_value = is_fire;
  }
}

void st_Fire(uint8_t objnr)
{
  st_obj *o = st_GetObj(objnr);
  uint8_t x;
  uint8_t y;
  
  switch(u8x8_pgm_read(&(st_object_types[o->ot].fire_fn)))
  {
    case ST_FIRE_NONE: 
      break;
    case ST_FIRE_PLAYER1: 
      if ( st_fire_player == 0 )
      {
	/* create missle at st_px_x and st_px_y */
	x = st_CalcXY(o);
	y = st_px_y;
	st_NewPlayerMissle(x , y );
      }
      break;
    case ST_FIRE_PLAYER2:
      if ( st_fire_player == 0 )
      {
	/* create missle at st_px_x and st_px_y */
	x = st_CalcXY(o);
	y = st_px_y;
	st_NewPlayerMissle(x , y );
	st_NewPlayerMissle(x , y+4 );
      }
      break;
    case ST_FIRE_PLAYER3:
      if ( st_fire_player == 0 )
      {
	/* create missle at st_px_x and st_px_y */
	x = st_CalcXY(o);
	y = st_px_y;
	st_NewPlayerMissle(x , y );
	st_NewPlayerMissle(x , y+4 );
	st_NewPlayerMissle(x , y-4 );
      }
      break;
  }
}

/*================================================================*/
/* object init functions */
/*================================================================*/

/*
  x,y are pixel coordinats within the play arey
*/
void st_NewGadget(uint8_t x, uint8_t y)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  st_SetXY(o, x, y);
  o->ot = ST_OT_GADGET;
  o->tmp = 8;
  //o->x = (x)<<ST_FP;
  //o->y = (y)<<ST_FP;
  o->x0 = -3;
  o->x1 = 1;
  o->y0 = -2;
  o->y1 = 2;
}

/*
  x,y are pixel coordinats within the play arey
  dir: direction
    0: random
   != 0 --> assigned
*/
void st_InitTrash(uint8_t x, uint8_t y, int8_t dir)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  if ( (st_rnd() & 1) == 0 )
    o->ot = ST_OT_TRASH1;
  else
    o->ot = ST_OT_TRASH2;
  if ( dir == 0 )
  {
    o->tmp = 0;
    if ( st_rnd() & 1 )
    {
      if ( st_rnd() & 1 )
	o->tmp++;
      else
	o->tmp--;
    }
  }
  else
  {
    o->tmp = dir;
  }
  st_SetXY(o, x, y);
  //o->x = (x)<<ST_FP;
  //o->y = (y)<<ST_FP;
  o->x0 = -3;
  o->x1 = 1;
  o->y0 = -2;
  o->y1 = 2;
  if ( st_difficulty >= 5 )
  {
    if ( (st_rnd() & 3) == 0 )
    {
      o->ot = ST_OT_BIG_TRASH;
      o->y0--;
      o->y1++;
      o->x0--;
      o->x1++;
    }
  }
}

/*
  x,y are pixel coordinats within the play arey
*/
void st_NewTrashDust(uint8_t x, uint8_t y, int ot)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  o->ot = ot;
  st_SetXY(o, x, y);
  //o->x = (x)<<ST_FP;
  //o->y = (y)<<ST_FP;
  o->x0 = 0;
  o->x1 = 0;
  o->y0 = -2;
  o->y1 = 2;
}

void st_NewTrashDustAreaArgs(int16_t x, int16_t y, int ot)
{
  st_NewTrashDust(x>>ST_FP, y>>ST_FP, ot);
}


void st_NewWall(void)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  int8_t h;
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  o->ot = ST_OT_WALL_SOLID;
  h = st_rnd();
  h &= 63;
  h = (int8_t)(((int16_t)h*(int16_t)(ST_AREA_HEIGHT/4))>>6);
  h += ST_AREA_HEIGHT/6;

  o->x0 = 0;
  o->x1 = 5;
  o->x = (ST_AREA_WIDTH-1)<<ST_FP;
 
  if ( (st_rnd() & 1) == 0 )
  {
    o->y = (ST_AREA_HEIGHT-1)<<ST_FP;
    
    o->y0 = -h;
    o->y1 = 0;
  }
  else
  {
    o->y = (0)<<ST_FP;
    o->y0 = 0;
    o->y1 = h;
  }
}

void st_NewPlayerMissle(uint8_t x, uint8_t y)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  o->ot = ST_OT_MISSLE;
  st_SetXY(o, x, y);
  //o->x = x<<ST_FP;
  //o->y = y<<ST_FP;
  o->x0 = -4;
  o->x1 = 1;
  o->y0 = 0;
  o->y1 = 0;
}

void st_SetupPlayer(uint8_t objnr, uint8_t ot)
{
  st_obj *o = st_GetObj(objnr);
  switch(ot)
  {
    case ST_OT_PLAYER:
      o->ot = ot;
      o->y0 = -2;
      o->y1 = 2;
      break;
    case ST_OT_PLAYER2:
      o->ot = ot;
      o->y0 = -2;
      o->y1 = 5;
      break;
    case ST_OT_PLAYER3:
      o->ot = ot;
      o->y0 = -5;
      o->y1 = 5;
      break;
  }
}

void st_NewPlayer(void)
{
  st_obj *o;
  int8_t objnr = st_NewObj();
  if ( objnr < 0 )
    return;
  o = st_GetObj(objnr);
  o->x = 6<<ST_FP;
  o->y = (ST_AREA_HEIGHT/2)<<ST_FP;
  o->x0 = -6;
  o->x1 = 0;
  st_SetupPlayer(objnr, ST_OT_PLAYER);
}

/*================================================================*/
/* trash creation */
/*================================================================*/

void st_InitDeltaWall(void)
{
  uint8_t i;
  uint8_t cnt = 0;
  uint8_t max_x = 0;
  uint8_t max_l;
  
  uint8_t min_dist_for_new = 40;
  uint8_t my_difficulty = st_difficulty;
  
  if ( st_difficulty >= 2 )
  {
    
    max_l = ST_AREA_WIDTH;
    max_l -= min_dist_for_new;
    
    if ( my_difficulty > 30 )
      my_difficulty = 30;
    min_dist_for_new -= my_difficulty;
    
    for( i = 0; i < ST_OBJ_CNT; i++ )
    {
      if ( st_objects[i].ot == ST_OT_WALL_SOLID )
      {
	cnt++;
	if ( max_x < (st_objects[i].x>>ST_FP) )
	  max_x = (st_objects[i].x>>ST_FP);
      }
    }    
    /* if ( cnt < upper_trash_limit ) */
    if ( max_x < max_l ) 
    {
      st_NewWall();
    }
  }
}


void st_InitDeltaTrash(void)
{
  uint8_t i;
  uint8_t cnt = 0;
  uint8_t max_x = 0;
  uint8_t max_l;
  
  uint8_t upper_trash_limit = ST_OBJ_CNT-7;
  uint8_t min_dist_for_new = 20;
  uint8_t my_difficulty = st_difficulty;
  
  if ( my_difficulty > 14 )
    my_difficulty = 14;
  min_dist_for_new -= my_difficulty;
  
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    if ( st_objects[i].ot == ST_OT_TRASH1 || st_objects[i].ot == ST_OT_TRASH2 || st_objects[i].ot == ST_OT_GADGET  || st_objects[i].ot == ST_OT_BIG_TRASH )
    {
      cnt++;
      if ( max_x < (st_objects[i].x>>ST_FP) )
	max_x = (st_objects[i].x>>ST_FP);
    }
  }
  
  max_l = ST_AREA_WIDTH;
  max_l -= min_dist_for_new;
  
  if ( cnt < upper_trash_limit )
    if ( max_x < max_l ) 
    {
      if (  (st_difficulty >= 3)  && ((st_rnd() & 7) == 0) )
	st_NewGadget(ST_AREA_WIDTH-1, rand() & (ST_AREA_HEIGHT-1));
      else
	st_InitTrash(ST_AREA_WIDTH-1, rand() & (ST_AREA_HEIGHT-1),0 );
    }
}

void st_InitDelta(void)
{
  st_InitDeltaTrash();
  st_InitDeltaWall();
  /*

  uint8_t cnt;
  cnt = st_CntObj(2);
  if ( cnt == 0 )
    st_InitBrick1();
  */
}

/*================================================================*/
/* API: game draw procedure */
/*================================================================*/

void st_DrawInGame(uint8_t fps)
{
  uint8_t i;
  /* draw all objects */
  for( i = 0; i < ST_OBJ_CNT; i++ )
      st_DrawObj(i);
  
  //dog_ClrBox(0, ST_AREA_HEIGHT, st_u8g2->width-1, ST_AREA_HEIGHT+3);

  u8g2_SetDrawColor(st_u8g2, 0);
  u8g2_DrawBox(st_u8g2, 0, u8g_height_minus_one - ST_AREA_HEIGHT-3, st_u8g2->width, 4);
  
  u8g2_SetDrawColor(st_u8g2, 1);
  u8g2_DrawHLine(st_u8g2, 0, u8g_height_minus_one - ST_AREA_HEIGHT+1, ST_AREA_WIDTH);
  u8g2_DrawHLine(st_u8g2, 0, u8g_height_minus_one, ST_AREA_WIDTH);
  u8g2_SetFont(st_u8g2, u8g_font_4x6r);
  u8g2_DrawStr(st_u8g2, 0, u8g_height_minus_one - ST_AREA_HEIGHT, st_itoa(st_difficulty));
  u8g2_DrawHLine(st_u8g2, 10, u8g_height_minus_one - ST_AREA_HEIGHT-3, (st_to_diff_cnt>>ST_DIFF_FP)+1);
  u8g2_DrawVLine(st_u8g2, 10, u8g_height_minus_one - ST_AREA_HEIGHT-4, 3);
  u8g2_DrawVLine(st_u8g2, 10+ST_DIFF_VIS_LEN, u8g_height_minus_one - ST_AREA_HEIGHT-4, 3);
  
  
  /* player points */
  u8g2_DrawStr(st_u8g2, ST_AREA_WIDTH-5*4-2, u8g_height_minus_one - ST_AREA_HEIGHT, st_itoa(st_player_points_delayed));
  
  
  /* FPS output */
  if ( fps > 0 )
  {
    //i = dog_DrawStr(ST_AREA_WIDTH-5*4-2-7*4, ST_AREA_HEIGHT, font_4x6, "FPS:");
    i = u8g2_DrawStr(st_u8g2, ST_AREA_WIDTH-5*4-2-7*4, u8g_height_minus_one - ST_AREA_HEIGHT, "FPS:");

    //dog_DrawStr(ST_AREA_WIDTH-5*4-2-7*4+i, ST_AREA_HEIGHT, font_4x6, st_itoa(fps));
    u8g2_DrawStr(st_u8g2, ST_AREA_WIDTH-5*4-2-7*4+i, u8g_height_minus_one - ST_AREA_HEIGHT, st_itoa(fps));
  }
  /*dog_DrawStr(60+i, ST_AREA_HEIGHT, font_4x6, st_itoa(st_CntObj(0)));*/
}

void st_Draw(uint8_t fps)
{
  switch(st_state)
  {
    case ST_STATE_PREPARE:
    case ST_STATE_IPREPARE:
      //dog_DrawStr(0, (st_u8g2->height-6)/2, font_4x6, "SpaceTrash");
      u8g2_SetFont(st_u8g2, u8g_font_4x6r);
      u8g2_SetDrawColor(st_u8g2, 1);
      //dog_DrawStrP(0, (st_u8g2->height-6)/2, font_4x6, DOG_PSTR("SpaceTrash"));
      u8g2_DrawStr(st_u8g2, 0, u8g_height_minus_one - (st_u8g2->height-6)/2, "SpaceTrash");
      //dog_SetHLine(st_u8g2->width-st_to_diff_cnt-10, st_u8g2->width-st_to_diff_cnt, (st_u8g2->height-6)/2-1);
      u8g2_DrawHLine(st_u8g2, st_u8g2->width-st_to_diff_cnt-10, u8g_height_minus_one - (st_u8g2->height-6)/2+1, 11);
      break;
    case ST_STATE_GAME:
      st_DrawInGame(fps);
      break;
    case ST_STATE_END:
    case ST_STATE_IEND:
      u8g2_SetFont(st_u8g2, u8g_font_4x6r);
      u8g2_SetDrawColor(st_u8g2, 1);
      //dog_DrawStr(0, (st_u8g2->height-6)/2, font_4x6, "Game Over");
      //dog_DrawStrP(0, (st_u8g2->height-6)/2, font_4x6, DOG_PSTR("Game Over"));
      u8g2_DrawStr(st_u8g2, 0, u8g_height_minus_one - (st_u8g2->height-6)/2, "Game Over");
      //dog_DrawStr(50, (st_u8g2->height-6)/2, font_4x6, st_itoa(st_player_points));
      u8g2_DrawStr(st_u8g2, 50, u8g_height_minus_one - (st_u8g2->height-6)/2, st_itoa(st_player_points));
      //dog_DrawStr(75, (st_u8g2->height-6)/2, font_4x6, st_itoa(st_highscore));
      u8g2_DrawStr(st_u8g2, 75, u8g_height_minus_one - (st_u8g2->height-6)/2, st_itoa(st_highscore));
      //dog_SetHLine(st_to_diff_cnt, st_to_diff_cnt+10, (st_u8g2->height-6)/2-1);
      u8g2_DrawHLine(st_u8g2, st_to_diff_cnt, u8g_height_minus_one - (st_u8g2->height-6)/2+1, 11);
      break;
  }
}

void st_SetupInGame(void)
{
  st_player_points = 0;
  st_player_points_delayed = 0;
  st_difficulty = 1;
  st_to_diff_cnt = 0;
  st_ClrObjs();
  st_NewPlayer();
  /* st_InitBrick1(); */
}


/*================================================================*/
/* API: game setup */
/*================================================================*/

void st_Setup(u8g2_t *u8g)
{
  st_u8g2 = u8g;
  u8g2_SetBitmapMode(u8g, 1);
  u8g_height_minus_one = u8g->height;
  u8g_height_minus_one--;
}

/*================================================================*/
/* API: game step execution */
/*================================================================*/

/*
  player_pos: 0..255
*/
void st_StepInGame(uint8_t player_pos, uint8_t is_auto_fire, uint8_t is_fire)
{
  uint8_t i, j;
  uint8_t missle_mask;
  
  /* rescale player pos */
  //st_player_pos = ((uint16_t)player_pos * (uint16_t)ST_AREA_HEIGHT)/256;
  if ( player_pos < 64 )
    st_player_pos = 0;
  else if ( player_pos >= 192 )
    st_player_pos = ST_AREA_HEIGHT-2-1;
  else 
    st_player_pos = ((uint16_t)((player_pos-64)) * (uint16_t)(ST_AREA_HEIGHT-2))/128;
  st_player_pos+=1;
  /* move all objects */
  for( i = 0; i < ST_OBJ_CNT; i++ )
    st_Move(i);

  /* check for objects which left the play area */
  for( i = 0; i < ST_OBJ_CNT; i++ )
    if ( st_objects[i].ot != 0 )
      if ( st_IsOut(i) != 0 )
	st_Disappear(i);
      
  /* missle and destruction handling */
  for( i = 0; i < ST_OBJ_CNT; i++ )
  {
    missle_mask = st_GetMissleMask(i);
    if ( missle_mask != 0 )						/* should we apply missle handling? */
      if ( st_CalcXY(st_objects+i) != 0 )						/* yes: calculate pixel reference point (st_px_x, st_px_y) */
	for( j = 0; j < ST_OBJ_CNT; j++ )			/* has any other object been hit? */
	  if ( i != j )							/* except missle itself... */
	    if ( st_IsHit(j, st_px_x, st_px_y, missle_mask) != 0 )		/* let the member function decide */
	    {									/* let the member function destroy the object if required */
	      st_Destroy(i);
	    }
  }
  
  /* handle fire counter */
  st_FireStep(is_auto_fire, is_fire);
  
  /* fire */
  for( i = 0; i < ST_OBJ_CNT; i++ )
    st_Fire(i);
	  
  /* create new objects */
  st_InitDelta();
	
  /* increase difficulty */
	    
  st_to_diff_cnt++;
  if ( st_to_diff_cnt == (ST_DIFF_VIS_LEN<<ST_DIFF_FP) )
  {
    st_to_diff_cnt = 0;
    st_difficulty++;
    st_player_points += ST_POINTS_PER_LEVEL;
  }  
  
  /* update visible player points */
  if ( st_player_points_delayed < st_player_points )
    st_player_points_delayed++;
}
  
void st_Step(uint8_t player_pos, uint8_t is_auto_fire, uint8_t is_fire)
{
  switch(st_state)
  {
    case ST_STATE_PREPARE:
      st_to_diff_cnt = st_u8g2->width-10;		/* reuse st_to_diff_cnt */
      st_state = ST_STATE_IPREPARE;
      break;
    case ST_STATE_IPREPARE:
      st_to_diff_cnt--;
      if ( st_to_diff_cnt == 0 )
      {
	st_state = ST_STATE_GAME;
	st_SetupInGame();
      }
      break;
    case ST_STATE_GAME:
      st_StepInGame(player_pos, is_auto_fire, is_fire);
      break;
    case ST_STATE_END:
      st_to_diff_cnt = st_u8g2->width-10;		/* reuse st_to_diff_cnt */
      if ( st_highscore < st_player_points)
	st_highscore = st_player_points;
      st_state = ST_STATE_IEND;
      break;
    case ST_STATE_IEND:
      st_to_diff_cnt--;
      if ( st_to_diff_cnt == 0 )
	st_state = ST_STATE_PREPARE;
      break;
  }
}



void setup(void) {
  u8g2.begin(); 
}

uint8_t a;
uint8_t b;
uint8_t y = 128;

void loop(void) {

  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setFontDirection(0);
  u8g2.setFontRefHeightAll();

  st_Setup(u8g2.getU8g2());
  for(;;)
  {
    st_Step(y, /* is_auto_fire */ 0, /* is_fire */ digitalRead(pin_fire));
    u8g2.firstPage();
    do
    {
      st_Draw(0);
    } while( u8g2.nextPage() );
    
    if ( digitalRead(pin_down) ) {
      y++;
    }
    
    if ( digitalRead(pin_up) ) {
      y--;
    }
  }
}


