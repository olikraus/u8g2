
#include "u8g2.h"
#include <stdio.h>


  
u8g2_t u8g2;

int main(void)
{

  u8g2_SetupBuffer_Utf8(&u8g2, U8G2_R0);
  
  
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);  
  
  //u8g2_SetFont(&u8g2, u8g2_font_6x13_mf);
  u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
  u8g2_SetFontDirection(&u8g2, 0);

  u8g2_UserInterfaceSelectionList(&u8g2, "Title", 0, "abc\ndef\nghi\njkl");

  utf8_show();

  return 0;
}

