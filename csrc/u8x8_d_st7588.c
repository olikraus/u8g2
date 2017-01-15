/*

  u8x8_d_st7588.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2017, olikraus@gmail.com
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


static const uint8_t u8x8_d_st7588_128x64_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0af),		                /* display on */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7588_128x64_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ae),		                /* display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};




static uint8_t u8x8_d_st7588_128x64_generic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_st7588_128x64_display_info);
      break;
    */
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_st7588_128x64_init_seq);    
      break;
    */
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7588_128x64_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7588_128x64_powersave1_seq);
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int );	/* ssd1327 has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 8;
      x += u8x8->x_offset;
    
    
      u8x8_cad_SendCmd(u8x8, 0x010 | (x>>4) );
      u8x8_cad_SendArg(u8x8, 0x000 | ((x&15)));
      u8x8_cad_SendArg(u8x8, 0x0b0 | (((u8x8_tile_t *)arg_ptr)->y_pos));
    
      
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

/*=============================================*/

static const u8x8_display_info_t u8x8_st7588_128x64_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 150,
  /* pre_chip_disable_wait_ns = */ 30,
  /* reset_pulse_width_ms = */ 5, 	
  /* post_reset_wait_ms = */ 5, 		/**/
  /* sda_setup_time_ns = */ 60,		/* */
  /* sck_pulse_width_ns = */ 60,	/*  */
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,	/* 400KHz */
  /* data_setup_time_ns = */ 80,
  /* write_pulse_width_ns = */ 50,	
  /* tile_width = */ 16,
  /* tile_hight = */ 8,
  /* default_x_offset = */ 16,
  /* flipmode_x_offset = */ 16,		
  /* pixel_width = */ 128,
  /* pixel_height = */ 64
};

/*  https://github.com/SeeedDocument/Grove_OLED_1.12/raw/master/resources/LY120-096096.pdf */
/*  http://www.seeedstudio.com/wiki/index.php?title=Twig_-_OLED_96x96 */
/* values from u8glib */
/*
  Re-map setting in Graphic Display Data RAM, command 0x0a0
    Bit 0: Column Address Re-map
    Bit 1: Nibble Re-map
    Bit 2: Horizontal/Vertical Address Increment
    Bit 3: Not used, must be 0
    
    Bit 4: COM Re-map
    Bit 5: Not used, must be 0
    Bit 6: COM Split Odd Even
    Bit 7: Not used, must be 0
*/


static const uint8_t u8x8_d_st7588_128x64_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  U8X8_CA(0x0fd, 0x012),		/* unlock display, usually not required because the display is unlocked after reset */
  U8X8_C(0x0ae),		                /* display off */
  //U8X8_CA(0x0a8, 0x03f),		/* multiplex ratio: 0x03f * 1/64 duty */
  U8X8_CA(0x0a8, 0x05f),		/* multiplex ratio: 0x05f * 1/64 duty */
  U8X8_CA(0x0a1, 0x000),		/* display start line */
  //U8X8_CA(0x0a2, 0x04c),		/* display offset, shift mapping ram counter */
  
  U8X8_CA(0x0a2, 0x020),		/* display offset, shift mapping ram counter */
  U8X8_CA(0x0a0, 0x051),		/* remap configuration */
  
  
  U8X8_CA(0x0ab, 0x001),		/* Enable internal VDD regulator (RESET) */
  //U8X8_CA(0x081, 0x070),		/* contrast, brightness, 0..128 */
  U8X8_CA(0x081, 0x053),		/* contrast, brightness, 0..128 */
  //U8X8_CA(0x0b1, 0x055),                    /* phase length */
  U8X8_CA(0x0b1, 0x051),                    /* phase length */  
  //U8X8_CA(0x0b3, 0x091),		/* set display clock divide ratio/oscillator frequency (set clock as 135 frames/sec) */			
  U8X8_CA(0x0b3, 0x001),		/* set display clock divide ratio/oscillator frequency  */			
  
  //? U8X8_CA(0x0ad, 0x002),		/* master configuration: disable embedded DC-DC, enable internal VCOMH */
  //? U8X8_C(0x086),				/* full current range (0x084, 0x085, 0x086) */
  
  U8X8_C(0x0b9),				/* use linear lookup table */

  //U8X8_CA(0x0bc, 0x010),                    /* pre-charge voltage level */
  U8X8_CA(0x0bc, 0x008),                    /* pre-charge voltage level */
  //U8X8_CA(0x0be, 0x01c),                     /* VCOMH voltage */
  U8X8_CA(0x0be, 0x007),                     /* VCOMH voltage */
  U8X8_CA(0x0b6, 0x001),		/* second precharge */
  U8X8_CA(0x0d5, 0x062),		/* enable second precharge, internal vsl (bit0 = 0) */


  
  U8X8_C(0x0a4),				/* normal display mode */
    
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


static const uint8_t u8x8_d_st7588_jlx12864_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CA(0x0a2, 0x020),		/* display offset, shift mapping ram counter */
  U8X8_CA(0x0a0, 0x051),		/* remap configuration */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7588_jlx12864_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CA(0x0a2, 0x060),		/* display offset, shift mapping ram counter */
  U8X8_CA(0x0a0, 0x042),		/* remap configuration */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


uint8_t u8x8_d_st7588_jlx12864(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  if ( u8x8_d_st7588_128x64_generic(u8x8, msg, arg_int, arg_ptr) != 0 )
    return 1;
  if ( msg == U8X8_MSG_DISPLAY_SETUP_MEMORY )
  {
    u8x8_d_helper_display_setup_memory(u8x8, &u8x8_st7588_128x64_display_info);
    return 1;
  }
  else if ( msg == U8X8_MSG_DISPLAY_INIT )
  {
    u8x8_d_helper_display_init(u8x8);
    u8x8_cad_SendSequence(u8x8, u8x8_d_st7588_128x64_init_seq);    
    return 1;
  }
  else if  ( msg == U8X8_MSG_DISPLAY_SET_FLIP_MODE )
  {
    if ( arg_int == 0 )
    {
      u8x8_cad_SendSequence(u8x8, u8x8_d_st7588_jlx12864_flip0_seq);
      u8x8->x_offset = u8x8->display_info->default_x_offset;
    }
    else
    {
      u8x8_cad_SendSequence(u8x8, u8x8_d_st7588_jlx12864_flip1_seq);
      u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
    }
    return 1;
  }
  return 0;
}


