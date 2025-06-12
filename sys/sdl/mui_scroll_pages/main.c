
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

uint8_t var1 = '0';
uint8_t var2 = '0';
uint8_t var3 = '0';
uint8_t var4 = '0';

muif_t muif_list[]  MUI_PROGMEM = {  
  MUIF_U8G2_LABEL(),
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_5x8_tr),
  
  MUIF_VARIABLE("B1",&var1,mui_u8g2_u8_char_wm_mud_pi),  
  MUIF_VARIABLE("B2",&var2,mui_u8g2_u8_char_wm_mud_pi), 
  MUIF_VARIABLE("B3",&var3,mui_u8g2_u8_char_wm_mud_pi), 
  MUIF_VARIABLE("B4",&var4,mui_u8g2_u8_char_wm_mud_pi), 

  MUIF_BUTTON("G0", mui_u8g2_btn_goto_wm_fi)
  
};


fds_t fds[] MUI_PROGMEM  =
MUI_FORM(1)
MUI_STYLE(0)
MUI_LABEL(5,10, "Password")
MUI_XY("B1", 5, 20)
MUI_XY("B2", 15, 20)
MUI_XY("B3", 25, 20)
MUI_XY("B4", 35, 20)
MUI_XYAT("G0", 64, 40, 10, "Done")

MUI_FORM(10)
MUI_STYLE(0)
MUI_LABEL(5,10, "Info Page")
MUI_XYAT("G0", 64, 40, 1, "Password")


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

uint8_t *old_buffer = NULL;
uint8_t *new_buffer = NULL;

void merge_buffer(u8g2_uint_t x)
{
  uint8_t *dest_buffer = u8g2_GetBufferPtr(&u8g2);
  uint8_t th = u8g2_GetBufferTileHeight(&u8g2);
  uint8_t tw = u8g2_GetBufferTileWidth(&u8g2);
  u8g2_uint_t w = tw*8;
  u8g2_uint_t i, j;
  uint8_t b;
  
  for( j = 0; j < th; j++ )
  {
    for( i = 0; i < w; i++ )
    {
      if ( i < x )
        b = new_buffer[w-1-x+i+j*w];
      else
        b = old_buffer[i-x+j*w];
      dest_buffer[i+j*w] = b;
    }
  }  
}

void render_buffer(void)
{
  /* assume full buffer */
  u8g2_SetBufferCurrTileRow(&u8g2, 0);  
  u8g2_ClearBuffer(&u8g2);
  mui_Draw(&ui);     /* render buffer without sending the content to the display */
}


int main(void)
{
  int x, y;
  int k;
  int form_id = -1;

  
  u8g2_SetupBuffer_SDL_128x64(&u8g2, &u8g2_cb_r0);
  
  old_buffer = (uint8_t *)malloc(u8g2_GetBufferSize(&u8g2));
  new_buffer = (uint8_t *)malloc(u8g2_GetBufferSize(&u8g2));
  
  printf("buffer size=%d\n", u8g2_GetBufferSize(&u8g2));
  
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */

  u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
  u8g2_SetFontMode(&u8g2, 1);
  u8g2_SetFontRefHeightExtendedText(&u8g2);
  mui_Init(&ui, &u8g2, fds, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui_GotoForm(&ui, 10, 0);


  form_id = mui_GetCurrentFormId(&ui);
  
  x = 4;  // use as height for the box
  y = 0;
  
  for(;;)
  {
    
    render_buffer();
    u8g2_SendBuffer(&u8g2);
    do_screenshot();
    memcpy(old_buffer, u8g2_GetBufferPtr(&u8g2), u8g2_GetBufferSize(&u8g2));
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
      mui_NextField(&ui);
    if ( k == 'p' ) 
      mui_PrevField(&ui);
    if ( k == 's' )
      mui_SendSelect(&ui);

    if ( form_id != mui_GetCurrentFormId(&ui) )
    {
      render_buffer();
      memcpy(new_buffer, u8g2_GetBufferPtr(&u8g2), u8g2_GetBufferSize(&u8g2));
      
      for( x = 0; x < u8g2_GetBufferTileWidth(&u8g2)*8; x+=2 )
      {
        merge_buffer(x);
        usleep(5*1000);
        u8g2_SendBuffer(&u8g2);
        do_screenshot();
      }
      form_id = mui_GetCurrentFormId(&ui);      
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

