/*
  Example for the 
      u8x8_UserInterfaceInputValue
  procedure.
  
*/


#include "u8x8.h"

u8x8_t u8x8;


int main(void)
{
  uint8_t value = 0;
  u8x8_Setup_SDL_128x64(&u8x8);
  u8x8_InitDisplay(&u8x8);  

  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_f);
  
  u8x8_UserInterfaceInputValue(&u8x8, "Title\n-----\n", "X=", &value, 0, 19, 2, "m");

  return 0;
}

