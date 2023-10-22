/*

  u8g2_arc.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2023, olikraus@gmail.com
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


static void u8g2_draw_arc(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t start, uint8_t end)
{
  // Manage angle inputs
  uint8_t full = (start == end);
  uint8_t inverted = (start > end);
  uint8_t a_start = inverted ? end : start;
  uint8_t a_end = inverted ? start : end;

  // Initialize variables
  uint32_t ratio;
  u8g2_int_t x = 0;
  u8g2_int_t y = rad;
  u8g2_int_t d = rad - 1;

  // Trace arc radius with the Andres circle algorithm (process each pixel of a 1/8th circle of radius rad)
  while (y >= x)
  {
    // Get the percentage of 1/8th circle drawn with a fast approximation of arctan(x/y)
    ratio = x * 255 / y; // x/y [0..255]
    ratio = ratio * (770195 - (ratio - 255) * (ratio + 941)) / 6137491; // arctan(x/y) [0..32]

    // Fill the pixels of the 8 sections of the circle, but only on the arc defined by the angles (start and end)
    if(full || ((ratio >= a_start && ratio < a_end) ^ inverted)) u8g2_DrawPixel(u8g2, x0 + y, y0 - x);
    if(full || (((ratio + a_end) > 63 && (ratio + a_start) <= 63) ^ inverted)) u8g2_DrawPixel(u8g2, x0 + x, y0 - y);
    if(full || (((ratio + 64) >= a_start && (ratio + 64) < a_end) ^ inverted)) u8g2_DrawPixel(u8g2, x0 - x, y0 - y);
    if(full || (((ratio + a_end) > 127 && (ratio + a_start) <= 127) ^ inverted)) u8g2_DrawPixel(u8g2, x0 - y, y0 - x);
    if(full || (((ratio + 128) >= a_start && (ratio + 128) < a_end) ^ inverted)) u8g2_DrawPixel(u8g2, x0 - y, y0 + x);
    if(full || (((ratio + a_end) > 191 && (ratio + a_start) <= 191) ^ inverted)) u8g2_DrawPixel(u8g2, x0 - x, y0 + y);
    if(full || (((ratio + 192) >= a_start && (ratio + 192) < a_end) ^ inverted)) u8g2_DrawPixel(u8g2, x0 + x, y0 + y);
    if(full || (((ratio + a_end) > 255 && (ratio + a_start) <= 255) ^ inverted)) u8g2_DrawPixel(u8g2, x0 + y, y0 + x);

    // Run Andres circle algorithm to get to the next pixel
    if (d >= 2 * x)
    {
      d = d - 2 * x - 1;
      x = x + 1;
    }
    else if (d < 2 * (rad - y))
    {
      d = d + 2 * y - 1;
      y = y - 1;
    }
    else
    {
      d = d + 2 * (y - x - 1);
      y = y - 1;
      x = x + 1;
    }
  }
}

void u8g2_DrawArc(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t start, uint8_t end)
{
  /* check for bounding box */
#ifdef U8G2_WITH_INTERSECTION
  {
    if ( u8g2_IsIntersection(u8g2, x0-rad, y0-rad, x0+rad+1, y0+rad+1) == 0 ) 
      return;
  }
#endif /* U8G2_WITH_INTERSECTION */
  
  
  /* draw arc */
  u8g2_draw_arc(u8g2, x0, y0, rad, start, end);
}


