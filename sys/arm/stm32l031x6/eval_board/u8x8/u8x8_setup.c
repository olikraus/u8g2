/*

  u8x8_setup.c

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

*/


#include "u8x8.h"

uint8_t u8x8_dummy_cb(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  /* the dummy callback will not handle any message and will fail for all messages */
  return 0;
}


/*
  Description:
    Setup u8x8
  Args:
    u8x8	An empty u8x8 structure
*/
void u8x8_SetupDefaults(u8x8_t *u8x8)
{
    u8x8->display_info = NULL;
    u8x8->display_cb = u8x8_dummy_cb;
    u8x8->cad_cb = u8x8_dummy_cb;
    u8x8->byte_cb = u8x8_dummy_cb;
    u8x8->gpio_and_delay_cb = u8x8_dummy_cb;
    u8x8->is_font_inverse_mode = 0;
    u8x8->device_address = 0;
    u8x8->utf8_state = 0;		/* also reset by u8x8_utf8_init */
    u8x8->i2c_address = 255;
    u8x8->debounce_default_pin_state = 255;	/* assume all low active buttons */
  
#ifdef U8X8_USE_PINS 
  {
    uint8_t i;
    for( i = 0; i < U8X8_PIN_CNT; i++ )
      u8x8->pins[i] = U8X8_PIN_NONE;
  }
#endif
}


/*
  Description:
    Setup u8x8 and assign the callback function. The dummy 
    callback "u8x8_dummy_cb" can be used, if no callback is required.
    This setup will not communicate with the display itself.
    Use u8x8_InitDisplay() to send the startup code to the Display.
  Args:
    u8x8				An empty u8x8 structure
    display_cb			Display/controller specific callback function
    cad_cb				Display controller specific communication callback function
    byte_cb			Display controller/communication specific callback funtion
    gpio_and_delay_cb	Environment specific callback function

*/
void u8x8_Setup(u8x8_t *u8x8, u8x8_msg_cb display_cb, u8x8_msg_cb cad_cb, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  /* setup defaults and reset pins to U8X8_PIN_NONE */
  u8x8_SetupDefaults(u8x8);

  /* setup specific callbacks */
  u8x8->display_cb = display_cb;
  u8x8->cad_cb = cad_cb;
  u8x8->byte_cb = byte_cb;
  u8x8->gpio_and_delay_cb = gpio_and_delay_cb;

  /* setup display info */
  u8x8_SetupMemory(u8x8);
}

