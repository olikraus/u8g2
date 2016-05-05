/*
	Use UTF-8 encoding for this file
*/


#include "u8x8.h"

u8x8_t u8g2;


int main(void)
{
  u8x8_Setup_SDL_128x64(&u8g2);
  u8x8_InitDisplay(&u8g2);  

  u8x8_SetFont(&u8g2, u8x8_font_amstrad_cpc_extended_f);
  u8x8_DrawString(&u8g2, 0, 0, "UTF-8 Test");

  u8x8_DrawString(&u8g2, 0, 1, "M\xfcnchen");/* works: select umlaut-u directly with code 0xfc */
  u8x8_DrawString(&u8g2, 0, 2, "München");	/* does not work: umlaut-u is translated to utf-8, broken output */
  u8x8_DrawUTF8(&u8g2, 0, 3, "M\xfcnchen");	/* does not work: Codes above 127 are reserved for UTF-8 encoding */
  u8x8_DrawUTF8(&u8g2, 0, 4, "München");	/* works: u8x8 can handle UTF-8 code correctly */
  
  while( u8g_sdl_get_key() < 0 )
    ;
  
  return 0;
}

