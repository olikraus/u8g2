

#include "u8g2.h"

/*
  draw a filled box
  restriction: does not work for w = 0 or h = 0
*/
void u8g2_DrawBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
{
#ifdef U8G2_WITH_INTERSECTION
  if ( u8g2_IsIntersection(u8g2, x, y, x+w, y+h) == 0 ) 
    return;
#endif /* U8G2_WITH_INTERSECTION */
  do
  { 
    u8g2_DrawHVLine(u8g2, x, y, w, 0);
    y++;    
    h--;
  } while( h != 0 );
}


/*
  draw a frame (empty box)
  restriction: does not work for w = 0 or h = 0
  ToDo:
    pixel in the corners are drawn twice. This could be optimized.
*/
void u8g2_DrawFrame(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
{
  u8g2_uint_t xtmp = x;
  
#ifdef U8G2_WITH_INTERSECTION
  if ( u8g2_IsIntersection(u8g2, x, y, x+w, y+h) == 0 ) 
    return;
#endif /* U8G2_WITH_INTERSECTION */
  
  u8g2_DrawHVLine(u8g2, x, y, w, 0);
  u8g2_DrawHVLine(u8g2, x, y, h, 1);
  x+=w;
  x--;
  u8g2_DrawHVLine(u8g2, x, y, h, 1);
  y+=h;
  y--;
  u8g2_DrawHVLine(u8g2, xtmp, y, w, 0);
}
