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

#ifndef _DATECALC_H
#define _DATECALC_H

#include <stdint.h>

uint8_t is_leap_year(uint16_t y);
uint16_t get_year_day_number(uint16_t y, uint8_t m, uint8_t d);
uint8_t get_month_by_year_day_number(uint16_t y, uint16_t ydn);
uint8_t get_day_by_year_day_number(uint16_t y, uint16_t ydn);
uint8_t get_weekday_by_year_day_number(uint16_t y, uint16_t ydn); /* returns day within a week: 0..6 with 0 = Sunday, 1 = Monday, ... */
uint16_t to_century_day_number(uint16_t y, uint16_t ydn);
void from_century_day_number(uint16_t cdn, uint16_t *year, uint16_t *ydn);
uint32_t to_time(uint16_t cdn, uint8_t h, uint8_t m, uint8_t s);
void from_time(uint32_t t, uint16_t *cdn, uint8_t *h, uint8_t *m, uint8_t *s);

uint32_t to_sec_since_2000(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi, uint8_t s);

uint32_t to_minutes(uint16_t cdn, uint8_t h, uint8_t m);
void from_minutes(uint32_t t, uint16_t *cdn, uint8_t *h, uint8_t *m);
uint32_t to_minutes_since_2000(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi);


#endif

