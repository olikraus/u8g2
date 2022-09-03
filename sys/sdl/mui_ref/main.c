/*
  mui_ref
  
  
  Documentatio
  
  mui-style
    monochrome user interface concepts
      selected element: inverted vs frame
      selection criteria: size vs. number of element types
        only buttons --> inverted cursor
        buttons and user elements --> frame cuser
    what means style:
      selected element concept (inverted vs frame)
      MUI_STYLE command for fonts
      callback postfix
    Edit modes
      unselected, selected, selected & update
  
  
*/
#include "u8g2.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mui.h"
#include "mui_u8g2.h"


/*=================================================*/
/* global variables */

u8g2_t u8g2;
mui_t ui;

/*
  global variables which form the communication gateway between the user interface and the rest of the code
*/
uint8_t number_input9 = 2;       // variable where the user can input a number between 0 and 9
uint8_t number_input99 = 95;       // variable where the user can input a number between 0 and 99
uint8_t chkbox1_input = 0;
uint8_t chkbox2_input = 0;
uint8_t radio_input = 0;
uint8_t fruit_input = 2;
uint8_t color_input = 0;
uint8_t food_input = 0;
uint8_t text_input[4] = { ' ',' ',' ',' '} ;

uint16_t list_selection_1 = 0;
uint16_t list_selection_2 = 0;
uint16_t list_selection_3 = 0;



/*=================================================*/
/* user element access functions */

static const char *animals[] = { "Bird", "Bison", "Cat", "Crow", "Dog", "Elephant", "Fish", "Gnu", "Horse", "Koala", "Lion", "Mouse", "Owl", "Rabbit", "Spider", "Turtle", "Zebra" };

uint16_t animals_get_cnt(void *data)
{
  return sizeof(animals)/sizeof(*animals);    /* number of animals */
}

const char *animals_get_str(void *data, uint16_t index)
{
  return animals[index];
}


/*=================================================*/
/* style */




uint8_t mui_style_helv_r_08(mui_t *ui, uint8_t msg)
{
  
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      
      u8g2_SetFont(u8g2, u8g2_font_helvR08_tr);
      //u8g2_SetFont(u8g2, u8g2_font_squeezed_r7_tr);
      break;
  }
  return 0;
}

uint8_t mui_style_helv_b_08(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      
      //u8g2_SetFont(u8g2, u8g2_font_helvB08_tr);
      u8g2_SetFont(u8g2, u8g2_font_squeezed_b7_tr);
      //su8g2_SetFont(u8g2, u8g2_font_lubBI14_te);
      break;
  }
  return 0;
}

uint8_t mui_style_menu_items(mui_t *ui, uint8_t msg)
{
  
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      
      //u8g2_SetFont(u8g2, u8g2_font_helvR08_tr);
      u8g2_SetFont(u8g2, u8g2_font_squeezed_r7_tr);
      break;
  }
  return 0;
}


uint8_t mui_style_monospaced(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_SetFont(u8g2, u8g2_font_profont12_tr);
      //u8g2_SetFont(u8g2, u8g2_font_6x10_tr);
      break;
  }
  return 0;
}


uint8_t mui_style_streamline_food_drink(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_SetFont(u8g2, u8g2_font_streamline_food_drink_t);
      break;
  }
  return 0;
}

uint8_t mui_style_small(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_SetFont(u8g2, u8g2_font_blipfest_07_tr );
      //u8g2_SetFont(u8g2, u8g2_font_squeezed_r7_tr);
      break;
  }
  return 0;
}

uint8_t mui_style_streamline_interface_essential_login(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_SetFont(u8g2, u8g2_font_streamline_interface_essential_login_t);
      //u8g2_SetFont(u8g2, u8g2_font_6x10_tr);
      break;
  }
  return 0;
}



/*=================================================*/
/* local mui functions */

uint8_t mui_hrule(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_DrawHLine(u8g2, 0, mui_get_y(ui), u8g2_GetDisplayWidth(u8g2));
      break;
  }
  return 0;
}




/*=================================================*/
/* list functions (get cnt/get element pairs) */


uint16_t list_get_cnt(void *data)
{
  return 17;    /* number of animals */
}

const char *list_get_str(void *data, uint16_t index)
{
  static const char *animals[] = { "Bird", "Bison", "Cat", "Crow", "Dog", "Elephant", "Fish", "Gnu", "Horse", "Koala", "Lion", "Mouse", "Owl", "Rabbit", "Spider", "Turtle", "Zebra" };
  return animals[index];
}


uint16_t menu_get_cnt(void *data)
{
  return 10;    /* number of menu entries */
}

const char *menu_get_str(void *data, uint16_t index)
{
  static const char *menu[] = 
  { 
    MUI_1 "Goto Main Menu",
    MUI_10 "Enter a number",
    MUI_11 "Parent/Child Selection",
    MUI_13 "Checkbox",
    MUI_14 "Radio Selection",
    MUI_15 "Text Input",
    MUI_16 "Single Line Selection",
    MUI_17 "List Line Selection",
    MUI_18 "Parent/Child List",
    MUI_20 "Array Edit",
  };
  return menu[index];
}



/*=================================================*/
/* list functions (get cnt/get element pairs) */

muif_t muif_list[] MUI_PROGMEM = {
  /* normal text style for the examples */
  MUIF_STYLE(0, mui_style_helv_r_08),
  
  /* bold text style for menu heading */
  MUIF_STYLE(1, mui_style_helv_b_08),

  /* very small font with programming info */
  MUIF_STYLE(2, mui_style_small),

  
  MUIF_STYLE(3, mui_style_streamline_interface_essential_login),

  
  /* food and drink */
  MUIF_STYLE(4, mui_style_streamline_food_drink),
  
  /* monospaced font, good for  mui_u8g2_u8_char_wm_mud_pi */
  MUIF_STYLE(5, mui_style_monospaced),

  /* smaller replacement for style 0 */
  /*MUIF_STYLE(6, mui_style_glasstown),*/
  
  /* style for menu entries */
  MUIF_STYLE(7, mui_style_menu_items),
  
  
  
  /* horizontal line (hrule) */
  MUIF_RO("HR", mui_hrule),

  /* read only fixed text */
  MUIF_U8G2_LABEL(),

  MUIF_RO("GP",mui_u8g2_goto_data),
  MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

  /* Form 10 */
  MUIF_GOTO(mui_u8g2_btn_goto_wm_fi),
  
  MUIF_BUTTON("G0", mui_u8g2_btn_goto_wm_fi),
  MUIF_BUTTON("G1", mui_u8g2_btn_goto_w1_fi),
  MUIF_BUTTON("G2", mui_u8g2_btn_goto_w1_pi),
  MUIF_BUTTON("G3", mui_u8g2_btn_goto_w2_fi),

  MUIF_BUTTON("G6", mui_u8g2_btn_goto_wm_if),

  /* Form 20 */
  MUIF_U8G2_U8_MIN_MAX("N0", &number_input9, 0, 9, mui_u8g2_u8_min_max_wm_mse_pi),
  MUIF_U8G2_U8_MIN_MAX("N1", &number_input99, 0, 99, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("N2", &number_input9, 0, 9, mui_u8g2_u8_min_max_wm_mse_pf),
  MUIF_U8G2_U8_MIN_MAX("N3", &number_input99, 0, 99, mui_u8g2_u8_min_max_wm_mud_pf),

  /* Form 30 */
  MUIF_VARIABLE("C1",&chkbox1_input,mui_u8g2_u8_chkbox_wm_pi),
  MUIF_VARIABLE("C2",&chkbox2_input,mui_u8g2_u8_chkbox_wm_pi),
  MUIF_VARIABLE("RB",&radio_input,mui_u8g2_u8_radio_wm_pi),

  /* Form 40: Cycle Option */
  MUIF_VARIABLE("O1",&fruit_input,mui_u8g2_u8_opt_line_wa_mse_pi),
  MUIF_VARIABLE("O2",&fruit_input,mui_u8g2_u8_opt_line_wa_mud_pi),
  MUIF_VARIABLE("O3",&fruit_input,mui_u8g2_u8_opt_line_wa_mse_pf),
  MUIF_VARIABLE("O4",&fruit_input,mui_u8g2_u8_opt_line_wa_mud_pf),


  /* Form 50: Parent/Child Option */
  MUIF_VARIABLE("P1",&color_input,mui_u8g2_u8_opt_parent_wm_pi),
  MUIF_VARIABLE("ca",&color_input,mui_u8g2_u8_opt_radio_child_w1_pi),
  
  MUIF_VARIABLE("P2",&food_input,mui_u8g2_u8_opt_parent_wm_pi),
  MUIF_VARIABLE("cf",&food_input,mui_u8g2_u8_opt_child_wm_pi),

  /* Form 60: Char Input */
  MUIF_VARIABLE("T0", text_input+0, mui_u8g2_u8_char_wm_mud_pi),
  MUIF_VARIABLE("T1", text_input+1, mui_u8g2_u8_char_wm_mud_pi),
  MUIF_VARIABLE("T2", text_input+2, mui_u8g2_u8_char_wm_mud_pi),
  MUIF_VARIABLE("T3", text_input+3, mui_u8g2_u8_char_wm_mud_pi),

  /* Form 70: One of many selection within the same field */
  MUIF_U8G2_U16_LIST("A1", &list_selection_1, NULL, animals_get_str, animals_get_cnt, mui_u8g2_u16_list_line_wa_mse_pi),
  MUIF_U8G2_U16_LIST("A2", &list_selection_2, NULL, animals_get_str, animals_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),

  MUIF_U8G2_U16_LIST("AP", &list_selection_3, NULL, animals_get_str, animals_get_cnt, mui_u8g2_u16_list_parent_wm_pi),
  MUIF_U8G2_U16_LIST("AC", &list_selection_3, NULL, animals_get_str, animals_get_cnt, mui_u8g2_u16_list_child_w1_pi),

  /* Form 90: bar */
  MUIF_U8G2_U8_MIN_MAX_STEP("B0", &number_input9, 0, 20, 2, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mse_pi),
  MUIF_U8G2_U8_MIN_MAX_STEP("B1", &number_input9, 0, 20, 2, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX_STEP("B2", &number_input9, 0, 20, 2, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mse_pf),
  MUIF_U8G2_U8_MIN_MAX_STEP("B3", &number_input9, 0, 20, 2, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mud_pf),


};


fds_t fds[] =

MUI_FORM(1)
MUI_STYLE(1)
MUI_LABEL(5,10, "MUI Elements")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
    MUI_10 "Goto Buttons|"
    MUI_20 "uint8 Number|"
    MUI_30 "uint8 Checkbox|"
    MUI_40 "uint8 Cycle Options|"
    MUI_50 "uint8 ParentChild Select|"
    MUI_60 "uint8 Char/Text Input|"
    MUI_70 "uint16 Cycle Options|"
    MUI_80 "uint16 ParentChild Select|"
    MUI_90 "uint8 Bar Graph|"
    MUI_18 "Parent/Child List|"
    MUI_20 "Array Edit|"
    MUI_16 "Goto Test")
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(10)
MUI_STYLE(1)
MUI_LABEL(5,10, "Goto Buttons")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
  MUI_11 "btn_goto_wm_fi|" 
  MUI_12 "btn_goto_w1_fi|" 
  MUI_13 "btn_goto_w1_pi|" 
  MUI_14 "btn_goto_w2_fi|"
  MUI_1 "Back to Main Menu" )
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(11)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_btn_goto_wm_fi")
MUI_LABEL(1,12, "MUIF_GOTO, MUIF_BUTTON")
MUI_LABEL(1,19, "MUI_GOTO, MUI_XYAT")
MUI_LABEL(1,25, "Centered button, jumps to form")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_GOTO(64, 42, 10, "Button")
MUI_GOTO(64, 59, 10, " Ok ")

MUI_FORM(12)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_btn_goto_w1_fi")
MUI_LABEL(1,12, "MUIF_GOTO, MUIF_BUTTON")
MUI_LABEL(1,19, "MUI_GOTO, MUI_XYAT")
MUI_LABEL(1,25, "Full width button with frame")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_XYAT("G1", 1, 42, 10, "Button")
MUI_GOTO(64, 59, 10, " Ok ")

MUI_FORM(13)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_btn_goto_w1_pi")
MUI_LABEL(1,12, "MUIF_GOTO, MUIF_BUTTON")
MUI_LABEL(1,19, "MUI_GOTO, MUI_XYAT")
MUI_LABEL(1,25, "Full width button without frame")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_XYAT("G2", 1, 42, 10, "Button")
MUI_GOTO(64, 59, 10, " Ok ")

MUI_FORM(14)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_btn_goto_w2_fi")
MUI_LABEL(1,12, "MUIF_GOTO, MUIF_BUTTON")
MUI_LABEL(1,19, "MUI_GOTO, MUI_XYAT")
MUI_LABEL(1,25, "Centered half display width button")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_XYAT("G3", 32, 42, 10, "Btn 1")
MUI_XYAT("G3", 96, 42, 10, "Btn 2")
MUI_GOTO(64, 59, 10, " Ok ")


MUI_FORM(16)
MUI_STYLE(0)
MUI_GOTO(64, 59, 1, " Back ")



MUI_FORM(20)
MUI_STYLE(1)
MUI_LABEL(5,10, "uint8 Number")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
  MUI_21 "u8_min_max_wm_mse_pi|" 
  MUI_22 "u8_min_max_wm_mud_pi|" 
  MUI_23 "u8_min_max_wm_mse_pf|" 
  MUI_24 "u8_min_max_wm_mud_pf|" 
  MUI_1 "Back to Main Menu" )
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(21)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_min_max_wm_mse_pi")
MUI_LABEL(1,12, "MUIF_U8G2_U8_MIN_MAX")
MUI_LABEL(1,19, "MUI_XY")
MUI_LABEL(1,25, "Input for uint8_t number")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(1,40, "Number: ")
MUI_XY("N0",70, 40)
MUI_GOTO(64, 59, 20, " Ok ")

MUI_FORM(22)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_min_max_wm_mud_pi")
MUI_LABEL(1,12, "MUIF_U8G2_U8_MIN_MAX")
MUI_LABEL(1,19, "MUI_XY")
MUI_LABEL(1,25, "Input for uint8_t number")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(1,40, "Number: ")
MUI_XY("N1",70, 40)
MUI_GOTO(64, 59, 20, " Ok ")

MUI_FORM(23)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_min_max_wm_mse_pf")
MUI_LABEL(1,12, "MUIF_U8G2_U8_MIN_MAX")
MUI_LABEL(1,19, "MUI_XY")
MUI_LABEL(1,25, "Input for uint8_t number")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(1,40, "Number: ")
MUI_XY("N2",70, 40)
MUI_GOTO(64, 59, 20, " Ok ")

MUI_FORM(24)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_min_max_wm_mud_pf")
MUI_LABEL(1,12, "MUIF_U8G2_U8_MIN_MAX")
MUI_LABEL(1,19, "MUI_XY")
MUI_LABEL(1,25, "Input for uint8_t number")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(1,40, "Number: ")
MUI_XY("N3",70, 40)
MUI_GOTO(64, 59, 20, " Ok ")




MUI_FORM(30)
MUI_STYLE(1)
MUI_LABEL(5,10, "uint8 Checkbox")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
  MUI_31 "u8_chkbox_wm_pi|" 
  MUI_32 "u8_radio_wm_pi|" 
  MUI_1 "Back to Main Menu" )
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(31)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_chkbox_wm_pi")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XY")
MUI_LABEL(1,25, "Checkbox, values 0 and 1, uint8_t)")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(1,40, "Checkbox 1: ")
MUI_XY("C1",66, 40)
MUI_LABEL(1,55, "Checkbox 2: ")
MUI_XY("C2",66, 55)
MUI_STYLE(3)
MUI_GOTO(110, 54, 30, "\x31")

MUI_FORM(32)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_radio_wm_pi")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYA, MUI_XYAT")
MUI_LABEL(1,25, "Radio buttons, assign arg to variable")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_XYAT("RB", 1, 40, 1, "Radio 1")
MUI_XYAT("RB", 1, 55, 2, "Radio 2")
MUI_STYLE(3)
MUI_GOTO(110, 54, 30, "\x31")


MUI_FORM(40)
MUI_STYLE(1)
MUI_LABEL(5,10, "uint8 Cycle Options")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
  MUI_41 "u8_opt_line_wa_mse_pi|" 
  MUI_42 "u8_opt_line_wa_mud_pi|" 
  MUI_43 "u8_opt_line_wa_mse_pf|" 
  MUI_44 "u8_opt_line_wa_mud_pf|" 
  MUI_1 "Back to Main Menu" )
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(41)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_opt_line_wa_mse_pi")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYAT")
MUI_LABEL(1,25, "Options forward cycle")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(5,40, "Fruit:")
MUI_XYAT("O1",60, 40, 60, "Banana|Apple|Melon|Cranberry")
MUI_STYLE(0)
//MUI_XYAT("G6", 64, 59, 40, " Ok ")
MUI_GOTO(64, 59, 40, " Ok ")


MUI_FORM(42)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_opt_line_wa_mud_pi")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYAT")
MUI_LABEL(1,25, "Options forward+backward cycle")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(5,40, "Fruit:")
MUI_XYAT("O2",60, 40, 60, "Banana|Apple|Melon|Cranberry")
MUI_STYLE(0)
//MUI_XYAT("G6", 64, 59, 40, " Ok ")
MUI_GOTO(64, 59, 40, " Ok ")

MUI_FORM(43)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_opt_line_wa_mse_pf")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYAT")
MUI_LABEL(1,25, "Options forward cycle")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(5,40, "Fruit:")
MUI_XYAT("O3",60, 40, 60, "Banana|Apple|Melon|Cranberry")
MUI_STYLE(0)
MUI_XYAT("G6", 64, 59, 40, " Ok ")
//MUI_GOTO(64, 59, 40, " Ok ")

MUI_FORM(44)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_opt_line_wa_mud_pf")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYAT")
MUI_LABEL(1,25, "Options forward+backward cycle")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(5,40, "Fruit:")
MUI_XYAT("O4",60, 40, 60, "Banana|Apple|Melon|Cranberry")
MUI_STYLE(0)
MUI_XYAT("G6", 64, 59, 40, " Ok ")
//MUI_GOTO(64, 59, 40, " Ok ")



MUI_FORM(50)
MUI_STYLE(1)
MUI_LABEL(5,10, "uint8 ParentChild Select")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
  MUI_51 "u8_opt_radio_child_w1_pi|" 
  MUI_53 "u8_opt_child_wm_pi|" 
  MUI_1 "Back to Main Menu" )
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
//MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(51)
MUI_STYLE(2)
MUI_LABEL(1,5, "..._u8_opt_parent_wm_pi")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYAT")
MUI_LABEL(1,25, "Parent: Show selected option")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(5,40, "Color:")
MUI_XYAT("P1",60, 40, 52, "red|orange|yellow|green|cyan|azure|blue|violet|magenta|rose")
MUI_STYLE(0)
MUI_GOTO(64, 59, 50, " Ok ")

MUI_FORM(52)
MUI_STYLE(2)
MUI_LABEL(1,5, "..._u8_opt_radio_child_w1_pi")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYA")
MUI_LABEL(1,25, "Child: Show selectable options")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_XYA("ca", 5, 38, 0) 
MUI_XYA("ca", 5, 49, 1) 
MUI_XYA("ca", 5, 60, 2) 

MUI_FORM(53)
MUI_STYLE(2)
MUI_LABEL(1,5, "..._u8_opt_parent_wm_pi")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYAT")
MUI_LABEL(1,25, "Parent: Show selected option")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(15,42, "We need:")
MUI_STYLE(4)
MUI_XYAT("P2",80, 49, 54, "\x30|\x31|\x32|\x33|\x34|\x35|\x36|\x37|\x38|\x39|\x40|\x41|\x42|\x43")
MUI_STYLE(0)
MUI_GOTO(64, 59, 50, " Ok ")

MUI_FORM(54)
MUI_STYLE(2)
MUI_LABEL(1,5, "..._u8_opt_child_wm_pi")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYA")
MUI_LABEL(1,25, "Child: Show selectable options")
MUI_XY("HR", 0,26)
MUI_STYLE(4)
MUI_XYA("cf", 3, 53, 0)
MUI_XYA("cf", 28, 53, 1)
MUI_XYA("cf", 53, 53, 2)
MUI_XYA("cf", 78, 53, 3)
MUI_XYA("cf", 103, 53, 4)


MUI_FORM(60)
MUI_STYLE(1)
MUI_LABEL(5,10, "uint8 Char/Text Input")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
  MUI_61 "u8_char_wm_mud_pi|" 
  MUI_1 "Back to Main Menu" )
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(61)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_char_wm_mud_pi")
MUI_LABEL(1,12, "MUIF_VARIABLE")
MUI_LABEL(1,19, "MUI_XYAT")
MUI_LABEL(1,25, "4x Char Input")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(5,40, "Text:")
MUI_STYLE(5)
MUI_XY("T0",40, 40)
MUI_XY("T1",48, 40)
MUI_XY("T2",56, 40)
MUI_XY("T3",64, 40)
MUI_STYLE(0)
MUI_GOTO(64, 59, 60, " Ok ")





MUI_FORM(70)
MUI_STYLE(1)
MUI_LABEL(5,10, "uint16 Cycle Options")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
  MUI_71 "u16_list_line_wa_mse_pi|" 
  MUI_72 "u16_list_line_wa_mud_pi|" 
  MUI_1 "Back to Main Menu" )
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(71)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u16_list_line_wa_mse_pi")
MUI_LABEL(1,12, "MUIF_U8G2_U16_LIST")
MUI_LABEL(1,19, "MUI_XYA")
MUI_LABEL(1,25, "Cycle through options")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(5,40, "Animal:")
MUI_XYA("A1",45, 40, 50)
MUI_GOTO(64, 59, 70, " Ok ")

MUI_FORM(72)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u16_list_line_wa_mud_pi")
MUI_LABEL(1,12, "MUIF_U8G2_U16_LIST")
MUI_LABEL(1,19, "MUI_XYA")
MUI_LABEL(1,25, "Cycle through options")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(5,40, "Animal:")
MUI_XYA("A2",45, 40, 50)
MUI_GOTO(64, 59, 70, " Ok ")









//uint8_t mui_u8g2_u16_list_parent_wm_pi(mui_t *ui, uint8_t msg);     /* MUIF_U8G2_U16_LIST, MUI_XYA, arg=subform */
//uint8_t mui_u8g2_u16_list_child_w1_pi(mui_t *ui, uint8_t msg);      /* MUIF_U8G2_U16_LIST, MUI_XYA, arg=sub element number */


MUI_FORM(80)
MUI_STYLE(1)
MUI_LABEL(5,10, "uint16 ParentChild Select")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
  MUI_81 "u16_list_parent_wm_pi|" 
  MUI_1 "Back to Main Menu" )
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
//MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(81)
MUI_STYLE(2)
MUI_LABEL(1,5, "..._u16_list_parent_wm_pi")
MUI_LABEL(1,12, "MUIF_U8G2_U16_LIST")
MUI_LABEL(1,19, "MUI_XYA")
MUI_LABEL(1,25, "Parent: Show selected option")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(5,40, "Animal:")
MUI_XYA("AP",50, 40, 82)     /* jump to sub form 82 */
MUI_GOTO(64, 59, 80, " Ok ")

MUI_FORM(82)
MUI_STYLE(2)
MUI_LABEL(1,5, "...u16_list_child_w1_pi")
MUI_LABEL(1,12, "MUIF_U8G2_U16_LIST")
MUI_LABEL(1,19, "MUI_XYA")
MUI_LABEL(1,25, "Child: Show selectable options")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_XYA("AC", 5, 38, 0) 
MUI_XYA("AC", 5, 49, 1) 
MUI_XYA("AC", 5, 60, 2) 


MUI_FORM(90)
MUI_STYLE(1)
MUI_LABEL(5,10, "uint8 Bar Graph")
MUI_XY("HR", 0,13)
MUI_STYLE(7)
MUI_DATA("GP", 
  MUI_91 "u8_bar_wm_mse_pi|" 
  MUI_92 "u8_bar_wm_mud_pi|" 
  MUI_93 "u8_bar_wm_mse_pf|" 
  MUI_94 "u8_bar_wm_mud_pf|" 
  MUI_1 "Back to Main Menu" )
MUI_XYA("GC", 5, 25, 0) 
MUI_XYA("GC", 5, 37, 1) 
MUI_XYA("GC", 5, 49, 2) 
MUI_XYA("GC", 5, 61, 3) 

MUI_FORM(91)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_bar_wm_mse_pi")
MUI_LABEL(1,12, "MUIF_U8G2_U8_MIN_MAX_STEP")
MUI_LABEL(1,19, "MUI_XY")
MUI_LABEL(1,25, "uint8_t bar graph input")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(1,40, "Bar: ")
MUI_XY("B0",40, 40)
MUI_GOTO(64, 59, 90, " Ok ")

MUI_FORM(92)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_bar_wm_mud_pi")
MUI_LABEL(1,12, "MUIF_U8G2_U8_MIN_MAX_STEP")
MUI_LABEL(1,19, "MUI_XY")
MUI_LABEL(1,25, "uint8_t bar graph input")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(1,40, "Bar: ")
MUI_XY("B1",40, 40)
MUI_GOTO(64, 59, 90, " Ok ")

MUI_FORM(93)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_bar_wm_mse_pf")
MUI_LABEL(1,12, "MUIF_U8G2_U8_MIN_MAX_STEP")
MUI_LABEL(1,19, "MUI_XY")
MUI_LABEL(1,25, "uint8_t bar graph input")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(1,40, "Bar: ")
MUI_XY("B2",40, 40)
MUI_GOTO(64, 59, 90, " Ok ")

MUI_FORM(94)
MUI_STYLE(2)
MUI_LABEL(1,5, "mui_u8g2_u8_bar_wm_mud_pf")
MUI_LABEL(1,12, "MUIF_U8G2_U8_MIN_MAX_STEP")
MUI_LABEL(1,19, "MUI_XY")
MUI_LABEL(1,25, "uint8_t bar graph input")
MUI_XY("HR", 0,26)
MUI_STYLE(0)
MUI_LABEL(1,40, "Bar: ")
MUI_XY("B3",40, 40)
MUI_GOTO(64, 59, 90, " Ok ")

;


int screenshot_n = 0;

void do_screenshot(void)
{
  char s[4096];
  u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2),  "screenshot.tga");
  sprintf(  s, 
  "convert -border 4 -bordercolor 'rgb(255,190,40)'"
  " -fill 'rgb(255,170,0)' -opaque white"
  "  -filter point -resize 200%%" 
  " screenshot.tga pic%04d.png", screenshot_n);
  system(s);
  screenshot_n++;
  /*
  gif animation:
  convert -delay 40 -loop 0 pic*.png animation.gif
  */
}

char key_queue[1024] = "";
int key_pos = 0;

int get_key(void)
{
  int k;
  
  if ( key_queue[key_pos] != '\0' )
  {
    k = key_queue[key_pos++];
  }
  else
  {
    k = u8g_sdl_get_key();
  }
  return k;
}

void set_key_queue(const char *s)
{
  strcpy(key_queue, s);
  key_pos = 0;  
}

int draw(int form, const char *keys, const char *gif)
{
  int k;
  int is_screenshot_enabled = 0;

  mui_GotoForm(&ui, form, 0);
  set_key_queue(keys);

  for(;;)
  {
    u8g2_SetFontRefHeightExtendedText(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      mui_Draw(&ui);
    } while( u8g2_NextPage(&u8g2) );
    if ( is_screenshot_enabled )
    {
      do_screenshot();
    } 
    // printf("mui_GetCurrentCursorFocusPosition=%d\n", mui_GetCurrentCursorFocusPosition(&ui));
    
    do
    {
      k = get_key();
    } while( k < 0 );
    printf("%c", k);
    
    /*
    if ( k == 'e' ) y -= 1;
    if ( k == 'x' ) y += 1;
    if ( k == 's' ) x -= 1;
    if ( k == 'd' ) x += 1;
    */
    
    if ( k == 'q' ) break;

    if ( k == 'n' ) 
    {
      mui_NextField(&ui);
    }
    if ( k == 'p' ) 
    {
      mui_PrevField(&ui);
    }
    if ( k == 's' )
    {
      mui_SendSelect(&ui);
    }

    if ( k == 't' ) 
    {
      puts("screenshot");
      do_screenshot();
    }
    
    if ( k == 'a' )     // start animation
    {
      screenshot_n = 0;
      system("rm pic*.png");
      is_screenshot_enabled  = 1;
    }
    
    if ( k == '.' )
    {
      char sys[1024];
      is_screenshot_enabled  = 0;
      sprintf(sys, "convert -delay 100 -loop 0 pic*.png %s", gif);
      system(sys);
    }
    
    if ( k == 'x' )
    {
      mui_GotoForm(&ui, 10, 0);
      set_key_queue("asnnsnsnnsnnsnnns.");
    }
  }
  return 0;
}

int main(void)
{
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */
  
  u8g2_SetFontMode(&u8g2, 1);
  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui_GotoForm(&ui, 1, 0);

  draw(1, "", ""); return 0;
  
  /*
  
    a: start animation
    .: end and write animation
    goto form (first argument) then press a to see where the animation has to start
  */
  /*
  draw(10, "asnnnsnsnnnsnnsnnnsnnnsnnnnns.q", "mui_btn_goto.gif");
  draw(20, "assssnsnssnnnnnnsnsnnssppssssssnsnnnssnpppppppsns.q", "mui_uint8_number.gif");
  draw(30, "assnsnsnssnsnsssnsns.q", "mui_uint8_checkbox_radio.gif");
  draw(40, "asnnssnsnssppsnsnnsnnsssnsnnnssnnsns.q", "mui_u8g2_u8_opt_line_wa.gif");
  draw(50, "assnnnnsnnspppsnsnssnnnsnnsnnnnnnnsns.q", "mui_u8g2_u8_opt_parent.gif");
  draw(60, "assppppppsnsnnnnnnnnnsnsppppppppppppppppppppsnsnnnsns.q", "mui_u8g2_u8_char_wm_mud_pi.gif");
  draw(70, "assssssnsnssnnnnnnsnnspsns.q", "mui_u8g2_u16_list_line.gif");
  draw(80, "assnnssnnnssoppppppnsns.q", "mui_u8g2_u16_list_child.gif");
  draw(90, "assssnsnssnnpsnsnnsssnnsnsnnnsspppnsns.q", "mui_u8g2_u8_bar.gif");
  */

  draw(10, "asnnnsnsnnnsnsnnnsnsnnnnns.q", "mui_btn_goto.gif");
  draw(20, "assssnsnssnnnnnnsnsnssppssssssnsnssnpppppppsns.q", "mui_uint8_number.gif");
  draw(30, "assnsnsnssnsnsssnsns.q", "mui_uint8_checkbox_radio.gif");
  draw(40, "assssspsnssnnnnnppsnsnssssnsnssppsns.q", "mui_u8g2_u8_opt_line_wa.gif");
  draw(50, "assnnnnsnnspppsnsnssnnnsnnsnnnnnnnsns.q", "mui_u8g2_u8_opt_parent.gif");
  draw(60, "assppppppsnsnnnnnnnnnsnsppppppppppppppppppppsnsnnnsns.q", "mui_u8g2_u8_char_wm_mud_pi.gif");
  draw(70, "assssssnsnssnnnnnnsnnspsns.q", "mui_u8g2_u16_list_line.gif");
  draw(80, "assnnssnnnssoppppppnsns.q", "mui_u8g2_u16_list_child.gif");
  draw(90, "assssnsnssnnpsnsnsssnnsnsnsspppnsns.q", "mui_u8g2_u8_bar.gif");



  
  
  return 0;
}

