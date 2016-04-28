/*

  u8g_u8toa.c

  
*/


#include "u8g2.h"

static const unsigned char u8g2_u8toa_tab[3]  = { 100, 10, 1 } ;
const char *u8g2_u8toap(char * dest, uint8_t v)
{
  uint8_t pos;
  uint8_t d;
  uint8_t c;
  for( pos = 0; pos < 3; pos++ )
  {
      d = '0';
      c = *(u8g2_u8toa_tab+pos);
      while( v >= c )
      {
	v -= c;
	d++;
      }
      dest[pos] = d;
  }  
  dest[3] = '\0';
  return dest;
}

/* v = value, d = number of digits */
const char *u8g2_u8toa(uint8_t v, uint8_t d)
{
  static char buf[4];
  d = 3-d;
  return u8g2_u8toap(buf, v) + d;
}

