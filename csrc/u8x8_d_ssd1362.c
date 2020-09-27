/*

  u8x8_d_ssd1362.c
  
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


  256x64 pixel, 16 grey levels(two pixel per byte in CGRAM)
  
*/
#include "u8x8.h"

static const uint8_t u8x8_d_ssd1362_powersave0_seq[] = {
  U8X8_START_TRANSFER(),      /* enable chip, delay is part of the transfer start */
  U8X8_C(0x0af),		          /* ssd1362: display on */
  U8X8_END_TRANSFER(),        /* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1362_powersave1_seq[] = {
  U8X8_START_TRANSFER(),      /* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ae),		          /* ssd1362: display off */
  U8X8_END_TRANSFER(),        /* disable chip */
  U8X8_END()             			/* end of sequence */
};

#ifdef U8X8_WITH_SET_GREY
static uint8_t u8x8_d_ssd1362_grey_level=0xff;
#endif

/* input: one byte 8px  output: four bytes 8px 4bit grey */
static uint8_t u8x8_d_ssd1362_8px_byte_conversion(u8x8_t *u8x8, uint8_t b, uint8_t *buf){
  static uint8_t map[4] = { 0, 0x0f, 0x0f0, 0x0ff };
#ifdef U8X8_WITH_SET_GREY
  uint8_t grey = (u8x8_d_ssd1362_grey_level>>4);//&0x0f; /* 16 grey levels */
  map[0] = 0;
  map[1] = grey;
  map[2] = grey<<4;
  map[3] = grey+(grey<<4);
#endif
  buf [3] = map[b & 3];
  b>>=2;
  buf [2] = map[b & 3];
  b>>=2;
  buf [1] = map[b & 3];
  b>>=2;
  buf [0] = map[b & 3];
  return 4;
}

uint8_t u8x8_d_ssd1362_draw_tile(u8x8_t *u8x8, uint8_t arg_int, void *arg_ptr){
  uint8_t x, y, r, c, i; 
  uint8_t *ptr;

  u8x8_cad_StartTransfer(u8x8);

  x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
  x *= 4;		// only every other col can be addressed 8/2 = 4 bytes 8px per tile
  x += u8x8->x_offset;		

  y = (((u8x8_tile_t *)arg_ptr)->y_pos);
  y *= 8; //8px per tile = 8 rows

  c = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles */
  ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;	/* data ptr to the tiles */

#if(0)
//#ifndef U8G2_H
  uint8_t rbuf[8];
  for( i = 0; i < 8; i++ ){ /* u8x8 needs 90 degree rotation */
    rbuf[i] =((*(ptr)  &(1<<i))>>i)<<7;
    rbuf[i]+=((*(ptr+1)&(1<<i))>>i)<<6;
    rbuf[i]+=((*(ptr+2)&(1<<i))>>i)<<5;
    rbuf[i]+=((*(ptr+3)&(1<<i))>>i)<<4;
    rbuf[i]+=((*(ptr+4)&(1<<i))>>i)<<3;
    rbuf[i]+=((*(ptr+5)&(1<<i))>>i)<<2;
    rbuf[i]+=((*(ptr+6)&(1<<i))>>i)<<1;
    rbuf[i]+=((*(ptr+7)&(1<<i))>>i);
  }
#endif
  static uint8_t buf[4];
  for( i = 0; i < 8; i++ ) {
    u8x8_cad_SendCmd(u8x8, 0x15 ); /* set column start, end address */
    u8x8_cad_SendCmd(u8x8, x );
    u8x8_cad_SendCmd(u8x8, 0x7f);//((x+4*c)<(0x7f-1)?x+c*4-1:0x7f));
    u8x8_cad_SendCmd(u8x8, 0x75 );	/* set row start,end address */
    u8x8_cad_SendArg(u8x8, y);
    u8x8_cad_SendArg(u8x8, 0x3f);//(y<(0x3f-7)?y+7:0x3f));
#if(0)
//#ifndef U8G2_H
    u8x8_d_ssd1362_8px_byte_conversion(u8x8, rbuf[i], buf); /*1 byte mono to 4 byte grey */
    
    r=arg_int; /* number of repeat tile copies */
    while (r>0) {
      c = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles */
      while ( c > 0 ){
        u8x8_cad_SendData(u8x8, 4, buf);
        c--;
      }
      r--;
    }
#else
    while ( c > 0 ){    
      u8x8_d_ssd1362_8px_byte_conversion(u8x8, *ptr, buf); /*1 byte mono to 4 byte grey */
      u8x8_cad_SendData(u8x8, 4, buf);
      c--;
      ptr++;
    }
    c = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles */
    ptr += u8x8->display_info->tile_width-c; /*point to next row for draw area*/
#endif
    y++;
  }
  u8x8_cad_EndTransfer(u8x8);

}

/*=========================================================*/
static const u8x8_display_info_t u8x8_ssd1362_256x64_display_info = {
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 45,
  /* pre_chip_disable_wait_ns = */ 20,
  /* reset_pulse_width_ms = */ 10, 	/* ssd1362: 100us */
  /* post_reset_wait_ms = */ 10, 	/* ssd1362: 100us */
  /* sda_setup_time_ns = */ 50,		/* ssd1362: cycle time is 100ns, so use 100/2 */
  /* sck_pulse_width_ns = */ 50,	/* ssd1362: cycle time is 100ns, so use 100/2 */
  /* sck_clock_hz = */ 10000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns  */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 15,
  /* write_pulse_width_ns = */ 150,	/* ssd1362: cycle time is 300ns, so use 300/2 = 150 */
  /* tile_width = */ 32,		/* 256 pixel, so we require 32 bytes for this */
  /* tile_hight = */ 8,
  /* default_x_offset = */ 0,	/* this is the byte offset (there are two pixel per byte with 4 bit per pixel) */
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 256,
  /* pixel_height = */ 64,
  /* pixel_byte_horizontal = */ 1
};


static const uint8_t u8x8_d_ssd1362_256x64_init_seq[] = {
  U8X8_DLY(1),
  U8X8_START_TRANSFER(),        /* enable chip, delay is part of the transfer start */
  U8X8_DLY(1),
  
  U8X8_CA(0xfd, 0x12),          /* unlock commands */
  U8X8_C(0xae),		              /* display off */
  U8X8_CAA(0x15, 0x00, 0x7f),   /* columns */
  U8X8_CAA(0x75, 0x00, 0x3f),   /* rows */
  U8X8_CA(0x81, 0x7f),			    /* set display contrast  */
  U8X8_CA(0xa0, 0xc3),			    /* remap  */
  U8X8_CA(0xa1, 0x00),	        /* display start line */
  U8X8_CA(0xa2, 0x00),	        /* display Offset COMn */
  U8X8_C(0xa4),	                /* normal display */
  U8X8_CA(0xa8, 0x3f),	        /* set multiplex Ratio */
  U8X8_CA(0xab, 0x01),	        /* set VDD regulator */
//  U8X8_CA(0xad, 0x8e),	        /* external IREF */
  U8X8_CA(0xad, 0x9e),	        /* internal IREF */
  U8X8_CA(0xb1, 0x22),	        /* set phase length */
  U8X8_CA(0xb3, 0xa0),	        /* display clock devider */
  U8X8_CA(0xb6, 0x04),	        /* set 2nd pre-charge period */
  U8X8_C(0xb9),	                /* set linear LUT */
  U8X8_CA(0xbc, 0x10),	        /* set pre-charge voltage level */
  U8X8_CA(0xbd, 0x01),	        /* external Vp capacitor */
  U8X8_CA(0xbe, 0x07),	        /* set COM deselect voltage level */

  U8X8_DLY(1),					        /* delay  */
  U8X8_END_TRANSFER(),          /* disable chip */
  U8X8_END()             			  /* end of sequence */
};

static const uint8_t u8x8_d_ssd1362_256x64_flip0_seq[] = {
  U8X8_START_TRANSFER(),  /* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a0),		      /* remap */
  U8X8_C(0x072),		      /* remap */
  U8X8_END_TRANSFER(),    /* disable chip */
  U8X8_END()              /* end of sequence */
};

static const uint8_t u8x8_d_ssd1362_256x64_flip1_seq[] = {
  U8X8_START_TRANSFER(),  /* enable chip, delay is part of the transfer start */
  U8X8_C(0x0a0),		      /* remap */
  U8X8_C(0x0c3),		      /* remap */
  U8X8_END_TRANSFER(),    /* disable chip */
  U8X8_END()              /* end of sequence */
};

uint8_t u8x8_d_ssd1362_256x64(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  switch(msg){
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ssd1362_256x64_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362_256x64_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 ){
        u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362_256x64_flip0_seq);
        u8x8->x_offset = u8x8->display_info->default_x_offset;
      }else{
        u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362_256x64_flip1_seq);
        u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
        u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362_powersave0_seq);
      else
        u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362_powersave1_seq);
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x81 );
      u8x8_cad_SendArg(u8x8, arg_int );	/* ssd1362 has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
#ifdef U8X8_WITH_SET_GREY
    case U8X8_MSG_DISPLAY_SET_GREY:
      u8x8_d_ssd1362_grey_level = arg_int; /* set display draw grey level */
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_d_ssd1362_draw_tile(u8x8, arg_int, arg_ptr);
      break;
    default:
      return 0;
  }
  return 1;
}