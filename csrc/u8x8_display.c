/*
  
  u8x8_display.c
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
  
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
      u8x8_gpio_Init(u8x8);             /* macro, which calls gpio_and_delay_cb with U8X8_MSG_GPIO_AND_DELAY_INIT */
      u8x8_cad_Init(u8x8);              /* this will also call U8X8_MSG_BYTE_INIT, byte init will NOT call GPIO_INIT */

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

uint8_t u8x8_DrawTile(u8x8_t *u8x8, uint8_t x, uint8_t y, uint8_t cnt, uint8_t *tile_ptr)
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

/*
  This will just init the display interface, compared to InitDisplay, it will not issue a reset and also not upload the init code.
  Comparison:
  Call                                                  u8x8_InitInterface              u8x8_InitDisplay
  Init Interface                                                yes                                     yes
  Reset Display                                                 no                                      yes
  Upload Display Init Code                              no                                      yes

  u8x8_InitInterface() is an alternative function to u8x8_InitDisplay(). Do not call both.
  
*/
void u8x8_InitInterface(u8x8_t *u8x8)
{
  u8x8_gpio_Init(u8x8);          /* macro, which calls gpio_and_delay_cb with U8X8_MSG_GPIO_AND_DELAY_INIT */
  u8x8_cad_Init(u8x8);              /* this will also call U8X8_MSG_BYTE_INIT, byte init will NOT call GPIO_INIT, which alread is called in the prev step */
}

/*
  This will sent the display init message to the display.
  The display itself will then call u8x8_d_helper_display_init() from above. This includes:
    GPIO Init (set port directions)
    BYTE init (part of CAD init: which may set some levels)
    CAD init (which will set things like I2C default address)
    Issue a reset to the display: This will usually turn off the display
  Additonally each display will set the init code to the display, which will also turn of the display in most cases (Arduino code disable power save mode later)

  Actually this procedure should be better called InitInterfaceAndDisplay, because it actually does both.
  (actually u8x8_InitInterface() is not called directly but only u8x8_gpio_Init and u8x8_cad_Init which
  in turn is called by u8x8_InitInterface())


  InitDisplay is called by the Arduino begin() function 

  In some cases it is not required to init the display (for example if the display is already running, but the controller comes out of deep sleep mode).
  Then InitDisplay can be skipped, but u8x8_InitInterface()  (== u8x8_gpio_Init() and u8x8_cad_Init()) need to be executed.

*/
void u8x8_InitDisplay(u8x8_t *u8x8)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_INIT, 0, NULL);       /* this will call u8x8_d_helper_display_init() and send the init seqence to the display */
  /* u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SET_FLIP_MODE, 0, NULL);  */ /* It would make sense to call flip mode 0 here after U8X8_MSG_DISPLAY_INIT */
}

void u8x8_SetPowerSave(u8x8_t *u8x8, uint8_t is_enable)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SET_POWER_SAVE, is_enable, NULL);  
}

void u8x8_SetFlipMode(u8x8_t *u8x8, uint8_t mode)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SET_FLIP_MODE, mode, NULL);  
}

void u8x8_SetContrast(u8x8_t *u8x8, uint8_t value)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SET_CONTRAST, value, NULL);  
}

void u8x8_RefreshDisplay(u8x8_t *u8x8)
{
  u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_REFRESH, 0, NULL);  
}

void u8x8_ClearDisplayWithTile(u8x8_t *u8x8, const uint8_t *buf)
{
  u8x8_tile_t tile;
  uint8_t h;

  tile.x_pos = 0;
  tile.cnt = 1;
  tile.tile_ptr = (uint8_t *)buf;		/* tile_ptr should be const, but isn't */
  
  h = u8x8->display_info->tile_height;
  tile.y_pos = 0;
  do
  {
    u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_DRAW_TILE, u8x8->display_info->tile_width, (void *)&tile);
    tile.y_pos++;
  } while( tile.y_pos < h );
}

void u8x8_ClearDisplay(u8x8_t *u8x8)
{
  uint8_t buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  u8x8_ClearDisplayWithTile(u8x8, buf);
}

void u8x8_FillDisplay(u8x8_t *u8x8)
{
  uint8_t buf[8] = { 255, 255, 255, 255, 255, 255, 255, 255 };
  u8x8_ClearDisplayWithTile(u8x8, buf);
}

void u8x8_ClearLine(u8x8_t *u8x8, uint8_t line)
{
  uint8_t buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  u8x8_tile_t tile;
  if ( line < u8x8->display_info->tile_height )
  {
    tile.x_pos = 0;
    tile.y_pos = line;
    tile.cnt = 1;
    tile.tile_ptr = (uint8_t *)buf;		/* tile_ptr should be const, but isn't */
    u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_DRAW_TILE, u8x8->display_info->tile_width, (void *)&tile);
  }  
}