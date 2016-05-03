/*
  this example was written to debug a problem with a display width, which
  is not multiple of 8.
  the display width is set to 102 for this purpose
*/



#include "u8g2.h"
#include <stdio.h>


  
u8g2_t u8g2;

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
    
    u8g2_DrawHLine(&u8g2, u8g2_GetDisplayWidth(&u8g2)-1, 2, 4);
    u8g2_DrawStr(&u8g2, 10, 20, "Clip");
  } while( u8g2_NextPage(&u8g2) );
    
  utf8_show();
  
  printf("DisplayWidth = %d\n", u8g2_GetDisplayWidth(&u8g2));
  
  return 0;
}

