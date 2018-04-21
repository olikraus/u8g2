
#include "u8g2.h"
#include <stdio.h>
#include <time.h>

  
u8g2_t u8g2;
u8log_t u8log;

#define U8LOG_WIDTH 10
#define U8LOG_HEIGHT 4

uint8_t u8log_buf[U8LOG_WIDTH*U8LOG_HEIGHT];

int main(void)
{
  int k;
  clock_t t;
  uint8_t c;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);    
  u8g2_SetFont(&u8g2, u8g2_font_helvB12_tr);
  u8log_Init(&u8log, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buf);
  u8log_SetCallback(&u8log, u8log_u8g2_cb, &u8g2);
  u8log_SetRedrawMode(&u8log, /* is_redraw_line_for_each_char = */ 1);
  //u8log_SetRedrawMode(&u8log, /* is_redraw_line_for_each_char = */ 0);
  u8log_SetLineHeightOffset(&u8log, -3);			/* decrese line spacing, u8g2 only */
  u8g2_SetFontRefHeightExtendedText(&u8g2);	/* increase distance from top and line height */
  u8g2_SetFontRefHeightAll(&u8g2);	/* increase distance from top and line height */
  
  t = clock()+CLOCKS_PER_SEC/10;
  for(;;)
  {
    
    if ( t < clock() )
    {
      c = (t % 10) + '0';
      if ( c == '0' )
	c = '\n';
      u8log_WriteChar(&u8log, c);
      t = clock()+CLOCKS_PER_SEC/10;      
    }
    
    k = u8g_sdl_get_key();
    
    if ( k == 'q' ) break;
    
    if ( k == 't' ) 
      u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2),  "screenshot.tga");
  }
  
  return 0;
}

