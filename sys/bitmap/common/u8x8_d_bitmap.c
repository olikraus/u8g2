/*

  u8x8_d_bitmap.c
  
  a bitmap device

*/

#include "stdlib.h"	/* malloc */
#include "stdint.h"	/* uint16_t */
#include "string.h"	/* memcpy */
#include "stdio.h"	/* FILE */
#include "u8g2.h"		/* because of u8g2_Setup... */

/*========================================================*/
/* bitmap struct */

struct _u8x8_bitmap_struct
{
  u8x8_msg_cb u8x8_bitmap_display_old_cb;
  uint16_t tile_width;
  uint16_t tile_height;
  uint16_t pixel_width;
  uint16_t pixel_height;
  uint8_t *u8x8_buf;
  uint8_t *u8g2_buf;
};

typedef struct _u8x8_bitmap_struct u8x8_bitmap_t;

/*========================================================*/
/* bitmap functions */

uint8_t u8x8_bitmap_SetSize(u8x8_bitmap_t *b, uint16_t pixel_width, uint16_t pixel_height)
{
  if ( b->u8x8_buf != NULL )
    free(b->u8x8_buf);

  b->tile_width = (pixel_width+7)/8;
  b->tile_height = (pixel_height+7)/8;
  b->pixel_width = pixel_width;
  b->pixel_height = pixel_height;

  /* allocate the bitmap twice, one for u8x8 and another bitmap for u8g2 */
  /* however, the final bitmap will always be in the first half of the buffer */
  b->u8x8_buf = (uint8_t *)malloc((size_t)b->tile_width*(size_t)b->tile_height*(size_t)8*(size_t)2);
  b->u8g2_buf = b->u8x8_buf+(size_t)b->tile_width*(size_t)b->tile_height*(size_t)8;

  if ( b->u8x8_buf == NULL ) {
    b->u8g2_buf = NULL;
    return 0;
  }
  //printf("pixel size: %d %d\n", b->pixel_width, b->pixel_height);
  //printf("tile size: %d %d\n", b->tile_width, b->tile_height);
  return 1;
}

void u8x8_bitmap_DrawTiles(u8x8_bitmap_t *b, uint16_t tx, uint16_t ty, uint8_t tile_cnt, uint8_t *tile_ptr)
{
  uint8_t *dest_ptr = b->u8x8_buf;
  //printf("tile pos: %d %d, cnt=%d\n", tx, ty, tile_cnt);
  if ( dest_ptr == NULL )
    return;
  dest_ptr += ty*b->pixel_width;
  dest_ptr += tx*8;
  memcpy(dest_ptr, tile_ptr, tile_cnt*8);
}

uint8_t u8x8_bitmap_GetPixel(u8x8_bitmap_t *b, uint16_t x, uint16_t y)
{
  uint8_t *dest_ptr = b->u8x8_buf;
  if ( dest_ptr == NULL )
    return 0;
  dest_ptr += (y/8)*b->pixel_width;
  y &= 7;
  dest_ptr += x;
  if ( (*dest_ptr & (1<<y)) == 0 )
    return 0;
  return 1;
}

static void tga_write_byte(FILE *fp, uint8_t byte)
{
  fputc(byte, fp);
}

static void tga_write_word(FILE *fp, uint16_t word)
{
  tga_write_byte(fp, word&255);
  tga_write_byte(fp, word>>8);
}

void u8x8_bitmap_SaveTGA(u8x8_bitmap_t *b, const char *name)
{
  FILE *fp;
  uint16_t x, y;
  
  fp = fopen(name, "wb");
  if ( fp != NULL )
  {
    tga_write_byte(fp, 0);		/* no ID */
    tga_write_byte(fp, 0);		/* no color map */
    tga_write_byte(fp, 2);		/* uncompressed true color */
    tga_write_word(fp, 0);		
    tga_write_word(fp, 0);		
    tga_write_byte(fp, 0);		
    tga_write_word(fp, 0);		/* x origin */
    tga_write_word(fp, 0);		/* y origin */
    tga_write_word(fp, b->pixel_width);		/* width */
    tga_write_word(fp, b->pixel_height);		/* height */
    tga_write_byte(fp, 24);		/* color depth */
    tga_write_byte(fp, 0);	
    for( y = 0; y < b->pixel_height; y++ )
    {
      for( x = 0; x < b->pixel_width; x++ )
      {
	if ( u8x8_bitmap_GetPixel(b, x, b->pixel_height-y-1) == 0 )
	{
	  tga_write_byte(fp, 255);		/* R */
	  tga_write_byte(fp, 255);		/* G */
	  tga_write_byte(fp, 255);		/* B */
	}
	else
	{
	  tga_write_byte(fp, 0);		/* R */
	  tga_write_byte(fp, 0);		/* G */
	  tga_write_byte(fp, 0);		/* B */
	}
      }
    }
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    fwrite("TRUEVISION-XFILE.", 18, 1, fp);
    fclose(fp);
  }
}


/*========================================================*/
/* global objects for the bitmap */

u8x8_bitmap_t u8x8_bitmap;


static u8x8_display_info_t u8x8_bitmap_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 0,
  /* pre_chip_disable_wait_ns = */ 0,
  /* reset_pulse_width_ms = */ 0, 
  /* post_reset_wait_ms = */ 0, 
  /* sda_setup_time_ns = */ 0,		
  /* sck_pulse_width_ns = */ 0,
  /* sck_clock_hz = */ 4000000UL,	
  /* spi_mode = */ 1,		
  /* i2c_bus_clock_100kHz = */ 0,
  /* data_setup_time_ns = */ 0,
  /* write_pulse_width_ns = */ 0,
  /* tile_width = */ 8,		/* dummy value */
  /* tile_hight = */ 4,		/* dummy value */
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 64,		/* dummy value */
  /* pixel_height = */ 32		/* dummy value */
};


/*========================================================*/
/* functions for handling of the global objects */

/* allocate bitmap */
/* will be called by u8x8_SetupBitmap or u8g2_SetupBitmap */
static uint8_t u8x8_SetBitmapDeviceSize(u8x8_t *u8x8, uint16_t pixel_width, uint16_t pixel_height)
{
  /* update the global bitmap object, allocate the bitmap */
  if ( u8x8_bitmap_SetSize(&u8x8_bitmap, pixel_width, pixel_height) == 0 )
    return 0;
  
  /* update the u8x8 object */
  u8x8_bitmap_info.tile_width = (pixel_width+7)/8;
  u8x8_bitmap_info.tile_height = (pixel_height+7)/8;
  u8x8_bitmap_info.pixel_width = pixel_width;
  u8x8_bitmap_info.pixel_height = pixel_height;
  return 1;
}

/* draw tiles to the bitmap, called by the device procedure */
static void u8x8_DrawBitmapTiles(u8x8_t *u8x8, uint16_t tx, uint16_t ty, uint8_t tile_cnt, uint8_t *tile_ptr)
{
  u8x8_bitmap_DrawTiles(&u8x8_bitmap, tx, ty, tile_cnt, tile_ptr);
}

uint8_t u8x8_GetBitmapPixel(u8x8_t *u8x8, uint16_t x, uint16_t y)
{
  return u8x8_bitmap_GetPixel(&u8x8_bitmap, x, y);
}

void u8x8_SaveBitmapTGA(u8x8_t *u8x8, const char *filename)
{
  u8x8_bitmap_SaveTGA(&u8x8_bitmap, filename);
}

/*========================================================*/

static uint8_t u8x8_d_bitmap(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  u8g2_uint_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_bitmap_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);	/* update low level interfaces (not required here) */
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      c = ((u8x8_tile_t *)arg_ptr)->cnt;
      ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
      do
      {
	u8x8_DrawBitmapTiles(u8x8, x, y, c, ptr);
	x += c;
	arg_int--;
      } while( arg_int > 0 );

      break;
    default:
      return 0;
  }
  return 1;
}


/*========================================================*/
/* u8x8 and u8g2 setup functions */

void u8x8_SetupBitmap(u8x8_t *u8x8, uint16_t pixel_width, uint16_t pixel_height)
{  
  u8x8_SetBitmapDeviceSize(u8x8, pixel_width, pixel_height);
  
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_bitmap;

  /* setup display info */
  u8x8_SetupMemory(u8x8);  
}

void u8g2_SetupBitmap(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb, uint16_t pixel_width, uint16_t pixel_height)
{
  /* allocate bitmap, assign the device callback to u8x8 */
  u8x8_SetupBitmap(u8g2_GetU8x8(u8g2), pixel_width, pixel_height);
  
  /* configure u8g2 in full buffer mode */
  u8g2_SetupBuffer(u8g2, u8x8_bitmap.u8g2_buf, (pixel_height+7)/8, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}


/*========================================================*/


static uint8_t u8x8_d_bitmap_chain(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  if ( msg == U8X8_MSG_DISPLAY_DRAW_TILE )
    u8x8_d_bitmap(u8x8, msg, arg_int, arg_ptr);
  return u8x8_bitmap.u8x8_bitmap_display_old_cb(u8x8, msg, arg_int, arg_ptr);
}

/* connect the bitmap to an existing u8g2 or u8x8 object */
uint8_t u8x8_ConnectBitmapToU8x8(u8x8_t *u8x8)
{
  if ( u8x8_SetBitmapDeviceSize(u8x8, u8x8_GetCols(u8x8)*8, u8x8_GetRows(u8x8)*8) == 0 )
    return 0;
  u8x8_bitmap.u8x8_bitmap_display_old_cb = u8x8->display_cb;
  u8x8->display_cb = u8x8_d_bitmap_chain;
  return 1;
}

