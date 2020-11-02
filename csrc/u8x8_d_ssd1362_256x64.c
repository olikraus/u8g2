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

#ifdef U8X8_WITH_SET_GREY
static uint8_t u8x8_d_ssd1362_grey_level=0xff; /* 2px 16 grey levels */
#endif

static uint8_t u8x8_d_ssd1362_8px_byte_conversion(u8x8_t *u8x8, uint8_t pixel_byte, uint8_t *buf){ /* input: one byte 8px; output: four bytes 8px 4bit grey */
#ifdef U8X8_WITH_SET_GREY
  buf[0] = u8x8_d_ssd1362_grey_level;
  if( !(pixel_byte&128) ) buf[0] &= 0x0f;
  if( !(pixel_byte&64) )  buf[0] &= 0xf0;
  buf[1] = u8x8_d_ssd1362_grey_level;
  if( !(pixel_byte&32) )  buf[1] &= 0x0f;
  if( !(pixel_byte&16) )  buf[1] &= 0xf0;
  buf[2] = u8x8_d_ssd1362_grey_level;
  if( !(pixel_byte&8) )   buf[2] &= 0x0f;
  if( !(pixel_byte&4) )   buf[2] &= 0xf0;
  buf[3] = u8x8_d_ssd1362_grey_level;
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

uint8_t u8x8_d_ssd1362_draw_tile(u8x8_t *u8x8, uint8_t arg_int, void *arg_ptr){
  uint8_t col, row, tile_buffer_width, tile_cnt, copies, i; 
  uint8_t *ptr;
  static uint8_t buf[4]; /* 4 bytes for a tile - 8px columns  */

  col = ((u8x8_tile_t *)arg_ptr)->x_pos;
  col *= 8/2;		/* 2px per column byte - 8px per tile = 4 columns(bytes) */
  col += u8x8->x_offset;
  row = (((u8x8_tile_t *)arg_ptr)->y_pos);
  row *= 8; /* 1px per row - 8px per tile = 8 rows */
  tile_buffer_width = ((u8x8_tile_t *)arg_ptr)->buffer_width;	/* tile width of buffer */

  u8x8_cad_StartTransfer(u8x8);
  for( i = 0; i < 8; i++ ) {
    u8x8_cad_SendCmd(u8x8, 0x15 ); /* set column start, end address */
    u8x8_cad_SendCmd(u8x8, col );
    u8x8_cad_SendCmd(u8x8, 0x7f);
    u8x8_cad_SendCmd(u8x8, 0x75 );	/* set row start,end address */
    u8x8_cad_SendArg(u8x8, row);
    u8x8_cad_SendArg(u8x8, 0x3f);
    copies = arg_int;
    do {
      tile_cnt = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles to send to display */
      ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
      ptr += i * tile_buffer_width;	/* loads at start and when tiles repeats */
      while ( tile_cnt > 0){ 
        u8x8_d_ssd1362_8px_byte_conversion(u8x8, *ptr, buf); /* 1 byte mono to 4 byte grey */
        u8x8_cad_SendData(u8x8, 4, buf);
        tile_cnt--;
        ptr++;
      }
      copies--;
    } while( copies > 0 ); /* number of copies of tile pattern */
    row++;
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
  /* pixel_height = */ 64
};

static const uint8_t u8x8_d_ssd1362_powersave0_seq[] = {
  U8X8_START_TRANSFER(),      /* enable chip, delay is part of the transfer start */
  U8X8_C(0x0af),		          /* display on - normal mode */
  U8X8_END_TRANSFER(),        /* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1362_powersave1_seq[] = {
  U8X8_START_TRANSFER(),      /* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ae),		          /* display off - sleep mode */
  U8X8_END_TRANSFER(),        /* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ssd1362_256x64_raystar_flip0_seq[] = {
  U8X8_START_TRANSFER(),        /* enable chip, delay is part of the transfer start */
  U8X8_CA(0xa0,0xc3),		        /* remap normal */
  U8X8_END_TRANSFER(),          /* disable chip */
  U8X8_END()                    /* end of sequence */
};

static const uint8_t u8x8_d_ssd1362_256x64_raystar_flip1_seq[] = {
  U8X8_START_TRANSFER(),        /* enable chip, delay is part of the transfer start */
  U8X8_CA(0xa0,0xd0),		        /* remap flipped */
///or  U8X8_CA(0xa0,0x52),		      /* remap flipped */
  U8X8_END_TRANSFER(),          /* disable chip */
  U8X8_END()                    /* end of sequence */
};

/* Raystar REX025664A-ZIF OLED 256x64 16 levels of grey */
/* Midas MDOT256064G1V-WM OLED 256x64 16 levels of grey */
static const uint8_t u8x8_d_ssd1362_256x64_raystar_init_seq[] = {
  U8X8_DLY(1),
  U8X8_START_TRANSFER(),        /* enable chip, delay is part of the transfer start */
  U8X8_DLY(1),

  U8X8_CA(0xfd, 0x12),          /*POR 0x12; 0/4; unlock command */
///  U8X8_CA(0xfd, 0x16),          /* lock command */
  U8X8_C(0xae),		              /*POR; 0/1; display off - sleep mode */
///  U8X8_C(0xaf),		              /* display on - normal mode */
  U8X8_CAA(0x15, 0x00, 0x7f),   /*POR 0x00,0x7f; 0..127,0..127; set column address range(0x15,start,end)*/
  U8X8_CAA(0x75, 0x00, 0x3f),   /*POR 0x00,0x3f; 0..63,0..63; set row address range(0x75,start,end) */
  U8X8_CA(0x81, 0x80),			    /*POR 0x7f; 0..255; set display contrast  */
#if(1) /* removed as setFlipMode(0) in .begin constructor(s), otherwise display flicks about when processor reset and using u8g2.beginSimple() for silent resets */
///  U8X8_CA(0xa0, 0x40),			    /*POR 0x40; 0bLS0C0VNA; set remap */
  U8X8_CA(0xa0, 0xc3),			    /* remap flip 0 */
//  U8X8_CA(0xa0, 0x01),			    /* A=0/1; enable column address remap */
//  U8X8_CA(0xa0, 0x02),			    /* N=0/2; enable nibble remap */
//  U8X8_CA(0xa0, 0x04),			    /* V=0/4; enable horizontal(0)/vertical(4) address increments */
//  U8X8_CA(0xa0, 0x10),			    /* C=0/16; enable COM remap */
//  U8X8_CA(0xa0, 0x40),			    /* S=0/64; enable SEG split odd even */
//  U8X8_CA(0xa0, 0x80),			    /* L=0/128; enable SEG left/right remap */
#endif
  U8X8_CA(0xa1, 0x00),	        /*POR 0x00; 0..63; display start line - RAM vertical shift */
  U8X8_CA(0xa2, 0x00),	        /*POR 0x00; 0..63; set display Offset - COM vertical shift */
  U8X8_C(0xa4),	                /*POR; normal display */
///  U8X8_C(0xa5),	                /* all pixels fully ON display */
///  U8X8_C(0xa6),	                /* all pixels OFF display */
///  U8X8_C(0xa7),	                /* enable inverse display */
  U8X8_CA(0xa8, 0x3f),	        /*POR 0x3f; 3..63; set multiplex MUX Ratio - active rows 4-64 */
  U8X8_CA(0xab, 0x01),	        /*POR 0X01; 0/1; enable internal VDD regulator */
///  U8X8_CA(0xab, 0x00),	        /* external VDD, disable internal VDD regulator */
///  U8X8_CA(0xad, 0x8e),	        /*POR 0x8e; external IREF */
  U8X8_CA(0xad, 0x9e),	        /* enable internal IREF */
  U8X8_CA(0xb1, 0x22),	        /*POR 0x82; set phase length */
//  U8X8_CA(0xb1, 0x0n),	        /*0x0n n=1..15; phase1(discharge) period = 2*n*dclk */
//  U8X8_CA(0xb1, 0xN0),	        /*0xN0 N=1..15<<4; phase2(1st precharge) period = 2*N*dclk */
  U8X8_CA(0xb3, 0xa0),	        /*POR 0xa1; display clock & divider */
//  U8X8_CA(0xb3, 0x0n),	        /*0x0n n=0..8; display clock divider = dclk>>n */
//  U8X8_CA(0xb3, 0xN0),	        /*0xN0 N=0..15<<4; display clock frequency increase n */
  U8X8_CA(0xb6, 0x04),	        /*POR 0x04; 1..15; set phase3(2nd precharge) period = n*dclk */
  U8X8_C(0xb9),	                /* set linear grey LUT */
///  U8X8_CAAA(0xb8, 12, 24, 36),  /* custom grey LUT, 0<GS1<GS2..<GS15>140, GS1, GS2, GS3 */
///  U8X8_A4(48, 60, 72, 84),      /* GS4, GS5, GS6, GS7 */
///  U8X8_A4(96, 108, 120, 132),   /* GS8, GS9, GS10,GS11 */
///  U8X8_A4(144, 156, 168, 180),  /* GS12,GS13,GS14,GS15>140 */
  U8X8_CA(0xbc, 0x10),	        /*POR 0x04; n=0..31 set pre-charge voltage level = 0.1+0.0133*n*Vcc */
///  U8X8_CA(0xbd, 0x00),	        /*POR 0x00; no external Vp capacitor */
  U8X8_CA(0xbd, 0x01),	        /* external Vp capacitor */
  U8X8_CA(0xbe, 0x07),	        /*POR 0x05; n=0..7; set COM level, deselect voltage = 0.72+0.02*n*Vcc */
  U8X8_CA(0x23, 0x00),	        /*POR 0x00; Disable fade mode */
//  U8X8_CA(0x23, 0x1x),	        /* enable fade in mode */
//  U8X8_CA(0x23, 0x2x),	        /* enable fade out mode */
//  U8X8_CA(0x23, 0x3x),	        /* enable continous fade out/in mode */
//  U8X8_CA(0x23, 0x0n),	        /* n=0..15 fade interval=8*(n+1) frames  */

  U8X8_DLY(1),					        /* delay  */
  U8X8_END_TRANSFER(),          /* disable chip */
  U8X8_END()             			  /* end of sequence */
};
/*=========================================================*/
uint8_t u8x8_d_ssd1362_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  switch(msg){
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
      u8x8_cad_SendArg(u8x8, arg_int );	/* device's contrast range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
#ifdef U8X8_WITH_SET_GREY
    case U8X8_MSG_DISPLAY_SET_GREY:
      u8x8_d_ssd1362_grey_level = arg_int>>4; /* device's pixel grey level range from 0 to 15 */
      u8x8_d_ssd1362_grey_level |= (u8x8_d_ssd1362_grey_level<<4); /* 2 pixel mask */
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
/*=========================================================*/
uint8_t u8x8_d_ssd1362_256x64_raystar(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  if (u8x8_d_ssd1362_common(u8x8, msg, arg_int, arg_ptr) != 0)
    return 1;

  switch(msg){
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ssd1362_256x64_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362_256x64_raystar_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 ){
        u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362_256x64_raystar_flip0_seq);
        u8x8->x_offset = u8x8->display_info->default_x_offset;
      }else{
        u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1362_256x64_raystar_flip1_seq);
        u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
    default:
      return 0;
  }
  return 1;
}
