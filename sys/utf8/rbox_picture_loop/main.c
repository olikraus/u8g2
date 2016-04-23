
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
    u8g2_DrawRBox(&u8g2, 2, 3, 8, 9, 3);
    u8g2_DrawRBox(&u8g2, 12, 3, 5, 13, 2);
    u8g2_DrawRBox(&u8g2, 12,17, 13, 3, 1);
    
    u8g2_DrawRFrame(&u8g2, 2,21, 11, 7, 3);
    u8g2_DrawRFrame(&u8g2, 25,24, 27, 5, 2);
    u8g2_DrawRFrame(&u8g2, 23,22, 31, 9, 3);
    u8g2_DrawStr(&u8g2, 30, 10, "RBox");
    u8g2_DrawStr(&u8g2, 30, 20, "RFrame");
  } while( u8g2_NextPage(&u8g2) );
    
  utf8_show();
  
  return 0;
}

