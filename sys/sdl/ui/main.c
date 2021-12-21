
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
MUIF("I1",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value,mui_u8g2_u8_value_0_9_wm_mse_pi),
MUIF("I2",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value2,mui_u8g2_u8_opt_line_wa_mse_pi),
MUIF("I3",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value3,mui_u8g2_u8_chkbox_wm_pi),
MUIF("I4",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_color,mui_u8g2_u8_opt_parent_wa_mse_pi),

MUIF("O4",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&my_color,mui_u8g2_u8_opt_child_wm_mse_pi),
  
MUIF("TS",MUIF_CFLAG_IS_CURSOR_SELECTABLE,&touchscreen_selection,mui_u8g2_u8_opt_line_wa_mse_pi),
MUIF(".G",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_u8g2_btn_jmp_w2_fi),    /* MUI_GOTO has the id FG */
MUIF(".g",MUIF_CFLAG_IS_CURSOR_SELECTABLE,0,mui_u8g2_btn_jmp_w1_fi),    /* MUI_goto has the id Fg */
MUIF(".L",0,0,mui_u8g2_draw_text)
};

fds_t *fds = 

MUI_FORM(1)
MUI_goto(12,10,2, "Test 1")
MUI_goto(12,22,10, "Button Test")
MUI_goto(12,34,20, "Button Test 2")
MUI_goto(12,46,101, "Story")

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

MUI_FORM(10)
MUI_goto(10,11,11, "Btn 1")
MUI_goto(10,21,12, "Btn 2")
MUI_goto(10,31,13, "Btn 3")
MUI_goto(10,41,14, "Btn 4")
MUI_goto(10,51,15, "Btn 5")
MUI_goto(10,61,1, "Home")

MUI_FORM(11)
MUI_goto(10,61,10, "Back")
MUI_FORM(12)
MUI_goto(10,61,10, "Back")
MUI_FORM(13)
MUI_goto(10,61,10, "Back")
MUI_FORM(14)
MUI_goto(10,61,10, "Back")
MUI_FORM(15)
MUI_goto(10,61,10, "Back")
MUI_FORM(16)
MUI_goto(10,61,10, "Back")


MUI_FORM(20)
MUI_goto(10,61,10, "Back")

MUI_FORM(101)
MUI_LABEL(0, 10, "Robot enabled.")
MUI_LABEL(0, 20, "KI activated.")
MUI_GOTO(32,60,102, "Move Out")
MUI_GOTO(96,60,3, "Diagnostics")

MUI_FORM(102)
MUI_LABEL(0, 9, "Floor.")
MUI_LABEL(0, 19, "Robot holder in the wall.")
MUI_LABEL(0, 29, "")
MUI_goto(10,41,107, "Forward")
MUI_goto(10,51,103, "Backward")
MUI_goto(10,61,101, "Into Holder")

MUI_FORM(103)
MUI_LABEL(0, 9, "Floor.")
MUI_LABEL(0, 19, "")
MUI_LABEL(0, 29, "")
MUI_goto(10,41,102, "Forward")
MUI_goto(10,51,104, "Visual Scan")
MUI_goto(10,61,106, "Audio Scan")

MUI_FORM(104)
MUI_LABEL(0, 9, "Visual scan mode.")
MUI_LABEL(0, 19, "Floor to front.")
MUI_LABEL(0, 29, "Backside Window.")
//MUI_goto(10,41,102, "Forward")
MUI_goto(10,51,105, "Scan Window")
MUI_goto(10,61,103, "Exit Scan")

MUI_FORM(105)
MUI_LABEL(0, 9, "Visual scan mode.")
MUI_LABEL(0, 19, "Window:")
MUI_LABEL(0, 29, "Black with white dots.")
//MUI_goto(10,41,102, "Forward")
MUI_goto(10,51,104, "Visual Scan")
MUI_goto(10,61,103, "Exit Scan")

MUI_FORM(106)
MUI_LABEL(0, 9, "Audio scan mode.")
MUI_LABEL(0, 19, "No audio.")
//MUI_LABEL(0, 29, "")
//MUI_goto(10,41,102, "Forward")
//MUI_goto(10,51,104, "Visual Scan")
MUI_goto(10,61,103, "Exit Scan")

MUI_FORM(107)
MUI_LABEL(0, 9, "Floor.")
MUI_LABEL(0, 19, "")
//MUI_LABEL(0, 29, "")
//MUI_goto(10,41,102, "Forward")
MUI_goto(10,51,108, "Visual Scan")
MUI_goto(10,61,102, "Backward")

MUI_FORM(108)
MUI_LABEL(0, 9, "Visual scan mode.")
MUI_LABEL(0, 19, "Touchscreen.")
MUI_LABEL(0, 29, "Door with button.")
//MUI_LABEL(0, 29, "")
MUI_goto(10,41,109, "Press Door Button")
MUI_goto(10,51,110, "Use Touchscreen")
MUI_goto(10,61,107, "Exit Scan")

MUI_FORM(109)
MUI_LABEL(0, 9, "Visual scan mode.")
MUI_LABEL(0, 19, "Door does not open.")
//MUI_LABEL(0, 29, "")
//MUI_goto(10,41,109, "Press Door Button")
//MUI_goto(10,51,110, "Use Touchscreen")
MUI_goto(10,61,107, "Exit Scan")

MUI_FORM(110)
MUI_LABEL(0, 9, "Touchscreen.")
MUI_XYT("TS",0, 25, "Navigation|Energy|Damage|Robot|Security")
MUI_goto(10,61,108, "Exit Touchscreen")

;


int main(void)
{
  int x, y;
  int k;

  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  
  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui_GotoForm(&ui, 1, 0);

  puts(fds);
  
  
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

      if ( ui.current_form_fds[1] == 11 )
      {
        u8g2_DrawButtonUTF8(&u8g2, 64, 9, /*flags*/U8G2_BTN_HCENTER, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0");
        u8g2_DrawButtonUTF8(&u8g2, 64, 22, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0 Inv");
        u8g2_DrawButtonUTF8(&u8g2, 64, 35, /*flags*/U8G2_BTN_HCENTER, /*w*/ 128, /*ph*/ 0,  /*pv*/0, "Center W128");
        u8g2_DrawButtonUTF8(&u8g2, 64, 48, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV, /*w*/ 128, /*ph*/ 0,  /*pv*/0, "Center W128 Inv");
      }
      if ( ui.current_form_fds[1] == 12 )
      {
        u8g2_DrawButtonUTF8(&u8g2, 64, 9, /*flags*/U8G2_BTN_HCENTER|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0");
        u8g2_DrawButtonUTF8(&u8g2, 64, 22, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0 Inv");
        u8g2_DrawButtonUTF8(&u8g2, 64, 35, /*flags*/U8G2_BTN_HCENTER|1, /*w*/ 126, /*ph*/ 0,  /*pv*/0, "Center W128");
        u8g2_DrawButtonUTF8(&u8g2, 64, 48, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 126, /*ph*/ 0,  /*pv*/0, "Center W128 Inv");
      }

      if ( ui.current_form_fds[1] == 13 )
      {
        u8g2_uint_t w = u8g2_GetStrWidth(&u8g2, "C");
        u8g2_uint_t h = u8g2_GetAscent(&u8g2)-u8g2_GetDescent(&u8g2);
        u8g2_uint_t x = 5;
        u8g2_uint_t y = 9;
        u8g2_DrawStr(&u8g2, x, y, "C");
        u8g2_DrawFrame(&u8g2, x-1,y-u8g2_GetAscent(&u8g2)-1, w+2, h+2);
        
        u8g2_DrawButtonUTF8(&u8g2, 64, 9, /*flags*/U8G2_BTN_HCENTER|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0");
        u8g2_DrawButtonUTF8(&u8g2, 64, 22, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0 Inv");
        
        u8g2_DrawButtonUTF8(&u8g2, 10, 35, /*flags*/U8G2_BTN_HCENTER|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "v");
        u8g2_DrawButtonUTF8(&u8g2, 10, 48, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "v");
        u8g2_DrawButtonUTF8(&u8g2, 24, 35, /*flags*/U8G2_BTN_HCENTER|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "vd");
        u8g2_DrawButtonUTF8(&u8g2, 24, 48, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "vd");
        u8g2_DrawButtonUTF8(&u8g2, 40, 35, /*flags*/U8G2_BTN_HCENTER|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "B");
        u8g2_DrawButtonUTF8(&u8g2, 40, 48, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "B");
        u8g2_DrawButtonUTF8(&u8g2, 60, 35, /*flags*/U8G2_BTN_HCENTER|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "A");
        u8g2_DrawButtonUTF8(&u8g2, 60, 48, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "A");
        u8g2_DrawButtonUTF8(&u8g2, 80, 35, /*flags*/U8G2_BTN_HCENTER|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Ab");
        u8g2_DrawButtonUTF8(&u8g2, 80, 48, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Ab");
        u8g2_DrawButtonUTF8(&u8g2, 100, 35, /*flags*/U8G2_BTN_HCENTER|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Abg");
        u8g2_DrawButtonUTF8(&u8g2, 100, 48, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Abg");
      }

      if ( ui.current_form_fds[1] == 14 )
      {
        u8g2_DrawButtonUTF8(&u8g2, 64, 9, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_SHADOW0|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0");
        u8g2_DrawButtonUTF8(&u8g2, 64, 22, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|1, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0 Inv");
        u8g2_DrawButtonUTF8(&u8g2, 64, 35, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_SHADOW0|1, /*w*/ 124, /*ph*/ 0,  /*pv*/0, "Center W128");
        u8g2_DrawButtonUTF8(&u8g2, 64, 48, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_INV|U8G2_BTN_SHADOW0|1, /*w*/ 124, /*ph*/ 0,  /*pv*/0, "Center W128 Inv");        
      }

      if ( ui.current_form_fds[1] == 15 )
      {
        u8g2_uint_t x = 24;
        u8g2_DrawButtonUTF8(&u8g2, 8-8, 9, /*flags*/U8G2_BTN_INV, /*w*/ 112+8*2, /*ph*/ 0,  /*pv*/0, "Line");
        u8g2_DrawButtonUTF8(&u8g2, 8-4, 22, /*flags*/U8G2_BTN_INV, /*w*/ 112+4*2, /*ph*/ 4,  /*pv*/0, "Line");
        u8g2_DrawButtonUTF8(&u8g2, x, 35, /*flags*/U8G2_BTN_INV, /*w*/ 128-2*x, /*ph*/ x,  /*pv*/0, "Line");
      }

      if ( ui.current_form_fds[1] == 20 )
      {
        u8g2_DrawButtonUTF8(&u8g2, 64, 12, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_XFRAME, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0");
        u8g2_DrawButtonUTF8(&u8g2, 64, 28, /*flags*/U8G2_BTN_HCENTER|U8G2_BTN_XFRAME|U8G2_BTN_INV, /*w*/ 0, /*ph*/ 0,  /*pv*/0, "Center W0 Inv");
      }
      
      //u8g2_DrawBox(&u8g2, 10, 10+y, 20, x);
      //u8g2_DrawFrame(&u8g2, 34, 10+y, 20, x);
      //u8g2_DrawStr(&u8g2, 10, 50, "XY");
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

