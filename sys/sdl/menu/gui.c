/*

  gui.c
  
*/

#include "gui.h"
#include "datecalc.h"
#include <string.h>

/*============================================*/
extern const me_t melist_top_menu[];
extern const me_t melist_alarm_menu[];
extern const me_t melist_setup_menu[];

/*============================================*/

#define GUI_ALARM_CNT 4
#define SNOOZE_MINUTES 10

struct _gui_data
{
  uint16_t week_time;	/* derived from h, mt, mo and weekday */
  uint8_t h, mt, mo, st, so;
  uint8_t day; 		/* 1 .. 31 */
  uint8_t month;	/* 1..12 */
  uint8_t year_t, year_o;
  uint8_t weekday; 	/* 0 = Sunday */
  
  uint8_t next_alarm_index;	/* index for the next alarm or GUI_ALARM_CNT if there is no next alarm */
};
typedef struct _gui_data gui_data_t;



struct _gui_alarm_struct
{
  /* next alarm, all na_ fields are derived from the alarm information */
  uint16_t na_week_time_in_minutes;
  uint16_t na_minutes_diff;	/* time in minutes until next alarm */
  uint8_t na_h;
  uint8_t na_m;
  uint8_t na_wd;	/* 0...6, 0=monday */
  
  /* alarm information */
  uint8_t enable;
  uint8_t snooze_count;
  uint8_t skip_wd;
  uint8_t h;
  uint8_t m;
  uint8_t wd[7];	/* 0 or 1, 0=weekday not selected */
  
  
};
typedef struct _gui_alarm_struct gui_alarm_t;


uint8_t gui_alarm_index = 0;
gui_alarm_t gui_alarm_current;
gui_alarm_t gui_alarm_list[GUI_ALARM_CNT];
char gui_alarm_str[GUI_ALARM_CNT][8];

gui_data_t gui_data;

menu_t gui_menu;


/*============================================*/

void gui_alarm_calc_next_alarm(uint8_t idx, uint16_t current_week_time_in_minutes)
{
  uint8_t i;
  uint16_t week_time_abs;
  uint16_t week_time_diff;	/* difference to current_week_time_in_minutes */
  uint16_t best_diff = 0x0ffff;
  gui_alarm_list[idx].na_week_time_in_minutes = 0x0ffff;		/* not found */
  gui_alarm_list[idx].na_minutes_diff = 0x0ffff;				/* not found */
  gui_alarm_list[idx].na_wd = 7;						/* not found */
  if ( gui_alarm_list[idx].enable != 0 )
  {
    for( i = 0; i < 7; i++ )
    {
      if ( gui_alarm_list[idx].wd[i] != 0 )
      {
	if ( gui_alarm_list[idx].skip_wd != i )
	{
	  week_time_abs = 24*i + gui_alarm_list[idx].h*60 + gui_alarm_list[idx].m;
	  week_time_abs += gui_alarm_list[idx].snooze_count*SNOOZE_MINUTES;
	  
	  if ( current_week_time_in_minutes <= week_time_abs )
	    week_time_diff = week_time_abs - current_week_time_in_minutes;
	  else
	    week_time_diff = week_time_abs + 7*24*60 - current_week_time_in_minutes;
	    
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
    gui_data.weekday = get_weekday_by_year_day_number(year, ydn);
    
    //cdn = to_century_day_number(y, ydn);
    //to_minutes(cdn, h, m);
}

/*
  calculate the minute within the week.
  this must be called after gui_date_adjust(), because the weekday is used here
*/
void gui_calc_week_time(void)
{
  gui_data.week_time = gui_data.weekday*24 + gui_data.h * 60 + gui_data.mt * 10 + gui_data.mo;
}

/*
  calculate the next alarm.
  this must be called after gui_calc_week_time() because, we need week_time
*/
void gui_calc_next_alarm()
{
  uint8_t i;
  uint8_t lowest_i;
  uint16_t lowest_diff;
  /* step 1: Calculate the difference to current weektime for each alarm */
  /* result is stored in gui_alarm_list[i].na_minutes_diff */
  for( i = 0; i < GUI_ALARM_CNT; i++ )
    gui_alarm_calc_next_alarm(i, gui_data.week_time);
  
  /* step 2: find the index with the lowest difference */
  lowest_diff = 0x0ffff;
  lowest_i = GUI_ALARM_CNT;
  for( i = 0; i < GUI_ALARM_CNT; i++ )
  {
    if ( lowest_diff < gui_alarm_list[i].na_minutes_diff )
    {
      lowest_diff = gui_alarm_list[i].na_minutes_diff;
      lowest_i = i;
    }
  }
  
  /* step 3: store the result */
  gui_data.next_alarm_index = lowest_i;  /* this can be GUI_ALARM_CNT */
}


void gui_Init(u8g2_t *u8g2)
{
  int i;
  menu_Init(&gui_menu, u8g2);
  menu_SetMEList(&gui_menu, melist_top_menu, 0);
  gui_date_adjust();
  gui_calc_week_time();
  for ( i = 0; i < GUI_ALARM_CNT; i++ )
  {
    gui_alarm_calc_str_time(i);
  }
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

const static uint8_t ok_xbm[] = {
   0xfe, 0x7f, 0x03, 0xc0, 0x01, 0x80, 0x01, 0xb8, 0x01, 0x9c, 0x01, 0x8e,
   0x01, 0x87, 0x01, 0x87, 0x9d, 0x83, 0xb9, 0x83, 0xf1, 0x81, 0xe1, 0x81,
   0xc1, 0x80, 0x01, 0x80, 0x03, 0xc0, 0xfe, 0x7f };

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
    menu_SetMEList(menu, melist_top_menu, 0);
    return 1;
  }
  return 0;
}

#define ME_TIME_Y 20
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


int me_action_save_date(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    gui_date_adjust();
    menu_SetMEList(menu, melist_top_menu, 0);
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


#define ME_ALARM_TIME_XO 28
#define ME_ALARM_TIME_Y 20
#define ME_ALARM_WD_Y 29
#define ME_ALARM_WD_XO 8

int me_action_alarm_done(menu_t *menu, const me_t *me, uint8_t msg)
{
  if ( msg == ME_MSG_SELECT )
  {
    gui_alarm_list[gui_alarm_index] = gui_alarm_current;
    gui_alarm_calc_str_time(gui_alarm_index);
    menu_SetMEList(menu, melist_alarm_menu, gui_alarm_index);
    return 1;
  }
  return 0;
}

const char weekdaystr[7][4] = {
  "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"
};

const me_t melist_setup_alarm[] = 
{
  { me_cb_big_toggle, &(gui_alarm_current.enable), NULL, 		4	, 6},
  
  { me_cb_0_23, &(gui_alarm_current.h), NULL, 		ME_ALARM_TIME_XO+2,ME_ALARM_TIME_Y },
  { me_cb_num_label, NULL, ":", 	ME_ALARM_TIME_XO+30,ME_ALARM_TIME_Y-3 },
  { me_cb_0_55, &(gui_alarm_current.m), NULL, 		ME_ALARM_TIME_XO+39,ME_ALARM_TIME_Y },
  
  { me_cb_16x16_bitmap_button, (void *)me_action_alarm_done, (void *)ok_xbm, ME_ALARM_TIME_XO+80,ME_ALARM_TIME_Y-17 },

  
  { me_cb_wd_toggle, &(gui_alarm_current.wd[0]), (void *)weekdaystr[0], 	ME_ALARM_WD_XO+17*0, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[1]), (void *)weekdaystr[1], 	ME_ALARM_WD_XO+17*1, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[2]), (void *)weekdaystr[2], 	ME_ALARM_WD_XO+17*2, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[3]), (void *)weekdaystr[3], 	ME_ALARM_WD_XO+17*3, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[4]), (void *)weekdaystr[4], 	ME_ALARM_WD_XO+17*4, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[5]), (void *)weekdaystr[5], 	ME_ALARM_WD_XO+17*5, ME_ALARM_WD_Y},
  { me_cb_wd_toggle, &(gui_alarm_current.wd[6]), (void *)weekdaystr[6], 	ME_ALARM_WD_XO+17*6, ME_ALARM_WD_Y},
  
  { me_cb_null, NULL, NULL, 0, 0 },
};

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

const me_t melist_alarm_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_alarm1, gui_alarm_str[0], 0,10 },
  { me_cb_button_half_line, (void *)me_action_alarm2, gui_alarm_str[1], 64,10 },
  { me_cb_button_half_line, (void *)me_action_alarm3, gui_alarm_str[2], 0,20 },
  { me_cb_button_half_line, (void *)me_action_alarm4, gui_alarm_str[3], 64,20 },
  { me_cb_button_full_line, (void *)me_action_to_top_menu, "Zurück", 40,30 },
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
    menu_SetMEList(menu, melist_setup_menu, 3);
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

const me_t melist_setup_menu[] = 
{
  { me_cb_button_half_line, (void *)me_action_setup_time, "Uhrzeit", 0,10 },
  { me_cb_button_half_line, (void *)me_action_setup_date, "Datum", 64,10 },
  { me_cb_button_half_line, (void *)NULL, "Power", 0,20 },
  { me_cb_button_half_line, (void *)me_action_goto_alarm_list, "Alarme", 64,20 },
  { me_cb_button_full_line, (void *)me_action_to_top_menu, "Zurück", 40,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};


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

const me_t melist_top_menu[] = 
{
  { me_cb_button_full_line, NULL, "Zurück", 3,10 },
  { me_cb_button_full_line, (void *)me_action_to_alarm_menu, "Alarm", 3,20 },
  { me_cb_button_full_line, (void *)me_action_to_setup_menu, "Einstellungen", 3,30 },
  { me_cb_null, NULL, NULL, 0, 0 },
};
