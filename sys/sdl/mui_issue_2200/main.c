
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

uint8_t var1;
uint8_t var2;
uint8_t var3;
uint8_t var4;

muif_t muif_list[]  MUI_PROGMEM = {  
  MUIF_U8G2_LABEL(),
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_5x8_tr),
  
  // MUIF_U8G2_U8_MIN_MAX_STEP("B1", /*valptr=*/ &var1, /*min=*/ 10 , /*max=*/ 20, /*step=*/ 2, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mse_pi),
  // MUIF_U8G2_U8_MIN_MAX_STEP("B2", /*valptr=*/ &var2, /*min=*/ 10 , /*max=*/ 20, /*step=*/ 2, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mud_pi),
  // MUIF_U8G2_U8_MIN_MAX_STEP("B3", /*valptr=*/ &var3, /*min=*/ 10 , /*max=*/ 20, /*step=*/ 2, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mse_pf),
  // MUIF_U8G2_U8_MIN_MAX_STEP("B4", /*valptr=*/ &var4, /*min=*/ 10 , /*max=*/ 20, /*step=*/ 2, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mud_pf)
  
  MUIF_U8G2_U8_MIN_MAX_STEP_WIDTH("B1", /*valptr=*/ &var1, /*min=*/ 10 , /*max=*/ 200, /*step=*/ 1, /*width=*/ 40, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_fixed_width_bar_wm_mse_pi),
  MUIF_U8G2_U8_MIN_MAX_STEP_WIDTH("B2", /*valptr=*/ &var2, /*min=*/ 10 , /*max=*/ 200, /*step=*/ 1, /*width=*/ 40, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_fixed_width_bar_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX_STEP_WIDTH("B3", /*valptr=*/ &var3, /*min=*/ 10 , /*max=*/ 200, /*step=*/ 1, /*width=*/ 40, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_fixed_width_bar_wm_mse_pf),
  MUIF_U8G2_U8_MIN_MAX_STEP_WIDTH("B4", /*valptr=*/ &var4, /*min=*/ 10 , /*max=*/ 200, /*step=*/ 1, /*width=*/ 40, MUI_MMS_2X_BAR|MUI_MMS_SHOW_VALUE, mui_u8g2_u8_fixed_width_bar_wm_mud_pf)
  
};


fds_t fds[] MUI_PROGMEM  =
MUI_FORM(1)
MUI_STYLE(0)
MUI_LABEL(5,10, "5x8")
MUI_XY("B1", 5, 20)
MUI_XY("B2", 5, 30)
MUI_XY("B3", 5, 40)
MUI_XY("B4", 5, 50)
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

