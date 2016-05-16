/*

  u8x8_sl.h
  
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
  if ( u8sl->current_pos > u8sl->total )
  {
    u8sl->current_pos = 0;
    u8sl->first_pos = 0;
  }
  else
  {
    if ( u8sl->first_pos + u8sl->visible < u8sl->current_pos  )
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
    if ( u8sl->first_pos < u8sl->current_pos )
      u8sl->first_pos = u8sl->current_pos;
  }
}

