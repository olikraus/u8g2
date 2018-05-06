#include "u8x8.h"
#include <stdio.h>
#include <time.h>

u8x8_t u8x8;
u8log_t u8log;

#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8

uint8_t u8log_buf[U8LOG_WIDTH*U8LOG_HEIGHT];


int main(void)
{
  int k;
  clock_t t;
  uint8_t c;
  
  u8x8_Setup_SDL_128x64(&u8x8);
  u8x8_InitDisplay(&u8x8);  

  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_f);

  u8log_Init(&u8log, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buf);
  u8log_SetCallback(&u8log, u8log_u8x8_cb, &u8x8);
  //u8log_SetRedrawMode(&u8log, /* is_redraw_line_for_each_char = */ 1);
  u8log_SetRedrawMode(&u8log, /* is_redraw_line_for_each_char = */ 0);
  
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
  }
  
  return 0;
}

