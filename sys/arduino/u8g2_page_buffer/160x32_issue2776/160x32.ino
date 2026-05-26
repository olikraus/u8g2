/*

  160x32.ino

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2026, olikraus@gmail.com
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

U8G2_SBN1661_80X32_1_6800 lcd1(U8G2_R0, 11, 10, 9, 8, 7, 6, 5, 4, /*enable=*/ 2, /*cs=*/ A1, /*dc=*/ 13, /*reset=*/ 3); // Set R/W to low!
U8G2_SBN1661_80X32_1_6800 lcd2(U8G2_R0, 11, 10, 9, 8, 7, 6, 5, 4, /*enable=*/ 2, /*cs=*/ A2, /*dc=*/ 13, /*reset=*/ U8X8_PIN_NONE); // Reset is already applied with lcd1

void setup(void) {

  /* U8g2 Project: SBN1661 160x32 LCD */
  pinMode(A0, OUTPUT);  // 6800 Mode: R/W must be low
  digitalWrite(A0, 0);

  /* the 160x32 LCD contains two 80x32 LCDs */
  lcd1.begin();  
  lcd2.begin();  
}

//#define MINI_LOGO

void drawLogo(U8G2 &u8g2, u8g2_uint_t offset)
{
  uint8_t mdy = 0;
  if ( u8g2.getDisplayHeight() < 59 )
    mdy = 5;
  

    u8g2.setFontMode(1);	// Transparent
    u8g2.setDrawColor(1);
#ifdef MINI_LOGO

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(0-offset, 22, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb19_mn);
    u8g2.drawStr(14-offset,8,"8");
    
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(36-offset,22,"g");
    u8g2.drawStr(48-offset,22,"\xb2");
    
    u8g2.drawHLine(2-offset, 25, 34);
    u8g2.drawHLine(3-offset, 26, 34);
    u8g2.drawVLine(32-offset, 22, 12);
    u8g2.drawVLine(33-offset, 23, 12);
#else

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0-offset, 30-mdy, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21-offset,8-mdy,"8");
        
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51-offset,30-mdy,"g");
    u8g2.drawStr(67-offset,30-mdy,"\xb2");
    
    u8g2.drawHLine(2-offset, 35-mdy, 47);
    u8g2.drawHLine(3-offset, 36-mdy, 47);
    u8g2.drawVLine(45-offset, 32-mdy, 12);
    u8g2.drawVLine(46-offset, 33-mdy, 12);

    
    //u8g2.setFont(u8g2_font_logisoso58_tr);
    //u8g2.setFontDirection(0);
    //u8g2.drawStr(20,250,"300x400");

#endif

}

void drawURL(U8G2 &u8g2, u8g2_uint_t offset)
{
#ifndef MINI_LOGO
  u8g2.setFont(u8g2_font_4x6_tr);
  if ( u8g2.getDisplayHeight() < 59 )
  {
    u8g2.drawStr(89-offset,20-5,"github.com");
    u8g2.drawStr(73-offset,29-5,"/olikraus/u8g2");
  }
  else
  {
    u8g2.drawStr(1-offset,54,"github.com/olikraus/u8g2");
  }
#endif
}


void draw(U8G2 &u8g2, u8g2_uint_t offset) {
  u8g2.firstPage();
  do {
    //u8g2.setFont(u8g2_font_ncenB14_tr);
    //u8g2.drawStr(20-offset, 24,"Hello World!");         // depending on the offset, draw the left or right picture
    drawLogo(u8g2, offset);
    drawURL(u8g2, offset);
  } while ( u8g2.nextPage() );
}

void loop(void) {
  draw(lcd1, 0);                // left LCD starts at 0
  draw(lcd2, 80);               // right LCD starts at offset 80
  delay(1000);
}


