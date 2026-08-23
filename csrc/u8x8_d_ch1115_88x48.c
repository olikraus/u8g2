/*

  u8x8_d_ch1115_88x48.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2026, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "u8x8.h"

/* CH1115 0.5" OLED, physical panel orientation is 48x88. */
static const uint8_t u8x8_d_ch1115_88x48_init_seq[] = {
  U8X8_START_TRANSFER(),
  U8X8_C(0x0ae),
  U8X8_CA(0x0d5, 0x080),
  U8X8_CA(0x0a8, 0x03f),
  U8X8_CA(0x020, 0x002),
  U8X8_CA(0x0d3, 0x000),
  U8X8_C(0x040),
  U8X8_CA(0x0ad, 0x08b),
  U8X8_C(0x0a1),
  U8X8_C(0x0c8),
  U8X8_CA(0x0da, 0x012),
  U8X8_CA(0x081, 0x080),
  U8X8_CA(0x0d9, 0x022),
  U8X8_CA(0x0db, 0x035),
  U8X8_C(0x0a4),
  U8X8_C(0x0a6),
  U8X8_C(0x0af),
  U8X8_END_TRANSFER(),
  U8X8_END()
};

static const uint8_t u8x8_d_ch1115_88x48_powersave0_seq[] = {
  U8X8_START_TRANSFER(),
  U8X8_C(0x0af),
  U8X8_END_TRANSFER(),
  U8X8_END()
};

static const uint8_t u8x8_d_ch1115_88x48_powersave1_seq[] = {
  U8X8_START_TRANSFER(),
  U8X8_C(0x0ae),
  U8X8_END_TRANSFER(),
  U8X8_END()
};

static const uint8_t u8x8_d_ch1115_88x48_flip0_seq[] = {
  U8X8_START_TRANSFER(),
  U8X8_C(0x0a1),
  U8X8_C(0x0c8),
  U8X8_END_TRANSFER(),
  U8X8_END()
};

static const uint8_t u8x8_d_ch1115_88x48_flip1_seq[] = {
  U8X8_START_TRANSFER(),
  U8X8_C(0x0a0),
  U8X8_C(0x0c0),
  U8X8_END_TRANSFER(),
  U8X8_END()
};

static const u8x8_display_info_t u8x8_ch1115_88x48_display_info = {
  0, 1,
  20, 10, 100, 100,
  50, 50, 8000000UL, 0, 4, 40, 150,
  11, 6, 40, 0, 88, 48
};

static const uint8_t u8x8_d_ch1115_88x48_clear_buf[128] = { 0 };
static const uint8_t u8x8_d_ch1115_88x48_hidden_pages[] = { 0, 7 };

static void u8x8_d_ch1115_88x48_clear_hidden_pages(u8x8_t *u8x8)
{
  uint8_t i;
  uint8_t page;

  /*
    The panel's 48 visible rows are controller pages 1 through 6. Clear
    the two RAM pages outside that window so that they cannot appear as
    noise with the required 64-row controller scan.
  */
  for (i = 0; i < sizeof(u8x8_d_ch1115_88x48_hidden_pages); i++)
  {
    page = u8x8_d_ch1115_88x48_hidden_pages[i];
    u8x8_cad_StartTransfer(u8x8);
    u8x8_cad_SendCmd(u8x8, 0x0b0 | page);
    u8x8_cad_SendCmd(u8x8, 0x010);
    u8x8_cad_SendCmd(u8x8, 0x000);
    u8x8_cad_SendData(u8x8, sizeof(u8x8_d_ch1115_88x48_clear_buf),
        (uint8_t *)u8x8_d_ch1115_88x48_clear_buf);
    u8x8_cad_EndTransfer(u8x8);
  }
}

uint8_t u8x8_d_ch1115_88x48(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x;
  uint8_t c;
  uint8_t *ptr;

  switch (msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ch1115_88x48_display_info);
      break;

    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ch1115_88x48_init_seq);
      u8x8_d_ch1115_88x48_clear_hidden_pages(u8x8);
      break;

    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      u8x8_cad_SendSequence(u8x8, arg_int == 0 ?
          u8x8_d_ch1115_88x48_powersave0_seq :
          u8x8_d_ch1115_88x48_powersave1_seq);
      break;

    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      u8x8_cad_SendSequence(u8x8, arg_int == 0 ?
          u8x8_d_ch1115_88x48_flip0_seq : u8x8_d_ch1115_88x48_flip1_seq);
      u8x8->x_offset = arg_int == 0 ? u8x8->display_info->default_x_offset :
          u8x8->display_info->flipmode_x_offset;
      break;

#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081);
      u8x8_cad_SendArg(u8x8, arg_int);
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif

    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
      x = ((u8x8_tile_t *)arg_ptr)->x_pos * 8 + u8x8->x_offset;
      u8x8_cad_SendCmd(u8x8, 0x010 | (x >> 4));
      u8x8_cad_SendCmd(u8x8, x & 15);
      u8x8_cad_SendCmd(u8x8, 0x0b1 + ((u8x8_tile_t *)arg_ptr)->y_pos);
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        u8x8_cad_SendData(u8x8, c * 8, ptr);
        arg_int--;
      } while (arg_int > 0);
      u8x8_cad_EndTransfer(u8x8);
      break;

    default:
      return 0;
  }
  return 1;
}
