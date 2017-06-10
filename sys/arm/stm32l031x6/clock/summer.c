/*

  summer.c 
  
  stm32l0 daylight saving time calculation
  
*/

#include "datecalc.h"
#include "stm32l031xx.h"


/*
  this function does not require gui_date_adjust() to be called
  0: Summertime
  1: Wintertime
  -1: unknown (last sunday in oct, between 2am and 3am 
*/
int is_summer_time(void)
{
  uint16_t ydn;
  uint16_t year;
  uint16_t month;
  uint16_t day;
  uint16_t hour;
  uint16_t ydn_change_to_wintertime;
  uint16_t ydn_change_to_summertime;

  /* read date from date register */
  
  year = ((RTC->DR >> 20) & 15);
  year *= 10;
  year += ((RTC->DR >> 16) & 15);
  year += 2000;
  
  month = ((RTC->DR >> 12) & 1);
  month *= 10;
  month += ((RTC->DR >> 8) & 15);
  
  day = ((RTC->DR >> 4) & 3);
  day *= 10;
  day += ((RTC->DR) & 15);
  
  /* also get the hour for later comparison */
  
  hour = ((RTC->TR >> 20) & 3);  /* assuming 24-hour clock, not sure about PM flag */
  hour *= 10;
  hour += ((RTC->TR >> 16) & 15);
  
  /* convert all dates to date numbers of the current year*/
  
  ydn = get_year_day_number(year, month, day);
  ydn_change_to_wintertime = get_year_day_number(year, 10, 31);
  ydn_change_to_wintertime -= get_weekday_by_year_day_number(year, ydn_change_to_wintertime);
  ydn_change_to_summertime = get_year_day_number(year, 3, 31);
  ydn_change_to_summertime -= get_weekday_by_year_day_number(year, ydn_change_to_summertime);
  
  if ( ydn == ydn_change_to_summertime )
    if ( hour >= 2 )
      return 1;	/* vorstellen */

  if ( ydn == ydn_change_to_wintertime )
  {
    if ( hour < 2 )
      return 1; /* vorstellen, immer noch sommerzeit */
    if ( hour > 3 )
      return 0; /* jetzt ist wieder Winterzeit */
    return -1; /* zwischen 2 und 3 Uhr wissen wir nichts... */
  }

  if ( ydn > ydn_change_to_summertime && ydn < ydn_change_to_wintertime )
    return 1;
    
  return 0;
}


