/*

  summer.c 
  
  stm32l0 daylight savings time calculation
 
  US:
  Der Energy Policy Act of 2005 (Public Law 109-58) bestimmt in Sec. 110 mit dem Titel Daylight Savings, dass ab 2007 die Sommerzeit am zweiten Sonntag im März beginnt und am ersten Sonntag im November endet.  
  

  Central European Summer Time (CEST, britisch) oder Central European Daylight Saving Time
  (applies to almost all european countries)
  
  Die mitteleuropäische Sommerzeit beginnt jeweils am letzten Sonntag im März um 2:00 Uhr MEZ, 
  indem die Stundenzählung um eine Stunde von 2:00 Uhr auf 3:00 Uhr vorgestellt wird. Sie endet jeweils am letzten Sonntag im Oktober um 
  3:00 Uhr MESZ, indem die Stundenzählung um eine Stunde von 3:00 Uhr auf 2:00 Uhr zurückgestellt wird. Die Stunde von 2:00 Uhr bis 3:00 Uhr erscheint im Herbst also zweimal. 
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


