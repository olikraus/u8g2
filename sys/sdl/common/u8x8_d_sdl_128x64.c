/*
  u8x8_d_sdl_128x64.c
*/

#include "u8g2.h"
#ifndef NO_SDL
#include "SDL.h"
#endif
#include <assert.h>

//#define HEIGHT (64)
//#define WIDTH 128

#define W(x,w) (((x)*(w))/100)

#ifndef NO_SDL
SDL_Window *u8g_sdl_window;
SDL_Surface *u8g_sdl_screen;
#endif

int u8g_sdl_multiple = 3;
uint32_t u8g_sdl_color[256];
int u8g_sdl_height, u8g_sdl_width;



static void u8g_sdl_set_pixel(int x, int y, int idx)
{
  uint32_t  *ptr;
  uint32_t offset;
  int i, j;
  
  if ( y >= u8g_sdl_height )
    return;
  if ( y < 0 )
    return;
  if ( x >= u8g_sdl_width )
    return;
  if ( x < 0 )
    return;
  
  for( i = 0; i < u8g_sdl_multiple; i++ )
    for( j = 0; j < u8g_sdl_multiple; j++ )
    {
#ifndef NO_SDL
      offset = (
        ((y * u8g_sdl_multiple) + i) * (u8g_sdl_width * u8g_sdl_multiple) + 
        ((x * u8g_sdl_multiple) + j)) * u8g_sdl_screen->format->BytesPerPixel;
        
      assert( offset < (Uint32)(u8g_sdl_width * u8g_sdl_multiple * u8g_sdl_height * u8g_sdl_multiple * u8g_sdl_screen->format->BytesPerPixel) );
      
      ptr = (uint32_t *)(((uint8_t *)(u8g_sdl_screen->pixels)) + offset);
      *ptr = u8g_sdl_color[idx];
#endif
    }
}

static void u8g_sdl_set_8pixel(int x, int y, uint8_t pixel)
{
  int cnt = 8;
  int bg = 0;
  if ( (x/8 + y/8)  & 1 )
    bg = 4;
  while( cnt > 0 )
  {
    if ( (pixel & 1) == 0 )
    {
      u8g_sdl_set_pixel(x,y,bg);
    }
    else
    {
      u8g_sdl_set_pixel(x,y,3);
    }
    pixel >>= 1;
    y++;
    cnt--;
  }
}

static void u8g_sdl_set_multiple_8pixel(int x, int y, int cnt, uint8_t *pixel)
{
  uint8_t b;
  while( cnt > 0 )
  {
    b = *pixel;
    u8g_sdl_set_8pixel(x, y, b);
    x++;
    pixel++;
    cnt--;
  }
}

static void u8g_sdl_init(int width, int height)
{
  u8g_sdl_height = height;
  u8g_sdl_width = width;
  
#ifndef NO_SDL
  
  if (SDL_Init(SDL_INIT_VIDEO) != 0) 
  {
    printf("Unable to initialize SDL:  %s\n", SDL_GetError());
    exit(1);
  }
  
  u8g_sdl_window = SDL_CreateWindow("U8g2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, u8g_sdl_width * u8g_sdl_multiple, u8g_sdl_height * u8g_sdl_multiple, 0);

  if ( u8g_sdl_window == NULL )
  {
    printf("Couldn't create window: %s\n", SDL_GetError());
    exit(1);
  }

  u8g_sdl_screen = SDL_GetWindowSurface(u8g_sdl_window);

  if ( u8g_sdl_screen == NULL )
  {
    printf("Couldn't create screen: %s\n", SDL_GetError());
    exit(1);
  }
  
  printf("%d bits-per-pixel mode\n", u8g_sdl_screen->format->BitsPerPixel);
  printf("%d bytes-per-pixel mode\n", u8g_sdl_screen->format->BytesPerPixel);
  
  u8g_sdl_color[0] = SDL_MapRGB( u8g_sdl_screen->format, 0, 0, 0 );
  u8g_sdl_color[1] = SDL_MapRGB( u8g_sdl_screen->format, W(100, 50), W(255,50), 0 );
  u8g_sdl_color[2] = SDL_MapRGB( u8g_sdl_screen->format, W(100, 80), W(255,80), 0 );
  u8g_sdl_color[3] = SDL_MapRGB( u8g_sdl_screen->format, 100, 255, 0 );
  u8g_sdl_color[4] = SDL_MapRGB( u8g_sdl_screen->format, 30, 30, 30 );

  /*
  u8g_sdl_set_pixel(0,0);
  u8g_sdl_set_pixel(1,1);
  u8g_sdl_set_pixel(2,2);
  */

  /* update all */
  SDL_UpdateWindowSurface(u8g_sdl_window);

  atexit(SDL_Quit);
#endif  
  return;
}



/*
void main(void)
{
  u8g_sdl_init();
  u8g_sdl_set_pixel(0,0,3);
  u8g_sdl_set_pixel(0,1,3);
  u8g_sdl_set_pixel(0,2,3);
  u8g_sdl_set_pixel(1,1,3);
  u8g_sdl_set_pixel(2,2,3);
  while( u8g_sdl_get_key() < 0 )
    ;
}
*/


static uint8_t u8x8_d_sdl_gpio(u8x8_t *u8x8, uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	static int debounce_cnt = 0;
	static int curr_msg = 0;
	static int db_cnt = 10;
	int event;
  
	if ( curr_msg > 0 )
	{
	  if ( msg == curr_msg )
	  {
		  u8x8_SetGPIOResult(u8x8, 0);
		  if ( debounce_cnt == 0 )
		    curr_msg = 0;
		  else
		    debounce_cnt--;
		  return 1;
	  }
	  
	}
	else
	{
	  event = u8g_sdl_get_key();
	  
	  switch(event)
	  {
		  case 273:
			  curr_msg = U8X8_MSG_GPIO_MENU_UP;
			  debounce_cnt = db_cnt;
			  break;
		  case 274:
			  curr_msg = U8X8_MSG_GPIO_MENU_DOWN;
			  debounce_cnt = db_cnt;
			  break;
		  case 275:
			  curr_msg = U8X8_MSG_GPIO_MENU_NEXT;
			  debounce_cnt = db_cnt;
			  break;
		  case 276:
			  curr_msg = U8X8_MSG_GPIO_MENU_PREV;
			  debounce_cnt = db_cnt;
			  break;
		  case 's':
			  curr_msg = U8X8_MSG_GPIO_MENU_SELECT;
			  debounce_cnt = db_cnt;
			  break;
		  case 'q':
			  curr_msg = U8X8_MSG_GPIO_MENU_HOME;
			  debounce_cnt = db_cnt;
			  break;
	  }
	}
	u8x8_SetGPIOResult(u8x8, 1);
	return 1;
}


/*========================================*/
/* 128x64 */

static const u8x8_display_info_t u8x8_sdl_128x64_info =
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
  /* tile_width = */ 16,
  /* tile_hight = */ 8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 128,
  /* pixel_height = */ 64
};


uint8_t u8x8_d_sdl_128x64(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8g2, &u8x8_sdl_128x64_info);
      u8g_sdl_init(128, 64);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8g2->x_offset;
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y *= 8;
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        u8g_sdl_set_multiple_8pixel(x, y, c*8, ptr);
        arg_int--;
	x+=c*8;
      } while( arg_int > 0 );
      
      /* update all */
#ifndef NO_SDL
      SDL_UpdateWindowSurface(u8g_sdl_window);
#endif      
      break;
    default:
      return 0;
  }
  return 1;
}

void u8x8_Setup_SDL_128x64(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_sdl_128x64;
	
  u8x8->gpio_and_delay_cb = u8x8_d_sdl_gpio;

  /* setup display info */
  u8x8_SetupMemory(u8x8);  
}

void u8g2_SetupBuffer_SDL_128x64(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  
  static uint8_t buf[128*8];
  
  u8x8_Setup_SDL_128x64(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 8, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}


void u8g2_SetupBuffer_SDL_128x64_4(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  
  static uint8_t buf[128*3];
  
  u8x8_Setup_SDL_128x64(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 3, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}

void u8g2_SetupBuffer_SDL_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  
  static uint8_t buf[128];
  
  u8x8_Setup_SDL_128x64(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 1, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}

/*========================================*/
/* 240x160 */

static const u8x8_display_info_t u8x8_sdl_240x160_info =
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
  /* tile_width = */ 30,		/* width of 30*8=240 pixel */
  /* tile_hight = */ 20,		/* height: 160 pixel */
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 240,
  /* pixel_height = */ 160
};


uint8_t u8x8_d_sdl_240x160(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8g2, &u8x8_sdl_240x160_info);
      u8g_sdl_init(240, 160);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8g2->x_offset;
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y *= 8;
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        u8g_sdl_set_multiple_8pixel(x, y, c*8, ptr);
        arg_int--;
      } while( arg_int > 0 );

#ifndef NO_SDL
      /* update all */
      SDL_UpdateWindowSurface(u8g_sdl_window);
#endif
      
      break;
    default:
      return 0;
  }
  return 1;
}


void u8x8_Setup_SDL_240x160(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_sdl_240x160;
	
  u8x8->gpio_and_delay_cb = u8x8_d_sdl_gpio;

  /* setup display info */
  u8x8_SetupMemory(u8x8);  
}

void u8g2_SetupBuffer_SDL_240x160(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  
  static uint8_t buf[240*20];
  
  u8x8_Setup_SDL_240x160(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 20, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}

/*========================================*/
/* 256x128 */

static const u8x8_display_info_t u8x8_sdl_256x128_info =
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
  /* tile_width = */ 32,		/* width of 32*8=256 pixel */
  /* tile_hight = */ 16,		/* height: 128 pixel */
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 256,
  /* pixel_height = */ 128
};


uint8_t u8x8_d_sdl_256x128(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8g2, &u8x8_sdl_256x128_info);
      u8g_sdl_init(256, 128);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8g2->x_offset;
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y *= 8;
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        u8g_sdl_set_multiple_8pixel(x, y, c*8, ptr);
        arg_int--;
      } while( arg_int > 0 );

#ifndef NO_SDL
      /* update all */
      SDL_UpdateWindowSurface(u8g_sdl_window);
#endif
      
      break;
    default:
      return 0;
  }
  return 1;
}


void u8x8_Setup_SDL_256x128(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_sdl_256x128;
	
  u8x8->gpio_and_delay_cb = u8x8_d_sdl_gpio;

  /* setup display info */
  u8x8_SetupMemory(u8x8);  
}

void u8g2_SetupBuffer_SDL_256x128(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  
  static uint8_t buf[256*16];
  
  u8x8_Setup_SDL_256x128(u8g2_GetU8x8(u8g2));
  u8g2_SetupBuffer(u8g2, buf, 16, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}
