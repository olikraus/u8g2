
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
    u8g2_DrawBox(&u8g2, 2, 3, 7, 9);
    u8g2_DrawBox(&u8g2, 12, 3, 2, 13);
    u8g2_DrawBox(&u8g2, 12,17, 13, 2);
    u8g2_DrawFrame(&u8g2, 2,21, 11, 7);
    u8g2_DrawFrame(&u8g2, 25,24, 27, 5);
    u8g2_DrawStr(&u8g2, 30, 10, "DrawBox");
    u8g2_DrawStr(&u8g2, 30, 20, "DrawFrame");
  } while( u8g2_NextPage(&u8g2) );
    
  utf8_show();
  
  return 0;
}

