/*

  u8x8_d_e3keys.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2020, me@sephalon.net
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
  
*/


#include "u8x8.h"


static const u8x8_display_info_t u8x8_e3keys_sb6432_display_info =
{
  /* chip_enable_level = */ 0, /* not applicable */
  /* chip_disable_level = */ 1, /* not applicable */
  
  /* post_chip_enable_wait_ns = */ 0, /* not applicable */
  /* pre_chip_disable_wait_ns = */ 0, /* not applicable */
  /* reset_pulse_width_ms = */ 0, /* not applicable */
  /* post_reset_wait_ms = */ 0, /* not applicable */
  /* sda_setup_time_ns = */ 140,
  /* sck_pulse_width_ns = */ 250,
  /* sck_clock_hz = */ 2000000UL, /* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 2, /* active low, falling edge */
  /* i2c_bus_clock_100kHz = */ 0, /* not applicable */
  /* data_setup_time_ns = */ 0, /* not applicable */
  /* write_pulse_width_ns = */ 0, /* not applicable */
  /* tile_width = */ 8,
  /* tile_hight = */ 4,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 64,
  /* pixel_height = */ 32
};


#define E3KEYS_AT_CMD_SET_DISPLAY_ADDR_WRITE_DATA 0x40
#define E3KEYS_AT_CMD_SET_RGB_COLOR 0x42
#define E3KEYS_AT_CMD_END_TRANSMISSION 0x43

#define E3KEYS_AT_EXT_CMD_START 0x47
#define E3KEYS_AT_EXT_CMD_SET_MODE 0x01


/*
 * Set Mode extended command data layout:
 * D_7 D_6 D_5 D_4 D_3 D_2 D_1 D_0
 *
 * D_7: =1 set Group   /=0 only if group matches
 * D_6: =1 Display ON  /=0 Display OFF
 * D_5: =1 Backlite ON /=0 Backlite OFF
 * D_4: Reserve
 * D_3..0: Group Number
 */

static const uint8_t u8x8_d_e3keys_sb6432_powersave0_seq[] = {
  U8X8_START_TRANSFER(),
  U8X8_C(E3KEYS_AT_EXT_CMD_START),
  U8X8_C(E3KEYS_AT_EXT_CMD_SET_MODE),
  U8X8_D1((1 << 6) | (1 << 5)),
  U8X8_C(E3KEYS_AT_CMD_END_TRANSMISSION),
  U8X8_DLY(1), /* only 2us required */
  U8X8_END_TRANSFER(),
  U8X8_END()
};

static const uint8_t u8x8_d_e3keys_sb6432_powersave1_seq[] = {
  U8X8_START_TRANSFER(),
  U8X8_C(E3KEYS_AT_EXT_CMD_START),
  U8X8_C(E3KEYS_AT_EXT_CMD_SET_MODE),
  U8X8_D1(0x0),
  U8X8_C(E3KEYS_AT_CMD_END_TRANSMISSION),
  U8X8_DLY(1), /* only 2us required */
  U8X8_END_TRANSFER(),
  U8X8_END()
};


uint8_t u8x8_d_e3keys_sb6432(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
#ifdef U8X8_WITH_SET_BACKLIGHT_COLOR
  uint32_t color;
  uint8_t color_bytes[3];
#endif
  const u8x8_display_info_t *d_info = u8x8->display_info;
  u8x8_tile_t *tile;
  const uint16_t y_addr_offset = d_info->pixel_height / 8;
  const uint16_t upper_left_addr = d_info->pixel_width * (d_info->pixel_height / 4) - y_addr_offset;

  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_e3keys_sb6432_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if (arg_int)
        u8x8_cad_SendSequence(u8x8, u8x8_d_e3keys_sb6432_powersave1_seq);
      else
        u8x8_cad_SendSequence(u8x8, u8x8_d_e3keys_sb6432_powersave0_seq);
      break;
#ifdef U8X8_WITH_SET_BACKLIGHT_COLOR
    case U8X8_MSG_DISPLAY_SET_BACKLIGHT_COLOR:
      color = *((uint32_t *) arg_ptr);
      /*
       * Set RGB Color command data layout:
       * Red:   0 D_6 D_5 D_4 D_3 D_2 D_1 D_0
       * Green: 0 D_6 D_5 D_4 D_3 D_2 D_1 D_0
       * Blue:  0 D_6 D_5 D_4 D_3 D_2 D_1 D_0
       */
      color_bytes[0] = (color >> 16) & 0x7f; /* red */
      color_bytes[1] = (color >> 8) & 0x7f; /* green */
      color_bytes[2] = color & 0x7f; /* blue */

      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, E3KEYS_AT_CMD_SET_RGB_COLOR);
      u8x8_cad_SendData(u8x8, 3, color_bytes);
      u8x8_cad_SendCmd(u8x8, E3KEYS_AT_CMD_END_TRANSMISSION);
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      tile = (u8x8_tile_t *)arg_ptr;

      u8x8_cad_StartTransfer(u8x8);
      for (int i = 0; i < arg_int; i++) {
        for (int c = 0; c < tile->cnt; c++) {
          for (int j = 0; j < 8; j++) {
            /* Add x position */
            uint16_t addr = upper_left_addr - (tile->x_pos + c + i) * 8 * 8;
            /* Add y position */
            if (tile->y_pos < (d_info->tile_height / 2)) /* upper half */
              addr += tile->y_pos * 2;
            else /* lower half */
              addr += (tile->y_pos - (d_info->tile_height / 2)) * 2 - y_addr_offset;
            /* Add tile offset */
            addr -= j * 8;

            /*
             * Set Display Address & Write Display Data command data layout:
             * 0 0 0 0 0   0   0   A_8
             * 0 0 0 0 A_7 A_6 A_5 A_4
             * 0 0 0 0 A_3 A_2 A_1 A_0
             * 0 0 0 0 D_3 D_2 D_1 D_0
             */
            uint8_t addr_bytes[3], data_bytes[2];
            addr_bytes[0] = (addr >> 8) & 0x01;
            addr_bytes[1] = (addr >> 4) & 0x0f;
            addr_bytes[2] = addr & 0x0f;
            data_bytes[0] = tile->tile_ptr[c * 8 + j] & 0x0f;
            data_bytes[1] = (tile->tile_ptr[c * 8 + j] >> 4) & 0x0f;

            u8x8_cad_SendCmd(u8x8, E3KEYS_AT_CMD_SET_DISPLAY_ADDR_WRITE_DATA);
            u8x8_cad_SendData(u8x8, 3, addr_bytes);
            u8x8_cad_SendData(u8x8, 2, data_bytes);
          };
        };
      };
      u8x8_cad_SendCmd(u8x8, E3KEYS_AT_CMD_END_TRANSMISSION);
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}
