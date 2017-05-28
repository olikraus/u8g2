/*

  MAX7219_U8g2.ino
  
  A special example for the MAX7219 based LED display matrix

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

U8G2_MAX7219_32X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 11, /* data=*/ 12, /* cs=*/ 10, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);

uint16_t pos;
uint16_t len;
char *str = "    Hello from U8g2... Arduino monochrome graphics library    ";

void setup(void) {
  u8g2.begin();
  u8g2.setFont(u8g2_font_victoriabold8_8r);	// choose a 8x8 u8g2 font
  pos = 0;
  len = strlen(str)*8;
}

#define BUF_LEN 12
void draw_string_at_bitpos(const char *str, uint16_t bitpos)
{
  char buf[BUF_LEN];
  uint16_t start = bitpos / 8;
  uint16_t i;
  for( i = 0; i < BUF_LEN-1; i++)
  {
    buf[i] = str[start+i];
    if ( str[start+i] == '\0' )
      break;
  }
  buf[BUF_LEN-1] = '\0';
  u8g2.drawStr( -(bitpos & 7), 7, buf);			// write buffer  
}

void loop(void) {
  u8g2.clearBuffer();					// clear the internal memory
  draw_string_at_bitpos(str, pos);
  u8g2.sendBuffer();					// transfer internal memory to the display
  pos++;
  if ( pos >= len )
    pos = 0;
  delay(50);  
}

