
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
    u8g2_DrawCircle(&u8g2, 7, 11, 5, U8G2_DRAW_ALL);
    u8g2_DrawEllipse(&u8g2, 19, 11, 5, 5, U8G2_DRAW_ALL);
    u8g2_DrawDisc(&u8g2, 7, 11+14, 5, U8G2_DRAW_ALL);
    u8g2_DrawFilledEllipse(&u8g2, 19, 11+14, 5, 5, U8G2_DRAW_ALL);
    u8g2_DrawEllipse(&u8g2, 60, 11+12, 17, 5, U8G2_DRAW_ALL);
    u8g2_DrawStr(&u8g2, 30, 10, "Circle&Disk");
  } while( u8g2_NextPage(&u8g2) );
    
  utf8_show();
  
  return 0;
}

