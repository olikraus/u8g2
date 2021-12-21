/*

  FPS.cpp

  Frames Per Second: Display Performance Test

  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).

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



==============
  U8glib

  ST7920_192X32, SPI:     FPS: Box=7.6   @=9.8                iFPS: Box=11.4  @=14.7
  ST7920_192X32, 8Bit:    FPS: Box=6.2   @=7.5                iFPS: Box=9.3 @=11.2
  DOGM128 SW SPI:         FPS: Box=5.1   @=5.9  Pix=2.6       iFPS: Box=10.2 @=11.8 Pix=5.2
  DOGM128 HW SPI:         FPS: Box=5.5   @=6.3                iFPS: Box=11.0 @=12.6
  DOGXL160 SW SPI:        FPS: Box=1.7   @=1.9               iFPS: Box=6.9 @=7.7
  DOGXL160 HW SPI:        FPS: Box=1.8   @=2.1

  NHD27OLED_BW, SW SPI: FPS: Box=3.0  @=3.7
  NHD27OLED_BW, HW SPI: FPS: Box=3.5  @=4.5
  NHD27OLED_2X_BW, SW SPI: FPS: Box=3.8  @=4.9
  NHD27OLED_2X_BW, HW SPI: FPS: Box=4.6  @=6.4

  30 Sep 2012
  NHD27OLED_BW, SW SPI: FPS: Clip=9.2 Box=3.9  @=4.4    NEW_CODE
  NHD27OLED_BW, SW SPI: FPS: Clip=9.2 Box=3.6  @=4.5
  NHD27OLED_BW, HW SPI: FPS: Clip=16.3 Box=4.7  @=5.6
  NHD27OLED_2X_BW, SW SPI: FPS: Clip=9.7 Box=4.5  @=5.8
  NHD27OLED_2X_BW, SW SPI: FPS: Clip=18.0 Box=5.8  @=7.9

  1 Oct 2012
  ST7920_192X32, 8Bit:    FPS: Box=7.2   @=10.0
  DOGM128 SW SPI:         FPS: Box=5.2   @=6.6  Pix=2.6
  DOGM128 HW SPI:         FPS: Clip=33.2 Box=5.5   @=7.1
  DOGXL160 SW SPI:        FPS: Box=1.7   @=2.0
  DOGXL160 HW SPI:        FPS: Box=1.8   @=2.2

  DOGXL160 GR SW SPI:     FPS: Box=1.1   @=1.3

  1 Mar 2013
  ST7920_192X32_1X, SPI:      FPS: Clip=10.3 Box=5.5  @=7.2 Pix=3.9
  ST7920_192X32_4X, SPI:      FPS: Clip=10.9 Box=6.7  @=8.8 Pix=7.4
  ST7920_192X32_1X, 8Bit:     FPS: Clip=14.2 Box=6.1  @=8.4 Pix=4.2
  ST7920_192X32_4X, 8Bit:     FPS: Clip=14.2 Box=7.8  @=10.7 Pix=8.7
  ST7920_192X32_1X, HW SPI:   FPS: Clip=14.2 Box=6.3  @=8.7 Pix=4.3
  ST7920_192X32_4X, HW SPI:   FPS: Clip=15.3 Box=8.0  @=11.2 Pix=9.0

  2 Jun 2013
  U8GLIB_DOGM128 SW SPI:      FPS: Clip=23.9 Box=4.5  @=6.6 Pix=2.1
  U8GLIB_DOGM128_2X SW SPI:   FPS: Clip=28.5 Box=6.6  @=9.7 Pix=3.9
  U8GLIB_DOGM128_2X HW SPI:   FPS: Clip=40.8 Box=7.1  @=10.8 Pix=4.1
  
  3 Jun 2013
  U8GLIB_ST7920_192X32_1X -Os  SW SPI  FPS: Clip=11.0 Box=5.4  @=7.1 Pix=3.9  Size=11828
  U8GLIB_ST7920_192X32_1X -O3  SW SPI  FPS: Clip=10.9 Box=5.6  @=7.5 Pix=4.0  Size=13800
  U8GLIB_ST7920_192X32_1X -Os  SW SPI  FPS: Clip=16.8 Box=6.7  @=9.6 Pix=4.5  Size=11858  (new seq data output)
  U8GLIB_ST7920_192X32_1X -Os  HW SPI  FPS: Clip=25.7 Box=7.5  @=11.3 Pix=4.8  (new seq data output)
  
  6 Jun 2013
  U8GLIB_DOGS102 u8g(13, 11, 10, 9);    STD SW SPI  FPS: Clip=9.5 Box=7.6  @=8.2 Pix=6.2  Size=15652
  U8GLIB_DOGS102 u8g(13, 11, 10, 9);    SW SPI          FPS: Clip=19.1 Box=12.8  @=14.0 Pix=9.2  Size=15532


  12 Jun 2013
  SSD1351_128X128_332  SW SPI Clip=1.3 Box=0.7  @=0.9 Pix=0.4
  SSD1351_128X128_332  HW SPI Clip=3.6 Box=1.1  @=1.5 Pix=0.5

  24 Jun 2013
  Uno SSD1351_128X128_332               SW SPI Clip=1.4 Box=0.8  @=0.9 Pix=0.4
  
  Uno SSD1351_128X128_332               HW SPI Clip=4.4 Box=1.2  @=1.6 Pix=0.5
  Uno SSD1351_128X128_HICOLOR           HW SPI Clip=3.7 Box=0.8  @=1.0 Pix=0.3

  Mega2560 SSD1351_128X128_332          HW SPI Clip=4.4 Box=1.2  @=1.6 Pix=0.5
  Mega2560 SSD1351_128X128_4X_332       HW SPI Clip=4.6 Box=2.3  @=2.8 Pix=1.5
  Mega2560 SSD1351_128X128_HICOLOR      HW SPI Clip=3.6 Box=0.8  @=1.0 Pix=0.3
  Mega2560 SSD1351_128X128_4X_HICOLOR   HW SPI Clip=4.2 Box=1.7  @=2.1 Pix=1.0

  Due SSD1351_128X128_332               HW SPI Clip=24.6 Box=6.3  @=7.8 Pix=2.8
  Due SSD1351_128X128_4X_332            HW SPI Clip=28.1 Box=13.0  @=15.1 Pix=8.5
  Due SSD1351_128X128_HICOLOR           HW SPI Clip=20.8 Box=3.4  @=4.5 Pix=1.4
  Due SSD1351_128X128_4X_HICOLOR        HW SPI Clip=26.3 Box=8.9  @=11.1 Pix=4.8

  Due SSD1351_128X128_4X_HICOLOR        SW SPI Clip=0.4 Box=0.4  @=0.4 Pix=0.4

  Due DOGS102 u8g(13, 11, 10, 9);       SW SPI    FPS: Clip=19.1 Box=13.1  @=14.3 Pix=9.4
  Due DOGS102 u8g(10, 9);               HW SPI    FPS: Clip=128.9 Box=30.7  @=40.6 Pix=15.4

  Due NHD27OLED_BW u8g(10, 9)           HW SPI    FPS: Clip=53.0 Box=19.6  @=23.8 Pix=10.6
  Due NHD27OLED_2X_BW u8g(10, 9)        HW SPI    FPS: Clip=57.0 Box=25.3  @=31.7 Pix=18.1
  Due NHD27OLED_GR u8g(10, 9)           HW SPI    FPS: Clip=34.1 Box=11.7  @=13.7 Pix=5.6
  Due NHD27OLED_2X_GR u8g(10, 9)        HW SPI    FPS: Clip=38.1 Box=15.5  @=20.0 Pix=8.8


  28. Apr 2016
  Uno DOGS102 u8g(13, 11, 10, 9);       SW SPI          FPS: Clip=26.6 Box=6.1  @=8.5 Pix=2.8    11548 Bytes
  Uno DOGS102 u8g(10, 9);               HW SPI          FPS: Clip=33.1 Box=6.4  @=9.1 Pix=2.9    11366 Bytes

==============
  U8g2

  28. Apr 2016
  Uno DOGS102 u8g(13, 11, 10, 9);       SW SPI          FPS: Clip=3.0 Box=3.3  @=2.1 Pix=2.4    11472 Bytes
  Uno DOGS102 u8g(10, 9);               HW SPI          FPS: Clip=24.5 Box=87.4  @=5.0 Pix=7.3  11554 Bytes

  15. Mai 2016
  Uno U8G2_UC1701_DOGS102_1_4W_SW_SPI   SW SPI          FPS: Clip=3.5 Box=3.8  @=2.3 Pix=2.7    Arduino Flash Size: 13262 Bytes
  Uno U8G2_UC1701_DOGS102_1_4W_HW_SPI   HW SPI          FPS: Clip=26.3 Box=90.5  @=5.1 Pix=8.5  Arduino Flash Size: 13370 Bytes
  Uno U8G2_UC1701_DOGS102_F_4W_HW_SPI   HW SPI          FPS: Clip=50.6 Box=119.6  @=9.4 Pix=15.7  Arduino Flash Size: 13370 Bytes

  16. Mai 2016 (improved SW 4-Wire SPI)
  Uno U8G2_UC1701_DOGS102_1_4W_SW_SPI   SW SPI          FPS: Clip=18.6 Box=42.2  @=4.7 Pix=7.7    

  24. Mai 2016
  Uno U8G2_ST7565_DOGM128_1_4W_SW_SPI   SW SPI          FPS: Clip=18.1 Box=35.8  @=3.9 Pix=6.2    
  Uno U8G2_ST7565_DOGM128_1_4W_HW_SPI   HW SPI          FPS: Clip=25.4 Box=78.7  @=4.2 Pix=6.9    

  25. June 2016
  U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI  HW SPI    FPS: Clip=56.4 Box=114.1  @=8.6 Pix=12.9
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  HW SPI    FPS: Clip=27.7 Box=87.7  @=4.3 Pix=7.1

  18. Aug 2016
  U8G2_ST7920_128X64_1_HW_SPI        Uno      FPS: Clip=13.2 Box=11.5  @=3.7 Pix=5.9
  U8G2_ST7920_128X64_1_SW_SPI        Uno      FPS: Clip=1.2 Box=1.2  @=0.9 Pix=1.1
  
  U8G2_ST7920_128X64_1_SW_SPI        Due      FPS: Clip=3.1 Box=3.1  @=2.8 Pix=2.9
  U8G2_ST7920_128X64_1_HW_SPI        Due      FPS: Clip=26.2 Box=25.8  @=13.9 Pix=15.3


  U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI    Uno      Clip=12.6 Box=19.1  @=3.6 Pix=5.8
  U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI    Uno      Clip=15.1 Box=23.3  @=3.6 Pix=5.8    with graphics acceleration

  Issue #79:
  U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C Atmega32U8     Clip=15.7 Box=26.6  @=5.3 Pix=8.4

  26.Nov 2016
  U8G2_ST7920_128X64_1_8080        Uno          Clip=13.5 Box=12.1  @=3.6 Pix=5.9    atmega&arduino optimized
  U8G2_ST7920_128X64_1_SW_SPI         Uno       Clip=13.2 Box=11.4  @=3.5 Pix=5.6      issue 87
  U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI  Uno   Clip=21.7 Box=46.8  @=4.0 Pix=7.1      
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno   Clip=25.8 Box=84.2  @=4.2 Pix=7.8      
  U8G2_SSD1306_128X64_NONAME_1_8080    Uno      Clip=16.1 Box=29.1  @=3.9 Pix=6.6    
  U8G2_SSD1306_128X64_NONAME_1_6800    Uno      Clip=6.8 Box=8.4  @=2.9 Pix=4.3      6800 mode is not yet optimized for Atmega    
  
  7 Dec 2016
  U8G2_KS0108_128X64_1           Uno            Clip=6.7 Box=8.2  @=2.9 Pix=4.3
  
  11 Dec 2016
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno   Clip=25.8 Box=84.2  @=4.2 Pix=7.8    Old: no pixel optimization
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno   Clip=25.9 Box=84.1  @=4.3 Pix=8.3    Pixel (len=1) optimization

  14 Dec 2016
  U8G2_KS0108_128X64_1           Uno            Clip=15.9 Box=28.0  @=3.9 Pix=6.9

  31 Dec 2016
  U8G2_LC7981_160X80_1_6800         Uno         Clip=3.3 Box=3.0  @=1.5 Pix=2.1

  14 Jan 2017
  U8G2_ST7920_128X64_1_HW_SPI         Uno       Clip=16.3 Box=20.2  @=4.1 Pix=7.2
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno   Clip=31.7 Box=77.2  @=4.7 Pix=8.9

  11 Mar 2017
  before 
  U8G2_ST7920_128X64_1_SW_SPI         Uno      Clip=16.0 Box=18.5  @=4.0 Pix=6.8    issue 200
  U8G2_ST7920_128X64_1_HW_SPI         Uno      Clip=16.3 Box=20.2  @=4.1 Pix=7.2    issue 200

  after
  U8G2_ST7920_128X64_1_SW_SPI         Uno      Clip=17.2 Box=20.2  @=4.1 Pix=7.0    issue 200
  U8G2_ST7920_128X64_1_HW_SPI         Uno      Clip=17.4 Box=21.8  @=4.2 Pix=7.4    issue 200

  14 May 2017, Arduino 1.8
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno  4MHz  Clip=31.7 Box=77.2  @=4.7 Pix=8.9    issue 215
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno  8MHz  Clip=32.9 Box=84.1  @=4.7 Pix=9.0    issue 215

  U8G2_ST7565_EA_DOGM128_1_4W_HW_SPI      Uno  4MHz  Clip=32.0 Box=78.5  @=4.7 Pix=8.9    issue 215
  
  U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI     Uno  8MHz 16 Bit  Clip=9.0 Box=10.7  @=2.0 Pix=3.0    issue 302 before optimization
  U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI     Uno  10MHz 16 Bit  Clip=10.9 Box=13.5  @=2.0 Pix=3.2  issue 302 after optimization (runs with 8MHz)
  
  U8G2_SSD1327_SEEED_96X96_1_HW_I2C       Uno      Clip=1.2 Box=1.2  @=0.9 Pix=1.0    issue 302 before optimization
  U8G2_SSD1327_SEEED_96X96_1_HW_I2C       Uno      Clip=1.4 Box=1.4  @=1.0 Pix=1.2    issue 302 after optimization

  U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI    Uno      Clip=18.2 Box=25.1  @=4.0 Pix=6.7    issue 302 before optimization
  U8G2_SSD1325_NHD_128X64_1_4W_HW_SPI    Uno      Clip=19.1 Box=28.3  @=4.1 Pix=7.2    issue 302 after optimization

  15 Aug 2017, Arduino 1.8.3
  U8G2_SSD1306_128X64_NONAME_1_HW_I2C    MKR Zero    Clip=20.8 Box=24.1  @=9.9 Pix=11.4

  05 Sep 2017, Arduino 1.8,0 
  U8G2_SH1106_128X64_NONAME_1_HW_I2C    Uno      Clip=13.6 Box=19.4  @=3.8 Pix=5.9

  6 Oct 2017, Arduino 1.8.4
  U8G2_SSD1306_128X64_NONAME_1_SW_I2C    EPS32    Clip=9.5 Box=9.6  @=8.9 Pix=9.1
  U8G2_SSD1306_128X64_NONAME_1_HW_I2C    EPS32    Clip=30.5 Box=31.3  @=24.7 Pix=26.7

  9 Mar 2018, Arduino 1.8.4, 16 Bit Mode
  U8G2_ST75256_JLX256128_1_4W_SW_SPI    MKR Zero    Clip=2.0 Box=2.0  @=1.2 Pix=1.3
  U8G2_ST75256_JLX256128_1_4W_HW_SPI    MKR Zero    Clip=30.3 Box=31.0  @=2.6 Pix=2.9

  23 Apr 2018, Arduino 1.8.4, 8 Bit Mode
  U8G2_SSD1306_128X64_NONAME_1_3W_SW_SPI   Uno      Clip=2.7 Box=2.9  @=1.8 Pix=2.2

  29 Apr 2018, Arduino 1.8.4, 8 Bit Mode
  U8G2_SSD1306_128X64_NONAME_1_SW_I2C    Uno      Clip=1.8 Box=1.9  @=1.4 Pix=1.6    issue 593, before optimization
  U8G2_SSD1306_128X64_NONAME_1_SW_I2C    Uno      Clip=4.9 Box=5.3  @=2.6 Pix=3.6    issue 593, after optimization, however, disabled by default
  
  U8G2_SSD1306_128X64_NONAME_1_3W_SW_SPI   Uno      Clip=2.7 Box=2.9  @=1.8 Pix=2.2    issue 586, before optimization
  U8G2_SSD1306_128X64_NONAME_1_3W_SW_SPI   Uno      Clip=23.4 Box=40.8  @=4.4 Pix=8.1  issue 586, after optimization


  28 Oct 2018, Arduino 1.8.4, 8 Bit Mode
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno      Clip=34.0 Box=88.4  @=4.5 Pix=8.2  issue 364, clip window 

  U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C   Uno      Clip=28.4 Box=39.2  @=10.0 Pix=14.9  I2C default
  U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C   Uno      Clip=32.9 Box=48.3  @=10.5 Pix=16.1  I2C default, u8g2.setBusClock(600000UL)

  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno      Clip=33.9 Box=88.2  @=4.5 Pix=8.2  SPI default speed      
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno      Clip=32.8 Box=80.6  @=4.5 Pix=8.2  SPI default speed  u8g2.setBusClock(6000000UL);
  U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI  Uno      Clip=33.9 Box=88.2  @=4.5 Pix=8.2  SPI default speed  u8g2.setBusClock(9000000UL);

  2 Dez 2018, Arduino 1.8.4
  U8G2_UC1610_EA_DOGXL160_1_4W_SW_SPI    Due      Clip=15.8 Box=16.9  @=6.3 Pix=6.4
  U8G2_UC1610_EA_DOGXL160_1_4W_SW_SPI    Due      Clip=17.6 Box=19.2  @=6.4 Pix=6.8  issue 749 (atomic writes)

*/


#include <rtthread.h>
#include <U8g2lib.h>

// You may reference Drivers/drv_gpio.c for pinout
// In u8x8.h #define U8X8_USE_PINS 
#define OLED_I2C_PIN_SCL                    22  // PB6
#define OLED_I2C_PIN_SDA                    23  // PB7

static U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ OLED_I2C_PIN_SCL, /* data=*/ OLED_I2C_PIN_SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
//static U8G2_NULL u8g2(U8G2_R0);    // null device, a 8x8 pixel display which does nothing
//static U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6);    // Arduboy (Production, Kickstarter Edition)
//static U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1306_128X64_NONAME_F_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* reset=*/ 8);
//static U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//static U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // same as the NONAME variant, but may solve the "every 2nd line skipped" problem
//static U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* reset=*/ 8);
//static U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ OLED_I2C_PIN_SCL, /* data=*/ OLED_I2C_PIN_SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
//static U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 16, /* data=*/ 17, /* reset=*/ U8X8_PIN_NONE);   // ESP32 Thing, pure SW emulated I2C
//static U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17);   // ESP32 Thing, HW I2C with pin remapping
//static U8G2_SSD1306_128X64_NONAME_F_6800 u8g2(U8G2_R0, 13, 11, 2, 3, 4, 5, 6, A4, /*enable=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8);
//static U8G2_SSD1306_128X64_NONAME_F_8080 u8g2(U8G2_R0, 13, 11, 2, 3, 4, 5, 6, A4, /*enable=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8);
//static U8G2_SSD1306_128X64_VCOMH0_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // same as the NONAME variant, but maximizes setContrast() range
//static U8G2_SSD1306_128X64_ALT0_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // same as the NONAME variant, but may solve the "every 2nd line skipped" problem
//static U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//static U8G2_SH1106_128X64_VCOMH0_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);        // same as the NONAME variant, but maximizes setContrast() range
//static U8G2_SH1106_128X64_WINSTAR_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);        // same as the NONAME variant, but uses updated SH1106 init sequence
//static U8G2_SH1106_72X40_WISE_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SH1107_64X128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SH1107_128X128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SH1107_128X128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
//static U8G2_SH1107_PIMORONI_128X128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
//static U8G2_SH1107_SEEED_96X96_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SH1108_160X160_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SH1122_256X64_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);                // Enable U8G2_16BIT in u8g2.h
//static U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 21, /* data=*/ 20, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather M0 Basic Proto + FeatherWing OLED
//static U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
//static U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//static U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C
//static U8G2_SSD1306_128X32_WINSTAR_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C
//static U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered
//static U8G2_SSD1306_48X64_WINSTAR_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   
//static U8G2_SSD1306_64X32_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
//static U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
//static U8G2_SSD1306_96X16_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // EastRising 0.69" OLED
//static U8G2_SSD1322_NHD_256X64_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // Enable U8G2_16BIT in u8g2.h
//static U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // Enable U8G2_16BIT in u8g2.h
//static U8G2_SSD1322_NHD_128X64_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1322_NHD_128X64_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1325_NHD_128X64_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//static U8G2_SSD1325_NHD_128X64_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    
//static U8G2_SSD0323_OS128064_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//static U8G2_SSD0323_OS128064_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    
//static U8G2_SSD1326_ER_256X32_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);         // experimental driver for ER-OLED018-1
//static U8G2_SSD1327_SEEED_96X96_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    // Seeedstudio Grove OLED 96x96
//static U8G2_SSD1327_SEEED_96X96_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);    // Seeedstudio Grove OLED 96x96
//static U8G2_SSD1327_EA_W128128_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1327_EA_W128128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1327_EA_W128128_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);
//static U8G2_SSD1327_EA_W128128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//static U8G2_SSD1327_MIDAS_128X128_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1327_MIDAS_128X128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1329_128X96_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1329_128X96_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1305_128X32_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1305_128X32_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1305_128X32_ADAFRUIT_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1305_128X32_ADAFRUIT_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1305_128X64_ADAFRUIT_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1305_128X64_ADAFRUIT_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_SSD1317_96X96_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // not tested, not confirmed
//static U8G2_SSD1317_96X96_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);     // not tested, not confirmed
//static U8G2_SSD1318_128X96_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_SSD1318_128X96_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//static U8G2_LD7032_60X32_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 9, /* dc=*/ 10, /* reset=*/ 8);    // SW SPI Nano Board
//static U8G2_LD7032_60X32_F_4W_SW_I2C u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* reset=*/ U8X8_PIN_NONE);    // NOT TESTED!
//static U8G2_UC1701_EA_DOGS102_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_UC1701_EA_DOGS102_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // Nokia 5110 Display
//static U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);         // Nokia 5110 Display
//static U8G2_PCF8812_96X65_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // Could be also PCF8814
//static U8G2_PCF8812_96X65_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);                        // Could be also PCF8814
//static U8G2_HX1230_96X68_F_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* reset=*/ 8);
//static U8G2_HX1230_96X68_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_KS0108_128X64_F u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*dc=*/ 17, /*cs0=*/ 14, /*cs1=*/ 15, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE);     // Set R/W to low!
//static U8G2_KS0108_ERM19264_F u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*dc=*/ 17, /*cs0=*/ 14, /*cs1=*/ 15, /*cs2=*/ 16, /* reset=*/  U8X8_PIN_NONE);     // Set R/W to low!
//static U8G2_ST7920_192X32_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ U8X8_PIN_NONE, /*dc=*/ 17, /*reset=*/ U8X8_PIN_NONE);
//static U8G2_ST7920_192X32_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 16 /* A2 */, /* CS=*/ 17 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
//static U8G2_ST7920_128X64_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18 /* A4 */, /*cs=*/ U8X8_PIN_NONE, /*dc/rs=*/ 17 /* A3 */, /*reset=*/ 15 /* A1 */);    // Remember to set R/W to 0 
//static U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 16 /* A2 */, /* CS=*/ 17 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
//static U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8);
//static U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);
//static U8G2_ST7565_EA_DOGM128_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_EA_DOGM128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_64128N_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_64128N_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_EA_DOGM132_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ U8X8_PIN_NONE);    // DOGM132 Shield
//static U8G2_ST7565_EA_DOGM132_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ U8X8_PIN_NONE);    // DOGM132 Shield
//static U8G2_ST7565_ZOLEN_128X64_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_ZOLEN_128X64_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_LM6059_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);        // Adafruit ST7565 GLCD
//static U8G2_ST7565_LM6059_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);        // Adafruit ST7565 GLCD
//static U8G2_ST7565_LX12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_LX12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_ERC12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_ERC12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); // contrast improved version for ERC12864
//static U8G2_ST7565_ERC12864_ALT_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // contrast improved version for ERC12864
//static U8G2_ST7565_NHD_C12832_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_NHD_C12832_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_JLX12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7565_JLX12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST7567_PI_132X64_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  // Pax Instruments Shield, LCD_BL=6
//static U8G2_ST7567_PI_132X64_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  // Pax Instruments Shield, LCD_BL=6
//static U8G2_ST7567_JLX12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_ST7567_JLX12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_ST7567_ENH_DG128064_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//static U8G2_ST7567_ENH_DG128064_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//static U8G2_ST7567_ENH_DG128064I_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//static U8G2_ST7567_ENH_DG128064I_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//static U8G2_ST7567_64X32_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
//static U8G2_ST75256_JLX172104_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST75256_JLX172104_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST75256_JLX19296_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST75256_JLX19296_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST75256_JLX256128_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // Enable U8g2 16 bit mode for this display
//static U8G2_ST75256_JLX256128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);        // Enable U8g2 16 bit mode for this display
//static U8G2_ST75256_JLX256128_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 9, /* data=*/ 8, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 5);  // MKR Zero, Enable U8g2 16 bit mode for this display
//static U8G2_ST75256_JLX256128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 5);  // MKR Zero, Enable U8g2 16 bit mode for this display
//static U8G2_ST75256_JLX256160_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // Enable U8g2 16 bit mode for this display
//static U8G2_ST75256_JLX256160_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);        // Enable U8g2 16 bit mode for this display
//static U8G2_ST75256_JLX240160_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST75256_JLX240160_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_ST75256_JLX25664_F_2ND_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);    // Due, 2nd I2C, enable U8g2 16 bit mode for this display
//static U8G2_NT7534_TG12864R_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_NT7534_TG12864R_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_ST7588_JLX12864_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ 5);  
//static U8G2_ST7588_JLX12864_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 5);
//static U8G2_IST3020_ERC19264_F_6800 u8g2(U8G2_R0, 44, 43, 42, 41, 40, 39, 38, 37,  /*enable=*/ 28, /*cs=*/ 32, /*dc=*/ 30, /*reset=*/ 31); // Connect WR pin with GND
//static U8G2_IST3020_ERC19264_F_8080 u8g2(U8G2_R0, 44, 43, 42, 41, 40, 39, 38, 37,  /*enable=*/ 29, /*cs=*/ 32, /*dc=*/ 30, /*reset=*/ 31); // Connect RD pin with 3.3V
//static U8G2_IST3020_ERC19264_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//static U8G2_LC7981_160X80_F_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RW with GND
//static U8G2_LC7981_160X160_F_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RW with GND
//static U8G2_LC7981_240X128_F_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RW with GND
//static U8G2_LC7981_240X64_F_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RW with GND
//static U8G2_SED1520_122X32_F u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*dc=*/ A0, /*e1=*/ A3, /*e2=*/ A2, /* reset=*/  A4);     // Set R/W to low!
//static U8G2_T6963_240X128_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//static U8G2_T6963_256X64_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//static U8G2_T6963_160X80_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//static U8G2_T6963_128X64_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//static U8G2_T6963_128X64_ALT_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable/wr=*/ 17, /*cs/ce=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FS0 and FS1 with GND
//static U8G2_SED1330_240X128_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect RD with +5V, FG with GND
//static U8G2_SED1330_240X128_F_6800 u8g2(U8G2_R0, 13, 11, 2, 3, 4, 5, 6, A4, /*enable=*/ 7, /*cs=*/ 10, /*dc=*/ 9, /*reset=*/ 8); // A0 is dc pin!
//static U8G2_RA8835_NHD_240X128_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // Connect /RD = E with +5V, enable is /WR = RW, FG with GND, 14=Uno Pin A0
//static U8G2_RA8835_NHD_240X128_F_6800 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 17, /*cs=*/ 14, /*dc=*/ 15, /*reset=*/ 16); // A0 is dc pin, /WR = RW = GND, enable is /RD = E
//static U8G2_UC1604_JLX19264_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//static U8G2_UC1604_JLX19264_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  
//static U8G2_UC1608_ERC24064_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // SW SPI, Due ERC24064-1 Test Setup
//static U8G2_UC1608_ERC240120_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8); 
//static U8G2_UC1608_240X128_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // SW SPI, Due ERC24064-1 Test Setup
//static U8G2_UC1610_EA_DOGXL160_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/  U8X8_PIN_NONE);
//static U8G2_UC1610_EA_DOGXL160_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/  U8X8_PIN_NONE);
//static U8G2_UC1611_EA_DOGM240_F_2ND_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);    // Due, 2nd I2C, DOGM240 Test Board
//static U8G2_UC1611_EA_DOGM240_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);   // Due, SW SPI, DOGXL240 Test Board
//static U8G2_UC1611_EA_DOGXL240_F_2ND_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);    // Due, 2nd I2C, DOGXL240 Test Board
//static U8G2_UC1611_EA_DOGXL240_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);   // Due, SW SPI, DOGXL240 Test Board
//static U8G2_UC1611_EW50850_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,  /*enable=*/ 18, /*cs=*/ 3, /*dc=*/ 16, /*reset=*/ 16); // 240x160, Connect RD/WR1 pin with 3.3V, CS is aktive high
//static U8G2_UC1638_160X128_F_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/ 2, /* dc=*/ 3, /* reset=*/ 4);      // Not tested
//static U8G2_SSD1606_172X72_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);        // eInk/ePaper Display
//static U8G2_SSD1607_200X200_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // eInk/ePaper Display, original LUT from embedded artists
//static U8G2_SSD1607_GD_200X200_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // Good Display
//static U8G2_SSD1607_WS_200X200_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // Waveshare
//static U8G2_IL3820_296X128_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // WaveShare 2.9 inch eInk/ePaper Display, enable 16 bit mode for this display!
//static U8G2_IL3820_V2_296X128_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);    // ePaper Display, lesser flickering and faster speed, enable 16 bit mode for this display!
//static U8G2_MAX7219_64X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);
//static U8G2_MAX7219_32X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);
//static U8G2_MAX7219_8X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);
//static U8G2_LS013B7DH03_128X128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);    // there is no DC line for this display
//static U8G2_LS027B7DH01_400X240_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);    // there is no DC line for this display
//static U8G2_LS013B7DH05_144X168_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ 8);    // there is no DC line for this display


// End of constructor list

typedef u8g2_uint_t u8g_uint_t;

#define SECONDS 10
uint8_t flip_color = 0;
uint8_t draw_color = 1;

static void draw_set_screen(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g2.setColorIndex(flip_color);
  u8g2.drawBox( 0, 0, u8g2.getWidth(), u8g2.getHeight() );
}

static void draw_clip_test(void) {
  u8g_uint_t i, j, k;
  char buf[3] = "AB";
  k = 0;
  u8g2.setColorIndex(draw_color);
  u8g2.setFont(u8g2_font_6x10_tf);
  
  for( i = 0; i  < 6; i++ ) {
    for( j = 1; j  < 8; j++ ) {
      u8g2.drawHLine(i-3, k, j);
      u8g2.drawHLine(i-3+10, k, j);
      
      u8g2.drawVLine(k+20, i-3, j);
      u8g2.drawVLine(k+20, i-3+10, j);
      
      k++;
    }
  }
  u8g2.setFontDirection(0);
  u8g2.drawStr(0-3, 50, buf);
  u8g2.setFontDirection(2);
  u8g2.drawStr(0+3, 50, buf);
  
  u8g2.setFontDirection(0);
  u8g2.drawStr(u8g2.getWidth()-3, 40, buf);
  u8g2.setFontDirection(2);
  u8g2.drawStr(u8g2.getWidth()+3, 40, buf);

  u8g2.setFontDirection(1);
  u8g2.drawStr(u8g2.getWidth()-10, 0-3, buf);
  u8g2.setFontDirection(3);
  u8g2.drawStr(u8g2.getWidth()-10, 3, buf);

  u8g2.setFontDirection(1);
  u8g2.drawStr(u8g2.getWidth()-20, u8g2.getHeight()-3, buf);
  u8g2.setFontDirection(3);
  u8g2.drawStr(u8g2.getWidth()-20, u8g2.getHeight()+3, buf);

  u8g2.setFontDirection(0);

}

static void draw_char(void) {
  char buf[2] = "@";
  u8g_uint_t i, j;
  // graphic commands to redraw the complete screen should be placed here  
  u8g2.setColorIndex(draw_color);
  u8g2.setFont(u8g2_font_6x10_tf);
  j = 8;
  for(;;) {
    i = 0;
    for(;;) {
      u8g2.drawStr( i, j, buf);
      i += 8;
      if ( i > u8g2.getWidth() )
        break;
    }
    j += 8;
    if ( j > u8g2.getHeight() )
      break;
  }
  
}

static void draw_pixel(void) {
  u8g_uint_t x, y, w2, h2;
  u8g2.setColorIndex(draw_color);
  w2 = u8g2.getWidth();
  h2 = u8g2.getHeight();
  w2 /= 2;
  h2 /= 2;
  for( y = 0; y < h2; y++ ) {
    for( x = 0; x < w2; x++ ) {
      if ( (x + y) & 1 ) {
        u8g2.drawPixel(x,y);
        u8g2.drawPixel(x,y+h2);
        u8g2.drawPixel(x+w2,y);
        u8g2.drawPixel(x+w2,y+h2);
      }
    }
  }
}

// returns unadjusted FPS
static uint16_t picture_loop_with_fps(void (*draw_fn)(void)) {
  uint16_t FPS10 = 0;
  uint32_t time;

  time = ((unsigned long)rt_tick_get() / RT_TICK_PER_SECOND * 1000) + SECONDS*1000;

  // picture loop
  do {
    u8g2.firstPage();
    do {
      draw_fn();
    } while( u8g2.nextPage() );
    FPS10++;
    flip_color = flip_color ^ 1;
  } while( ((unsigned long)rt_tick_get() / RT_TICK_PER_SECOND * 1000) < time );
  return FPS10;
}

const char *convert_FPS(uint16_t fps) {
  static char buf[6];
  strcpy(buf, u8g2_u8toa( (uint8_t)(fps/10), 3));
  buf[3] =  '.';
  buf[4] = (fps % 10) + '0';
  buf[5] = '\0';
  return buf;
}

static void show_result(const char *s, uint16_t fps) {
  // assign default color value
  u8g2.setColorIndex(draw_color);
  u8g2.setFont(u8g2_font_8x13B_tf);
  u8g2.firstPage();
  do {
    u8g2.drawStr(0,12, s);
    u8g2.drawStr(0,24, convert_FPS(fps));
  } while( u8g2.nextPage() );
}

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       1024
#define THREAD_TIMESLICE        5

static rt_thread_t tid1 = RT_NULL;

static void u8g2_fps_entry(void *parameter)
{
  u8g2.begin();
  draw_color = 1;         // pixel on
  while(1)
  {
    uint16_t fps;
    fps = picture_loop_with_fps(draw_clip_test);
    show_result("draw clip test", fps);
    rt_thread_mdelay(5000);
    fps = picture_loop_with_fps(draw_set_screen);
    show_result("clear screen", fps);
    rt_thread_mdelay(5000);
    fps = picture_loop_with_fps(draw_char);
    show_result("draw @", fps);
    rt_thread_mdelay(5000);
    fps = picture_loop_with_fps(draw_pixel);
    show_result("draw pixel", fps);
    rt_thread_mdelay(5000);
  }
}

static void u8g2_page_buffer_fps(int argc,char *argv[])
{
  tid1 = rt_thread_create("tu8g29",
                          u8g2_fps_entry, RT_NULL,
                          THREAD_STACK_SIZE,
                          THREAD_PRIORITY, THREAD_TIMESLICE);

  if (tid1 != RT_NULL)
    rt_thread_startup(tid1);
}
MSH_CMD_EXPORT(u8g2_page_buffer_fps, u8g2 page buffer clip window sample);
