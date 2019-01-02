#include "u8x8.h"

u8x8_t u8x8;



char *s = "ABC defg";


int main(void)
{
  u8x8_Setup_SDL_128x64(&u8x8);
  u8x8_InitDisplay(&u8x8);  

  puts("u8x8_font_px437wyse700b_2x2_r, u8x8_font_px437wyse700a_2x2_r");
  
  u8x8_SetFont(&u8x8, u8x8_font_px437wyse700b_2x2_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 2, s);

  u8x8_SetFont(&u8x8, u8x8_font_px437wyse700a_2x2_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 4, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 6, s);


  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_lucasarts_scumm_subtitle_o_2x2_r, u8x8_font_lucasarts_scumm_subtitle_r_2x2_r");

  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_lucasarts_scumm_subtitle_o_2x2_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 2, s);
  
  u8x8_SetFont(&u8x8, u8x8_font_lucasarts_scumm_subtitle_r_2x2_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 4, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 6, s);

  while( u8g_sdl_get_key() < 0 )
    ;
  
  puts("u8x8_font_courB18_2x3_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_courB18_2x3_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 3, s);

  while( u8g_sdl_get_key() < 0 )
    ;
  
  puts("u8x8_font_courR18_2x3_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_courR18_2x3_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 3, s);

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_courB24_3x4_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_courB24_3x4_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, "AB fg");
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 4, "AB fg");

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_courR24_3x4_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_courR24_3x4_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, "AB fg");
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 4, "AB fg");

  while( u8g_sdl_get_key() < 0 )
    ;
  
  puts("u8x8_font_profont29_2x3_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_profont29_2x3_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 3, s);

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_7x14_1x2_r, u8x8_font_7x14B_1x2_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_7x14_1x2_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 2, s);
  
  u8x8_SetFont(&u8x8, u8x8_font_7x14B_1x2_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 4, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 6, s);

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_8x13_1x2_r, u8x8_font_8x13B_1x2_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_8x13_1x2_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 2, s);
  
  u8x8_SetFont(&u8x8, u8x8_font_8x13B_1x2_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 4, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 6, s);

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_shylock_nbp_1x2_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_shylock_nbp_1x2_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 2, s);  

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_inr21_2x4_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_inr21_2x4_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 4, s);  

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_inr33_3x6_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_inr33_3x6_r);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 0, "058fg");

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_inr46_4x8_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_inr46_4x8_r);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 0, "08fg");

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_inb21_2x4_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_inb21_2x4_r);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, s);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 4, s);  

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_inb33_3x6_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_inb33_3x6_r);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 0, "058fg");

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_inb46_4x8_r");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_inb46_4x8_r);
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 0, "08fg");

  while( u8g_sdl_get_key() < 0 )
    ;


  puts("u8x8_font_open_iconic_weather_2x2");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_open_iconic_weather_2x2);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, "@ABC");
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 2, "@ABC");

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_open_iconic_weather_4x4");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_open_iconic_weather_4x4);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, "@ABC");
  u8x8_SetInverseFont(&u8x8, 1);
  u8x8_DrawString(&u8x8, 0, 4, "@ABC");

  while( u8g_sdl_get_key() < 0 )
    ;

  puts("u8x8_font_open_iconic_weather_8x8");
  u8x8_ClearDisplay(&u8x8);
  u8x8_SetFont(&u8x8, u8x8_font_open_iconic_weather_8x8);
  u8x8_SetInverseFont(&u8x8, 0);
  u8x8_DrawString(&u8x8, 0, 0, "@A");

  while( u8g_sdl_get_key() < 0 )
    ;
  
  return 0;
}

