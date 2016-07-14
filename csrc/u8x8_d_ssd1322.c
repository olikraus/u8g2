/*

  u8x8_d_ssd1322.c
  
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
#include "u8x8.h"



static const uint8_t u8x8_d_ssd1322_init_seq[] = {
    
  U8X8_DLY(100),
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_DLY(100),
  
  U8X8_CA(0xfd, 0x12),            	/* unlock */
  U8X8_C(0xae),		                /* display off */
  U8X8_CA(0xb3, 0x91),			/* set display clock divide ratio/oscillator frequency (set clock as 80 frames/sec)  */  
  U8X8_CA(0xca, 0x3f),			/* multiplex ratio 1/64 Duty (0x0F~0x3F) */  
  U8X8_CA(0xa2, 0x00),			/* display offset, shift mapping ram counter */  
  U8X8_CA(0xa1, 0x00),			/* display start line */  
  U8X8_CAA(0xa0, 0x14, 0x11),	/* Set Re-Map / Dual COM Line Mode */  
  U8X8_CA(0xab, 0x01),			/* Enable Internal VDD Regulator */  
  U8X8_CAA(0xb4, 0xa0, 0x005|0x0fd),	/* Display Enhancement A */  
  U8X8_CA(0xc1, 0x9f),			/* contrast */  
  U8X8_CA(0xc7, 0x0f),			/* Set Scale Factor of Segment Output Current Control */  
  U8X8_C(0xb9),		                /* linear grayscale */
  U8X8_CA(0xb1, 0xe2),			/* Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment */  
  U8X8_CAA(0xd1, 0x082|0x020, 0x020),	/* Display Enhancement B */  
  U8X8_CA(0xbb, 0x1f),			/* precharge  voltage */  
  U8X8_CA(0xb6, 0x08),			/* precharge  period */  
  U8X8_CA(0xbe, 0x07),			/* vcomh */  
  U8X8_C(0xa6),		                /* normal display */
  U8X8_C(0xa9),		                /* exit partial display */
  
  
  U8X8_DLY(2),					/* delay 2ms */

  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1322_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0af),		                /* ssd1322: display on */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1322_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ae),		                /* ssd1322: display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


/* interpret b as a monochrome bit pattern, write value 15 for high bit and value 0 for a low bit */
/* topbit (msb) is sent last */
/* example: b = 0x083 will send 0xff, 0x00, 0x00, 0xf0 */
uint8_t u8x8_write_byte_to_16gr_device(u8x8_t *u8x8, uint8_t b)
{
  static uint8_t buf[4];
  static uint8_t map[4] = { 0, 0x00f, 0x0f0, 0x0ff };
  buf [3] = map[b & 3];
  b>>=2;
  buf [2] = map[b & 3];
  b>>=2;
  buf [1] = map[b & 3];
  b>>=2;
  buf [0] = map[b & 3];
  return u8x8_cad_SendData(u8x8, 4, buf);	/* note: SendData can not handle more than 255 bytes, send one line of data */
}


uint8_t u8x8_d_ssd1322_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  // uint8_t x; 
  uint8_t y, c, i;
  uint8_t *ptr;
  switch(msg)
  {
    /* U8X8_MSG_DISPLAY_SETUP_MEMORY is handled by the calling function */
    /*
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      break;
    */
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1322_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1322_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1322_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      /* flip mode is NOT supported at the moment, however at least we assign the x_offset */
      if ( arg_int == 0 )
      {
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      y = (((u8x8_tile_t *)arg_ptr)->y_pos);
      y*=8;
      /* u8x8 API is not supported, so x is not required */
      /* x = ((u8x8_tile_t *)arg_ptr)->x_pos; */
    
      u8x8_cad_StartTransfer(u8x8);
        

      /* 
	Tile structure is reused here for the ssd1322, however u8x8 is not supported 
	tile_ptr points to data which has cnt*8 bytes (same as SSD1306 tiles)
	Buffer is expected to have 8 lines of code fitting to the ssd1322 internal memory
	"cnt" includes the number of horizontal bytes. width is equal to cnt*8
	Also important: Width must be a multiple of 16 (ssd1322 requirement), so cnt must be even.
      */
    
      ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;	/* data ptr to the tiles */
      for( i = 0; i < 8; i++ )
      {
	u8x8_cad_SendCmd(u8x8, 0x015 );	/* column adr */
	u8x8_cad_SendArg(u8x8, u8x8->x_offset );		/* start col */
	u8x8_cad_SendArg(u8x8, u8x8->x_offset + u8x8->display_info->pixel_width/4 + 1 );	/* end col */
	
	u8x8_cad_SendCmd(u8x8, 0x075 );	/* row adr */
	u8x8_cad_SendArg(u8x8, y );      		/* row adr*/
	u8x8_cad_SendArg(u8x8, y+1 );     	 /* row adr + 1 */
	
	u8x8_cad_SendCmd(u8x8, 0x05c);       /* write to ram */  
	
	c = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles */	
	while( c > 0 )
	{
	  u8x8_write_byte_to_16gr_device(u8x8, *ptr);
	  c--;
	  ptr++;
	}	
      }

      u8x8_cad_EndTransfer(u8x8);

      break;
    default:
      return 0;
  }
  return 1;
}

static const u8x8_display_info_t u8x8_ssd1322_256x64_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 20,
  /* pre_chip_disable_wait_ns = */ 10,
  /* reset_pulse_width_ms = */ 100, 	/* SSD1322: 2 us */
  /* post_reset_wait_ms = */ 100, /* far east OLEDs need much longer setup time */
  /* sda_setup_time_ns = */ 50,		/* SSD1322: 15ns, but cycle time is 100ns, so use 100/2 */
  /* sck_pulse_width_ns = */ 50,	/* SSD1322: 20ns, but cycle time is 100ns, so use 100/2, AVR: below 70: 8 MHz, >= 70 --> 4MHz clock */
  /* sck_takeover_edge = */ 1,		/* rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 10,
  /* write_pulse_width_ns = */ 150,	/* SSD1322: cycle time is 300ns, so use 300/2 = 150 */
  /* tile_width = */ 32,		/* 256 pixel, so we require 32 bytes for this */
  /* tile_hight = */ 8,
  /* default_x_offset = */ 0x01c,	/* this is the byte offset (there are two pixel per byte with 4 bit per pixel) */
  /* flipmode_x_offset = */ 0x01c,
  /* pixel_width = */ 256,
  /* pixel_height = */ 32
};



uint8_t u8x8_d_ssd1322_256x64(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ssd1322_256x64_display_info);
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x0c1 );
      u8x8_cad_SendArg(u8x8, arg_int );	/* ssd1322 has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    
    default:
      return u8x8_d_ssd1322_common(u8x8, msg, arg_int, arg_ptr);
  }
  return 1;
}

