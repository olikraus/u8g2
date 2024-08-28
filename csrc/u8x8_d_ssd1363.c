/*

  u8x8_d_ssd1363.c
  
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


  SSD1363: 
    320 x 160 dot matrix
    16 gray scale

  https://github.com/olikraus/u8g2/issues/2490
  
*/
#include "u8x8.h"




static const uint8_t u8x8_d_ssd1363_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0af),		                /* ssd1363: display on */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1363_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ae),		                /* ssd1363: display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};



/* interpret b as a monochrome bit pattern, write value 15 for high bit and value 0 for a low bit */
/* topbit (msb) is sent last */
/* example: b = 0x083 will send 0xff, 0x00, 0x00, 0xf0 */



/*
  input:
    one tile (8 Bytes)
  output:
    Tile for SSD1363 (32 Bytes)
*/

static uint8_t u8x8_ssd1363_to32_dest_buf[32];

static uint8_t *u8x8_ssd1363_8to32(U8X8_UNUSED u8x8_t *u8x8, uint8_t *ptr)
{
  uint8_t a;
  uint8_t i;
  uint8_t *dest;

  dest = u8x8_ssd1363_to32_dest_buf;
  
  if ( ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 0 && ptr[3] == 0 && ptr[4] == 0 && ptr[5] == 0 && ptr[6] == 0 && ptr[7] == 0 )
  {
    for( i = 0; i < 32; i++ )
      *dest++ = 0;
    return u8x8_ssd1363_to32_dest_buf;
  }
  
  a = 1;
  for( i = 0; i < 8; i++ )
  {
    dest[0] = 0;
    dest[1] = 0;
    dest[2] = 0;
    dest[3] = 0;
    if ( ptr[0] & a )
      dest[1] |= 0x0f;
    if ( ptr[1] & a )
      dest[1] |= 0xf0;
    if ( ptr[2] & a )
      dest[0] |= 0x0f;
    if ( ptr[3] & a )
      dest[0] |= 0xf0;

    if ( ptr[4] & a )
      dest[3] |= 0x0f;
    if ( ptr[5] & a )
      dest[3] |= 0xf0;
    if ( ptr[6] & a )
      dest[2] |= 0x0f;
    if ( ptr[7] & a )
      dest[2] |= 0xf0;
    
    a <<= 1;
    dest += 4;
  }  

    
  return u8x8_ssd1363_to32_dest_buf;
}


uint8_t u8x8_d_ssd1363_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
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
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1363_256x128_init_seq);
      break;
    */
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1363_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1363_powersave1_seq);
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x0C1 );
      u8x8_cad_SendArg(u8x8, arg_int );	/* ssd1363 has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 2;		// only every 4th col can be addressed
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
	  u8x8_cad_SendArg(u8x8, x+1 );	/* end */

	  u8x8_cad_SendCmd(u8x8, 0x05c );	/* write to ram */
	  
	  u8x8_cad_SendData(u8x8, 32, u8x8_ssd1363_8to32(u8x8, ptr));
	  
	  ptr += 8;
	  x += 2;
	  c--;
	} while( c > 0 );
	
	//x += 2;
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

static const uint8_t u8x8_d_ssd1363_256x128_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CAA(0x0a0, 0x032, 0x000),		/* remap */
  U8X8_CA(0xa2, 0x20),			/* display offset, shift mapping ram counter */  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1363_256x128_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CAA(0x0a0, 0x020, 0x000),		/* remap */
  U8X8_CA(0xa2, 0x80),			/* display offset, shift mapping ram counter */  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


/*=========================================================*/
/*
  https://github.com/olikraus/u8g2/issues/2490
  ZJY270S0700XG21
*/


static const u8x8_display_info_t u8x8_ssd1363_256x128_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 20,
  /* pre_chip_disable_wait_ns = */ 10,
  /* reset_pulse_width_ms = */ 100, 	/* ssd1363: 2 us */
  /* post_reset_wait_ms = */ 100, /* far east OLEDs need much longer setup time */
  /* sda_setup_time_ns = */ 50,		/* 15ns, but cycle time is 100ns, so use 100/2 */
  /* sck_pulse_width_ns = */ 50,	/* 20ns, but cycle time is 100ns, so use 100/2, AVR: below 70: 8 MHz, >= 70 --> 4MHz clock */
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns, increased to 8MHz (issue 215), 10 MHz (issue 301) */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 10,
  /* write_pulse_width_ns = */ 150,	/* ssd1363: cycle time is 300ns, so use 300/2 = 150 */
  /* tile_width = */ 32,		/* 256 pixel, so we require 32 bytes for this */
  /* tile_height = */ 16,
  /* default_x_offset = */ 8,	/* this is the byte offset (there are two pixel per byte with 4 bit per pixel) */
  /* flipmode_x_offset = */ 8,
  /* pixel_width = */ 256,
  /* pixel_height = */ 128
};


static const uint8_t u8x8_d_ssd1363_256x128_init_seq[] = {
  
  U8X8_DLY(1),
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_DLY(1),
  
  U8X8_CA(0xfd, 0x12),            	/* unlock */
  U8X8_C(0xae),		                /* display off */
  U8X8_CA(0xb3, 0x30),			/* clock divide ratio/oscillator frequency (midas: 0x30 )  */  
  U8X8_CA(0xca, 127),			/* multiplex ratio  (3..159) */  
  U8X8_CA(0xa2, 0x20),			/* display offset, shift mapping ram counter */  

  U8X8_CA(0xa1, 0x00),			/* display start line */  
  
  //U8X8_CA(0xab, 0x01),			/* Enable Internal VDD Regulator */  
/*
	A[0]=0b, Horizontal address increment [reset]    ***
	A[0]=1b, Vertical address increment
	
	A[1]=0b, Disable Column Address Re-map [reset]
	A[1]=1b, Enable Column Address Re-map		***
	
	A[4]=0b, Scan from COM0 to COM[N –1] [reset]	
	A[4]=1b, Scan from COM[N-1] to COM0, where N is the	***
	
	Multiplex ratio
	A[5]=0b, Disable COM Split Odd Even [reset]	***
	A[5]=1b, Enable COM Split Odd Even
	
	B[4], Enable / disable Dual COM Line mode
	0b, Disable Dual COM mode [reset]
	1b, Enable Dual COM mode (MUX ≤ 79)
	
	0x16 = 00010110
*/
  U8X8_CAA(0xa0, 0x32, 0x000),	/* Set Re-Map / Dual COM Line Mode (midas datasheet) */  
  U8X8_CAA(0xb4, 0x32, 0x00c),	/* Display Enhancement A (midas datasheet) NOT DOCUMENTED */  
  //U8X8_CA(0xc7, 0x0f),			/* Set Scale Factor of Segment Output Current Control */  
  U8X8_CA(0xc1, 0xff),			/* contrast */  
  U8X8_CA(0xba, 0x03),			/* voltage config: Vp pin is connected to cap */    
  U8X8_C(0xb9),		                /* linear grayscale */
  U8X8_CA(0xad, 0x90),			/* internal IREF: 0x90, external IREF: 0x80 */    

  U8X8_CA(0xb1, 0x74),			/* Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment (midas) */  
  U8X8_CA(0xbb, 0x0c),			/* precharge  voltage */    
  
  U8X8_CA(0xb6, 0xc8),			/* second pre charge (midas) */  
  U8X8_CA(0xbe, 0x04),			/* vcomh (midas) */  
  
  U8X8_C(0xa6),		                /* normal display */
  U8X8_C(0xa9),		                /* exit partial display */


  U8X8_DLY(1),					/* delay 2ms */

  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

uint8_t u8x8_d_ssd1363_256x128(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ssd1363_256x128_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1363_256x128_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1363_256x128_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1363_256x128_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
    
    default:
      return u8x8_d_ssd1363_common(u8x8, msg, arg_int, arg_ptr);
  }
  return 1;
}


