
#include "u8g2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define FACTOR 3
//#define XOFFSET (FACTOR*32)
//#define YOFFSET (FACTOR*32)
#define DEFAULT_WIDTH (512+512+256)
#define DEFAULT_HEIGHT (1024+512+256)


uint16_t tga_max_x;
uint16_t tga_max_y;
size_t tga_max_offset = 0;

static uint16_t tga_width;
static uint16_t tga_height;
static uint8_t *tga_data = NULL;
uint8_t tga_r = 0;
uint8_t tga_g = 0;
uint8_t tga_b = 0;

uint8_t tga_desc_r = 0;
uint8_t tga_desc_g = 0;
uint8_t tga_desc_b = 0;

int tga_init(uint16_t w, uint16_t h)
{
  tga_max_x = 0;
  tga_max_y = 64;
  tga_width = 0;
  tga_height = 0;
  tga_max_offset = 0;
  if ( tga_data != NULL )
  {
    tga_data = (uint8_t *)realloc(tga_data, w*h*3);
  }
  else
  {
    tga_data = (uint8_t *)malloc(w*h*3);
  }
  if ( tga_data == NULL )
    return 0;
  memset(tga_data, 255, (long)w*(long)h*3L);
  tga_width = w;
  tga_height = h;
  return 1;
}

void tga_set_pixel(uint16_t x, uint16_t y)
{
  uint8_t *p;
  size_t offset;
	offset = (tga_height-y-1)*tga_width*3 + x*3;
	p = tga_data + offset;
	*p++ = tga_b;
	*p++ = tga_g;
	*p++ = tga_r;
}

void tga_clr_pixel(uint16_t x, uint16_t y)
{
  uint8_t *p;
  size_t offset;
	
	offset = (tga_height-y-1)*tga_width*3 + x*3;
	p = tga_data + offset;
	*p++ = 255;
	*p++ = 255;
	*p++ = 255;
}

void tga_set_8pixel(int x, int y, uint8_t pixel)
{
  int cnt = 8;
  uint8_t *p;
  size_t offset;
	
  offset = (tga_height-y-1)*tga_width*3 + x*3;
  p = tga_data + offset;
  offset = 3*tga_width + 3;
  while( cnt > 0 )
  {
    if ( (pixel & 1) != 0 )
    {
      // tga_set_pixel(x,y);
	*p++ = tga_b;
	*p++ = tga_g;
	*p++ = tga_r;
	    
	  if ( tga_max_y <= y+8 )
		  tga_max_y = y+8;
		
    }
    else
    {
      // tga_clr_pixel(x,y);
	*p++ = 255;
	*p++ = 255;
	*p++ = 255;
    }
    pixel >>= 1;
    //y+=1;
    p -= offset;
    cnt--;
  }
}

void tga_set_multiple_8pixel(int x, int y, int cnt, uint8_t *pixel)
{
  uint8_t b;

  while( cnt > 0 )
  {
    b = *pixel;
    tga_set_8pixel(x, y, b);
    x+=1;
    pixel++;
    cnt--;
  }
}



void tga_write_byte(FILE *fp, uint8_t byte)
{
  fputc(byte, fp);
}

void tga_write_word(FILE *fp, uint16_t word)
{
  tga_write_byte(fp, word&255);
  tga_write_byte(fp, word>>8);
}

void tga_save(const char *name)
{
  FILE *fp;
  uint16_t h;
  size_t offset;
  if ( tga_data == NULL )
    return;
  

  if ( tga_max_y >= tga_height )
	  tga_max_y = tga_height-1;
	
  offset = (tga_height-tga_max_y-1)*tga_width*3;
  h = tga_max_y+1;

  //printf("tga_save: File %s with %dx%d pixel, offset=%ld\n", name, tga_width, h, offset);
  
  //offset = 0;
  //h = tga_height;
  
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
    tga_write_word(fp, tga_width);		/* width */
    tga_write_word(fp, h);		/* height */
    tga_write_byte(fp, 24);		/* color depth */
    tga_write_byte(fp, 0);		
    fwrite(tga_data+offset, tga_width*h*3, 1, fp);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    fwrite("TRUEVISION-XFILE.", 18, 1, fp);
    fclose(fp);
  }
}

/*==========================================*/
/* tga procedures */


static const u8x8_display_info_t u8x8_tga_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 0,
  /* pre_chip_disable_wait_ns = */ 0,
  /* reset_pulse_width_ms = */ 0, 
  /* post_reset_wait_ms = */ 0, 
  /* sda_setup_time_ns = */ 0,		
  /* sck_pulse_width_ns = */ 0,
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 1,		
  /* i2c_bus_clock_100kHz = */ 0,
  /* data_setup_time_ns = */ 0,
  /* write_pulse_width_ns = */ 0,
  /* tile_width = */ (DEFAULT_WIDTH)/8,
  /* tile_hight = */ (DEFAULT_HEIGHT)/8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  DEFAULT_WIDTH,
  DEFAULT_HEIGHT
};


uint8_t u8x8_d_tga(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  u8g2_uint_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8g2, &u8x8_tga_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2);
      //if ( tga_data == NULL )
      tga_init(DEFAULT_WIDTH, DEFAULT_HEIGHT);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      
      tga_r = tga_desc_r;
      tga_g = tga_desc_g;
      tga_b = tga_desc_b;
    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      //printf("U8X8_MSG_DISPLAY_DRAW_TILE x=%d, ", x);
      x *= 8;
      x += u8g2->x_offset;	
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      //printf("y=%d, c=%d\n", y, ((u8x8_tile_t *)arg_ptr)->cnt);
      y *= 8;
    
      do
      {
	c = ((u8x8_tile_t *)arg_ptr)->cnt;
	ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
	tga_set_multiple_8pixel(x, y, c*8, ptr);

	      
	arg_int--;
      } while( arg_int > 0 );

      break;
    default:
      return 0;
  }
  return 1;
}


void u8x8_Setup_TGA(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_tga;

  /* setup display info */
  u8x8_SetupMemory(u8x8);  
}

void u8g2_SetupBuffer_TGA(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  static uint8_t buf[(DEFAULT_WIDTH)*8*32];
  
  u8x8_Setup_TGA(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 32, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}

