#include "u8x8.h"

u8x8_t u8x8;


int main(void)
{
  u8x8_Setup_SDL_128x64(&u8x8);
  u8x8_InitDisplay(&u8x8);  

  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_f);
  
  u8x8_DrawString(&u8x8, 0, 0, "Hello World!");

  u8x8_Draw1x2String(&u8x8, 0, 1, "Hello World!");
  
  u8x8_Draw2x2String(&u8x8, 0, 3, "Hello World!");


  while( u8g_sdl_get_key() < 0 )
    ;
  
  return 0;
}

