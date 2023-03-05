/*

  u8x8_d_s1d15300.c
  
  WARNING: As of today, flip mode 1 doesn't seem to work (see issue 2063)
  
  Created as a copy of u8x8_d_st7565.c, see https://github.com/olikraus/u8g2/issues/2063
  
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

  
*/
#include "u8x8.h"





static const uint8_t u8x8_d_s1d15300_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a4),		                /* all pixel off, see datasheet for the s1d15300 */
  U8X8_C(0x0af),		                /* display on */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_s1d15300_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ae),		                /* display off */
  U8X8_C(0x0a5),		                /* enter powersafe, see datasheet for the s1d15300 */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_s1d15300_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a0),				/* segment remap a0/a1*/
  U8X8_C(0x0c0),				/* c0: scan dir normal, c8: reverse */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_s1d15300_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a1),				/* segment remap a0/a1*/
  U8X8_C(0x0c8),				/* c0: scan dir normal, c8: reverse */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

uint8_t u8x8_d_s1d15300_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8x8->x_offset;
      u8x8_cad_SendCmd(u8x8, 0x010 | (x>>4) );
      u8x8_cad_SendCmd(u8x8, 0x000 | ((x&15)));
      u8x8_cad_SendCmd(u8x8, 0x0b0 | (((u8x8_tile_t *)arg_ptr)->y_pos));
    
      c = ((u8x8_tile_t *)arg_ptr)->cnt;
      c *= 8;
      ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
      /* 
	The following if condition checks the hardware limits of the st7565 
	controller: It is not allowed to write beyond the display limits.
	This is in fact an issue within flip mode.
      */
      if ( c + x > 132u )
      {
	c = 132u;
	c -= x;
      }
      do
      {
	u8x8_cad_SendData(u8x8, c, ptr);	/* note: SendData can not handle more than 255 bytes */
	arg_int--;
      } while( arg_int > 0 );
      
      u8x8_cad_EndTransfer(u8x8);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_s1d15300_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_s1d15300_powersave1_seq);
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x080 |  (arg_int >> 3) ); /* s1d15300 has only 5 bits for contrast */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    default:
      return 0;
  }
  return 1;
}


/*================================================*/
/* LM6023, S1D15300 controller, issue 2063 */

static const u8x8_display_info_t u8x8_s1d15300_lm6023_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 250,	/*  */
  /* pre_chip_disable_wait_ns = */ 120,	/*  */
  /* reset_pulse_width_ms = */ 1, 
  /* post_reset_wait_ms = */ 1, 
  /* sda_setup_time_ns = */ 200,		/* */
  /* sck_pulse_width_ns = */ 200,	/* half of cycle time (100ns according to datasheet), AVR: below 70: 8 MHz, >= 70 --> 4MHz clock */
  /* sck_clock_hz = */ 1000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 200,	/* st7565 datasheet, table 24, tds8 */
  /* write_pulse_width_ns = */ 200,	/* st7565 datasheet, table 24, tcclw */
  /* tile_width = */ 16,		/* width of 16*8=128 pixel */
  /* tile_height = */ 8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 4,
  /* pixel_width = */ 128,
  /* pixel_height = */ 64
};


static const uint8_t u8x8_d_s1d15300_lm6023_init_seq[] = {

#ifdef OBSOLETE  
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  U8X8_C(0x0e2),            			/* soft reset */
  U8X8_C(0x0ae),		                /* display off */
  U8X8_C(0x040),		                /* set display start line to 0 */
  
  U8X8_C(0x0a1),		                /* ADC set to reverse */
  U8X8_C(0x0c0),		                /* common output mode */
  // Flipmode
  //U8X8_C(0x0a0),		                /* ADC set to reverse */
  //U8X8_C(0x0c8),		                /* common output mode */
  
  U8X8_C(0x0a6),		                /* display normal, bit val 0: LCD pixel off. */
  U8X8_C(0x0a2),		                /* LCD bias 1/9 */
  U8X8_C(0x02f),		                /* all power  control circuits on */
  U8X8_CA(0x0f8, 0x000),		/* set booster ratio to 4x */
  U8X8_C(0x025),		                /* set V0 voltage resistor ratio to large,  issue 1678: changed from 0x23 to 0x25 */
  U8X8_CA(0x081, 170),			/* set contrast, contrast value NHD C12864, see issue 186, increased contrast to 180 (issue 219), reduced to 170 (issue 1678) */
  
  U8X8_C(0x0ae),		                /* display off */
  U8X8_C(0x0a5),		                /* enter powersafe: all pixel on, issue 142 */
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
#endif
  

    U8X8_START_TRANSFER(), /* enable chip, delay is part of the transfer start */

    U8X8_C(0x0e2), /* soft reset */

    U8X8_C(0x0ae), /* display off */
    U8X8_C(0x040), /* set display start line to 0 */
    U8X8_C(0x0a0), /* ADC set to normal */
    U8X8_C(0x0a6), /* Display normal */
    U8X8_C(0x0a4), /* all point normal */

    U8X8_C(0x0a2), /* LCD bias 1/8 */
    U8X8_C(0x0c0), /* common output mode */
    U8X8_C(0x02f), /* all power control circuits on */


    U8X8_C(0x090), /* set contrast */
    //U8X8_C(0x0af), /* display on */

    U8X8_C(0x0b0), /* set page address */
    U8X8_C(0x01f), /* set column address upper */
    U8X8_C(0x000), /* set column address lower */

    U8X8_END_TRANSFER(), /* disable chip */
    U8X8_END()           /* end of sequence */  
};

uint8_t u8x8_d_s1d15300_lm6023(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* call common procedure first and handle messages there */
  if ( u8x8_d_s1d15300_common(u8x8, msg, arg_int, arg_ptr) == 0 )
  {
    /* msg not handled, then try here */
    switch(msg)
    {
      case U8X8_MSG_DISPLAY_SETUP_MEMORY:
	u8x8_d_helper_display_setup_memory(u8x8, &u8x8_s1d15300_lm6023_display_info);
	break;
      case U8X8_MSG_DISPLAY_INIT:
	u8x8_d_helper_display_init(u8x8);
	u8x8_cad_SendSequence(u8x8, u8x8_d_s1d15300_lm6023_init_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
	if ( arg_int == 0 )
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_s1d15300_flip0_seq);
	  u8x8->x_offset = u8x8->display_info->default_x_offset;
	}
	else
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_s1d15300_flip1_seq);
	  u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
	}	
	break;
      default:
	return 0;		/* msg unknown */
    }
  }
  return 1;
}

/*================================================*/


