/*

  u8x8_d_st7301.c

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2024, olikraus@gmail.com
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


  ST7301: 
    Ultra-Low Power Active Matrix 240 x 320
    Mono TFT Display Driver with Controller
  
  https://github.com/olikraus/u8g2/issues/2436

*/


#include "u8x8.h"

static const uint8_t u8x8_d_st7301_122x250_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x29), 				// display on
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7301_122x250_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x028),		                /* display off */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7301_122x250_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_CA(0xC4, 0x02), 			/* COM Output Status, Bits 0 & 1 */
  //U8X8_C(0xA1), 				/* Column Address Direction: Bit 0 */
  //U8X8_C(0x0a1),				/* segment remap a0/a1*/
  //U8X8_C(0x0c8),				/* c0: scan dir normal, c8: reverse */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_st7301_122x250_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  //U8X8_C(0x0a0),				/* segment remap a0/a1*/
  //U8X8_C(0x0c0),				/* c0: scan dir normal, c8: reverse */
  //U8X8_CA(0xC4, 0x03), 			/* COM Output Status, Bits 0 & 1 */
  //U8X8_C(0xA0), 				/* Column Address Direction: Bit 0 */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             			/* end of sequence */
};




/*===================================================*/
/* see also: http://www.technoblogy.com/show?3YB0 */

static uint8_t u8x8_d_st7301_generic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint16_t x;
  uint8_t c;
  uint8_t *ptr;
  switch(msg)
  {
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_st7301_122x250_display_info);
      break;
    */
    /* handled by the calling function
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_init_seq);    
      break;
    */
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_flip1_seq);
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

      u8x8_cad_StartTransfer(u8x8);
    
      u8x8_cad_SendCmd(u8x8, 0x013);
      u8x8_cad_SendArg(u8x8, (x>>8) );
      u8x8_cad_SendArg(u8x8, (x&255) );
      u8x8_cad_SendCmd(u8x8, 0x0b1 ); 
      u8x8_cad_SendArg(u8x8, (((u8x8_tile_t *)arg_ptr)->y_pos)); 


      u8x8_cad_SendCmd(u8x8, 0x01d );		// write data 
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        /* SendData can not handle more than 255 bytes */
        if ( c > 31 )
        {
          u8x8_cad_SendData(u8x8, 248, ptr); 	/* 31*8=248 */
          ptr+=248;
          c -= 31;
        }
        
        u8x8_cad_SendData(u8x8, c*8, ptr); 	
        arg_int--;
      } while( arg_int > 0 );

      
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;
}

/*===================================================*/


/* 

  https://github.com/zhcong/ST7302-for-arduino/blob/c9390fabcacefe7c36a113cd3e62959418c13b97/libraries/ST7302SPI/ST7302SPI.cpp#L21

  send_command(0x38);
  send_command(0xEB);//Enable OTP
  send_param(0x02);
  send_command(0xD7);//OTP Load Control
  send_param(0x68);
  send_command(0xD1);//Auto Power Control
  send_param(0x01);
  send_command(0xC0);//Gate Voltage Setting VGH=12V ; VGL=-5V
  send_param(0x80);
  send_command(0xC1);//VSH Setting
  send_param(0x28);//
  send_param(0x28);
  send_param(0x28);
  send_param(0x28);
  send_param(0x14);
  send_param(0x00);
  send_command(0xC2);//VSL Setting VSL=0
  send_param(0x00);
  send_param(0x00);
  send_param(0x00);
  send_param(0x00);
  send_command(0xCB);//VCOMH Setting
  send_param(0x14);//14  0C   7
  send_command(0xB4);//Gate EQ Setting HPM EQ LPM EQ
  send_param(0xE5);
  send_param(0x77);
  send_param(0xF1);
  send_param(0xFF);
  send_param(0xFF);
  send_param(0x4F);
  send_param(0xF1);
  send_param(0xFF);
  send_param(0xFF);
  send_param(0x4F);
  send_command(0x11);//Sleep out
  delay(100);  // delay_ms 100ms
  send_command(0xC7);//OSC Setting
  send_param(0xA6);
  send_param(0xE9);
  send_command(0xB0);   //Duty Setting
  send_param(0x64);  //250duty/4=63
  send_command(0x36);//Memory Data Access Control
  send_param(0x20);
  send_command(0x3A);//Data Format Select 3 write for 24 bit
  send_param(0x11);
  send_command(0xB9);//Source Setting
  send_param(0x23);
  send_command(0xB8);//Panel Setting Frame inversion
  send_param(0x09);
  send_command(0x2A);////Column Address Setting S61~S182
  send_param(0x05);
  send_param(0x36);
  send_command(0x2B);////Row Address Setting G1~G250
  send_param(0x00);
  send_param(0xC7);
  

  send_command(0xD0);
  send_param(0x1F);
  send_command(0x29);//Display on
  send_command(0xB9);//enable CLR RAM
  send_param(0xE3);
  delay(100);
  send_command(0xB9);//enable CLR RAM
  send_param(0x23);
  send_command(0x72);
  send_param(0x00);         //Destress OFF
  send_command(0x39);//LPM
  send_command(0x2A);   //Column Address Setting
  send_command(0x19);
  send_param(0x23);  //35
  send_command(0x2B);   //Row Address Setting
  send_param(0);
  send_param(0x7C);
  send_param(0x2C);   //write image data
  delay(100);

*/
static const uint8_t u8x8_d_st7301_122x250_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  
  U8X8_C(0x28), 				// display off
  U8X8_C(0x10),                                 // sleep in: enter sleep mode
  U8X8_DLY(5),
  U8X8_C(0x38), 				// High Power Mode
  U8X8_CA(0xEB, 0x02), 			// Disable NVM Load
  U8X8_CA(0xD7, 0x68), 			// NVM Load Control: Enable ID1 ID2 ID3 Load
  U8X8_CA(0xD1, 0x01), 			// Booster Enable
  U8X8_CA(0xC0, 0x80), 			// Gate Voltage Setting VGH=12V (upper 4 bit, 8V-15V); VGL=-5V (lower 4 bit, -5V .. -10V)
  U8X8_C(0x0C1),                                // Source Voltage Control 1
  U8X8_A6(0x28,0x28,0x28,0x28,0x14,0x00),       // Source high voltage in reflective and transmissive mode, gamma voltage 1&2 
  U8X8_C(0x0C2),                                // Source Voltage Control 1
  U8X8_A4(0x00,0x00,0x00,0x00),       // Source low voltage in reflective and transmissive mode 
  U8X8_CA(0xCB, 0x14), 			// VCOMH: 0x14 = 4V (0x28 = 5V)
  U8X8_CAA(0xB4, 0xE5, 0x77), 			// Update Period Gate EQ Control, why 0x77??? it should be 0x66 according to the datasheet
  U8X8_A8(0xF1, 0xFF, 0xFF, 0x4F, 0xF1, 0xFF, 0xFF, 0X4F),
  U8X8_CA(0xB0, 0x64), 			// Duty Cycle... this must be before sleep out
  U8X8_C(0x11),                                 // sleep out: furn off sleep mode
  U8X8_DLY(120),
  U8X8_CAA(0xC7, 0xA6, 0xE9), 			// Enable OSC
  U8X8_CA(0x36, 0x20), 			// Memory Control 
  U8X8_CA(0x3A, 0x11), 			// Data Format 
  U8X8_CA(0xB9, 0x23), 			// Source Setting: Clear RAM off 
  U8X8_CA(0xB8, 0x09), 			// Panel Setting / Panel Layout 
  U8X8_CAA(0x2A, 0x05, 0x36), 			// col addr
  U8X8_CAA(0x2B, 0x00, 0xC7), 			// row addr
  U8X8_CA(0xD0, 0x1F), 			// Not in datasheet
  U8X8_C(0x29), 				// display on
  U8X8_CA(0x72, 0x00), 			// Not in datasheet
  U8X8_CAA(0xB2,1, 5),                  // Frame Rate for High and Low Power Mode (hier: 32Hz and 8Hz) 
  U8X8_C(0x39), 				// enable Low Power Mode...: 8Hz, see above
  U8X8_DLY(100),
    
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()           			/* end of sequence */
};




static const u8x8_display_info_t u8x8_st7301_122x250_display_info =
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

uint8_t u8x8_d_st7301_122x250(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    
  if ( u8x8_d_st7301_generic(u8x8, msg, arg_int, arg_ptr) != 0 )
    return 1;
  
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_st7301_122x250_init_seq);    
      break;
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_st7301_122x250_display_info);
      break;
    default:
      return 0;
  }
  return 1;
}
