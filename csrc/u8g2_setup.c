/*

  u8g2_setup.c
  
*/

#include "u8g2.h"
#include <string.h>

/*============================================*/

void u8g2_Setup(u8g2_t *u8g2, uint8_t *buf, uint8_t tile_buf_height, const u8g2_cb_t *u8g2_cb)
{
  u8g2->tile_buf_ptr = buf;
  u8g2->tile_buf_height = tile_buf_height;
  
  u8g2->tile_curr_row = 0;
  u8g2->draw_color = 1;
  
  u8g2->cb = u8g2_cb;
  u8g2->cb->update(u8g2);

#ifdef U8G2_WITH_FONT_ROTATION  
  u8g2->font_decode.dir = 0;
#endif
}


/*============================================*/
/* 
  update dimension: 
  calculate the following variables:
    u8g2_uint_t buf_x0;	left corner of the buffer
    u8g2_uint_t buf_x1;	right corner of the buffer (excluded)
    u8g2_uint_t buf_y0;
    u8g2_uint_t buf_y1;
*/

static void u8g2_update_dimension_common(u8g2_t *u8g2)
{
  u8g2_uint_t t;
  
  t = u8g2->tile_buf_height;
  t *= 8;
  u8g2->pixel_buf_height = t;
  
  t = u8g2_GetU8x8(u8g2)->display_info->tile_width;
  t *= 8;
  u8g2->pixel_buf_width = t;
  
  t = u8g2->tile_curr_row;
  t *= 8;
  u8g2->pixel_curr_row = t;
  
  t = u8g2->tile_buf_height;
  /* handle the case, where the buffer is larger than the (remaining) part of the display */
  if ( t + u8g2->tile_curr_row > u8g2_GetU8x8(u8g2)->display_info->tile_height )
    t = u8g2_GetU8x8(u8g2)->display_info->tile_height - u8g2->tile_curr_row;
  t *= 8;
  
  u8g2->buf_y0 = u8g2->pixel_curr_row;   
  u8g2->buf_y1 = u8g2->buf_y0;
  u8g2->buf_y1 += t;

  u8g2->width = u8g2->pixel_buf_width;
  t = u8g2_GetU8x8(u8g2)->display_info->tile_height;
  t *= 8;
  u8g2->height = t;
}

void u8g2_update_dimension_r0(u8g2_t *u8g2)
{
  u8g2_update_dimension_common(u8g2);

  u8g2->user_x0 = 0;
  u8g2->user_x1 = u8g2->pixel_buf_width;
  
  u8g2->user_y0 = u8g2->buf_y0;
  u8g2->user_y1 = u8g2->buf_y1;
  
//  printf("x0=%d x1=%d y0=%d y1=%d\n", 
//      u8g2->user_x0, u8g2->user_x1, u8g2->user_y0, u8g2->user_y1);
}

void u8g2_update_dimension_r1(u8g2_t *u8g2)
{
  u8g2_update_dimension_common(u8g2);
  
  u8g2->width = u8g2->height;
  u8g2->height = u8g2->pixel_buf_width;

  u8g2->user_x0 = u8g2->buf_y0;
  u8g2->user_x1 = u8g2->buf_y1;
  
  u8g2->user_y0 = 0;
  u8g2->user_y1 = u8g2->pixel_buf_width;
  
//  printf("x0=%d x1=%d y0=%d y1=%d\n", 
//      u8g2->user_x0, u8g2->user_x1, u8g2->user_y0, u8g2->user_y1);
}

void u8g2_update_dimension_r2(u8g2_t *u8g2)
{
  u8g2_update_dimension_common(u8g2);

  u8g2->user_x0 = 0;
  u8g2->user_x1 = u8g2->pixel_buf_width;
  
  u8g2->user_y0 = u8g2->height - u8g2->buf_y1;
  u8g2->user_y1 = u8g2->height - u8g2->buf_y0;
  
//  printf("x0=%d x1=%d y0=%d y1=%d\n", 
//      u8g2->user_x0, u8g2->user_x1, u8g2->user_y0, u8g2->user_y1);
}

void u8g2_update_dimension_r3(u8g2_t *u8g2)
{
  u8g2_update_dimension_common(u8g2);
  
  u8g2->width = u8g2->height;
  u8g2->height = u8g2->pixel_buf_width;

  u8g2->user_x0 = u8g2->height - u8g2->buf_y1;
  u8g2->user_x1 = u8g2->height - u8g2->buf_y0;
  
  u8g2->user_y0 = 0;
  u8g2->user_y1 = u8g2->pixel_buf_width;
  
//  printf("x0=%d x1=%d y0=%d y1=%d\n", 
//      u8g2->user_x0, u8g2->user_x1, u8g2->user_y0, u8g2->user_y1);
}

/*============================================*/
extern void u8g2_draw_hv_line_4dir(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);


void u8g2_draw_l90_r0(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
{
  u8g2_draw_hv_line_4dir(u8g2, x, y, len, dir);
}

void u8g2_draw_l90_r1(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
{
  u8g2_uint_t xx, yy;
  
  yy = x;
  
  xx = u8g2->height;
  xx -= y;
  xx--;
  
  dir ++;
  dir &= 3;
  u8g2_draw_hv_line_4dir(u8g2, xx, yy, len, dir);
}

void u8g2_draw_l90_r2(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
{
  u8g2_uint_t xx, yy;

  yy = u8g2->height;
  yy -= y;
  yy--;
  
  xx = u8g2->width;
  xx -= x;
  xx--;
  
  dir +=2;
  dir &= 3;
  u8g2_draw_hv_line_4dir(u8g2, xx, yy, len, dir);
}

void u8g2_draw_l90_r3(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
{
  u8g2_uint_t xx, yy;

  xx = y;
  
  yy = u8g2->height;
  yy -= x;
  yy--;
  
  dir +=3;
  dir &= 3;
  u8g2_draw_hv_line_4dir(u8g2, xx, yy, len, dir);
}



/*============================================*/
const u8g2_cb_t u8g2_cb_r0 = { u8g2_update_dimension_r0, u8g2_draw_l90_r0 };
const u8g2_cb_t u8g2_cb_r1 = { u8g2_update_dimension_r1, u8g2_draw_l90_r1 };
const u8g2_cb_t u8g2_cb_r2 = { u8g2_update_dimension_r2, u8g2_draw_l90_r2 };
const u8g2_cb_t u8g2_cb_r3 = { u8g2_update_dimension_r3, u8g2_draw_l90_r3 };
  
  
  
  
  
  