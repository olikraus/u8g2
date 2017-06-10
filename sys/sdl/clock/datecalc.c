/*

  datecalc.c

  Written 1996/96 by Oliver Kraus
  Published by Heinz Heise Verlag 1997 (c't 15/97) 
  Completly rewritten and put under GPL 2011 by Oliver Kraus
  
  (c) 2011 by Oliver Kraus (olikraus@gmail.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  Development goals:
    - English version
    - Optimized for 8 bit microcontroller 
    
  Definitions:
    Short Name: 	y
    Long Name: 	year 
    Range:			2000...
    
    Short Name: 	ydn
    Long Name: 	year day number
    Range:			1..366
    
    Short Name: 	cdn
    Long Name: 	century day number
    Range:			1...65535
    
    Short Name: 	ymd
    Long Name: 	Year, Month, Day
    Range:			2000...65535, 1..12, 1..31
    
  Conversions
    ymd --> y, ydn
      get_year_day_number()
    y, ydn --> ymd
      get_month_by_year_day_number()
      get_day_by_year_day_number()
    y, ydn  --> cdn
      to_century_day_number();
    cdn --> y, ydn
      from_century_day_number();
      

*/

#include <stdint.h>

/*
  Prototype:
    uint8_t is_leap_year(uint16_t y)
  Description:
    Calculate leap year
  Arguments:
    y           year, e.g. 2011 for year 2011
  Result:
    0           not a leap year
    1           leap year
*/
uint8_t is_leap_year(uint16_t y)
{
   if ( 
          ((y % 4 == 0) && (y % 100 != 0)) || 
          (y % 400 == 0) 
      )
      return 1;
   return 0;
}

/*
  Prototype:
    uint16_t get_year_day_number(uint16_t y, uint8_t m, uint8_t d)
  Description:
    Calculate the day number within a year. 1st of Jan has the number 1.
    "Robertson" Algorithm IDAY (CACM Vol 15/#10/Oct 1972)
  Arguments:
    y           year, e.g. 2011 for year 2011
    m           month with 1 = january to 12 = december
    d           day starting with 1
  Result:
    The "day number" within the year: 1 for the 1st of Jan.
  See also:
    get_month_by_day_number()

*/
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

/*
  Prototype:
    uint8_t get_month_by_year_day_number(uint16_t y, uint16_t ydn)
  Description:
    Get the month from year and day number within a year.
    "R. A. Stone" Algorithm (CACM Vol 13/#10/Oct 1970)
  Arguments:
    y           year, e.g. 2011 for year 2011
    ydn	year day number (1st of Jan has the number 1)
  Result:
    The month within the year: 1 for January.
  See also:
    get_year_day_number()
*/

static uint16_t corrected_year_day_number(uint16_t y, uint16_t ydn)
{
   uint8_t a;
   a = is_leap_year(y);
   if ( ydn > 59+a )
   {
      ydn += 2;
      ydn -= a;
   }
   ydn += 91;
   return ydn;
}

uint8_t get_month_by_year_day_number(uint16_t y, uint16_t ydn)
{
  uint8_t a;
  ydn = corrected_year_day_number(y, ydn);
  ydn *= 20;
  ydn /= 611;
  a = ydn;
  a -= 2;
  return a;  
}

/*
  Prototype:
    uint8_t get_day_by_year_day_number(uint16_t y, uint16_t ydn)
  Description:
    Get the day within month from year and day number within a year.
    "R. A. Stone" Algorithm (CACM Vol 13/#10/Oct 1970)
  Arguments:
    y           year, e.g. 2011 for year 2011
    ydn	year day number (1st of Jan has the number 1)
  Result:
    The day within a month: 1 for the first day of a month.
  See also:
    get_year_day_number()
*/
uint8_t get_day_by_year_day_number(uint16_t y, uint16_t ydn)
{
  uint8_t m;
  uint16_t tmp;
  m = get_month_by_year_day_number(y, ydn);
  m += 2;
  ydn = corrected_year_day_number(y, ydn);
  tmp = 611;
  tmp *= m;
  tmp /= 20;
  ydn -= tmp;
  return ydn;
}

/*
  Prototype:
    uint8_t get_weekday_by_year_day_number(uint16_t y, uint16_t ydn)
  Description:
    Get the day within week from year and day number within a year.
    "Zeller" Algorithm
    https://de.wikisource.org/wiki/Index:Acta_Mathematica_vol._009_(1886)
    https://ia801407.us.archive.org/8/items/actamathematica09upps/actamathematica09upps.pdf
  Arguments:
    y           year, e.g. 2011 for year 2011
    ydn	year day number (1st of Jan has the number 1)
  Result:
    The day within a week: 0..6 with 0 = Sunday, 1 = Monday, ...
  See also:
    get_year_day_number()
*/
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

/*
  Prototype:
    uint16_t to_century_day_number(uint16_t y, uint16_t ydn)
  Description:
    Calculate days since January, 1st, 2000
  Arguments:
    y           year, e.g. 2011 for year 2011
    ydn	year day number (1st of Jan has the number 1)
*/
uint16_t to_century_day_number(uint16_t y, uint16_t ydn)
{
  uint16_t cdn;
  cdn = ydn;
  cdn--;
  while( y > 2000 )
  {
    y--;
    cdn += 365;
    cdn += is_leap_year(y);
  }
  return cdn;
}

void from_century_day_number(uint16_t cdn, uint16_t *year, uint16_t *ydn)
{
  uint16_t y, days_per_year;
  y = 2000;
  for(;;)
  {
    days_per_year = 365;
    days_per_year += is_leap_year(y);
    if ( cdn >= days_per_year )
    {
      cdn -= days_per_year;
      y++;
    }
    else
      break;
  }
  cdn++;
  *year = y;
  *ydn = cdn;
}

/*
  Calculate the seconds after 2000-01-01 00:00. The largest possible
  time is 2136-02-07 06:28:15
*/
uint32_t to_time(uint16_t cdn, uint8_t h, uint8_t m, uint8_t s)
{
  uint32_t t;
  t = cdn;
  t *= 24;
  t += h;
  t *= 60;
  t += m;
  t *= 60;
  t += s;
  return t;
}


void from_time(uint32_t t, uint16_t *cdn, uint8_t *h, uint8_t *m, uint8_t *s)
{
  *s = t % 60;
  t /= 60;
  *m = t % 60;
  t /= 60;
  *h = t % 24;
  t /= 24;
  *cdn = t;
}

uint32_t to_sec_since_2000(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi, uint8_t s)
{
  uint16_t ydn = get_year_day_number(y, mo, d);
  uint16_t cdn = to_century_day_number(y, ydn);
  return to_time(cdn, h, mi, s);
}

/*
  Calculate the minutes after 2000-01-01 00:00. 
*/
uint32_t to_minutes(uint16_t cdn, uint8_t h, uint8_t m)
{
  uint32_t t;
  t = cdn;
  t *= 24;
  t += h;
  t *= 60;
  t += m;
  return t;
}


void from_minutes(uint32_t t, uint16_t *cdn, uint8_t *h, uint8_t *m)
{
  *m = t % 60;
  t /= 60;
  *h = t % 24;
  t /= 24;
  *cdn = t;
}

uint32_t to_minutes_since_2000(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi)
{
  uint16_t ydn = get_year_day_number(y, mo, d);
  uint16_t cdn = to_century_day_number(y, ydn);
  return to_minutes(cdn, h, mi);
}




/*
   Eingabe: Jahr
   Ausgabe: Tagesnummer des Ostersonntag, 
            rel. zum Jahresanfang.
   Algorithmus "Computus ecclesiasticus"

   325 n.Chr. wurde Ostern auf den Sonntag nach 
   dem ersten Fruehlingsvollmond festgelegt. Damit 
   liegt Ostern zwischen dem 22. Maerz und
   dem 25. April.
*/
/*
int ostersonntag(int jahr)
{
   int gz, jhd, ksj, korr, so, epakte, n;
   gz = (jahr%19)+1;
   jhd = jahr/100+1;
   ksj = (3*jhd)/4-12;
   korr = (8*jhd+5)/25-5;
   so = (5*jahr)/4-ksj-10;
   epakte = (11*gz+20+korr-ksj) % 30;
   if ( (epakte == 25 && gz > 11) || epakte == 24 )
      epakte++;
   n = 44-epakte;
   if ( n < 21 )
      n = n + 30;
   n = n + 7 - (so+n) % 7;
   n += schaltjahr(jahr);
   return n+59;
}
*/