/*

  u8x8_draw_hvtile.c
  
  draw hvtile

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
    
*/

#include "u8x8.h"

/*=================================================*/
/*
  u8x8_draw_hvtile_vertical_top_lsb
   
  x,y       tile position
  tile_cnt           number of tiles to draw
  copies             number of copies of tile pattern to draw
  tile_buffer_width  tile width of buffer being used for drawing
  tile_ptr           tile buffer
*/
/* SSD13XX, UC1701 */
uint8_t u8x8_draw_hvtile_vertical_top_lsb(u8x8_t *u8x8, uint8_t tx, uint8_t ty, uint8_t tile_cnt, uint8_t copies, uint8_t tile_buffer_width, uint8_t *tile_ptr)
{
   return u8x8_DrawTile(u8x8, tx, ty, tile_cnt, copies, tile_buffer_width, tile_ptr); /* tiles vertical by default */
}

/*=================================================*/
/*
  u8x8_draw_hvtile_horizontal_right_lsb

  x,y                tile position
  tile_cnt           number of tiles to draw
  copies             number of copies of tile pattern to draw
  tile_buffer_width  tile width of buffer being used for drawing
  tile_ptr           tile buffer
*/
/* SH1122, LD7032, ST7920, ST7986, LC7981, T6963, SED1330, RA8835, MAX7219, LS0 */
uint8_t u8x8_draw_hvtile_horizontal_right_lsb(u8x8_t *u8x8, uint8_t tx, uint8_t ty, uint8_t tile_cnt, uint8_t copies, uint8_t tile_buffer_width, uint8_t *tile_ptr)
{
  uint8_t hvbuffer[8 * tile_cnt]; /* buffer for converted tile */
  uint8_t *ptr;
  uint8_t  t,i;

  for( t = 0; t < tile_cnt; t++ ){
    ptr = hvbuffer + t; /* horizontal tile, move +1 byte */
    for( i = 0; i < 8; i++ ){
      *ptr  =((*(tile_ptr)  &(1<<i))>>i)<<7;
      *ptr +=((*(tile_ptr+1)&(1<<i))>>i)<<6;
      *ptr +=((*(tile_ptr+2)&(1<<i))>>i)<<5;
      *ptr +=((*(tile_ptr+3)&(1<<i))>>i)<<4;
      *ptr +=((*(tile_ptr+4)&(1<<i))>>i)<<3;
      *ptr +=((*(tile_ptr+5)&(1<<i))>>i)<<2;
      *ptr +=((*(tile_ptr+6)&(1<<i))>>i)<<1;
      *ptr +=((*(tile_ptr+7)&(1<<i))>>i);
      ptr += tile_cnt; /* next horizontal tile row */
    }
    tile_ptr += 8; /* vertical tile, move +8 bytes */
  }

  return u8x8_DrawTile(u8x8, tx, ty, tile_cnt, copies, tile_cnt/*tile_buffer_width*/, hvbuffer);
}

