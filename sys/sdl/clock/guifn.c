/*

  guifn.c
  
*/

#include "gui.h"
#include "datecalc.h"
#include <string.h>



/*============================================*/

gui_alarm_t gui_alarm_list[GUI_ALARM_CNT];
char gui_alarm_str[GUI_ALARM_CNT][8];
uint32_t gui_backup_array[5];


gui_data_t gui_data;

menu_t gui_menu;

/*============================================*/
/* local functions */

uint32_t get_u32_by_alarm_data(gui_alarm_t *alarm);
void set_alarm_data_by_u32(gui_alarm_t *alarm, uint32_t u);
void gui_alarm_calc_next_wd_alarm(uint8_t idx, uint16_t current_week_time_in_minutes);
void gui_alarm_calc_str_time(uint8_t idx) U8G2_NOINLINE;
void gui_date_adjust(void) U8G2_NOINLINE;
void gui_calc_week_time(void);
void gui_calc_next_alarm(void);


/*============================================*/


uint32_t get_u32_by_alarm_data(gui_alarm_t *alarm)
{
  uint32_t u;
  int i;
  u = 0;
  for( i = 0; i < 7; i++ )
  {
    if ( alarm->wd[i] )
    {
      u |= 1<<i;
    }
  }
  /* 17 Sep 2017: fixed 32 bit handling for the next statements */
  u |= ((uint32_t)alarm->m&63) << (7);
  u |= ((uint32_t)alarm->h&31) << (7+6);
  u |= ((uint32_t)alarm->skip_wd&7) << (7+6+5);
  u |= ((uint32_t)alarm->enable&1) << (7+6+5+3);
  u |= ((uint32_t)alarm->snooze_count&1) << (7+6+5+3+1);
  return u;
}

void set_alarm_data_by_u32(gui_alarm_t *alarm, uint32_t u)
{
  int i;
  for( i = 0; i < 7; i++ )
  {
    if ( (u & (1<<i)) != 0 )
      alarm->wd[i] = 1;
    else
      alarm->wd[i] = 0;
  }
  u>>=7;
  alarm->m = u & 63;
  u>>=6;
  alarm->h = u & 31;
  u>>=5;
  alarm->skip_wd =  u & 7;
  u>>=3;
  alarm->enable =  u & 1;
  u>>=1;
  alarm->snooze_count =  u & 1;  
}


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
/* this function must be called after reading from RTC or after setting the input vars by the user */
void gui_date_adjust(void)
{
    uint16_t ydn;
    uint16_t year;
    //uint16_t cdn;
    //uint32_t minutes_since_2000;
  
    if ( gui_data.month == 0 )
      gui_data.month++;
    if ( gui_data.day == 0 )
      gui_data.day++;
    year = 2000+gui_data.year_t*10 + gui_data.year_o;
    ydn = get_year_day_number(year, gui_data.month, gui_data.day);
    //cdn = to_century_day_number(year, ydn);
    //minutes_since_2000 = to_minutes(cdn, gui_data.h, gui_data.mt*10+gui_data.mo);
  
    // maybe adjust time by +/- 1h based on argument given to gui_date_adjust... but then it should be renamed also
  
    gui_data.month = get_month_by_year_day_number(year, ydn);
    gui_data.day = get_day_by_year_day_number(year, ydn);
    gui_data.weekday = get_weekday_by_year_day_number(year, ydn);	/* 0 = Sunday */
    /* adjust the weekday so that 0 will be Monday */
    gui_data.weekday += 6;
    if ( gui_data.weekday >= 7 ) 
      gui_data.weekday -= 7;
    
    if ( gui_data.day != gui_data.last_day )
    {
      gui_data.uptime++;
      gui_data.last_day = gui_data.day;
    }
    
    
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
  uint8_t redo;
  
  do
  {
    redo = 0;
    
    /* step 1: Calculate the difference to current weektime for each alarm */
    /* result is stored in gui_alarm_list[i].na_minutes_diff */
    for( i = 0; i < GUI_ALARM_CNT; i++ )
      gui_alarm_calc_next_wd_alarm(i, gui_data.week_time+(uint16_t)gui_data.is_equal);	/* is_equal flag is used as a offset */
    
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
      
      if ( gui_alarm_list[lowest_i].na_minutes_diff == 0 )
      {
	if ( gui_data.is_equal == 0 )
	{
	  gui_data.is_alarm = 1;
	  gui_data.is_equal = 1;
	  gui_data.active_alarm_idx = lowest_i;
	  //gui_data.active_equal_idx = lowest_i;
	  
	  gui_data.equal_h = gui_data.h;
	  gui_data.equal_mt = gui_data.mt;
	  gui_data.equal_mo = gui_data.mo;
	  
	  redo = 1;
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
  } while( redo );
  
  /* reset the equal flag */
  if ( gui_data.is_equal != 0 )
  {
    if ( gui_data.equal_h != gui_data.h ||
	  gui_data.equal_mt != gui_data.mt ||
	  gui_data.equal_mo != gui_data.mo )
    {
      gui_data.is_equal = 0;
    }
  }
}


/*============================================*/

void gui_LoadData(void)
{
  uint32_t data[5];
  int i;
  
  //printf("Load Data\n");
  
  load_gui_data(data);
  for( i = 0; i < GUI_ALARM_CNT; i++ )
  {
    set_alarm_data_by_u32(gui_alarm_list+i, data[i]);
  }
  gui_data.uptime = data[4] & (uint32_t)0x03ff;
  gui_data.last_day =  (data[4]>>10) & (uint32_t)31;
  gui_data.contrast = (data[4]>>15) & (uint32_t)7;
  gui_data.display_voltage = (data[4]>>16) & (uint32_t)1;
}

void gui_StoreData(void)
{
  uint32_t data[5];
  int i;
  for( i = 0; i < GUI_ALARM_CNT; i++ )
  {
    data[i] = get_u32_by_alarm_data(gui_alarm_list+i);
    //printf("%d: %08lx\n", i, data[i]);
  }
  data[4] = 0;
  data[4] |= gui_data.uptime & (uint32_t)0x03ff;	/* 0...1023 */
  data[4] |= (gui_data.last_day & (uint32_t)31)<<10;
  data[4] |= (gui_data.contrast & (uint32_t)7)<<15;
  data[4] |= (gui_data.display_voltage & (uint32_t)1)<<16;
  
  store_gui_data(data);
}



/* recalculate all internal data */
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
  gui_StoreData();
}

/* minute and/or hour has changed */
/* additionally the active alarm menu might be set by this function */
void gui_SignalTimeChange(void)
{
  /* recalculate dependent values */
  gui_Recalculate();
  
  /* setup menu */
  menu_SetMEList(&gui_menu, melist_display_time, 0);
  
  /* enable alarm */
  if ( gui_data.is_alarm != 0 )
  {
    menu_SetMEList(&gui_menu, melist_active_alarm_menu, 0);
    enable_alarm();
  }
}

void gui_Init(u8g2_t *u8g2, uint8_t is_por)
{
  if ( is_por == 0 )
  {
    /* not a POR reset, so load current values */
    gui_LoadData();
    /* do NOT init the display, otherwise there will be some flicker visible */
    /* however, the GPIO subsystem still has to be setup, so call the other init procedures */
    /* this acts like a warm start for the display */
    /* the display setup code for the display is NOT send */
    u8x8_gpio_Init(u8g2_GetU8x8(u8g2));
    u8x8_cad_Init(u8g2_GetU8x8(u8g2));
    u8x8_gpio_SetReset(u8g2_GetU8x8(u8g2), 1);

    //u8g2_InitDisplay(u8g2);
    //u8x8_d_helper_display_init(u8g2_GetU8x8(u8g2));

    // u8g2_SetPowerSave(u8g2, 0);   // this will be done later
  }
  else
  {
    /* POR reset, so do NOT load any values (they will be 0 in the best case) */
    /* instead do a proper reset of the display */
    // u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8g2_InitDisplay(u8g2);
    
    // u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
    u8g2_SetPowerSave(u8g2, 0);
  }
  
  menu_Init(&gui_menu, u8g2);
  
  gui_SignalTimeChange();
}


void gui_Draw(void)
{
    menu_Draw(&gui_menu);
}

void gui_Next(void)
{
  if ( gui_menu.me_list == melist_active_alarm_menu )
  {
    disable_alarm();
    if ( gui_alarm_list[gui_data.active_alarm_idx].snooze_count != 0 )
    {
      int i;
      /* this is already the snooze alarm, so clear all and behave like the normal alarm off */
      for( i = 0; i < GUI_ALARM_CNT; i++ )
	gui_alarm_list[i].snooze_count = 0;
    }
    else
    {
      /* enable snooze */
      gui_alarm_list[gui_data.active_alarm_idx].snooze_count = 1;
    }
    gui_data.is_alarm = 0;
    gui_Recalculate();
    menu_SetMEList(&gui_menu, melist_display_time, 0);
  }
  else
  {
    menu_NextFocus(&gui_menu);
  }
}

void gui_Select(void)
{
  if ( gui_menu.me_list == melist_active_alarm_menu )
  {
    int i;
    disable_alarm();
    for( i = 0; i < GUI_ALARM_CNT; i++ )
      gui_alarm_list[i].snooze_count = 0;
    gui_data.is_alarm = 0;
    gui_Recalculate();
    menu_SetMEList(&gui_menu, melist_display_time, 0);
  }
  else
  {
    menu_Select(&gui_menu);
  }
}
