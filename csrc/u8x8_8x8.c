/*

  u8x8_8x8.c
  
  font procedures, directly interfaces display procedures

*/

#include "u8x8.h"




void u8x8_Set8x8Font(u8x8_t *u8x8, const uint8_t *font_8x8)
{
  u8x8->font = font_8x8;
}

void u8x8_Draw8x8Glyph(u8x8_t *u8x8, uint8_t x, uint8_t y, uint8_t encoding)
{
  uint8_t first, last, i;
  uint8_t buf[8];
  uint16_t offset;
  first = u8x8_pgm_read(u8x8->font+0);
  last = u8x8_pgm_read(u8x8->font+1);
  
  if ( first <= encoding && encoding <= last )
  {
    offset = encoding;
    offset -= first;
    offset *= 8;
    offset +=2;
    for( i = 0; i < 8; i++ )
    {
      buf[i] = u8x8_pgm_read(u8x8->font+offset);
      offset++;
    }
  }
  else
  {
    for( i = 0; i < 8; i++ )
    {
      buf[i] = 0;
    }
  }
  u8x8_display_DrawTile(u8x8, x, y, 1, buf);
}

void u8x8_Draw8x8String(u8x8_t *u8x8, uint8_t x, uint8_t y, const char *s)
{
  while( *s != '\0'  )
  {
    u8x8_Draw8x8Glyph(u8x8, x, y, (uint8_t)*s);
    s++;
    x++;
  }
}

/*
source: https://en.wikipedia.org/wiki/UTF-8
Bits	from 		to			bytes	Byte 1 		Byte 2 		Byte 3 		Byte 4 		Byte 5 		Byte 6
  7 	U+0000 		U+007F 		1 		0xxxxxxx
11 	U+0080 		U+07FF 		2 		110xxxxx 	10xxxxxx
16 	U+0800 		U+FFFF 		3 		1110xxxx 	10xxxxxx 	10xxxxxx
21 	U+10000 	U+1FFFFF 	4 		11110xxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
26 	U+200000 	U+3FFFFFF 	5 		111110xx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
31 	U+4000000 	U+7FFFFFFF 	6 		1111110x 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx  

This function returns 0x0ffff for end of string (0x0ffff is not a unicode glyph)

*/
uint16_t u8x8_get_encoding_from_utf8_string(const char **str) 
{
  uint16_t e;
  uint8_t b;
  b = **str;
  if ( b >= 0xc0 )
  {
    if ( b >= 0xf0 )	/* check for UTF-8 4, 5 and 6-byte sequence */
    {
      b &= 0x01;	/* only consider lowest bit, because only plane 0 (16 bit) is supported with u8glib v2 */
    }
    else if ( b >= 0xe0 )	/* check for UTF-8 3-byte sequence */
    {
      b &= 0x0f;
    }
    else /* assume UTF-8 2-byte sequence */
    {
      b &= 0x1f;
    }
    e = b;    
    for(;;)
    {
      (*str)++;
      b = **str;
      if ( (b & 0x0c0) != 0x080 )
	break;
      b &= 0x3f;
      e <<=6;
      e |= b;
    }      
  }
  else
  {
    e = b;		/* init with ASCII code */
    (*str)++;
  }
  return e;
}

const uint8_t u8x8_draw_string(u8x8_t *u8x8, uint8_t x, uint8_t y, const char *s) U8X8_NOINLINE;
const uint8_t u8x8_draw_string(u8x8_t *u8x8, uint8_t x, uint8_t y, const char *s)
{
  uint16_t c;
  uint8_t cnt = 0;
  for(;;)
  {
    c = u8x8->char_cb(&s);
    if ( c == 0 )
      break;
    if ( c <= 255 )
    {
      u8x8_Draw8x8Glyph(u8x8, x, y, (uint8_t)c);
      x++;
      cnt++;
    }
  }
  return cnt;
}

uint8_t u8x8_Draw8x8UTF8(u8x8_t *u8x8, uint8_t x, uint8_t y, const char *s)
{
  u8x8->char_cb = u8x8_get_encoding_from_utf8_string;
  return u8x8_draw_string(u8x8, x, y, s);
}


/*
void u8x8_Draw8x8UTF8(u8x8_t *u8x8, uint8_t x, uint8_t y, const char *s)
{
  uint16_t unicode;
  for(;;)
  {
    unicode = u8x8_get_encoding_from_utf8_string(&s);
    if ( unicode == 0 )
      break;
    if ( unicode <= 255 )
    {
      u8x8_Draw8x8Glyph(u8x8, x, y, (uint8_t)unicode);
      x++;
    }
  }
}
*/




