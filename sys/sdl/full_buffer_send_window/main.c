
#include "u8g2.h"
#include <stdio.h>


u8g2_t u8g2;

/*
  Limitations:
    - Tile positions and sizes (pixel position divided by 8)
    - Any display rotation/mirror is ignored
    - Only works with displays, which support U8x8 API
    - Only available in full buffer mode
    - Will not send the e-paper refresh message 
*/
u8g2_SendTileSubBuffer(u8g2_t *u8g2, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
  uint16_t page_size;
  uint8_t *ptr;

  page_size = u8g2->pixel_buf_width;  /* 8*u8g2->u8g2_GetU8x8(u8g2)->display_info->tile_width */
    
  ptr = u8g2_GetBufferPtr(u8g2);
  ptr += x*8;
  ptr += page_size*y;
  
  while( h > 0 )
  {
    u8x8_DrawTile( u8g2_GetU8x8(u8g2), x, y, w, ptr );
    ptr += page_size;
    y++;
    h--;
  }  
}

/*
  sub buffer window in pixel coordinates
  lower left corner is NOT included, this means the transfer range is from
  x0..x1-1
  y0..y1-1
*/
void u8g2_SendWindow(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t x1, u8g2_uint_t y1)
{
  x0 /= 8;
  y0 /= 8;
  x1 += 7;
  x1 /= 8;
  y1 +=7;
  y1 /= 8;
  u8g2_SendTileSubBuffer(u8g2, x0, y0, x1-x0, y1-y0);
}



int main(void)
{
  int x, y;
  int k;
  
  //u8g2_SetupBuffer_SDL_240x160(&u8g2, &u8g2_cb_r0);
  u8g2_SetupBuffer_SDL_128x64(&u8g2, &u8g2_cb_r0);
  //u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */
  
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  u8g2_SetFont(&u8g2, u8g2_font_helvB18_tr);
  
  x = 30;
  y = 35;

  
  for(;;)
  {
#ifdef U8G2_WITH_HVLINE_COUNT
    u8g2.hv_cnt = 0UL;
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
    u8g2_ClearBuffer(&u8g2);
    
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_DrawStr(&u8g2, x, y, "A");
      u8g2_SetFontDirection(&u8g2, 1);
      u8g2_DrawStr(&u8g2, x, y, "a");
      u8g2_SetFontDirection(&u8g2, 2);
      u8g2_DrawStr(&u8g2, x, y, "a");
      u8g2_SetFontDirection(&u8g2, 3);
      u8g2_DrawStr(&u8g2, x, y, "a");

	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y0, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y0, 1, 0);
    
    
    //u8g2_SendBuffer(&u8g2);
    //u8g2_SendTileSubBuffer(&u8g2, 2, 2, 12, 3);
    u8g2_SendWindow(&u8g2, 9, 17, 120, 42);
    //u8g2_SendTileWindow(&u8g2, 0, 0, 16, 8);

/*    
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
*/
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


    //if ( k == 't' ) 
    //  u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2), "screenshot.tga");
    
  }
  
  //u8x8_Set8x8Font(u8g2_GetU8x8(&u8g2), bdf_font);
  //u8x8_Draw8x8String(u8g2_GetU8x8(&u8g2), 0, 0, "Hello World!");
  
  
  
  
  
  
  return 0;
}

