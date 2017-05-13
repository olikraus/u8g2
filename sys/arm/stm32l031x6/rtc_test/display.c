/*

  display.c 
  
  LPC11U3x GPS Logger (https://github.com/olikraus/lpc11u3x-gps-logger)

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

#include "display.h"
#include "term.h"
#include "u8x8.h"


/*=======================================================================*/
/* u8x8cb.c */
uint8_t u8x8_gpio_and_delay_stm32l0(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/*=======================================================================*/

#define WIDTH 16
#define HEIGHT 8

/*=======================================================================*/

term_t display_term;
u8x8_t display_u8x8;
char display_buffer[WIDTH*HEIGHT];

/*=======================================================================*/

static void display_write_screen(term_t *term)
{
  uint8_t x, y;
  char *ptr = term->screen;
  for( y = 0; y < term->height; y++ )
  {
    for( x = 0; x < term->width; x++ )
    {
      u8x8_DrawGlyph(&display_u8x8, x, y, *ptr);
      ptr++;
    }
  }
}

void display_show_cb(term_t *term, uint8_t msg)
{
  switch(msg)
  {
    case TERM_MSG_POST_NEWLINE:
    //case TERM_MSG_POST_SCROLL:
    //case TERM_MSG_POST_CHAR:
      display_write_screen(term);
      break;
    
  }
}

void display_Init(void)
{
  u8x8_Setup(&display_u8x8, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_stm32l0);

  //u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_lpc11u3x);
  u8x8_InitDisplay(&display_u8x8);
  u8x8_ClearDisplay(&display_u8x8);
  u8x8_SetPowerSave(&display_u8x8, 0);
  u8x8_SetFont(&display_u8x8, u8x8_font_amstrad_cpc_extended_r);  
  
  term_Init(&display_term, WIDTH, HEIGHT, display_buffer, display_show_cb);
}

void display_Write(const char *str)
{
  term_WriteString(&display_term, str);
}



void display_WriteUnsigned(uint32_t v)
{
  char buf[12];
  char *p = buf+11;
  *p = '\0';
  p--;
  while (v >= 10) 
  {
    *p = (v % 10);
    v /= 10;
    *p += '0';
    p--;
  }
  *p = v + '0';
  display_Write(p);
}


