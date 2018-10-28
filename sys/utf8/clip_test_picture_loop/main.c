/*
  this example was written to debug a problem with a display width, which
  is not multiple of 8.
  the display width is set to 102 for this purpose
  
  this is also used to do more clip testing.
  any assertion indicates a bug.
*/



#include "u8g2.h"
#include <stdio.h>


  
u8g2_t u8g2;

void draw(void)
{
    u8g2_SetFont(&u8g2, u8g2_font_ncenB24_tf);
    u8g2_DrawHLine(&u8g2, u8g2_GetDisplayWidth(&u8g2)-1, 2, 4);
    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_DrawStr(&u8g2, -4, 20, "Clip/////");
    u8g2_DrawStr(&u8g2, -5, 50, "Clip/////");
    u8g2_DrawStr(&u8g2, -6, 80, "Clip/////");
    u8g2_DrawStr(&u8g2, -6, 110, "Clip/////");
    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB12_tf);
  
    u8g2_DrawStr(&u8g2, 40, 40, "F");
    u8g2_SetFontDirection(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 40, 40, "F");
    u8g2_SetFontDirection(&u8g2, 2);
    u8g2_DrawStr(&u8g2, 40, 40, "F");
    u8g2_SetFontDirection(&u8g2, 3);
    u8g2_DrawStr(&u8g2, 40, 40, "F");
}

int main(void)
{

  u8g2_SetupBuffer_Utf8(&u8g2, U8G2_R0);
  
  
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);  
  
  u8g2_SetFontDirection(&u8g2, 0);
  
  //u8g2_SetClipWindow(&u8g2, 10, 10, 50, 60 );
  
  
  u8g2_SetDisplayRotation(&u8g2, U8G2_R0);  
  u8g2_FirstPage(&u8g2);
  do
  {      
    draw();
  } while( u8g2_NextPage(&u8g2) );

  puts("=======================================");
  utf8_show();

  u8g2_SetDisplayRotation(&u8g2, U8G2_R1);  
  u8g2_FirstPage(&u8g2);
  do
  {      
    draw();
  } while( u8g2_NextPage(&u8g2) );

  puts("=======================================");
  utf8_show();

  u8g2_SetDisplayRotation(&u8g2, U8G2_R2);  
  u8g2_FirstPage(&u8g2);
  do
  {      
    draw();
  } while( u8g2_NextPage(&u8g2) );
  
  puts("=======================================");
  utf8_show();

  u8g2_SetDisplayRotation(&u8g2, U8G2_R3);  
  u8g2_FirstPage(&u8g2);
  do
  {      
    draw();
  } while( u8g2_NextPage(&u8g2) );
  
  puts("=======================================");
  utf8_show();

  u8g2_SetDisplayRotation(&u8g2, U8G2_MIRROR);  
  u8g2_FirstPage(&u8g2);
  do
  {      
    draw();
  } while( u8g2_NextPage(&u8g2) );
  
  puts("=======================================");
  utf8_show();
  
  printf("DisplayWidth = %d\n", u8g2_GetDisplayWidth(&u8g2));
  
  return 0;
}

