
#include "u8x8.h"

u8x8_t u8x8;


int main(void)
{
  u8x8_Setup_TGA_DESC(&u8x8);
  u8x8_InitDisplay(&u8x8);  
  u8x8_SetPowerSave(&u8x8, 0);

  u8x8_SetFont(&u8x8, u8x8_font_amstrad_cpc_extended_r );
  u8x8_DrawString(&u8x8, 0, 0, "Hello World!");

  tga_save("u8x8.tga");
  return 0;
}

