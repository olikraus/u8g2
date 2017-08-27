
#include "u8g2.h"
#include <stdio.h>

#define LUT_ARRAY_LEN 20
#define LUT_WAVE_CNT 4

/*
  level
    0:		neutral, middle
    1:		upper line
    2:		lower line
    
*/

uint8_t lut_level[LUT_WAVE_CNT][LUT_ARRAY_LEN];
uint8_t lut_time[LUT_ARRAY_LEN];
#define LUT_DY 4

u8g2_uint_t get_dx(uint8_t i)
{
  return 3;
}

void draw_lut_wave(u8g2_t *u8g2, uint8_t w, u8g2_uint_t y)
{
  uint8_t i;
  u8g2_uint_t x1, y1;
  u8g2_uint_t x2, y2;
  x1 = 0;
  y1 = y;
  for( i = 0; i < LUT_ARRAY_LEN; i++ )
  {
    x2 = x1 + get_dx(i);
    y2 = y;
    if ( lut_level[w][i] == 1)
      y2 -= LUT_DY;
    else if ( lut_level[w][i] == 2 )
      y2 += LUT_DY;
    if ( y1 != y2 )
      u8g2_DrawLine(u8g2, x1, y1, x1, y2);
    u8g2_DrawHLine(u8g2, x1, y2, x2-x1+1);
    x1 = x2;
    y1 = y2;
  }
}

void draw_all(u8g2_t *u8g2)
{
  draw_lut_wave(u8g2, 0, LUT_DY +2);
  draw_lut_wave(u8g2, 1, (LUT_DY +2)*2 + LUT_DY);
}

void init_lut(void)
{
  uint8_t i, j;
  for( i = 0; i < LUT_ARRAY_LEN; i++ )
  {
    for( j = 0; j < LUT_WAVE_CNT; j++ )
      lut_level[j][i] = 0;
    lut_time[i] = 2;
  }
  
   lut_level[0][2] = 1;
   lut_level[1][4] = 1;
}

u8g2_t u8g2;


int main(void)
{
  uint8_t event;
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  

  u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_SetFontRefHeightAll(&u8g2);
  
  init_lut();

  u8g2_FirstPage(&u8g2);
  do
  {
    draw_all(&u8g2);
  } while( u8g2_NextPage(&u8g2) );
  
 for(;;)
  {
    event = u8x8_GetMenuEvent(u8g2_GetU8x8(&u8g2));
    if ( event == U8X8_MSG_GPIO_MENU_SELECT )
      return 0;	
    else if ( event == U8X8_MSG_GPIO_MENU_HOME )
      return 0;	
    else if ( event == U8X8_MSG_GPIO_MENU_NEXT || event == U8X8_MSG_GPIO_MENU_DOWN )
    {
      break;
    }
    else if ( event == U8X8_MSG_GPIO_MENU_PREV || event == U8X8_MSG_GPIO_MENU_UP )
    {
      break;
    }
  }

  
  return 0;
}

