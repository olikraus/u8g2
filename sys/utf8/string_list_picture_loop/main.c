
#include "u8g2.h"
#include <stdio.h>


  
u8g2_t u8g2;

const char *str = "abc\ndef";

int main(void)
{
  uint8_t i, cnt;

  u8g2_SetupBuffer_Utf8(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  u8g2_SetFont(&u8g2, u8g2_font_helvB10_tr);
  
  
#ifdef U8G2_WITH_HVLINE_COUNT
    u8g2.hv_cnt = 0UL;
#endif /* U8G2_WITH_HVLINE_COUNT */   
        
    u8g2_FirstPage(&u8g2);
    i = 0;
    cnt = u8x8_GetStringLineCnt(str);

    do
    {
      for( i = 0; i < cnt; i++ )
      {
	u8g2_DrawStr(&u8g2, 0, i*12+11, u8x8_GetStringLine(i, str));
      }      

    } while( u8g2_NextPage(&u8g2) );
#ifdef U8G2_WITH_HVLINE_COUNT
    printf("hv cnt: %ld\n", u8g2.hv_cnt);
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
  utf8_show();
  
  return 0;
}

