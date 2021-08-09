
#include "u8g2.h"
#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "ui_u8g2.h"

  
u8g2_t u8g2;
ui_t ui;

uint8_t my_value = 2;

uif_t uif_list[] = {
UIF("B0",UIF_CFLAG_IS_CURSOR_SELECTABLE,0,uif_frame_button_bold_select_u8g2),
UIF("B1",UIF_CFLAG_IS_CURSOR_SELECTABLE,0,uif_frame_button_bold_select_u8g2),
UIF("I1",UIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value,uif_input_uint8_invers_select_u8g2),
UIF("FL",0,0,uif_label_u8g2)
};

fds_t fds = 
UI_FORM("0")
UI_LABEL(64,10, "Ag(1jm")
UI_FIELD("I1",10, 30)
UI_BUTTON("B1",32,50, "Ok")
UI_BUTTON("B0",96,50, "Cancel")
;


int main(void)
{
  int x, y;
  int k;

  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  
  ui_Init(&ui, fds, uif_list, sizeof(uif_list)/sizeof(uif_t));
  ui.graphics_data = &u8g2;
  ui_EnterForm(&ui);

  
  
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
      u8g2_SetFont(&u8g2, u8g2_font_timR12_tr);      
      u8g2_SetFont(&u8g2, u8g2_font_helvR10_tr);      
      u8g2_SetFontMode(&u8g2, 1);
      //u8g2_SetFontPosTop(&u8g2);
      //u8g2_SetFontPosBottom(&u8g2);
      //u8g2_SetFontPosCenter(&u8g2);
      u8g2_SetFontRefHeightExtendedText(&u8g2);
      
      //u8g2_DrawBox(&u8g2, 10, 10+y, 20, x);
      //u8g2_DrawFrame(&u8g2, 34, 10+y, 20, x);
      //u8g2_DrawStr(&u8g2, 10, 50, "XY");
      ui_Draw(&ui);
    } while( u8g2_NextPage(&u8g2) );
    
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

    if ( k == 'n' ) ui_NextField(&ui);
    if ( k == 'p' ) ui_PrevField(&ui);
    if ( k == 's' ) ui_SendSelect(&ui);

    
    if ( x < 0 )
      x = 0;
    
  }
  return 0;
}

