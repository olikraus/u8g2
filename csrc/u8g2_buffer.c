/* 

  u8g2_buffer.c 
  
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
