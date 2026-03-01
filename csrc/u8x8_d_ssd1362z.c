/*
  u8x8_d_ssd1362z.c
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2022, olikraus@gmail.com
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

  PMOLED TRULY OEL1M0033-W-E:
    SSD1362Z: 
      256 x 64
      4bit gray scale

  Note: Currently the internal IREF is activated.
        Without external VP capacitor
  
*/
#include "u8x8.h"

static const uint8_t u8x8_d_ssd1362z_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0af),		                /* ssd1362z: display on */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1362z_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ae),		                /* ssd1362z: display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

/*
  input:
    one tile (8 Bytes)
  output:
    Tile for SSD1362Z (32 Bytes)
*/

static uint8_t u8x8_ssd1362z_oel1m0033we_to32_dest_buf[32];

static uint8_t *u8x8_ssd1362z_oel1m0033we_8to32(U8X8_UNUSED u8x8_t *u8x8, uint8_t *ptr)
{
  uint8_t v;
  uint8_t a,b;
  uint8_t i, j;
  uint8_t *dest;
  
  for( j = 0; j < 4; j++ )
  {
    dest = u8x8_ssd1362z_oel1m0033we_to32_dest_buf;
    dest += j;
    a =*ptr;
    ptr++;
    b = *ptr;
    ptr++;
    for( i = 0; i < 8; i++ )
    {
      v = 0;
      if ( a&1 ) v |= 0xf0;
      if ( b&1 ) v |= 0x0f;
      *dest = v;
      dest+=4;
      a >>= 1;
      b >>= 1;
    }
  }
  
  return u8x8_ssd1362z_oel1m0033we_to32_dest_buf;
}

uint8_t u8x8_d_ssd1362z_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x; 
  uint8_t y, c;
  uint8_t *ptr;
  switch(msg)
  {
    /* U8X8_MSG_DISPLAY_SETUP_MEMORY is handled by the calling function */
    /*
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362z_256x64_init_seq);
      break;
    */
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362z_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362z_powersave1_seq);
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int );	/* ssd1362z has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 4;		// convert from tile pos to display column
      x += u8x8->x_offset;		
    
      y = (((u8x8_tile_t *)arg_ptr)->y_pos);
      y *= 8;
    
      
      u8x8_cad_SendCmd(u8x8, 0x075 );	/* set row address, moved out of the loop (issue 302) */
      u8x8_cad_SendArg(u8x8, y);
      u8x8_cad_SendArg(u8x8, y+7);
      
      do
      {
	c = ((u8x8_tile_t *)arg_ptr)->cnt;
	ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;

	do
	{
	  u8x8_cad_SendCmd(u8x8, 0x015 );	/* set column address */
    u8x8_cad_SendArg(u8x8, x );	/* start */
    u8x8_cad_SendArg(u8x8, x+3 );	/* end */
    u8x8_cad_SendData(u8x8, 32, u8x8_ssd1362z_oel1m0033we_8to32(u8x8, ptr));
	  
	  ptr += 8;
	  x += 4;
	  c--;
	} while( c > 0 );
	
	arg_int--;
        
      } while( arg_int > 0 );
      
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}

/*=========================================================*/

static const uint8_t u8x8_d_ssd1362z_256x64_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CA(0xa0, 0xc3), //Set Remap c3 = 11000011  
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1362z_256x64_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CA(0xa0, 0xd0), 
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const u8x8_display_info_t u8x8_ssd1362z_oel1m0033we_256x64_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 20,
  /* pre_chip_disable_wait_ns = */ 10,
  /* reset_pulse_width_ms = */ 100, 	/* ssd1362z: 2 us */
  /* post_reset_wait_ms = */ 100, /* far east OLEDs need much longer setup time */
  /* sda_setup_time_ns = */ 50,		/* ssd1362z: 15ns, but cycle time is 100ns, so use 100/2 */
  /* sck_pulse_width_ns = */ 50,	/* ssd1362z: 20ns, but cycle time is 100ns, so use 100/2, AVR: below 70: 8 MHz, >= 70 --> 4MHz clock */
  /* sck_clock_hz = */ 10000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns, increased to 8MHz (issue 215), 10 MHz (issue 301) */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 10,
  /* write_pulse_width_ns = */ 150,	/* ssd1362z: cycle time is 300ns, so use 300/2 = 150 */
  /* tile_width = */ 32,		/* 256 pixel, so we require 32 bytes for this */
  /* tile_height = */ 8,
  /* default_x_offset = */ 0,	/* this is the byte offset (there are two pixel per byte with 4 bit per pixel) */
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 256,
  /* pixel_height = */ 64
};

static const uint8_t u8x8_d_ssd1362z_256x64_init_seq[] = {
    
  U8X8_DLY(1),
  U8X8_START_TRANSFER(),  // enable chip, delay is part of the transfer start
  U8X8_DLY(1),

  U8X8_CA(0xfd, 0x12),    // unlock
  U8X8_C(0xae),           // display off
  U8X8_CA(0x81, 0x7f),    // Set contrast: 127
  U8X8_CA(0xa0, 0xc1), 
  U8X8_CA(0xa1, 0x00),    // Set Display Start Line
  U8X8_CA(0xa2, 0x00),    // Set Display Offset
  U8X8_C(0xa4),           // Normal Display
  U8X8_CA(0xa8, 0x3f),    // Set Multiplex Ratio: (63 rows)
  U8X8_CA(0xab, 0x01),    // Enable VDD regulator
  U8X8_CA(0xad, 0x9e),    // External /Internal IREF Selection, 9e: internal, 8e: external
  U8X8_CA(0xb1, 0x22),    // Set Phase Length
  U8X8_CA(0xb3, 0xa0),    // Display clock Divider
  U8X8_CA(0xb6, 0x04),    // Set Second precharge Period
  U8X8_C(0xb9),           // Set Linear LUT
  U8X8_CA(0xbc, 0x04),    // Set pre-charge voltage level, 0..0x1f, 0x04 = 0.15*VCC
  U8X8_CA(0xbd, 0x00),    // Pre-charge voltage capacitor Selection, 0: without, 1: with Vp capacitor
  U8X8_CA(0xbe, 0x05),    // Set COM deselect voltage level, 5 = 0.82*VCC
  U8X8_DLY(1),            // delay 1ms

  U8X8_END_TRANSFER(),    // disable chip
  U8X8_END()              // end of sequence
};

uint8_t u8x8_d_ssd1362z_256x64_oel1m0033we(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ssd1362z_oel1m0033we_256x64_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362z_256x64_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362z_256x64_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362z_256x64_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
    
    default:
      return u8x8_d_ssd1362z_common(u8x8, msg, arg_int, arg_ptr);
  }
  return 1;
}