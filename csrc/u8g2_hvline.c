/*

  u8g2_hvline.c

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


  Calltree
    void u8g2_DrawHVLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
    u8g2->cb->draw_l90
    void u8g2_draw_hv_line_4dir(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
    void u8g2_draw_hv_line_2dir(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
    void u8g2_draw_low_level_hv_line(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
    void u8g2_draw_pixel(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y)

*/

#include "u8g2.h"
#include <assert.h>




#ifdef U8G2_WITH_CLIPPING

/*
  Description:
    clip range from a (included) to b (excluded) agains c (included) to d (excluded)
  Assumptions:
    a <= b		(this is checked and handled correctly)
    c <= d		(this is not checked)
  will return 0 if there is no intersection and if a > b

  optimized clipping: c is set to 0
*/
//static uint8_t u8g2_clip_intersection(u8g2_uint_t *ap, u8g2_uint_t *bp, u8g2_uint_t c, u8g2_uint_t d)
static uint8_t u8g2_clip_intersection(u8g2_uint_t *ap, u8g2_uint_t *bp, u8g2_uint_t d)
{
  u8g2_uint_t a = *ap;
  u8g2_uint_t b = *bp;
  
  /* handle the a>b case correctly. If code and time is critical, this could */
  /* be removed completly (be aware about memory curruption for wrong */
  /* arguments) or return 0 for a>b (will lead to skipped lines for wrong */
  /* arguments) */  
  
  /* removing the following if clause completly may lead to memory corruption of a>b */
  if ( a > b )
  {    
    /* replacing this if with a simple "return 0;" will not handle the case with negative a */    
    if ( a < d )
    {
      b = d;
      b--;
      *bp = b;
    }
    else
    {
      a = 0;
      *ap = a;
    }
  }
  
  /* from now on, the asumption a <= b is ok */
  
  if ( a >= d )
    return 0;
  if ( b <= 0 )		// was b <= c, could be replaced with b == 0
    return 0;
  //if ( a < c )		// never true with c == 0
  //  *ap = c;
  if ( b > d )
    *bp = d;
    
  return 1;
}



/*
  x,y		Upper left position of the line
  len		length of the line in pixel, len must not be 0
  dir		0: horizontal line (left to right)
		1: vertical line (top to bottom)
  This function first adjusts the y position to the local buffer. Then it
  will clip the line and call u8g2_draw_low_level_hv_line()
  
*/
static void u8g2_draw_hv_line_2dir(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
{
  u8g2_uint_t a;
  register u8g2_uint_t w, h;

  y -= u8g2->tile_curr_row*8;
  
  h = u8g2->pixel_buf_height;
  w = u8g2->pixel_buf_width;
  
  if ( dir == 0 )
  {
    if ( y >= h )
      return;
    a = x;
    a += len;
    if ( u8g2_clip_intersection(&x, &a, w) == 0 )
      return;
    len = a;
    len -= x;
  }
  else
  {
    if ( x >= w )
      return;
    a = y;
    a += len;
    if ( u8g2_clip_intersection(&y, &a, h) == 0 )
      return;
    len = a;
    len -= y;
  }
  
  u8g2->ll_hvline(u8g2, x, y, len, dir);
  //u8g2_draw_low_level_hv_line(u8g2, x, y, len, dir);
}

#endif

/*
  x,y		Upper left position of the line
  len		length of the line in pixel, len must not be 0
  dir		0: horizontal line (left to right)
		1: vertical line (top to bottom)
		2: horizontal line (right to left)
		3: vertical line (bottom to top)

  This function will remove directions 2 and 3. Instead 0 and 1 are used.

*/
void u8g2_draw_hv_line_4dir(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
{
#ifdef U8G2_WITH_HVLINE_COUNT
  u8g2->hv_cnt++;
#endif /* U8G2_WITH_HVLINE_COUNT */   
  
  if ( dir == 2 )
  {
    x -= len;
    x++;
  }
  else if ( dir == 3 )
  {
    y -= len;
    y++;
  }
  dir &= 1;  
#ifdef U8G2_WITH_CLIPPING
  u8g2_draw_hv_line_2dir(u8g2, x, y, len, dir);
#else
  u8g2->ll_hvline(u8g2, x, y, len, dir);
  //u8g2_draw_low_level_hv_line(u8g2, x, y, len, dir);
#endif
}

/*
  This is the toplevel function for the hv line draw procedures.
  This function should be called by the user.
*/
void u8g2_DrawHVLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
{
  /* Make a call to the callback function. The callback may rotate the hv line */
  /* after rotation this will call u8g2_draw_hv_line_4dir() */
  if ( len != 0 )
    u8g2->cb->draw_l90(u8g2, x, y, len, dir);
}

void u8g2_DrawHLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len)
{
#ifdef U8G2_WITH_INTERSECTION
  if ( u8g2_IsIntersection(u8g2, x, y, x+len, y+1) == 0 ) 
    return;
#endif /* U8G2_WITH_INTERSECTION */
  u8g2_DrawHVLine(u8g2, x, y, len, 0);
}

void u8g2_DrawVLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len)
{
#ifdef U8G2_WITH_INTERSECTION
  if ( u8g2_IsIntersection(u8g2, x, y, x+1, y+len) == 0 ) 
    return;
#endif /* U8G2_WITH_INTERSECTION */
  u8g2_DrawHVLine(u8g2, x, y, len, 1);
}

void u8g2_DrawPixel(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y)
{
#ifdef U8G2_WITH_INTERSECTION
  if ( x < u8g2->user_x0 )
    return;
  if ( x >= u8g2->user_x1 )
    return;
  if ( y < u8g2->user_y0 )
    return;
  if ( y >= u8g2->user_y1 )
    return;
#endif /* U8G2_WITH_INTERSECTION */
  u8g2_DrawHVLine(u8g2, x, y, 1, 0);
}





