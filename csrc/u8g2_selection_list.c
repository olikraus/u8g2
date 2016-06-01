/*

  u8g2_selection_list.c
  
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

#include "u8g2.h"


/*
  Draw a string
  Extend the string to size "w"
  Center the string, if the first char is a '\t' (center with respect to "w")
  return the size of the string

*/
void u8g2_DrawUTF8Line(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, const char *s)
{
  u8g2_uint_t d, lw;
    
  d = 0;
  if ( *s == '\t' )
  {
    s++;		/* skip '\t' */
    lw = u8g2_GetUTF8Len(u8g2, s);
    if ( lw < w )
    {
      d = w;
      d -=lw;
      d /= 2;
    }
  }
  x += d
  u8g2_DrawUTF8(u8g2, x, y, s);
}


/* selection list with string line */
#ifdef NOT_FINISHED
void u8g2_draw_string_line(u8g2_t *u8g2, u8sl_t *u8sl, uint8_t idx, const char *s)
{
  uint8_t row;
  /* calculate offset from display upper border */
  row = u8sl->y;
  
  /* calculate target pos */
  row += idx;
  row -= u8sl->first_pos;
  
  /* check whether this is the current cursor line */
  if ( idx == u8sl->current_pos )
    u8x8_SetInverseFont(u8x8, 1);
  else
    u8x8_SetInverseFont(u8x8, 0);
  
  /* get the line from the array */
  s = u8x8_GetStringLineStart(idx, s);
  
  /* draw the line */
  if ( s == NULL )
    s = "";
  u8x8_DrawUTF8Line(u8x8, u8sl->x, row, u8x8_GetCols(u8x8), s);  
  u8x8_SetInverseFont(u8x8, 0);
}
#endif
