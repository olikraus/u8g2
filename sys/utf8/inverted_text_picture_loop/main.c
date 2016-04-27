
#include "u8g2.h"
#include <stdio.h>


  
u8g2_t u8g2;

int main(void)
{

  u8g2_SetupBuffer_Utf8(&u8g2, U8G2_R0);
  
  
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_6x13_mf);
  u8g2_SetFontDirection(&u8g2, 0);
  
  u8g2_FirstPage(&u8g2);
  do
  {      
    u8g2.draw_color = 1;
    u8g2_DrawBox(&u8g2, 2, 8, 70, 20);
    u8g2.draw_color = 0;
    u8g2_DrawStr(&u8g2, 10, 14, "Hello World!");
    u8g2.draw_color = 0;
    u8g2_DrawBox(&u8g2, 9, 15, 31, 2);
  } while( u8g2_NextPage(&u8g2) );
    
  utf8_show();
  
  return 0;
}

