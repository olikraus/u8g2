/*

  u8g2_intersection.c 
  
  code taken from u8g_clip.c

*/

#include "u8g2.h"

#ifdef __GNUC__
#define U8G2_ALWAYS_INLINE __inline__ __attribute__((always_inline))
#else
#define U8G2_ALWAYS_INLINE
#endif 


#ifdef U8G2_WITH_INTERSECTION    


/*
  intersection assumptions:
    a1 <= a2 is always true    
    
    minimized version
    ---1----0 1             b1 <= a2 && b1 > b2
    -----1--0 1             b2 >= a1 && b1 > b2
    ---1-1--- 1             b1 <= a2 && b2 >= a1
  */


//static uint8_t U8G2_ALWAYS_INLINE u8g2_is_intersection_decision_tree(u8g_uint_t a0, u8g_uint_t a1, u8g_uint_t v0, u8g_uint_t v1) 
static uint8_t u8g2_is_intersection_decision_tree(u8g2_uint_t a0, u8g2_uint_t a1, u8g2_uint_t v0, u8g2_uint_t v1) 
{
  if ( v0 <= a1 )
  {
    if ( v1 >= a0 )
    {
      return 1;
    }
    else
    {
      if ( v0 > v1 )
      {
	return 1;
      }
      else
      {
	return 0;
      }
    }
  }
  else
  {
    if ( v1 >= a0 )
    {
      if ( v0 > v1 )
      {
	return 1;
      }
      else
      {
	return 0;
      }
    }
    else
    {
      return 0;
    }
  }
}


uint8_t u8g2_IsIntersection(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t x1, u8g2_uint_t y1)
{
  if ( u8g2_is_intersection_decision_tree(u8g2->user_y0, u8g2->user_y1, y0, y1) == 0 )
    return 0; 
  
  return u8g2_is_intersection_decision_tree(u8g2->user_x0, u8g2->user_x1, x0, x1);
}


#endif /* U8G2_WITH_INTERSECTION */

