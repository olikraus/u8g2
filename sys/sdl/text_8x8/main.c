#include "u8x8.h"

u8x8_t u8x8;


int main(void)
{
  u8x8_Setup_SDL_128x64(&u8x8);
  u8x8_InitDisplay(&u8x8);  

  u8x8_SetFont(&u8x8, u8x8_font_px437wyse700a_2x2_r);
  u8x8_SetFont(&u8x8, u8x8_font_courB24_3x4_f);
  
  
  //u8x8_DrawString(&u8x8, 0, 0, "abc");
  //u8x8_Draw1x2String(&u8x8, 0, 4, "abc");
  u8x8_Draw2x2String(&u8x8, 0, 0, "Abcd");


  while( u8g_sdl_get_key() < 0 )
    ;
  
  
  return 0;
}

