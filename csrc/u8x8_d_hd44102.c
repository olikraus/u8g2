/*

  u8x8_d_hd44102.c 
  
  Support for HD44102 and T7932 controller (https://github.com/olikraus/u8g2/issues/1492)
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2021, olikraus@gmail.com
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




static const uint8_t u8x8_d_hd44102_init_seq[] = {
  U8X8_C(0x03b),		                /* upcount  */  
  U8X8_C(0x03e),		                /* start at the top  */  
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_hd44102_powersave0_seq[] = {
  U8X8_C(0x039),		                /* display on */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_hd44102_powersave1_seq[] = {
  U8X8_C(0x038),		                /* display off */
  U8X8_END()             			/* end of sequence */
};

/*
  data: pointer to "cnt" bytes
  cnt: number of bytes, which should be sent
  page: 0..3: the current page within the display
  offset: offset location on the display, where the data should be placed
  cs_none: 3-bit chip select pattern for not selecting any display
  cs0: 3-bit chip select pattern for the left display (0..49)
  cs1: 3-bit chip select pattern for the middle display (50..99)
  cs2: 3-bit chip select pattern for the right display (100..149)

  The csX bit pattern is:
    bit 0: CS 
    bit 1: CS1
    bit 2: CS2
    see u8x8_byte_set_ks0108_cs() in u8x8_byte.c

*/
static void u8x8_hd44102_outy(u8x8_t *u8x8, uint8_t *data, uint8_t cnt, uint8_t page, uint8_t offset, uint8_t cs_none, uint8_t cs0, uint8_t cs1, uint8_t cs2)
{
  uint8_t pos = offset;
  uint8_t data_cnt;
  
  /* write to left display? */
  if ( pos < 50 && cnt > 0 )
  {
    /* calculate number of bytes to sent */
    data_cnt = 50 - pos;
    if ( data_cnt > cnt )
      data_cnt = cnt;
    if ( data_cnt > 50 )
      data_cnt = 50;

    /* sent the data */
    u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, cs0, NULL);
    u8x8_cad_SendCmd(u8x8, (page << 6) | pos );
    u8x8_cad_SendData(u8x8, data_cnt, data);
    u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, cs_none, NULL); 

    /* adjust the data */
    data += data_cnt;
    pos += data_cnt;
    cnt -= data_cnt;
  }
  
  /* write to middle display? */
  if ( pos < 100 && cnt > 0 )
  {
    /* calculate number of bytes to sent */
    data_cnt = 100 - pos;
    if ( data_cnt > cnt )
      data_cnt = cnt;
    if ( data_cnt > 50 )
      data_cnt = 50;

    /* sent the data */
    u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, cs1, NULL);
    u8x8_cad_SendCmd(u8x8, (page << 6) | (pos-50) );
    u8x8_cad_SendData(u8x8, data_cnt, data);
    u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, cs_none, NULL); 

    /* adjust the data */
    data += data_cnt;
    pos += data_cnt;
    cnt -= data_cnt;
  }
  
  /* write to right display? */
  if ( pos < 150 && cnt > 0 )
  {
    /* calculate number of bytes to sent */
    data_cnt = 150 - pos;
    if ( data_cnt > cnt )
      data_cnt = cnt;
    if ( data_cnt > 50 )
      data_cnt = 50;

    /* sent the data */
    u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, cs2, NULL);
    u8x8_cad_SendCmd(u8x8, (page << 6) | (pos-100) );
    u8x8_cad_SendData(u8x8, data_cnt, data);
    u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, cs_none, NULL); 

    /* adjust the data */
    data += data_cnt;
    pos += data_cnt;
    cnt -= data_cnt;
  }
}




static const u8x8_display_info_t u8x8_hd44102_150x32_display_info =
{
  /* chip_enable_level = */ 0,		/* KS0108/HD44102: Not used */
  /* chip_disable_level = */ 1,		/* KS0108/HD44102: Not used */
  
  /* post_chip_enable_wait_ns = */ 100,
  /* pre_chip_disable_wait_ns = */ 20,
  /* reset_pulse_width_ms = */ 1, 
  /* post_reset_wait_ms = */ 6, 	
  /* sda_setup_time_ns = */ 12,		
  /* sck_pulse_width_ns = */ 75,	/* KS0108/HD44102: Not used */
  /* sck_clock_hz = */ 4000000UL,	/* KS0108/HD44102: Not used */
  /* spi_mode = */ 0,				/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,	/* KS0108/HD44102: Not used */
  /* data_setup_time_ns = */ 200,
  /* write_pulse_width_ns = */ 250,	/* KS0108/HD44102: actially 450 ns, but additional 200 ns are added by the byte transfer function */
  /* tile_width = */ 19,		/* width of 19*8=152 pixel */
  /* tile_height = */ 4,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 150,
  /* pixel_height = */ 32
};

//uint8_t u8x8_d_hd44102_150x32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)

uint8_t u8x8_d_t7932_150x32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t cs[4] = {1, 2, 4, 0};
  uint8_t i;
  uint8_t x, cnt;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_hd44102_150x32_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);

      for( i = 0; i < 3; i++ )
      {
        u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, cs[i], NULL);
        u8x8_cad_SendSequence(u8x8, u8x8_d_hd44102_init_seq);
        u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, cs[3], NULL);
      }
    
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      
      for( i = 0; i < 3; i++ )
      {
        u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, cs[i], NULL);
        if ( arg_int == 0 )
        {
          u8x8_cad_SendSequence(u8x8, u8x8_d_hd44102_powersave0_seq);
        }
        else
        {
          u8x8_cad_SendSequence(u8x8, u8x8_d_hd44102_powersave1_seq);
        }
        u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, cs[3], NULL);	
      }
      break;
// The HD44102 can not mirror the cols and rows, use U8g2 for rotation
//    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
//      break;
// The HD44102 has no internal contrast command
//    case U8X8_MSG_DISPLAY_SET_CONTRAST:
//      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      cnt = ((u8x8_tile_t *)arg_ptr)->cnt;
      x*=8;
      cnt*=8;
    
      do
      {
        u8x8_hd44102_outy(u8x8, 
          ((u8x8_tile_t *)arg_ptr)->tile_ptr, 
          cnt, 
          ((u8x8_tile_t *)arg_ptr)->y_pos, 
          x, 
          cs[3], cs[0], cs[1], cs[2]);
        arg_int--;
        x += cnt;
      } while (arg_int > 0);
    
    
      break;
    default:
      return 0;
  }
  return 1;
}

static const u8x8_display_info_t u8x8_hd44102_100x64_display_info =
{
  /* chip_enable_level = */ 0,		/* KS0108/HD44102: Not used */
  /* chip_disable_level = */ 1,		/* KS0108/HD44102: Not used */
  
  /* post_chip_enable_wait_ns = */ 100,
  /* pre_chip_disable_wait_ns = */ 20,
  /* reset_pulse_width_ms = */ 1, 
  /* post_reset_wait_ms = */ 6, 	
  /* sda_setup_time_ns = */ 12,		
  /* sck_pulse_width_ns = */ 75,	/* KS0108/HD44102: Not used */
  /* sck_clock_hz = */ 4000000UL,	/* KS0108/HD44102: Not used */
  /* spi_mode = */ 0,				/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,	/* KS0108/HD44102: Not used */
  /* data_setup_time_ns = */ 200,
  /* write_pulse_width_ns = */ 250,	/* KS0108/HD44102: actially 450 ns, but additional 200 ns are added by the byte transfer function */
  /* tile_width = */ 13,		/* width of 13*8=104 pixel */
  /* tile_height = */ 8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 100,
  /* pixel_height = */ 64
};

uint8_t u8x8_d_hd44102_100x64(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t i;
  uint8_t x, cnt, page;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_hd44102_100x64_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);

      for( i = 0; i < 4; i++ )
      {
        u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, i, NULL);
        u8x8_cad_SendSequence(u8x8, u8x8_d_hd44102_init_seq);
        u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, 7, NULL);
      }
    
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      
      for( i = 0; i < 4; i++ )
      {
        u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, i, NULL);
        if ( arg_int == 0 )
        {
          u8x8_cad_SendSequence(u8x8, u8x8_d_hd44102_powersave0_seq);
        }
        else
        {
          u8x8_cad_SendSequence(u8x8, u8x8_d_hd44102_powersave1_seq);
        }
        u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, 7, NULL);	
      }
      break;
// The HD44102 can not mirror the cols and rows, use U8g2 for rotation
//    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
//      break;
// The HD44102 has no internal contrast command
//    case U8X8_MSG_DISPLAY_SET_CONTRAST:
//      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;      
      cnt = ((u8x8_tile_t *)arg_ptr)->cnt;
      page = ((u8x8_tile_t *)arg_ptr)->y_pos;
      x*=8;
      cnt*=8;
      if ( page < 4 )
      {
        do
        {
          u8x8_hd44102_outy(u8x8, 
            ((u8x8_tile_t *)arg_ptr)->tile_ptr, 
            cnt, 
            page, 
            x, 
            7, 0, 1, 7);
          arg_int--;
          x += cnt;
        } while (arg_int > 0);
      }
      else
      {
        do
        {
          u8x8_hd44102_outy(u8x8, 
            ((u8x8_tile_t *)arg_ptr)->tile_ptr, 
            cnt, 
            page-4, 
            x, 
            7, 2, 3, 7);
          arg_int--;
          x += cnt;
        } while (arg_int > 0);
      }
    
      break;
    default:
      return 0;
  }
  return 1;
}
