/*

  u8x8_d_sh1122.c
  
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

static uint8_t u8x8_d_sh1122_parent_class=0x00; /* 0=u8x8, 1=u8g2 */
#ifdef U8X8_WITH_SET_GREY
static uint8_t u8x8_d_sh1122_grey_level=0xff;  /* 2px 16 grey levels */
#endif
/*=========================================================*/
static uint8_t u8x8_d_sh1122_8px_byte_conversion(u8x8_t *u8x8, uint8_t pixel_byte, uint8_t *buf){ /* input: one byte 8px; output: four bytes 8px 4bit grey */
#ifdef U8X8_WITH_SET_GREY
  buf[0] = u8x8_d_sh1122_grey_level;
  if( !(pixel_byte&128) ) buf[0] &= 0x0f;
  if( !(pixel_byte&64) )  buf[0] &= 0xf0;
  buf[1] = u8x8_d_sh1122_grey_level;
  if( !(pixel_byte&32) )  buf[1] &= 0x0f;
  if( !(pixel_byte&16) )  buf[1] &= 0xf0;
  buf[2] = u8x8_d_sh1122_grey_level;
  if( !(pixel_byte&8) )   buf[2] &= 0x0f;
  if( !(pixel_byte&4) )   buf[2] &= 0xf0;
  buf[3] = u8x8_d_sh1122_grey_level;
  if( !(pixel_byte&2) )   buf[3] &= 0x0f;
  if( !(pixel_byte&1) )   buf[3] &= 0xf0;
#else
  buf[0] = 0xff;
  if( !(pixel_byte&128) ) buf[0] &= 0x0f;
  if( !(pixel_byte&64) )  buf[0] &= 0xf0;
  buf[1] = 0xff;
  if( !(pixel_byte&32) )  buf[1] &= 0x0f;
  if( !(pixel_byte&16) )  buf[1] &= 0xf0;
  buf[2] = 0xff;
  if( !(pixel_byte&8) )   buf[2] &= 0x0f;
  if( !(pixel_byte&4) )   buf[2] &= 0xf0;
  buf[3] = 0xff;
  if( !(pixel_byte&2) )   buf[3] &= 0x0f;
  if( !(pixel_byte&1) )   buf[3] &= 0xf0;  
#endif
  return 4;
}

uint8_t u8x8_d_sh1122_draw_tile(u8x8_t *u8x8, uint8_t arg_int, void *arg_ptr){
  uint8_t col, row, tile_rep, tile_cnt, i;
  uint8_t *ptr;
  uint8_t rbuf[(u8x8_d_sh1122_parent_class == u8x8_parent_class?8:0)]; /* 8 bytes for tile rotation when parent class is u8x8 */
  static uint8_t buf[4]; /* 4 bytes for a tile - 8px columns  */
 
  col = ((u8x8_tile_t *)arg_ptr)->x_pos;
  col *= 8/2;		/* 2px per column byte - 8px per tile = 4 columns(bytes) */
  col += u8x8->x_offset;
  row = (((u8x8_tile_t *)arg_ptr)->y_pos);
  row *= 8; /* 1px per row - 8px per tile = 8 rows */
  tile_cnt = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles to send to display */
  ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;	/* buffer pointer to 1st tile to send to display */
  
  if( u8x8_d_sh1122_parent_class == u8x8_parent_class ){ /*parent class u8x8, sends individual tiles, but it needs to rotated by 90 degrees */
    for( i = 0; i < 8; i++ ){
      rbuf[i] =((*(ptr)  &(1<<i))>>i)<<7;
      rbuf[i]+=((*(ptr+1)&(1<<i))>>i)<<6;
      rbuf[i]+=((*(ptr+2)&(1<<i))>>i)<<5;
      rbuf[i]+=((*(ptr+3)&(1<<i))>>i)<<4;
      rbuf[i]+=((*(ptr+4)&(1<<i))>>i)<<3;
      rbuf[i]+=((*(ptr+5)&(1<<i))>>i)<<2;
      rbuf[i]+=((*(ptr+6)&(1<<i))>>i)<<1;
      rbuf[i]+=((*(ptr+7)&(1<<i))>>i);
    }
  }
  u8x8_cad_StartTransfer(u8x8);
  for( i = 0; i < 8; i++ ) {
    u8x8_cad_SendCmd(u8x8, col & 15 );	/* lower 4 bit*/
    u8x8_cad_SendCmd(u8x8, 0x10 | (col >> 4) );	/* higher 3 bit */
    u8x8_cad_SendCmd(u8x8, 0xb0 );	/* set row address */
    u8x8_cad_SendArg(u8x8, row);

    if( u8x8_d_sh1122_parent_class == u8g2_parent_class ){ /* parent class u8g2 */
      while ( tile_cnt > 0 ){ /* number of tiles to send; tile repeats=1 for u8g2*/
        u8x8_d_sh1122_8px_byte_conversion(u8x8, *ptr, buf); /* 1 byte mono to 4 byte grey */
        u8x8_cad_SendData(u8x8, 4, buf);
        tile_cnt--;
        ptr++;
      }
      tile_cnt = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles */
      ptr += u8x8->display_info->tile_width;
      ptr -= tile_cnt; /* point to next row for draw area */

    }else if( u8x8_d_sh1122_parent_class == u8x8_parent_class ){ /* parent class u8x8 */
      u8x8_d_sh1122_8px_byte_conversion(u8x8, rbuf[i], buf); /* 1 byte mono to 4 byte grey */
      tile_rep = arg_int; /* number of repeats of same tile to send to display */
      while (tile_rep>0) {  /* number of repeats of same tile to send; number of tiles=1 in u8x8 */
//        tile_cnt = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles */
//        while ( tile_cnt > 0 ){
          u8x8_cad_SendData(u8x8, 4, buf);
//          tile_cnt--;
//        }
        tile_rep--;
      }
    }
    row++;
  }
  u8x8_cad_EndTransfer(u8x8);
}
/*=========================================================*/
static const u8x8_display_info_t u8x8_sh1122_256x64_display_info = {
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  /* post_chip_enable_wait_ns = */ 40,
  /* pre_chip_disable_wait_ns = */ 10,
  /* reset_pulse_width_ms = */ 10, 	/* sh1122: 10 us */
  /* post_reset_wait_ms = */ 10, 	/* sh1122: 2us */
  /* sda_setup_time_ns = */ 150,		/* sh1122: cycle time is 250ns, so use 300/2 */
  /* sck_pulse_width_ns = */ 150,	/* sh1122: cycle time is 250ns, so use 300/2 */
  /* sck_clock_hz = */ 40000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns  */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 40,
  /* write_pulse_width_ns = */ 150,	/* sh1122: cycle time is 300ns, so use 300/2 = 150 */
  /* tile_width = */ 32,		/* 256 pixel, so we require 32 bytes for this */
  /* tile_hight = */ 8,
  /* default_x_offset = */ 0,	/* this is the byte offset (there are two pixel per byte with 4 bit per pixel) */
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 256,
  /* pixel_height = */ 64
};

static const uint8_t u8x8_d_sh1122_powersave0_seq[] = {
  U8X8_START_TRANSFER(),    /* enable chip, delay is part of the transfer start */
  U8X8_C(0xaf),		          /* display on - normal mode */
  U8X8_END_TRANSFER(),      /* disable chip */
  U8X8_END()             		/* end of sequence */
};

static const uint8_t u8x8_d_sh1122_powersave1_seq[] = {
  U8X8_START_TRANSFER(),    /* enable chip, delay is part of the transfer start */
  U8X8_C(0xae),		          /* display off - some commands only work when display off! */
  U8X8_END_TRANSFER(),      /* disable chip */
  U8X8_END()             		/* end of sequence */
};

static const uint8_t u8x8_d_sh1122_256x64_flip0_seq[] = {
  U8X8_START_TRANSFER(),    /* enable chip, delay is part of the transfer start */
  U8X8_C(0xa0),		          /* enable right rotation */
  U8X8_C(0xc0),		          /* normal COM scan */
  U8X8_C(0x40),             /* 0x00 - display start line - RAM vertical shift */
  U8X8_END_TRANSFER(),      /* disable chip */
  U8X8_END()                /* end of sequence */
};

static const uint8_t u8x8_d_sh1122_256x64_flip1_seq[] = {
  U8X8_START_TRANSFER(),    /* enable chip, delay is part of the transfer start */
  U8X8_C(0xa1),		          /* enable left rotation */
  U8X8_C(0xc8),		          /* reverse COM scan */
  U8X8_C(0x60),             /* 0x20 - display start line - RAM vertical shift */
  U8X8_END_TRANSFER(),      /* disable chip */
  U8X8_END()                /* end of sequence */
};

/* Midas MCOT256064DY-WM OLED 256x64 16 levels of grey */
static const uint8_t u8x8_d_sh1122_256x64_init_seq[] = {
  U8X8_DLY(1),
  U8X8_START_TRANSFER(),    /* enable chip, delay is part of the transfer start */
  U8X8_DLY(1),

  U8X8_C(0xae),		          /*POR 0xae; 0/1; display off - some commands only work when display off! */
///  U8X8_C(0xaf),		          /* display on - normal mode */
  U8X8_C(0x00),             /*POR 0x00; 0x0n 0..15 Nn=N*16+n=0..127; column RAM address */
  U8X8_C(0x10),             /*POR 0x10; 0x1N 0..7  Nn=N*16+n=0..127; column RAM address */
  U8X8_CA(0xb0, 0x00),      /*POR 0x00; 0..63; row RAM address */
  U8X8_CA(0xd3, 0x00),			/*POR 0x00; 0..63; set display offset - COM vertical shift */
#if(0) /* removed as setFlipMode(0) in .begin constructor(s), otherwise display flicks about when processor reset and using u8g2.beginSimple() for silent resets */
  U8X8_C(0x40),				      /*POR 0x40; 0..63; display start line - RAM vertical shift */
  U8X8_C(0xa0),		          /*POR 0xa0; 0/1; enable right rotation */
///  U8X8_C(0xa1),		          /* enable left rotation */
  U8X8_C(0xc0),		          /*POR 0xc0; 0/8; normal COM scan */
///  U8X8_C(0xc8),		          /* reverse COM scan */
#endif
///  U8X8_CA(0x81, 0x38),
  U8X8_CA(0x81, 0x80),			/*POR 0x80; 0..255; set display contrast */
  U8X8_C(0xa4),             /*POR 0xa4; 0/1; normal display */
///  U8X8_C(0xa5),             /* entire display ON */
  U8X8_C(0xa6),             /*POR 0xa6; 0/1; normal display */
///  U8X8_C(0xa7),             /* enable inverse display */
  U8X8_CA(0xa8, 0x3f),			/*POR 0x3f; 0..63; multiplex MUX ratio - active rows 1-64 */
///  U8X8_CA(0xad, 0x81),			/*POR 0x81; 0x8Nn=0b1000NNNn 0..7,0..1; enable builtin DC-DC & frequency - display needs to be off */
  U8X8_CA(0xad, 0x80),      /* disable builtin DC-DC - display needs to be off */
//  U8X8_CA(0xad, 0x8N),			/*0x0N N=0..7<<1; set builtin DC-DC frequency = (0.6+0.1*N)*500 kHz */
//  U8X8_CA(0xad, 0x8n),			/*0x0n n=0/1; enable(1) builtin DC-DC */
  U8X8_CA(0xd5, 0x90),      /* set display clock(fosc*1.2) & divide(dclk/1) */
///  U8X8_CA(0xd5, 0x50),			/*POR 0x50; 0xNn N=0..15,n=0..15; set display clock & divide */
//  U8X8_CA(0xd5, 0x0n),			/*0x0n n=0..15; set display divide ratio = dclk/(1+n) */
//  U8X8_CA(0xd5, 0xN0),			/*0xN0 N=0..15<<4; set display clock frequency = fosc*(0.75+0.05*N) */
  U8X8_CA(0xd9, 0x44),      /* discharge(4*dclk) & precharge(12*dclk) period */
///  U8X8_CA(0xd9, 0x22),			/*POR 0x22; 0xNn N=0..15,n=0..15; discharge & precharge period */
//  U8X8_CA(0xd9, 0x0n),			/*0x0n n=1..15; precharge period = n*dclk */
//  U8X8_CA(0xd9, 0xN0),			/*0xN0 N=1..15<<4; discharge period = n*dclk */
  U8X8_CA(0xdb, 0x3b),      /* VCOM level - deselect voltage(0.808485*VREF) */
///  U8X8_CA(0xdb, 0x35),			/*POR 0x35; n=0..255; VCOM level - deselect voltage; VCOM = β1*VREF = (0.43+n*0.006415)*VREF */
//  U8X8_CA(0xdc, 0x1a),      /* VSEGM level - precharge voltage(0.59679*VREF) */
  U8X8_CA(0xdc, 0x35),			/*POR 0x35; 0..255; VSEGM level - precharge voltage; VSEGM = β2*VREF = (0.43+n*0.006415)*VREF  */
  U8X8_C(0x030),				    /*POR 0x30; n=0..15; VSL level - discharge voltage(0); VSL = n==0?0:(0.75+0.25*n) */

  U8X8_DLY(1),					    /* delay  */
  U8X8_END_TRANSFER(),      /* disable chip */
  U8X8_END()             	  /* end of sequence */
};
/*=========================================================*/
uint8_t u8x8_d_sh1122_256x64(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  switch(msg) {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_sh1122_256x64_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_sh1122_parent_class = arg_int;  /* 0=u8x8, 1=u8g2 */
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_sh1122_256x64_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 ){
        u8x8_cad_SendSequence(u8x8, u8x8_d_sh1122_256x64_flip0_seq);
        u8x8->x_offset = u8x8->display_info->default_x_offset;
      }else{
        u8x8_cad_SendSequence(u8x8, u8x8_d_sh1122_256x64_flip1_seq);
        u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
        u8x8_cad_SendSequence(u8x8, u8x8_d_sh1122_powersave0_seq);
      else
        u8x8_cad_SendSequence(u8x8, u8x8_d_sh1122_powersave1_seq);
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x81 );
      u8x8_cad_SendArg(u8x8, arg_int );	/* device's contrast range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
#ifdef U8X8_WITH_SET_GREY
    case U8X8_MSG_DISPLAY_SET_GREY:
      u8x8_d_sh1122_grey_level = arg_int>>4; /* device's pixel grey level range from 0 to 15 */
      u8x8_d_sh1122_grey_level |= (u8x8_d_sh1122_grey_level<<4); /* 2 pixel mask */
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_d_sh1122_draw_tile(u8x8, arg_int, arg_ptr);
      break;
    default:
      return 0;
  }
  return 1;
}
