
#include "menu.h"
#include <stdio.h>

  
u8g2_t u8g2;

uint8_t h, m, mt, mo, st, so;
uint8_t d1, d2, d3, d4, d5, d6, d7;
uint8_t enable;

extern const me_t melist_top_menu[];

const static uint8_t ok_xbm[] = {
   0xfe, 0x7f, 0x03, 0xc0, 0x01, 0x80, 0x01, 0xb8, 0x01, 0x9c, 0x01, 0x8e,
   0x01, 0x87, 0x01, 0x87, 0x9d, 0x83, 0xb9, 0x83, 0xf1, 0x81, 0xe1, 0x81,
   0xc1, 0x80, 0x01, 0x80, 0x03, 0xc0, 0xfe, 0x7f };


#define ME_TIME_Y 20
#define ME_TIME_XO 11
const me_t melist_setup_time[] = 
{
  { me_cb_0_23, &h, NULL, 		ME_TIME_XO+2,ME_TIME_Y },
  { me_cb_num_label, NULL, ":", 	ME_TIME_XO+30,ME_TIME_Y-3 },
  { me_cb_0_5, &mt, NULL, 		ME_TIME_XO+39,ME_TIME_Y },
  { me_cb_0_9, &mo, NULL, 		ME_TIME_XO+52,ME_TIME_Y },
  { me_cb_num_label, NULL, ":", 	ME_TIME_XO+67,ME_TIME_Y-3 },
  { me_cb_0_5, &st, NULL, 		ME_TIME_XO+67+9,ME_TIME_Y },
  { me_cb_0_9, &so, NULL, 		ME_TIME_XO+80+9,ME_TIME_Y },
  { me_cb_text_line, (void *)melist_top_menu, "Speichern", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


#define ME_ALARM_TIME_XO 21
#define ME_ALARM_TIME_Y 20
#define ME_ALARM_WD_Y 29

const me_t melist_setup_alarm[] = 
{
  { me_cb_big_toggle, &enable, NULL, 		2	, 6},
  
  { me_cb_0_23, &h, NULL, 		ME_ALARM_TIME_XO+2,ME_ALARM_TIME_Y },
  { me_cb_num_label, NULL, ":", 	ME_ALARM_TIME_XO+30,ME_ALARM_TIME_Y-3 },
  { me_cb_0_55, &m, NULL, 		ME_ALARM_TIME_XO+39,ME_ALARM_TIME_Y },
  
  { me_cb_16x16_bitmap_button, (void *)melist_top_menu, ok_xbm, ME_ALARM_TIME_XO+70,ME_ALARM_TIME_Y-17 },

  
  { me_cb_wd_toggle, &d1, "Mo", 5, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d2, "Di", 20, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d3, "Mi", 35, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d4, "Do", 50, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d5, "Fr", 65, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d6, "Sa", 80, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d7, "So", 95, ME_ALARM_WD_Y},
  
  { me_cb_null, NULL, NULL, 0, 0 },
};

const me_t melist_top_menu[] = 
{
  { me_cb_text_line, NULL, "Zurück", 3,10 },
  { me_cb_text_line, NULL, "Alarm", 3,20 },
  { me_cb_text_line, NULL, "Einstellungen", 3,30 },
  { me_cb_null, NULL, 0, 0 },
};



menu_t menu;


int main(void)
{
  int x, y;
  int k;
  int i;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  //u8g2_SetFont(&u8g2, u8g2_font_helvB18_tf);
  u8g2_SetFont(&u8g2, u8g2_font_ncenR08_tf);
  
  menu_Init(&menu, &u8g2);
  menu_SetMEList(&menu, melist_top_menu, 0);
  menu_SetMEList(&menu, melist_setup_time, 0);
  menu_SetMEList(&menu, melist_setup_alarm, 0);
  
  x = 50;
  y = 50;

  
  for(;;)
  {
    u8g2_FirstPage(&u8g2);
    i = 0;
    do
    {
      menu_Draw(&menu);
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_SetFont(&u8g2, u8g2_font_ncenR08_tf);
      u8g2_DrawStr(&u8g2, x, y, "-- 123");
      u8g2_DrawXBM(&u8g2, x, y+20, 16, 16, ok_xbm);
      i++;
      
    } while( u8g2_NextPage(&u8g2) );
   
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 'n' )
      menu_NextFocus(&menu);

    if ( k == 's' )
      menu_Select(&menu);
    
    if ( k == 273 ) y -= 1;
    if ( k == 274 ) y += 1;
    if ( k == 276 ) x -= 1;
    if ( k == 275 ) x += 1;
    
    if ( k == 'q' ) break;
    
  }
  
  //u8x8_Set8x8Font(u8g2_GetU8x8(&u8g2), bdf_font);
  //u8x8_Draw8x8String(u8g2_GetU8x8(&u8g2), 0, 0, "Hello World!");
  
  
  
  
  
  
  return 0;
}

