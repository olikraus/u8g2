
#include "u8g2.h"
#include <stdio.h>

  
u8g2_t u8g2;

int main(void)
{
  int x, y;
  int k;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);
  
  x = 50;
  y = 30;

  
  for(;;)
  {
    printf("x=%d y=%d\n", x, y);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawBox(&u8g2, 0,0,128,32);
      
      u8g2_SetFontMode(&u8g2, 0);
      u8g2_DrawGlyph(&u8g2, x, y, 'A');
      u8g2_DrawStrX2(&u8g2, x+20, y, "Abc");
      
      u8g2_SetFontMode(&u8g2, 1);
      u8g2_DrawGlyph(&u8g2, x, y+24, 'A');
      u8g2_DrawStrX2(&u8g2, x+16, y+24, "Abc");
    } while( u8g2_NextPage(&u8g2) );
    
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 273 ) y -= 7;
    if ( k == 274 ) y += 7;
    if ( k == 276 ) x -= 7;
    if ( k == 275 ) x += 7;
    
    if ( k == 'e' ) y -= 1;
    if ( k == 'x' ) y += 1;
    if ( k == 's' ) x -= 1;
    if ( k == 'd' ) x += 1;
    if ( k == 'q' ) break;
    
  }
  
  
  return 0;
}

