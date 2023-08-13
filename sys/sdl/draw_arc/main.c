/*

  test procedure for a draw arc procedure

  screenshot conversion:
  convert -flip -resize 256x128 screenshot.tga screenshot.png

*/


#include "u8g2.h"
#include <stdio.h>
#include <math.h>


typedef float (*u8g2_atan2f_t)(float, float);


//static const float M_PI_2 = 1.57079632679489661923;
//static const float M_PI_4 = 0.78539816339744830962;


float myatan(float a, float b)
{
  return atan2f(a, b);
}

/*
#define C360 360
#define C270 270
#define C180 180
#define C90 90
#define C45 45
*/

#define C360 256
#define C270 192
#define C180 128
#define C90 64
#define C45 32



void u8g2_draw_arc(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad_in, u8g2_uint_t rad_out, u8g2_uint_t angle_start, u8g2_uint_t angle_end, u8g2_atan2f_t atan2f_func)
{
  // Declare variables
  u8g2_long_t x, y, d, r, as, ae, cnt, num_pts;

  // Manage angle inputs
  if(angle_start == angle_end) return;
  uint8_t inverted = (angle_start > angle_end);
  as = inverted ? angle_end : angle_start;
  ae = inverted ? angle_start : angle_end;

  // Trace each arc radius with the Andres circle algorithm
  for(r = rad_in; r <= rad_out; r++)
  {
    x = 0;
    y = r;
    d = r - 1;
    cnt = -1;
    num_pts = atan2f_func ? 100 : (r * 8 / 10); // if no atan2f() function is provided, we make a low cost approximation of the number of pixels drawn for a 1/8th circle of radius r
    
    // Process each pixel of a 1/8th circle of radius r
    while (y >= x)
    {
      // If atan2f() function is provided, get the percentage of 1/8th circle drawn, otherwise count the drawn pixels
      cnt = atan2f_func ? ((M_PI_2 - atan2f_func(y, x)) * 100 / M_PI_4) : (cnt + 1);

      // Fill the pixels of the 8 sections of the circle, but only on the arc defined by the angles (start and end)
      if((cnt > num_pts * as / C45 && cnt <= num_pts * ae / C45) ^ inverted) u8g2_DrawPixel(u8g2, x0 + y, y0 - x);
      if((cnt > num_pts * (C90 - ae) / C45 && cnt <= num_pts * (C90 - as) / C45) ^ inverted) u8g2_DrawPixel(u8g2, x0 + x, y0 - y);
      if((cnt > num_pts * (as - C90) / C45 && cnt <= num_pts * (ae - C90) / C45) ^ inverted) u8g2_DrawPixel(u8g2, x0 - x, y0 - y);
      if((cnt > num_pts * (C180 - ae) / C45 && cnt <= num_pts * (C180 - as) / C45) ^ inverted) u8g2_DrawPixel(u8g2, x0 - y, y0 - x);
      if((cnt > num_pts * (as - C180) / C45 && cnt <= num_pts * (ae - C180) / C45) ^ inverted) u8g2_DrawPixel(u8g2, x0 - y, y0 + x);
      if((cnt > num_pts * (C270 - ae) / C45 && cnt <= num_pts * (C270 - as) / C45) ^ inverted) u8g2_DrawPixel(u8g2, x0 - x, y0 + y);
      if((cnt > num_pts * (as - C270) / C45 && cnt <= num_pts * (ae - C270) / C45) ^ inverted) u8g2_DrawPixel(u8g2, x0 + x, y0 + y);
      if((cnt > num_pts * (C360 - ae) / C45 && cnt <= num_pts * (C360 - as) / C45) ^ inverted) u8g2_DrawPixel(u8g2, x0 + y, y0 + x);

      // Run Andres circle algorithm to get to the next pixel
      if (d >= 2 * x)
      {
        d = d - 2 * x - 1;
        x = x + 1;
      } else if (d < 2 * (r - y))
      {
        d = d + 2 * y - 1;
        y = y - 1;
      } else
      {
        d = d + 2 * (y - x - 1);
        y = y - 1;
        x = x + 1;
      }
    }
  }
}
  
u8g2_t u8g2;

int main(void)
{
  int x, y;
  int k;
  int i;
  
  u8g2_uint_t rad_in = 10;
  u8g2_uint_t rad_out = 14;
  
  u8g2_uint_t angle_start = 0;
  u8g2_uint_t angle_end = 60;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */
  
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_helvB18_tn);
  
  x = 50;
  y = 30;

  
  for(;;)
  {
    
    
    u8g2_FirstPage(&u8g2);
    do
    {
      //u8g2_SetFontDirection(&u8g2, 0);
      //u8g2_DrawStr(&u8g2, x, y, "123");

      //u8g2_draw_arc(&u8g2, x, y, rad_in, rad_out, angle_start, angle_end, myatan);
      u8g2_draw_arc(&u8g2, x, y, rad_in, rad_out, angle_start, angle_end, 0);

      
    } while( u8g2_NextPage(&u8g2) );
    
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 273 ) y -= 7;
    if ( k == 274 ) y += 7;
    if ( k == 276 ) x -= 7;
    if ( k == 275 ) x += 7;

    if ( k == 'o' ) rad_out -= 1;
    if ( k == 'p' ) rad_out += 1;

    if ( k == 'u' ) rad_in -= 1;
    if ( k == 'i' ) rad_in += 1;

    if ( k == 'k' ) angle_end -= 1;
    if ( k == 'l' ) angle_end += 1;

    
    if ( k == 'e' ) y -= 1;
    if ( k == 'x' ) y += 1;
    if ( k == 's' ) x -= 1;
    if ( k == 'd' ) x += 1;
    if ( k == 'q' ) break;

    if ( k == 't' ) 
      u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2), "screenshot.tga");

    printf("rad_in=%d rad_out=%d angle_end=%d\n", rad_in, rad_out, angle_end);
  }
  
  return 0;
}

