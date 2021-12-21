
#include "u8g2.h"
#include <stdio.h>

  
u8g2_t u8g2;

int main(void)
{
  int x, y;
  int k;
  int i;
  
    u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */
    u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);
  
  x = 50;
  y = 30;

  
  for(;;)
  {
#ifdef U8G2_WITH_HVLINE_COUNT
    u8g2.hv_cnt = 0UL;
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
    /*
    u8g2_ClearBuffer(&u8g2);
    
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_DrawStr(&u8g2, x, y, "ABC");
      u8g2_SetFontDirection(&u8g2, 1);
      u8g2_DrawStr(&u8g2, x, y, "abc");
      u8g2_SetFontDirection(&u8g2, 2);
      u8g2_DrawStr(&u8g2, x, y, "abc");
      u8g2_SetFontDirection(&u8g2, 3);
      u8g2_DrawStr(&u8g2, x, y, "abc");
    
    u8g2_SendBuffer(&u8g2);
    */
    
    u8g2_FirstPage(&u8g2);
    i = 0;
    do
    {
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_DrawStr(&u8g2, x, y, "ABC");
      u8g2_SetFontDirection(&u8g2, 1);
      u8g2_DrawStr(&u8g2, x, y, "abc");
      u8g2_SetFontDirection(&u8g2, 2);
      u8g2_DrawStr(&u8g2, x, y, "abc");
      u8g2_SetFontDirection(&u8g2, 3);
      u8g2_DrawStr(&u8g2, x, y, "abc");
      if ( i == 1 )
      {
	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y0, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y0, 1, 0);

      }
      
      i++;
      

    } while( u8g2_NextPage(&u8g2) );
#ifdef U8G2_WITH_HVLINE_COUNT
    printf("hv cnt: %ld\n", u8g2.hv_cnt);
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
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
    
    if ( k == 't' ) 
    {
      puts("screenshot");
      u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2),  "screenshot.tga");
    }
  }
  
  //u8x8_Set8x8Font(u8g2_GetU8x8(&u8g2), bdf_font);
  //u8x8_Draw8x8String(u8g2_GetU8x8(&u8g2), 0, 0, "Hello World!");
  
  
  
  
  
  
  return 0;
}

