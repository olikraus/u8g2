/*

  I2CLCDBoard.ino

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2023, olikraus@gmail.com
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
#include <Wire.h>


/*
  
  Example for the I2C GFX Board https://github.com/olikraus/u8g2/issues/2191
  Required updates for T6963 and LC7981 :
      1. Update write strobe and chip select signals in "u8x8_byte_i2c_lcd_board()"
      2. Update the setup procedure in "setup()" 
    
    
  Control Signal List for I2C Port Expander @0x3e:
      Bit 0: C/D
      Bit 1: R/W
      Bit 2: E
      Bit 3: E2
      Bit 4: /CS
      Bit 5: /RD
      Bit 6: /Reset
      Bit 7: LED
*/

#define I2C_LCD_CD 1
#define I2C_LCD_RW 2
#define I2C_LCD_E 4
#define I2C_LCD_E2 8
#define I2C_LCD_CS 16
#define I2C_LCD_RD 32
#define I2C_LCD_RESET 64
#define I2C_LCD_LED 128

/* default value for the LED control gpio line */
#define I2C_LCD_LED_DEFAULT I2C_LCD_LED

#define I2C_LCD_CTRL_ADR 0x3e
#define I2C_LCD_DATA_ADR 0x3f


/*
  set a bit in the internal register and in the i2c control gpio
  only update operation will be u8x8_set_ctrl_bit(u8x8, 0, 1);
  typically the second argument will be one of the I2C_LCD_xxx macros
  
  reusing the i2c_started member, which is not used by the i2c lcd board software (i2c_started is only used by sw emulated i2c) 
*/
void u8x8_set_ctrl_bit(u8x8_t *u8x8, uint8_t bit, uint8_t is_set) U8X8_NOINLINE;
void u8x8_set_ctrl_bit(u8x8_t *u8x8, uint8_t bit, uint8_t is_set)
{
  if ( is_set )
    u8x8->i2c_started |= (uint8_t)bit;          // reused i2c_started to store the gpio state
  else
    u8x8->i2c_started &= ~(uint8_t)bit;           // reused i2c_started to store the gpio state

  Wire.beginTransmission(I2C_LCD_CTRL_ADR); 
  Wire.write(u8x8->i2c_started);             // Sends value byte
  Wire.endTransmission();      // Stop transmitting
}

/*
  The new gpio&delay procedure extents the existing arduino procedure and adds proper reset handling
*/
uint8_t u8x8_gpio_and_delay_i2c_lcd_board(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  uint8_t i;
  switch(msg)
  {
    case U8X8_MSG_GPIO_RESET:
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_RESET, arg_int);
      return 1;
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      u8x8->i2c_started = I2C_LCD_CS|I2C_LCD_RD|I2C_LCD_RESET|I2C_LCD_LED_DEFAULT; // reused i2c_started to store the gpio state
      u8x8_set_ctrl_bit(u8x8, 0, 1);   // just transmit the control byte
      return 1;
  }
  return u8x8_gpio_and_delay_arduino(u8x8, msg, arg_int, arg_ptr);
}

/*
  This is an example byte level procedure for my T6963 board.
  Chips select and write strobe signals may differ.
*/
uint8_t u8x8_byte_i2c_lcd_board(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* usually only the following two varibables need to be modified */
  uint8_t write_strobe = I2C_LCD_RW; /* T6963 in 8080mode */
  uint8_t chip_select = I2C_LCD_CS;
  uint8_t *data;
 
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {
        Wire.beginTransmission(I2C_LCD_DATA_ADR); 
        Wire.write(*data);
        Wire.endTransmission();
        
	data++;
	arg_int--;
        	
        // Assumption: u8x8_set_ctrl_bit() will be slow enough, so that no extra delay is required
	//u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->data_setup_time_ns);
        u8x8_set_ctrl_bit(u8x8, write_strobe, 0);            // in 8080 mode, the RW signal is the /WR signal
	//u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->write_pulse_width_ns);
        u8x8_set_ctrl_bit(u8x8, write_strobe, 1);            // in 8080 mode, the RW signal is the /WR signal
      }
      break;
      
    case U8X8_MSG_BYTE_INIT:
      Wire.begin();
      Wire.setClock(400000);    // Assumption: PCF8574A supports fast I2C mode

      /* disable chipselect */
      u8x8_set_ctrl_bit(u8x8, chip_select, u8x8->display_info->chip_disable_level);            // disable display controller
      /* ensure that the enable signal is high */
      u8x8_set_ctrl_bit(u8x8, write_strobe, 1);            // in 8080 mode, the RW signal is the /WR signal
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_CD, arg_int);            // command/data   data/command 
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_set_ctrl_bit(u8x8, chip_select, u8x8->display_info->chip_enable_level);            // command/data   data/command 
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_set_ctrl_bit(u8x8, chip_select, u8x8->display_info->chip_disable_level);            // command/data   data/command 
      break;
    default:
      return 0;
  }
  return 1;
}



U8G2 u8g2;              // create a plain U8g2 object

void setup(void) {
  /*
    Do a manual update of the plain U8g2 object with the C API for the display.
    Use a special byte function, which includes the support for the I2C GFX boad.
    
    The C init functions are listed here: https://github.com/olikraus/u8g2/wiki/u8g2setupc
    Just replace the function name accordingly.    
  */
  u8g2_Setup_t6963_240x128_2(u8g2.getU8g2(), U8G2_R0, u8x8_byte_i2c_lcd_board, u8x8_gpio_and_delay_i2c_lcd_board);
  u8g2.begin();  
}

uint8_t x = 0; 
long t = 0;
long tt = 0;

void loop(void) {
  t = millis();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB24_tr);
    u8g2.drawStr(x*2,32,"Hello World!");
    u8g2.drawStr(x*2,64,"U8g2");
    u8g2.setCursor(x*2, 96);
    u8g2.print(tt, DEC);
  } while ( u8g2.nextPage() );
  x++;
  x &= 15;
  tt = millis() - t;
}

