
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


/* hb test: github.com/olikraus/u8g2/issues/2656 */
static const unsigned char hb_null[] U8X8_PROGMEM = { 0x00, 0x00 };
static const unsigned char hb_abc[] U8X8_PROGMEM = {
  0x00, 'a', 0x00, 0x00, 
  0x00, 'b', 0x08, 0x00, 
  0x00, 'c', 0x08, 0x00, 
  0x00, 0x00  // end of binary
};
static const unsigned char teststring[] U8X8_PROGMEM = {
  0x09, 0x28, 0x00, 0x00, // u8g2_DrawGlyph(&u8g2, 0, 0, 2344);
  0x09, 0x2e, 0x10, 0x00, // u8g2_DrawGlyph(&u8g2, 16, 0, 2350);
  0x09, 0x38, 0x10, 0x00, // u8g2_DrawGlyph(&u8g2, 32, 0, 2360);
  0x09, 0x4d, 0x00, 0x00, // u8g2_DrawGlyph(&u8g2, 32, 0, 2381);
  0x09, 0x24, 0x10, 0x00, // u8g2_DrawGlyph(&u8g2, 48, 0, 2340);
  0x09, 0x47, 0x00, 0x00, // u8g2_DrawGlyph(&u8g2, 48, 0, 2375);
  0x00, 0x00  // end of binary
};

uint8_t mui_u8g2_draw_hb(mui_t *ui, uint8_t msg)
{
  if ( msg == MUIF_MSG_DRAW )
  {
    const unsigned char *data = hb_null;
    const uint8_t  *old_font = mui_get_U8g2(ui)->font;  // backup the current font
    switch(mui_get_arg(ui))
    {
      case 123: 
          u8g2_SetFont(mui_get_U8g2(ui), u8g2_font_unifont_tr); // assign the font,which is used for the hb data
          data = hb_abc; 
          break; // MUI_XYA(x, y, 123)
      case 235: 
          u8g2_SetFont(mui_get_U8g2(ui), u8g2_font_unifont_t_devanagari); // assign the font,which is used for the hb data
          data = teststring; 
          break; // MUI_XYA(x, y, 235)
    }
    u8g2_DrawHB(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), data);  // draw the HB data
    u8g2_SetFont(mui_get_U8g2(ui), old_font);  // restore the previous font
  }
  return 0;
}


muif_t muif_list[]  MUI_PROGMEM = {  
  MUIF_U8G2_LABEL(),
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_5x8_tr),
  
  MUIF_VARIABLE("B1",&var1,mui_u8g2_u8_char_wm_mud_pi),  
  MUIF_VARIABLE("B2",&var2,mui_u8g2_u8_char_wm_mud_pi), 
  MUIF_VARIABLE("B3",&var3,mui_u8g2_u8_char_wm_mud_pi), 
  MUIF_VARIABLE("B4",&var4,mui_u8g2_u8_char_wm_mud_pi), 

  MUIF_BUTTON("G0", mui_u8g2_btn_goto_wm_fi),
  
  MUIF_RO("HB",mui_u8g2_draw_hb) // Use MUI_XYA for this
};


fds_t fds[] MUI_PROGMEM  =
MUI_FORM(1)
MUI_STYLE(0)
MUI_LABEL(5,10, "Password")
MUI_XY("B1", 5, 20)
MUI_XY("B2", 15, 20)
MUI_XY("B3", 25, 20)
MUI_XY("B4", 35, 20)
MUI_XYAT("G0", 64, 40, 10, "Goto Static Pages")

MUI_FORM(10)
MUI_STYLE(0)
MUI_LABEL(5,10, "Page 1")
MUI_XYA("HB", 5, 30, 123)
MUI_XYA("HB", 5, 50, 235)
MUI_FORM(11)
MUI_STYLE(0)
MUI_LABEL(5,10, "Page 2")

MUI_FORM(12)
MUI_STYLE(0)
MUI_LABEL(5,10, "Page 3")
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
  mui_GotoForm(&ui, 10, 0);

  
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

    
    if ( mui_GetCurrentFormId(&ui) >= 10 )  // static pages
    {
      if ( k == 'n' ) /* next static form */
      {
        if ( mui_GetCurrentFormId(&ui) >= 12 )
          mui_GotoForm(&ui, 10, 0);
        else
          mui_GotoForm(&ui, mui_GetCurrentFormId(&ui)+1, 0);
      }
      if ( k == 'p' ) /* previous static form */
      {
        if ( mui_GetCurrentFormId(&ui) <= 10 )
          mui_GotoForm(&ui, 12, 0);
        else
          mui_GotoForm(&ui, mui_GetCurrentFormId(&ui)-1, 0);
      }
      if ( k == 's' ) /* goto password form */
      {
        mui_GotoForm(&ui, 1, 0);
      }
    }
    else  /* password input, do the normal event handling */
    {
      if ( k == 'n' ) 
        mui_NextField(&ui);
      if ( k == 'p' ) 
        mui_PrevField(&ui);
      if ( k == 's' )
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

