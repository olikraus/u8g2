/*

  AirQuality.ino

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
#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_SGP30.h>
#include <Adafruit_SHT31.h>




U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Adafruit_SHT31 sht31 = Adafruit_SHT31();	// temperature & humidity sensor
Adafruit_SGP30 sgp;						// air quality sensor

U8G2LOG u8g2log;

// setup the terminal (U8G2LOG) and connect to u8g2 for automatic refresh of the display
// The size (width * height) depends on the selected font and the display

#define U8LOG_WIDTH 25
#define U8LOG_HEIGHT 9
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];



void setup(void) {
  u8g2.begin();  
  
  u8g2.setFont(u8g2_font_5x7_mr);	// set the font for the terminal window
  u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8g2log.setLineHeightOffset(0);	// set extra space between lines in pixel, this can be negative
  u8g2log.setRedrawMode(0);		// 0: Update screen with newline, 1: Update screen for every char  
  
  u8g2log.print(F("Air Quality\n"));
  if (sht31.begin(0x44)) {   				// 0x45 for alternate i2c addr
    u8g2log.print(F("SHT31 found at 0x44\n"));
  } else {
    u8g2log.print(F("SHT31 not found\n"));
    while (1);
  }

  if (sgp.begin()) {   				
    u8g2log.print(F("SGP30 found\n"));
  } else {
    u8g2log.print(F("SGP30 not found\n"));
    while (1);
  }
  
  u8g2log.print(F("SGP #"));
  u8g2log.print(sgp.serialnumber[0], HEX);
  u8g2log.print(sgp.serialnumber[1], HEX);
  u8g2log.println(sgp.serialnumber[2], HEX);

  delay(4000);
}

/* 
  Calculate absolute humidity [mg/m^3]
  Args: Temperature [°C], humidity [%RH]
*/

uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}


float temperature_raw;
float humidity_raw;
int32_t tvoc_raw;
int32_t tvoc_lp;
int32_t eco2_raw;
int32_t eco2_lp;
int is_air_quality_available = 0;

uint16_t tvoc_graph[128];
uint16_t eco2_graph[128];
uint16_t eco2_min;
uint16_t eco2_max;

void startAirQuality(void)
{
}

void readAirQuality(void)
{
  uint8_t i;
  int32_t tvoc_coeff = 7;	// 1..32
  int32_t eco2_coeff = 7;	// 1..32
  
  temperature_raw = sht31.readTemperature();
  humidity_raw = sht31.readHumidity();

  sgp.setHumidity(getAbsoluteHumidity(temperature_raw, humidity_raw));
  is_air_quality_available = sgp.IAQmeasure();

  if ( is_air_quality_available )
  {
    tvoc_raw = sgp.TVOC;
    eco2_raw = sgp.eCO2;
    
    tvoc_lp = tvoc_lp + ((tvoc_coeff * ( tvoc_raw - tvoc_lp )) >> 5);
    eco2_lp = eco2_lp + ((eco2_coeff * ( eco2_raw - eco2_lp )) >> 5);
    if ( eco2_lp < 400 )
      eco2_lp = 400;
    
    eco2_min = 0x0ffff;
    eco2_max = 0;
    for( i =1; i < 128; i++ )
    {
      tvoc_graph[i-1] = tvoc_graph[i];
      eco2_graph[i-1] = eco2_graph[i];
      eco2_min = eco2_min < eco2_graph[i] ? eco2_min : eco2_graph[i];
      eco2_max = eco2_max > eco2_graph[i] ? eco2_max : eco2_graph[i];
    }
    tvoc_graph[127] = tvoc_lp;
    eco2_graph[127] = eco2_lp;
  }
  
}


uint16_t eco2_base;
uint16_t tvoc_base;

void loop(void) {
  uint8_t i;
  u8g2_uint_t g;
  /*
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  int iaq;
  
  sgp.setHumidity(getAbsoluteHumidity(t, h));
  delay(500);
  iaq = sgp.IAQmeasure();
  delay(500);
  */
  
  
  readAirQuality();
  
  sgp.getIAQBaseline(&eco2_base, &tvoc_base);

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.setCursor(0, 10);
    u8g2.print(temperature_raw);
    u8g2.setCursor(64, 10);
    u8g2.print(humidity_raw);

    u8g2.setCursor(0, 20);
    u8g2.print(eco2_base, HEX);
    u8g2.print(" ");
    u8g2.print(tvoc_base, HEX);

    if ( is_air_quality_available )
    {
      u8g2.setCursor(0, 30);
      u8g2.print(tvoc_raw);
      u8g2.print(" ");
      u8g2.print(tvoc_lp);
      u8g2.print(" / ");
      u8g2.print(eco2_raw);
      u8g2.print(" ");
      u8g2.print(eco2_lp);
      
      eco2_max ++;
      for( i = 0; i < 128; i++ )
      {
	g = 32UL*((uint32_t)eco2_graph[i]-(uint32_t)400) / ( (uint32_t)eco2_max - (uint32_t)400);
	u8g2.drawPixel(i, 63-g );
      }
      
    }
  } while ( u8g2.nextPage() );
}

