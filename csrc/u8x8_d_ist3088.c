/*

  u8x8_d_ist3088.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2019, olikraus@gmail.com
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


  IST3088: 320x240 monochrome LCD, 16 gray levels with B/W mode
  
  https://github.com/olikraus/u8g2/issues/1887
  
  CAD: 011
  commands and arg/data are always 16 bit, MSB first
  
*/


#include "u8x8.h"

/* powersave: maybe we could go to sleep mode, see register 3 */
static const uint8_t u8x8_d_ist3088_320x240_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CCAA(0x0, 0x07, 0x00, 0x01), 			// Display Control Bit 2: BW, 1: Invert, 0: Display enable
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ist3088_320x240_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CCAA(0x0, 0x07, 0x00, 0x00), 			// Display Control Bit 2: BW, 1: Invert, 0: Display enable
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ist3088_320x240_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CCAA(0x0, 0x01, 0x00, 0x00), 			// Driver Control, 1/240 Duty
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ist3088_320x240_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_CCAA(0x0, 0x01, 0x00, 0x00), 			// Driver Control, 1/240 Duty
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};




/*===================================================*/

static uint8_t u8x8_d_ist3088_generic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  //uint16_t x;
  int i;
  uint8_t y;
  uint8_t c;
  uint8_t *ptr;
  switch(msg)
  {
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ist3088_320x240_display_info);
      break;
    */
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ist3088_320x240_init_seq);    
      break;
    */
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_ist3088_320x240_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_ist3088_320x240_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ist3088_320x240_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ist3088_320x240_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x00 );
      u8x8_cad_SendCmd(u8x8, 0x05 );
      u8x8_cad_SendArg(u8x8, 0 );	
      u8x8_cad_SendArg(u8x8, arg_int );	
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      /*
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 8;
      x += u8x8->x_offset;
    */

      y = (((u8x8_tile_t *)arg_ptr)->y_pos);
      y*=8;

      u8x8_cad_StartTransfer(u8x8);
    
    
        c = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles */
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;	/* data ptr to the tiles */
        for( i = 0; i < 8; i++ )
        {
          u8x8_cad_SendCmd(u8x8, 0x0);
          u8x8_cad_SendCmd(u8x8, 0x8);
          u8x8_cad_SendArg(u8x8, y); 
          u8x8_cad_SendArg(u8x8, 0 );


          u8x8_cad_SendCmd(u8x8, 0x0);		// write data 
          u8x8_cad_SendCmd(u8x8, 0x9);		// write data 
          
          
          //c = ((u8x8_tile_t *)arg_ptr)->cnt;	/* number of tiles */
          u8x8_cad_SendData(u8x8, c, ptr);	/* note: SendData can not handle more than 255 bytes, send one line of data */
          
          ptr += u8x8->display_info->tile_width;
          y ++;
        }

      
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}

/*===================================================*/

/*
    SdCmd(0x01);                // driver control
	SdData(0x00); SdData(0x00); // c1-->c240 s1-->s240 duty=1/240
    SdCmd(0x02);                // driver control
	SdData(0x01); SdData(0x4c);	//0x15
    SdCmd(0x03);                // power control
	SdData(0x00); SdData(0x70); // VC VR Vf=1
    SdCmd(0x04);                // power control
	SdData(0x04); SdData(0x61); //0x61  //1/14b
    SdCmd(0x05);                // CT control
	SdData(0x00); SdData(0x6a); // 0X76 
    SdCmd(0x06);                // CT control
	SdData(0x00); SdData(0x03); 
    SdCmd(0x23);                // B/W MODE,16G NEED REMOVE
	SdData(0x00); SdData(0x04); //04:mono
    SdCmd(0x28);                // OSC
	SdData(0x00); SdData(0x0c);
    SdCmd(0x37);                
        SdData(0x00); SdData(0x12);
    SdCmd(0x07);                
        SdData(0x00); SdData(0x01);
*/

/* https://github.com/olikraus/u8g2/issues/1887 */
static const uint8_t u8x8_d_ist3088_320x240_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */

  /* disableLCD */
  U8X8_CCAA(0x0, 0x07, 0x00, 0x00), 			// Display Control Bit 2: BW, 1: Invert, 0: Display enable

  U8X8_CCAA(0x0, 0x01, 0x00, 0x00), 			// Driver Control, 1/240 Duty
  U8X8_CCAA(0x0, 0x02, 0x01, 0x4c), 			// Polarity Control

  U8X8_CCAA(0x0, 0x06, 0x00, 0x03), 			// Entry mode: h/v increment  
  //U8X8_CCAA(0x0, 0x04, 0x07, 0x61), 			// Power Control 2: 0x07: 1/16 Bias, Vout1 x 4
  U8X8_CCAA(0x0, 0x04, 0x04, 0x61), 			// Power Control 2: 0x07: 1/16 Bias, Vout1 x 4
  U8X8_CCAA(0x0, 0x05, 0x00, 0x6a), 			// Contrast, 0..255, 

  U8X8_CCAA(0x0, 0x03, 0x00, 0x40), 			// Enable voltage converter 
  U8X8_DLY(10),
  U8X8_CCAA(0x0, 0x03, 0x00, 0x60), 			// Enable voltage regulator 
  U8X8_DLY(10),
  U8X8_CCAA(0x0, 0x03, 0x00, 0x70), 			// Enable voltage follower 
  U8X8_DLY(10),

  U8X8_CCAA(0x0, 0x23, 0x00, 0x04), 			// Monochrome mode
  U8X8_CCAA(0x0, 0x28, 0x00, 0x0c), 			// Frame Rate Control
  U8X8_CCAA(0x0, 0x37, 0x00, 0x01), 			// Frame Rate Control
  U8X8_CCAA(0x0, 0x0d, 39, 0x00), 			// X End and X Start
  
  U8X8_CCAA(0x0, 0x0e, 239, 0x00), 			// Y End and Y Start

  /* enable LCD (will be done by powersave0) */
  //U8X8_CCAA(0x0, 0x07, 0x00, 0x01), 			// Display Control Bit 2: BW, 1: Invert, 0: Display enable
    
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()           			/* end of sequence */
};




static const u8x8_display_info_t u8x8_ist3088_320x240_display_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 110,
  /* pre_chip_disable_wait_ns = */ 110,
  /* reset_pulse_width_ms = */ 10, 	
  /* post_reset_wait_ms = */ 10, 		/**/
  /* sda_setup_time_ns = */ 110,		/* */
  /* sck_pulse_width_ns = */ 125,	/*  */
  /* sck_clock_hz = */ 4000000UL,	/* 250ns cycle, see page 49 of the IST3088 datasheet*/
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,	/* 400KHz, IST8033 das not include a I2C interface */
  /* data_setup_time_ns = */ 70,
  /* write_pulse_width_ns = */ 60,	
  /* tile_width = */ 40,
  /* tile_hight = */ 30,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 320,
  /* pixel_height = */ 240
};

uint8_t u8x8_d_ist3088_320x240(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    
  if ( u8x8_d_ist3088_generic(u8x8, msg, arg_int, arg_ptr) != 0 )
    return 1;
  
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ist3088_320x240_init_seq);    
      break;
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ist3088_320x240_display_info);
      break;
    default:
      return 0;
  }
  return 1;
}
