/*

  u8x8_d_st7301.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2024, olikraus@gmail.com
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


  ST7301: 
    Ultra-Low Power Active Matrix 240 x 320
    Mono TFT Display Driver with Controller
  
  https://github.com/olikraus/u8g2/issues/2436
  
    No Hardware Flip
    No U8x8 Support

*/


#include "u8x8.h"
#include <string.h>

static const uint8_t u8x8_d_st7301_122x250_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x29), 				// display on
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7301_122x250_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x028),		                /* display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7301_122x250_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_CA(0x36, 0x60), 			// Memory Control 
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7301_122x250_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_CA(0x36, 0xa0), 			// Memory Control 
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};




/*===================================================*/
/* see also: http://www.technoblogy.com/show?3YB0 */


#ifdef NOT_USED
static uint8_t *u8x8_st7301_convert_60(u8x8_t *u8x8, uint8_t *p)   
{
  static uint8_t buf[6];
  uint8_t bytes_per_row = u8x8->display_info->tile_width;
  
  
  memset(buf, 0, 6);
  
  //   U8X8_CA(0x36, 0x60), 			// Memory Control 


  // first row, left 12 pixel
  
  if ( p[0] & 0x80 )
    buf[2] |= 0x02;
  if ( p[0] & 0x40 )
    buf[2] |= 0x08;
  if ( p[0] & 0x20 )
    buf[2] |= 0x20;
  if ( p[0] & 0x10 )
    buf[2] |= 0x80;

  if ( p[0] & 0x08 )
    buf[1] |= 0x02;
  if ( p[0] & 0x04 )
    buf[1] |= 0x08;
  if ( p[0] & 0x02 )
    buf[1] |= 0x20;
  if ( p[0] & 0x01 )
    buf[1] |= 0x80;

  if ( p[1] & 0x80 )
    buf[0] |= 0x02;
  if ( p[1] & 0x40 )
    buf[0] |= 0x08;
  if ( p[1] & 0x20 )
    buf[0] |= 0x20;
  if ( p[1] & 0x10 )
    buf[0] |= 0x80;

 // first row, right 12 pixel
   
  if ( p[1] & 0x08 )
    buf[5] |= 0x02;
  if ( p[1] & 0x04 )
    buf[5] |= 0x08;
  if ( p[1] & 0x02 )
    buf[5] |= 0x20;
  if ( p[1] & 0x01 )
    buf[5] |= 0x80;

  if ( p[2] & 0x80 )
    buf[4] |= 0x02;
  if ( p[2] & 0x40 )
    buf[4] |= 0x08;
  if ( p[2] & 0x20 )
    buf[4] |= 0x20;
  if ( p[2] & 0x10 )
    buf[4] |= 0x80;
  
  if ( p[2] & 0x08 )
    buf[3] |= 0x02;
  if ( p[2] & 0x04 )
    buf[3] |= 0x08;
  if ( p[2] & 0x02 )
    buf[3] |= 0x20;
  if ( p[2] & 0x01 )
    buf[3] |= 0x80;
  
  p += u8x8->display_info->tile_width;

  // second row, left 12 pixel
  
  if ( p[0] & 0x80 )
    buf[2] |= 0x01;
  if ( p[0] & 0x40 )
    buf[2] |= 0x04;
  if ( p[0] & 0x20 )
    buf[2] |= 0x10;
  if ( p[0] & 0x10 )
    buf[2] |= 0x40;

  if ( p[0] & 0x08 )
    buf[1] |= 0x01;
  if ( p[0] & 0x04 )
    buf[1] |= 0x04;
  if ( p[0] & 0x02 )
    buf[1] |= 0x10;
  if ( p[0] & 0x01 )
    buf[1] |= 0x40;

  if ( p[1] & 0x80 )
    buf[0] |= 0x01;
  if ( p[1] & 0x40 )
    buf[0] |= 0x04;
  if ( p[1] & 0x20 )
    buf[0] |= 0x10;
  if ( p[1] & 0x10 )
    buf[0] |= 0x40;

 // second row, right 12 pixel
   
  if ( p[1] & 0x08 )
    buf[5] |= 0x01;
  if ( p[1] & 0x04 )
    buf[5] |= 0x04;
  if ( p[1] & 0x02 )
    buf[5] |= 0x10;
  if ( p[1] & 0x01 )
    buf[5] |= 0x40;

  if ( p[2] & 0x80 )
    buf[4] |= 0x01;
  if ( p[2] & 0x40 )
    buf[4] |= 0x04;
  if ( p[2] & 0x20 )
    buf[4] |= 0x10;
  if ( p[2] & 0x10 )
    buf[4] |= 0x40;
  
  if ( p[2] & 0x08 )
    buf[3] |= 0x01;
  if ( p[2] & 0x04 )
    buf[3] |= 0x04;
  if ( p[2] & 0x02 )
    buf[3] |= 0x10;
  if ( p[2] & 0x01 )
    buf[3] |= 0x40;
  
  return buf;
}
#endif

static uint8_t *u8x8_st7301_convert_a0(u8x8_t *u8x8, uint8_t *p)   
{
  static uint8_t buf[6];
  static uint8_t map1[16] = {
      /* 0x00 0000 */ 0,
      /* 0x01 0001 */0x01,
      /* 0x02 0010 */0x04,
      /* 0x03 0011 */0x04+0x01,
      /* 0x04 0100 */0x10,
      /* 0x05 0101 */0x10+0x01,
      /* 0x06 0110 */0x10+0x04,
      /* 0x07 0111 */0x10+0x04+0x01,
      /* 0x08 1000 */ 0x40,
      /* 0x09 1001 */ 0x40+0x01,
      /* 0x0a 1010 */ 0x40+0x04,
      /* 0x0b 1011 */ 0x40+0x04+0x01,
      /* 0x0c 1100 */ 0x40+0x10,
      /* 0x0d 1101 */ 0x40+0x10+0x01,
      /* 0x0e 1110 */ 0x40+0x10+0x04,
      /* 0x0f 1111 */  0x40+0x10+0x04+0x01
  };
  static uint8_t map2[16] = {
      /* 0x00 0000 */ 0,
      /* 0x01 0001 */0x02,
      /* 0x02 0010 */0x08,
      /* 0x03 0011 */0x08+0x02,
      /* 0x04 0100 */0x20,
      /* 0x05 0101 */0x20+0x02,
      /* 0x06 0110 */0x20+0x08,
      /* 0x07 0111 */0x20+0x08+0x02,
      /* 0x08 1000 */ 0x80,
      /* 0x09 1001 */ 0x80+0x02,
      /* 0x0a 1010 */ 0x80+0x08,
      /* 0x0b 1011 */ 0x80+0x08+0x02,
      /* 0x0c 1100 */ 0x80+0x20,
      /* 0x0d 1101 */ 0x80+0x20+0x02,
      /* 0x0e 1110 */ 0x80+0x20+0x08,
      /* 0x0f 1111 */  0x80+0x20+0x08+0x02
  };
  
  
  memset(buf, 0, 6);
  
  //   U8X8_CA(0x36, 0x0), 			// Memory Control 


  // first row, left 12 pixel

  buf[0] |= map1[p[0]>>4];
  buf[1] |= map1[p[0] & 0x0f];
  buf[2] |= map1[p[1]>>4];

 // first row, right 12 pixel
   
  buf[3] |= map1[p[1] & 0x0f];
  buf[4] |= map1[p[2]>>4];
  buf[5] |= map1[p[2] & 0x0f];
  
  p += u8x8->display_info->tile_width;

  // second row, left 12 pixel

  buf[0] |= map2[p[0]>>4];
  buf[1] |= map2[p[0] & 0x0f];
  buf[2] |= map2[p[1]>>4];
  
 // second row, right 12 pixel

  buf[3] |= map2[p[1] & 0x0f];
  buf[4] |= map2[p[2]>>4];
  buf[5] |= map2[p[2] & 0x0f];
  return buf;
}




/*===================================================*/
/* 
  see also:
  https://github.com/zhcong/ST7302-for-arduino/blob/c9390fabcacefe7c36a113cd3e62959418c13b97/libraries/ST7302SPI/ST7302SPI.cpp#L21
*/
static const uint8_t u8x8_d_st7301_122x250_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  U8X8_C(0x01),                                 // software reset
  U8X8_DLY(100),
  U8X8_C(0x28), 				// display off
  U8X8_CAA(0xC7, 0x26, 0xE9), 			// disable OSC
  U8X8_CA(0xD1, 0x00), 			// Booster disable
  U8X8_DLY(20),
  U8X8_C(0x10),                                 // sleep in: enter sleep mode
  U8X8_DLY(20),
  U8X8_C(0x01),                                 // software reset
  U8X8_DLY(20),                                 // wait
  U8X8_C(0x38), 				// High Power Mode
  U8X8_CA(0xEB, 0x02), 			// Disable NVM Load
  U8X8_CA(0xD7, 0x68), 			// NVM Load Control: Enable ID1 ID2 ID3 Load
  U8X8_CA(0xD1, 0x01), 			// Booster Enable
  U8X8_CA(0xC0, 0x80), 			// Gate Voltage Setting VGH=12V (upper 4 bit, 8V-15V); VGL=-5V (lower 4 bit, -5V .. -10V)
  U8X8_C(0x0C1),                                // Source Voltage Control 1
  U8X8_A6(0x28,0x28,0x28,0x28,0x14,0x00),       // Source high voltage in reflective and transmissive mode, gamma voltage 1&2 
  U8X8_C(0x0C2),                                // Source Voltage Control 1
  U8X8_A4(0x00,0x00,0x00,0x00),       // Source low voltage in reflective and transmissive mode 
  U8X8_CA(0xCB, 0x14), 			// VCOMH: 0x14 = 4V (0x28 = 5V)
  U8X8_CAA(0xB4, 0xE5, 0x77), 			// Update Period Gate EQ Control, why 0x77??? it should be 0x66 according to the datasheet
  U8X8_A8(0xF1, 0xFF, 0xFF, 0x4F, 0xF1, 0xFF, 0xFF, 0X4F),
  U8X8_CA(0xB0, 0x64), 			// Duty Cycle... this must be before sleep out
  U8X8_C(0x11),                                 // sleep out: furn off sleep mode
  U8X8_DLY(120),
  U8X8_CAA(0xC7, 0xA6, 0xE9), 			// Enable OSC
  U8X8_CA(0x36, 0xa0), 			// Memory Control 
  
  U8X8_CA(0x3A, 0x11), 			// Data Format 
  U8X8_CA(0xB9, 0x23), 			// Source Setting: Clear RAM off 
  U8X8_CA(0xB8, 0x09), 			// Panel Setting / Panel Layout 
  U8X8_CAA(0x2A, 0x05, 0x36), 			// col addr
  U8X8_CAA(0x2B, 0x00, 0xC7), 			// row addr
  U8X8_CA(0xD0, 0x1F), 			// Not in datasheet
  U8X8_C(0x29), 				// display on
  U8X8_CA(0x72, 0x00), 			// Not in datasheet
  U8X8_CAA(0xB2,1, 5),                  // Frame Rate for High and Low Power Mode (hier: 32Hz and 8Hz) 
  U8X8_C(0x39), 				// enable Low Power Mode...: 8Hz, see above
  U8X8_DLY(100),

  /*
  U8X8_CAA(0x2A, 0x19, 0x3a), 			// col addr  0x14 < col < 0x3b
  U8X8_CAA(0x2B, 115, 115), 			// row addr (0..250, y in u8g2), 0 is top row in u8g2
  U8X8_C(0x2C), 			// write start
  U8X8_D1(0xff), 			// pixel data
  U8X8_D1(0xff), 			// pixel data
  U8X8_D1(0xff), 			// pixel data
  
  U8X8_D1(0xff), 			// pixel data
  U8X8_D1(0xff), 			// pixel data
  U8X8_D1(0xff), 			// pixel data

  U8X8_D1(0xff), 			// pixel data
  U8X8_D1(0xff), 			// pixel data
  U8X8_D1(0xff), 			// pixel data
  
  U8X8_D1(0xff), 			// pixel data
  U8X8_D1(0xff), 			// pixel data
  U8X8_D1(0xff), 			// pixel data

  U8X8_CAA(0x2A, 0x19, 0x3a), 			// col addr  0x14 < col < 0x3b
  U8X8_CAA(0x2B, 116, 116), 			// row addr (0..250, y in u8g2), 0 is top row in u8g2
  U8X8_C(0x2C), 			// write start
  U8X8_D1(0x80), 			// pixel data
  U8X8_D1(0x00), 			// pixel data
  U8X8_D1(0x00), 			// pixel data
  
  U8X8_D1(0x00), 			// pixel data
  U8X8_D1(0x00), 			// pixel data
  U8X8_D1(0x00), 			// pixel data

  U8X8_D1(0x00), 			// pixel data
  U8X8_D1(0x00), 			// pixel data
  U8X8_D1(0x00), 			// pixel data
  
  U8X8_D1(0x00), 			// pixel data
  U8X8_D1(0x00), 			// pixel data
  U8X8_D1(0x00), 			// pixel data
  */

  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()           			/* end of sequence */
};




static const u8x8_display_info_t u8x8_st7301_122x250_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 20,
  /* pre_chip_disable_wait_ns = */ 20,
  /* reset_pulse_width_ms = */ 3, 	
  /* post_reset_wait_ms = */ 3, 		/**/
  /* sda_setup_time_ns = */ 10,		/* */
  /* sck_pulse_width_ns = */ 30,	/*  */
  /* sck_clock_hz = */ 2000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,	/* 400KHz */
  /* data_setup_time_ns = */ 15,
  /* write_pulse_width_ns = */ 70,	
  /* tile_width = */ 16,
  /* tile_height = */ 32,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 122,
  /* pixel_height = */ 250
};

uint8_t u8x8_d_st7301_122x250(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint16_t x;
  uint8_t c, i, y;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_init_seq);    
      break;
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_st7301_122x250_display_info);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int<<2 );	
      u8x8_cad_SendArg(u8x8, arg_int>>6 );	
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 8;
      x += u8x8->x_offset;
      y= (((u8x8_tile_t *)arg_ptr)->y_pos);
      y*=4;
    
      y+=115;           // specific for the 122x250 LCD

      u8x8_cad_StartTransfer(u8x8);

      for( i = 0; i < 4; i++ )
      {
        
        u8x8_cad_SendCmd(u8x8, 0x2a);
        u8x8_cad_SendArg(u8x8, 0x19);   // specific for the 122x250 LCD
        u8x8_cad_SendArg(u8x8, 0x3a );
      
        u8x8_cad_SendCmd(u8x8, 0x2b ); 
        u8x8_cad_SendArg(u8x8, y+i); 
        u8x8_cad_SendArg(u8x8, y+i); 
        u8x8_cad_SendCmd(u8x8, 0x02c );		// write data 
      
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        
        ptr += u8x8->display_info->tile_width*i*2;
        
        c = (c+2)/3;          // calculate the number of 24 bit blocks to send
        
        
        while( c > 0 )
        {
          u8x8_cad_SendData(u8x8, 6, u8x8_st7301_convert_a0(u8x8, ptr)); 	
          ptr+=3;
          --c;
        }
      }
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}
