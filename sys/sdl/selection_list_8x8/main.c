/*
  Example for the 
      u8x8_UserInterfaceSelectionList
  procedure.
  
*/


#include "u8x8.h"

u8x8_t u8x8;


int main(void)
{
  u8x8_Setup_SDL_128x64(&u8x8);
  u8x8_InitDisplay(&u8x8);  

  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_f);
  
  u8x8_UserInterfaceSelectionList(&u8x8, "Title", 1, "abc\ndef\nghi\njkl\nmno");

  return 0;
}

