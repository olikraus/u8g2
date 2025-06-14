
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


static const unsigned char teststr[] U8X8_PROGMEM = {
  0x09, 0x28, 0x00, 0x00, // u8g2_DrawGlyph(&u8g2, 0, 0, 2344);
  0x09, 0x2e, 0x10, 0x00, // u8g2_DrawGlyph(&u8g2, 16, 0, 2350);
  0x09, 0x38, 0x10, 0x00, // u8g2_DrawGlyph(&u8g2, 32, 0, 2360);
  0x09, 0x4d, 0x00, 0x00, // u8g2_DrawGlyph(&u8g2, 32, 0, 2381);
  0x09, 0x24, 0x10, 0x00, // u8g2_DrawGlyph(&u8g2, 48, 0, 2340);
  0x09, 0x47, 0x00, 0x00, // u8g2_DrawGlyph(&u8g2, 48, 0, 2375);
  0x00, 0x00  // end of binary
};

static const unsigned char testarabic[] U8X8_PROGMEM = {
  0x06, 0x27, 0x00, 0x00, // u8g2_DrawGlyph(&u8g2, 0, 0, 1575);
  0x06, 0x28, 0x08, 0x00, // u8g2_DrawGlyph(&u8g2, 8, 0, 1576);
  0x06, 0x4b, 0x00, 0x00, // u8g2_DrawGlyph(&u8g2, 8, 0, 1611);
  0x06, 0x2d, 0x08, 0x00, // u8g2_DrawGlyph(&u8g2, 16, 0, 1581);
  0x06, 0x31, 0x08, 0x00, // u8g2_DrawGlyph(&u8g2, 24, 0, 1585);
  0x06, 0x45, 0x08, 0x00, // u8g2_DrawGlyph(&u8g2, 32, 0, 1605);
  0x00, 0x00  // end of binary
};



int main(void)
{
  int x, y;
  int k;
  int offset = 100;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */
  
  x = 4;  // use as height for the box
  y = 20;

  for(;;)
  {
    u8g2_FirstPage(&u8g2);
    do
    {

      
      u8g2_SetFont(&u8g2, u8g2_font_unifont_t_devanagari);
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_SetFontMode(&u8g2, 1);
      u8g2_DrawHB(&u8g2, x, y, teststr);
      u8g2_SetFont(&u8g2, u8g2_font_unifont_t_arabic);
      u8g2_DrawHB(&u8g2, x, y+16, testarabic);
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

