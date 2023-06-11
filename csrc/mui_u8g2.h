/*

  mui_u8g2.h

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

  Reference Manual:
    https://github.com/olikraus/u8g2/wiki/muiref

  MUIF_U8G2_LABEL()  
    replacement for MUIF_LABEL(mui_u8g2_draw_text), 
    used by MUI_LABEL(x,y,"text")
    Supports UTF8
    
  MUIF_U8G2_FONT_STYLE(n, font) 
    A special u8g2 style function, which replaces MUIF_STYLE, but restricts the style change to the
    specific font argument (however, this should be good enough in most cases).
    As usual, the style "n" can be activated with MUI_STYLE(n) in FDS.
    Example:
      muif_t muif_list[]  MUI_PROGMEM = {  
          MUIF_U8G2_LABEL(),
          MUIF_U8G2_FONT_STYLE(0, u8g2_font_5x8_tr) 
        };
        fds_t fds[] MUI_PROGMEM  =
        MUI_FORM(1)
        MUI_STYLE(0)
        MUI_LABEL(5,12, "5x8 Font")
        ;

    



*/

#ifndef MUI_U8G2_H
#define MUI_U8G2_H

#include "mui.h"

/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
extern "C" {
#endif


#define MUI_U8G2_COMMA ,

typedef const char * (*mui_u8g2_get_list_element_cb)(void *data, uint16_t index);
typedef uint16_t (*mui_u8g2_get_list_count_cb)(void *data);

struct mui_u8g2_list_struct
{
  uint16_t *selection;
  void *data;
  mui_u8g2_get_list_element_cb get_list_element;
  mui_u8g2_get_list_count_cb get_list_count;  
} MUI_PROGMEM;

typedef const struct mui_u8g2_list_struct mui_u8g2_list_t;

#if defined(__GNUC__) && defined(__AVR__)
#  define mui_u8g2_list_get_selection_ptr(list)         ((uint16_t *)mui_pgm_wread(&((list)->selection)))
#  define mui_u8g2_list_get_data_ptr(list)                ((void *)mui_pgm_wread(&((list)->data)))
#  define mui_u8g2_list_get_element_cb(list)          ((mui_u8g2_get_list_element_cb)mui_pgm_wread(&((list)->get_list_element)))
#  define mui_u8g2_list_get_count_cb(list)              ((mui_u8g2_get_list_count_cb)mui_pgm_wread(&((list)->get_list_count)))
#else
#  define mui_u8g2_list_get_selection_ptr(list)                 ((list)->selection)
#  define mui_u8g2_list_get_data_ptr(list)                         ((list)->data)
#  define mui_u8g2_list_get_element_cb(list)                   ((list)->get_list_element)
#  define mui_u8g2_list_get_count_cb(list)                      ((list)->get_list_count)
#endif


struct mui_u8g2_u8_min_max_struct
{
  uint8_t *value;
  uint8_t min;
  uint8_t max;
} MUI_PROGMEM;

typedef const struct mui_u8g2_u8_min_max_struct mui_u8g2_u8_min_max_t;

#if defined(__GNUC__) && defined(__AVR__)
#  define mui_u8g2_u8mm_get_min(u8mm) mui_pgm_read(&((u8mm)->min))
#  define mui_u8g2_u8mm_get_max(u8mm) mui_pgm_read(&((u8mm)->max))
#  define mui_u8g2_u8mm_get_valptr(u8mm) ((uint8_t *)mui_pgm_wread(&((u8mm)->value)))
#else
#  define mui_u8g2_u8mm_get_min(u8mm) ((u8mm)->min)
#  define mui_u8g2_u8mm_get_max(u8mm) ((u8mm)->max)
#  define mui_u8g2_u8mm_get_valptr(u8mm) ((u8mm)->value)
#endif


struct mui_u8g2_u8_min_max_step_struct
{
  uint8_t *value;
  uint8_t min;
  uint8_t max;
  uint8_t step;
  uint8_t flags;
  uint8_t width;        // added with issue 2200, might not be used by all bar graph functions
} MUI_PROGMEM;

typedef const struct mui_u8g2_u8_min_max_step_struct mui_u8g2_u8_min_max_step_t;

/* list of bit values for the "flags" variable */
#define MUI_MMS_2X_BAR 0x01
#define MUI_MMS_4X_BAR 0x02
#define MUI_MMS_SHOW_VALUE 0x04
#define MUI_MMS_NO_WRAP 0x08

#if defined(__GNUC__) && defined(__AVR__)
#  define mui_u8g2_u8mms_get_width(u8mm) mui_pgm_read(&((u8mm)->width))
#  define mui_u8g2_u8mms_get_step(u8mm) mui_pgm_read(&((u8mm)->step))
#  define mui_u8g2_u8mms_get_flags(u8mm) mui_pgm_read(&((u8mm)->flags))
#  define mui_u8g2_u8mms_get_min(u8mm) mui_pgm_read(&((u8mm)->min))
#  define mui_u8g2_u8mms_get_max(u8mm) mui_pgm_read(&((u8mm)->max))
#  define mui_u8g2_u8mms_get_valptr(u8mm) ((uint8_t *)mui_pgm_wread(&((u8mm)->value)))
#else
#  define mui_u8g2_u8mms_get_width(u8mm) ((u8mm)->width)
#  define mui_u8g2_u8mms_get_step(u8mm) ((u8mm)->step)
#  define mui_u8g2_u8mms_get_flags(u8mm) ((u8mm)->flags)
#  define mui_u8g2_u8mms_get_min(u8mm) ((u8mm)->min)
#  define mui_u8g2_u8mms_get_max(u8mm) ((u8mm)->max)
#  define mui_u8g2_u8mms_get_valptr(u8mm) ((u8mm)->value)
#endif




/* helper functions */

u8g2_uint_t mui_get_x(mui_t *ui);
u8g2_uint_t mui_get_y(mui_t *ui);
u8g2_t *mui_get_U8g2(mui_t *ui);

void mui_u8g2_draw_button_utf(mui_t *ui, u8g2_uint_t flags, u8g2_uint_t width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, const char *text);
u8g2_uint_t mui_u8g2_get_pi_flags(mui_t *ui);
void mui_u8g2_draw_button_pi(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text);
u8g2_uint_t mui_u8g2_get_fi_flags(mui_t *ui);
void mui_u8g2_draw_button_fi(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text);
u8g2_uint_t mui_u8g2_get_pf_flags(mui_t *ui);
void mui_u8g2_draw_button_pf(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text);
u8g2_uint_t mui_u8g2_get_if_flags(mui_t *ui);
void mui_u8g2_draw_button_if(mui_t *ui, u8g2_uint_t width, u8g2_uint_t padding_h, const char *text);



/* ready to use field functions */

uint8_t mui_u8g2_draw_text(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_btn_goto_wm_fi(mui_t *ui, uint8_t msg);        /* GIF */
uint8_t mui_u8g2_btn_goto_wm_if(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_btn_goto_w2_fi(mui_t *ui, uint8_t msg);         /* GIF */
uint8_t mui_u8g2_btn_goto_w2_if(mui_t *ui, uint8_t msg);

uint8_t mui_u8g2_btn_goto_w1_pi(mui_t *ui, uint8_t msg);        /* GIF */
uint8_t mui_u8g2_btn_goto_w1_fi(mui_t *ui, uint8_t msg);        /* GIF */

uint8_t mui_u8g2_btn_exit_wm_fi(mui_t *ui, uint8_t msg);        /* similar to 'mui_u8g2_btn_goto_wm_fi' but will exit the menu system */

uint8_t mui_u8g2_u8_chkbox_wm_pi(mui_t *ui, uint8_t msg);       /* GIF, MUIF_VARIABLE, MUI_XY */
uint8_t mui_u8g2_u8_radio_wm_pi(mui_t *ui, uint8_t msg);        /* GIF, MUIF_VARIABLE,MUI_XYAT */



uint8_t mui_u8g2_u8_opt_line_wa_mse_pi(mui_t *ui, uint8_t msg); /* GIF, MUIF_VARIABLE,MUI_XYAT */
uint8_t mui_u8g2_u8_opt_line_wa_mse_pf(mui_t *ui, uint8_t msg); /* GIF, MUIF_VARIABLE,MUI_XYAT */
uint8_t mui_u8g2_u8_opt_line_wa_mud_pi(mui_t *ui, uint8_t msg); /* GIF, MUIF_VARIABLE,MUI_XYAT */
uint8_t mui_u8g2_u8_opt_line_wa_mud_pf(mui_t *ui, uint8_t msg); /* GIF, MUIF_VARIABLE,MUI_XYAT */

/* dropdown list / combo box */
/* The text part of the parent defines a '|' separated list of elements, which can be selected by the child. */
/* Argument is a form number where the child element is placed multiple times */ 
/* The child form does not require the ok button, because the child function will return to the parent with the select element */
uint8_t mui_u8g2_u8_opt_parent_wm_pi(mui_t *ui, uint8_t msg);       /* GIF, MUIF_VARIABLE, MUI_XYAT */
uint8_t mui_u8g2_u8_opt_radio_child_wm_pi(mui_t *ui, uint8_t msg);        /* GIF, MUIF_VARIABLE, MUI_XYA */
uint8_t mui_u8g2_u8_opt_radio_child_w1_pi(mui_t *ui, uint8_t msg);          /* GIF, MUIF_VARIABLE, MUI_XYA */
uint8_t mui_u8g2_u8_opt_child_wm_pi(mui_t *ui, uint8_t msg);                /* MUIF_VARIABLE, MUI_XYA */ 
/* Note: there is no opt_child_goto muif, because this can be done with mui_u8g2_goto_form_w1_pi */

/* (scrollable) jump menu */
/* The text part of the parent defines a '|' separated list of elements, which can be selected goto_form functions. */
/* Each '|' separated element must be prefixed with the form number (MUI_x) */
uint8_t mui_u8g2_goto_data(mui_t *ui, uint8_t msg);                        /* REF, MUIF_RO, MUI_DATA (WARNING: Must appear only once per form!!! */
uint8_t mui_u8g2_goto_form_w1_pi(mui_t *ui, uint8_t msg);          /* REF, MUIF_BUTTON, MUI_XYA */
uint8_t mui_u8g2_goto_form_w1_pf(mui_t *ui, uint8_t msg);           /* REF, MUIF_BUTTON, MUI_XYA */


/* character input */
uint8_t mui_u8g2_u8_char_wm_mud_pi(mui_t *ui, uint8_t msg);     /* GIF, MUIF_VARIABLE,MUI_XY, usually requires a monospaced font line profont12 */


/*===== MUIF U8g2 Label  =====*/

#define MUIF_U8G2_LABEL()  MUIF_LABEL(mui_u8g2_draw_text)


/*===== data = u8g2 font data  =====*/

//#define MUIF_U8G2_FONT_STYLE(n,font)  MUIF("S" #n, 0, (void *)(font), mui_u8g2_set_font_style_function) 
#define MUIF_U8G2_FONT_STYLE(n, font) { 'S', #n[0], 0, 0, (void *)(font), mui_u8g2_set_font_style_function} 


uint8_t mui_u8g2_set_font_style_function(mui_t *ui, uint8_t msg);


/*===== data = mui_u8g2_u8_min_max_t*  =====*/

/* gcc note: the macro uses array compound literals to extend the lifetime in C++, see last section in https://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html */
#define MUIF_U8G2_U8_MIN_MAX(id, valptr, min, max, muif) \
  MUIF(id, MUIF_CFLAG_IS_CURSOR_SELECTABLE,  \
  (void *)((mui_u8g2_u8_min_max_t [] ) {{ (valptr) MUI_U8G2_COMMA (min) MUI_U8G2_COMMA (max)}}), \
  (muif))

uint8_t mui_u8g2_u8_min_max_wm_mse_pi(mui_t *ui, uint8_t msg);   /* GIF, MUIF_U8G2_U8_MIN_MAX, MUI_XY */
uint8_t mui_u8g2_u8_min_max_wm_mud_pi(mui_t *ui, uint8_t msg);  /* GIF, MUIF_U8G2_U8_MIN_MAX, MUI_XY */

uint8_t mui_u8g2_u8_min_max_wm_mse_pf(mui_t *ui, uint8_t msg);  /* GIF, MUIF_U8G2_U8_MIN_MAX, MUI_XY */
uint8_t mui_u8g2_u8_min_max_wm_mud_pf(mui_t *ui, uint8_t msg);  /* GIF, MUIF_U8G2_U8_MIN_MAX, MUI_XY */

/*===== data = mui_u8g2_u8_min_max_step_t*  =====*/

/* gcc note: the macro uses array compound literals to extend the lifetime in C++, see last section in https://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html */
#define MUIF_U8G2_U8_MIN_MAX_STEP(id, valptr, min, max, step, flags, muif) \
  MUIF(id, MUIF_CFLAG_IS_CURSOR_SELECTABLE,  \
  (void *)((mui_u8g2_u8_min_max_step_t [] ) {{ (valptr) MUI_U8G2_COMMA (min) MUI_U8G2_COMMA (max) MUI_U8G2_COMMA (step) MUI_U8G2_COMMA (flags) MUI_U8G2_COMMA (0) }}), \
  (muif))
  

uint8_t mui_u8g2_u8_bar_wm_mse_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_bar_wm_mud_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_bar_wm_mse_pf(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_bar_wm_mud_pf(mui_t *ui, uint8_t msg);


#define MUIF_U8G2_U8_MIN_MAX_STEP_WIDTH(id, valptr, min, max, step, width, flags, muif) \
  MUIF(id, MUIF_CFLAG_IS_CURSOR_SELECTABLE,  \
  (void *)((mui_u8g2_u8_min_max_step_t [] ) {{ (valptr) MUI_U8G2_COMMA (min) MUI_U8G2_COMMA (max) MUI_U8G2_COMMA (step) MUI_U8G2_COMMA (flags) MUI_U8G2_COMMA (width) }}), \
  (muif))
  

uint8_t mui_u8g2_u8_fixed_width_bar_wm_mse_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_fixed_width_bar_wm_mud_pi(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_fixed_width_bar_wm_mse_pf(mui_t *ui, uint8_t msg);
uint8_t mui_u8g2_u8_fixed_width_bar_wm_mud_pf(mui_t *ui, uint8_t msg);



/*===== data = mui_u8g2_list_t*  =====*/
/* similar to mui_u8g2_u8_opt_line, but u16 and dynamic list */


#define MUIF_U8G2_U16_LIST(id, valptr, dataptr, getcb, cntcb, muif) \
  MUIF(id, MUIF_CFLAG_IS_CURSOR_SELECTABLE,  \
  (void *)((mui_u8g2_list_t [] ) {{ (valptr) MUI_U8G2_COMMA (dataptr) MUI_U8G2_COMMA (getcb) MUI_U8G2_COMMA (cntcb)}}), \
  (muif))
  
uint8_t mui_u8g2_u16_list_line_wa_mse_pi(mui_t *ui, uint8_t msg);       /* GIF, MUIF_U8G2_U16_LIST, MUI_XYA, arg=pixel fieldsize */
uint8_t mui_u8g2_u16_list_line_wa_mud_pi(mui_t *ui, uint8_t msg);       /* GIF, MUIF_U8G2_U16_LIST, MUI_XYA, arg=pixel fieldsize */


/* dropdown list / combo box with 16 size and callback functions for MUIF_U8G2_U16_LIST */
uint8_t mui_u8g2_u16_list_parent_wm_pi(mui_t *ui, uint8_t msg);     /* GIF, MUIF_U8G2_U16_LIST, MUI_XYA, arg=subform */
uint8_t mui_u8g2_u16_list_child_w1_pi(mui_t *ui, uint8_t msg);      /* GIF, MUIF_U8G2_U16_LIST, MUI_XYA, arg=sub element number */
uint8_t mui_u8g2_u16_list_goto_w1_pi(mui_t *ui, uint8_t msg);               /* REF, MUIF_U8G2_U16_LIST first char of the string denotes the target form */


#ifdef __cplusplus
}
#endif


#endif /* MUI_U8G2_H */

