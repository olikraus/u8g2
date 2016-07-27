/*

  A2Printer.ino
  
  This is a special example for the A2 Thermo Printer
  This example can NOT be used with LCDs and OLEDs

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

// use this serial interface
#define PRINTER_SERIAL Serial
// #define PRINTER_SERIAL Serial1


#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif



/* define our own serial function, this will be assigned later */

extern "C" uint8_t u8x8_byte_arduino_serial(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t *data;
 
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {      
	while(PRINTER_SERIAL.available() > 0) 
                PRINTER_SERIAL.read();      
	PRINTER_SERIAL.write((uint8_t)*data);
	data++;
	arg_int--;
      }
      break;
      
    case U8X8_MSG_BYTE_INIT:
      delay(1000);
      PRINTER_SERIAL.begin(19200);
      /* send something.... */
      //PRINTER_SERIAL.begin(9600);
      PRINTER_SERIAL.write(27);	/* ESC */
      PRINTER_SERIAL.write(64); /* @ --> reset printer */
      delay(1000);
      
      break;
      
    case U8X8_MSG_BYTE_SET_DC:
      break;
      
    case U8X8_MSG_BYTE_START_TRANSFER:  
      break;
      
    case U8X8_MSG_BYTE_END_TRANSFER:      
      break;
      
    default:
      return 0;
  }
  return 1;
}

/* define a u8g2 object just with the base class */
/* the baseclass will be configured for the A2 printer in the setup() procedure */
class U8G2 u8g2;

void setup(void) {

  u8g2_Setup_a2printer_384x240_1(		/* several different a2 printer procs exist... */
    u8g2.getU8g2(), 						/* this is a c call, so use the u8g2 struct instead of the class */
    U8G2_R0, 							/* this defines the usual rotation of the bitmap */
    u8x8_byte_arduino_serial, 				/* this is the new procedure from above */
    u8x8_gpio_and_delay_arduino);			/* standard arduino low level proc, used only for delay */

  u8g2.beginSimple();				/* Do not clear the display, wake is not required */  
}

#define XO 10

void drawLogo(void)
{
    
    u8g2.setFontMode(1);	// Transparent
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0+XO, 30, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21+XO,8,"8");
        
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51+XO,30,"g");
    u8g2.drawStr(67+XO,30,"\xb2");
    
    u8g2.drawHLine(2+XO, 35, 47);
    u8g2.drawHLine(3+XO, 36, 47);
    u8g2.drawVLine(45+XO, 32, 12);
    u8g2.drawVLine(46+XO, 33, 12);
}

void drawURL(void)
{
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.drawStr(1+XO,54,"github.com/olikraus/u8g2");
}

void loop(void) {
  u8g2.firstPage();
  do {
    drawLogo();
    drawURL();
    
    /* worst case heating test */
    u8g2.drawRBox(4,90,384-8,20, 8);
    
    /* heating test with some text */
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(10,160,"Hello World!");  
    
    /* draw a frame around the picture */
    u8g2.drawFrame(0,0,384,240);
    u8g2.drawFrame(1,1,384-2,240-2);
  } while ( u8g2.nextPage() );
  delay(1000);
}

