/*

  gui.c
  
*/

#include "gui.h"
#include "datecalc.h"
#include <string.h>

/*============================================*/
extern const me_t melist_display_time[];
extern const me_t melist_top_menu[];
extern const me_t melist_active_alarm_menu[];
extern const me_t melist_setup_menu[];
extern const me_t melist_alarm_menu[];

/*============================================*/



uint8_t gui_alarm_index = 0;
gui_alarm_t gui_alarm_current;
gui_alarm_t gui_alarm_list[GUI_ALARM_CNT];
char gui_alarm_str[GUI_ALARM_CNT][8];


const char weekdaystr[7][4] = {
  "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"
};


gui_data_t gui_data;

menu_t gui_menu;


/*============================================*/

void gui_alarm_calc_next_wd_alarm(uint8_t idx, uint16_t current_week_time_in_minutes)
{
  uint8_t i;
  uint16_t week_time_abs;
  uint16_t week_time_diff;	/* difference to current_week_time_in_minutes */
  uint16_t best_diff = 0x0ffff;
  gui_alarm_list[idx].na_week_time_in_minutes = 0x0ffff;		/* not found */
  gui_alarm_list[idx].na_minutes_diff = 0x0ffff;				/* not found */
  gui_alarm_list[idx].na_wd = 7;						/* not found */
  
  //printf("gui_alarm_calc_next_wd_alarm: %d\n", idx);
  
  if ( gui_alarm_list[idx].enable != 0 )
  {
    //printf("gui_alarm_calc_next_wd_alarm: %d enabled\n", idx);
    for( i = 0; i < 7; i++ )
    {
      if ( gui_alarm_list[idx].wd[i] != 0 )
      {
	//printf("gui_alarm_calc_next_wd_alarm: %d i=%d gui_alarm_list[idx].skip_wd=%d \n", idx, i, gui_alarm_list[idx].skip_wd);
	if ( gui_alarm_list[idx].skip_wd != i+1 )
	{
	  week_time_abs = i; 
	  week_time_abs *= 24; 
	  week_time_abs += gui_alarm_list[idx].h;
	  week_time_abs *= 60;
	  week_time_abs += gui_alarm_list[idx].m;
	  week_time_abs += gui_alarm_list[idx].snooze_count*(uint16_t)SNOOZE_MINUTES;
	  
	  if ( current_week_time_in_minutes <= week_time_abs )
	    week_time_diff = week_time_abs - current_week_time_in_minutes;
	  else
	    week_time_diff = week_time_abs + 7*24*60 - current_week_time_in_minutes;

	  //printf("gui_alarm_calc_next_wd_alarm: %d week_time_abs=%d current_week_time_in_minutes=%d week_time_diff=%d\n", idx, week_time_abs, current_week_time_in_minutes,week_time_diff);
	  
	  if (  best_diff > week_time_diff )
	  {
	    best_diff = week_time_diff;
	    /* found for this alarm */
	    gui_alarm_list[idx].na_minutes_diff = week_time_diff;
	    gui_alarm_list[idx].na_week_time_in_minutes = week_time_abs;
	    gui_alarm_list[idx].na_h = gui_alarm_list[idx].h;
	    gui_alarm_list[idx].na_m = gui_alarm_list[idx].m;
	    gui_alarm_list[idx].na_wd = i;
	  }
	}
      }
    }
  }
  //printf("gui_alarm_calc_next_wd_alarm: %d na_minutes_diff=%d\n", idx, gui_alarm_list[idx].na_minutes_diff);
  //printf("gui_alarm_calc_next_wd_alarm: %d na_wd=%d\n", idx, gui_alarm_list[idx].na_wd);
}


void gui_alarm_calc_str_time(uint8_t idx) U8G2_NOINLINE;
void gui_alarm_calc_str_time(uint8_t idx)
{
  gui_alarm_str[idx][0] = ' ';
  strcpy(gui_alarm_str[idx]+1, u8x8_u8toa(gui_alarm_list[idx].h, 2));
  strcpy(gui_alarm_str[idx]+4, u8x8_u8toa(gui_alarm_list[idx].m, 2));
  gui_alarm_str[idx][3] = ':';
  if ( gui_alarm_list[idx].enable == 0 )
  {
    gui_alarm_str[idx][0] = '(';
    gui_alarm_str[idx][6] = ')';
    gui_alarm_str[idx][7] = '\0';
  }    
}

/* adjust day/month and calculates the weekday */
void gui_date_adjust(void) U8G2_NOINLINE;
void gui_date_adjust(void)
{
    uint16_t ydn;
    //uint16_t cdn;
    uint16_t year;
    if ( gui_data.month == 0 )
      gui_data.month++;
    if ( gui_data.day == 0 )
      gui_data.day++;
    year = 2000+gui_data.year_t*10 + gui_data.year_o;
    ydn = get_year_day_number(year, gui_data.month, gui_data.day);
  
    gui_data.month = get_month_by_year_day_number(year, ydn);
    gui_data.day = get_day_by_year_day_number(year, ydn);
    gui_data.weekday = get_weekday_by_year_day_number(year, ydn);	/* 0 = Sunday */
    /* adjust the weekday so that 0 will be Monday */
    gui_data.weekday += 6;
    if ( gui_data.weekday >= 7 ) 
      gui_data.weekday -= 7;
    //cdn = to_century_day_number(y, ydn);
    //to_minutes(cdn, h, m);
}

/*
  calculate the minute within the week.
  this must be called after gui_date_adjust(), because the weekday is used here
*/
void gui_calc_week_time(void)
{
  gui_data.week_time = gui_data.weekday;
  gui_data.week_time  *= 24;
  gui_data.week_time += gui_data.h;
  gui_data.week_time  *= 60;
  gui_data.week_time  += gui_data.mt * 10 + gui_data.mo;
  
}

/*
  calculate the next alarm.
  this must be called after gui_calc_week_time() because, we need week_time
*/
void gui_calc_next_alarm(void)
{
  uint8_t i;
  uint8_t lowest_i;
  uint16_t lowest_diff;
  /* step 1: Calculate the difference to current weektime for each alarm */
  /* result is stored in gui_alarm_list[i].na_minutes_diff */
  for( i = 0; i < GUI_ALARM_CNT; i++ )
    gui_alarm_calc_next_wd_alarm(i, gui_data.week_time);
  
  /* step 2: find the index with the lowest difference */
  lowest_diff = 0x0ffff;
  lowest_i = GUI_ALARM_CNT;
  for( i = 0; i < GUI_ALARM_CNT; i++ )
  {
    if ( lowest_diff > gui_alarm_list[i].na_minutes_diff )
    {
      lowest_diff = gui_alarm_list[i].na_minutes_diff;
      lowest_i = i;
    }
  }
  
  
  /* step 3: store the result */
  gui_data.next_alarm_index = lowest_i;  /* this can be GUI_ALARM_CNT */
  //printf("gui_calc_next_alarm gui_data.next_alarm_index=%d\n", gui_data.next_alarm_index);
  
  /* calculate the is_skip_possible and the is_alarm flag */
  gui_data.is_skip_possible = 0;
  if ( lowest_i < GUI_ALARM_CNT )
  {
    
    if ( gui_alarm_list[lowest_i].na_minutes_diff <= 1 )
    {
      if ( gui_data.is_alarm == 0 )
      {
	gui_data.is_alarm = 1;
	gui_data.active_alarm_idx = lowest_i;
	menu_SetMEList(&gui_menu, melist_active_alarm_menu, 0);

      }
    }
    else
    {
      /* valid next alarm time */
      if ( gui_alarm_list[lowest_i].skip_wd == 0 )
      {
	/* skip flag not yet set */
	if ( gui_alarm_list[lowest_i].na_minutes_diff <= (uint16_t)60*(uint16_t)ALLOW_SKIP_HOURS )
	{
	  /* within the limit before alarm */
	  gui_data.is_skip_possible = 1;
	}
      }
    }
  }
}

/* recalculate all internal data */
/* additionally the active alarm menu might be set by this function */
void gui_Recalculate(void)
{
  int i;
  
  gui_date_adjust();
  gui_calc_week_time();
  gui_calc_next_alarm();
  for ( i = 0; i < GUI_ALARM_CNT; i++ )
  {
    gui_alarm_calc_str_time(i);
  }
}

void gui_Init(u8g2_t *u8g2)
{
  menu_Init(&gui_menu, u8g2);
  menu_SetMEList(&gui_menu, melist_display_time, 0);
  gui_Recalculate();
}


void gui_Draw(void)
{
    menu_Draw(&gui_menu);
}

void gui_Next(void)
{
  menu_NextFocus(&gui_menu);
}

void gui_Select(void)
{
  menu_Select(&gui_menu);
}

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
  
  { me_cb_button_empty, (void *)me_action_handle_display_time, NULL, 0, 0 },
  { me_cb_null, NULL, NULL, 0, 0 },
};

/*============================================*/
/* Date Edit Dialog */

int me_action_save_date(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
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


const me_t melist_setup_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_setup_time, "Uhrzeit", 0,10 },
  { me_cb_button_half_line, (void *)me_action_setup_date, "Datum", 64,10 },
  { me_cb_button_half_line, (void *)NULL, "Power", 0,20 },
  { me_cb_button_half_line, (void *)me_action_goto_alarm_list, "Alarme", 64,20 },
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
  { me_cb_button_full_line, (void *)0, "", 3,10 },
  { me_cb_button_full_line, (void *)me_action_deactivate_alarm, "Alarm abschalten", 3,20 },
  { me_cb_button_full_line, (void *)me_action_do_snooze, "5 Min schlummern", 3,30 },
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
