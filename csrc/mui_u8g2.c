/*

  mui_u8g2.c

  Monochrome minimal user interface: Glue code between mui and u8g2.

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

#include "mui.h"
#include "u8g2.h"
#include "mui_u8g2.h"

/*

uint8_t mui_template(mui_t *ui, uint8_t msg)
{
  //u8g2_t *u8g2 = mui_get_U8g2(ui);
  //ui->dflags                          MUIF_DFLAG_IS_CURSOR_FOCUS       MUIF_DFLAG_IS_TOUCH_FOCUS
  //muif_get_cflags(ui->uif)       MUIF_CFLAG_IS_CURSOR_SELECTABLE
  //muif_get_data(ui->uif)
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}
*/


/*=========================================================================*/
/* extra u8g2 drawing functions */


void u8g2_DrawCheckbox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t is_checked)
{
  u8g2_DrawFrame(u8g2, x, y-w, w, w);
  if ( is_checked )
  {
    w-=4;
    u8g2_DrawBox(u8g2, x+2, y-w-2, w, w);
  }
}

void u8g2_DrawValueMark(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w)
{
  u8g2_DrawBox(u8g2, x, y-w, w, w);
}


/*=========================================================================*/
/* helper function */

u8g2_uint_t mui_get_x(mui_t *ui)
{
  return ui->x;
}

u8g2_uint_t mui_get_y(mui_t *ui)
{
  return ui->y;
}

u8g2_t *mui_get_U8g2(mui_t *ui)
{
  return (u8g2_t *)(ui->graphics_data);
}




/*=========================================================================*/
/* incomplete (draw only) field functions */



/*
  xy: yes, arg: no, text: yes
*/
uint8_t mui_frame_button_bold_select_u8g2(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = U8G2_BTN_HCENTER;
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      //printf("DRAW fds=%p uif=%p text=%s\n", ui->fds, ui->uif, ui->text);
      //u8g2_DrawStr(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), ui->text);
      //u8g2_DrawButtonUTF8(mui_get_U8g2(mui), mui_get_x(mui), mui_get_y(mui), U8G2_BTN_HCENTER | U8G2_BTN_BW1 | U8G2_BTN_INV, 4, 1, ui->text);
      //u8g2_DrawButtonUTF8(mui_get_U8g2(mui), mui_get_x(mui), mui_get_y(mui), U8G2_BTN_HCENTER | U8G2_BTN_PADWIDTH | U8G2_BTN_SHADOW2 | 2, 100, 1, ui->text);
      //u8g2_DrawRButtonUTF8(mui_get_U8g2(mui), mui_get_x(mui), mui_get_y(mui), U8G2_BTN_HCENTER  | U8G2_BTN_INV | 3, 2, 1, ui->text);
      if ( ui->dflags & MUIF_DFLAG_IS_CURSOR_FOCUS )
      {
        flags |= 3;
      }
      else
      {
        flags |= 1;
      }
      u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, u8g2_GetDisplayWidth(u8g2)/2 - 10, 0, 1, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

/*

  uint8_t mui_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A button with size equal to the inner text width plus 1 pixel extra padding
    The button has a one pixel frame around the text.
    
  Message Handling: DRAW

  Style
    No Selection: Text + Frame
    Cursor Selection: Inverted text + Frame

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: not used

  Field definition string (fds):
    xy: Center position of the text (required)
    arg: not used
    text: Button label
    
*/

uint8_t mui_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = U8G2_BTN_HCENTER | 1;
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, 0, 1, 1, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

/*

  uint8_t mui_half_width_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A button with size equal to display width / 2 - 10 pixel
    The button has a one pixel frame around the text.
    
  Message Handling: DRAW

  Style
    No Selection: Text + Frame
    Cursor Selection: Inverted text + Frame

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: not used

  Field definition string (fds):
    xy: Center position of the text (required)
    arg: not used
    text: Button label
    
*/

uint8_t mui_half_width_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = U8G2_BTN_HCENTER | 1;
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, u8g2_GetDisplayWidth(u8g2)/2 - 10, 0, 1, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

/*

  uint8_t mui_line_button_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A full line button (covers complete width of the display).
    
  Message Handling: DRAW

  Style
    No Selection: Text only
    Cursor Selection: Inverted text

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: not used

  Field definition string (fds):
    xy: Left position of the text (required)
    arg: not used
    text: Button label
    
*/
uint8_t mui_line_button_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  //u8g2_uint_t flags = U8G2_BTN_HCENTER ;
  u8g2_uint_t flags = 0;
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui) , 0, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}


/*

  uint8_t mui_radio_mark_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A selectable button with a rectangle in front of it, if arg is equal to the current value (*(uint8_t *)data)
    If the text argument is empty, then this cb tries to get the text from the parent.
    
  Message Handling: DRAW

  Style
    No Selection: Text only
    Cursor Selection: Inverted text

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: uint8_t *, pointer to a uint8_t variable, which defines, whether there is a rectangle in front of the text

  Field definition string (fds):
    xy: Left position of the text (required)
    arg: The number of this button, which is compared against *(uint8_t *)data (required)
    text: Button label (optional, might be taken from previous field)
    
*/
uint8_t mui_radio_mark_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = 0;
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  //if ( value == NULL )
  //  value = &(ui->selected_value);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      
      {
        u8g2_uint_t w = 0;
        u8g2_uint_t a = u8g2_GetAscent(u8g2) - 2;
        u8g2_uint_t x = mui_get_x(ui);   // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        u8g2_uint_t y = mui_get_y(ui);  // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        if ( *value == ui->arg )
          u8g2_DrawValueMark(u8g2, x, y, a);

        if ( ui->text[0] == '\0' )
        {
          /* if the text is not provided, then try to get the text from the previous (saved) element, assuming that this contains the selection */
          /* this will overwrite all ui member functions, so we must not access any ui members (except ui->text) any more */
          mui_GetSelectableFieldTextOption(ui, ui->last_form_id, ui->last_form_cursor_focus_position, ui->arg);
        }
        
        if ( ui->text[0] != '\0' )
        {
          w =  u8g2_GetUTF8Width(u8g2, ui->text);
          u8g2_SetFontMode(u8g2, 1);
          a += 2;       /* add gap between the checkbox and the text area */
          u8g2_DrawUTF8(u8g2, x+a, y, ui->text);
        }
        
        u8g2_DrawButtonFrame(u8g2, x, y, flags, w+a, 1, 1);
      }
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      *value = ui->arg;
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
  
}

/*

  uint8_t mui_radio_mark_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A selectable button with a checkbox in front of it. Selected, if arg is equal to the current value (*(uint8_t *)data)
    
  Message Handling: DRAW

  Style
    No Selection: Text only
    Cursor Selection: Inverted text

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: uint8_t *, pointer to a uint8_t variable, which defines, whether there checkbox is checked or not
  Field definition string (fds):
    xy: Left position of the text (required)
    arg: The number of this button, which is compared against *(uint8_t *)data (required)
    text: Button label (usually required)
    
*/
uint8_t mui_radio_checkbox_mark_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = 0;
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      
      {
        u8g2_uint_t w = 0;
        u8g2_uint_t a = u8g2_GetAscent(u8g2);
        if ( *value == ui->arg )
          u8g2_DrawCheckbox(u8g2, mui_get_x(ui), mui_get_y(ui), a, 1);
        else
          u8g2_DrawCheckbox(u8g2, mui_get_x(ui), mui_get_y(ui), a, 0);
        
        if ( ui->text[0] != '\0' )
        {
          w =  u8g2_GetUTF8Width(u8g2, ui->text);
          u8g2_SetFontMode(u8g2, 1);
          a += 2;       /* add gap between the checkbox and the text area */
          u8g2_DrawUTF8(u8g2, mui_get_x(ui)+a, mui_get_y(ui), ui->text);
        }
        
        u8g2_DrawButtonFrame(u8g2, mui_get_x(ui), mui_get_y(ui), flags, w+a, 1, 1);
      }
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
  
}

/*

  uint8_t mui_radio_mark_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A selectable text with a checkbox in front of it. Selected, if *(uint8_t *)data is not 0.
    
  Message Handling: DRAW

  Style
    No Selection: Text only
    Cursor Selection: Inverted text

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: uint8_t *, pointer to a uint8_t variable, which defines, whether there checkbox is checked or not
  Field definition string (fds):
    xy: Left position of the text (required)
    arg: not used
    text: Button label (optional)
    
*/
uint8_t mui_checkbox_mark_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = 0;
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( *value > 1 ) *value = 1;
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      
      {
        u8g2_uint_t w = 0;
        u8g2_uint_t a = u8g2_GetAscent(u8g2);
        if ( *value )
          u8g2_DrawCheckbox(u8g2, mui_get_x(ui), mui_get_y(ui), a, 1);
        else
          u8g2_DrawCheckbox(u8g2, mui_get_x(ui), mui_get_y(ui), a, 0);
        
        if ( ui->text[0] != '\0' )
        {
          w =  u8g2_GetUTF8Width(u8g2, ui->text);
          u8g2_SetFontMode(u8g2, 1);
          a += 2;       /* add gap between the checkbox and the text area */
          u8g2_DrawUTF8(u8g2, mui_get_x(ui)+a, mui_get_y(ui), ui->text);
        }
        
        u8g2_DrawButtonFrame(u8g2, mui_get_x(ui), mui_get_y(ui), flags, w+a, 1, 1);
      }
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
  
}





/*=========================================================================*/
/* ready to use field functions */

/*
  xy: yes, arg: no, text: yes
*/

uint8_t mui_label_u8g2(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_DrawStr(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}


/*

  uint8_t mui_goto_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A button with size equal to button text plus one pixel padding
    The button has a one pixel frame around the text.
    If the selected, then the form will change to the specified form number.
    
  Message Handling: DRAW, CURSOR_SELECT

  Style
    No Selection: Text + Frame
    Cursor Selection: Inverted text + Frame

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: not used

  Field definition string (fds):
    xy: Left position of the text (required)
    arg: Form numner (required)
    text: Button label
    
*/
uint8_t mui_goto_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
      return mui_GotoForm(ui, ui->arg, 0);
  }
  return mui_frame_button_invers_select_u8g2(ui, msg);
}

/*

  uint8_t mui_goto_half_width_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A button with size equal to display width / 2 - 10 pixel
    The button has a one pixel frame around the text.
    If the selected, then the form will change to the specified form number.
    
  Message Handling: DRAW, CURSOR_SELECT

  Style
    No Selection: Text + Frame
    Cursor Selection: Inverted text + Frame

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: not used

  Field definition string (fds):
    xy: Left position of the text (required)
    arg: Form numner (required)
    text: Button label
    
*/
uint8_t mui_goto_half_width_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
      return mui_GotoForm(ui, ui->arg, 0);
  }
  return mui_half_width_frame_button_invers_select_u8g2(ui, msg);
}

/*

  uint8_t mui_leave_menu_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A button with size equal to button text plus one pixel padding
    The button has a one pixel frame around the text.
    If the selected, then the menu system will be closed.
    The arg value will be stored at the specified data location (if not NULL).
    The arg value can be used as an exit value of the button.
    
  Message Handling: DRAW, CURSOR_SELECT

  Style
    No Selection: Text + Frame
    Cursor Selection: Inverted text + Frame

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: Optionally points to uint8_t value which will receive the arg value of the field.

  Field definition string (fds):
    xy: Left position of the text (required)
    arg: Value which will be stored at *data (optional)
    text: Button label
    
*/
uint8_t mui_leave_menu_frame_button_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
      {
        uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
        if ( value != NULL )
          *value = ui->arg;
      }
      mui_LeaveForm(ui);
      return 1;
  }
  return mui_frame_button_invers_select_u8g2(ui, msg);
}


/*

  uint8_t mui_goto_line_button_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    A full line button (covers complete width of the display).
    If the selected, then the form will change to the specified form number.
    
  Message Handling: DRAW, CURSOR_SELECT

  Style
    No Selection: Text only
    Cursor Selection: Inverted text

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: not used

  Field definition string (fds):
    xy: Left position of the text (required)
    arg: If selected, the form will change to the form specified by this argument
    text: Button label
    
*/

uint8_t mui_goto_line_button_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
      return mui_GotoForm(ui, ui->arg, 0);
  }
  return mui_line_button_invers_select_u8g2(ui, msg);
}


/*

  uint8_t mui_input_uint8_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    Select a number between 0 and 9.
    Only one number is visible.
    The visible option is automatically the selected number.
    The selected number is stored ad the target data location.

  Message Handling: DRAW, SELECT

  Style
    No Selection: Number only
    Cursor Selection: Inverted number

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: uint8_t *, pointer to a uint8_t variable, which contains the selected option 

  Field definition string (fds):
    xy: Left position of the text (required)
    arg: not used 
    text: not used
    
*/
uint8_t mui_input_uint8_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  //ui->dflags                          MUIF_DFLAG_IS_CURSOR_FOCUS       MUIF_DFLAG_IS_TOUCH_FOCUS
  //mui_get_cflags(ui->uif)       MUIF_CFLAG_IS_CURSOR_SELECTABLE
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = 0;
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  char buf[6];
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( *value > 9 ) *value = 9;
      buf[0] = (char)(*value + '0');
      buf[1] = '\0';
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      
      u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, u8g2_GetStrWidth(u8g2, "9"), 1, 1, buf);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      (*value)++;
      if ( *value > 9 ) *value = 0;      
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}

/*

  uint8_t mui_single_line_option_invers_select_u8g2(mui_t *ui, uint8_t msg)

  Description:
    Select one of several options. First option has value 0.
    Only one option is visible.
    The visible option is automatically the selected option.

  Message Handling: DRAW, SELECT

  Style
    No Selection: Text only
    Cursor Selection: Inverted text

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: uint8_t *, pointer to a uint8_t variable, which contains the selected option 

  Field definition string (fds):
    xy: Left position of the text (required)
    arg: total width of the selectable option (optional), 
    text: '|' separated list of options
    
*/
uint8_t mui_single_line_option_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  //ui->dflags                          MUIF_DFLAG_IS_CURSOR_FOCUS       MUIF_DFLAG_IS_TOUCH_FOCUS
  //mui_get_cflags(ui->uif)       MUIF_CFLAG_IS_CURSOR_SELECTABLE
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = 0;
  //u8g2_uint_t flags = 0;
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_fds_get_nth_token(ui, *value) == 0 )
      {
        *value = 0;
        mui_fds_get_nth_token(ui, *value);
      }      
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, ui->arg, 1, 1, ui->text);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      (*value)++;
      if ( mui_fds_get_nth_token(ui, *value) == 0 ) 
        *value = 0;      
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}
   


uint8_t mui_select_options_parent_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  //ui->dflags                          MUIF_DFLAG_IS_CURSOR_FOCUS       MUIF_DFLAG_IS_TOUCH_FOCUS
  //mui_get_cflags(ui->uif)       MUIF_CFLAG_IS_CURSOR_SELECTABLE
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = 0;
  //u8g2_uint_t flags = 0;
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_fds_get_nth_token(ui, *value) == 0 )
      {
        *value = 0;
        mui_fds_get_nth_token(ui, *value);
      }      
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, 0, 1, 1, ui->text);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      mui_SaveForm(ui);          // store the current form and position so that the child can jump back
      mui_GotoForm(ui, ui->arg, *value);  // assumes that the selectable values are at the top of the form
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}


/*

  uint8_t mui_checkbox_invers_select_u8g2(mui_t *ui, uint8_t msg)
  
  Description:
    Checkbox with the values 0 (not selected) and 1 (selected). 

  Message Handling: DRAW, SELECT

  Style
    No Selection: Plain checkbox and text
    Cursor Selection: Checkbox and text is inverted

  User interface field list (muif):
    flags: MUIF_CFLAG_IS_CURSOR_SELECTABLE
    data: uint8_t *, pointer to a uint8_t variable, which contains the values 0 or 1

  Field definition string (fds):
    xy: Left position of the text (required)
    arg: not used
    text: Optional: Text will be printed after the checkbox with a small gap
    
*/

uint8_t mui_checkbox_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  //u8g2_t *u8g2 = mui_get_U8g2(ui);
  //u8g2_uint_t flags = 0;
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      return mui_checkbox_mark_invers_select_u8g2(ui, msg);
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      (*value)++;
      if ( *value > 1 ) *value = 0;      
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}

/*
  radio button style, arg is assigned as value
*/
uint8_t mui_radio_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      return mui_radio_checkbox_mark_invers_select_u8g2(ui, msg);
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      *value = ui->arg;
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;  
}

#ifdef OBSOLETE
uint8_t mui_assign_arg_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  //if ( value == NULL )
  //  value = &(ui->selected_value);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      return mui_radio_mark_invers_select_u8g2(ui, msg);
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      *value = ui->arg;
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}
#endif

uint8_t mui_select_options_child_invers_select_u8g2(mui_t *ui, uint8_t msg)
{
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  //if ( value == NULL )
  //  value = &(ui->selected_value);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      return mui_radio_mark_invers_select_u8g2(ui, msg);
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      *value = ui->arg;
      mui_RestoreForm(ui);
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
  }
  return 0;
}
