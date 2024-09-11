
#include "u8g2.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mui.h"
#include "mui_u8g2.h"


/*=================================================*/

/* array element definition */

/* changeing the values below will require major updates to the form layout */
#define ARRAY_VALUE_MAX 15
#define ARRAY_TIME_MAX 16

struct array_element_struct
{
  uint8_t value;
  uint8_t time;  
};

/* array list  */

#define WAVEFORM_ELEMENT_CNT 4
struct array_element_struct waveform_array[WAVEFORM_ELEMENT_CNT];

/* array editable local copy */

volatile uint8_t array_edit_pos = 0;                            // "volatile" might not be required, but still; array_edit_pos is modified by MUI callbacks and used in the extended MUIF
struct array_element_struct array_edit_element;


uint8_t current_value = 0;
uint8_t current_time = 0;
uint8_t play_waveform = 1;


/*=================================================*/
/* global variables */

u8g2_t u8g2;
mui_t ui;

/*=================================================*/
/* waveform array utility functions */

void waveform_array_init(void)
{
  uint8_t i;
  for( i = 0; i < WAVEFORM_ELEMENT_CNT; i++ )
  {
      waveform_array[i].value = i;
      waveform_array[i].time = ARRAY_TIME_MAX;
  }  
}


/* draw wave form based on the array content */
/* x,y is the lower left position of the wave form */
uint8_t waveform_array_draw(u8g2_uint_t x, u8g2_uint_t y, uint8_t array_cnt, uint8_t current_pos, uint8_t current_time)
{
  uint8_t i, j;
  uint8_t prev_value;
  uint8_t curr_value;
  uint8_t time;
  uint8_t t = 0;
  uint8_t v = 255;
    
  prev_value = waveform_array[array_cnt-1].value;
  for( i = 0; i < array_cnt; i++ )
  {
    curr_value = waveform_array[i].value;
    time = waveform_array[i].time;
    if ( curr_value < prev_value )
      u8g2_DrawVLine(&u8g2, x, y-prev_value, prev_value-curr_value+1);  
    else
      u8g2_DrawVLine(&u8g2, x, y-curr_value, curr_value-prev_value+1);  
      
    u8g2_DrawHLine(&u8g2, x, y-curr_value, time+1);
    
    for( j = 0; j < ARRAY_VALUE_MAX; j+=2 )
      u8g2_DrawPixel(&u8g2, x, y-j);
    
    if ( i == current_pos )
    {
      u8g2_DrawHLine(&u8g2, x, y+2, time+1);
    }
    if ( t <= current_time && current_time < t+time )
    {
      printf("waveform_draw %d %d\n", t, current_time);
      u8g2_DrawVLine(&u8g2, x-t+current_time, y-ARRAY_VALUE_MAX, ARRAY_VALUE_MAX+2);  
      v = curr_value;
    }
    
    t += time;
    x += time;
    prev_value = curr_value;
  }
  
  for( j = 0; j < ARRAY_VALUE_MAX; j+=2 )
    u8g2_DrawPixel(&u8g2, x, y-j);
  
  return v;     // return the current value
}


/*=================================================*/
/* special muif */

/*
  Idea: Draw some nice decoration for the waveform editor
*/
uint8_t muif_waveform_editor_decoration(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      /* decoration for the array position and the waveform values */
      u8g2_DrawHLine(&u8g2, 0, 24, 128);
      /* separator between values and waveform graphics */ 
      u8g2_DrawVLine(&u8g2, 50, 0, 24);

      /* separator between waveform graphics and lower buttons */ 
      u8g2_DrawHLine(&u8g2, 0, 46, 128);

    break;
  }
  return 0;
}

/*
  Idea: Introduce a field, which represents a graphical visualization of the waveform
*/
uint8_t muif_waveform_draw(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      waveform_array[array_edit_pos] = array_edit_element;                                          // store the modified local copy back to the array, so that we can draw the current array
      waveform_array_draw(mui_get_x(ui), mui_get_y(ui), WAVEFORM_ELEMENT_CNT, array_edit_pos, 255);
      break;
  }
  return 0;
}


/*
  Same as muif_waveform_draw but doesn't show the current cursor position
*/
uint8_t muif_waveform_show(mui_t *ui, uint8_t msg)
{
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      current_value = waveform_array_draw(mui_get_x(ui), mui_get_y(ui), WAVEFORM_ELEMENT_CNT, 255, current_time);
      break;
  }
  return 0;
}


/*
  Extends:      "mui_u8g2_u8_min_max_wm_mse_pi"
  muif_list:    Use together with "MUIF_U8G2_U8_MIN_MAX" 
  Example:      MUIF_U8G2_U8_MIN_MAX("AP", &array_edit_pos, 0, WAVEFORM_ELEMENT_CNT, muif_array_edit_pos)
  Idea: 
    Whenever MUI modifies the array position, then also store modified values and load new values
  
*/
uint8_t muif_array_edit_pos(mui_t *ui, uint8_t msg)
{
  uint8_t return_value = 0; 
  switch(msg)
  {
    case MUIF_MSG_FORM_START:
      array_edit_element = waveform_array[array_edit_pos];                                          // copy local array element to the local editable copy
      return_value = mui_u8g2_u8_min_max_wm_mse_pi(ui, msg);                    // call the original MUIF
      break;
    case MUIF_MSG_FORM_END:
      return_value = mui_u8g2_u8_min_max_wm_mse_pi(ui, msg);                    // finalise the form
      waveform_array[array_edit_pos] = array_edit_element ;                                         // store the current elements in the array before leaving the form
      break;
    case MUIF_MSG_CURSOR_SELECT:
    case MUIF_MSG_EVENT_NEXT:
    case MUIF_MSG_EVENT_PREV:
      waveform_array[array_edit_pos] = array_edit_element;                                          // store the modified local copy back to the array
      return_value = mui_u8g2_u8_min_max_wm_mse_pi(ui, msg);                            // let MUI modify the current array position
      array_edit_element = waveform_array[array_edit_pos] ;                                         // load the new element from the array to the local editable copy
      break;
    default:
      return_value = mui_u8g2_u8_min_max_wm_mse_pi(ui, msg);                    // for any other messages, just call the original MUIF
  }
  return return_value;
}

/*=================================================*/
/* muif */

muif_t muif_list[]  MUI_PROGMEM = {  
  MUIF_U8G2_LABEL(),
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),

  MUIF_BUTTON("GO", mui_u8g2_btn_goto_wm_fi),
  MUIF_RO("WS", muif_waveform_show),
  MUIF_RO("HL", mui_hline),

  /* waveform editor */
  
  MUIF_RO("DE", muif_waveform_editor_decoration),
  MUIF_RO("AD", muif_waveform_draw),
  MUIF_U8G2_U8_MIN_MAX("AP", (uint8_t *)&array_edit_pos, 0, WAVEFORM_ELEMENT_CNT-1, muif_array_edit_pos),               // the pointer cast avoids warning because of the volatile keyword
  MUIF_U8G2_U8_MIN_MAX("AV", &array_edit_element.value, 0, ARRAY_VALUE_MAX, mui_u8g2_u8_min_max_wm_mse_pi),
  MUIF_U8G2_U8_MIN_MAX("AT", &array_edit_element.time, 0, ARRAY_TIME_MAX, mui_u8g2_u8_min_max_wm_mse_pi),
  MUIF_BUTTON("AG", mui_u8g2_btn_back_wm_fi)
};

/*=================================================*/
/* fields and forms... */

fds_t fds[] MUI_PROGMEM  =

// Main Menu

MUI_FORM(1)
MUI_STYLE(0)

MUI_XYAT("GO", 63, 12, 20, "Waveform Editor")
MUI_XYAT("GO", 63, 28, 2, "LED Test Start")

MUI_XY("HL", 0, 45)
MUI_LABEL(3,60, "Waveform")
MUI_XY("WS", 54, 62)

// Output Waveform to LED

MUI_FORM(2)
MUI_STYLE(0)

MUI_XYAT("GO", 63, 28, 1, "LED Test Stop")

MUI_XY("HL", 0, 45)
MUI_LABEL(3,60, "Waveform")
MUI_XY("WS", 54, 62)


// Waveform Editor

MUI_FORM(20)
MUI_STYLE(0)
MUI_AUX("DE")
MUI_LABEL(3,15, "Pos:")
MUI_XY("AP", 30, 15)

MUI_LABEL(54,10, "Value:")
MUI_XY("AV", 100, 10)

MUI_LABEL(54,20, "Time:")
MUI_XY("AT", 100, 20)

MUI_LABEL(3,39, "Waveform")
MUI_XY("AD", 54, 41)

MUI_XYAT("AG", 63, 59, 1, "Done")
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


  waveform_array_init();
  
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
    
    // printf("mui_GetCurrentCursorFocusPosition=%d\n", mui_GetCurrentCursorFocusPosition(&ui));
    
    do
    {
      k = u8g_sdl_get_key();
      if ( mui_GetCurrentFormId(&ui) == 2 )
      {
        if ( current_value == 255 )
          current_time = 0;
        else
          current_time++;
        sleep(1);
        break;
      }
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
      do_screenshot();
      mui_NextField(&ui);
    }
    if ( k == 'p' ) 
    {
      do_screenshot();
      mui_PrevField(&ui);
    }
    if ( k == 's' )
    {
      do_screenshot();
      mui_SendSelect(&ui);
    }

    if ( k == 't' ) 
    {
      puts("screenshot");
      do_screenshot();
    }
    
  }
  return 0;
}

