
#include "u8g2.h"
#include <stdio.h>
#include <string.h>
#include "mui.h"
#include "mui_u8g2.h"

  
u8g2_t u8g2;
mui_t ui;

uint8_t my_value = 2;
uint8_t my_value2 = 2;
uint8_t my_value3 = 0;
uint8_t my_color = 0;

uint8_t touchscreen_selection = 255;

uint8_t muif_tsingle_line_option_invers_select_u8g2(mui_t *ui, uint8_t msg);


muif_t muif_list[] = {
  MUIF("B0",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_half_width_frame_button_invers_select_u8g2),
  MUIF("B1",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_half_width_frame_button_invers_select_u8g2),
  MUIF("I1",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value,mui_input_uint8_invers_select_u8g2),
  MUIF("I2",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value2,mui_single_line_option_invers_select_u8g2),
  MUIF("I3",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value3,mui_checkbox_invers_select_u8g2),
  MUIF("I4",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_color,mui_show_option_goto_form_invers_select_u8g2),

  MUIF("O4",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_color,mui_assign_arg_go_back_invers_select_u8g2),
    
  MUIF("TS",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&touchscreen_selection,mui_single_line_option_invers_select_u8g2),
  MUIF(".G",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_goto_half_width_frame_button_invers_select_u8g2),    /* MUI_GOTO has the id FG */
  MUIF(".g",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_goto_line_button_invers_select_u8g2),    /* MUI_goto has the id Fg */
  MUIF(".L",0,0,mui_label_u8g2)
};

fds_t fds = 

MUI_FORM(1)
MUI_goto(12,10,2, "Test 1")
//MUI_goto(12,22,10, "Button Test")
//MUI_goto(12,34,101, "Story")

MUI_FORM(2)
MUI_LABEL(0,10, "Number:")
MUI_XY("I1",50, 10)
MUI_LABEL(0,22, "Fruit:")
MUI_XYAT("I2",50, 22, 60, "Banana|Apple|Peach")
MUI_LABEL(0,34, "Check:")
//MUI_XYT("I3",50, 44, "ABCxyz")
MUI_XY("I3",50, 34)

MUI_LABEL(0,46, "Color:")
MUI_XYAT("I4",50, 46, 4, "red|green|blue")


//MUI_BUTTON("B1",32,50, "Ok")
MUI_GOTO(32,60,3, "Goto 2")
MUI_GOTO(96,60,1, "Home")

MUI_FORM(3)
MUI_goto(32,50,2, "Goto 1")

MUI_FORM(4)
MUI_XYA("O4", 2, 10, 0)
MUI_XYA("O4", 2, 22, 1)
MUI_XYAT("O4", 2, 34, 2, "blue color")  /* provide a different text for this option */


;


int main(void)
{
  int x, y;
  int k;

  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  
  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui_EnterForm(&ui, 0);

  //puts(fds);
  
  
  x = 4;  // use as height for the box
  y = 0;
  
  for(;;)
  {
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);      
      u8g2_SetFont(&u8g2, u8g2_font_inb16_mf  );      
      u8g2_SetFont(&u8g2, u8g2_font_courB12_tr);      
      u8g2_SetFont(&u8g2, u8g2_font_timR08_tr); 
      u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);
      u8g2_SetFontMode(&u8g2, 1);
      //u8g2_SetFontPosTop(&u8g2);
      //u8g2_SetFontPosBottom(&u8g2);
      //u8g2_SetFontPosCenter(&u8g2);
      u8g2_SetFontRefHeightExtendedText(&u8g2);

      
      mui_Draw(&ui);
    } while( u8g2_NextPage(&u8g2) );
    
    // printf("mui_GetCurrentCursorFocusPosition=%d\n", mui_GetCurrentCursorFocusPosition(&ui));
    
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 273 ) y -= 1;
    if ( k == 274 ) y += 1;
    if ( k == 276 ) x -= 1;
    if ( k == 275 ) x += 1;
    
    /*
    if ( k == 'e' ) y -= 1;
    if ( k == 'x' ) y += 1;
    if ( k == 's' ) x -= 1;
    if ( k == 'd' ) x += 1;
    */
    
    if ( k == 'q' ) break;

    if ( k == 'n' ) mui_NextField(&ui);
    if ( k == 'p' ) mui_PrevField(&ui);
    if ( k == 's' ) mui_SendSelect(&ui);

    
    if ( x < 0 )
      x = 0;
    
  }
  return 0;
}

