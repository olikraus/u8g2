
#include "u8g2.h"
#include <stdio.h>

  
u8g2_t u8g2;

int main(void)
{
  uint8_t value = 0;
  uint8_t r;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  

  //u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);
  u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_SetFontRefHeightAll(&u8g2);


  r = u8g2_UserInterfaceInputValue(&u8g2, "Title\n-----\n", "X=", &value, 0, 19, 2, "m");
  
  printf("r= %d\n", r);
  printf("value= %d\n", value);

/*
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
    
  */
  
  return 0;
}

