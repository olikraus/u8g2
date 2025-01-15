/*

  u8x8_d_ch1120.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2018, olikraus@gmail.com
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

/* 
  code copyied from sh1107
  ch1120: 160x160 controller from Sino Wealth
  https://github.com/olikraus/u8g2/issues/2496
*/



static const uint8_t u8x8_d_ch1120_noname_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0af),		                /* display on */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ch1120_noname_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ae),		                /* display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ch1120_160x160_noname_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a0),				/* segment remap a0/a1*/
  U8X8_C(0x0c0),				/* c0: scan dir normal, c8: reverse */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ch1120_160x160_noname_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a1),				/* segment remap a0/a1*/
  U8X8_C(0x0c8),				/* c0: scan dir normal, c8: reverse */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


static uint8_t u8x8_d_ch1120_generic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, c;
  uint8_t *ptr;
  switch(msg)
  {
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ch1120_64x128_noname_display_info);
      break;
    */
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ch1120_64x128_noname_init_seq);    
      break;
    */
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_ch1120_noname_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_ch1120_noname_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ch1120_160x160_noname_powersave0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ch1120_160x160_noname_powersave1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int );	/* ch1120 has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 8;
      x += u8x8->x_offset;

      //u8x8_cad_SendCmd(u8x8, 0x040 ); /* set line offset to 0 */

      // set column address
      u8x8_cad_SendCmd(u8x8, 0x010 | (x >> 4));
      u8x8_cad_SendCmd(u8x8, 0x000 | ((x & 15))); 
      
      // set page address
      u8x8_cad_SendCmd(u8x8, 0x0b0 ); 		// page cmd is a two byte command
      u8x8_cad_SendArg(u8x8, (((u8x8_tile_t *)arg_ptr)->y_pos)); 
    
      do
      {
	c = ((u8x8_tile_t *)arg_ptr)->cnt;
	ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
	u8x8_cad_SendData(u8x8, c*8, ptr); 	/* note: SendData can not handle more than 255 bytes */
	arg_int--;
      } while( arg_int > 0 );
      
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}

/*==================================================*/

#ifdef NOT_YET_REQUESTED
/* 160x160 OLED */
static const uint8_t u8x8_d_ch1120_160x160_noname_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
    
  U8X8_C(0x0ae),		                /* display off */
  U8X8_CA(0x0d5, 0x060),		/* clock divide ratio and oscillator frequency */
  U8X8_CA(0x0a9, 0x003), 		/* set display resolution, 0=64x160, 1=96x160, 2=128x160, 3=160x160 */
  U8X8_C(0x020),		                /* addressing mode */
  U8X8_CA(0x081, 0x01f), 		/* set contrast control */
  U8X8_CA(0x0ad, 0x80),			/* DC/DC control 80=Use external Vpp, 89=Use internal DC/DC*/
  U8X8_C(0x030),				/* set discharge VSL level, 0x030..0x03f */
  U8X8_CA(0x0d9, 0x028), 		/* pre-charge period */
  U8X8_CA(0x0db, 0x035), 		/* vcomh deselect level */    
  U8X8_CA(0x0dc, 0x035),		/* VSEGM Deselect Level */

  U8X8_C(0x0a0),				/* segment remap a0/a1*/
  U8X8_C(0x0c0),				/* c0: scan dir normal, c8: reverse */
  
  U8X8_C(0x0a4),				/* output ram to display */
  U8X8_C(0x0a6),				/* none inverted normal display mode */
    
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const u8x8_display_info_t u8x8_ch1120_160x160_noname_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 60,
  /* pre_chip_disable_wait_ns = */ 120,
  /* reset_pulse_width_ms = */ 100, 	/* ch1120: 3 us */
  /* post_reset_wait_ms = */ 100, /* sometimes OLEDs need much longer setup time */
  /* sda_setup_time_ns = */ 100,		/* ch1120: 100ns */
  /* sck_pulse_width_ns = */ 100,	/* ch1120: 100ns */
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 40,
  /* write_pulse_width_ns = */ 150,	/* ch1120: cycle time is 300ns, so use 300/2 = 150 */
  /* tile_width = */ 20,
  /* tile_height = */ 20,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 160,
  /* pixel_height = */ 160
};

uint8_t u8x8_d_ch1120_160x160(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    
  if ( u8x8_d_ch1120_generic(u8x8, msg, arg_int, arg_ptr) != 0 )
    return 1;
  
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ch1120_160x160_noname_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ch1120_160x160_noname_display_info);
      break;
    default:
      return 0;
  }
  return 1;
}
#endif

/*==================================================*/


/* https://github.com/olikraus/u8g2/issues/2496, 128x160 OLED */
static const uint8_t u8x8_d_ch1120_128x160_noname_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
    
  U8X8_C(0x0ae),		                /* display off */
  //U8X8_CAA(0x21, 0x00, 0x1f),           /* Set Column Start/End Address of Display RAM, mono mode */
  //U8X8_CAA(0x22, 0x00, 0x4f),           /* Set Row Start/End Address of Display RAM, mono mode */
  U8X8_CA(0x0a2, 0x000),		/* display start */
  U8X8_CA(0x081, 0x08f), 		/* set contrast control */
  U8X8_CA(0x0ac, 0x001),		/* mono mode, or 0x003???*/
  U8X8_CA(0x020, 0x000),		/* addressing mode */
  U8X8_C(0x0C8),		                /* scan direction */
  U8X8_CA(0x0a3, 0x000),		/* display rotation */
  
  U8X8_CA(0x0a8, 0x07f),		/* multiplex ratio */
  U8X8_CA(0x0d3, 0x010),		/* display offset */
  
  U8X8_CA(0x0d5, 0x01f),		/* clock divide ratio and oscillator frequency */
  
  U8X8_CA(0x048, 0x002),		/*  */
  U8X8_CA(0x093, 0x002),		/*  */
  U8X8_CA(0x0d8, 0x001),		/*  */
  U8X8_CA(0x049, 0x006),		/*  */
  U8X8_CA(0x0d9, 0x00f),		/*  */
  U8X8_CA(0x094, 0x01f),		/*  */
  U8X8_CA(0x04b, 0x004),		/*  */
  U8X8_CA(0x0da, 0x000),		/*  */
  U8X8_CA(0x0dc, 0x040),		/* VSEGM Deselect Level */
  U8X8_CA(0x0ad, 0x002),		/*  */
   
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const u8x8_display_info_t u8x8_ch1120_128x160_noname_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 20,
  /* pre_chip_disable_wait_ns = */ 20,
  /* reset_pulse_width_ms = */ 100, 	/* ch1120: 3 us */
  /* post_reset_wait_ms = */ 100, /* sometimes OLEDs need much longer setup time */
  /* sda_setup_time_ns = */ 100,		/* ch1120: 100ns */
  /* sck_pulse_width_ns = */ 100,	/* ch1120: 100ns */
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 40,
  /* write_pulse_width_ns = */ 40,	/* ch1120: cycle time is 300ns, so use 300/2 = 150 */
  /* tile_width = */ 16,
  /* tile_height = */ 20,
  /* default_x_offset = */ 16,
  /* flipmode_x_offset = */ 16,
  /* pixel_width = */ 128,
  /* pixel_height = */ 160
};

uint8_t u8x8_d_ch1120_128x160(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    
  if ( u8x8_d_ch1120_generic(u8x8, msg, arg_int, arg_ptr) != 0 )
    return 1;
  
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ch1120_128x160_noname_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ch1120_128x160_noname_display_info);
      break;
    default:
      return 0;
  }
  return 1;
}

/*==================================================*/
