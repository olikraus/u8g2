/*

  Arduboy.ino
  
  Arduboy Test Example with U8x8

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
#include <Arduboy.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

// Arduboy 10 (Production, Kickstarter Edition)
U8X8_SSD1306_128X64_NONAME_4W_HW_SPI u8x8(/* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6);	

ArduboyCore arduboyCore;
ArduboyAudio arduboyAudio;
ArduboyTunes arduboyTunes;
  
void setup(void)
{
  //u8x8.begin(/*Select=*/ A0, /*Right/Next=*/ 5, /*Left/Prev=*/ 9, /*Up=*/ 8, /*Down=*/ 10, /*Home/Cancel=*/ A1); // Arduboy DevKit
  u8x8.begin(/*Select=*/ 7, /*Right/Next=*/ A1, /*Left/Prev=*/ A2, /*Up=*/ A0, /*Down=*/ A3, /*Home/Cancel=*/ 8); // Arduboy 10 (Production)
  
  arduboyTunes.initChannel(PIN_SPEAKER_1);
  arduboyTunes.initChannel(PIN_SPEAKER_2);
  arduboyAudio.begin();
}

void msg(const char *txt)
{
  u8x8.clear();
  u8x8.print(txt);
}

void loop(void)
{
  static uint8_t c = 1;
  static uint8_t b = 0;
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  c = u8x8.userInterfaceSelectionList("Arduboy Test\n", c, "LED Test\nButton Test\nSpeaker Test");
  if ( c == 1 )
  {
    msg("LED Test");
    arduboyCore.setRGBled(255, 0, 0);
    delay(500);
    arduboyCore.setRGBled(0, 255, 0);    // green is not there ???
    delay(500);
    arduboyCore.setRGBled(0, 0, 255);
    delay(500);
    arduboyCore.setRGBled(0, 0, 0);
  }
  else if ( c == 2 )
  {
    u8x8.clear();
    for(;;)
    {
      u8x8.drawUTF8(0, 0, "Button Test");
      b = arduboyCore.buttonsState();
      
      u8x8.noInverse();
      if ( b & A_BUTTON )
        u8x8.inverse();
      u8x8.drawUTF8(0, 1, "A Button");
      
      u8x8.noInverse();
      if ( b & B_BUTTON )
        u8x8.inverse();
      u8x8.drawUTF8(0, 2, "B Button");

      u8x8.noInverse();
      if ( b & UP_BUTTON )
        u8x8.inverse();
      u8x8.drawUTF8(0, 3, "UP Button");

      u8x8.noInverse();
      if ( b & DOWN_BUTTON )
        u8x8.inverse();
      u8x8.drawUTF8(0, 4, "DOWN Button");

      u8x8.noInverse();
      if ( b & LEFT_BUTTON )
        u8x8.inverse();
      u8x8.drawUTF8(0, 5, "LEFT Button");

      u8x8.noInverse();
      if ( b & RIGHT_BUTTON )
        u8x8.inverse();
      u8x8.drawUTF8(0, 6, "RIGHT Button");

      u8x8.noInverse();
      u8x8.drawUTF8(0, 7, "Quit: A&B Button");

      if ( (b & A_BUTTON) &&  (b & B_BUTTON) )
        break;
    }
  }
  else if ( c == 3 )
  {
    u8x8.clear();
    u8x8.drawUTF8(0, 0, "Speaker Test");
    arduboyTunes.tone(1000, 1000);
    delay(1000);
    arduboyTunes.tone(2000, 1000);
    delay(1000);
  }
}
