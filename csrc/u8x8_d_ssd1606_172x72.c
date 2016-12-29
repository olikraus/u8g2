/*

  u8x8_d_ssd1606_172x72.c

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
  
  SSD1606: 128x180x2
  
*/


#include "u8x8.h"



/* GDE021A1, 2.1" EPD */
static const uint8_t u8x8_d_ssd1606_172x72_gde021a1_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */

  U8X8_CA(0x10, 0x00),	/* Deep Sleep mode Control: Disable */
  U8X8_CA(0x11, 0x03),	/* Define data entry mode, x&y inc, x first */
  U8X8_CAA(0x44, 0, 31),	/* RAM x start & end, each byte has 4 pixel, 32*4=128 */
  U8X8_CAA(0x45, 0, 179),	/* RAM y start & end, 179 MAX */
  
  U8X8_CA(0x4e, 0),	/* set x pos, 0..31 */
  U8X8_CA(0x4f, 0),	/* set y pos, 0...179 */

  U8X8_CA(0xf0, 0x1f),	/* set booster feedback to internal */
  U8X8_CA(0x22, 0xc0),	/* display update seq. option: enable clk, enable CP, .... todo: this is never activated */
  
  U8X8_C(0x32),	/* write LUT register*/

  /* wavefrom part of the LUT */
  U8X8_A4(0x00,0x00,0x00,0x55),  /* step 0 */
  U8X8_A4(0x00,0x00,0x55,0x55),	/* step 1 */
  U8X8_A4(0x00,0x55,0x55,0x55),
  U8X8_A4(0xAA,0xAA,0xAA,0xAA),
  U8X8_A4(0x15,0x15,0x15,0x15),
  U8X8_A4(0x05,0x05,0x05,0x05),
  U8X8_A4(0x01,0x01,0x01,0x01),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),
  U8X8_A4(0x00,0x00,0x00,0x00),	/* step 19 */
  
  /* timing part of the LUT */
  U8X8_A8(0x22,0xFB,0x22,0x1B,0x00,0x00,0x00,0x00),
  U8X8_A(0x00),U8X8_A(0x00),
  
  U8X8_CA(0x2c, 0xa0),	/* write vcom value*/
  U8X8_CA(0x3c, 0x63),	/* select boarder waveform */
  U8X8_CA(0x22, 0xc4),	/* display update seq. option: clk -> CP -> LUT -> initial display -> pattern display */
    /* 0x0c4 is mentioned in chapter 9.2 of the GDE021A1 data sheet */
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1606_to_display_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CA(0x22, 0xc4),	/* display update seq. option: clk -> CP -> LUT -> initial display -> pattern display */
  U8X8_C(0x20),	/* execute sequence */
  U8X8_DLY(250),	/* the sequence above requires about 970ms */
  U8X8_DLY(250),
  U8X8_DLY(250),
  U8X8_DLY(250),
  U8X8_CA(0x22, 0x03),	/* disable clock and charge pump */
  U8X8_DLY(200),		/* this requres about 370ms */
  U8X8_DLY(200),  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1606_172x72_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_C(0x0af),		                /* display on */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1606_172x72_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_C(0x0ae),		                /* display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1606_172x72_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_C(0x0a1),				/* segment remap a0/a1*/
  //U8X8_C(0x0c8),				/* c0: scan dir normal, c8: reverse */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1606_172x72_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_C(0x0a0),				/* segment remap a0/a1*/
  //U8X8_C(0x0c0),				/* c0: scan dir normal, c8: reverse */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};


static uint8_t *u8x8_convert_tile_for_ssd1606(uint8_t *t)
{
  uint8_t i;
  uint16_t r;
  static uint8_t buf[16];
  uint8_t *pbuf = buf;

  for( i = 0; i < 8; i++ )
  {
    r = u8x8_upscale_byte(*t++);
    *pbuf++ = r & 255;
    r >>= 8;
    *pbuf++ = r;
  }
  return buf;
}


static uint8_t u8x8_d_ssd1606_172x72_generic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, c, page;
  uint8_t *ptr;
  switch(msg)
  {
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ssd1606_172x72_display_info);
      break;
    */
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1606_172x72_gde021a1_init_seq);    


      u8x8_cad_SendCmd(u8x8, 0x044 );
      u8x8_cad_SendArg(u8x8, 0 );
      u8x8_cad_SendArg(u8x8, 31 );
      u8x8_cad_SendCmd(u8x8, 0x045 );
      u8x8_cad_SendArg(u8x8, 0 );
      u8x8_cad_SendArg(u8x8, 179 );

      u8x8_cad_SendCmd(u8x8, 0x04e );
      u8x8_cad_SendArg(u8x8, 4 );
      u8x8_cad_SendCmd(u8x8, 0x04f );
      u8x8_cad_SendArg(u8x8, 4 );
    
      u8x8_cad_SendCmd(u8x8, 0x024 );
      u8x8_cad_SendArg(u8x8, 0xff );    
      u8x8_cad_SendArg(u8x8, 0xff );
      u8x8_cad_SendArg(u8x8, 0xff );    
      u8x8_cad_SendArg(u8x8, 0xff );
    
      u8x8_cad_SendArg(u8x8, 0xf0 );    
      u8x8_cad_SendArg(u8x8, 0xf0 );
      u8x8_cad_SendArg(u8x8, 0xf0);    
      u8x8_cad_SendArg(u8x8, 0xf0);
    
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1606_to_display_seq);    

    
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1606_172x72_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1606_172x72_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1606_172x72_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1606_172x72_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int );	/* ssd1306 has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);

      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8x8->x_offset;
    
      page = u8x8->display_info->tile_height;
      page --;
      page -= (((u8x8_tile_t *)arg_ptr)->y_pos);
      page *= 2;
    


      u8x8_cad_SendCmd(u8x8, 0x011 );	/* cursor increment mode */
      u8x8_cad_SendArg(u8x8, 7);

      u8x8_cad_SendCmd(u8x8, 0x045 );	/* window start column */
      u8x8_cad_SendArg(u8x8, 0);
      u8x8_cad_SendArg(u8x8, 171);		/* end of display */
    
      u8x8_cad_SendCmd(u8x8, 0x044 );	/* window end page */
      u8x8_cad_SendArg(u8x8, page);
      u8x8_cad_SendArg(u8x8, page+1);

      u8x8_cad_SendCmd(u8x8, 0x04f );	/* window column */
      u8x8_cad_SendArg(u8x8, x);

      u8x8_cad_SendCmd(u8x8, 0x04e );	/* window column */
      u8x8_cad_SendArg(u8x8, page);

      do
      {
	c = ((u8x8_tile_t *)arg_ptr)->cnt;
	ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
	do
	{
	  u8x8_cad_SendData(u8x8, 16, u8x8_convert_tile_for_ssd1606(ptr));
	  ptr += 8;
	  x += 8;
	  c--;
	} while( c > 0 );
	
	arg_int--;
      } while( arg_int > 0 );

      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1606_to_display_seq);    
      
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}


static const u8x8_display_info_t u8x8_ssd1606_172x72_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 120,
  /* pre_chip_disable_wait_ns = */ 60,
  /* reset_pulse_width_ms = */ 100, 	
  /* post_reset_wait_ms = */ 100, 
  /* sda_setup_time_ns = */ 50,		/* SSD1606: */
  /* sck_pulse_width_ns = */ 100,	/* SSD1606: 100ns */
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 40,
  /* write_pulse_width_ns = */ 150,	
  /* tile_width = */ 22,		/* 22*8 = 176 */
  /* tile_hight = */ 9,		/* 9*8 = 72 */
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 172,
  /* pixel_height = */ 72		
};

uint8_t u8x8_d_ssd1606_172x72(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    if ( msg == U8X8_MSG_DISPLAY_SETUP_MEMORY )
    {
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ssd1606_172x72_display_info);
      return 1;
    }
    return u8x8_d_ssd1606_172x72_generic(u8x8, msg, arg_int, arg_ptr);
}


