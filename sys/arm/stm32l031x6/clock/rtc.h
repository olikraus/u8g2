/*

  rtc.h

*/

#ifndef _RTC_VIEW_H
#define _RTC_VIEW_H

#include "stdint.h"

#define BCD_SEC_UNITS 0
#define BCD_SEC_TENS 1
#define BCD_MIN_UNITS 2
#define BCD_MIN_TENS 3
#define BCD_HOUR_UNITS 4
#define BCD_HOUR_TENS 5
#define BCD_DAY_UNITS 6
#define BCD_DAY_TENS 7
#define BCD_MONTH_UNITS 8
#define BCD_MONTH_TENS 9
#define BCD_YEAR_UNITS 10
#define BCD_YEAR_TENS 11


struct _rtc_struct
{
  uint8_t bcd[12];
  
  uint8_t sec;		/* 0..59 */
  uint8_t min;		/* 0..59 */
  uint8_t hour;		/* 0..23 */
  uint8_t day;		/* 1..31 */
  uint8_t month;	/* 1..12 */
  uint8_t year;		/* 0..99 */
};
typedef struct _rtc_struct rtc_t;

void rtc_register_to_bcd(rtc_t *rtc);
void rtc_bcd_to_ymd_hms(rtc_t *rtc);
void rtc_draw_time(rtc_t *rtc, u8g2_t *u8g2);

#endif