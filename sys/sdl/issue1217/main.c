
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
  char s[10] = "123";
  
  s[0] += 115;
  s[1] += 115;

  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  
  x = 4;  // use as height for the box
  y = 0;
  
  for(;;)
  {
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawBox(&u8g2, 10, 10+y, 20, x);
      u8g2_DrawFrame(&u8g2, 34, 10+y, 20, x);
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
    
    if ( x < 0 )
      x = 0;
    
  }
  return 0;
}

