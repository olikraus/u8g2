/*

  u8x8_d_uc1698.c
  
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


  
  There are one controller:
  UC1698u		160x160
  
  
  
*/
#include "u8x8.h"

static const uint8_t u8x8_d_uc1698u_powersave0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0ad), //on/off mode		/* display on, UC1698s */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

static const uint8_t u8x8_d_uc1698u_powersave1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */  
  U8X8_C(0x0ac), //on/off mode		/* display off, UC1698s */     
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

static const uint8_t u8x8_d_uc1698u_flip0_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0c4),            	        /* LCD Mapping */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

static const uint8_t u8x8_d_uc1698u_flip1_seq[] = {
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0c0),            	        /* LCD Mapping */
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

static const uint8_t lut_nibble[16][2] = {
  {0x00, 0x00}, // 0b0000
  {0x00, 0x0F}, // 0b0001
  {0x00, 0xF0}, // 0b0010
  {0x00, 0xFF}, // 0b0011
  {0x0F, 0x00}, // 0b0100
  {0x0F, 0x0F}, // 0b0101
  {0x0F, 0xF0}, // 0b0110
  {0x0F, 0xFF}, // 0b0111
  {0xF0, 0x00}, // 0b1000
  {0xF0, 0x0F}, // 0b1001
  {0xF0, 0xF0}, // 0b1010
  {0xF0, 0xFF}, // 0b1011
  {0xFF, 0x00}, // 0b1100
  {0xFF, 0x0F}, // 0b1101
  {0xFF, 0xF0}, // 0b1110
  {0xFF, 0xFF}, // 0b1111
};

uint8_t u8x8_d_uc1698u_common(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  
  uint8_t data1=0, data2=0;
  
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
    
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x = x<<3;         // Fast multiplication by 8 with shift
      x /= 3;           //The X-axis is addressed using 1 RAM cell. 1 cell is 3 pixels in 4k RGB format.
      x += u8x8->x_offset;
      
      u8x8_cad_SendCmd(u8x8,0xF4); //start X
      u8x8_cad_SendCmd(u8x8,x);
      
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y = y<<3;
      u8x8_cad_SendCmd(u8x8,0xF5); //start y
      u8x8_cad_SendCmd(u8x8,y);
          
      u8x8_cad_SendCmd(u8x8,0xF6); //width
      u8x8_cad_SendCmd(u8x8,x+53); // 53=159/3 The X-axis is addressed using 1 RAM cell. 1 cell is 3 pixels in 4k RGB format.
      
      u8x8_cad_SendCmd(u8x8,0xF7); //height
      u8x8_cad_SendCmd(u8x8,y+u8x8->display_info->tile_height); //20 tile_height
      
      data1 = (x>>0)&0x0F;
      data2 = (x>>4)&0x0F;
      data2 |= 0x10;
      
      u8x8_cad_SendCmd(u8x8,data1);
      u8x8_cad_SendCmd(u8x8,data2);
      
      data1=0, data2=0;
      data1 = (y>>0)&0x0F;
      data1 |= 0x60;   
      
      data2 = (y>>4)&0x0F; 
      data2 |= 0x70;
      
      u8x8_cad_SendCmd(u8x8,data1);
      u8x8_cad_SendCmd(u8x8,data2);
            
      c = ((u8x8_tile_t *)arg_ptr)->cnt;
      c = c<<3;                                 // Fast multiplication by 8 with shift
      ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
      
      do
      {
        for (uint16_t i=0; i<c; i++)
        {  
          const uint8_t *low  = lut_nibble[*ptr & 0x0F];
          const uint8_t *high = lut_nibble[*ptr >> 4];
          
          uint8_t arr_4k[4];
          arr_4k[3] = low[1];
          arr_4k[2] = low[0];
          arr_4k[1] = high[1];
          arr_4k[0] = high[0];
          
          u8x8_cad_SendData(u8x8, 4, arr_4k);
          
          if ((i - 19) % 20 == 0) 
          {
            u8x8_cad_SendData(u8x8, 1, NULL);
          }
          ptr++;
        }        
        arg_int--;
      } while( arg_int > 0 );

      
      u8x8_cad_EndTransfer(u8x8);
      break;
    /*	handled in the calling procedure 
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_uc1611_128x64_display_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_uc1701_dogs102_init_seq);
      break;
    */
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int  );	/* uc1611 has range from 0 to 255 */
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    default:
      return 0;
  }
  return 1;
}

/*================================================*/
/* https://github.com/olikraus/u8g2/issues/2164 */


static const u8x8_display_info_t u8x8_uc1698u_cg160160_display_info =
{
  /* chip_enable_level = */ 0,			/* use CS0 of the UC1611 */
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* pre_chip_disable_wait_ns = */ 10,	/* uc1611 datasheet, page 60, actually 0 */
  /* reset_pulse_width_ms = */     20, 
  /* post_reset_wait_ms = */    200, 	/* uc1611 datasheet, page 67 */
  /* sda_setup_time_ns = */ 10,		/* uc1611 datasheet, page 64, actually 0 */
  /* sck_pulse_width_ns = */ 60,	/* half of cycle time  */
  /* sck_clock_hz = */ 8000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,		/* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 30,	/* uc1611 datasheet, page 60 */
  /* write_pulse_width_ns = */ 80,	/* uc1611 datasheet, page 60 */
  /* tile_width = */ 20,		/* width of 20*8=160 pixel */
  /* tile_height = */ 20,
  /* default_x_offset = */ 37,
  /* flipmode_x_offset = */ 37,
  /* pixel_width = */ 160,
  /* pixel_height = */ 160
};

/*
System Reset: E2H 	--> DONE
Set Temp. Compensation: 24H --> DONE
Set up LCD format specific parameters MX,MY,etc(double-byte command): C0H,04H  --> FLIP0
Set line rate: A3H --> DONE
Set Pump Control (internal Vlcd): 2FH --> DONE
Set Isolation Clock Front (3 bytes command): 82H, 13H, 01H  --> DONE
Set Isolation Clock Back (3 bytes command): 82H, 14H, 00H  --> DONE
Set LCD Bias Ratio: EAH 
LCD Specific Operation Voltage Setting (double-byte command): 81H, 90H --> DONE
Set RAM Address Control: 80H --> DOES NOT MAKE SENSE
Set Page Addr. MSB: 72H 		--> DONE
Set Page Addr. LSB : 60H 		--> DONE
Set Column Addr. LSB: 00H 		--> DONE
Set Column Addr.MSB: 10H 		--> DONE
Window Program Enable : F8H 		--> NOT REQURED
Window Starting Column (double-byte command): F4H , 00H --> NOT REQURED
Window Ending Column (double-byte command): F6H, 9FH --> NOT REQURED
Set one bit for one pixel: D1H 		--> DONE
Set Display Enable: A9H 
*/

static const uint8_t u8x8_d_uc1698u_cg160160_init_seq[] = {
    
  U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */
  U8X8_C(0x0e2),		        /* system reset */
  U8X8_DLY(10),
  
  /*power control*/					
	U8X8_C(0x0e9),			//Bias Ratio:1/10 bias
	U8X8_C(0x02b),			//power control set as internal power
	U8X8_C(0x024),			//set temperate compensation as 0%
	U8X8_C(0x081),			//electronic potentionmeter
	U8X8_C(0x096),	                //start contrast level

	/*display control*/
	U8X8_C(0x0a4),			//all pixel off 
	U8X8_C(0x0a6),			//inverse display off

	/*lcd control*/
	U8X8_C(0x0c4),			//Set LCD Maping Control (MY=1, MX=0)
       
        
        
        U8X8_C(0x0a1),			//line rate 15.2klps
	U8X8_C(0x0d1),			//rgb-rgb
        
//        U8X8_C(0x0d6),			//64k color mode
        U8X8_C(0x0d5),			//4k color mode
 	      
	U8X8_C(0x084),			//12:partial display control disable


	/*n-line inversion*/
	U8X8_C(0x0c8),
	U8X8_C(0x010),			//enable NIV

	/*com scan fuction*/
	U8X8_C(0x0da),			//enable FRC,PWM,LRM sequence

	/*window*/
	U8X8_C(0x0f4),			//wpc0:column
	U8X8_C(0x025),			//start from 112
	U8X8_C(0x0f6),			//wpc1
	U8X8_C(0x05A),			//end:272

	U8X8_C(0x0f5),			//wpp0:row
	U8X8_C(0x000),			//start from 0
	U8X8_C(0x0f7),			//wpp1
	U8X8_C(0x09F),			//end 160

	U8X8_C(0x0f8),			//inside mode 

	U8X8_C(0x089),			//RAM control /** Draw right then down **/

	
	/*scroll line*/
	U8X8_C(0x040),			//low bit of scroll line
	U8X8_C(0x050),			//high bit of scroll line

	U8X8_C(0x090),			//14:FLT,FLB set
	U8X8_C(0x000),

	/*partial display*/
	U8X8_C(0x084),			//12,set partial display control:off
	U8X8_C(0x0f1),			//com end
	U8X8_C(0x09f),			//160
	U8X8_C(0x0f2),			//display start 
	U8X8_C(0x000),				//0
	U8X8_C(0x0f3),			//display end
	U8X8_C(0x0a0),	                //160
  
        U8X8_C(0x0ac),                   //display OFF,select on/off mode.Green Enhance mode disable
  
  U8X8_END_TRANSFER(),             	/* disable chip */
  U8X8_END()             		/* end of sequence */
};

/* cg160160 display */
uint8_t u8x8_d_uc1698_160x160(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  /* call common procedure first and handle messages there */
  if ( u8x8_d_uc1698u_common(u8x8, msg, arg_int, arg_ptr) == 0 )
  {
    /* msg not handled, then try here */
    switch(msg)
    {
      case U8X8_MSG_DISPLAY_SETUP_MEMORY:
	u8x8_d_helper_display_setup_memory(u8x8, &u8x8_uc1698u_cg160160_display_info);
	break;
      case U8X8_MSG_DISPLAY_INIT:
	u8x8_d_helper_display_init(u8x8);
	u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_cg160160_init_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
	if ( arg_int == 0 )
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_powersave0_seq);
	else
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_powersave1_seq);
	break;
      case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
	if ( arg_int == 0 )
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_flip0_seq);
	  u8x8->x_offset = u8x8->display_info->default_x_offset;
	}
	else
	{
	  u8x8_cad_SendSequence(u8x8, u8x8_d_uc1698u_flip1_seq);
	  u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
	}	
	break;
      default:
	return 0;		/* msg unknown */
    }
  }
  return 1;
}