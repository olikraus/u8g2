/*

  PeltierCooler.ino

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

/*
  This is a small Peltier Cooler project for the ESP C3 with OLED

  GPIO 0: Temp. Sensor
  GPIO 2: MOSFET Gate
  GPIO 5/6: 72x40 OLED
*/


#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#define MOSFET_GPIO 2

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);

int32_t temp_raw = 0;
int32_t temp_lp = 0;
int32_t temp_lp_state = 0;
int32_t temp_celsius = 0;

/* low pass filter with 8 bit resolution, p = 0..255 */
#define LOW_PASS_BITS 8
int32_t low_pass(int32_t *a, int32_t x, int32_t p)
{
  int32_t n;
  n = ((1<<LOW_PASS_BITS)-p) * (*a) + p * x ;
  n >>= LOW_PASS_BITS;
  *a = n;
  return n;
}


void setup(void) {
  ledcAttach(MOSFET_GPIO, 20000, 8); // 20kHz, 8 Bit Resolution
  u8g2.begin();
}


void loop(void) {
  //temp_raw = analogRead(0);
  temp_raw = analogReadMilliVolts(0);
  temp_lp = low_pass(&temp_lp_state, temp_raw, 17);   // last value between 0 and 255, low values --> more filter
  temp_celsius = ((1035 - temp_lp)*100) / 55;   // 1/10 degree

  if ( temp_celsius > 230 )
     ledcWrite(MOSFET_GPIO, 128 + (temp_celsius-230)*2);
  else
     ledcWrite(MOSFET_GPIO, 0);



  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_helvB12_te);	// choose a suitable font
  //u8g2.drawStr(0,25,"ESP32");	// write something to the internal memory
  u8g2.drawStr(0,18, u8x8_u16toa(temp_lp, 4));
  u8g2.drawStr(0,38, u8x8_u16toa(temp_celsius, 4));
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(100);  
}

