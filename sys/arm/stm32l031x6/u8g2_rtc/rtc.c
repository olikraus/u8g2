/*

  rtc.c

  PA0 TAMP2	Button
  PA2 TAMP3	Button

*/


#include <string.h>
#include "stm32l031xx.h"
#include "delay.h"
#include "u8g2.h"
#include "rtc.h"

/* read RTC register into bcd array */
void rtc_register_to_bcd(rtc_t *rtc)
{
  uint32_t r;
  int i;
  
  r = RTC->TR;
  i = 0;
  do
  {
    rtc->bcd[i] = r & 15;
    r >>= 4;
    i++;
  } while( i < 6 );
  
  rtc->bcd[1] &= 7;	/* seconds */
  rtc->bcd[3] &= 7;	/* minutes */
  rtc->bcd[5] &= 3; /* hours */
  
  r = RTC->DR;
  i = 6;
  do
  {
    rtc->bcd[i] = r & 15;
    r >>= 4;
    i++;
  } while( i < 12 );

  rtc->bcd[7] &= 3;	/* days */
  rtc->bcd[9] &= 1;	/* months */  
}

static uint8_t rtc_bcd_to_uint8(rtc_t *rtc, int idx) U8G2_NOINLINE;
static uint8_t rtc_bcd_to_uint8(rtc_t *rtc, int idx)
{
  return rtc->bcd[idx+1]*10 + rtc->bcd[idx];
}

/* convert the content of the bcd array to the ymd&hms vars */
void rtc_bcd_to_ymd_hms(rtc_t *rtc)
{
  rtc->sec = rtc_bcd_to_uint8(rtc, 0);
  rtc->min = rtc_bcd_to_uint8(rtc, 2);
  rtc->hour = rtc_bcd_to_uint8(rtc, 4);
  rtc->day = rtc_bcd_to_uint8(rtc, 6);
  rtc->month = rtc_bcd_to_uint8(rtc, 8);
  rtc->year = rtc_bcd_to_uint8(rtc, 10);
}

void rtc_draw_time(rtc_t *rtc, u8g2_t *u8g2)
{
  char s[10];
  strcpy(s, u8x8_u8toa(rtc->hour, 2));
  strcat(s, ":");
  strcat(s, u8x8_u8toa(rtc->min, 2));
  strcat(s, ":");
  strcat(s, u8x8_u8toa(rtc->sec, 2));
  
  u8g2_SetFont(u8g2, u8g2_font_helvB18_tn);
  u8g2_DrawStr(u8g2, 0,23, s);
  
}