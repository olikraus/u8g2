
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
#define LUT_DY 3

uint8_t lut_time_to_width[16] = 
{
  2,	// 0
  2,	// 1
  2,	// 2
  2,	// 3
  3,	// 4
  3,	// 5
  3,	// 6
  4,	// 7
  4,	// 8
  4,	// 9
  5,	// 10
  5,	// 11
  5,	// 12
  6,	// 13
  6,	// 14
  6,	// 15
};

u8g2_uint_t get_dx(uint8_t i)
{
    
  return lut_time_to_width[lut_time[i]];
}

void draw_lut_wave(u8g2_t *u8g2, uint8_t w, uint8_t cx, u8g2_uint_t y)
{
  uint8_t i;
  u8g2_uint_t x1, y1;
  u8g2_uint_t x2, y2;
  x1 = 1;
  y1 = y;
  for( i = 0; i < LUT_ARRAY_LEN; i++ )
  {
    x2 = x1 + get_dx(i);
    y2 = y;
    if ( lut_level[w][i] == 1)
      y2 -= LUT_DY-1;
    else if ( lut_level[w][i] == 2 )
      y2 += LUT_DY-1;
    if ( y1 != y2 )
      u8g2_DrawLine(u8g2, x1, y1, x1, y2);
    u8g2_DrawHLine(u8g2, x1, y2, x2-x1+1);
    
    if ( i == cx )
    {
      u8g2_DrawHLine(u8g2, x1, y-LUT_DY-1, x2-x1+1);
      u8g2_DrawHLine(u8g2, x1, y+LUT_DY+1, x2-x1+1);      
    }
    
    x1 = x2;
    y1 = y2;
  }
}

void draw_all_lut(u8g2_t *u8g2, uint8_t cx, uint8_t cy)
{
  uint8_t i;
  u8g2_uint_t x, y;
  u8g2_uint_t xs = 1;
  y = LUT_DY +1;
  for( i = 0; i < LUT_WAVE_CNT; i++ )
  {
    if ( i == cy )
      draw_lut_wave(u8g2, i, cx, y);
    else
      draw_lut_wave(u8g2, i, 255, y);		// 255: no cursor
      
    y += (LUT_DY+2)*2;
  }

  y+=1;
  u8g2_SetFont(u8g2, u8g2_font_5x7_mr);
  u8g2_SetFontMode(u8g2, 0);
  if ( cy == LUT_WAVE_CNT )
  {
    u8g2_SetDrawColor(u8g2, 0);
  }
  else
  {
    u8g2_SetDrawColor(u8g2, 1);
  }
  x = u8g2_DrawStr(u8g2, xs, y, " TP[");
  x += u8g2_DrawStr(u8g2, x, y, u8x8_u8toa( cx, 2));
  x += u8g2_DrawStr(u8g2, x, y, "]=");
  x += u8g2_DrawStr(u8g2, x, y, u8x8_u8toa( lut_time[cx], 2));
  x += u8g2_DrawStr(u8g2, x, y, " ");
  
  y+=8;
  if ( cy == LUT_WAVE_CNT+1 )
  {
    u8g2_SetDrawColor(u8g2, 0);
  }
  else
  {
    u8g2_SetDrawColor(u8g2, 1);
  }
  x = u8g2_DrawStr(u8g2, xs, y, " Test ");
  y+=8;
  if ( cy == LUT_WAVE_CNT+2 )
  {
    u8g2_SetDrawColor(u8g2, 0);
  }
  else
  {
    u8g2_SetDrawColor(u8g2, 1);
  }
  x = u8g2_DrawStr(u8g2, xs, y, " Print ");
  u8g2_SetDrawColor(u8g2, 1);

  
}

void init_lut(void)
{
  uint8_t i, j;
  for( i = 0; i < LUT_ARRAY_LEN; i++ )
  {
    for( j = 0; j < LUT_WAVE_CNT; j++ )
      lut_level[j][i] = 0;
    lut_time[i] = 15;
  }
  
   lut_level[0][2] = 1;
   lut_level[0][3] = 2;
   lut_level[0][4] = 2;
   lut_level[0][7] = 1;
   lut_level[0][8] = 2;
   lut_level[0][LUT_ARRAY_LEN-1] = 1;
   lut_level[1][4] = 1;
   lut_level[1][6] = 2;
   lut_level[2][7] = 1;
   lut_level[2][8] = 3;
}

u8g2_t u8g2;


int main(void)
{
  uint8_t event;
  uint8_t cx, cy;
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  

  u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
  u8g2_SetFontDirection(&u8g2, 0);
  //u8g2_SetFontRefHeightAll(&u8g2);
  
  init_lut();
  cx = 0;
  cy = 0;

  for(;;)
  {
    u8g2_FirstPage(&u8g2);
    do
    {
      draw_all_lut(&u8g2, cx, cy);
    } while( u8g2_NextPage(&u8g2) );

    for(;;)
    {
      event = u8x8_GetMenuEvent(u8g2_GetU8x8(&u8g2));
      if ( event == U8X8_MSG_GPIO_MENU_SELECT )
      {
	if ( cx < LUT_ARRAY_LEN && cy < LUT_WAVE_CNT )
	{
	  lut_level[cy][cx]++;
	  if ( lut_level[cy][cx] >= 3 )
	    lut_level[cy][cx]  = 0;
	}
	else
	{
	  if ( cy == LUT_WAVE_CNT )
	  {
	    lut_time[cx]++;
	    if ( lut_time[cx] >= 16 )
	      lut_time[cx]  = 0;
	  }
	}
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_HOME )
	return 0;	
      else if ( event == U8X8_MSG_GPIO_MENU_NEXT )
      {
	cx++;
	if ( cx >= LUT_ARRAY_LEN )
	  cx = 0;
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_DOWN )
      {
	cy++;
	if ( cy >= LUT_WAVE_CNT+3 )
	  cy = 0;
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_PREV )
      {
	if ( cx == 0 )
	  cx = LUT_ARRAY_LEN;
	cx--;
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_UP )
      {
	if ( cy == 0 )
	  cy = LUT_WAVE_CNT+3;
	cy--;
	break;
      }
    }
  }
  
  return 0;
}

