/*

  u8g2_u16toa.c


  
*/


#include "u8g2.h"

const char *u8g2_u16toap(char * dest, uint16_t v)
{
  uint8_t pos;
  uint8_t d;
  uint16_t c;
  c = 10000;
  for( pos = 0; pos < 5; pos++ )
  {
      d = '0';
      while( v >= c )
      {
	v -= c;
	d++;
      }
      dest[pos] = d;
      c /= 10;
  }  
  dest[5] = '\0';
  return dest;
}

/* v = value, d = number of digits */
const char *u8g2_u16toa(uint16_t v, uint8_t d)
{
  static char buf[6];
  d = 5-d;
  return u8g2_u16toap(buf, v) + d;
}

