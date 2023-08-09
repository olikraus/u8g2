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
      unselected                selected                        up/down edit                            postfix         Use for
      plain                          invers                             invers + gap + frame            pi                      input elements
      frame                         invers+frame                frame                                       fi                  buttons
      
      plain                          frame                              invers + frame                         pf               input elements
      invers                        frame                               invers + frame                          if              buttons
      
      
    mui_u8g2_[action]_[field_width]_[edit_mode]_[style]

  mui _label_u8g2 --> mui_u8g2_draw_text
  mui _goto_frame_button_invers_select_u8g2                              --> mui_u8g2_btn_goto_wm_fi
  mui _goto_half_width_frame_button_invers_select_u8g2           --> mui_u8g2_btn_goto_w2_fi
  mui _goto_line_button_invers_select_u8g2 -->  mui_u8g2_btn_goto_w1_fi
  mui _leave_menu_frame_button_invers_select_u8g2 --> mui_u8g2_btn_exit_wm_fi
  
  mui _input_uint8_invers_select_u8g2 --> mui_u8g2_u8_value_0_9_wm_mse_pi
  mui _single_line_option_invers_select_u8g2     --> mui_u8g2_u8_opt_line_wa_mse_pi
  mui _select_options_parent_invers_select_u8g2  --> mui_u8g2_u8_opt_parent_wa_mse_pi
  mui _select_options_child_invers_select_u8g2  --> mui_u8g2_u8_opt_child_wm_pi

  mui _checkbox_invers_select_u8g2 --> mui_u8g2_u8_chkbox_wm_pi
  mui _radio_invers_select_u8g2 --> mui_u8g2_u8_radio_wm_pi

  mui _input_char_invers_select_u8g2 --> mui_u8g2_u8_char_wm_mud_pi



  2 Buttons
    Only use "mse", don't use "mud"
  
    Button      Call                            Description
    1                mui_SendSelect()    Activate elements & change values
    2                mui_NextField()      Goto next field
    
  3 Buttons
    Use "mse" or "mud"
    Button      Call                            Description
    1                mui_SendSelect()    Activate elements / change values (mse) / enter "mud" mode (mud)
    2                mui_NextField()      Goto next field, increment value (mud)
    3                mui_PrevField()      Goto prev field, decrement value (mud)
    
  4 Buttons
    Prefer "mse"
    Button      Call                                            Description
    1                mui_SendValueIncrement()    Activate elements / increment values (mse)
    2                mui_SendValueDecrement()   Activate elements / decrement values (mse)
    3                mui_NextField()                       Goto next field
    4                mui_PrevField()                        Goto prev field

  5 Buttons
    Prefer "mse", use the MUIF_EXECUTE_ON_SELECT_BUTTON on forms to finish the form with the "form select" button 5
    Button      Call                                                                                            Description
    1                mui_SendValueIncrement()                                                           Activate elements / increment values (mse)
    2                mui_SendValueDecrement()                                                         Activate elements / decrement values (mse)
    3                mui_NextField()                                                                            Goto next field
    4                mui_PrevField()                                                                     Goto prev field
    5                mui_SendSelectWithExecuteOnSelectFieldSearch()             Execute the MUIF_EXECUTE_ON_SELECT_BUTTON button or activate the current element if there is no EOS button
    
  rotary encoder, push&release
    Prefer "mud"
    Button      Call                            Description
    encoder button                 mui_SendSelect()    Activate elements / change values (mse) / enter "mud" mode (mud)
    encoder CW                      mui_NextField()      Goto next field, increment value (mud)
    encoder CCW                    mui_PrevField()      Goto prev field, decrement value (mud)
  
  rotary encoder, push&rotate
    Prefer "mse"
    Button                                      Call                                            Description
    encoder CW                                  mui_SendValueIncrement()    Activate elements / increment values (mse)
    encoder CCW                                 mui_SendValueDecrement()   Activate elements / decrement values (mse)
    encoder CW+button press                mui_NextField()                       Goto next field
    encoder CCW+button press                mui_PrevField()                        Goto prev field


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
    case MUIF_MSG_VALUE_INCREMENT:
      break;
    case MUIF_MSG_VALUE_DECREMENT:
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

static void u8g2_DrawCheckbox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t is_checked) MUI_NOINLINE;
static void u8g2_DrawCheckbox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t is_checked)
{
  u8g2_DrawFrame(u8g2, x, y-w, w, w);
  if ( is_checked )
  {
    w-=4;
    u8g2_DrawBox(u8g2, x+2, y-w-2, w, w);
  }
}

static void u8g2_DrawValueMark(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w)
{
  u8g2_DrawBox(u8g2, x, y-w, w, w);
}


/*=========================================================================*/
/* helper function */

u8g2_uint_t mui_get_x(mui_t *ui) MUI_NOINLINE;
u8g2_uint_t mui_get_x(mui_t *ui)
{
  if ( u8g2_GetDisplayWidth(mui_get_U8g2(ui)) >= 255 )
      return ui->x * 2;
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

//void u8g2_DrawButtonUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text);
void mui_u8g2_draw_button_utf(mui_t *ui, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text)
{
  if ( text==NULL)
    text = "";
  u8g2_DrawButtonUTF8(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), flags, width, padding_h, padding_v, text);
}

u8g2_uint_t mui_u8g2_get_pi_flags(mui_t *ui)
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


void mui_u8g2_draw_button_pi(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text)
{
  mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), width, padding_h , MUI_U8G2_V_PADDING, text);
}


u8g2_uint_t mui_u8g2_get_fi_flags(mui_t *ui)
{
  u8g2_uint_t flags = 1;
  if ( mui_IsCursorFocus(ui) )
  {
    flags |= U8G2_BTN_INV;
    if ( ui->is_mud )
    {
      flags = 1;        // undo INV
    }      
  }
  return flags;
}

void mui_u8g2_draw_button_fi(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text)
{
  mui_u8g2_draw_button_utf(ui, mui_u8g2_get_fi_flags(ui), width, padding_h , MUI_U8G2_V_PADDING, text);
}


u8g2_uint_t mui_u8g2_get_pf_flags(mui_t *ui)
{
  u8g2_uint_t flags = 0;
  if ( mui_IsCursorFocus(ui) )
  {
    flags |= 1;
    if ( ui->is_mud )
    {
      flags |= U8G2_BTN_INV;
    }      
  }
  return flags;
}

void mui_u8g2_draw_button_pf(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text)
{
  mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pf_flags(ui), width, padding_h , MUI_U8G2_V_PADDING, text);
}


u8g2_uint_t mui_u8g2_get_if_flags(mui_t *ui)
{
  u8g2_uint_t flags = 0;
  if ( mui_IsCursorFocus(ui) )
  {
    if ( ui->is_mud )
    {
      flags |= 1;
      flags |= U8G2_BTN_INV;
    }
    else
    {
      flags |= 1;
    }
  }
  else
  {
      flags |= U8G2_BTN_INV;
  }
  return flags;
}

void mui_u8g2_draw_button_if(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text)
{
  mui_u8g2_draw_button_utf(ui, mui_u8g2_get_if_flags(ui), width, padding_h , MUI_U8G2_V_PADDING, text);
}


static uint8_t mui_u8g2_handle_scroll_next_prev_events(mui_t *ui, uint8_t msg) MUI_NOINLINE;
static uint8_t mui_u8g2_handle_scroll_next_prev_events(mui_t *ui, uint8_t msg)
{
  uint8_t arg = ui->arg;
  switch(msg)
  {
    case MUIF_MSG_CURSOR_ENTER:
      if ( (arg > 0) && (ui->form_scroll_top + arg >= ui->form_scroll_total) )
        return 255;
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
          if ( ui->form_scroll_total >  ui->form_scroll_visible  )
          {
            ui->form_scroll_top = ui->form_scroll_total - ui->form_scroll_visible;
          }
          else
          {
            ui->form_scroll_top = 0;
          }
        }
      }
      break;
  }
  return 0;
}

/*=========================================================================*/
/* simplified style function  */

/*
Used for MUIF_U8G2_FONT_STYLE(n,font)
*/

uint8_t mui_u8g2_set_font_style_function(mui_t *ui, uint8_t msg)
{  
  if ( msg == MUIF_MSG_DRAW )
  {
    u8g2_SetFont(mui_get_U8g2(ui), (uint8_t *)muif_get_data(ui->uif));
  }
  return 0;
}



/*=========================================================================*/
/* field functions */

/*
  xy: yes, arg: no, text: yes
*/

uint8_t mui_u8g2_draw_text(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_DrawUTF8(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
      break;
    case MUIF_MSG_VALUE_INCREMENT:
      break;
    case MUIF_MSG_VALUE_DECREMENT:
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

  uint8_t mui_u8g2_btn_goto_wm_fi(mui_t *ui, uint8_t msg)

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
uint8_t mui_u8g2_btn_goto_wm_fi(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, U8G2_BTN_HCENTER |mui_u8g2_get_fi_flags(ui), 0, 1, MUI_U8G2_V_PADDING, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      //return mui_GotoForm(ui, ui->arg, 0);
      return mui_GotoFormAutoCursorPosition(ui, ui->arg);
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
    
  }
  return 0;
}

uint8_t mui_u8g2_btn_goto_wm_if(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, U8G2_BTN_HCENTER |mui_u8g2_get_if_flags(ui), 0, 1, MUI_U8G2_V_PADDING, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      //return mui_GotoForm(ui, ui->arg, 0);
      return mui_GotoFormAutoCursorPosition(ui, ui->arg);
   case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
    
  }
  return 0;
}

uint8_t mui_u8g2_btn_goto_w2_fi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, U8G2_BTN_HCENTER | mui_u8g2_get_fi_flags(ui), u8g2_GetDisplayWidth(u8g2)/2 - 10, 0, MUI_U8G2_V_PADDING, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      //return mui_GotoForm(ui, ui->arg, 0);
      return mui_GotoFormAutoCursorPosition(ui, ui->arg);
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

uint8_t mui_u8g2_btn_goto_w2_if(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, U8G2_BTN_HCENTER | mui_u8g2_get_if_flags(ui), u8g2_GetDisplayWidth(u8g2)/2 - 10, 0, MUI_U8G2_V_PADDING, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      //return mui_GotoForm(ui, ui->arg, 0);
      return mui_GotoFormAutoCursorPosition(ui, ui->arg);
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

  uint8_t mui_u8g2_btn_exit_wm_fi(mui_t *ui, uint8_t msg)

  Description:
    A button with size equal to button text plus one pixel padding
    The button has a one pixel frame around the text.
    If selected, then the menu system will be closed.
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
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_utf(ui, U8G2_BTN_HCENTER |mui_u8g2_get_fi_flags(ui), 0, 1, MUI_U8G2_V_PADDING, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      {
        uint8_t *value = (uint8_t *)muif_get_data(ui->uif);
        if ( value != NULL )
          *value = ui->arg;
      }
      mui_SaveForm(ui);          // store the current form and position so that the child can jump back
      mui_LeaveForm(ui);
      return 1;
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}


uint8_t mui_u8g2_btn_goto_w1_pi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_pi(ui, u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui) , ui->text);
      //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui) , MUI_U8G2_V_PADDING, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      //return mui_GotoForm(ui, ui->arg, 0);
      return mui_GotoFormAutoCursorPosition(ui, ui->arg);
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}


uint8_t mui_u8g2_btn_goto_w1_fi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_draw_button_fi(ui, u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui)-1 , ui->text);
      //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui) , MUI_U8G2_V_PADDING, ui->text);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      //return mui_GotoForm(ui, ui->arg, 0);
      return mui_GotoFormAutoCursorPosition(ui, ui->arg);
    case MUIF_MSG_CURSOR_LEAVE:
      break;
    case MUIF_MSG_TOUCH_DOWN:
      break;
    case MUIF_MSG_TOUCH_UP:
      break;    
  }
  return 0;
}

/*===============================================================================*/

static void mui_u8g2_u8_vmm_draw_wm_pi(mui_t *ui) MUI_NOINLINE;
static void mui_u8g2_u8_vmm_draw_wm_pi(mui_t *ui)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  mui_u8g2_u8_min_max_t *vmm= (mui_u8g2_u8_min_max_t *)muif_get_data(ui->uif);
  char buf[4] = "999";
  char *s = buf;
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
  //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), u8g2_GetStrWidth(u8g2, s)+1, 1, MUI_U8G2_V_PADDING, u8x8_u8toa(*value, cnt));
  mui_u8g2_draw_button_pi(ui, u8g2_GetStrWidth(u8g2, s)+1, 1, u8x8_u8toa(*value, cnt));
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
    case MUIF_MSG_VALUE_INCREMENT:
      (*value)++;
      if ( *value > max ) *value = min;
      break;
    case MUIF_MSG_VALUE_DECREMENT:
      if ( *value > min ) (*value)--; else *value = max;
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
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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



static void mui_u8g2_u8_vmm_draw_wm_pf(mui_t *ui) MUI_NOINLINE;
static void mui_u8g2_u8_vmm_draw_wm_pf(mui_t *ui)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  mui_u8g2_u8_min_max_t *vmm= (mui_u8g2_u8_min_max_t *)muif_get_data(ui->uif);
  char buf[4] = "999";
  char *s = buf;
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
  //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), u8g2_GetStrWidth(u8g2, s)+1, 1, MUI_U8G2_V_PADDING, u8x8_u8toa(*value, cnt));
  mui_u8g2_draw_button_pf(ui, u8g2_GetStrWidth(u8g2, s)+1, 1, u8x8_u8toa(*value, cnt));
}


uint8_t mui_u8g2_u8_min_max_wm_mse_pf(mui_t *ui, uint8_t msg)
{
  mui_u8g2_u8_min_max_t *vmm= (mui_u8g2_u8_min_max_t *)muif_get_data(ui->uif);
  uint8_t *value = mui_u8g2_u8mm_get_valptr(vmm);
  uint8_t min = mui_u8g2_u8mm_get_min(vmm);
  uint8_t max = mui_u8g2_u8mm_get_max(vmm);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_vmm_draw_wm_pf(ui);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
      (*value)++;
      if ( *value > max ) *value = min;
      break;
    case MUIF_MSG_VALUE_DECREMENT:
      if ( *value > min ) (*value)--; else *value = max;
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

uint8_t mui_u8g2_u8_min_max_wm_mud_pf(mui_t *ui, uint8_t msg)
{
  mui_u8g2_u8_min_max_t *vmm= (mui_u8g2_u8_min_max_t *)muif_get_data(ui->uif);
  uint8_t *value = mui_u8g2_u8mm_get_valptr(vmm);
  uint8_t min = mui_u8g2_u8mm_get_min(vmm);
  uint8_t max = mui_u8g2_u8mm_get_max(vmm);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_vmm_draw_wm_pf(ui);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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

static uint8_t mui_u8g2_u8_bar_mse_msg_handler(mui_t *ui, uint8_t msg) MUI_NOINLINE;
static uint8_t mui_u8g2_u8_bar_mse_msg_handler(mui_t *ui, uint8_t msg)
{
  mui_u8g2_u8_min_max_step_t *vmms= (mui_u8g2_u8_min_max_step_t *)muif_get_data(ui->uif);
  uint8_t *value = mui_u8g2_u8mms_get_valptr(vmms);
  uint8_t min = mui_u8g2_u8mms_get_min(vmms);
  uint8_t max = mui_u8g2_u8mms_get_max(vmms);
  uint8_t step = mui_u8g2_u8mms_get_step(vmms);
  uint8_t flags = mui_u8g2_u8mms_get_flags(vmms);

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
    case MUIF_MSG_VALUE_INCREMENT:
      (*value)+=step;
      if ( *value > max )
      {
          if ( flags & MUI_MMS_NO_WRAP )
            *value = max;
          else
            *value = min;
      }
      break;
    case MUIF_MSG_VALUE_DECREMENT:
      if ( *value >= min+step ) 
        (*value)-=step; 
      else 
      {
          if ( flags & MUI_MMS_NO_WRAP )
            *value = min;
          else
            *value = max;
      }
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

static uint8_t mui_u8g2_u8_bar_mud_msg_handler(mui_t *ui, uint8_t msg) MUI_NOINLINE;
static uint8_t mui_u8g2_u8_bar_mud_msg_handler(mui_t *ui, uint8_t msg)
{
  mui_u8g2_u8_min_max_step_t *vmms= (mui_u8g2_u8_min_max_step_t *)muif_get_data(ui->uif);
  uint8_t *value = mui_u8g2_u8mms_get_valptr(vmms);
  uint8_t min = mui_u8g2_u8mms_get_min(vmms);
  uint8_t max = mui_u8g2_u8mms_get_max(vmms);
  uint8_t step = mui_u8g2_u8mms_get_step(vmms);
  uint8_t flags = mui_u8g2_u8mms_get_flags(vmms);
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
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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
        (*value)+=step;
        if ( *value > max )
        {
          if ( flags & MUI_MMS_NO_WRAP )
            *value = max;
          else
            *value = min;
        }
        return 1;
      }
      break;
    case MUIF_MSG_EVENT_PREV:
      if ( ui->is_mud )
      {
        if ( *value <= min || *value > max)
        {
          if ( flags & MUI_MMS_NO_WRAP )
            *value = min;
          else
            *value = max;
        }
        else
          (*value)-=step;
        return 1;
      }
      break;
  }
  return 0;
}



static void mui_u8g2_u8_bar_draw_wm(mui_t *ui, uint8_t flags, uint8_t is_fixed_width) MUI_NOINLINE;
static void mui_u8g2_u8_bar_draw_wm(mui_t *ui, uint8_t flags, uint8_t is_fixed_width)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  mui_u8g2_u8_min_max_step_t *vmms= (mui_u8g2_u8_min_max_step_t *)muif_get_data(ui->uif);
  char buf[4] = "999";
  char *s = buf;
  uint8_t *value = mui_u8g2_u8mms_get_valptr(vmms);
  uint8_t min = mui_u8g2_u8mms_get_min(vmms);
  uint8_t max = mui_u8g2_u8mms_get_max(vmms);
  uint8_t scale = 0;
  //uint8_t step = mui_u8g2_u8mms_get_step(vmms);
  uint8_t mms_flags = mui_u8g2_u8mms_get_flags(vmms);
  uint8_t cnt = 3;
  uint8_t height = u8g2_GetAscent(u8g2);
  int8_t backup_descent;
  u8g2_uint_t x = mui_get_x(ui);
  u8g2_uint_t w = 0;
  u8g2_uint_t v;  // the calculated pixel value
  
  if ( mms_flags & MUI_MMS_2X_BAR )
    scale |= 1;
  if ( mms_flags & MUI_MMS_4X_BAR )
    scale |= 2;
  
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

  if ( is_fixed_width == 0 )
  {
    w += (max<<scale);          // total width of the bar is derived from the max value
    v = (*value)<<scale;          // pixel position for the current value
  }
  else
  {
    u8g2_uint_t width = mui_u8g2_u8mms_get_width(vmms);
    
    w += (width<<scale);          // total width of bar is defined by the width argument
    v = ((u8g2_long_t)(*value) * (u8g2_long_t)(width<<scale)) / (u8g2_long_t)max;    // u8g2_long_t is int32_t if 16 bit mode is enabled
  }

  w += 2;                               // add gap for the frame
  
  u8g2_DrawFrame( u8g2, x, mui_get_y(ui)-height, w, height);
  u8g2_DrawBox( u8g2, x+1, mui_get_y(ui)-height+1, v, height-2);
  if ( mms_flags & MUI_MMS_SHOW_VALUE )
  {
    w += 2;
    u8g2_DrawStr(u8g2,  x+w, mui_get_y(ui), u8x8_u8toa(*value, cnt) );
    w += u8g2_GetStrWidth(u8g2, s);
    w += 1; 
  }
  backup_descent = u8g2->font_ref_descent;
  u8g2->font_ref_descent = 0; /* hmm... that's a low level hack so that DrawButtonFrame ignores the descent value of the font */
  u8g2_DrawButtonFrame(u8g2, x, mui_get_y(ui), flags, w, 1, 1);
  u8g2->font_ref_descent = backup_descent;  
}

// #define MUIF_U8G2_U8_MIN_MAX_STEP(id, valptr, min, max, step, flags, muif)

uint8_t mui_u8g2_u8_bar_wm_mse_pi(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_bar_draw_wm(ui, mui_u8g2_get_pi_flags(ui), 0);
      break;
    default:
      return mui_u8g2_u8_bar_mse_msg_handler(ui, msg);
  }
  return 0;
}


uint8_t mui_u8g2_u8_bar_wm_mud_pi(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_bar_draw_wm(ui, mui_u8g2_get_pi_flags(ui), 0);
      break;
    default:
      return mui_u8g2_u8_bar_mud_msg_handler(ui, msg);
  }
  return 0;
}

uint8_t mui_u8g2_u8_bar_wm_mse_pf(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_bar_draw_wm(ui, mui_u8g2_get_pf_flags(ui), 0);
      break;
    default:
      return mui_u8g2_u8_bar_mse_msg_handler(ui, msg);
  }
  return 0;
}

uint8_t mui_u8g2_u8_bar_wm_mud_pf(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_bar_draw_wm(ui, mui_u8g2_get_pf_flags(ui), 0);
      break;
    default:
      return mui_u8g2_u8_bar_mud_msg_handler(ui, msg);
  }
  return 0;
}



// #define MUIF_U8G2_U8_MIN_MAX_STEP_WIDTH(id, valptr, min, max, step, width, flags, muif) 


uint8_t mui_u8g2_u8_fixed_width_bar_wm_mse_pi(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_bar_draw_wm(ui, mui_u8g2_get_pi_flags(ui), 1);
      break;
    default:
      return mui_u8g2_u8_bar_mse_msg_handler(ui, msg);
  }
  return 0;
}


uint8_t mui_u8g2_u8_fixed_width_bar_wm_mud_pi(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_bar_draw_wm(ui, mui_u8g2_get_pi_flags(ui), 1);
      break;
    default:
      return mui_u8g2_u8_bar_mud_msg_handler(ui, msg);
  }
  return 0;
}

uint8_t mui_u8g2_u8_fixed_width_bar_wm_mse_pf(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_bar_draw_wm(ui, mui_u8g2_get_pf_flags(ui), 1);
      break;
    default:
      return mui_u8g2_u8_bar_mse_msg_handler(ui, msg);
  }
  return 0;
}

uint8_t mui_u8g2_u8_fixed_width_bar_wm_mud_pf(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      mui_u8g2_u8_bar_draw_wm(ui, mui_u8g2_get_pf_flags(ui), 1);
      break;
    default:
      return mui_u8g2_u8_bar_mud_msg_handler(ui, msg);
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
      mui_u8g2_draw_button_pi(ui, u8g2_GetMaxCharWidth(u8g2), 1, buf);
      //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), u8g2_GetMaxCharWidth(u8g2), 1, MUI_U8G2_V_PADDING, buf);
      //u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), mui_u8g2_get_pi_flags(ui), u8g2_GetMaxCharWidth(u8g2), 1, MUI_U8G2_V_PADDING, buf);
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
     case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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
      mui_u8g2_draw_button_pi(ui, ui->arg, 1, ui->text);
      //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, ui->text);
      //u8g2_DrawButtonUTF8(u8g2, mui_get_x(ui), mui_get_y(ui), mui_u8g2_get_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, ui->text);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
      (*value)++;
      if ( mui_fds_get_nth_token(ui, *value) == 0 ) 
        *value = 0;      
      break;
    case MUIF_MSG_VALUE_DECREMENT:
      if ( *value > 0 ) 
        (*value)--;
      else
        (*value) = mui_fds_get_token_cnt(ui)-1;
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

uint8_t mui_u8g2_u8_opt_line_wa_mse_pf(mui_t *ui, uint8_t msg)
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
      mui_u8g2_draw_button_pf(ui, ui->arg, 1, ui->text);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
      (*value)++;
      if ( mui_fds_get_nth_token(ui, *value) == 0 ) 
        *value = 0;      
      break;
    case MUIF_MSG_VALUE_DECREMENT:
      if ( *value > 0 ) 
        (*value)--;
      else
        (*value) = mui_fds_get_token_cnt(ui)-1;
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
      mui_u8g2_draw_button_pi(ui, ui->arg, 1, ui->text);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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

uint8_t mui_u8g2_u8_opt_line_wa_mud_pf(mui_t *ui, uint8_t msg)
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
      mui_u8g2_draw_button_pf(ui, ui->arg, 1, ui->text);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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
          //u8g2_SetFontMode(u8g2, 1);
          a += 2;       /* add gap between the checkbox and the text area */
          u8g2_DrawUTF8(u8g2, mui_get_x(ui)+a, mui_get_y(ui), ui->text);
        }
        
        u8g2_DrawButtonFrame(u8g2, mui_get_x(ui), mui_get_y(ui), flags, w+a, 1, MUI_U8G2_V_PADDING);
      }
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      (*value)++;
      if ( *value > 1 ) *value = 0;      
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
          //u8g2_SetFontMode(u8g2, 1);
          a += 2;       /* add gap between the checkbox and the text area */
          u8g2_DrawUTF8(u8g2, mui_get_x(ui)+a, mui_get_y(ui), ui->text);
        }
        
        u8g2_DrawButtonFrame(u8g2, mui_get_x(ui), mui_get_y(ui), flags, w+a, 1, MUI_U8G2_V_PADDING);
      }
      break;
   case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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


uint8_t mui_u8g2_u8_opt_parent_wm_pi(mui_t *ui, uint8_t msg)
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
      mui_u8g2_draw_button_pi(ui, 0, 1, ui->text);
      //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), 0, 1, MUI_U8G2_V_PADDING, ui->text);
      
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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
          ui->form_scroll_total = mui_GetSelectableFieldOptionCnt(ui, ui->last_form_fds);
      //printf("MUIF_MSG_FORM_START: arg=%d visible=%d top=%d total=%d\n", arg, ui->form_scroll_visible, ui->form_scroll_top, ui->form_scroll_total);
      break;
    case MUIF_MSG_FORM_END:  
      break;
    case MUIF_MSG_CURSOR_ENTER:
      return mui_u8g2_handle_scroll_next_prev_events(ui, msg);
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      if ( value != NULL )
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
      return mui_u8g2_handle_scroll_next_prev_events(ui, msg);
    case MUIF_MSG_EVENT_PREV:
      return mui_u8g2_handle_scroll_next_prev_events(ui, msg);
  }
  return 0;
}


uint8_t mui_u8g2_u8_opt_radio_child_wm_pi(mui_t *ui, uint8_t msg)
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
          mui_GetSelectableFieldTextOption(ui, ui->last_form_fds, arg + ui->form_scroll_top);
        }
        
        if ( ui->text[0] != '\0' )
        {
          w =  u8g2_GetUTF8Width(u8g2, ui->text);
          //u8g2_SetFontMode(u8g2, 1);
          a += 2;       /* add gap between the checkbox and the text area */
          u8g2_DrawUTF8(u8g2, x+a, y, ui->text);
        }        
        if ( is_focus )
        {
          u8g2_DrawButtonFrame(u8g2, x, y, U8G2_BTN_INV, w+a, 1, MUI_U8G2_V_PADDING);
        }
      }
      break;
    default:
      return mui_u8g2_u8_opt_child_mse_common(ui, msg);
  }
  return 0;
}


uint8_t mui_u8g2_u8_opt_radio_child_w1_pi(mui_t *ui, uint8_t msg)
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
          mui_GetSelectableFieldTextOption(ui, ui->last_form_fds, arg + ui->form_scroll_top);
        }
        
        if ( ui->text[0] != '\0' )
        {
          //w =  u8g2_GetUTF8Width(u8g2, ui->text);
          //u8g2_SetFontMode(u8g2, 1);
          a += 2;       /* add gap between the checkbox and the text area */
          u8g2_DrawUTF8(u8g2, x+a, y, ui->text);
        }        
        if ( is_focus )
        {
          u8g2_DrawButtonFrame(u8g2, 0, y, U8G2_BTN_INV, u8g2_GetDisplayWidth(u8g2), 0, MUI_U8G2_V_PADDING);
        }
      }
      break;
    default:
      return mui_u8g2_u8_opt_child_mse_common(ui, msg);
  }
  return 0;
}


uint8_t mui_u8g2_u8_opt_child_wm_pi(mui_t *ui, uint8_t msg)
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
        uint8_t flags = mui_u8g2_get_pi_flags(ui);
        //if ( mui_IsCursorFocus(ui) )
        //{
        //  flags = U8G2_BTN_INV;
        //}

        if ( ui->text[0] == '\0' )
        {
          /* if the text is not provided, then try to get the text from the previous (saved) element, assuming that this contains the selection */
          /* this will overwrite all ui member functions, so we must not access any ui members (except ui->text) any more */
          mui_GetSelectableFieldTextOption(ui, ui->last_form_fds, arg + ui->form_scroll_top);
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
  an invisible field (which will not show anything). It should also not be selectable 
  it just provides the menu entries, see "mui_u8g2_u8_opt_child_mse_common" and friends 
  as a consequence it does not have width, input mode and style

  MUIF: MUIF_RO()
  FDS: MUI_DATA()

  mui_u8g2_goto_parent --> mui_u8g2_goto_data

  Used together with mui_u8g2_goto_form_w1_pi

*/
uint8_t mui_u8g2_goto_data(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      break;
    case MUIF_MSG_FORM_START:
      // store the field (and the corresponding elements) in the last_form_fds variable.
      // last_form_fds is later used to access the elements (see mui_u8g2_u8_opt_child_mse_common and friends)
      ui->last_form_fds = ui->fds;
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
mui_u8g2_goto_child_w1_mse_pi --> mui_u8g2_goto_form_w1_pi
*/
uint8_t mui_u8g2_goto_form_w1_pi(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  uint8_t arg = ui->arg;        // remember the arg value, because it might be overwritten  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_GetSelectableFieldTextOption(ui, ui->last_form_fds, arg + ui->form_scroll_top) )
        mui_u8g2_draw_button_pi(ui, u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui), ui->text+1);
      break;
    case MUIF_MSG_CURSOR_SELECT:
      if ( mui_GetSelectableFieldTextOption(ui, ui->last_form_fds, ui->arg + ui->form_scroll_top) )
      {
        mui_SaveCursorPosition(ui, ui->arg + ui->form_scroll_top);     // store the current cursor position, so that the user can jump back to the corresponding cursor position
        return mui_GotoFormAutoCursorPosition(ui, (uint8_t)ui->text[0]);
      }
      break;
    default:
      return mui_u8g2_u8_opt_child_mse_common(ui, msg);
  }
  return 0;
}

uint8_t mui_u8g2_goto_form_w1_pf(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  uint8_t arg = ui->arg;        // remember the arg value, because it might be overwritten  
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      if ( mui_GetSelectableFieldTextOption(ui, ui->last_form_fds, arg + ui->form_scroll_top) )
        mui_u8g2_draw_button_pf(ui, u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui)-1, ui->text+1);
      break;
    case MUIF_MSG_CURSOR_SELECT:
      if ( mui_GetSelectableFieldTextOption(ui, ui->last_form_fds, ui->arg + ui->form_scroll_top) )
      {
        mui_SaveCursorPosition(ui, ui->arg + ui->form_scroll_top);     // store the current cursor position, so that the user can jump back to the corresponding cursor position
        return mui_GotoFormAutoCursorPosition(ui, (uint8_t)ui->text[0]);
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
      mui_u8g2_draw_button_pi(ui, ui->arg, 1, element_cb(data, *selection));
      //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, element_cb(data, *selection));
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
      (*selection)++;
      if ( *selection >= count_cb(data) ) 
        *selection = 0;
      break;
    case MUIF_MSG_VALUE_DECREMENT:
      if ( *selection > 0 )
        (*selection)--;
      else
        (*selection) = count_cb(data)-1;
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
      mui_u8g2_draw_button_pi(ui, ui->arg, 1, element_cb(data, *selection));
      //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, element_cb(data, *selection));
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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
uint8_t mui_u8g2_u16_list_parent_wm_pi(mui_t *ui, uint8_t msg)
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
      mui_u8g2_draw_button_pi(ui, 0, 1, element_cb(data, *selection));
      //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), ui->arg, 1, MUI_U8G2_V_PADDING, element_cb(data, *selection));
      break;
    case MUIF_MSG_FORM_START:
      break;
    case MUIF_MSG_FORM_END:
      break;
    case MUIF_MSG_CURSOR_ENTER:
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
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
      return mui_u8g2_handle_scroll_next_prev_events(ui, msg);
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      if ( selection != NULL )
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
      return mui_u8g2_handle_scroll_next_prev_events(ui, msg);
    case MUIF_MSG_EVENT_PREV:
      return mui_u8g2_handle_scroll_next_prev_events(ui, msg);
  }
  return 0;
}

uint8_t mui_u8g2_u16_list_child_w1_pi(mui_t *ui, uint8_t msg)
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

        //u8g2_SetFontMode(u8g2, 1);
        a += 2;       /* add gap between the checkbox and the text area */
        if ( pos < count_cb(data) )
          u8g2_DrawUTF8(u8g2, x+a, y, element_cb(data, pos));
        if ( is_focus )
        {
          u8g2_DrawButtonFrame(u8g2, 0, y, U8G2_BTN_INV, u8g2_GetDisplayWidth(u8g2), 0, MUI_U8G2_V_PADDING);
        }
      }
      break;
    default:
      return mui_u8g2_u16_list_child_mse_common(ui, msg);
  }
  return 0;
}

uint8_t mui_u8g2_u16_list_goto_w1_pi(mui_t *ui, uint8_t msg)
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
      mui_u8g2_draw_button_pi(ui, u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui), element_cb(data, pos)+1);
      //mui_u8g2_draw_button_utf(ui, mui_u8g2_get_pi_flags(ui), u8g2_GetDisplayWidth(u8g2)-mui_get_x(ui)*2, mui_get_x(ui), MUI_U8G2_V_PADDING, element_cb(data, pos)+1);
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_VALUE_INCREMENT:
    case MUIF_MSG_VALUE_DECREMENT:
      if ( selection != NULL )
        *selection = pos;
      mui_SaveCursorPosition(ui, pos >= 255 ? 0 : pos);     // store the current cursor position, so that the user can jump back to the corresponding cursor position
      mui_GotoFormAutoCursorPosition(ui, (uint8_t)element_cb(data, pos)[0]); 
      break;
    default:
      return mui_u8g2_u16_list_child_mse_common(ui, msg);
  }
  return 0;
}
