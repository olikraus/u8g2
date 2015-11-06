
#include "u8x8.h"

#include <stdio.h>

#define W 8
#define H 2

uint8_t bitmap[W*H*8];

void bitmap_place_tile(uint8_t x, uint8_t y, uint8_t *tile)
{
  uint8_t i;
  for(i = 0; i < 8; i++ )
    bitmap[x*8+y*W*8+i] = tile[i];
}

void bitmap_show(void)
{
  int x, y;
  for( y = 0; y < H*8; y++ )
  {
    for( x = 0; x < W*8; x++ )
    {
      if ( (bitmap[x+(y/8)*W*8] & (1<<((y&7)))) != 0 )
      {
	printf("*");
      }
      else
      {
	printf(".");
      }
    }
    printf("\n");
  }
}


uint8_t u8x8_d_stdio(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	bitmap_show();
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:      
      bitmap_place_tile(((u8x8_tile_t *)arg_ptr)->x_pos, ((u8x8_tile_t *)arg_ptr)->y_pos, ((u8x8_tile_t *)arg_ptr)->tile_ptr);
      break;
    default:
      break;
  }
  return 1;
}



void u8x8_SetupStdio(u8x8_t *u8x8)
{
  u8x8_SetupDefaults(u8x8);
  u8x8->display_cb = u8x8_d_stdio;
}

