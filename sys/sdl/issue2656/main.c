
#include "u8g2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  

u8g2_t u8g2;

int screenshot_n = 0;

void do_screenshot(void)
{
  char s[4096];
  u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2),  "screenshot.tga");
  sprintf(  s, 
  "convert -border 4 -bordercolor 'rgb(255,190,40)'"
  " -fill 'rgb(255,170,0)' -opaque white"
  "  -filter point -resize 200%%" 
  " screenshot.tga pic%04d.png", screenshot_n);
  system(s);
  screenshot_n++;
  /*
  gif animation:
  convert -delay 40 -loop 0 pic*.png animation.gif
  */
}

int main(void)
{
  int x, y;
  int k;
  int offset = 100;
  int tw;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */
  
  x = 4;  // use as height for the box
  y = 0;

  for(;;)
  {
    u8g2_FirstPage(&u8g2);
    do
    {

      
      u8g2_SetFont(&u8g2, u8g2_font_unifont_t_devanagari);
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_SetFontMode(&u8g2, 1);
u8g2_DrawGlyph(&u8g2, 0, 20, 2345); // ax=64 ay=0 dx=0 dy=0 xb=0 yb=44
u8g2_DrawGlyph(&u8g2, 0, 50, 2345); // reference 
u8g2_DrawPixel(&u8g2, 0, 50);
u8g2_DrawGlyph(&u8g2, 16, 20, 2351); // ax=64 ay=0 dx=0 dy=0 xb=0 yb=44
u8g2_DrawGlyph(&u8g2, 20, 50, 2351); // reference 
u8g2_DrawPixel(&u8g2, 20, 50);
u8g2_DrawGlyph(&u8g2, 32, 20, 2361); // ax=64 ay=0 dx=0 dy=0 xb=0 yb=44
u8g2_DrawGlyph(&u8g2, 40, 50, 2361); // reference 
u8g2_DrawPixel(&u8g2, 40, 50);
u8g2_DrawGlyph(&u8g2, 48, 20, 2382); // ax=0 ay=0 dx=0 dy=0 xb=-28 yb=4
u8g2_DrawGlyph(&u8g2, 60, 50, 2382); // reference 
u8g2_DrawPixel(&u8g2, 60, 50);
u8g2_DrawGlyph(&u8g2, 48, 20, 2341); // ax=64 ay=0 dx=0 dy=0 xb=0 yb=44
u8g2_DrawGlyph(&u8g2, 80, 50, 2341); // reference 
u8g2_DrawPixel(&u8g2, 80, 50);
u8g2_DrawGlyph(&u8g2, 64, 20, 2376); // ax=0 ay=0 dx=0 dy=0 xb=-52 yb=56
u8g2_DrawGlyph(&u8g2, 100, 50, 2376); // reference 
u8g2_DrawPixel(&u8g2, 100, 50);

    } while( u8g2_NextPage(&u8g2) );
    
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 273 ) y -= 1;
    if ( k == 274 ) y += 1;
    if ( k == 276 ) x -= 1;
    if ( k == 275 ) x += 1;
    
    if ( k == 'e' ) y -= 1;
    if ( k == 'x' ) y += 1;
    if ( k == 's' ) x -= 1;
    if ( k == 'd' ) x += 1;
    if ( k == 'q' ) break;
    
    if ( k == 32 )
      offset -= 1;
    if ( k == 't' ) 
    {
      puts("screenshot");
      do_screenshot();
    }
    
    if ( x < 0 )
      x = 0;
    
  }
  return 0;
}

