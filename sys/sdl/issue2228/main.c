
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
  int offset = 100;

  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
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
      u8g2_DrawUTF8(&u8g2, offset,30,"123456789QWERTYUIOPASDFGHJKLZXCVBNM123456789QWERTYUIOPASDFGHJKLZXCVBNM");
      u8g2_DrawUTF8X2(&u8g2, offset,60,"123456789QWERTYUIOPASDFGHJKLZXCVBNM123456789QWERTYUIOPASDFGHJKLZXCVBNM");
      //u8g2_DrawHLine(&u8g2, 0, 64, 64);
      u8g2_DrawLine(&u8g2, 30, 10, 30, 100);
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

