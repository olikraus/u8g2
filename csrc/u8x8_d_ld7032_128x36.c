/*
    Support Futaba ELW0901AA
    u8x8_d_ld7032_128x36.c

    Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

    Copyright (c) 2023, olikraus@gmail.com
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

static const uint8_t u8x8_d_ld7032_128x36_powersave0_seq[] = 
{
    U8X8_START_TRANSFER(),          /* enable chip, delay is part of the transfer start */
    U8X8_CA(0x02, 0x01),		    /* ld7032: display on */
    U8X8_END_TRANSFER(),            /* disable chip */
    U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ld7032_128x36_powersave1_seq[] = 
{
    U8X8_START_TRANSFER(),          /* enable chip, delay is part of the transfer start */
    U8X8_CA(0x02, 0x00),		    /* ld7032: display off */
    U8X8_END_TRANSFER(),            /* disable chip */
    U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ld7032_128x36_flip0_seq[] = 
{
    U8X8_START_TRANSFER(),          /* enable chip, delay is part of the transfer start */
    U8X8_CA(0x09, 0x00),		    /* Display Direction:  reset default (x,y: min --> max) */  
    U8X8_END_TRANSFER(),            /* disable chip */
    U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ld7032_128x36_flip1_seq[] = 
{
    U8X8_START_TRANSFER(),          /* enable chip, delay is part of the transfer start */
    U8X8_CA(0x09, 0x02),		    /* Display Direction:  reset default (x: min --> max, y: max --> min) */  
    U8X8_END_TRANSFER(),            /* disable chip */
    U8X8_END()             			/* end of sequence */
};

static const uint8_t u8x8_d_ld7032_128x36_init_seq[] = 
{
    U8X8_START_TRANSFER(),             	/* enable chip, delay is part of the transfer start */

    U8X8_CA(0x14, 0x00),		  		// release standby mode
    U8X8_CA(0x02, 0x00),		  		// set display off
    U8X8_CA(0x1A, 0x05),		  		// set frame frequency
    U8X8_CA(0x1D, 0x00),		  	    // set data write direction (do not use 0x1b)
    U8X8_CA(0x09, 0x00),	      		// set scan direction
    U8X8_CAA(0x30, 0x00, 0x7F),         // set column driver active range 
    U8X8_CAA(0x32, 0x04, 0x27),         // set row driver active range
    U8X8_CA(0x34, 0x00),				// set start line
    U8X8_CA(0x35, 0x0F),		        // set end line
    U8X8_CA(0x36, 0x04),	            // set row start line
    U8X8_CA(0x37, 0x27),			    // set row end line
    U8X8_CA(0x10, 0x1F),		  	    // set peak pulse width
    U8X8_CA(0x16, 0x05),		        // set peak pulse delay width  	
    U8X8_CA(0x18, 0x1F),		  		// set pre-charge width
    U8X8_CA(0x13, 0x01),                // set row scan sequence
    U8X8_CA(0x12, 0x7F),		  	    // set contrast control
    U8X8_CA(0x3D, 0x00),                // set vdd selection 0x00 = 3.3V
    // U8X8_CA(0x3D, 0x01),                // set vdd selection 0x01 = 1.8V

    U8X8_END_TRANSFER(),             	/* disable chip */
    U8X8_END()             			    /* end of sequence */
};

static const u8x8_display_info_t u8x8_ld7032_128x36_display_info =
{
    /* chip_enable_level = */ 0,
    /* chip_disable_level = */ 1,
    
    /* post_chip_enable_wait_ns = */ 60,
    /* pre_chip_disable_wait_ns = */ 100,
    /* reset_pulse_width_ms = */ 100, 	
    /* post_reset_wait_ms = */ 100, 
    /* sda_setup_time_ns = */ 60,		/* 20ns, but cycle time is 60ns, so use 60/2 */
    /* sck_pulse_width_ns = */ 60,	/* 20ns, but cycle time is 60ns, so use 60/2  */
    /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
    /* spi_mode = */ 0,		/* active high, rising edge */
    /* i2c_bus_clock_100kHz = */ 4,
    /* data_setup_time_ns = */ 20,
    /* write_pulse_width_ns = */ 40,	
    /* tile_width = */ 16,
    /* tile_height = */ 5,
    /* default_x_offset = */ 0,
    /* flipmode_x_offset = */ 0,
    /* pixel_width = */ 128,
    /* pixel_height = */ 36
};

static uint8_t u8x8_d_ld7032_128_generic(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t x, c;
    uint8_t *ptr;
    switch(msg)
    {
        // handled by the calling function
        case U8X8_MSG_DISPLAY_SETUP_MEMORY:
            u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ld7032_128x36_display_info);
            break;
        case U8X8_MSG_DISPLAY_INIT:
            u8x8_d_helper_display_init(u8x8);
            u8x8_cad_SendSequence(u8x8, u8x8_d_ld7032_128x36_init_seq);    
            break;
        case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
            if ( arg_int == 0 )
                u8x8_cad_SendSequence(u8x8, u8x8_d_ld7032_128x36_powersave0_seq);
            else
                u8x8_cad_SendSequence(u8x8, u8x8_d_ld7032_128x36_powersave1_seq);
            break;
        case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
            if ( arg_int == 0 )
            {
                u8x8_cad_SendSequence(u8x8, u8x8_d_ld7032_128x36_flip0_seq);
                u8x8->x_offset = u8x8->display_info->default_x_offset;
            }
            else
            {
                u8x8_cad_SendSequence(u8x8, u8x8_d_ld7032_128x36_flip1_seq);
                u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
            }
            break;
    #ifdef U8X8_WITH_SET_CONTRAST
        case U8X8_MSG_DISPLAY_SET_CONTRAST:
            u8x8_cad_StartTransfer(u8x8);
            u8x8_cad_SendCmd(u8x8, 0x12);
            u8x8_cad_SendArg(u8x8, arg_int);
            u8x8_cad_EndTransfer(u8x8);
            break;
    #endif
        case U8X8_MSG_DISPLAY_DRAW_TILE:
            u8x8_cad_StartTransfer(u8x8);
            x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
            x += u8x8->x_offset/8;
            u8x8_cad_SendCmd(u8x8, 0x34);
            u8x8_cad_SendArg(u8x8, x);
            u8x8_cad_SendCmd(u8x8, 0x35);
            u8x8_cad_SendArg(u8x8, 0x0f);
            u8x8_cad_SendCmd(u8x8, 0x36);
            u8x8_cad_SendArg(u8x8, (((u8x8_tile_t *)arg_ptr)->y_pos)*8 + 4);
            u8x8_cad_SendCmd(u8x8, 0x37);
            u8x8_cad_SendArg(u8x8, 0x27);
            u8x8_cad_SendCmd(u8x8, 0x08);
            do
            {
                c = ((u8x8_tile_t *)arg_ptr)->cnt;
                ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
                u8x8_cad_SendData(u8x8, c*8, ptr); 	/* note: SendData can not handle more than 255 bytes */
                arg_int--;
            } 
            while( arg_int > 0 );
        
            u8x8_cad_EndTransfer(u8x8);
            break;
    default:
        return 0;
    }
    return 1;
}

uint8_t u8x8_d_ld7032_128x36(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    if ( msg == U8X8_MSG_DISPLAY_SETUP_MEMORY )
    {
        u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ld7032_128x36_display_info);
        return 1;
    }

    if ( msg ==U8X8_MSG_DISPLAY_INIT )
    {
        u8x8_d_helper_display_init(u8x8);
        u8x8_cad_SendSequence(u8x8, u8x8_d_ld7032_128x36_init_seq);    
        return 1;
    }

    return u8x8_d_ld7032_128_generic(u8x8, msg, arg_int, arg_ptr);
}
