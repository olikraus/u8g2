
#include "u8g2.h"
#include <stdio.h>

  
u8g2_t u8g2;

int main(void)
{
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  

  u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_SetFontRefHeightAll(&u8g2);

  u8g2_UserInterfaceSelectionList(&u8g2, "Title", 0, "abc\ndef\nghi\njkl\n12345\n67890");


  
  return 0;
}

