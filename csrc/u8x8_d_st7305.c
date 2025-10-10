/*

  u8x8_d_st7305.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2025, olikraus@gmail.com
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


  st7305: 240 x 320             122x250
  st7305: 264 x 320             168x384, 200x200
    Mono TFT Display Driver with Controller
    
  st7305 seems to be almost compatible to the st7303, however they have a different memory architecture
  
  https://github.com/olikraus/u8g2/issues/2436
  
    No Hardware Flip
    No U8x8 Support

*/


#include "u8x8.h"
#include <string.h>

static const uint8_t u8x8_d_st7305_122x250_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x29), 				// display on
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7305_122x250_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x028),		                /* display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7305_122x250_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_CA(0x36, 0x60), 			// Memory Control 
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7305_122x250_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_CA(0x36, 0xa0), 			// Memory Control 
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};




/*===================================================*/
/* see also: http://www.technoblogy.com/show?3YB0 */


static uint8_t *u8x8_st7305_convert_a0(u8x8_t *u8x8, uint8_t *p)   
{
  static uint8_t buf[6];
  
#ifdef NOT_WORKING
  /* u8g2 first row */
  static uint8_t map1[16] = {
      /* 0x00 0000 */ 0,
      /* 0x01 0001 */0x04,
      /* 0x02 0010 */0x01,
      /* 0x03 0011 */0x04+0x01,
      /* 0x04 0100 */0x40,
      /* 0x05 0101 */0x40+0x04,
      /* 0x06 0110 */0x40+0x01,
      /* 0x07 0111 */0x40+0x04+0x01,
      /* 0x08 1000 */ 0x10,
      /* 0x09 1001 */ 0x10+0x04,
      /* 0x0a 1010 */ 0x10+0x01,
      /* 0x0b 1011 */ 0x10+0x04+0x01,
      /* 0x0c 1100 */ 0x40+0x10,
      /* 0x0d 1101 */ 0x40+0x10+0x04,
      /* 0x0e 1110 */ 0x40+0x10+0x01,
      /* 0x0f 1111 */  0x40+0x10+0x04+0x01
  };
  /* u8g2 second row */
  static uint8_t map2[16] = {
      /* 0x00 0000 */ 0,
      /* 0x01 0001 */0x08,
      /* 0x02 0010 */0x02,
      /* 0x03 0011 */0x08+0x02,
      /* 0x04 0100 */0x80,
      /* 0x05 0101 */0x80+0x08,
      /* 0x06 0110 */0x80+0x02,
      /* 0x07 0111 */0x80+0x08+0x02,
      /* 0x08 1000 */ 0x20,
      /* 0x09 1001 */ 0x20+0x08,
      /* 0x0a 1010 */ 0x20+0x02,
      /* 0x0b 1011 */ 0x20+0x08+0x02,
      /* 0x0c 1100 */ 0x80+0x20,
      /* 0x0d 1101 */ 0x80+0x20+0x08,
      /* 0x0e 1110 */ 0x80+0x20+0x02,
      /* 0x0f 1111 */  0x80+0x20+0x08+0x02
  };
  #endif

 /* contributed by https://github.com/ischenz */
 /* u8g2 first row */ 
  static uint8_t map1[16] = { 
      0x00, 0x02, 0x08, 0x0A, 
      0x20, 0x22, 0x28, 0x2A, 
      0x80, 0x82, 0x88, 0x8A, 
      0xA0, 0xA2, 0xA8, 0xAA }; 
 /* u8g2 second row */ 
  static uint8_t map2[16] = { 
    0x00, 0x01, 0x04, 0x05, 
    0x10, 0x11, 0x14, 0x15, 
    0x40, 0x41, 0x44, 0x45, 
    0x50, 0x51, 0x54, 0x55 };  
  
  memset(buf, 0, 6);
  

  // first row, left 12 pixel out of 24 pixel (=3 bytes)

  buf[0] |= map1[p[0]>>4];
  buf[1] |= map1[p[0] & 0x0f];
  buf[2] |= map1[p[1]>>4];

 // first row, right 12 pixel
   
  buf[3] |= map1[p[1] & 0x0f];
  buf[4] |= map1[p[2]>>4];
  buf[5] |= map1[p[2] & 0x0f];
  
  p += u8x8->display_info->tile_width;

  // second row, left 12 pixel

  buf[0] |= map2[p[0]>>4];
  buf[1] |= map2[p[0] & 0x0f];
  buf[2] |= map2[p[1]>>4];
  
 // second row, right 12 pixel

  buf[3] |= map2[p[1] & 0x0f];
  buf[4] |= map2[p[2]>>4];
  buf[5] |= map2[p[2] & 0x0f];
  return buf;
}




/*===================================================*/
/* 
  see also:
  https://github.com/zhcong/st7302-for-arduino/blob/c9390fabcacefe7c36a113cd3e62959418c13b97/libraries/st7305SPI/st7305SPI.cpp#L21
*/
static const uint8_t u8x8_d_st7305_122x250_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  U8X8_C(0x01),                                 // software resets
  U8X8_DLY(100),
  U8X8_C(0x28), 				// display off
  U8X8_CAA(0xC7, 0x26, 0xE9), 			// disable OSC
  U8X8_CA(0xD1, 0x00), 			// Booster disable
  U8X8_DLY(20),
  U8X8_C(0x10),                                 // sleep in: enter sleep mode
  U8X8_DLY(20),
  U8X8_C(0x01),                                 // software reset
  U8X8_DLY(20),                                 // wait
  //U8X8_C(0x38), 				// High Power Mode
  //U8X8_CA(0xEB, 0x02), 			// Disable NVM Load
  //U8X8_CA(0xD7, 0x68), 			// NVM Load Control: Enable ID1 ID2 ID3 Load
  //U8X8_CA(0xD1, 0x01), 			// Booster Enable
  //U8X8_CA(0xC0, 0x80), 			// Gate Voltage Setting VGH=12V (upper 4 bit, 8V-15V); VGL=-5V (lower 4 bit, -5V .. -10V)
  U8X8_CAA(0xD6, 0x17, 0x02),
  U8X8_CA(0xD1, 0x01), 
  U8X8_CAA(0xC0, 0x12, 0x0a),//chen.x
  U8X8_C(0x0C1),                                // Source Voltage Control 1
  U8X8_A4(115,0x3E,0x3C,0x3C), 
  U8X8_C(0x0C2),                                // Source Voltage Control 1
  U8X8_A4(0,0x21,0x23,0x23),       // Source low voltage in reflective and transmissive mode 
  U8X8_C(0x0C4),    //ischen.x
  U8X8_A4(50,0x5C,0x5A,0x5A), 
  U8X8_C(0x0C5),    //ischen.x
  U8X8_A4(50,0x35,0x37,0x37),
  U8X8_CAA(0xD8, 0x80, 0xE9),
  
  //U8X8_CA(0xCB, 0x14), 			// VCOMH: 0x14 = 4V (0x28 = 5V)
  U8X8_CA(0xB2, 0x12),
  U8X8_CAA(0xB3, 0xE5, 0xF6), 			// Update Period Gate EQ Control, why 0x77??? it should be 0x66 according to the datasheet
  U8X8_A8(0x17, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x71),
  U8X8_CAA(0xB4, 0x05, 0x46), 			// Update Period Gate EQ Control, why 0x77??? it should be 0x66 according to the datasheet
  U8X8_A6(0x77, 0x77, 0x77, 0x77, 0x76, 0x45),
  U8X8_CAAA(0x62, 0x32, 0x03, 0x1f),                                
  U8X8_CA(0xB7, 0x13),
  U8X8_CA(0xB0, 0x32), 			// Duty Cycle... this must be before sleep out, 200x200 display: 0x64 --> 0x32
  
  U8X8_C(0x11),                                 // sleep out: furn off sleep mode
  U8X8_DLY(120),
  U8X8_CA(0xC9, 0x00),
  U8X8_CA(0x36, 0xa4), 			// Memory Control, 0xa4 for the 200x200 display
  
  U8X8_CA(0x3A, 0x11), 			// Data Format 
  U8X8_CA(0xB9, 0x20), 			// Source Setting: Clear RAM off *******<-----
  U8X8_CA(0xB8, 0x29), 			// Panel Setting / Panel Layout 
  U8X8_CAA(0x2A, 0x16, 0x27), 			// col addr
  U8X8_CAA(0x2B, 0x00, 0x63), 			// row addr
  U8X8_CA(0x35, 0x00),
  U8X8_CA(0xD0, 0xFF), 			// Not in datasheet
  //U8X8_CA(0x72, 0x00), 			// Not in datasheet
  //U8X8_CAA(0xB2,1, 5),                  // Frame Rate for High and Low Power Mode (hier: 32Hz and 8Hz) 
  U8X8_C(0x38), 				// enable Low Power Mode...: 8Hz, see above
  U8X8_C(0x29), 				// display on
  U8X8_C(0x20),
  U8X8_CA(0xBB, 0x4F),
  U8X8_DLY(100),
 
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()           			/* end of sequence */
};



static const u8x8_display_info_t u8x8_st7305_122x250_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 20,
  /* pre_chip_disable_wait_ns = */ 20,
  /* reset_pulse_width_ms = */ 3, 	
  /* post_reset_wait_ms = */ 3, 		/**/
  /* sda_setup_time_ns = */ 10,		/* */
  /* sck_pulse_width_ns = */ 30,	/*  */
  /* sck_clock_hz = */ 2000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,	/* 400KHz */
  /* data_setup_time_ns = */ 15,
  /* write_pulse_width_ns = */ 70,	
  /* tile_width = */ 16,
  /* tile_height = */ 32,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 122,
  /* pixel_height = */ 250
};

uint8_t u8x8_d_st7305_122x250(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint16_t x;
  uint8_t c, i, y;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_init_seq);    
      break;
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_st7305_122x250_display_info);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int<<2 );	
      u8x8_cad_SendArg(u8x8, arg_int>>6 );	
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 8;
      x += u8x8->x_offset;
      y= (((u8x8_tile_t *)arg_ptr)->y_pos);
      y*=4;
    
      y+=115;           // specific for the 122x250 LCD

      u8x8_cad_StartTransfer(u8x8);

      for( i = 0; i < 4; i++ )
      {
        
        u8x8_cad_SendCmd(u8x8, 0x2a);
        u8x8_cad_SendArg(u8x8, 0x19);   // specific for the 122x250 LCD
        u8x8_cad_SendArg(u8x8, 0x3a );
      
        u8x8_cad_SendCmd(u8x8, 0x2b ); 
        u8x8_cad_SendArg(u8x8, y+i); 
        u8x8_cad_SendArg(u8x8, y+i); 
        u8x8_cad_SendCmd(u8x8, 0x02c );		// write data 
      
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        
        ptr += u8x8->display_info->tile_width*i*2;
        
        c = (c+2)/3;          // calculate the number of 24 bit blocks to send
        
        
        while( c > 0 )
        {
          u8x8_cad_SendData(u8x8, 6, u8x8_st7305_convert_a0(u8x8, ptr)); 	
          ptr+=3;
          --c;
        }
      }
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}


/*=====================================================================*/
/* 200x200, https://admin.osptek.com/uploads/YDP_154_H008_V3_c24b455ff9.pdf */


static const u8x8_display_info_t u8x8_st7305_200x200_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 20,
  /* pre_chip_disable_wait_ns = */ 20,
  /* reset_pulse_width_ms = */ 3, 	
  /* post_reset_wait_ms = */ 3, 		/**/
  /* sda_setup_time_ns = */ 10,		/* */
  /* sck_pulse_width_ns = */ 30,	/*  */
  /* sck_clock_hz = */ 2000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,	/* 400KHz */
  /* data_setup_time_ns = */ 15,
  /* write_pulse_width_ns = */ 70,	
  /* tile_width = */ 26,   /* tile width is 26*8=208, because this display requires 12 bit blocks, which would be 204 pixel, so next tile is at 208 */
  /* tile_height = */ 25,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 200,              /* not 100% sure, whether this works with the tile_width of 26... */
  /* pixel_height = */ 200
};

uint8_t u8x8_d_st7305_200x200(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint16_t x;
  uint8_t c, i, y;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_init_seq);    
      break;
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_st7305_200x200_display_info);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int<<2 );	
      u8x8_cad_SendArg(u8x8, arg_int>>6 );	
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 8;
      x += u8x8->x_offset;
      y= (((u8x8_tile_t *)arg_ptr)->y_pos);
      y*=4;
    
      y+=0;           // 200x200 display

      u8x8_cad_StartTransfer(u8x8);

      for( i = 0; i < 4; i++ )
      {
        
        u8x8_cad_SendCmd(u8x8, 0x2a);   // column address set
        u8x8_cad_SendArg(u8x8, 0x16);   // 0x019 for the 122x250 LCD --> 0x016 for the 200x200 display
        u8x8_cad_SendArg(u8x8, 0x27 );  // 204 pixel for the 200x200 display
      
        u8x8_cad_SendCmd(u8x8, 0x2b ); 
        u8x8_cad_SendArg(u8x8, y+i); 
        u8x8_cad_SendArg(u8x8, y+i); 
        u8x8_cad_SendCmd(u8x8, 0x02c );		// write data 
      
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        
        ptr += u8x8->display_info->tile_width*i*2;
        
        c = (c+2)/3;          // calculate the number of 24 bit blocks to send
        
        
        while( c > 0 )
        {
          u8x8_cad_SendData(u8x8, 6, u8x8_st7305_convert_a0(u8x8, ptr)); 	
          ptr+=3;
          --c;
        }
      }
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}


/*=====================================================================*/
/* 168x384, https://github.com/olikraus/u8g2/issues/2661 */


#ifdef NOT_USED
static const uint8_t u8x8_d_st7305_168x384_powersave0_seq[] = {
    U8X8_START_TRANSFER(), /* enable chip, delay is part of the transfer start */
    U8X8_C(0x29),          // display on
    U8X8_END_TRANSFER(),   /* disable chip */
    U8X8_END()             /* end of sequence */
};


static const uint8_t u8x8_d_st7305_168x384_powersave1_seq[] = {
    U8X8_START_TRANSFER(), /* enable chip, delay is part of the transfer start */
    U8X8_C(0x028),         /* display off */
    U8X8_END_TRANSFER(),   /* disable chip */
    U8X8_END()             /* end of sequence */
};


static const uint8_t u8x8_d_st7305_168x384_flip0_seq[] = {
    U8X8_START_TRANSFER(), /* enable chip, delay is part of the transfer start */
    // U8X8_CA(0x36, 0x60), 			// Memory Control
    U8X8_END_TRANSFER(), /* disable chip */
    U8X8_END()           /* end of sequence */
};
#endif


static const uint8_t u8x8_d_st7305_168x384_init_seq[] = {
    U8X8_START_TRANSFER(),


    /* Software Reset */
    U8X8_C(0x01),
    U8X8_DLY(100),


    /* Settings */
    // 以下在芯片手册基础上，按照显示屏厂家给的示例代码调整
    U8X8_CAA(0xD6, 0x13, 0x02), // NVM Load Control
    U8X8_CA(0xD1, 0x01),        // Booster Enable
    U8X8_CAA(0xC0, 0x12, 0x0A), // Gate Voltage Setting: VGH=15V, VGL=-10V


    U8X8_CAAAA(0xC1, 0x3C, 0x3E, 0x3C, 0x3C), // VSHP Setting: VSHP1~4 = 4.8V
    U8X8_CAAAA(0xC2, 0x23, 0x21, 0x23, 0x23), // VSLP Setting: VSHP1~4 = 0.98V
    U8X8_CAAAA(0xC4, 0x5A, 0x5C, 0x5A, 0x5A), // VSHN Setting: VSHP1~4 = -3.6V
    U8X8_CAAAA(0xC5, 0x37, 0x35, 0x37, 0x37), // VSLN Setting: VSHP1~4 = 0.22V


    U8X8_CAA(0xD8, 0xA6, 0xE9), // OSC Setting
    U8X8_CA(0xB2, 0x12),        // Frame Rate Control: HPM=32Hz, LPM=1Hz


    U8X8_CAAAA(0xB3, 0xE5, 0xF6, 0x17, 0x77), // Update Period Gate EQ Control in HPM
    U8X8_A6(0x77, 0x77, 0x77, 0x77, 0x77, 0x71),


    U8X8_CAA(0xB4, 0x05, 0x46), // Update Period Gate EQ Control in LPM
    U8X8_A6(0x77, 0x77, 0x77, 0x77, 0x76, 0x45),


    U8X8_CAAA(0x62, 0x32, 0x03, 0x1F), // Gate Timing Control
    U8X8_CA(0xB7, 0x13),               // Source EQ Enable
    U8X8_CA(0xB0, 0x60),               // Gate Line Setting: 384 line


    U8X8_C(0x11), // Sleep Out
    U8X8_DLY(10),


    U8X8_CA(0xC9, 0x00), // Source Voltage Select: VSHP1; VSLP1 ; VSHN1 ; VSLN1
    U8X8_CA(0x36, 0x48), // Memory Data Access Control
    U8X8_CA(0x3A, 0x11), // Data Format Select
    U8X8_CA(0xB9, 0x20), // Gamma Mode Setting: Mono 00:4GS
    U8X8_CA(0xB8, 0x29), // Panel Setting, 1-Dot inversion, Frame inversion, One Line Interlace


    U8X8_CAA(0x2A, 0x17, 0x24), // Column Address Setting: 0X24-0X16=14, 14*12=168
    U8X8_CAA(0x2B, 0x00, 0xBF), // Row Address Setting: 192*2=384
    U8X8_CA(0x35, 0x00),        // TE Setting
    U8X8_CA(0xD0, 0xFF),        // Enable Auto Power down
    U8X8_C(0x38),               // High Power Mode ON
    // U8X8_C(0x39),               // Low Power Mode ON


    /* Display ON */
    U8X8_C(0x29),        // Display ON
    U8X8_C(0x20),        // Display Inversion Off
    U8X8_CA(0xBB, 0x4F), // Enable Clear RAM to 0


    U8X8_END_TRANSFER(),
    U8X8_END()


};


static const u8x8_display_info_t u8x8_st7305_168x384_display_info =
    {
        /* chip_enable_level = */ 0,
        /* chip_disable_level = */ 1,


        /* post_chip_enable_wait_ns = */ 20,
        /* pre_chip_disable_wait_ns = */ 20,
        /* reset_pulse_width_ms = */ 3,
        /* post_reset_wait_ms = */ 3,   /**/
        /* sda_setup_time_ns = */ 10,   /* */
        /* sck_pulse_width_ns = */ 30,  /*  */
        /* sck_clock_hz = */ 2000000UL, /* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
        /* spi_mode = */ 0,             /* active high, rising edge */
        /* i2c_bus_clock_100kHz = */ 4, /* 400KHz */
        /* data_setup_time_ns = */ 15,
        /* write_pulse_width_ns = */ 70,
        /* tile_width = */ 21, /* tile width is 21*8=168, because this display requires 12 bit blocks, which would be 168 pixel, so next tile is at 168 */
        /* tile_height = */ 48,
        /* default_x_offset = */ 0,
        /* flipmode_x_offset = */ 0,
        /* pixel_width = */ 168, /* not 100% sure, whether this works with the tile_width of 21... */
        /* pixel_height = */ 384};


uint8_t u8x8_d_st7305_168x384(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint16_t x;
  uint8_t c, i, y;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_168x384_init_seq);    
      break;
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_st7305_168x384_display_info);
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7305_122x250_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int<<2 );	
      u8x8_cad_SendArg(u8x8, arg_int>>6 );	
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 8;
      x += u8x8->x_offset;
      y= (((u8x8_tile_t *)arg_ptr)->y_pos);
      y*=4;
    
      y+=0;         // specific for the 168x384 LCD


      u8x8_cad_StartTransfer(u8x8);


      for( i = 0; i < 4; i++ )
      {
        
        u8x8_cad_SendCmd(u8x8, 0x2a);   // column address set
        u8x8_cad_SendArg(u8x8, 0x17);   // 0x019 for the 122x250 LCD --> 0x016 for the 200x200 display
        u8x8_cad_SendArg(u8x8, 0x24);  // 204 pixel for the 200x200 display
      
        u8x8_cad_SendCmd(u8x8, 0x2b); 
        u8x8_cad_SendArg(u8x8, y+i); 
        u8x8_cad_SendArg(u8x8, y+i); 
        u8x8_cad_SendCmd(u8x8, 0x2c);		// write data 
      
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        
        ptr += u8x8->display_info->tile_width*i*2;
        
        c = (c+2)/3;          // calculate the number of 24 bit blocks to send
        
        
        while( c > 0 )
        {
          u8x8_cad_SendData(u8x8, 6, u8x8_st7305_convert_a0(u8x8, ptr)); 	
          ptr+=3;
          --c;
        }
      }
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}