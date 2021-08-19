
#include "ui.h"
#include "u8g2.h"
#include "ui_u8g2.h"

/*

uint8_t uif_template(ui_t *ui, uint8_t msg)
{
  //ui->dflags                          UIF_DFLAG_IS_CURSOR_FOCUS       UIF_DFLAG_IS_TOUCH_FOCUS
  //uif_get_cflags(ui->uif)       UIF_CFLAG_IS_CURSOR_SELECTABLE
  //uif_get_data(ui->uif)
  switch(msg)
  {
    case UIF_MSG_DRAW:
      break;
    case UIF_MSG_FORM_START:
      break;
    case UIF_MSG_FORM_END:
      break;
    case UIF_MSG_CURSOR_ENTER:
      break;
    case UIF_MSG_CURSOR_SELECT:
      break;
    case UIF_MSG_CURSOR_LEAVE:
      break;
    case UIF_MSG_TOUCH_DOWN:
      break;
    case UIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}
*/

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

  U8G2_BTN_PADWIDTH | padding_h_or_width >= u8g2_GetDisplayWidth(u8g2)/2 | Description
    no          |  no | padding_h_or_width is the left and right gab between text and border
    no          |  yes | the total width of the button is equal to the display width
    yes         | - | The inner width of the button is equal to padding_h_or_width or the text width (whatever is larger)
    


  if padding_h is larger than or equal to half of the display width, then the button size is maximized to the display size
*/



void u8g2_DrawButtonUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t padding_h_or_width, u8g2_uint_t padding_v, const char *text)
{
  u8g2_uint_t w = u8g2_GetUTF8Width(u8g2, text);
  
  u8g2_uint_t xx, yy, ww, hh;
  
  u8g2_uint_t border_width = flags & U8G2_BTN_BW_MASK;
  u8g2_uint_t padding_h = padding_h_or_width;
  u8g2_uint_t text_x_offset = 0;        // used for U8G2_BTN_PADWIDTH mode

  int8_t a = u8g2_GetAscent(u8g2);
  int8_t d = u8g2_GetDescent(u8g2);
  uint8_t color_backup = u8g2->draw_color;


  if ( flags & U8G2_BTN_HCENTER )
    x -= (w)/2;

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
    u8g2_DrawFrame(u8g2, xx, yy, ww, hh);
    
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
  }
  if ( flags & U8G2_BTN_INV )
  {
    u8g2_DrawBox(u8g2, xx, yy, ww, hh);
    u8g2_SetDrawColor(u8g2, 1-u8g2->draw_color);
  }
  u8g2_DrawUTF8(u8g2, x,y, text);
  u8g2_SetDrawColor(u8g2, color_backup);
}



void nu8g2_DrawButtonUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text)
{
  u8g2_uint_t w = u8g2_GetUTF8Width(u8g2, text);
  
  u8g2_uint_t xx, yy, ww, hh;
  
  u8g2_uint_t border_width = flags & U8G2_BTN_BW_MASK;
  u8g2_uint_t text_x_offset = 0;        // used for U8G2_BTN_PADWIDTH mode

  int8_t a = u8g2_GetAscent(u8g2);
  int8_t d = u8g2_GetDescent(u8g2);
  uint8_t color_backup = u8g2->draw_color;


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
    
  for(;;)
  {

    xx = x;
    xx -= text_x_offset;
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
    u8g2_DrawFrame(u8g2, xx, yy, ww, hh);
    
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
  }
  if ( flags & U8G2_BTN_INV )
  {
    u8g2_DrawBox(u8g2, xx, yy, ww, hh);
    u8g2_SetDrawColor(u8g2, 1-u8g2->draw_color);
  }
  u8g2_DrawUTF8(u8g2, x,y, text);
  u8g2_SetDrawColor(u8g2, color_backup);
}


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

/*=========================================================================*/
/* helper function */

u8g2_uint_t ui_get_x(ui_t *ui)
{
  return ui->x;
}

u8g2_uint_t ui_get_y(ui_t *ui)
{
  return ui->y;
}

u8g2_t *ui_get_U8g2(ui_t *ui)
{
  return (u8g2_t *)(ui->graphics_data);
}

/*=========================================================================*/
/* field functions */

uint8_t uif_label_u8g2(ui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case UIF_MSG_DRAW:
      //printf("DRAW fds=%p uif=%p text=%s\n", ui->fds, ui->uif, ui->text);
      u8g2_DrawStr(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), ui->text);
      //u8g2_DrawButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER | U8G2_BTN_BW1 | U8G2_BTN_INV, 4, 1, ui->text);
      //u8g2_DrawButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER | U8G2_BTN_PADWIDTH | U8G2_BTN_SHADOW2 | 2, 100, 1, ui->text);
      //u8g2_DrawRButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER  | U8G2_BTN_INV | 3, 2, 1, ui->text);
      //u8g2_DrawRButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER   | 3, 2, 1, 8, ui->text);
      break;
    case UIF_MSG_FORM_START:
      break;
    case UIF_MSG_FORM_END:
      break;
    case UIF_MSG_CURSOR_ENTER:
      break;
    case UIF_MSG_CURSOR_SELECT:
      break;
    case UIF_MSG_CURSOR_LEAVE:
      break;
    case UIF_MSG_TOUCH_DOWN:
      break;
    case UIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

uint8_t uif_frame_button_bold_select_u8g2(ui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = ui_get_U8g2(ui);
  u8g2_uint_t flags = U8G2_BTN_HCENTER | U8G2_BTN_PADWIDTH;
  switch(msg)
  {
    case UIF_MSG_DRAW:
      //printf("DRAW fds=%p uif=%p text=%s\n", ui->fds, ui->uif, ui->text);
      //u8g2_DrawStr(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), ui->text);
      //u8g2_DrawButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER | U8G2_BTN_BW1 | U8G2_BTN_INV, 4, 1, ui->text);
      //u8g2_DrawButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER | U8G2_BTN_PADWIDTH | U8G2_BTN_SHADOW2 | 2, 100, 1, ui->text);
      //u8g2_DrawRButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER  | U8G2_BTN_INV | 3, 2, 1, ui->text);
      if ( ui->dflags & UIF_DFLAG_IS_CURSOR_FOCUS )
      {
        flags |= 3;
      }
      else
      {
        flags |= 1;
      }
      u8g2_DrawButtonUTF8(u8g2, ui_get_x(ui), ui_get_y(ui), flags, u8g2_GetDisplayWidth(u8g2)/2 - 10, 1, ui->text);
      break;
    case UIF_MSG_FORM_START:
      break;
    case UIF_MSG_FORM_END:
      break;
    case UIF_MSG_CURSOR_ENTER:
      break;
    case UIF_MSG_CURSOR_SELECT:
      break;
    case UIF_MSG_CURSOR_LEAVE:
      break;
    case UIF_MSG_TOUCH_DOWN:
      break;
    case UIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

/*
  - Fixed width: Half of the display width -10
  - Inverted if selected
  - Reference Position: x=Center, y=Text Baseline
*/
uint8_t uif_frame_button_invers_select_u8g2(ui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = ui_get_U8g2(ui);
  u8g2_uint_t flags = U8G2_BTN_HCENTER | U8G2_BTN_PADWIDTH | 1;
  switch(msg)
  {
    case UIF_MSG_DRAW:
      //printf("DRAW fds=%p uif=%p text=%s\n", ui->fds, ui->uif, ui->text);
      //u8g2_DrawStr(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), ui->text);
      //u8g2_DrawButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER | U8G2_BTN_BW1 | U8G2_BTN_INV, 4, 1, ui->text);
      //u8g2_DrawButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER | U8G2_BTN_PADWIDTH | U8G2_BTN_SHADOW2 | 2, 100, 1, ui->text);
      //u8g2_DrawRButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER  | U8G2_BTN_INV | 3, 2, 1, ui->text);
      if ( ui->dflags & UIF_DFLAG_IS_CURSOR_FOCUS )
      {
        flags |= U8G2_BTN_INV;
      }
      u8g2_DrawButtonUTF8(u8g2, ui_get_x(ui), ui_get_y(ui), flags, u8g2_GetDisplayWidth(u8g2)/2 - 10, 1, ui->text);
      break;
    case UIF_MSG_FORM_START:
      break;
    case UIF_MSG_FORM_END:
      break;
    case UIF_MSG_CURSOR_ENTER:
      break;
    case UIF_MSG_CURSOR_SELECT:
      break;
    case UIF_MSG_CURSOR_LEAVE:
      break;
    case UIF_MSG_TOUCH_DOWN:
      break;
    case UIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

/*
  - Fixed width: width of display width
  - Inverted if selected
  - Reference Position: x=lower left of text, y=Text Baseline
*/
uint8_t uif_line_button_invers_select_u8g2(ui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = ui_get_U8g2(ui);
  //u8g2_uint_t flags = U8G2_BTN_HCENTER ;
  u8g2_uint_t flags = 0;
  switch(msg)
  {
    case UIF_MSG_DRAW:
      //printf("DRAW fds=%p uif=%p text=%s\n", ui->fds, ui->uif, ui->text);
      //u8g2_DrawStr(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), ui->text);
      //u8g2_DrawButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER | U8G2_BTN_BW1 | U8G2_BTN_INV, 4, 1, ui->text);
      //u8g2_DrawButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER | U8G2_BTN_PADWIDTH | U8G2_BTN_SHADOW2 | 2, 100, 1, ui->text);
      //u8g2_DrawRButtonUTF8(ui_get_U8g2(ui), ui_get_x(ui), ui_get_y(ui), U8G2_BTN_HCENTER  | U8G2_BTN_INV | 3, 2, 1, ui->text);
      if ( ui->dflags & UIF_DFLAG_IS_CURSOR_FOCUS )
      {
        flags |= U8G2_BTN_INV;
      }
      u8g2_DrawButtonUTF8(u8g2, ui_get_x(ui), ui_get_y(ui), flags, u8g2_GetDisplayWidth(u8g2)/2 , 0, ui->text);
      break;
    case UIF_MSG_FORM_START:
      break;
    case UIF_MSG_FORM_END:
      break;
    case UIF_MSG_CURSOR_ENTER:
      break;
    case UIF_MSG_CURSOR_SELECT:
      break;
    case UIF_MSG_CURSOR_LEAVE:
      break;
    case UIF_MSG_TOUCH_DOWN:
      break;
    case UIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

uint8_t uif_goto_frame_button_invers_select_u8g2(ui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case UIF_MSG_CURSOR_SELECT:
      return ui_GotoForm(ui, ui->arg);
  }
  return uif_frame_button_invers_select_u8g2(ui, msg);
}


uint8_t uif_goto_line_button_invers_select_u8g2(ui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case UIF_MSG_CURSOR_SELECT:
      return ui_GotoForm(ui, ui->arg);
  }
  return uif_line_button_invers_select_u8g2(ui, msg);
}


/*
  data: uint8_t *

  Uses:
    ui->arg     --> total field width

*/
uint8_t uif_input_uint8_invers_select_u8g2(ui_t *ui, uint8_t msg)
{
  //ui->dflags                          UIF_DFLAG_IS_CURSOR_FOCUS       UIF_DFLAG_IS_TOUCH_FOCUS
  //uif_get_cflags(ui->uif)       UIF_CFLAG_IS_CURSOR_SELECTABLE
  u8g2_t *u8g2 = ui_get_U8g2(ui);
  u8g2_uint_t flags = U8G2_BTN_PADWIDTH;
  uint8_t *value = (uint8_t *)uif_get_data(ui->uif);
  char buf[6];
  switch(msg)
  {
    case UIF_MSG_DRAW:
      if ( *value > 9 ) *value = 9;
      buf[0] = (char)(*value + '0');
      buf[1] = '\0';
      if ( ui->dflags & UIF_DFLAG_IS_CURSOR_FOCUS )
      {
        flags |= U8G2_BTN_INV;
      }
      u8g2_DrawButtonUTF8(u8g2, ui_get_x(ui), ui_get_y(ui), flags, ui->arg, 1, buf);
      
      break;
    case UIF_MSG_FORM_START:
      break;
    case UIF_MSG_FORM_END:
      break;
    case UIF_MSG_CURSOR_ENTER:
      break;
    case UIF_MSG_CURSOR_SELECT:
      (*value)++;
      if ( *value > 9 ) *value = 0;      
      break;
    case UIF_MSG_CURSOR_LEAVE:
      break;
    case UIF_MSG_TOUCH_DOWN:
      break;
    case UIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}

/*
  data: uint8_t *
  will use options from FSD
*/
uint8_t uif_single_line_option_invers_select_u8g2(ui_t *ui, uint8_t msg)
{
  //ui->dflags                          UIF_DFLAG_IS_CURSOR_FOCUS       UIF_DFLAG_IS_TOUCH_FOCUS
  //uif_get_cflags(ui->uif)       UIF_CFLAG_IS_CURSOR_SELECTABLE
  u8g2_t *u8g2 = ui_get_U8g2(ui);
  u8g2_uint_t flags = U8G2_BTN_PADWIDTH;
  //u8g2_uint_t flags = 0;
  uint8_t *value = (uint8_t *)uif_get_data(ui->uif);
  switch(msg)
  {
    case UIF_MSG_DRAW:
      if ( ui_fds_get_nth_token(ui, *value) == 0 )
      {
        *value = 0;
        ui_fds_get_nth_token(ui, *value);
      }      
      if ( ui->dflags & UIF_DFLAG_IS_CURSOR_FOCUS )
      {
        flags |= U8G2_BTN_INV;
      }
      u8g2_DrawButtonUTF8(u8g2, ui_get_x(ui), ui_get_y(ui), flags, ui->arg, 1, ui->text);
      
      break;
    case UIF_MSG_FORM_START:
      break;
    case UIF_MSG_FORM_END:
      break;
    case UIF_MSG_CURSOR_ENTER:
      break;
    case UIF_MSG_CURSOR_SELECT:
      (*value)++;
      if ( ui_fds_get_nth_token(ui, *value) == 0 ) 
        *value = 0;      
      break;
    case UIF_MSG_CURSOR_LEAVE:
      break;
    case UIF_MSG_TOUCH_DOWN:
      break;
    case UIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}
