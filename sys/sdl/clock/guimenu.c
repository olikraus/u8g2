/*

  gui.c
  
*/

#include "gui.h"
//#include "datecalc.h"
#include <string.h>

/*============================================*/

/* global variable for the gui menues */

uint8_t gui_alarm_index = 0;
gui_alarm_t gui_alarm_current;

const char weekdaystr[7][4] = {
  "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"
};




/*============================================*/

const static uint8_t ok_xbm[] = {	/* 16x16 */
   0xfe, 0x7f, 0x03, 0xc0, 0x01, 0x80, 0x01, 0xb8, 0x01, 0x9c, 0x01, 0x8e,
   0x01, 0x87, 0x01, 0x87, 0x9d, 0x83, 0xb9, 0x83, 0xf1, 0x81, 0xe1, 0x81,
   0xc1, 0x80, 0x01, 0x80, 0x03, 0xc0, 0xfe, 0x7f };

const static uint8_t alarm_xbm[] = {  /* 12x12 */
0x00, 0x00, 0x0c, 0x06, 0xf6, 0x0d, 0x1a, 0x0b, 0x4c, 0x06, 0x44, 0x04,
0xc4, 0x05, 0x04, 0x04, 0x0c, 0x06, 0x18, 0x03, 0xf0, 0x01, 0x00, 0x00 };

   
int me_action_to_top_menu(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_top_menu, 0);
    return 1;
  }
  return 0;
}


int me_action_save_time(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    set_time(gui_data.h / 10, gui_data.h % 10, gui_data.mt, gui_data.mo, gui_data.st, gui_data.so);
    menu_SetMEList(menu, melist_top_menu, 0);	/* first set the normal menu */
    gui_Recalculate();							/* because it might be overwritten with the alarm menu */
    return 1;
  }
  return 0;
}

#define ME_TIME_Y 19
#define ME_TIME_XO 11
const me_t melist_setup_time[] = 
{
  { me_cb_0_23, &gui_data.h, NULL, 		ME_TIME_XO+2,ME_TIME_Y },
  { me_cb_num_label, NULL, ":", 	ME_TIME_XO+30,ME_TIME_Y-3 },
  { me_cb_0_5, &gui_data.mt, NULL, 		ME_TIME_XO+39,ME_TIME_Y },
  { me_cb_0_9, &gui_data.mo, NULL, 		ME_TIME_XO+52,ME_TIME_Y },
  { me_cb_num_label, NULL, ":", 	ME_TIME_XO+67,ME_TIME_Y-3 },
  { me_cb_0_5, &gui_data.st, NULL, 		ME_TIME_XO+67+9,ME_TIME_Y },
  { me_cb_0_9, &gui_data.so, NULL, 		ME_TIME_XO+80+9,ME_TIME_Y },
  { me_cb_button_full_line, (void *)me_action_save_time, "Speichern", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Display Time */

void gui_alarm_to_str(uint8_t idx)
{
      strcpy(gui_data.s, weekdaystr[gui_alarm_list[gui_data.next_alarm_index].na_wd]);
      gui_data.s[2] = ',';
      gui_data.s[3] = ' ';
      strcpy(gui_data.s+4, u8x8_u8toa(gui_alarm_list[gui_data.next_alarm_index].na_h, 2));
      gui_data.s[6] = ':';
      strcpy(gui_data.s+7, u8x8_u8toa(gui_alarm_list[gui_data.next_alarm_index].na_m, 2));
      gui_data.s[9] = '\0';      
}

int me_action_handle_display_time(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_DRAW )
  {
    char s[14];
    u8g2_uint_t x = 4;
    u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);

    if ( gui_data.next_alarm_index < GUI_ALARM_CNT )
    {
      u8g2_DrawXBM(menu->u8g2, 67, 20, 12, 12, (const uint8_t *)(alarm_xbm));
      gui_alarm_to_str(gui_data.next_alarm_index);
      u8g2_DrawUTF8(menu->u8g2, 81, 30, gui_data.s);
      
    }
    else
    {
      x= 34;
    }
    
    strcpy(s, weekdaystr[gui_data.weekday]);
    s[2] = ',';
    s[3] = ' ';
    strcpy(s+4, u8x8_u8toa(gui_data.day, 2));
    s[6] = '.';
    strcpy(s+7, u8x8_u8toa(gui_data.month, 2));
    s[9] = '.';
    s[10] = gui_data.year_t+'0';
    s[11] = gui_data.year_o+'0';
    s[12] = '\0';
    u8g2_DrawUTF8(menu->u8g2, x, 30, s);
    
    
    return 1;
  }
  else if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_top_menu, 0);
    return 1;
  }
  return 0;
}


#define ME_TIME_DXO 30
const me_t melist_display_time[] = 
{
  { me_cb_0_23_ro, &gui_data.h, NULL, 		ME_TIME_DXO+2,ME_TIME_Y },
  { me_cb_num_label, NULL, ":", 			ME_TIME_DXO+30,ME_TIME_Y-3 },
  { me_cb_0_9_ro, &gui_data.mt, NULL, 		ME_TIME_DXO+39,ME_TIME_Y },
  { me_cb_0_9_ro, &gui_data.mo, NULL, 		ME_TIME_DXO+52,ME_TIME_Y },

  { me_cb_0_9_small_ro, &gui_data.st, NULL, 		118,ME_TIME_Y },
  { me_cb_0_9_small_ro, &gui_data.so, NULL, 		123,ME_TIME_Y },

  
  
  { me_cb_button_empty, (void *)me_action_handle_display_time, NULL, 0, 0 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Date Edit Dialog */

int me_action_save_date(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    gui_date_adjust();	/* calculate the weekday */
    
    set_date(gui_data.year_t, gui_data.year_o, gui_data.month / 10, gui_data.month % 10, gui_data.day / 10 , gui_data.day % 10, gui_data.weekday);
    
    menu_SetMEList(menu, melist_top_menu, 0);	/* first set the normal menu */
    gui_Recalculate();							/* because it might be overwritten with the alarm menu */
    return 1;
  }
  return 0;
}

const me_t melist_setup_date[] = 
{
  { me_cb_1_31, &gui_data.day, NULL, 		ME_TIME_XO+2,ME_TIME_Y },
  { me_cb_num_label, NULL, ".", 	ME_TIME_XO+30,ME_TIME_Y },
  { me_cb_1_12, &gui_data.month, NULL, 		ME_TIME_XO+39,ME_TIME_Y },
  { me_cb_num_label, NULL, ".", 	ME_TIME_XO+67,ME_TIME_Y },
  { me_cb_0_9, &gui_data.year_t, NULL, 		ME_TIME_XO+67+9,ME_TIME_Y },
  { me_cb_0_9, &gui_data.year_o, NULL, 		ME_TIME_XO+80+9,ME_TIME_Y },
  { me_cb_button_full_line, (void *)me_action_save_date, "Speichern", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Alarm Edit Dialog */


#define ME_ALARM_TIME_XO 28
#define ME_ALARM_TIME_Y 20
#define ME_ALARM_WD_Y 29
#define ME_ALARM_WD_XO 8

int me_action_alarm_done(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    gui_alarm_list[gui_alarm_index] = gui_alarm_current;
    gui_alarm_list[gui_alarm_index].skip_wd = 0;		/* clear the skip alarm (if any) */
    gui_alarm_list[gui_alarm_index].snooze_count = 0;		/* clear snooze (if any) */
    //gui_alarm_calc_str_time(gui_alarm_index);
    menu_SetMEList(menu, melist_alarm_menu, gui_alarm_index);	/* first set the normal menu */
    gui_Recalculate();							/* because it might be overwritten with the alarm menu */
   return 1;
  }
  return 0;
}

const me_t melist_setup_alarm[] = 
{
  { me_cb_big_toggle, &(gui_alarm_current.enable), NULL, 		4	, 6},
  
  { me_cb_0_23, &(gui_alarm_current.h), NULL, 		ME_ALARM_TIME_XO+2,ME_ALARM_TIME_Y },
  { me_cb_num_label, NULL, ":", 	ME_ALARM_TIME_XO+30,ME_ALARM_TIME_Y-3 },
  { me_cb_0_55, &(gui_alarm_current.m), NULL, 		ME_ALARM_TIME_XO+39,ME_ALARM_TIME_Y },
  
  
  { me_cb_wd_toggle, &(gui_alarm_current.wd[0]), (void *)weekdaystr[0], 	ME_ALARM_WD_XO+17*0, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[1]), (void *)weekdaystr[1], 	ME_ALARM_WD_XO+17*1, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[2]), (void *)weekdaystr[2], 	ME_ALARM_WD_XO+17*2, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[3]), (void *)weekdaystr[3], 	ME_ALARM_WD_XO+17*3, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[4]), (void *)weekdaystr[4], 	ME_ALARM_WD_XO+17*4, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[5]), (void *)weekdaystr[5], 	ME_ALARM_WD_XO+17*5, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[6]), (void *)weekdaystr[6], 	ME_ALARM_WD_XO+17*6, ME_ALARM_WD_Y},

  { me_cb_16x16_bitmap_button, (void *)me_action_alarm_done, (void *)ok_xbm, ME_ALARM_TIME_XO+80,ME_ALARM_TIME_Y-17 },

  
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Alarm Setup Menu */


static int me_action_alarm_common(menu_t *menu, const me_t *me, uint8_t msg) U8G2_NOINLINE;
static int me_action_alarm_common(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    gui_alarm_current = gui_alarm_list[gui_alarm_index];
    menu_SetMEList(menu, melist_setup_alarm, 0);
    return 1;
  }
  return 0;
}

int me_action_alarm1(menu_t *menu, const me_t *me, uint8_t msg)
{
  gui_alarm_index = 0;
  return me_action_alarm_common(menu, me, msg);
}

int me_action_alarm2(menu_t *menu, const me_t *me, uint8_t msg)
{
  gui_alarm_index = 1;
  return me_action_alarm_common(menu, me, msg);
}
int me_action_alarm3(menu_t *menu, const me_t *me, uint8_t msg)
{
  gui_alarm_index = 2;
  return me_action_alarm_common(menu, me, msg);
}
int me_action_alarm4(menu_t *menu, const me_t *me, uint8_t msg)
{
  gui_alarm_index = 3;
  return me_action_alarm_common(menu, me, msg);
}

int me_action_handle_alarm_list(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_DRAW )
  {
    uint8_t y, x;
    uint8_t ym;
    u8g2_SetFont(menu->u8g2, MENU_SMALL_FONT);
    for( y = 0; y < 4; y++ )
    {
      ym = y*8+7;
      for( x = 0; x < 7; x++ )
      {
	u8g2_DrawGlyph(menu->u8g2, 0, ym, y+'1');
	u8g2_DrawGlyph(menu->u8g2, 5, ym, ':');
	u8g2_DrawStr(menu->u8g2, 9, ym, gui_alarm_str[y]);
	if ( gui_alarm_list[y].wd[x] )
	{
	  u8g2_DrawStr(menu->u8g2, 40+x*12, ym, weekdaystr[x]);
	}
      }
    }
  }
  else if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_alarm_menu, 4);
    return 1;
  }
  return 0;
}

const me_t melist_alarm_list_menu[] = 
{
  { me_cb_button_empty, (void *)me_action_handle_alarm_list, NULL, 0, 0 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

int me_action_goto_alarm_list(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_alarm_list_menu, 0);
    return 1;
  }
  return 0;
}

const me_t melist_alarm_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_alarm1, gui_alarm_str[0], 0,10 },
  { me_cb_button_half_line, (void *)me_action_alarm2, gui_alarm_str[1], 64,10 },
  { me_cb_button_half_line, (void *)me_action_alarm3, gui_alarm_str[2], 0,20 },
  { me_cb_button_half_line, (void *)me_action_alarm4, gui_alarm_str[3], 64,20 },
  { me_cb_button_half_line, (void *)me_action_goto_alarm_list, "Liste", 0,30 },
  { me_cb_button_half_line, (void *)me_action_to_top_menu, "Zurück", 64,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Reset Menu */

int me_action_reset_no(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_setup_menu, 0);
    return 1;
  }
  return 0;
}

int me_action_reset_yes(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    do_reset();
    return 1;
  }
  return 0;
}

const me_t melist_reset_menu[] = 
{
  { me_cb_label, NULL, "Reset?",			44, 13},
  { me_cb_button_half_line, (void *)me_action_reset_no, "Nein", 0,30 },
  { me_cb_button_half_line, (void *)me_action_reset_yes, "Ja", 64,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


/*============================================*/
/* System Menu */

int me_action_goto_reset(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_reset_menu, 0);
    return 1;
  }
  return 0;
}

const me_t melist_system_menu[] = 
{
  //{ me_cb_button_half_line, (void *)me_action_setup_time, "Uhrzeit", 0,10 },
  //{ me_cb_button_half_line, (void *)me_action_setup_date, "Datum", 64,10 },
  { me_cb_button_half_line, (void *)NULL, "b", 0,20 },
  { me_cb_button_half_line, (void *)me_action_goto_reset, "Reset", 64,20 },
  { me_cb_button_full_line, (void *)me_action_to_top_menu, "Zurück", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


/*============================================*/
/* Setup Menu */

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


int me_action_goto_system(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    //menu_SetMEList(menu, melist_reset_menu, 0);
    menu_SetMEList(menu, melist_system_menu, 0);
    return 1;
  }
  return 0;
}


const me_t melist_setup_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_setup_time, "Uhrzeit", 0,10 },
  { me_cb_button_half_line, (void *)me_action_setup_date, "Datum", 64,10 },
  { me_cb_button_half_line, (void *)NULL, "Power", 0,20 },
  { me_cb_button_half_line, (void *)me_action_goto_system, "System", 64,20 },
  { me_cb_button_full_line, (void *)me_action_to_top_menu, "Zurück", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Alarm Menu */


int me_action_deactivate_alarm(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_display_time, 0);
    return 1;
  }
  return 0;
}

int me_action_do_snooze(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_display_time, 0);
    return 1;
  }
  return 0;
}

const me_t melist_active_alarm_menu[] = 
{
  { me_cb_label, NULL, "Alarm",			2, 13},
  
  { me_cb_0_23_ro, &gui_data.h, NULL, 		ME_TIME_DXO+2+20,ME_TIME_Y },
  { me_cb_num_label, NULL, ":", 			ME_TIME_DXO+30+20,ME_TIME_Y-3 },
  { me_cb_0_9_ro, &gui_data.mt, NULL, 		ME_TIME_DXO+39+20,ME_TIME_Y },
  { me_cb_0_9_ro, &gui_data.mo, NULL, 		ME_TIME_DXO+52+20,ME_TIME_Y },
  
  { me_cb_button_empty, (void *)me_action_deactivate_alarm, NULL, 0,0 },
  //{ me_cb_button_half_line, (void *)me_action_deactivate_alarm, "Alarm aus", 0,30 },
  //{ me_cb_button_half_line, (void *)me_action_do_snooze, "+5 Min ", 64,30 },

  { me_cb_inv_label, NULL, "Alarm aus", 4,30 },
  { me_cb_inv_label, NULL, "+5 Min", 76,30 },

  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* toplevel menu */


int me_action_to_display_time(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_display_time, 0);
    return 1;
  }
  return 0;
}


int me_action_to_alarm_menu(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_alarm_menu, 0);
    return 1;
  }
  return 0;
}

int me_action_to_setup_menu(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    menu_SetMEList(menu, melist_setup_menu, 0);
    return 1;
  }
  return 0;
}

int me_cb_button_skip_alarm(menu_t *menu, const me_t *me, uint8_t msg)
{  
  int r = 0;
  u8g2_uint_t x;
  switch(msg)
  {
    case ME_MSG_IS_FOCUS:
      return gui_data.is_skip_possible;
    case ME_MSG_DRAW_FOCUS:
      menu_DrawBoxFocus(menu, 
	  0, 
	  me->y - u8g2_GetAscent(menu->u8g2)-1, 
	  u8g2_GetDisplayWidth(menu->u8g2) , 
	  u8g2_GetAscent(menu->u8g2) - u8g2_GetDescent(menu->u8g2) +1);
      r = 1;
      break;
    case ME_MSG_DRAW:
      if ( gui_data.is_skip_possible )
      {
	u8g2_SetFont(menu->u8g2, MENU_NORMAL_FONT);
	gui_alarm_to_str(gui_data.next_alarm_index);
      	x = u8g2_DrawUTF8(menu->u8g2, me->x, me->y, gui_data.s );
      	u8g2_DrawUTF8(menu->u8g2, me->x+x, me->y, " deaktvieren" );
      }
      r = 1;
      break;
    case ME_MSG_SELECT:
      //printf("me_cb_button_skip_alarm ME_MSG_SELECT\n");
      gui_alarm_list[gui_data.next_alarm_index].skip_wd = 
	gui_alarm_list[gui_data.next_alarm_index].na_wd + 1;       
      gui_alarm_list[gui_data.next_alarm_index].snooze_count = 0;		/* clear snooze (if any) */
      menu_SetMEList(menu, melist_display_time, 0);		/* first set the normal menu */
      gui_Recalculate();							/* it might be changed here to the alarm menu */
      r = 1;
      break;
  }
  return r;
}


const me_t melist_top_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_to_display_time, "Zurück", 0,10 },
  { me_cb_button_half_line, (void *)me_action_to_alarm_menu, "Alarm", 64,10 },
  { me_cb_button_skip_alarm, NULL, NULL, 3,20 },
  { me_cb_button_full_line, (void *)me_action_to_setup_menu, "Weitere Funktionen", 3,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};
