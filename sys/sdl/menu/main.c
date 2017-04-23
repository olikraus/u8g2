
#include "menu.h"
#include <stdio.h>

  
u8g2_t u8g2;

uint8_t h, m, mt, mo, st, so;
uint8_t d1, d2, d3, d4, d5, d6, d7;
uint8_t day, month, year_t, year_o;
uint8_t enable;

extern const me_t melist_top_menu[];
extern const me_t melist_alarm_menu[];

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
  { me_cb_button_full_line, (void *)melist_top_menu, "Speichern", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


const me_t melist_setup_date[] = 
{
  { me_cb_1_31, &day, NULL, 		ME_TIME_XO+2,ME_TIME_Y },
  { me_cb_num_label, NULL, ".", 	ME_TIME_XO+30,ME_TIME_Y },
  { me_cb_1_12, &month, NULL, 		ME_TIME_XO+39,ME_TIME_Y },
  { me_cb_num_label, NULL, ".", 	ME_TIME_XO+67,ME_TIME_Y },
  { me_cb_0_9, &year_t, NULL, 		ME_TIME_XO+67+9,ME_TIME_Y },
  { me_cb_0_9, &year_o, NULL, 		ME_TIME_XO+80+9,ME_TIME_Y },
  { me_cb_button_full_line, (void *)melist_top_menu, "Speichern", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


#define ME_ALARM_TIME_XO 28
#define ME_ALARM_TIME_Y 20
#define ME_ALARM_WD_Y 29
#define ME_ALARM_WD_XO 8

const me_t melist_setup_alarm[] = 
{
  { me_cb_big_toggle, &enable, NULL, 		4	, 6},
  
  { me_cb_0_23, &h, NULL, 		ME_ALARM_TIME_XO+2,ME_ALARM_TIME_Y },
  { me_cb_num_label, NULL, ":", 	ME_ALARM_TIME_XO+30,ME_ALARM_TIME_Y-3 },
  { me_cb_0_55, &m, NULL, 		ME_ALARM_TIME_XO+39,ME_ALARM_TIME_Y },
  
  { me_cb_16x16_bitmap_button, (void *)melist_alarm_menu, (void *)ok_xbm, ME_ALARM_TIME_XO+80,ME_ALARM_TIME_Y-17 },

  
  { me_cb_wd_toggle, &d1, "Mo", 	ME_ALARM_WD_XO+17*0, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d2, "Di", 	ME_ALARM_WD_XO+17*1, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d3, "Mi", 	ME_ALARM_WD_XO+17*2, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d4, "Do", 	ME_ALARM_WD_XO+17*3, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d5, "Fr", 	ME_ALARM_WD_XO+17*4, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d6, "Sa", 	ME_ALARM_WD_XO+17*5, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &d7, "So", 	ME_ALARM_WD_XO+17*6, ME_ALARM_WD_Y},
  
  { me_cb_null, NULL, NULL, 0, 0 },
};

int me_action_alarm1(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_setup_alarm, 0);
    return 1;
  }
  return 0;
}

const me_t melist_alarm_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_alarm1, "Alarm 1", 0,10 },
  { me_cb_button_half_line, (void *)me_action_alarm1, "Alarm 2", 64,10 },
  { me_cb_button_half_line, (void *)me_action_alarm1, "Alarm 3", 0,20 },
  { me_cb_button_half_line, (void *)me_action_alarm1, "Alarm 4", 64,20 },
  { me_cb_button_full_line, (void *)melist_top_menu, "Zurück", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

int me_action_setup_time(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_setup_time, 0);
    return 1;
  }
  return 0;
}

int me_action_setup_date(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_setup_date, 0);
    return 1;
  }
  return 0;
}

const me_t melist_setup_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_setup_time, "Uhrzeit", 0,10 },
  { me_cb_button_half_line, (void *)me_action_setup_date, "Datum", 64,10 },
  { me_cb_button_half_line, (void *)NULL, "Power", 0,20 },
  { me_cb_button_half_line, (void *)NULL, "Test", 64,20 },
  { me_cb_button_full_line, (void *)melist_top_menu, "Zurück", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


const me_t melist_top_menu[] = 
{
  { me_cb_button_full_line, NULL, "Zurück", 3,10 },
  { me_cb_button_full_line, (void *)melist_alarm_menu, "Alarm", 3,20 },
  { me_cb_button_full_line, (void *)melist_setup_menu, "Einstellungen", 3,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
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
  //menu_SetMEList(&menu, melist_setup_time, 0);
  //menu_SetMEList(&menu, melist_setup_alarm, 0);
  
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

