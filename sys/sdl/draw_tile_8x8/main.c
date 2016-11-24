#include "u8x8.h"

u8x8_t u8x8;
uint8_t tile_list[] = 
{
  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0,

  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0,
  0x0f0
  
};



int main(void)
{
  u8x8_Setup_SDL_128x64(&u8x8);
  u8x8_InitDisplay(&u8x8);  

  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_f);
  
  u8x8_DrawString(&u8x8, 0, 0, "draw tile");

  u8x8_DrawTile(&u8x8, 0, 1, 2, tile_list);


  while( u8g_sdl_get_key() < 0 )
    ;
  
  return 0;
}

