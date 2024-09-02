
#include "u8g2.h"
#include <stdio.h>
#include <string.h>


char str1[100];
char str2[100];
  
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

  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */

  
  x = 4;  // use as height for the box
  y = 0;
  
  for(;;)
  {
    //printf("offset=%d\n", offset);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_SetDrawColor(&u8g2, 1);
      
      u8g2_SetFont(&u8g2, u8g2_font_helvB08_tf);  
      u8g2_DrawUTF8(&u8g2, x,10+y,"ABC helvB08");
      u8g2_DrawButtonUTF8(&u8g2, x+76, 10+y, U8G2_BTN_BW1, 0, /* padding_h*/ 0, /* padding_v */0, "AB");
      u8g2_DrawButtonUTF8(&u8g2, x+95, 10+y, U8G2_BTN_BW1, 0, /* padding_h*/ 1, /* padding_v */0, "AB");
      
      u8g2_SetFont(&u8g2, u8g2_font_ncenB08_te);        
      u8g2_DrawUTF8(&u8g2, x,25+y,"ABC ncenB08");
      u8g2_DrawButtonUTF8(&u8g2, x+76, 25+y, U8G2_BTN_BW1, 0, /* padding_h*/ 0, /* padding_v */0, "AB");
      u8g2_DrawButtonUTF8(&u8g2, x+95, 25+y, U8G2_BTN_BW1, 0, /* padding_h*/ 1, /* padding_v */0, "AB");
      
      printf("ncenB08 A xoffset=%d\n", (int8_t)u8g2_GetXOffsetGlyph(&u8g2, 'A'));

      u8g2_SetFont(&u8g2, u8g2_font_helvR08_te);  
      u8g2_DrawUTF8(&u8g2, x,40+y,"ĈD helvR08");
      u8g2_DrawButtonUTF8(&u8g2, x+76, 40+y, U8G2_BTN_BW1, 0, /* padding_h*/ 0, /* padding_v */0, "ĈD");
      u8g2_DrawButtonUTF8(&u8g2, x+95, 40+y, U8G2_BTN_BW1, 0, /* padding_h*/ 1, /* padding_v */0, "ĈD");

      printf("helvR08 C xoffset=%d\n", (int8_t)u8g2_GetXOffsetGlyph(&u8g2, 'C'));
      printf("helvR08 Ĉ xoffset=%d\n", (int8_t)u8g2_GetXOffsetUTF8(&u8g2, "Ĉ"));
      printf("helvR08 Ĉ xoffset=%d\n", (int8_t)u8g2_GetStrX(&u8g2, "Ĉ"));
      
      u8g2_SetFont(&u8g2, u8g2_font_ncenR08_tf);        
      u8g2_DrawUTF8(&u8g2, x,55+y,"CD ncenR08");
      u8g2_DrawButtonUTF8(&u8g2, x+76, 55+y, U8G2_BTN_BW1, 0, /* padding_h*/ 0, /* padding_v */0, "CD");
      u8g2_DrawButtonUTF8(&u8g2, x+95, 55+y, U8G2_BTN_BW1, 0, /* padding_h*/ 1, /* padding_v */0, "CD");



      /*
      u8g2_SetFont(&u8g2, u8g2_font_helvR08_tf);  
      u8g2_DrawUTF8(&u8g2, x,30+y,"ABC helvR08");
      u8g2_SetFont(&u8g2, u8g2_font_ncenR08_tf);  
      u8g2_DrawUTF8(&u8g2, x,40+y,"ABC ncenR08");

      u8g2_SetFont(&u8g2, u8g2_font_5x8_tf);  
      u8g2_DrawUTF8(&u8g2, x,50+y,"ABC 5x8");
      u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);  
      u8g2_DrawUTF8(&u8g2, x,60+y,"ABC 6x10");


      u8g2_SetFont(&u8g2, u8g2_font_mozart_nbp_tf);  
      u8g2_DrawUTF8(&u8g2, x,70+y,"ABC mozart_nbp");
      u8g2_SetFont(&u8g2, u8g2_font_profont12_tf);  
      u8g2_DrawUTF8(&u8g2, x,80+y,"ABC profont12");

      u8g2_SetFont(&u8g2, u8g2_font_simple1_tf);  
      u8g2_DrawUTF8(&u8g2, x,90+y,"ABC simple1");
      u8g2_SetFont(&u8g2, u8g2_font_princess_te);  
      u8g2_DrawUTF8(&u8g2, x,100+y,"ABC princess");
      */
      
      u8g2_SetDrawColor(&u8g2, 2);
      u8g2_DrawVLine(&u8g2, x, 0, 64);
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

