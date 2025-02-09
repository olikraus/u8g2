
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

#define Row1Y 50
#define MinXT 80
#define MinXO 110

#define HourXT 13
#define HourXO 45  
  
#define clockMinT "0"
#define clockMinO "0"
#define clockHourT "0"
#define clockHourO "0"
      
      u8g2_SetFont(&u8g2, u8g2_font_fur42_tr);
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_DrawPixel(&u8g2, 0, 0);
      u8g2_DrawStr(&u8g2, -4, 45, "00");
      u8g2_DrawLine(&u8g2, 0,22,127,22);

      /*
      tw = (int)u8g2_GetUTF8Width(&u8g2, clockMinT);
      u8g2_DrawStr(&u8g2, MinXT-tw/2, Row1Y, clockMinT);
      tw = (int)u8g2_GetUTF8Width(&u8g2, clockMinO);
      u8g2_DrawStr(&u8g2, MinXO-tw/2, Row1Y, clockMinO);
      //hours
      tw = (int)u8g2_GetUTF8Width(&u8g2, clockHourT);
      u8g2_DrawStr(&u8g2, HourXT-tw/2, Row1Y, clockHourT);
      tw = (int)u8g2_GetUTF8Width(&u8g2, clockHourO);
      u8g2_DrawStr(&u8g2, HourXO-tw/2, Row1Y, clockHourO);      
      */        
      
      //u8g2_SetDrawColor(&u8g2, 1);
      
      //u8g2_SetFont(&u8g2, u8g2_font_helvB08_tf);  
      //u8g2_SetDrawColor(&u8g2, 2);
      //u8g2_DrawVLine(&u8g2, x, 0, 64);
      //u8g2_DrawLine(&u8g2, 30, 10, 30, 100);
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

