/*

  u8x8_message.h
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/

#include "u8x8.h"

uint8_t u8x8_draw_button_line(u8x8_t *u8x8, uint8_t y, uint8_t w, uint8_t cursor, const char *s)
{
  uint8_t i;
  uint8_t cnt;
  uint8_t total;
  uint8_t d;
  uint8_t x;
  cnt = u8x8_GetStringLineCnt(s);
  
  /* calculate the width of the button */
  total = 0;
  for( i = 0; i < cnt; i++ )
  {
    total += u8x8_GetUTF8Len(u8x8, u8x8_GetStringLine(i, s));
  }
  total += (cnt-1);	/* had one space between the buttons */
  
  /* calculate the left offset */
  d = 0;
  if ( total < w )
  {
    d = w;
    d -= total;
    d /= 2;
  }
  
  /* draw the buttons */
  x = d;
  for( i = 0; i < cnt; i++ )
  {
    if ( i == cursor )
      u8x8_SetInverseFont(u8x8, 1);
    else
      u8x8_SetInverseFont(u8x8, 0);
      
    x+=u8x8_DrawUTF8(x, y, u8x8_GetStringLine(i, s));
    x+=u8x8_DrawUTF8(x, y, " ");
  }
  
  /* return the number of buttons */
  return cnt;
}

uint8_t u8x8_UserInterfaceMessage(u8x8_t *u8x8, const char *title1, const char *title2, const char *buttons,  )
{
  uint8_t height;
  uint8_t y;
  uint8_t cursor = 0;
  uint8_t button_cnt;
  
  /* calculate overall height of the message box */
  height = 1;	/* button line */
  height += u8x8_GetStringLineCnt(title1);
  height += u8x8_GetStringLineCnt(title2);
  
  /* calculate offset from the top */
  y = 0;
  if ( u8x8_GetRows(u8x8) < height )
  {
    y = height;
    y -= u8x8_GetRows(u8x8);
    y /= 2;
  }
  
  y += u8x8_DrawUTF8Lines(u8x8, 0, y, u8x8_GetCols(u8x8), title1);
  y += u8x8_DrawUTF8Lines(u8x8, 0, y, u8x8_GetCols(u8x8), title2);
  
  button_cnt = u8x8_draw_button_line(u8x8, y, u8x8_GetCols(u8x8), cursor, buttons);
  
  for(;;)
  {
    event = u8x8_GetMenuEvent(u8x8);
    if ( event == U8X8_MSG_GPIO_MENU_SELECT )
      return cursor+1;
    else if ( event == U8X8_MSG_GPIO_MENU_HOME )
      break;
    else if ( event == U8X8_MSG_GPIO_MENU_NEXT )
    {
      cursor++;
      if ( cursor >= button_cnt )
	cursor = 0;
      u8x8_draw_button_line(u8x8, y, u8x8_GetCols(u8x8), cursor, buttons);
    }
    else if ( event == U8X8_MSG_GPIO_MENU_PREV )
    {
      if ( cursor == 0 )
	cursor = button_cnt;
      cursor--;
      u8x8_draw_button_line(u8x8, y, u8x8_GetCols(u8x8), cursor, buttons);
    }    
  }  
  return 0;
}

