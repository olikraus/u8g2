
#include "u8g2.h"
#include <stdio.h>


  
u8g2_t u8g2;

int main(void)
{
  int y;

  u8g2_SetupBuffer_Utf8(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
  u8g2_SetFontDirection(&u8g2, 0);
  
  
#ifdef U8G2_WITH_HVLINE_COUNT
    u8g2.hv_cnt = 0UL;
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
    
    u8g2_FirstPage(&u8g2);
    do
    {
      for( y = 3; y < 21; y++ )
      {
	  u8g2_DrawHLine(&u8g2, 3, y, 11);
      }
      
      u8g2_DrawStr(&u8g2, 17, 8, "Test Inter-");
      u8g2_DrawStr(&u8g2, 17, 16, "section:");
      u8g2_DrawStr(&u8g2, 17, 24, "One Block");
      

    } while( u8g2_NextPage(&u8g2) );
#ifdef U8G2_WITH_HVLINE_COUNT
    printf("hv cnt: %ld\n", u8g2.hv_cnt);
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
  utf8_show();
  
  return 0;
}

