
#include "u8g2.h"
#include <stdio.h>

  
u8g2_t u8g2;
u8g2_t u8g2_bitmap;


/* width and height must be multipe of 8 */
#define BITMAP_WIDTH 16
#define BITMAP_HEIGHT 16
static unsigned char bitmap_data[BITMAP_WIDTH/8*BITMAP_HEIGHT];


static void u8g2_draw_pixel_horizontal_xbm(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y)
{
  uint16_t offset;
  uint8_t *ptr;
  uint8_t bit_pos, mask;

  bit_pos = x;		/* overflow truncate is ok here... */
  bit_pos &= 7; 	/* ... because only the lowest 3 bits are needed */
  mask = 1;
  mask <<= bit_pos;
  x >>= 3;

  offset = y;		/* y might be 8 or 16 bit, but we need 16 bit, so use a 16 bit variable */
  offset *= u8g2->pixel_buf_width/8;
  offset += x;
  ptr = u8g2->tile_buf_ptr;
  ptr += offset;
  

  if ( u8g2->draw_color <= 1 )
    *ptr |= mask;
  if ( u8g2->draw_color != 1 )
    *ptr ^= mask;
  
}

/*
  x,y		Upper left position of the line within the local buffer (not the display!)
  len		length of the line in pixel, len must not be 0
  dir		0: horizontal line (left to right)
		1: vertical line (top to bottom)
  asumption: 
    all clipping done
*/
void u8g2_ll_hvline_xbm(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
{
  if ( dir == 0 )
  {
    do
    {
      u8g2_draw_pixel_horizontal_xbm(u8g2, x, y);
      x++;
      len--;
    } while( len != 0 );
  }
  else
  {
    do
    {
      u8g2_draw_pixel_horizontal_xbm(u8g2, x, y);
      y++;
      len--;
    } while( len != 0 );
  }
}

void u8g2_update_dimension_xbm(u8g2_t *u8g2)
{
  /* do nothing */
}


const u8g2_cb_t u8g2_cb_xbm = { u8g2_update_dimension_xbm, u8g2_draw_l90_r0 };


int main(void)
{
  int x, y;
  int k;
  int i;
  
  /* setup bitmap */
  
  u8x8_SetupDefaults(u8g2_GetU8x8(&u8g2_bitmap));
  u8g2_SetupBuffer(&u8g2_bitmap, bitmap_data, BITMAP_HEIGHT/8, u8g2_ll_hvline_xbm, &u8g2_cb_xbm);
  u8g2_bitmap.pixel_buf_height = BITMAP_HEIGHT;
  u8g2_bitmap.pixel_buf_width= ((BITMAP_WIDTH+7)/8)*8;
  u8g2_bitmap.pixel_curr_row=0;
  u8g2_bitmap.buf_y0 = 0;
  u8g2_bitmap.buf_y1 = u8g2_bitmap.pixel_buf_height;
  
  u8g2_bitmap.width = u8g2_bitmap.pixel_buf_width;
  u8g2_bitmap.height = u8g2_bitmap.pixel_buf_height;
  
  u8g2_bitmap.user_x0 = 0;
  u8g2_bitmap.user_x1 = u8g2_bitmap.pixel_buf_width;
  u8g2_bitmap.user_y0 = u8g2_bitmap.buf_y0;
  u8g2_bitmap.user_y1 = u8g2_bitmap.buf_y1;  
  
  /* draw something into the bitmap */
  /* send buffer is not required, in fact it MUST NOT BE CALLED */
  
  u8g2_DrawLine(&u8g2_bitmap, 0, 0, 15, 15 );
  u8g2_SetFont(&u8g2_bitmap, u8g2_font_6x10_tr);
  u8g2_DrawStr(&u8g2_bitmap, 0, 11, "ABC");

  /* setup target display */

  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
  
  x = 30;
  y = 31-7;

  
  for(;;)
  {
#ifdef U8G2_WITH_HVLINE_COUNT
    u8g2.hv_cnt = 0UL;
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
    /*
    u8g2_ClearBuffer(&u8g2);
    
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_DrawStr(&u8g2, x, y, "ABC");
      u8g2_SetFontDirection(&u8g2, 1);
      u8g2_DrawStr(&u8g2, x, y, "abc");
      u8g2_SetFontDirection(&u8g2, 2);
      u8g2_DrawStr(&u8g2, x, y, "abc");
      u8g2_SetFontDirection(&u8g2, 3);
      u8g2_DrawStr(&u8g2, x, y, "abc");
    
    u8g2_SendBuffer(&u8g2);
    */
    
    u8g2_FirstPage(&u8g2);
    i = 0;
    do
    {
      
      u8g2_DrawXBMP(&u8g2, x, y, BITMAP_WIDTH, BITMAP_HEIGHT, u8g2_bitmap.tile_buf_ptr);


      
      if ( i == 1 )
      {
	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y0, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y0, 1, 0);

      }
      
      i++;
      

    } while( u8g2_NextPage(&u8g2) );
#ifdef U8G2_WITH_HVLINE_COUNT
    printf("hv cnt: %ld\n", u8g2.hv_cnt);
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 273 ) y -= 1;
    if ( k == 274 ) y += 1;
    if ( k == 276 ) x -= 1;
    if ( k == 275 ) x += 1;
    
    if ( k == 'e' ) y -= 1;
    if ( k == 'x' ) y += 1;
    if ( k == 's' ) x -= 1;
    if ( k == 'd' ) x += 1;
    if ( k == 'q' ) break;
    
  }
  
  //u8x8_Set8x8Font(u8g2_GetU8x8(&u8g2), bdf_font);
  //u8x8_Draw8x8String(u8g2_GetU8x8(&u8g2), 0, 0, "Hello World!");
  
  
  
  
  
  
  return 0;
}

