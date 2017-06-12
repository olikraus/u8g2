/*

  gui.h
  
*/

#ifndef _GUI_H
#define _GUI_H

#include "menu.h"


extern u8g2_t u8g2;

#define GUI_STATE_STOP 0
#define GUI_STATE_SIGNAL_ALARM 1
#define GUI_STATE_DISPLAY_TIME 2
#define GUI_STATE_MENU 3

#define GUI_ALARM_CNT 4
#define SNOOZE_MINUTES 5
#define ALLOW_SKIP_HOURS 10

struct _gui_data
{
  uint16_t uptime;				/* uptime in days, 10 bits, counts from 0 to 999, this value will be stored in the backup register */
  
  
  uint16_t week_time;			/* calculated: derived from h, mt, mo and weekday */
  uint8_t gui_state;				/* global running state, see guistate.c, defaults to 0 (GUI_STATE_STOP) */
  uint8_t h, mt, mo, st, so;		/* input: current time */
  uint8_t day; 					/* input: 1 .. 31 current day in month */
  uint8_t last_day;				/* last day. This is used to check, whether the day has changed. Required for uptime calc. This is also stored in the backup register. */
  uint8_t month;				/* input: 1..12 */
  uint8_t year_t, year_o;			/* input: current year */
  uint8_t weekday; 				/* calculated: 0 = Monday */
  
  uint8_t next_alarm_index;	/* calculated: index for the next alarm or GUI_ALARM_CNT if there is no next alarm */
  uint8_t is_skip_possible;		/* calculated: whether the next alarm (next_alarm_index) can be skipped */
  
  uint8_t is_equal;			/* calculated: whether the current time matches any alarm, will be set to 0 automatically */
  uint8_t equal_h;
  uint8_t equal_mt;
  uint8_t equal_mo;
  
  uint8_t is_alarm;			/* input/calculated: set by the software, has to be reset by the user */
  uint8_t active_alarm_idx;	/* input/calculated: set by the software, has to be reset by the user */
  uint8_t contrast;			/* value 1..7, 0 is default (do not set) */
  uint8_t display_voltage;		
  
  char s[16];				/* string buffer */
};
typedef struct _gui_data gui_data_t;


struct _gui_alarm_struct
{
  /* next alarm, all na_ fields are derived from the alarm information */
  uint16_t na_week_time_in_minutes;
  uint16_t na_minutes_diff;		/* calculated: time in minutes until next alarm, 0x0ffff = no alarm */
  uint8_t na_h;					/* calculated */
  uint8_t na_m;					/* calculated */
  uint8_t na_wd;				/* calculated: 0...7, 0=monday, 7=no alarm */
  
  /* alarm information */
  uint8_t snooze_count;	/* input, 1 bit*/
  volatile uint8_t enable;		/* input, 1 bit */
  uint8_t skip_wd;		/* input 0 = no skip, 1 = Monday, ... 3 bits*/
  uint8_t h;			/* input 5 bits */
  uint8_t m;			/* input 6 bits */
  uint8_t wd[7];		/* input: 0 or 1, 0=weekday not selected, 7 bits */
};
typedef struct _gui_alarm_struct gui_alarm_t;



/* guimenu.c */

extern const me_t melist_setup_time[];
extern const me_t melist_display_time[];
extern const me_t melist_setup_date[];
extern const me_t melist_setup_alarm[];
extern const me_t melist_alarm_menu[];
extern const me_t melist_setup_menu[];
extern const me_t melist_active_alarm_menu[];
extern const me_t melist_top_menu[];

/* guifn.c */

extern gui_alarm_t gui_alarm_list[GUI_ALARM_CNT];
extern char gui_alarm_str[GUI_ALARM_CNT][8];
extern gui_data_t gui_data;
extern menu_t gui_menu;

void gui_date_adjust(void);


void gui_LoadData(void);
void gui_Recalculate(void);
void gui_SignalTimeChange(void);
void gui_Init(u8g2_t *u8g2, uint8_t is_por);
void gui_Draw(void);
void gui_Next(void);
void gui_Select(void);

/* guihal.c */

void do_reset(void);
void store_gui_data(uint32_t *data);
void load_gui_data(uint32_t *data);
int is_por_reset(void);
int is_button_reset(void);
uint32_t get_boot_status_register(void);
uint32_t get_pwr_status_register(void);
uint32_t get_reset_reason(void);
uint32_t get_wakeup_count(void);
uint32_t get_dst_by_date(void);
uint32_t get_dst_by_RTC(void);

void enable_alarm(void);
void disable_alarm(void);
void set_time(uint8_t ht, uint8_t ho, uint8_t mt, uint8_t mo, uint8_t st, uint8_t so);
void set_date(uint8_t yt, uint8_t yo, uint8_t mt, uint8_t mo, uint8_t dayt, uint8_t dayo, uint8_t weekday);
void set_contrast(void);  /* set contrast to gui_data.contrast, value 1..7, 0 is default (do not set) */


#endif

