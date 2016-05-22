/*

  u8x8_sl.h
  
  selection list with scroll option
  
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

/*
  increase the cursor position
*/
void u8sl_Next(u8sl_t *u8sl)
{
  u8sl->current_pos++;
  if ( u8sl->current_pos >= u8sl->total )
  {
    u8sl->current_pos = 0;
    u8sl->first_pos = 0;
  }
  else
  {
    if ( u8sl->first_pos + u8sl->visible <= u8sl->current_pos + 1 )
    {
      u8sl->first_pos = u8sl->current_pos - u8sl->visible + 1;
    }
  }
}

void u8sl_Prev(u8sl_t *u8sl)
{
  if ( u8sl->current_pos == 0 )
  {
    u8sl->current_pos = u8sl->total - 1;
    u8sl->first_pos = 0;
    if ( u8sl->total > u8sl->visible )
      u8sl->first_pos = u8sl->total - u8sl->visible;
  }
  else
  {
    u8sl->current_pos--;
    if ( u8sl->first_pos > u8sl->current_pos )
      u8sl->first_pos = u8sl->current_pos;
  }
}

void u8x8_DrawSelectionList(u8x8_t *u8x8, u8sl_t *u8sl, u8x8_sl_cb sl_cb, const void *aux)
{
  uint8_t i;
  for( i = 0; i < u8sl->visible; i++ )
  {
    sl_cb(u8x8, u8sl, i+u8sl->first_pos, aux);
  }
}

/* selection list with string line */
void u8x8_sl_string_line_cb(u8x8_t *u8x8, u8sl_t *u8sl, uint8_t idx, const void *aux)
{
  const char *s;
  uint8_t col;
  uint8_t row;
  /* calculate offset from display upper border */
  row = u8x8_GetRows(u8x8);
  row -= u8sl->visible;
  row /= 2;
  
  /* calculate target pos */
  row += idx;
  row -= u8sl->first_pos;
  
  /* check whether this is the current cursor line */
  if ( idx == u8sl->current_pos )
    u8x8_SetInverseFont(u8x8, 1);
  else
    u8x8_SetInverseFont(u8x8, 0);
  
  /* get the line from the array */
  s = u8x8_GetStringLine(idx, (const char *)aux);
  
  /* draw the line */
  if ( s == NULL )
    s = "";
  col = u8x8_DrawUTF8(u8x8, 0, row, s);
  
  /* fill up the line with some more spaced */
  while( col < u8x8_GetCols(u8x8) )
    u8x8_DrawUTF8(u8x8, col++, row, " ");
}

/*

  returns start_pos if user has pressed the home key
  returns the selected line if user has pressed the select key
*/
uint8_t u8x8_UserInterfaceSelectionList(u8x8_t *u8x8, uint8_t start_pos, const char *sl)
{
  u8sl_t u8sl;
  uint8_t event;
  u8sl.visible = u8x8_GetRows(u8x8);
  u8sl.total = u8x8_GetStringLineCnt(sl);
  u8sl.first_pos = 0;
  u8sl.current_pos = start_pos;
  if ( u8sl.current_pos >= u8sl.total )
    u8sl.current_pos = u8sl.total-1;

  u8x8_DrawSelectionList(u8x8, &u8sl, u8x8_sl_string_line_cb, sl);      
  
  for(;;)
  {
    event = u8x8_GetMenuEvent(u8x8);
    if ( event == U8X8_MSG_GPIO_MENU_SELECT )
      return u8sl.current_pos;
    else if ( event == U8X8_MSG_GPIO_MENU_HOME )
      return start_pos;
    else if ( event == U8X8_MSG_GPIO_MENU_NEXT )
    {
      u8sl_Next(&u8sl);
      u8x8_DrawSelectionList(u8x8, &u8sl, u8x8_sl_string_line_cb, sl);      
    }
    else if ( event == U8X8_MSG_GPIO_MENU_PREV )
    {
      u8sl_Prev(&u8sl);
      u8x8_DrawSelectionList(u8x8, &u8sl, u8x8_sl_string_line_cb, sl);      
    }    
  }
}

