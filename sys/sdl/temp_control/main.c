
#include "u8g2.h"
#include <stdio.h>
#include <string.h>


char str1[100];
char str2[100];
  
u8g2_t u8g2;


void drawBigNum(u8g2_t *u, uint16_t x, uint16_t y, const char *heading, uint16_t n, uint16_t fraction, uint16_t is_temp)
{
  uint16_t e;
  uint16_t xx;
  
  u8g2_SetFont(u, u8g2_font_spleen8x16_mr);
  u8g2_DrawUTF8(u, x+4, y-46, heading);
  
  u8g2_SetFont(u, u8g2_font_spleen32x64_mn);
  e = n / 10;
  if ( e > 0 )
  {
    u8g2_DrawGlyph(u, x, y, e+'0');
  }
  x += 30;
  e = n - e*10;
  u8g2_DrawGlyph(u, x, y, e+'0');
  u8g2_SetFont(u, u8g2_font_spleen12x24_mf);
  
  x += 30;
  xx = x;
  
  x += u8g2_DrawGlyph(u, x, y, '.');
  x += u8g2_DrawGlyph(u, x, y, fraction+'0');  

  if ( is_temp )
  {
    x += u8g2_DrawUTF8(u, xx, y-25, "°C");  
  }
  else
  {
    u8g2_SetFont(u, u8g2_font_spleen16x32_mr);
    x += u8g2_DrawUTF8(u, xx+8, y-20, "%");  
  }
}

void draw(u8g2_t *u)
{
  u8g2_SetFontMode(u, 1);

  u8g2_DrawLine(u, 0, 63, 255, 63);  
  u8g2_DrawLine(u, 127, 0, 127, 127);  
  u8g2_DrawLine(u, 127+64, 64, 127+64, 127);  

  drawBigNum(u, 20, 59, "Temp", 83, 4, 1);
  drawBigNum(u, 10+128+10, 59, "Humidity", 99, 9, 0);
  drawBigNum(u, 20, 59+64, "Set", 85, 0, 1);

  u8g2_SetFont(u, u8g2_font_streamline_coding_apps_websites_t);  
  u8g2_DrawGlyph(u, 150, 105, 58);

  u8g2_SetFont(u, u8g2_font_streamline_interface_essential_login_t);  
  u8g2_DrawGlyph(u, 214, 105, 48);
}

int main(void)
{
  int x, y;
  int k;
  int offset = 100;

  
  u8g2_SetupBuffer_SDL_256x128(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  

  u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_gb2312);  

  x = 4;  // use as height for the box
  y = 0;
  
  for(;;)
  {
    printf("offset=%d\n", offset);
    u8g2_FirstPage(&u8g2);
    do
    {
      //u8g2_DrawUTF8(&u8g2, offset,30,"123456789QWERTYUIOPASDFGHJKLZXCVBNM123456789QWERTYUIOPASDFGHJKLZXCVBNM");
      //u8g2_DrawUTF8X2(&u8g2, offset,60,"123456789QWERTYUIOPASDFGHJKLZXCVBNM123456789QWERTYUIOPASDFGHJKLZXCVBNM");
      //u8g2_DrawHLine(&u8g2, 0, 64, 64);
      draw(&u8g2);
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
    
    if ( x < 0 )
      x = 0;
    
  }
  return 0;
}

