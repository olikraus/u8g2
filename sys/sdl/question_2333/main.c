
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

uint8_t num_value = 0;
uint8_t bar_value = 0;
uint8_t TestModeRadioChoice = 0;
long LastTime;


uint16_t menu_get_cnt(void *data) {
  return 7;    /* number of menu entries */
}

uint16_t menu_get_cnt2(void *data) {
  return 5;    /* number of menu entries */
}

const char *menu_get_str(void *data, uint16_t index) {
  static const char *menu[] = { 
    MUI_11 "Device 1",
    MUI_11 "Device 2",
    MUI_11 "Device 3",
    MUI_11 "Device 4",
    MUI_11 "Device 5",
    MUI_11 "Device 6"
  };
  return menu[index];
}

const char *menuAC_get_str(void *data, uint16_t index) {
  static const char *menu[] = { 
    MUI_21 "Test",
    MUI_22 "Battery",
    MUI_23 "Timer",
    MUI_24 "Activation",
    MUI_1 "Back..",
  };
  return menu[index];
}

uint16_t selection = 0, selection2 = 0;

uint8_t mui_hrule(mui_t *ui, uint8_t msg) {
  if (msg == MUIF_MSG_DRAW) {
    u8g2_DrawHLine(&u8g2, 0, mui_get_y(ui), 128);
    //Serial.println("drawHLine " + String(selection + 1));

    //Serial.println(String(selection2));

    selection2 = 0;
  }
  return 0;
}

uint8_t mui_draw_current_device(mui_t *ui, uint8_t msg) {
  if (msg == MUIF_MSG_DRAW) {
    //u8g2_SetCursor(mui_get_x(ui ), mui_get_y(ui));
    u8g2_DrawStr(&u8g2, mui_get_x(ui ), mui_get_y(ui ), "Device: ??");
    
    //u8g2.print("Device: " + String(selection + 1));

  }
  return 0;
}

//forced exit
uint8_t mui_go_main_menu(mui_t *ui, uint8_t msg) {
  if (msg == MUIF_MSG_FORM_START) {
    mui_SendSelectWithExecuteOnSelectFieldSearch(ui);

    //Serial.println("mui_go_main_menu");
  }
  return 0;
}

muif_t muif_list[] = {
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_6x12_t_cyrillic), /* regular font */
  MUIF_U8G2_FONT_STYLE(1, u8g2_font_6x12_t_cyrillic), /* bold font */

  MUIF_VARIABLE("TM",&TestModeRadioChoice,mui_u8g2_u8_chkbox_wm_pi),

  MUIF_RO("HR", mui_hrule),
  MUIF_U8G2_LABEL(),
  MUIF_RO("GP", mui_u8g2_goto_data),
  MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

  MUIF_U8G2_U16_LIST("ID", &selection, NULL, menu_get_str, menu_get_cnt, mui_u8g2_u16_list_goto_w1_pi),

  MUIF_U8G2_U16_LIST("AC", &selection2, NULL, menuAC_get_str, menu_get_cnt2, mui_u8g2_u16_list_goto_w1_pi),

  MUIF_RO("DD", mui_draw_current_device),

  MUIF_RO("GG", mui_go_main_menu),

  /* a button for the menu... */
  MUIF_EXECUTE_ON_SELECT_BUTTON("ST", mui_u8g2_btn_exit_wm_fi)  
  //MUIF_BUTTON("ST", mui_u8g2_btn_exit_wm_fi)   
};

fds_t fds_data[] =

  MUI_FORM(1)
  MUI_STYLE(1)
  MUI_LABEL(5, 10, "Choose:")
  MUI_XY("HR", 0,11)

  MUI_XYA("ID", 5, 24, 0)
  MUI_XYA("ID", 5, 36, 1)
  MUI_XYA("ID", 5, 48, 2)
  MUI_XYA("ID", 5, 60, 3)


  MUI_FORM(11)
  MUI_STYLE(1)
  MUI_XY("HR", 0,11)
  MUI_XY("DD", 5, 10)

  MUI_XYA("AC", 5, 24, 0)
  MUI_XYA("AC", 5, 36, 1)                                      
  MUI_XYA("AC", 5, 48, 2)
  MUI_XYA("AC", 5, 60, 3)

  MUI_FORM(25)
  MUI_STYLE(1)
  MUI_XYT("ST", 5, 24, "Exit")
  MUI_AUX("GG") 
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

void clearFormPositionCache( mui_t *mui, uint8_t form_id )
{
  uint8_t i;
  for( i = 0; i < MUI_MENU_CACHE_CNT; i++ )
    if ( mui->menu_form_id[i] == form_id )
      mui->menu_form_cursor_focus_position[i] = 0;
}

int main(void)
{
  int x, y;
  int k;

  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */

  u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
  u8g2_SetFontMode(&u8g2, 1);
  mui_Init(&ui, &u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui_GotoForm(&ui, 1, 0);

  
  x = 4;  // use as height for the box
  y = 0;
  
  for(;;)
  {
    u8g2_SetFontRefHeightExtendedText(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      mui_Draw(&ui);
    } while( u8g2_NextPage(&u8g2) );
    do_screenshot();
    
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
      clearFormPositionCache( &ui, 11 );
      mui_SendSelect(&ui);
    }

    if ( k == 't' ) 
    {
      puts("screenshot");
      do_screenshot();
    }
    
    if ( x < 0 )
      x = 0;
    
  }
  return 0;
}

