
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


uint16_t menu_get_cnt(void *data) {
  return 10;    /* number of menu entries */
}

const char *menu_get_str(void *data, uint16_t index) {
  static const char *menu[] = { 
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
  
uint16_t selection = 0;

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

uint8_t my_u16_list_goto_w1_pi(mui_t *ui, uint8_t msg)
{
  uint8_t arg = ui->arg; /* arg provided with MUI_XYA() */
  switch(msg)
  {
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
          /* no wrap around to the top */
          return 1;
          /*
          ui->form_scroll_top = 0;
          */
        }
      }
      break;
    case MUIF_MSG_EVENT_PREV:
      if ( arg == 0 ) /* only if we are at the topmost field */
      {
        if ( ui->form_scroll_top > 0 )
        {
          ui->form_scroll_top--;
          return 1;
        }
        else
        {
          /* no wrap around to the button */
          return 1;
          /*
          if ( ui->form_scroll_total >  ui->form_scroll_visible  )
          {
            ui->form_scroll_top = ui->form_scroll_total - ui->form_scroll_visible;
          }
          else
          {
            ui->form_scroll_top = 0;
          }
          */
        }
      }
      break;
    default:
      return mui_u8g2_u16_list_goto_w1_pi(ui, msg);
  }
  return 0;
}

muif_t muif_list[]  MUI_PROGMEM = {  
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),
  MUIF_LABEL(mui_u8g2_draw_text),  
  /* horizontal line (hrule) */
  MUIF_RO("HR", mui_hrule),
  
  MUIF_U8G2_U16_LIST("ID", &selection, NULL, menu_get_str, menu_get_cnt, my_u16_list_goto_w1_pi)
};


fds_t fds[] MUI_PROGMEM  =
MUI_FORM(1)
MUI_STYLE(0)
MUI_LABEL(5,10, "Issue 2597")
MUI_XY("HR", 0, 13)
MUI_XYA("ID", 5, 25, 0) 
MUI_XYA("ID", 5, 37, 1) 
MUI_XYA("ID", 5, 49, 2) 
MUI_XYA("ID", 5, 61, 3) 
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
  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list)/sizeof(muif_t));
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

