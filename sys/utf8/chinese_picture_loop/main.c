
#include "u8g2.h"
#include <stdio.h>


  
u8g2_t u8g2;

int main(void)
{

  u8g2_SetupBuffer_Utf8(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_chinese2);
  u8g2_SetFontDirection(&u8g2, 0);
  
  u8g2_FirstPage(&u8g2);
  do
  {      
    u8g2_DrawUTF8(&u8g2, 2, 15, "你好世界");
    u8g2_DrawStr(&u8g2, 2, 30, "Hello World");
  } while( u8g2_NextPage(&u8g2) );
    
  utf8_show();
  
  return 0;
}

