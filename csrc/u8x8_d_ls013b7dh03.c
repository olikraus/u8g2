/*

  u8x8_d_ls013b7dh03.c
  
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


  The LS013B7DH02 is a simple display and controller
  --> no support for contrast adjustment, flip and power down.
  
  
*/
#include "u8x8.h"


static const u8x8_display_info_t u8x8_ls013b7dh03_128x128_display_info =
{
  /* chip_enable_level = */ 1,
  /* chip_disable_level = */ 0,
  
  /* post_chip_enable_wait_ns = */ 255,	/* 6000ns ??? */
  /* pre_chip_disable_wait_ns = */ 255,  /* 2000ns ??? */
  /* reset_pulse_width_ms = */ 1, 
  /* post_reset_wait_ms = */ 6, 
  /* sda_setup_time_ns = */ 227,	/* 227 nsec according to the datasheet */		
  /* sck_pulse_width_ns = */  255,	/* 450 nsec according to the datasheet */
  /* sck_takeover_edge = */ 1,		/* rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 100,
  /* write_pulse_width_ns = */ 100,
  /* tile_width = */ 16,
  /* tile_hight = */ 16,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 128,
  /* pixel_height = */ 128
};


uint8_t u8x8_d_ls013b7dh03_128x128(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t y, c, i;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ls013b7dh03_128x128_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      /* u8x8_cad_SendSequence(u8x8, u8x8_d_ls013b7dh03_init_seq); */
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      /* not available for the ls013b7dh03 */
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      y = (((u8x8_tile_t *)arg_ptr)->y_pos);
      y++;		/* lines are from 1..128, so add 1 here */
    
      /* x has to be zero for all transfers to the ls013b7dh03 */
      /* in fact all data has to start at the left edge */
    
      u8x8_cad_StartTransfer(u8x8);
        

      /* 
	Tile structure is reused here for the ls013b7dh03, however u8x8 is not supported 
	tile_ptr points to data which has cnt*8 bytes (same as SSD1306 tiles)
	Buffer is expected to have 8 lines of code fitting to the ls013b7dh03 internal memory
	"cnt" includes the number of horizontal bytes. width is equal to cnt*8
	
      */
      c = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles. This must be 16 for the ls013b7dh03 128x128 */
      ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;	/* data ptr to the tiles */
      for( i = 0; i < 8; i++ )
      {
	u8x8_cad_SendCmd(u8x8, 0x080 );	/* data mode byte */
	u8x8_cad_SendCmd(u8x8, y );      /* y pos  */
	u8x8_cad_SendData(u8x8, c, ptr);	/* note: SendData can not handle more than 255 bytes, send one line of data */
	u8x8_cad_SendCmd(u8x8, 0);      /* two bytes of dummy data  */
	u8x8_cad_SendCmd(u8x8, 0);      /* two bytes of dummy data  */
	ptr += c;
      }

      u8x8_cad_EndTransfer(u8x8);

      break;
    default:
      return 0;
  }
  return 1;
}


