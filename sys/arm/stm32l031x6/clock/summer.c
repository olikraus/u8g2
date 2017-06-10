/*

  summer.c 
  
  stm32l0 daylight savings time (DST) calculation
 
  US:
  Der Energy Policy Act of 2005 (Public Law 109-58) bestimmt in Sec. 110 mit dem Titel Daylight Savings, 
  dass ab 2007 die Sommerzeit am zweiten Sonntag im März beginnt und am ersten Sonntag im November endet.  
  

  EU:
  Central European Summer Time (CEST, britisch) oder Central European Daylight Saving Time
  (applies to almost all european countries)
  
  Die mitteleuropäische Sommerzeit beginnt jeweils am letzten Sonntag im März um 2:00 Uhr MEZ, 
  indem die Stundenzählung um eine Stunde von 2:00 Uhr auf 3:00 Uhr vorgestellt wird. Sie endet jeweils am letzten Sonntag im Oktober um 
  3:00 Uhr MESZ, indem die Stundenzählung um eine Stunde von 3:00 Uhr auf 2:00 Uhr zurückgestellt wird. Die Stunde von 2:00 Uhr bis 3:00 Uhr erscheint im Herbst also zweimal. 
*/

#ifdef __unix
#include <stdio.h>
#include <stdint.h>

static uint8_t is_leap_year(uint16_t y)
{
   if ( 
          ((y % 4 == 0) && (y % 100 != 0)) || 
          (y % 400 == 0) 
      )
      return 1;
   return 0;
}

uint16_t get_year_day_number(uint16_t y, uint8_t m, uint8_t d)
{
  uint8_t tmp1; 
  uint16_t tmp2;
  tmp1 = 0;
  if ( m >= 3 )
    tmp1++;
  tmp2 = m;
  tmp2 +=2;
  tmp2 *=611;
  tmp2 /= 20;
  tmp2 += d;
  tmp2 -= 91;
  tmp1 <<=1;
  tmp2 -= tmp1;
  if ( tmp1 != 0 )
    tmp2 += is_leap_year(y);
  return tmp2;
}

uint8_t get_weekday_by_year_day_number(uint16_t y, uint16_t ydn)
{
  uint8_t j, c, tmp8;
  uint16_t tmp16;
  y--;
  j = y % 100;
  c = y / 100;
  tmp16 = c;
  tmp16 *= 5;
  tmp16 += ydn;
  tmp8 = j;
  j >>= 2;
  c >>= 2;
  tmp8 += j;
  tmp8 += c;
  tmp8 += 28;
  tmp16 += tmp8;
  tmp16 %= 7;
  return tmp16;  
}

uint16_t year;
uint16_t month;
uint16_t day;
uint16_t hour;

#else
#include "datecalc.h"
#include "stm32l031xx.h"
#endif


/*
  this function does not require gui_date_adjust() to be called
  0: no DST (Wintertime)
  1: DST (Summertime)
  -1: unknown (last sunday in oct, between 2am and 3am )
  
  region 
  0:		do nothing
  1:		EU
  2:		US
  
*/
int is_dst_by_date(uint8_t region)
{
  uint16_t ydn;
  uint16_t ydn_change_to_wintertime;
  uint16_t ydn_change_to_summertime;


#ifndef __unix
  
  uint16_t year;
  uint16_t month;
  uint16_t day;
  uint16_t hour;


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
  
#endif

  /* convert all dates to date numbers of the current year*/
  
  if ( region == 1 ) /* EU */
  {
    /* European Summer Time: Last Sunday in March and last Sunday in October */
    ydn_change_to_summertime = get_year_day_number(year, 3, 31);
    ydn_change_to_wintertime = get_year_day_number(year, 10, 31);
  }
  else if ( region == 2 ) /* US */
  {
    /* US DST rules: Second Sunday in March and First Sunday in November */
    ydn_change_to_summertime = get_year_day_number(year, 2, 28+is_leap_year(year));
    ydn_change_to_summertime += 14;

    ydn_change_to_wintertime = get_year_day_number(year, 10, 31);
    ydn_change_to_wintertime += 7;
  }
  else
  {
    return 0;
  }
  
  ydn_change_to_summertime -= get_weekday_by_year_day_number(year, ydn_change_to_summertime);
  ydn_change_to_wintertime -= get_weekday_by_year_day_number(year, ydn_change_to_wintertime);

  ydn = get_year_day_number(year, month, day);
  
  if ( ydn == ydn_change_to_summertime )
    if ( hour >= 2 )
      return 1;	/* yes, it is DST */

  if ( ydn == ydn_change_to_wintertime )
  {
    if ( hour < 2 )
      return 1; /* still DST */
    if ( hour > 3 )
      return 0; /* not DST any more */
    return -1; /* not sure whether DST or not */
  }

  if ( ydn > ydn_change_to_summertime && ydn < ydn_change_to_wintertime )
    return 1; 	/* within DST */
    
  return 0;
}


#ifdef __unix

int main()
{
  int dd;
  int d = -2;
   for( year = 2000; year < 2030; year++ )
  {
    printf("%d: ", year);
    for( month = 3; month <= 11; month ++ )
    {
      for( day = 1; day<= 31; day++ )
      {
	hour = 2;
	dd = is_dst_by_date(1);
	if ( d == 0 && dd == 1 )
	{
	  printf("%d.%d.%d", day, month, year);
	}	
	switch(dd)
	{
	  //case -1: printf("%d.%d.%d", day, month, year); break;
	  //case 0: printf("w"); break;
	  //case 1: printf("s"); break;

	}
	d = dd;
      }
    }
    printf("\n");

  }
}

#else

/*
  region 
  0:		do nothing
  1:		EU
  2:		US
  
*/

void adjustDST(uint8_t region)
{
  int is_dst;
  int dst_state;
  
  is_dst = is_dst_by_date(region);
  
  if ( is_dst >= 0 )
  {
    dst_state = 0;
    if ( RTC->CR & RTC_CR_BCK )	/* BKP flag in the CR register */
      dst_state = 1;
    
    if ( is_dst != dst_state )
    {

      __disable_irq();
      
      PWR->CR |= PWR_CR_DBP;		/* disable write protection (step 1) */

      RTC->WPR = 0x0ca;			/* disable RTC write protection (step 2) */
      RTC->WPR = 0x053;
      
      if ( is_dst != 0 )
      {
	RTC->CR |= RTC_CR_BCK;		/* the RTC will now run in summer time (set DST flag) */
	RTC->CR |= RTC_CR_ADD1H;	/* change to DST (Summertime): add one hour */
      }
      else
      {
	RTC->CR &= ~RTC_CR_BCK;	/* the RTC will now run in winter time (turn off DST flag) */
	RTC->CR |= RTC_CR_SUB1H;	/* change back to none DST (Wintertime): subtract one hour */
      }

      RTC->WPR = 0;				/* enable RTC write protection (step 2) */
      RTC->WPR = 0;

      __enable_irq();
      
    }      
  }
}

#endif
