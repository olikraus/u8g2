/*

  mui_u8g2.c

  Monochrome minimal user interface: Glue code between mui and u8g2.

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2021, olikraus@gmail.com
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

/*

  field function naming convention

    action
      draw_text:                        (rename from draw label)
      draw_str:                      
      btn_jmp   button jump to:                      a button which jumps to a specific form
      btn_exit          button leave:                     a button which leaves the form and places an exit code into a uint8 variable
      u8_value_0_9      
      u8_chkbox
      u8_radio
      u8_opt_line       edit value options in the same line
      u8_opt_parent       edit value options parent
      u8_opt_child       edit value options child
    
    
    field width (not for draw text/str)
      wm                minimum width
      wa                width can be provided via FDS argument
      w1                full display width
      w2                half display size (minus some pixel)
      w3                one/third of the dispay width (minus some pixel)

    edit mode  (not for draw text/str, buttons and checkbox)                  
      mse       select: select event will increment the value or activate the field (buttons)
      mud      up/down:  select will enter the up/down edit mode. Next/prev event will increment/decrement the value
      
    styles (not for draw text/str)
      unselected                selected                        up/down edit
      plain                          invers                             invers + gap + frame            pi
      frame                         invers+frame        ?                                                       fi
      
      plain                          frame                              invers + frame                         pf
      
    mui_u8g2_[action]_[field_width]_[edit_mode]_[style]

  mui _label_u8g2 --> mui_u8g2_draw_text
  mui _goto_frame_button_invers_select_u8g2                              --> mui_u8g2_btn_jmp_wm_fi
  mui _goto_half_width_frame_button_invers_select_u8g2           --> mui_u8g2_btn_jmp_w2_fi
  mui _goto_line_button_invers_select_u8g2 -->  mui_u8g2_btn_jmp_w1_fi
  mui _leave_menu_frame_button_invers_select_u8g2 --> mui_u8g2_btn_exit_wm_fi
  
  mui _input_uint8_invers_select_u8g2 --> mui_u8g2_u8_value_0_9_wm_mse_pi
  mui _single_line_option_invers_select_u8g2     --> mui_u8g2_u8_opt_line_wa_mse_pi
  mui _select_options_parent_invers_select_u8g2  --> mui_u8g2_u8_opt_parent_wa_mse_pi
  mui _select_options_child_invers_select_u8g2  --> mui_u8g2_u8_opt_child_wm_mse_pi

  mui _checkbox_invers_select_u8g2 --> mui_u8g2_u8_chkbox_wm_pi
  mui _radio_invers_select_u8g2 --> mui_u8g2_u8_radio_wm_pi

  mui _input_char_invers_select_u8g2 --> mui_u8g2_u8_char_wm_mud_pi

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
#define MUI_U8G2_V_PADDING 1

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

u8g2_uint_t mui_u8g2_get_draw_button_pi_flags(mui_t *ui)
{
  u8g2_uint_t flags = 0;
  if ( mui_IsCursorFocus(ui) )
  {
    flags |= U8G2_BTN_INV;
    if ( ui->is_mud )
    {
      flags |= U8G2_BTN_XFRAME;
    }      
  }
  return flags;
}

//void u8g2_DrawButtonUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text);
void mui_u8g2_draw_button_utf(mui_t *ui, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text)
{
  if ( text==NULL)
    text = "";
  u8g2_DrawButtonUTF8(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), flags, width, padding_h, padding_v, text);
}



/*=========================================================================*/
/* incomplete (draw only) field functions */



/*

  uint8_t mui_u8g2_btn_draw_wm_fi(mui_t *ui, uint8_t msg)

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

uint8_t mui_u8g2_btn_draw_wm_fi(mui_t *ui, uint8_t msg)
{
  //u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = U8G2_BTN_HCENTER | 1;
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      mui_u8g2_draw_button_utf(ui, flags, 0, 1, MUI_U8G2_V_PADDING, ui->text);
      //u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, 0, 1, MUI_U8G2_V_PADDING, ui->text);
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

  uint8_t mui_u8g2_btn_draw_w2_fi(mui_t *ui, uint8_t msg)

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

uint8_t mui_u8g2_btn_draw_w2_fi(mui_t *ui, uint8_t msg)
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
      mui_u8g2_draw_button_utf(ui, flags, u8g2_GetDisplayWidth(u8g2)/2 - 10, 0, MUI_U8G2_V_PADDING, ui->text);
      // u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, u8g2_GetDisplayWidth(u8g2)/2 - 10, 0, MUI_U8G2_V_PADDING, ui->text);
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

  uint8_t mui_u8g2_btn_draw_w1_pi(mui_t *ui, uint8_t msg)

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
uint8_t mui_u8g2_btn_draw_w1_pi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  u8g2_uint_t flags = 0;
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_IsCursorFocus(ui) )
      {
        flags |= U8G2_BTN_INV;
      }
      //mui_u8g2_draw_button_utf(ui, flags, u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui) , MUI_U8G2_V_PADDING, ui->text);
      u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), flags, u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui) , MUI_U8G2_V_PADDING, ui->text);
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

uint8_t mui_u8g2_draw_text(mui_t *ui, uint8_t msg)
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

  uint8_t mui_u8g2_btn_jmp_wm_fi(mui_t *ui, uint8_t msg)

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
uint8_t mui_u8g2_btn_jmp_wm_fi(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
      return mui_GotoForm(ui, ui->arg, 0);
  }
  return mui_u8g2_btn_draw_wm_fi(ui, msg);
}

/*

  uint8_t mui_u8g2_btn_jmp_w2_fi(mui_t *ui, uint8_t msg)

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
uint8_t mui_u8g2_btn_jmp_w2_fi(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
      return mui_GotoForm(ui, ui->arg, 0);
  }
  return mui_u8g2_btn_draw_w2_fi(ui, msg);
}

/*

  uint8_t mui_u8g2_btn_exit_wm_fi(mui_t *ui, uint8_t msg)

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
uint8_t mui_u8g2_btn_exit_wm_fi(mui_t *ui, uint8_t msg)
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
  return mui_u8g2_btn_draw_wm_fi(ui, msg);
}


/*

  uint8_t mui_u8g2_btn_jmp_w1_pi(mui_t *ui, uint8_t msg)

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

uint8_t mui_u8g2_btn_jmp_w1_pi(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_CURSOR_SELECT:
      return mui_GotoForm(ui, ui->arg, 0);
  }
  return mui_u8g2_btn_draw_w1_pi(ui, msg);
}

/*===============================================================================*/


static void mui_u8g2_u8_vmm_draw_wm_pi(mui_t *ui) MUI_NOINLINE;
static void mui_u8g2_u8_vmm_draw_wm_pi(mui_t *ui)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  mui_u8g2_u8_min_max_t *vmm= (mui_u8g2_u8_min_max_t *)muif_get_data(ui->uif);
  char *s = "999";
  uint8_t *value = mui_u8g2_u8mm_get_valptr(vmm);
  uint8_t min = mui_u8g2_u8mm_get_min(vmm);
  uint8_t max = mui_u8g2_u8mm_get_max(vmm);
  uint8_t cnt = 3;
  
  if ( *value > max ) 
    *value = max;
  if ( *value <= min )
    *value = min;
  if ( max < 100 )
  {
    s++;
    cnt--;
  }
  if ( max < 10 )
  {
    s++;
    cnt--;
  }
  mui_u8g2_draw_button_utf(ui, mui_u8g2_get_draw_button_pi_flags(ui), u8g2_GetStrWidth(u8g2, s)+1, 1, MUI_U8G2_V_PADDING, u8x8_u8toa(*value, cnt));
}


uint8_t mui_u8g2_u8_min_max_wm_mse_pi(mui_t *ui, uint8_t msg)
{
  mui_u8g2_u8_min_max_t *vmm= (mui_u8g2_u8_min_max_t *)muif_get_data(ui->uif);
  uint8_t *value = mui_u8g2_u8mm_get_valptr(vmm);
  uint8_t min = mui_u8g2_u8mm_get_min(vmm);
  uint8_t max = mui_u8g2_u8mm_get_max(vmm);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_vmm_draw_wm_pi(ui);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      (*value)++;
      if ( *value > max ) *value = min;
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

uint8_t mui_u8g2_u8_min_max_wm_mud_pi(mui_t *ui, uint8_t msg)
{
  mui_u8g2_u8_min_max_t *vmm= (mui_u8g2_u8_min_max_t *)muif_get_data(ui->uif);
  uint8_t *value = mui_u8g2_u8mm_get_valptr(vmm);
  uint8_t min = mui_u8g2_u8mm_get_min(vmm);
  uint8_t max = mui_u8g2_u8mm_get_max(vmm);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_vmm_draw_wm_pi(ui);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      /* toggle between normal mode and capture next/prev mode */
      ui->is_mud = !ui->is_mud;
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
    case MUIF_MSG_EVENT_NEXT:
      if ( ui->is_mud )
      {
        (*value)++;
        if ( *value > max )
          *value = min;
        return 1;
      }
      break;
    case MUIF_MSG_EVENT_PREV:
      if ( ui->is_mud )
      {
        if ( *value <= min )
          *value = max;
        else
          (*value)--;
        return 1;
      }
      break;
  }
  return 0;
}


/*===============================================================================*/

static uint8_t mui_is_valid_char(uint8_t c) MUI_NOINLINE;
uint8_t mui_is_valid_char(uint8_t c)
{
  if ( c == 32 )
    return 1;
  if ( c >= 'A' && c <= 'Z' )
    return 1;
  if ( c >= 'a' && c <= 'z' )
    return 1;
  if ( c >= '0' && c <= '9' )
    return 1;
  return 0;
}



uint8_t mui_u8g2_u8_char_wm_mud_pi(mui_t *ui, uint8_t msg)
{
  //ui->dflags                          MUIF_DFLAG_IS_CURSOR_FOCUS       MUIF_DFLAG_IS_TOUCH_FOCUS
  //mui_get_cflags(ui->uif)       MUIF_CFLAG_IS_CURSOR_SELECTABLE
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  char buf[6];
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      while( mui_is_valid_char(*value) == 0 )
          (*value)++;
      buf[0] = *value;
      buf[1] = '\0';
      mui_u8g2_draw_button_utf(ui, mui_u8g2_get_draw_button_pi_flags(ui), u8g2_GetMaxCharWidth(u8g2), 1, MUI_U8G2_V_PADDING, buf);
      //u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), mui_u8g2_get_draw_button_pi_flags(ui), u8g2_GetMaxCharWidth(u8g2), 1, MUI_U8G2_V_PADDING, buf);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      /* toggle between normal mode and capture next/prev mode */
       ui->is_mud = !ui->is_mud;
      /*
      if ( ui->is_mud )
        ui->is_mud = 0;
      else
        ui->is_mud = 1;
      */
     break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
    case MUIF_MSG_EVENT_NEXT:
      if ( ui->is_mud )
      {
        do {
          (*value)++;
        } while( mui_is_valid_char(*value) == 0 );
        return 1;
      }
      break;
    case MUIF_MSG_EVENT_PREV:
      if ( ui->is_mud )
      {
        do {
          (*value)--;
        } while( mui_is_valid_char(*value) == 0 );
        return 1;
      }
      break;
  }
  return 0;
}





/*

  uint8_t mui_u8g2_u8_opt_line_wa_mse_pi(mui_t *ui, uint8_t msg)

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
uint8_t mui_u8g2_u8_opt_line_wa_mse_pi(mui_t *ui, uint8_t msg)
{
  //u8g2_t *u8g2 = mui_get_U8g2(ui);
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_fds_get_nth_token(ui, *value) == 0 )
      {
        *value = 0;
        mui_fds_get_nth_token(ui, *value);
      }
      mui_u8g2_draw_button_utf(ui, mui_u8g2_get_draw_button_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, ui->text);
      //u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), mui_u8g2_get_draw_button_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, ui->text);
      
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
   
uint8_t mui_u8g2_u8_opt_line_wa_mud_pi(mui_t *ui, uint8_t msg)
{
  //u8g2_t *u8g2 = mui_get_U8g2(ui);
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_fds_get_nth_token(ui, *value) == 0 )
      {
        *value = 0;
        mui_fds_get_nth_token(ui, *value);
      }
      mui_u8g2_draw_button_utf(ui, mui_u8g2_get_draw_button_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, ui->text);
      //u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), mui_u8g2_get_draw_button_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, ui->text);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      /* toggle between normal mode and capture next/prev mode */
       ui->is_mud = !ui->is_mud;
     break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
    case MUIF_MSG_EVENT_NEXT:
      if ( ui->is_mud )
      {
        (*value)++;
        if ( mui_fds_get_nth_token(ui, *value) == 0 ) 
          *value = 0;      
        return 1;
      }
      break;
    case MUIF_MSG_EVENT_PREV:
      if ( ui->is_mud )
      {
        if ( *value == 0 )
          *value = mui_fds_get_token_cnt(ui);
        (*value)--;
        return 1;
      }
      break;
  }
  return 0;
}




/*

  uint8_t mui_u8g2_u8_chkbox_wm_pi(mui_t *ui, uint8_t msg)
  
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

uint8_t mui_u8g2_u8_chkbox_wm_pi(mui_t *ui, uint8_t msg)
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
uint8_t mui_u8g2_u8_radio_wm_pi(mui_t *ui, uint8_t msg)
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


uint8_t mui_u8g2_u8_opt_parent_wm_mse_pi(mui_t *ui, uint8_t msg)
{
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_fds_get_nth_token(ui, *value) == 0 )
      {
        *value = 0;
        mui_fds_get_nth_token(ui, *value);
      }      
      mui_u8g2_draw_button_utf(ui, mui_u8g2_get_draw_button_pi_flags(ui), 0, 1, MUI_U8G2_V_PADDING, ui->text);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      mui_SaveForm(ui);          // store the current form and position so that the child can jump back
      mui_GotoForm(ui, ui->arg, *value);  // assumes that the selectable values are at the beginning of the form definition
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

uint8_t mui_u8g2_u8_opt_child_mse_common(mui_t *ui, uint8_t msg)
{
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  uint8_t arg = ui->arg;        // remember the arg value, because it might be overwritten
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      /* done by the calling function */
      break;
    case MUIF_MSG_FORM_START:
      /* we can assume that the list starts at the top. It will be adjisted by cursor down events later */
      /* ui->form_scroll_top = 0 and all other form_scroll values are set to 0 if a new form is entered in mui_EnterForm() */
      if ( ui->form_scroll_visible <= arg )
        ui->form_scroll_visible = arg+1;
      if ( ui->form_scroll_total == 0 )
          ui->form_scroll_total = mui_GetSelectableFieldOptionCnt(ui, ui->last_form_id, ui->last_form_cursor_focus_position);
      //printf("MUIF_MSG_FORM_START: arg=%d visible=%d top=%d total=%d\n", arg, ui->form_scroll_visible, ui->form_scroll_top, ui->form_scroll_total);
      break;
    case MUIF_MSG_FORM_END:  
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      *value = ui->form_scroll_top + arg;
      mui_RestoreForm(ui);
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
    case MUIF_MSG_EVENT_NEXT:
      //printf("MUIF_MSG_EVENT_NEXT: arg=%d visible=%d top=%d total=%d\n", arg, ui->form_scroll_visible, ui->form_scroll_top, ui->form_scroll_total);
      if ( arg+1 == ui->form_scroll_visible )
      {
        if ( ui->form_scroll_visible + ui->form_scroll_top < ui->form_scroll_total )
        {
          ui->form_scroll_top++;
          return 1;
        }
        else
        {
          ui->form_scroll_top = 0;
        }
      }
      break;
    case MUIF_MSG_EVENT_PREV:
      if ( arg == 0 )
      {
        if ( ui->form_scroll_top > 0 )
        {
          ui->form_scroll_top--;
          return 1;
        }
        else
        {
          ui->form_scroll_top = ui->form_scroll_total - ui->form_scroll_visible;
        }
      }
      break;
  }
  return 0;
}


uint8_t mui_u8g2_u8_opt_radio_child_wm_mse_pi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  uint8_t arg = ui->arg;        // remember the arg value, because it might be overwritten
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      {
        u8g2_uint_t w = 0;
        u8g2_uint_t a = u8g2_GetAscent(u8g2) - 2;
        u8g2_uint_t x = mui_get_x(ui);   // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        u8g2_uint_t y = mui_get_y(ui);  // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        uint8_t is_focus = mui_IsCursorFocus(ui);
        if ( *value == arg + ui->form_scroll_top )
          u8g2_DrawValueMark(u8g2, x, y, a);

        if ( ui->text[0] == '\0' )
        {
          /* if the text is not provided, then try to get the text from the previous (saved) element, assuming that this contains the selection */
          /* this will overwrite all ui member functions, so we must not access any ui members (except ui->text) any more */
          mui_GetSelectableFieldTextOption(ui, ui->last_form_id, ui->last_form_cursor_focus_position, arg + ui->form_scroll_top);
        }
        
        if ( ui->text[0] != '\0' )
        {
          w =  u8g2_GetUTF8Width(u8g2, ui->text);
          u8g2_SetFontMode(u8g2, 1);
          a += 2;       /* add gap between the checkbox and the text area */
          u8g2_DrawUTF8(u8g2, x+a, y, ui->text);
        }        
        if ( is_focus )
        {
          u8g2_DrawButtonFrame(u8g2, x, y, U8G2_BTN_INV, w+a, 1, 1);
        }
      }
      break;
    default:
      return mui_u8g2_u8_opt_child_mse_common(ui, msg);
  }
  return 0;
}


uint8_t mui_u8g2_u8_opt_radio_child_w1_mse_pi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  uint8_t arg = ui->arg;        // remember the arg value, because it might be overwritten
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      {
        //u8g2_uint_t w = 0;
        u8g2_uint_t a = u8g2_GetAscent(u8g2) - 2;
        u8g2_uint_t x = mui_get_x(ui);   // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        u8g2_uint_t y = mui_get_y(ui);  // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        uint8_t is_focus = mui_IsCursorFocus(ui);
        
        if ( *value == arg + ui->form_scroll_top )
          u8g2_DrawValueMark(u8g2, x, y, a);

        if ( ui->text[0] == '\0' )
        {
          /* if the text is not provided, then try to get the text from the previous (saved) element, assuming that this contains the selection */
          /* this will overwrite all ui member functions, so we must not access any ui members (except ui->text) any more */
          mui_GetSelectableFieldTextOption(ui, ui->last_form_id, ui->last_form_cursor_focus_position, arg + ui->form_scroll_top);
        }
        
        if ( ui->text[0] != '\0' )
        {
          //w =  u8g2_GetUTF8Width(u8g2, ui->text);
          u8g2_SetFontMode(u8g2, 1);
          a += 2;       /* add gap between the checkbox and the text area */
          u8g2_DrawUTF8(u8g2, x+a, y, ui->text);
        }        
        if ( is_focus )
        {
          u8g2_DrawButtonFrame(u8g2, 0, y, U8G2_BTN_INV, u8g2_GetDisplayWidth(u8g2), 0, 1);
        }
      }
      break;
    default:
      return mui_u8g2_u8_opt_child_mse_common(ui, msg);
  }
  return 0;
}


uint8_t mui_u8g2_u8_opt_child_wm_mse_pi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  //uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
  uint8_t arg = ui->arg;        // remember the arg value, because it might be overwritten
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      {
        //u8g2_uint_t w = 0;
        u8g2_uint_t x = mui_get_x(ui);   // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        u8g2_uint_t y = mui_get_y(ui);  // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        uint8_t flags = mui_u8g2_get_draw_button_pi_flags(ui);
        //if ( mui_IsCursorFocus(ui) )
        //{
        //  flags = U8G2_BTN_INV;
        //}

        if ( ui->text[0] == '\0' )
        {
          /* if the text is not provided, then try to get the text from the previous (saved) element, assuming that this contains the selection */
          /* this will overwrite all ui member functions, so we must not access any ui members (except ui->text) any more */
          mui_GetSelectableFieldTextOption(ui, ui->last_form_id, ui->last_form_cursor_focus_position, arg + ui->form_scroll_top);
        }
        if ( ui->text[0] != '\0' )
        {
          u8g2_DrawButtonUTF8(u8g2, x, y, flags, 0, 1, MUI_U8G2_V_PADDING, ui->text);
        }        
      }
      break;
    default:
      return mui_u8g2_u8_opt_child_mse_common(ui, msg);
  }
  return 0;
}


/*
  data: mui_u8g2_list_t *
*/
uint8_t mui_u8g2_u16_list_line_wa_mse_pi(mui_t *ui, uint8_t msg)
{
  //u8g2_t *u8g2 = mui_get_U8g2(ui);
  mui_u8g2_list_t *list = (mui_u8g2_list_t *)muif_get_data(ui->uif);
  uint16_t *selection =  mui_u8g2_list_get_selection_ptr(list);
  void *data = mui_u8g2_list_get_data_ptr(list);
  mui_u8g2_get_list_element_cb element_cb =  mui_u8g2_list_get_element_cb(list);
  mui_u8g2_get_list_count_cb count_cb = mui_u8g2_list_get_count_cb(list);
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, mui_u8g2_get_draw_button_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, element_cb(data, *selection));
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      (*selection)++;
      if ( *selection >= count_cb(data) ) 
        *selection = 0;
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

uint8_t mui_u8g2_u16_list_line_wa_mud_pi(mui_t *ui, uint8_t msg)
{
  //u8g2_t *u8g2 = mui_get_U8g2(ui);
  mui_u8g2_list_t *list = (mui_u8g2_list_t *)muif_get_data(ui->uif);
  uint16_t *selection =  mui_u8g2_list_get_selection_ptr(list);
  void *data = mui_u8g2_list_get_data_ptr(list);
  mui_u8g2_get_list_element_cb element_cb =  mui_u8g2_list_get_element_cb(list);
  mui_u8g2_get_list_count_cb count_cb = mui_u8g2_list_get_count_cb(list);
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, mui_u8g2_get_draw_button_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, element_cb(data, *selection));
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      /* toggle between normal mode and capture next/prev mode */
       ui->is_mud = !ui->is_mud;
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
    case MUIF_MSG_EVENT_NEXT:
      if ( ui->is_mud )
      {
        (*selection)++;
        if ( *selection >= count_cb(data)  ) 
          *selection = 0;      
        return 1;
      }
      break;
    case MUIF_MSG_EVENT_PREV:
      if ( ui->is_mud )
      {
        if ( *selection == 0 )
          *selection = count_cb(data);
        (*selection)--;
        return 1;
      }
      break;
  }
  return 0;
}

/*
  MUIF: MUIF_U8G2_U16_LIST
  FDS: MUI_XYA, arg=form id
  data: mui_u8g2_list_t *
*/
uint8_t mui_u8g2_u16_list_parent_wm_mse_pi(mui_t *ui, uint8_t msg)
{
  //u8g2_t *u8g2 = mui_get_U8g2(ui);
  mui_u8g2_list_t *list = (mui_u8g2_list_t *)muif_get_data(ui->uif);
  uint16_t *selection =  mui_u8g2_list_get_selection_ptr(list);
  void *data = mui_u8g2_list_get_data_ptr(list);
  mui_u8g2_get_list_element_cb element_cb =  mui_u8g2_list_get_element_cb(list);
  //mui_u8g2_get_list_count_cb count_cb = mui_u8g2_list_get_count_cb(list);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, mui_u8g2_get_draw_button_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, element_cb(data, *selection));
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      mui_SaveForm(ui);          // store the current form and position so that the child can jump back
      mui_GotoForm(ui, ui->arg, *selection);  // assumes that the selectable values are at the beginning of the form definition
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

static uint8_t mui_u8g2_u16_list_child_mse_common(mui_t *ui, uint8_t msg)
{
  mui_u8g2_list_t *list = (mui_u8g2_list_t *)muif_get_data(ui->uif);
  uint16_t *selection =  mui_u8g2_list_get_selection_ptr(list);
  void *data = mui_u8g2_list_get_data_ptr(list);
  //mui_u8g2_get_list_element_cb element_cb =  mui_u8g2_list_get_element_cb(list);
  mui_u8g2_get_list_count_cb count_cb = mui_u8g2_list_get_count_cb(list);

  uint8_t arg = ui->arg;        // remember the arg value, because it might be overwritten  
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      /* done by the calling function */
      break;
    case MUIF_MSG_FORM_START:
      /* we can assume that the list starts at the top. It will be adjisted by cursor down events later */
      ui->form_scroll_top = 0;
      if ( ui->form_scroll_visible <= arg )
        ui->form_scroll_visible = arg+1;
      if ( ui->form_scroll_total == 0 )
          ui->form_scroll_total = count_cb(data);
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      *selection = ui->form_scroll_top + arg;
      mui_RestoreForm(ui);
      break;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;
    case MUIF_MSG_EVENT_NEXT:
      if ( arg+1 == ui->form_scroll_visible )
      {
        if ( ui->form_scroll_visible + ui->form_scroll_top < ui->form_scroll_total )
        {
          ui->form_scroll_top++;
          return 1;
        }
        else
        {
          ui->form_scroll_top = 0;
        }
      }
      break;
    case MUIF_MSG_EVENT_PREV:
      if ( arg == 0 )
      {
        if ( ui->form_scroll_top > 0 )
        {
          ui->form_scroll_top--;
          return 1;
        }
        else
        {
          ui->form_scroll_top = ui->form_scroll_total - ui->form_scroll_visible;
        }
      }
      break;
  }
  return 0;
}

uint8_t mui_u8g2_u16_list_child_w1_mse_pi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  mui_u8g2_list_t *list = (mui_u8g2_list_t *)muif_get_data(ui->uif);
  uint16_t *selection =  mui_u8g2_list_get_selection_ptr(list);
  void *data = mui_u8g2_list_get_data_ptr(list);
  mui_u8g2_get_list_element_cb element_cb =  mui_u8g2_list_get_element_cb(list);
  mui_u8g2_get_list_count_cb count_cb = mui_u8g2_list_get_count_cb(list);

  uint16_t pos = ui->arg;        // remember the arg value, because it might be overwritten  
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      {
        //u8g2_uint_t w = 0;
        u8g2_uint_t a = u8g2_GetAscent(u8g2) - 2;
        u8g2_uint_t x = mui_get_x(ui);   // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        u8g2_uint_t y = mui_get_y(ui);  // if mui_GetSelectableFieldTextOption is called, then field vars are overwritten, so get the value
        uint8_t is_focus = mui_IsCursorFocus(ui);

        pos += ui->form_scroll_top;
        
        if ( *selection == pos )
          u8g2_DrawValueMark(u8g2, x, y, a);

        u8g2_SetFontMode(u8g2, 1);
        a += 2;       /* add gap between the checkbox and the text area */
        if ( pos < count_cb(data) )
          u8g2_DrawUTF8(u8g2, x+a, y, element_cb(data, pos));
        if ( is_focus )
        {
          u8g2_DrawButtonFrame(u8g2, 0, y, U8G2_BTN_INV, u8g2_GetDisplayWidth(u8g2), 0, 1);
        }
      }
      break;
    default:
      return mui_u8g2_u16_list_child_mse_common(ui, msg);
  }
  return 0;
}

uint8_t mui_u8g2_u16_list_goto_w1_mse_pi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  mui_u8g2_list_t *list = (mui_u8g2_list_t *)muif_get_data(ui->uif);
  uint16_t *selection =  mui_u8g2_list_get_selection_ptr(list);
  void *data = mui_u8g2_list_get_data_ptr(list);
  mui_u8g2_get_list_element_cb element_cb =  mui_u8g2_list_get_element_cb(list);
  //mui_u8g2_get_list_count_cb count_cb = mui_u8g2_list_get_count_cb(list);

  uint16_t pos = ui->arg;        // remember the arg value, because it might be overwritten  
  pos += ui->form_scroll_top;
  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, mui_u8g2_get_draw_button_pi_flags(ui), u8g2_GetDisplayWidth(u8g2), 1, MUI_U8G2_V_PADDING, element_cb(data, pos)+1);
      break;
    case MUIF_MSG_CURSOR_SELECT:
      if ( selection != NULL )
        *selection = pos;
      mui_GotoForm(ui, (uint8_t)element_cb(data, pos)[0], 0); 
      break;
    default:
      return mui_u8g2_u16_list_child_mse_common(ui, msg);
  }
  return 0;
}
