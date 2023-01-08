/*

  u8x8_d_gu800.c
  
  Support for GU800 series of VFD displays
  https://www.noritake-elec.com/products/vfd-display-module/dot-matrix-graphic-display/gu-800-series
  
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

  
 
  u8x8_gu800_cad_110
  
  WARNING: DO NOT USE THIS FILE AS TEMPLATE:
  
  This code requires a special cad callback which includes the start
  and end transfer code. As a consequence this code does not include any
  start / end transfer calls.
  
*/
#include "u8x8.h"




static const uint8_t u8x8_d_gu800_128x64_powersave0_seq[] = {
  U8X8_CA(0x024, 0x040),		        /* Byte 1: Layer 0010xx00, Byte 2: 0x40 (graphics on, normal mode, no or/xor) */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_gu800_128x64_powersave1_seq[] = {
  U8X8_CA(0x020, 0x040),		        /* Byte 1: Layer 0010xx00, Byte 2: 0x40 (graphics on, normal mode, no or/xor) */
  U8X8_END()             			/* end of sequence */
};

/* hardware flip is not supported by GU800 */
#ifdef NOT_SUPPORTED
static const uint8_t u8x8_d_gu800_128x64_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_gu800_128x64_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};
#endif

uint8_t u8x8_d_gu800_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_gu800_128x64_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_gu800_128x64_powersave1_seq);
      break;
#ifdef NOT_SUPPORTED
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_gu800_128x64_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_gu800_128x64_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }	
      break;
#endif
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_SendCmd(u8x8, 0x4f - ((arg_int >> 4)&0x0f) );	/* GU800 has range from 0..15, max brightness is 0x040  */
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8x8->x_offset;
      u8x8_cad_SendCmd(u8x8, 0x064 );
      u8x8_cad_SendArg(u8x8, x );
      u8x8_cad_SendCmd(u8x8, 0x060 );
      u8x8_cad_SendArg(u8x8, (((u8x8_tile_t *)arg_ptr)->y_pos) );
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        /* SendData can not handle more than 255 bytes, treat c > 31 correctly  */
        if ( c > 31 )
        {
          u8x8_cad_SendData(u8x8, 248, ptr); 	/* 31*8=248 */
          ptr+=248;
          c -= 31;
        }
        u8x8_cad_SendData(u8x8, c*8, ptr); 	
	arg_int--;
      } while( arg_int > 0 );
      break;
    default:
      return 0;
  }
  return 1;
}

/*=================================================*/

static const uint8_t u8x8_d_gu800_128x64_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  U8X8_C(0x05f),                                /* clear all RAM, clear address counters */
  U8X8_DLY(1),                                  /* delay for 1 ms (see datasheet) */
  
  /* configure all area as graphics RAM */
  U8X8_CA(0x62,0), U8X8_D1(0xff),
  U8X8_CA(0x62,1), U8X8_D1(0xff),
  U8X8_CA(0x62,2), U8X8_D1(0xff),
  U8X8_CA(0x62,3), U8X8_D1(0xff),
  U8X8_CA(0x62,4), U8X8_D1(0xff),
  U8X8_CA(0x62,5), U8X8_D1(0xff),
  U8X8_CA(0x62,6), U8X8_D1(0xff),
  U8X8_CA(0x62,7), U8X8_D1(0xff),
  
  U8X8_CA(0x70, 0),                             /* horizontal shift */
  U8X8_C(0xb0),                                 /* vertical shift */  
  U8X8_C(0x40),                                 /* min (0x04f) / max (0x040) brightness */
  U8X8_C(0x84),                                 /* x increment */
  
  // U8X8_CA(0x024, 0x040)              /* display on */
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


static const u8x8_display_info_t u8x8_d_gu800_128x64_display_info =
{
  /* chip_enable_level = */ 0,      /* GU800: CSS signal */
  /* chip_disable_level = */ 1,     /* GU800: CSS signal */
  
  /* post_chip_enable_wait_ns = */ 40,
  /* pre_chip_disable_wait_ns = */ 150,
  /* reset_pulse_width_ms = */ 2,   /* GU800: Unspecified in datasheet */
  /* post_reset_wait_ms = */ 2,     /* GU800: Min 1.5ms per datasheet */
  /* sda_setup_time_ns = */ 40,     /* GU800: 40ns according to the timing diagram */
  /* sck_pulse_width_ns = */ 80,    /* GU800: Min 80ns per datasheet */
  /* sck_clock_hz = */ 4000000UL,
  /* spi_mode = */ 2,               /* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,   /* GU800: Not used */
  /* data_setup_time_ns = */ 40,    /* GU800: Min 40ns per datasheet */
  /* write_pulse_width_ns = */ 150, /* GU800: Min 150ns per datasheet */
  /* tile_width = */ 16,            /* width of 16*8=128 pixel */
  /* tile_hight = */ 8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 128,
  /* pixel_height = */ 64
};


uint8_t u8x8_d_gu800_128x64(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  
  if ( u8x8_d_gu800_common(u8x8, msg, arg_int, arg_ptr) )
    return 1;
  
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_d_gu800_128x64_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_gu800_128x64_init_seq);
      break;
    default:
      return 0;
  }
  return 1;
}

/*=================================================*/
/* https://github.com/olikraus/u8g2/issues/1970 */

static const uint8_t u8x8_d_gu800_160x16_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  U8X8_C(0x05f),                                /* clear all RAM, clear address counters */
  U8X8_DLY(1),                                  /* delay for 1 ms (see datasheet) */
  
  /* configure all area as graphics RAM */
  U8X8_CA(0x62,0), U8X8_D1(0xff),
  U8X8_CA(0x62,1), U8X8_D1(0xff),
  U8X8_CA(0x62,2), U8X8_D1(0xff),
  U8X8_CA(0x62,3), U8X8_D1(0xff),
  U8X8_CA(0x62,4), U8X8_D1(0xff),
  U8X8_CA(0x62,5), U8X8_D1(0xff),
  U8X8_CA(0x62,6), U8X8_D1(0xff),
  U8X8_CA(0x62,7), U8X8_D1(0xff),
  
  U8X8_CA(0x70, 0),                             /* horizontal shift */
  U8X8_C(0xb0),                                 /* vertical shift */  
  U8X8_C(0x40),                                 /* min (0x04f) / max (0x040) brightness */
  U8X8_C(0x84),                                 /* x increment */
  
  // U8X8_CA(0x024, 0x040)              /* display on */
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


static const u8x8_display_info_t u8x8_d_gu800_160x16_display_info =
{
  /* chip_enable_level = */ 0,      /* GU800: CSS signal */
  /* chip_disable_level = */ 1,     /* GU800: CSS signal */
  
  /* post_chip_enable_wait_ns = */ 40,
  /* pre_chip_disable_wait_ns = */ 150,
  /* reset_pulse_width_ms = */ 2,   /* GU800: Unspecified in datasheet */
  /* post_reset_wait_ms = */ 2,     /* GU800: Min 1.5ms per datasheet */
  /* sda_setup_time_ns = */ 40,     /* GU800: 40ns according to the timing diagram */
  /* sck_pulse_width_ns = */ 80,    /* GU800: Min 80ns per datasheet */
  /* sck_clock_hz = */ 4000000UL,
  /* spi_mode = */ 3,               /* active high, rising edge, ISSUE 1970 */
  /* i2c_bus_clock_100kHz = */ 4,   /* GU800: Not used */
  /* data_setup_time_ns = */ 40,    /* GU800: Min 40ns per datasheet */
  /* write_pulse_width_ns = */ 150, /* GU800: Min 150ns per datasheet */
  /* tile_width = */ 20,            /* width of 8*20=160 pixel */
  /* tile_hight = */ 2,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 160,
  /* pixel_height = */ 16
};


uint8_t u8x8_d_gu800_160x16(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  
  if ( u8x8_d_gu800_common(u8x8, msg, arg_int, arg_ptr) )
    return 1;
  
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_d_gu800_160x16_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_gu800_160x16_init_seq);
      break;
    default:
      return 0;
  }
  return 1;
}

/*=================================================*/

