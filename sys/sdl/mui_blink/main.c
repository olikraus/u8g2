
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

uint8_t blink_light = 4;        // brightness
uint8_t blink_time = 1;
uint8_t blink_duty = 1;

uint8_t blink_state = 0;
long blink_last_update = 0;



void blink_update(void)
{
  /*
  long next_update_time = blink_last_update;
  if ( blink_state == 0 )
  {
    next_update_time += (long)(blink_duty+1)*(blink_time+1)*40L;
  }
  else
  {
    next_update_time += (long)(5-blink_duty)*(blink_time+1)*40L;
  }
  
  if ( next_update_time < millis() )
  {
  
    blink_last_update = millis();
    
    blink_state = 1-blink_state;
    
    if ( blink_state == 0 ) 
      analogWrite(LED_BUILTIN, (blink_light*255)>>2);
    else
      digitalWrite(LED_BUILTIN, LOW);
    
  }
  */
}



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

muif_t muif_list[]  = {
  
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),
  MUIF_U8G2_FONT_STYLE(1, u8g2_font_helvB08_tr),
  //MUIF_U8G2_FONT_STYLE(9, u8g2_font_streamline_interface_essential_loading_t),
  MUIF_U8G2_FONT_STYLE(9, u8g2_font_percent_circle_25_hn),

  MUIF_RO("HR", mui_hrule),
  MUIF_U8G2_LABEL(),
  MUIF_RO("GP",mui_u8g2_goto_data),
  MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

  MUIF_U8G2_U8_MIN_MAX("NB", &blink_light, 0, 4, mui_u8g2_u8_min_max_wm_mse_pi),
  MUIF_U8G2_U8_MIN_MAX("NT", &blink_time, 0, 4, mui_u8g2_u8_min_max_wm_mse_pi),
  MUIF_U8G2_U8_MIN_MAX("ND", &blink_duty, 0, 4, mui_u8g2_u8_min_max_wm_mse_pi),

  MUIF_U8G2_U8_MIN_MAX_STEP("BB", &blink_light, 0, 4, 1, MUI_MMS_4X_BAR, mui_u8g2_u8_bar_wm_mse_pf),
  MUIF_U8G2_U8_MIN_MAX_STEP("BT", &blink_time, 0, 4, 1, MUI_MMS_4X_BAR, mui_u8g2_u8_bar_wm_mse_pf),
  MUIF_U8G2_U8_MIN_MAX_STEP("BD", &blink_duty, 0, 4, 1, MUI_MMS_4X_BAR, mui_u8g2_u8_bar_wm_mse_pf),

  MUIF_VARIABLE("PB", &blink_light, mui_u8g2_u8_opt_line_wa_mse_pi),
  MUIF_VARIABLE("PT", &blink_time, mui_u8g2_u8_opt_line_wa_mse_pi),
  MUIF_VARIABLE("PD", &blink_duty, mui_u8g2_u8_opt_line_wa_mse_pi),

  MUIF_U8G2_U8_MIN_MAX("nB", &blink_light, 0, 4, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("nT", &blink_time, 0, 4, mui_u8g2_u8_min_max_wm_mud_pi),
  MUIF_U8G2_U8_MIN_MAX("nD", &blink_duty, 0, 4, mui_u8g2_u8_min_max_wm_mud_pi),

  MUIF_U8G2_U8_MIN_MAX_STEP("bB", &blink_light, 0, 4, 1, MUI_MMS_4X_BAR, mui_u8g2_u8_bar_wm_mud_pf),
  MUIF_U8G2_U8_MIN_MAX_STEP("bT", &blink_time, 0, 4, 1, MUI_MMS_4X_BAR, mui_u8g2_u8_bar_wm_mud_pf),
  MUIF_U8G2_U8_MIN_MAX_STEP("bD", &blink_duty, 0, 4, 1, MUI_MMS_4X_BAR, mui_u8g2_u8_bar_wm_mud_pf),

  MUIF_VARIABLE("pB", &blink_light, mui_u8g2_u8_opt_line_wa_mud_pi),
  MUIF_VARIABLE("pT", &blink_time, mui_u8g2_u8_opt_line_wa_mud_pi),
  MUIF_VARIABLE("pD", &blink_duty, mui_u8g2_u8_opt_line_wa_mud_pi),

  /* a button for the menu... */
  MUIF_BUTTON("GO", mui_u8g2_btn_goto_wm_fi)
};


fds_t fds[] =

MUI_FORM(1)
MUI_STYLE(1)
MUI_LABEL(5, 8, "MUI Blink")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_DATA("GP", 
    MUI_10 "Numeric (MSE)|"
    MUI_12 "Progress Bar (MSE)|"
    MUI_14 "Progress Pie (MSE)|"
    MUI_20 "Numeric (MUD)|"
    MUI_22 "Progress Bar (MUD)|"
    MUI_24 "Progress Pie (MUD)")
MUI_XYA("GC", 5, 24, 0) 
MUI_XYA("GC", 5, 36, 1) 
MUI_XYA("GC", 5, 48, 2) 
MUI_XYA("GC", 5, 60, 3) 

MUI_FORM(10)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Numeric (MSE)")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,23, "Light:")
MUI_LABEL(5,35, "Time:")
MUI_LABEL(5,47, "Duty:")
MUI_XY("NB", 50, 23)
MUI_XY("NT", 50, 35)
MUI_XY("ND", 50, 47)
MUI_XYAT("GO", 20, 60, 1, " Exit ") 

MUI_FORM(12)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Bar Graph (MSE)")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,23, "Light:")
MUI_LABEL(5,35, "Time:")
MUI_LABEL(5,47, "Duty:")
MUI_XY("BB", 50, 23)
MUI_XY("BT", 50, 35)
MUI_XY("BD", 50, 47)
MUI_XYAT("GO", 20, 60, 1, " Exit ") 

MUI_FORM(14)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Pie Graph (MSE)")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,21, "Light")
MUI_LABEL(37,21, "Time")
MUI_LABEL(69,21, "Duty")
MUI_STYLE(9)
MUI_XYAT("PB", 6, 49, 21, "0|2|4|6|8")
MUI_XYAT("PT", 38, 49, 21, "0|2|4|6|8")
MUI_XYAT("PD", 70, 49, 21, "0|2|4|6|8")
MUI_STYLE(0)
MUI_XYAT("GO", 20, 60, 1, " Exit ") 

MUI_FORM(20)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Numeric (MUD)")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,23, "Light:")
MUI_LABEL(5,35, "Time:")
MUI_LABEL(5,47, "Duty:")
MUI_XY("nB", 50, 23)
MUI_XY("nT", 50, 35)
MUI_XY("nD", 50, 47)
MUI_XYAT("GO", 20, 60, 1, " Exit ") 

MUI_FORM(22)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Bar Graph (MUD)")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,23, "Light:")
MUI_LABEL(5,35, "Time:")
MUI_LABEL(5,47, "Duty:")
MUI_XY("bB", 50, 23)
MUI_XY("bT", 50, 35)
MUI_XY("bD", 50, 47)
MUI_XYAT("GO", 20, 60, 1, " Exit ") 

MUI_FORM(24)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Pie Graph (MUD)")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_LABEL(5,21, "Light")
MUI_LABEL(37,21, "Time")
MUI_LABEL(69,21, "Duty")
MUI_STYLE(9)
MUI_XYAT("pB", 6, 49, 21, "0|2|4|6|8")
MUI_XYAT("pT", 38, 49, 21, "0|2|4|6|8")
MUI_XYAT("pD", 70, 49, 21, "0|2|4|6|8")
MUI_STYLE(0)
MUI_XYAT("GO", 20, 60, 1, " Exit ") 

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
  
  u8g2_SetFontMode(&u8g2, 1);
  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list)/sizeof(muif_t));
  //mui_GotoForm(&ui, 201, 0);
  mui_GotoForm(&ui, 1, 0);

  //puts(fds);
  
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

