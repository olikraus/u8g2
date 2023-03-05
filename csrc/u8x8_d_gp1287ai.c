/*

  u8x8_d_gp1287ai.c
  
  https://github.com/olikraus/u8g2/issues/1907
  https://github.com/olikraus/u8g2/pull/1892  

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2022, olikraus@gmail.com
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

#define SWAP8(a) ((((a)&0x80) >> 7) | (((a)&0x40) >> 5) | (((a)&0x20) >> 3) | (((a)&0x10) >> 1) | (((a)&0x08) << 1) | (((a)&0x04) << 3) | (((a)&0x02) << 5) | (((a)&0x01) << 7))

/* ========== GP1287AI ========== */
static const u8x8_display_info_t u8x8_gp1287ai_display_info = {
    /* chip_enable_level = */ 0,
    /* chip_disable_level = */ 1,

    /* post_chip_enable_wait_ns = */ 240, /* tCS-CLK */
    /* pre_chip_disable_wait_ns = */ 120, /* tCLK-CS */
    /* reset_pulse_width_ms = */ 1,       /* Trw     */
    /* post_reset_wait_ms = */ 1,         /* Trth    */
    /* sda_setup_time_ns = */ 60,         /* tsu     */
    /* sck_pulse_width_ns = */ 120,       /* tcyc/2  */
    /* sck_clock_hz = */ 4000000UL,       /* MAX 4.16 MHz */
    /* spi_mode = */ 3,                   /* active low, falling edge, MSBFIRST */
    /* i2c_bus_clock_100kHz = */ 4,       /*         */
    /* data_setup_time_ns = */ 60,        /* tsu     */
    /* write_pulse_width_ns = */ 120,     /* tcyc/2  */
    /* tile_width = */ 32,                /* 32*8=256 memory size */
    /* tile_height = */ 7,                 /* 7*8=56 memory size */
    /* default_x_offset = */ 0,           /*         */
    /* flipmode_x_offset = */ 0,          /*         */
    /* pixel_width = */ 256,              /* display size */
    /* pixel_height = */ 50               /* display size */
};
static const uint8_t u8x8_d_gp1287ai_init_seq[] = {
    U8X8_START_TRANSFER(),
    U8X8_C(SWAP8(0x0AA)), /* Software reset */
    U8X8_END_TRANSFER(),
    U8X8_DLY(1), /* Wait for reset */

    U8X8_START_TRANSFER(),
    U8X8_CA(SWAP8(0x078), SWAP8(0x008)), /* Oscillation Setting */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_CAA(SWAP8(0x0CC), SWAP8(0x002), SWAP8(0x000)), /* VFD Mode Setting */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_CAAA(SWAP8(0x0E0), SWAP8(0x0FF), SWAP8(0x031), SWAP8(0x000)), /* Display Area Setting */
    U8X8_A4(SWAP8(0x020), SWAP8(0x000), SWAP8(0x000), SWAP8(0x080)),   /* Display Area Setting */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_CAAAA(SWAP8(0x0B1), SWAP8(0x020), SWAP8(0x03F), SWAP8(0x000), SWAP8(0x001)), /* Internal Speed Setting */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_CAA(SWAP8(0x0A0), SWAP8(0x000), SWAP8(0x028)), /* Dimming level Setting (1024 level, 0x3FF max) */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_C(SWAP8(0x055)), /* Memory Map Clear */
    U8X8_END_TRANSFER(),
    U8X8_DLY(15), /* Waiting for memory clear */

    U8X8_START_TRANSFER(),
    U8X8_CAA(SWAP8(0x0C0), SWAP8(0x000), SWAP8(0x004)), /* DW1 position setting */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_CAA(SWAP8(0x0D0), SWAP8(0x000), SWAP8(0x03C)), /* DW2 position setting */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_CA(SWAP8(0x090), SWAP8(0x000)), /* Internal Command */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_CA(SWAP8(0x008), SWAP8(0x000)), /* T1 INT Setting */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_CA(SWAP8(0x080), SWAP8(0x000)), /* Display Mode Setting */
    U8X8_END_TRANSFER(),

    U8X8_START_TRANSFER(),
    U8X8_C(SWAP8(0x061)), /* Standby Mode */
    U8X8_END_TRANSFER(),

    U8X8_END() /* end of sequence */
};
static const uint8_t u8x8_d_gp1287ai_standby_seq[] = {
    U8X8_START_TRANSFER(),
    U8X8_C(SWAP8(0x061)), /* Standby */
    U8X8_END_TRANSFER(),

    U8X8_END() /* end of sequence */
};
static const uint8_t u8x8_d_gp1287ai_wakeup_seq[] = {
    U8X8_START_TRANSFER(),
    U8X8_C(SWAP8(0x06D)), /* Wake up */
    U8X8_END_TRANSFER(),
    U8X8_DLY(1), /* Wait for OSC stabilize */

    U8X8_START_TRANSFER(),
    U8X8_CA(SWAP8(0x080), SWAP8(0x000)), /* After entering standby mode, the SC bit will be automatically cleared */
    U8X8_END_TRANSFER(),

    U8X8_END() /* end of sequence */
};
uint8_t u8x8_d_gp1287ai_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t *ptr;
    uint8_t x, y;
    uint16_t tx_cnt;
    uint8_t swapped_byte;
    switch (msg)
    {
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
        if (arg_int == 0)
            u8x8_cad_SendSequence(u8x8, u8x8_d_gp1287ai_wakeup_seq);
        else
            u8x8_cad_SendSequence(u8x8, u8x8_d_gp1287ai_standby_seq);
        break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
        u8x8_cad_StartTransfer(u8x8);
        u8x8_cad_SendCmd(u8x8, SWAP8(0x0A0));
        u8x8_cad_SendArg(u8x8, SWAP8((arg_int * 4) >> 8));   /* Dimming level */
        u8x8_cad_SendArg(u8x8, SWAP8((arg_int * 4) & 0xFF)); /* Dimming level */
        u8x8_cad_EndTransfer(u8x8);
        break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
        x = ((u8x8_tile_t *)arg_ptr)->x_pos * 8;
        y = ((u8x8_tile_t *)arg_ptr)->y_pos * 8 + 4;

        u8x8_cad_StartTransfer(u8x8);

        u8x8_cad_SendCmd(u8x8, SWAP8(0x0F0));
        u8x8_cad_SendArg(u8x8, SWAP8(x));
        u8x8_cad_SendArg(u8x8, SWAP8(y));
        u8x8_cad_SendArg(u8x8, SWAP8(0x007)); /* return every 8 pixels */
        do
        {
            ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
            tx_cnt = ((u8x8_tile_t *)arg_ptr)->cnt * 8;
            while (tx_cnt > 0)
            {
                swapped_byte = SWAP8(*ptr);
                u8x8_cad_SendData(u8x8, 1, &swapped_byte);
                ptr += 1;
                tx_cnt -= 1;
            }
            arg_int--;
        } while (arg_int > 0);

        u8x8_cad_EndTransfer(u8x8);
        break;
    default:
        return 0;
    }
    return 1;
}
uint8_t u8x8_d_gp1287ai_256x50(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
        u8x8_d_helper_display_setup_memory(u8x8, &u8x8_gp1287ai_display_info);
        break;
    case U8X8_MSG_DISPLAY_INIT:
        u8x8_d_helper_display_init(u8x8);
        u8x8_cad_SendSequence(u8x8, u8x8_d_gp1287ai_init_seq);
        break;
    default:
        return u8x8_d_gp1287ai_common(u8x8, msg, arg_int, arg_ptr);
    }
    return 1;
}
