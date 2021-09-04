/*

  u8g2_button.c

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

  Description: 
    Draws normal or inverted text with optional frame around text.
    The text (and the frame) can be horizontally centered around the provided reference position.
    This procedure will use the current draw color and current font. The height of the frame
    depends on the setting of setFontRefHeightText(), setFontRefHeightExtendedText() or setFontRefHeightAll()


  Note 1: drawColor 2 (XOR) is not supported. Result will be broken with draw color 2.
  Note 2: This procedure will enforce font mode 1 (transparent mode)
  Note 3: Some fonts may add an extra gap on the right side. This is a font problem and can not be avoided (for example inb16 has this problem).
  

  The height of the button is defined by the current font and
    setFontRefHeightText,
    setFontRefHeightExtendedText
    setFontRefHeightAll
  Right padding might be influenced by the font.
  For example u8g2_font_inb16 may add an extra pixel gap (increase padding by one) 
  on the right side.
  The current draw color is considered, but only draw color 0 and 1 are supported
  
  This function requires transparent font mode: setFontMode(1)
  
  Side effect: Font transparent mode is enabled setFontMode(1)

*/



/*

  flags:

    U8G2_BTN_BW1 0x01
    U8G2_BTN_BW2 0x02
    U8G2_BTN_BW3 0x03


    U8G2_BTN_SHADOW0 0x08
    U8G2_BTN_SHADOW1 0x10
    U8G2_BTN_SHADOW2 0x18

    U8G2_BTN_INV 0x20

    U8G2_BTN_HCENTER 0x40

    U8G2_BTN_XFRAME 0x80

  Total size without shadow: width+2*padding_h+2*border
  Total size with U8G2_BTN_SHADOW0: width+2*padding_h+3*border
  Total size with U8G2_BTN_SHADOW1: width+2*padding_h+3*border+1
  Total size with U8G2_BTN_SHADOW2: width+2*padding_h+3*border+2
  
  U8G2_BTN_XFRAME:
    draw another one pixel frame with one pixel gap, will not look good with shadow
*/

void u8g2_DrawButtonFrame(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t text_width, u8g2_uint_t padding_h, u8g2_uint_t padding_v)
{
  u8g2_uint_t w = text_width;
  
  u8g2_uint_t xx, yy, ww, hh;
  
  u8g2_uint_t gap_frame = U8G2_BTN_BW_MASK+1;
    
  u8g2_uint_t border_width = flags & U8G2_BTN_BW_MASK;

  int8_t a = u8g2_GetAscent(u8g2);
  int8_t d = u8g2_GetDescent(u8g2);
  
  uint8_t color_backup = u8g2->draw_color;
  
  
  if ( flags & U8G2_BTN_XFRAME )
  {
    border_width++;
    gap_frame = border_width;
    border_width++;
  }
  

  
  for(;;)
  {

    xx = x;
    xx -= padding_h;
    xx -= border_width;
    ww = w+2*padding_h+2*border_width;
    
    yy = y;
    yy += u8g2->font_calc_vref(u8g2);
    yy -= a;
    yy -= padding_v;
    yy -= border_width;
    hh = a-d+2*padding_v+2*border_width;
    if ( border_width == 0 )
      break;
    if ( border_width == gap_frame )
    {
      u8g2_SetDrawColor(u8g2, color_backup == 0 ? 1 : 0);
    }
    u8g2_DrawFrame(u8g2, xx, yy, ww, hh);
    u8g2_SetDrawColor(u8g2, color_backup);
    
    if ( flags & U8G2_BTN_SHADOW_MASK )
    {
      if ( border_width == (flags & U8G2_BTN_BW_MASK) )
      {
        u8g2_uint_t i;
        u8g2_uint_t shadow_gap = (flags & U8G2_BTN_SHADOW_MASK) >> U8G2_BTN_SHADOW_POS;
        shadow_gap--;
        for( i = 0; i < border_width; i++ )
        {
          u8g2_DrawHLine(u8g2, xx+border_width+shadow_gap,yy+hh+i+shadow_gap,ww);
          u8g2_DrawVLine(u8g2, xx+ww+i+shadow_gap,yy+border_width+shadow_gap,hh);
        }
      }
    }
    border_width--;
  } /* for */
  
  if ( flags & U8G2_BTN_INV )
  {
    u8g2_SetDrawColor(u8g2, 2);         /* XOR */
    u8g2_DrawBox(u8g2, xx, yy, ww, hh);
    u8g2_SetDrawColor(u8g2, color_backup);
  }
}

void u8g2_DrawButtonUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text)
{
  u8g2_uint_t w = u8g2_GetUTF8Width(u8g2, text);
  
  u8g2_uint_t text_x_offset = 0; 

  if ( flags & U8G2_BTN_HCENTER )
    x -= (w+1)/2;

  if ( w < width )
  {
    if ( flags & U8G2_BTN_HCENTER )
    {
      text_x_offset = (width-w)/2;
    }
    w = width;
  }
  
  u8g2_SetFontMode(u8g2, 1);    
  u8g2_DrawUTF8(u8g2, x,y, text);
  u8g2_DrawButtonFrame(u8g2, x-text_x_offset, y, flags, w, padding_h, padding_v);
  
}



#ifdef NOT_USED
void u8g2_Draw4Pixel(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w)
{
  u8g2_DrawPixel(u8g2, x,y-1);
  u8g2_DrawPixel(u8g2, x+w-1,y-1);
  u8g2_DrawPixel(u8g2, x+w-1,y-w);
  u8g2_DrawPixel(u8g2, x,y-w);
}

void u8g2_DrawRadio(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t is_checked)
{
  uint8_t color_backup = u8g2->draw_color;
  u8g2_DrawCheckbox(u8g2, x,y,w,is_checked);
  u8g2_SetDrawColor(u8g2, 2);
  u8g2_Draw4Pixel(u8g2, x,y,w);
  if ( is_checked )
  {
    //u8g2_Draw4Pixel(u8g2, x+2,y-2,w-4);
  }
  
  u8g2_SetDrawColor(u8g2, color_backup );
}
#endif
 

#ifdef _THIS_CODE_SHOULD_BE_REWRITTEN_WITHOUT_PADWIDTH_

/*
  Shadow is not supported
  Note: radius must be at least as high as the border width

  border width | good radius values
  1             | 3, 5, 7, 8, ...
  2             | 3, 5, 7, 8, ...
  
*/

void u8g2_DrawRButtonUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t padding_h_or_width, u8g2_uint_t padding_v, u8g2_uint_t r, const char *text)
{
  u8g2_uint_t w = u8g2_GetUTF8Width(u8g2, text);
  //u8g2_uint_t w = u8g2_GetExactStrWidth(u8g2, text);
  
  u8g2_uint_t xx, yy, ww, hh;
  
  u8g2_uint_t border_width = flags & U8G2_BTN_BW_MASK;
  u8g2_uint_t padding_h = padding_h_or_width;
  u8g2_uint_t text_x_offset = 0;        // used for U8G2_BTN_PADWIDTH mode

  int8_t a = u8g2_GetAscent(u8g2);
  int8_t d = u8g2_GetDescent(u8g2);
  uint8_t color_backup = u8g2->draw_color;


  if ( flags & U8G2_BTN_HCENTER )
    x -= w/2;

  if ( flags & U8G2_BTN_PADWIDTH )
  {
    padding_h = 0;
    if ( w < padding_h_or_width )
    {
      if ( flags & U8G2_BTN_HCENTER )
      {
        text_x_offset = (padding_h_or_width-w)/2;
      }
      w = padding_h_or_width;
    }
  }
  
  
  u8g2_SetFontMode(u8g2, 1);
    
  for(;;)
  {
    if ( padding_h >= u8g2_GetDisplayWidth(u8g2)/2 )    // padding_h is zero if U8G2_BTN_PADWIDTH is set
    {
      xx = (flags & U8G2_BTN_BW_MASK) - border_width;
      ww = u8g2_GetDisplayWidth(u8g2);
      ww -= 2*((flags & U8G2_BTN_BW_MASK) - border_width);
      //printf("xx=%d ww=%d\n", xx, ww);
      //printf("clip_x1=%d clip_x0=%d\n", u8g2->clip_x1, u8g2->clip_x0);
    }
    else
    {
      xx = x;
      xx -= text_x_offset;
      xx -= padding_h;
      xx -= border_width;
      ww = w+2*padding_h+2*border_width;
    }
    
    yy = y;
    yy += u8g2->font_calc_vref(u8g2);
    yy -= a;
    yy -= padding_v;
    yy -= border_width;
    hh = a-d+2*padding_v+2*border_width;
    if ( border_width == 0 )
      break;
    u8g2_DrawRFrame(u8g2, xx, yy, ww, hh, r);
    if ( (flags & U8G2_BTN_BW_MASK) > 1 )
      u8g2_DrawRFrame(u8g2, xx, yy, ww, hh, r+1);
    
    border_width--;
    if ( r > 1 )
      r--;
  }
  if ( flags & U8G2_BTN_INV )
  {
    u8g2_DrawRBox(u8g2, xx, yy, ww, hh,r);
    u8g2_SetDrawColor(u8g2, 1-u8g2->draw_color);
  }
  u8g2_DrawUTF8(u8g2, x,y, text);
  u8g2_SetDrawColor(u8g2, color_backup);
}

#endif
