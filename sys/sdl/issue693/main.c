
#include "u8g2.h"
#include <stdio.h>
#include <string.h>

char str1[100];
char str2[100];
  
u8g2_t u8g2;

int main(void)
{
  int x, y;
  int k;
  int len1, len2;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_helvB18_tn);

  strcpy(str1,"Супердискотека 90х");
  strcpy(str2,"Virgin Radio New");

  u8g2_SetFont(&u8g2, u8g2_font_6x13_t_cyrillic);

  len1 = u8g2_GetUTF8Width(&u8g2,str1);
  len2 = u8g2_GetUTF8Width(&u8g2,str2);

  printf("len1: %d, str1: %s\n",len1,str1);
  printf("len2: %d, str2: %s\n",len2,str2);
  
  x = 10;
  y = 30;
  
  for(;;)
  {
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawUTF8(&u8g2, x, y, str1);
      u8g2_DrawUTF8(&u8g2, x, y+15, str2);
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

