
#include "u8g2.h"
#include <stdio.h>


  
u8g2_t u8g2;

uint8_t b[2] = { 0x55, 0x55 };

int main(void)
{

  u8g2_SetupBuffer_Utf8(&u8g2, U8G2_R0);
  
  
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_6x13_tf);
  u8g2_SetFontDirection(&u8g2, 0);
  
  u8g2_FirstPage(&u8g2);
  do
  {      
    u8g2_DrawHorizontalBitmap(&u8g2, 0, 0, 10, b);
    u8g2_DrawStr(&u8g2, 1, 20, "HorizontalBitmap");
  } while( u8g2_NextPage(&u8g2) );
    
  utf8_show();
  
  return 0;
}

