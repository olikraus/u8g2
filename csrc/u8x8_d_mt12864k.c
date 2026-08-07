/*

  u8x8_d_mt12864k.c

  MELT (МЭЛТ) MT-12864K: 128x64 monochrome graphic LCD module, I2C.

  The module contains two RW1065 LCD controller/driver chips (KS0108/KS108
  compatible command set) and one INF8574A (PCF8574A compatible) control
  register. All three devices are I2C slaves, write only.

  I2C addresses (7-bit, default):
    left controller  (chip 1): 0x3C  (8-bit write address: 0x078)
    right controller (chip 2): 0x3D  (8-bit write address: 0x07A)
    control register         : 0x3B  (8-bit write address: 0x076)

  Control register bits (INF8574A, after POR all bits are "1"):
    bit 0: ^RST      hardware reset of both controllers, active low
    bit 1: BL_ON1    backlight on (full brightness), active high
    bit 2: BL_ON2    backlight on (half brightness), active high
    bit 3: P_ON      built-in power converter on, active high
    bits 4-6: CT_*   contrast, active low
    bit 7: not used

  The controllers use the two byte control protocol with a significant C0 bit
  (bit 7, see datasheet table 5):
    C0=0: this is the last control byte, all following bytes are payload
          (commands if D/~C=0, data if D/~C=1) until STOP or RE-START.
    C0=1: "control byte + payload byte" pairs, another control byte follows.
  Control byte values: 0x80 = cmd (C0=1), 0x00 = cmd (C0=0), 0x40 = data (C0=0),
  0xC0 = data (C0=1).  u8x8_cad_ssd13xx_i2c is compatible: each command is its
  own I2C transaction "0x00 cmd" and each data block its own transaction
  "0x40 data...", i.e. every transaction carries a single C0=0 control byte
  followed by a homogeneous payload stream, terminated by STOP.

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2026, Nick Gagin
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

/* control register INF8574A at I2C address 0x3B */
#define U8X8_MT12864K_REG_ADR	0x076	/* 8-bit address, 0x3B << 1 */

/* default control register value after POR: all bits "1" */
/* bit 0 (^RST) = 1 -> controllers not in reset */
#define U8X8_MT12864K_REG_DEFAULT	0x0ff
/* same value with ^RST = 0 (reset asserted) */
#define U8X8_MT12864K_REG_RESET		0x0fe

/* 8-bit I2C addresses of the two RW1065 controllers */
#define U8X8_MT12864K_CHIP1_ADR	0x078	/* 0x3C << 1, left half */
#define U8X8_MT12864K_CHIP2_ADR	0x07a	/* 0x3D << 1, right half */

static const uint8_t u8x8_d_mt12864k_init_seq[] = {
  U8X8_C(0x0d),		                /* Mode Set: EXT=1, extended command mode */
  U8X8_C(0x02),		                /* Set Power Save Mode: PS=0, normal power mode */
  U8X8_C(0x0c),		                /* Mode Set: EXT=0, back to KS0108-compatible mode */
  U8X8_C(0x0c0),		                /* start at the top */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_mt12864k_powersave0_seq[] = {
  U8X8_C(0x03f),		                /* display on */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_mt12864k_powersave1_seq[] = {
  U8X8_C(0x03e),		                /* display off */
  U8X8_END()             			/* end of sequence */
};


/* select the i2c address for the next controller transfer */
static void u8x8_mt12864k_select(u8x8_t *u8x8, uint8_t adr)
{
  u8x8->i2c_address = adr;
}

/* write a single byte to the INF8574A control register (0x3B, 8-bit 0x076).
   The register is an 8 bit latch: every write sets all bits, so the value
   must always contain the state of all other control bits (^RST, backlight,
   P_ON) as well. */
static void u8x8_mt12864k_write_register(u8x8_t *u8x8, uint8_t value)
{
  u8x8_mt12864k_select(u8x8, U8X8_MT12864K_REG_ADR);
  u8x8_byte_StartTransfer(u8x8);
  u8x8_byte_SendByte(u8x8, value);
  u8x8_byte_EndTransfer(u8x8);
}

/* reset both LCD controllers through the INF8574A control register */
static void u8x8_mt12864k_reset(u8x8_t *u8x8)
{
  /* assert ^RST (bit 0 = 0), keep the other bits at their default value */
  u8x8_mt12864k_write_register(u8x8, U8X8_MT12864K_REG_RESET);

  u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_MILLI, 1);

  /* release ^RST (bit 0 = 1) */
  u8x8_mt12864k_write_register(u8x8, U8X8_MT12864K_REG_DEFAULT);

  /* datasheet: 10 ms delay after ^RST goes inactive */
  u8x8_gpio_Delay(u8x8, U8X8_MSG_DELAY_MILLI, 10);
}

/* send the sequence to both controllers */
static void u8x8_mt12864k_send_sequence(u8x8_t *u8x8, const uint8_t *seq)
{
  u8x8_mt12864k_select(u8x8, U8X8_MT12864K_CHIP1_ADR);
  u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, 1, NULL);
  u8x8_cad_SendSequence(u8x8, seq);
  u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, 0, NULL);

  u8x8_mt12864k_select(u8x8, U8X8_MT12864K_CHIP2_ADR);
  u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, 2, NULL);
  u8x8_cad_SendSequence(u8x8, seq);
  u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, 0, NULL);
}


struct u8x8_mt12864k_vars
{
  uint8_t *ptr;
  uint8_t x;
  uint8_t c;
  uint8_t arg_int;
};

static void u8x8_mt12864k_out(u8x8_t *u8x8, struct u8x8_mt12864k_vars *v, void *arg_ptr)
{
  uint8_t cnt;
  u8x8_cad_SendCmd(u8x8, 0x040 | ((v->x << 3) & 63) );
  u8x8_cad_SendCmd(u8x8, 0x0b8 | (((u8x8_tile_t *)arg_ptr)->y_pos));
  
  while( v->arg_int > 0 )
  {
      /* calculate tiles to next boundary (end or chip limit) */
      cnt = v->x;
      cnt += 8;
      cnt &= 0x0f8;
      cnt -= v->x;
            
      if ( cnt > v->c )
	cnt = v->c;
    
      v->x +=cnt;
      v->c-=cnt;
      cnt<<=3;
      u8x8_cad_SendData(u8x8, cnt, v->ptr);	/* note: SendData can not handle more than 255 bytes */    
      v->ptr += cnt;
    
      if ( v->c == 0 )
      {
	v->ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
	v->c = ((u8x8_tile_t *)arg_ptr)->cnt;
	v->arg_int--;
      }
      if ( ((v->x) & 7) == 0 )
	break;       
  } 
}


static const u8x8_display_info_t u8x8_mt12864k_128x64_display_info =
{
  /* chip_enable_level = */ 0,		/* not used */
  /* chip_disable_level = */ 1,		/* not used */
  
  /* post_chip_enable_wait_ns = */ 100,
  /* pre_chip_disable_wait_ns = */ 20,
  /* reset_pulse_width_ms = */ 5, 	/* not used, reset is done via I2C control register */
  /* post_reset_wait_ms = */ 6, 		/* not used, reset is done via I2C control register */
  /* sda_setup_time_ns = */ 10,		
  /* sck_pulse_width_ns = */ 140,	/* 400 kHz I2C */
  /* sck_clock_hz = */ 4000000UL,	/* not used */
  /* spi_mode = */ 0,				/* not used */
  /* i2c_bus_clock_100kHz = */ 4,	/* 400 kHz */
  /* data_setup_time_ns = */ 10,
  /* write_pulse_width_ns = */ 10,	/* not used */
  /* tile_width = */ 16,		/* width of 16*8=128 pixel */
  /* tile_height = */ 8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 128,
  /* pixel_height = */ 64
};

uint8_t u8x8_d_mt12864k_128x64(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  struct u8x8_mt12864k_vars v;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_mt12864k_128x64_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_gpio_Init(u8x8);
      u8x8_cad_Init(u8x8);
      
      /* reset both controllers through the control register */
      u8x8_mt12864k_reset(u8x8);

      u8x8_mt12864k_send_sequence(u8x8, u8x8_d_mt12864k_init_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_mt12864k_send_sequence(u8x8, u8x8_d_mt12864k_powersave0_seq);
      else
	u8x8_mt12864k_send_sequence(u8x8, u8x8_d_mt12864k_powersave1_seq);
      break;
/* The MT-12864K controllers can not mirror the cols and rows, use U8g2 for rotation */
//    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
//      break;
/* The MT-12864K has no internal contrast command. Contrast is controlled by
   the INF8574A register bits 4-6. The datasheet (table 4) defines only four
   discrete levels, the CT_* bits are mutually exclusive: when one of them is
   "0" the other two must be "1". Bits 0-3 (^RST, backlight, P_ON) and bit 7
   stay at their default value "1". */
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      {
	uint8_t ct;
	if ( arg_int < 64 )
	  ct = 0x060;		/* CT_LOW: contrast reduced */
	else if ( arg_int < 128 )
	  ct = 0x070;		/* normal contrast */
	else if ( arg_int < 192 )
	  ct = 0x050;		/* CT_HIGH1: contrast slightly increased */
	else
	  ct = 0x030;		/* CT_HIGH2: contrast strongly increased */
	u8x8_mt12864k_write_register(u8x8, 0x08f | ct);
      }
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:

      v.ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
      v.x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      v.c = ((u8x8_tile_t *)arg_ptr)->cnt;
      v.arg_int = arg_int;    
      
      if ( v.x < 8 )
      {
	u8x8_mt12864k_select(u8x8, U8X8_MT12864K_CHIP1_ADR);
	u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, 1, NULL);
	u8x8_mt12864k_out(u8x8, &v, arg_ptr);
	u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, 0, NULL);
      }
      if ( v.x < 16 )
      {
	u8x8_mt12864k_select(u8x8, U8X8_MT12864K_CHIP2_ADR);
	u8x8->cad_cb(u8x8, U8X8_MSG_CAD_START_TRANSFER, 2, NULL);
	u8x8_mt12864k_out(u8x8, &v, arg_ptr);
	u8x8->cad_cb(u8x8, U8X8_MSG_CAD_END_TRANSFER, 0, NULL);
      }
      break;
    default:
      return 0;
  }
  return 1;
}
