/*

  I2CLCDBoard.ino
  
  https://github.com/olikraus/u8g2/issues/2191
  
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
  
  Example for the I2C LCD GFX Board https://github.com/olikraus/u8g2/issues/2191
  Required updates for T6963 and LC7981 :
      1. Update write strobe and chip select signals in "u8x8_byte_i2c_lcd_gfx_board_t6963_8080()"
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
//#define I2C_LCD_LED_DEFAULT 0

#define I2C_LCD_CTRL_ADR 0x3e
#define I2C_LCD_DATA_ADR 0x3f


/*
  Set a bit in the internal register and in the i2c control gpio
  The call "u8x8_set_ctrl_bit(u8x8, 0, 1);" will just refresh the gpio lines
  Typically the second argument will be one of the I2C_LCD_xxx macros
  
  Reusing the "i2c_started" member variable, which is not used by the i2c lcd board software (i2c_started is only used by sw emulated i2c) 
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
uint8_t u8x8_gpio_and_delay_i2c_lcd_gfx_board(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  uint8_t i;
  switch(msg)
  {
    case U8X8_MSG_GPIO_RESET:
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_RESET, arg_int);
      return 1;
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      u8x8->i2c_started = I2C_LCD_CS|I2C_LCD_RD|I2C_LCD_RESET|I2C_LCD_LED_DEFAULT; // assign default value for ctrl byte, reuse "i2c_started" member variable to store the gpio state
      u8x8_set_ctrl_bit(u8x8, 0, 1);   // just transmit the control byte (which is stored in "i2c_started" member variable)
      return 1;
  }
  return u8x8_gpio_and_delay_arduino(u8x8, msg, arg_int, arg_ptr);
}

/*
  This is an example byte level procedure for a T6963 board.
  This procedure will assume, that the T6963 is configured in 8080 mode.
  Probably the same procedure can be also used for LC7981 displays.
  
  In general chip select and write strobe signals may differ and need to be checked.
*/
uint8_t u8x8_byte_i2c_lcd_gfx_board_t6963_8080(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
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


/*
  This is an example byte level procedure for a LC7981 board.
  This procedure will assume, that the LC7981 is configured in 6800 mode.
  Probably the same procedure can be also used for T6963 displays.
  
  In general chip select and write strobe signals may differ and need to be checked.
*/
uint8_t u8x8_byte_i2c_lcd_gfx_board_lc7981_6800(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* usually only the following two varibables need to be modified */
  uint8_t rw_line = I2C_LCD_RW;
  uint8_t write_strobe = I2C_LCD_E; // 6800 mode
  uint8_t chip_select = I2C_LCD_CS;
  uint8_t *data;
 
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      u8x8_set_ctrl_bit(u8x8, rw_line, 0);
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
        u8x8_set_ctrl_bit(u8x8, write_strobe, 1);           
	//u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->write_pulse_width_ns);
        u8x8_set_ctrl_bit(u8x8, write_strobe, 0);            // in 6800 mode, data is taken over with the falling edge of the E data line
      }
      break;
      
    case U8X8_MSG_BYTE_INIT:
      Wire.begin();
      Wire.setClock(400000);    // Assumption: PCF8574A supports fast I2C mode

      /* disable chipselect */
      u8x8_set_ctrl_bit(u8x8, chip_select, u8x8->display_info->chip_disable_level);            // disable display controller

      /* low signal level for the R/W line */
      u8x8_set_ctrl_bit(u8x8, rw_line, 0);            // in 6800 mode, the R/W signal must be low for writing
      
      /* ensure that the enable signal is low  */
      u8x8_set_ctrl_bit(u8x8, write_strobe, 0);            // in 8080 mode, the RW signal is the /WR signal
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


/*
  This is an example byte level procedure for SED1520 / SBN1661 displays.
  The display needs to be configured in 6800 mode.
  
  In general chip select and write strobe signals may differ and need to be checked.
*/
uint8_t u8x8_byte_i2c_lcd_gfx_board_sed1520_6800(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t chip_select = I2C_LCD_CS;
  uint8_t *data;
  static uint8_t enable_pin = I2C_LCD_E;            // the enable pin may differ. The current enable pin number is stored here
 
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
	u8x8_set_ctrl_bit(u8x8, enable_pin, 0);
	//u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, 200); 
	//u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->write_pulse_width_ns); 
	u8x8_set_ctrl_bit(u8x8, enable_pin, 1);
      }
      break;
      
    case U8X8_MSG_BYTE_INIT:
      Wire.begin();
      Wire.setClock(400000);    // Assumption: PCF8574A supports fast I2C mode
      
      /* 6800 mode: Permanently assign R/W  to GND */
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_RW, 0);            // in 6800 mode, the R/W can 0 all the time (write mode only)

      /* chipselect */
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_CS, 0);            // there is no CS for the SED1520... but still, we apply a default value here
      
      /* ensure that the enable signal is high */
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_E, 0);            // clear E1
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_E2, 0);            // clear E2
      enable_pin = I2C_LCD_E;                                   // assign E1 as initial enable signal
      break;
      
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_CD, arg_int);            // command/data   data/command 
      break;
      
    case U8X8_MSG_BYTE_START_TRANSFER:
      /* arg_int is set by u8x8_d_sbn1661_122x32() function (which is also used by sed1520) */
      enable_pin = I2C_LCD_E;
      if ( arg_int != 0 )                                                       // the CAD procedure will tell, which enable signal to use
	enable_pin = I2C_LCD_E2;
      break;
      
    case U8X8_MSG_BYTE_END_TRANSFER:
      break;
      
    default:
      return 0;
  }
  return 1;
}


/*
  This is an example byte level procedure for a ST7920 display board.
  This procedure will put the ST7920 in parallel mode.
  The ST7920 must be connected to J3 of the I2C GFX Board.
  See also: https://github.com/olikraus/u8g2/issues/2191
*/
uint8_t u8x8_byte_i2c_lcd_gfx_board_st7920(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* usually only the following two varibables need to be modified */
  uint8_t read_write = I2C_LCD_RW; /* Must be permanently 0 for the ST7920 to use the write mode */
  uint8_t enable_strobe = I2C_LCD_E; /* Used to latch in data for the ST7920 */
  uint8_t psb_select = I2C_LCD_CS;  /* CS is connected to PSB at J3, so CS must be permanently 1 for parallel mode */
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
        u8x8_set_ctrl_bit(u8x8, enable_strobe, 1);            // in 8080 mode, the RW signal is the /WR signal
	//u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->write_pulse_width_ns);
        u8x8_set_ctrl_bit(u8x8, enable_strobe, 0);            // in 8080 mode, the RW signal is the /WR signal
      }
      break;
      
    case U8X8_MSG_BYTE_INIT:
      Wire.begin();
      Wire.setClock(400000);    // Assumption: PCF8574A supports fast I2C mode

      /* ensure that the enable signal is low (ST7920 datasheet) */
      u8x8_set_ctrl_bit(u8x8, enable_strobe, 0);            // ST7920: Generate low-high-low pulse for data transfer, so start with 0
      u8x8_set_ctrl_bit(u8x8, read_write, 0);            // ST7920: must be permanently low for write mode
      u8x8_set_ctrl_bit(u8x8, psb_select, 1);            // ST7920: This is actually connected to PSB on J1, so make this permanently 1 for parallel mode
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_CD, 0);            // ST7920: Command Mode
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_set_ctrl_bit(u8x8, I2C_LCD_CD, arg_int);            // command/data   data/command 
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      //u8x8_set_ctrl_bit(u8x8, chip_select, u8x8->display_info->chip_enable_level);            // command/data   data/command 
      //u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      //u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      //u8x8_set_ctrl_bit(u8x8, chip_select, u8x8->display_info->chip_disable_level);            // command/data   data/command 
      break;
    default:
      return 0;
  }
  return 1;
}



U8G2 u8g2;              // create a plain U8g2 object

void setup(void) 
{
  /*
    Do a manual update of the plain U8g2 object with the C API for the display.
    Use a special byte function, which includes the support for the I2C LCD GFX boad.
    
    The C init functions are listed here: https://github.com/olikraus/u8g2/wiki/u8g2setupc
    Just replace the function name accordingly.    
    
    Note:
      For T6963 / ST7920 displays, use the "u8x8_byte_i2c_lcd_gfx_board_t6963_8080" callback function.
      For LC7981 displays, use the "u8x8_byte_i2c_lcd_gfx_board_lc7981_6800" callback function.
      For SBN1661 / SED1520 use the "u8x8_byte_i2c_lcd_gfx_board_sed1520_6800" callback function.
    
  */
  //u8g2_Setup_t6963_240x128_2(u8g2.getU8g2(), U8G2_R0, u8x8_byte_i2c_lcd_gfx_board_t6963_8080, u8x8_gpio_and_delay_i2c_lcd_gfx_board);   // J4
  //u8g2_Setup_t6963_128x128_2(u8g2.getU8g2(), U8G2_R0, u8x8_byte_i2c_lcd_gfx_board_t6963_8080, u8x8_gpio_and_delay_i2c_lcd_gfx_board);  // J2
  //u8g2_Setup_sed1520_122x32_2(u8g2.getU8g2(), U8G2_R0, u8x8_byte_i2c_lcd_gfx_board_sed1520_6800, u8x8_gpio_and_delay_i2c_lcd_gfx_board);  // J5
  //u8g2_Setup_st7920_128x64_2(u8g2.getU8g2(), U8G2_R0, u8x8_byte_i2c_lcd_gfx_board_st7920, u8x8_gpio_and_delay_i2c_lcd_gfx_board);           // J3
  u8g2_Setup_lc7981_240x128_2(u8g2.getU8g2(), U8G2_R0, u8x8_byte_i2c_lcd_gfx_board_lc7981_6800, u8x8_gpio_and_delay_i2c_lcd_gfx_board);           // J3
  
  /*
    After providing the callback function, start the communication with the display by using u8g2.begin()
  */
  u8g2.begin();  
}


void drawLogo(void)
{
  uint8_t mdy = 0;
  if ( u8g2.getDisplayHeight() < 59 )
    mdy = 5;
  

    u8g2.setFontMode(1);	// Transparent
    u8g2.setDrawColor(1);
#ifdef MINI_LOGO

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(0, 22, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb19_mn);
    u8g2.drawStr(14,8,"8");
    
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(36,22,"g");
    u8g2.drawStr(48,22,"\xb2");
    
    u8g2.drawHLine(2, 25, 34);
    u8g2.drawHLine(3, 26, 34);
    u8g2.drawVLine(32, 22, 12);
    u8g2.drawVLine(33, 23, 12);
#else

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0, 30-mdy, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21,8-mdy,"8");
        
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51,30-mdy,"g");
    u8g2.drawStr(67,30-mdy,"\xb2");
    
    u8g2.drawHLine(2, 35-mdy, 47);
    u8g2.drawHLine(3, 36-mdy, 47);
    u8g2.drawVLine(45, 32-mdy, 12);
    u8g2.drawVLine(46, 33-mdy, 12);
    
#endif

}

void drawURL(void)
{
#ifndef MINI_LOGO
  u8g2.setFont(u8g2_font_4x6_tr);
  if ( u8g2.getDisplayHeight() < 59 )
  {
    u8g2.drawStr(89,20-5,"github.com");
    u8g2.drawStr(73,29-5,"/olikraus/u8g2");
  }
  else
  {
    u8g2.drawStr(1,54,"github.com/olikraus/u8g2");
  }
#endif
}


uint8_t x = 0; 
long t = 0;
long tt = 0;

void loop(void) 
{
  t = millis();
  u8g2.firstPage();
  do 
  {
    u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight() );
    drawLogo();
    drawURL();
  
    //u8g2.setFont(u8g2_font_ncenB18_tr);
    //u8g2.drawStr(x*2,19,"Hello World!");
    u8g2.setFont(u8g2_font_4x6_tr);
    u8g2.setCursor(x*2, 62);
    u8g2.print(tt, DEC);
    
  } while ( u8g2.nextPage() );
  x++;
  x &= 15;
  tt = millis() - t;
}

