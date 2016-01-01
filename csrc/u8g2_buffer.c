/* 

  u8g2_buffer.c 

  Universal 8bit Graphics Library (http://code.google.com/p/u8g2/)

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

#include "u8g2.h"
#include <string.h>

/*============================================*/
void u8g2_ClearBuffer(u8g2_t *u8g2)
{
  size_t cnt;
  cnt = u8g2_GetU8x8(u8g2)->display_info->tile_width;
  cnt *= u8g2->tile_buf_height;
  cnt *= 8;
  memset(u8g2->tile_buf_ptr, 0, cnt);
}

/*============================================*/

static void u8g2_send_tile_row(u8g2_t *u8g2, uint8_t src_tile_row, uint8_t dest_tile_row)
{
  uint8_t *ptr;
  uint16_t offset;
  uint8_t w;
  
  w = u8g2_GetU8x8(u8g2)->display_info->tile_width;
  offset = src_tile_row;
  ptr = u8g2->tile_buf_ptr;
  offset *= w;
  offset *= 8;
  ptr += offset;
    
  u8x8_DrawTile(u8g2_GetU8x8(u8g2), 0, dest_tile_row, w, ptr);
}

void u8g2_SendBuffer(u8g2_t *u8g2)
{
  uint8_t src_row;
  uint8_t src_max;
  uint8_t dest_row;
  uint8_t dest_max;

  src_row = 0;
  src_max = u8g2->tile_buf_height;
  dest_row = u8g2->tile_curr_row;
  dest_max = u8g2_GetU8x8(u8g2)->display_info->tile_height;
  
  do
  {
    u8g2_send_tile_row(u8g2, src_row, dest_row);
    src_row++;
    dest_row++;
  } while( src_row < src_max && dest_row < dest_max );
}

/*============================================*/
void u8g2_FirstPage(u8g2_t *u8g2)
{
  u8g2_ClearBuffer(u8g2);
  u8g2->tile_curr_row = 0;
  u8g2->cb->update(u8g2);
}

uint8_t u8g2_NextPage(u8g2_t *u8g2)
{
  uint8_t row;
  u8g2_SendBuffer(u8g2);
  row = u8g2->tile_curr_row;
  row += u8g2->tile_buf_height;
  if ( row >= u8g2_GetU8x8(u8g2)->display_info->tile_height )
    return 0;
  u8g2_ClearBuffer(u8g2);
  u8g2->tile_curr_row = row;
  u8g2->cb->update(u8g2); 
  return 1;
}
