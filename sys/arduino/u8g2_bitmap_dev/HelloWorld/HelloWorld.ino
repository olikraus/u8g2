/*

  HelloWorld.ino

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

/*
  This is a memory-backed bitmap rendering example
*/

#define WIDTH 128
#define HEIGHT 32

U8G2_BITMAP u8g2(WIDTH, HEIGHT, U8G2_R0);

void setup(void) {
  Serial.begin(115200);
  u8g2.begin();  
}

void loop(void) {
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_10x20_tr);	// choose a suitable font
  u8g2.drawStr(0,30,"Hello World!");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display

  for (unsigned y=0; y < HEIGHT; ++y) {
    Serial.print("|");
    for (unsigned x=0; x < WIDTH; ++x) {
      if (u8x8_GetBitmapPixel(u8g2.getU8x8(), x, y))
        Serial.print("X");
      else
        Serial.print(" ");
    }
    Serial.println("|");
  }
  for (unsigned x=0; x < WIDTH + 2; ++x)
    Serial.print("-");
  Serial.println();

  delay(1000);  
}
