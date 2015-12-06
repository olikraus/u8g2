/*
  
  u8x8_display.c
  
  Abstraction layer for the graphics controller.
  Main goal is the placement of a 8x8 pixel block (tile) on the display.
  
*/


#include "u8x8.h"


/*==========================================*/
/* internal library function */

/*
  this is a helper function for the U8X8_MSG_DISPLAY_SETUP_MEMORY function.
  It can be called within the display callback function to carry out the usual standard tasks.
  
*/
void u8x8_d_helper_display_setup_memory(u8x8_t *u8x8, const u8x8_display_info_t *display_info)
{
      /* 1) set display info struct */
      u8x8->display_info = display_info;
      u8x8->x_offset = u8x8->display_info->default_x_offset;
}

/*
  this is a helper function for the U8X8_MSG_DISPLAY_INIT function.
  It can be called within the display callback function to carry out the usual standard tasks.
  
*/
void u8x8_d_helper_display_init(u8x8_t *u8x8)
{
      /* 2) apply port directions to the GPIO lines and apply default values for the IO lines*/
      u8x8_gpio_Init(u8x8);
      u8x8_cad_Init(u8x8);

      /* 3) do reset */
      u8x8_gpio_SetReset(u8x8, 1);
      u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_MILLI, u8x8->display_info->reset_pulse_width_ms);
      u8x8_gpio_SetReset(u8x8, 0);
      u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_MILLI, u8x8->display_info->reset_pulse_width_ms);
      u8x8_gpio_SetReset(u8x8, 1);
      u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_MILLI, u8x8->display_info->post_reset_wait_ms);
}    

/*==========================================*/
/* official functions */

uint8_t u8x8_display_DrawTile(u8x8_t *u8x8, uint8_t x, uint8_t y, uint8_t cnt, uint8_t *tile_ptr)
{
  u8x8_tile_t tile;
  tile.x_pos = x;
  tile.y_pos = y;
  tile.cnt = cnt;
  tile.tile_ptr = tile_ptr;
  return u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_DRAW_TILE, 1, (void *)&tile);
}

/* should be implemented as macro */
void u8x8_SetupMemory(u8x8_t *u8x8)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SETUP_MEMORY, 0, NULL);  
}

void u8x8_InitDisplay(u8x8_t *u8x8)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_INIT, 0, NULL);  
}

void u8x8_display_SetPowerSave(u8x8_t *u8x8, uint8_t is_enable)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SET_POWER_SAVE, is_enable, NULL);  
}

void u8x8_display_SetFlipMode(u8x8_t *u8x8, uint8_t mode)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SET_FLIP_MODE, mode, NULL);  
}

void u8x8_display_SetContrast(u8x8_t *u8x8, uint8_t value)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SET_CONTRAST, value, NULL);  
}


void u8x8_display_ClearScreen(u8x8_t *u8x8)
{
  uint8_t buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  u8x8_tile_t tile;
  uint8_t h;

  tile.x_pos = 0;
  tile.cnt = 1;
  tile.tile_ptr = buf;
  
  h = u8x8->display_info->tile_height;
  tile.y_pos = 0;
  do
  {
    u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_DRAW_TILE, u8x8->display_info->tile_width, (void *)&tile);
    tile.y_pos++;
  } while( tile.y_pos < h );
}

