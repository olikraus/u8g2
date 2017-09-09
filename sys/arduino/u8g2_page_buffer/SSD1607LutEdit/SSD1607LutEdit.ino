/*

  SSD1607LutEdit.ino

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

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif



// display for the LUT editor
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2_editor(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// target e-paper device
#define BUSY_PIN 0
U8G2_SSD1607_200X200_1_4W_SW_SPI u8g2_epaper(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// eInk/ePaper Display
//U8G2_SSD1607_V2_200X200_1_4W_SW_SPI u8g2_epaper(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// eInk/ePaper Display

//#define BUSY_PIN 2
//U8G2_IL3820_296X128_1_4W_SW_SPI u8g2_epaper(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// WaveShare 2.9 inch eInk/ePaper Display, enable 16 bit mode for this display!
//U8G2_IL3820_V2_296X128_1_4W_SW_SPI u8g2_epaper(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);	// WaveShare 2.9 inch eInk/ePaper Display, enable 16 bit mode for this display!

/*================================================*/
/* lut editor */

uint16_t lut_measured_duration = 0;

#define LUT_ARRAY_LEN 20
#define LUT_WAVE_CNT 4

/*
  level
    0:		neutral, middle
    1:		upper line
    2:		lower line
    
    00   	black to black (upper line required)
    01	black to white (lower line required)
    10	white to black
    11	white to white
    
*/

uint8_t lut_level[LUT_WAVE_CNT][LUT_ARRAY_LEN];
uint8_t lut_time[LUT_ARRAY_LEN];
#define LUT_DY 3

u8g2_uint_t cursor_x1, cursor_x2;

uint8_t lut_time_to_width[16] = 
{
  2,	// 0
  2,	// 1
  2,	// 2
  2,	// 3
  3,	// 4
  3,	// 5
  3,	// 6
  3,	// 7
  4,	// 8
  4,	// 9
  4,	// 10
  4,	// 11
  5,	// 12
  5,	// 13
  5,	// 14
  5,	// 15
};

u8g2_uint_t get_dx(uint8_t i)
{
  //return 5;
  return lut_time_to_width[lut_time[i]];
}

void read_lut(const uint8_t *lut)
{
  uint8_t i;
  for ( i = 0; i < LUT_ARRAY_LEN; i++ )
  {
    lut_level[0][i] = lut[i] & 3; 
    lut_level[1][i] = (lut[i]>>2) & 3; 
    lut_level[2][i] = (lut[i]>>4) & 3; 
    lut_level[3][i] = (lut[i]>>6) & 3; 
    
  }
  for ( i = 0; i < LUT_ARRAY_LEN/2; i++ )
  {
    lut_time[i*2 + 0] = lut[LUT_ARRAY_LEN+i] & 15;
    lut_time[i*2 + 1] = (lut[LUT_ARRAY_LEN+i]>>4) & 15;
  }
}

void draw_lut_wave(u8g2_t *u8g2, uint8_t w, uint8_t cx, uint8_t is_cursor, u8g2_uint_t y)
{
  uint8_t i;
  u8g2_uint_t x1, y1;
  u8g2_uint_t x2, y2;
  x1 = 18;
  y1 = y;
  u8g2_DrawGlyph(u8g2, 2,y+2, (w&2)?'1':'0');
  u8g2_DrawGlyph(u8g2, 8,y+2, (w&1)?'1':'0');
  for( i = 0; i < LUT_ARRAY_LEN; i++ )
  {
    x2 = x1 + get_dx(i);
    y2 = y;
    if ( lut_level[w][i] == 1)
      y2 -= LUT_DY-1;
    else if ( lut_level[w][i] == 2 )
      y2 += LUT_DY-1;
    if ( y1 != y2 )
      u8g2_DrawLine(u8g2, x1, y1, x1, y2);
    u8g2_DrawHLine(u8g2, x1, y2, x2-x1+1);
    
    if ( i == cx )
    {
      cursor_x1 = x1; 
      cursor_x2 = x2;
      if ( is_cursor )
      {
	u8g2_DrawHLine(u8g2, x1, y-LUT_DY-1, x2-x1+1);
	u8g2_DrawHLine(u8g2, x1, y+LUT_DY+1, x2-x1+1);
      }
    }
    
    x1 = x2;
    y1 = y2;
  }
}

void draw_all_lut(u8g2_t *u8g2, uint8_t cx, uint8_t cy)
{
  uint8_t i;
  uint16_t total;
  int16_t area;
  u8g2_uint_t x, y;
  u8g2_uint_t xs = 1;
  y = LUT_DY +1;
  for( i = 0; i < LUT_WAVE_CNT; i++ )
  {
    if ( i == cy )
      draw_lut_wave(u8g2, i, cx, 1, y);
    else
      draw_lut_wave(u8g2, i, cx, 0, y);		// 255: no cursor
      
    y += (LUT_DY+2)*2;
  }

  total = 0;
  area = 0;
  if ( cy == LUT_WAVE_CNT )
    for( i = 0; i < LUT_ARRAY_LEN; i++ )
      total += lut_time[i];

  if ( cy < LUT_WAVE_CNT )
  {
    for( i = 0; i < LUT_ARRAY_LEN; i++ )
    {
      if ( lut_level[cy][i] == 1 )
	area += lut_time[i];
      else if ( lut_level[cy][i] == 2 )
	area -= lut_time[i];
    }
  }

  y+=1;
  if ( cy == LUT_WAVE_CNT )
  {
    u8g2_SetDrawColor(u8g2, 2);
    u8g2_DrawBox(u8g2, cursor_x1, 0, cursor_x2-cursor_x1+1, (LUT_DY+2)*2*4-1 );
  
    u8g2_SetDrawColor(u8g2, 0);
  }
  else
  {
    u8g2_SetDrawColor(u8g2, 1);
  }
  x = u8g2_DrawStr(u8g2, xs, y, " TP[");
  x += u8g2_DrawStr(u8g2, x, y, u8x8_u8toa( cx, 2));
  x += u8g2_DrawStr(u8g2, x, y, "]=");
  x += u8g2_DrawStr(u8g2, x, y, u8x8_u8toa( lut_time[cx], 2));
  x += u8g2_DrawStr(u8g2, x, y, " ");

  u8g2_SetDrawColor(u8g2, 1);
  x = 70;
  
  if ( cy == LUT_WAVE_CNT+1 )
  {
    if ( lut_measured_duration > 0 )
    {
	x = 60;
	x += u8g2_DrawStr(u8g2, x, y, "measure=");
	x += u8g2_DrawStr(u8g2, x, y, u8x8_u16toa( lut_measured_duration, 4));
    }
  }
  if ( cy == LUT_WAVE_CNT )
  {
    x += u8g2_DrawStr(u8g2, x, y, " total=");
    x += u8g2_DrawStr(u8g2, x, y, u8x8_u16toa( total, 3));
    x += u8g2_DrawStr(u8g2, x, y, " ");
  } 
  else if ( cy < LUT_WAVE_CNT )
  {
    x += u8g2_DrawStr(u8g2, x, y, " area=");
    if ( area >= 0 )
      x += u8g2_DrawStr(u8g2, x, y, u8x8_u16toa( area, 3));
    else
    {
      x += u8g2_DrawStr(u8g2, x, y, "-");
      x += u8g2_DrawStr(u8g2, x, y, u8x8_u16toa( -area, 3));
    }
    x += u8g2_DrawStr(u8g2, x, y, " ");
  }
  
  y+=8;
  x = u8g2_DrawStr(u8g2, xs, y, "Test: ");

  if ( cy == LUT_WAVE_CNT+1 && (cx & 1) == 0 )
  {
    u8g2_SetDrawColor(u8g2, 0);
  }
  else
  {
    u8g2_SetDrawColor(u8g2, 1);
  }
  x += u8g2_DrawStr(u8g2, x, y, " New ");
  
  if ( cy == LUT_WAVE_CNT+1 && (cx & 1) != 0 )
  {
    u8g2_SetDrawColor(u8g2, 0);
  }
  else
  {
    u8g2_SetDrawColor(u8g2, 1);
  }
  x += u8g2_DrawStr(u8g2, x, y, " Old ");
  
  u8g2_SetDrawColor(u8g2, 1);
  //if ( lut_measured_duration > 0 )
  //{
  //    x = 60;
  //    x += u8g2_DrawStr(u8g2, x, y, "measure=");
  //    x += u8g2_DrawStr(u8g2, x, y, u8x8_u16toa( lut_measured_duration, 4));
  //}
  
  y+=8;
  if ( cy == LUT_WAVE_CNT+2 )
  {
    u8g2_SetDrawColor(u8g2, 0);
  }
  else
  {
    u8g2_SetDrawColor(u8g2, 1);
  }
  x = u8g2_DrawStr(u8g2, xs, y, " Show Binary LUT ");
  u8g2_SetDrawColor(u8g2, 1);

  //extern uint16_t refresh_cnt;
  //u8g2_DrawStr(u8g2, 115, y, u8x8_u16toa( refresh_cnt, 2));  
}

void init_lut(void)
{
  uint8_t i, j;
  for( i = 0; i < LUT_ARRAY_LEN; i++ )
  {
    for( j = 0; j < LUT_WAVE_CNT; j++ )
      lut_level[j][i] = 0;
    lut_time[i] = 0;
  }
  
}

uint8_t edit_lut(u8g2_t *u8g2)
{
  uint8_t event;
  uint8_t cx, cy;

  u8g2_SetFont(u8g2, u8g2_font_5x7_mr);
  u8g2_SetFontMode(u8g2, 0);
  u8g2_SetFontDirection(u8g2, 0);

  cx = 0;
  cy = 0;

  for(;;)
  {
    u8g2_FirstPage(u8g2);
    do
    {
      draw_all_lut(u8g2, cx, cy);
    } while( u8g2_NextPage(u8g2) );

    for(;;)
    {
      event = u8x8_GetMenuEvent(u8g2_GetU8x8(u8g2));
      if ( event == U8X8_MSG_GPIO_MENU_SELECT )
      {
	if ( cx < LUT_ARRAY_LEN && cy < LUT_WAVE_CNT )
	{
	  lut_level[cy][cx]++;
	  if ( lut_level[cy][cx] >= 3 )
	    lut_level[cy][cx]  = 0;
	}
	else if ( cy == LUT_WAVE_CNT )
	  {
	    lut_time[cx]++;
	    if ( lut_time[cx] >= 16 )
	      lut_time[cx]  = 0;
	  }	  
	else if ( cy == LUT_WAVE_CNT+1 )
	{
	  if ( (cx & 1) == 0 )
	    return 1;	/* new waveform */
	  return 2; /* old/buildin refresh waveform */
	}
	else if ( cy == LUT_WAVE_CNT+2 )
	{
	  return 10; /* show lut */
	}
	
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_HOME )
      {
	if ( cy == LUT_WAVE_CNT )
	{
	  if ( lut_time[cx] == 0 )
	    lut_time[cx]  = 16;
	  lut_time[cx]--;
	}
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_NEXT )
      {
	cx++;
	if ( cx >= LUT_ARRAY_LEN )
	  cx = 0;
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_DOWN )
      {
	cy++;
	if ( cy >= LUT_WAVE_CNT+3 )
	  cy = 0;
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_PREV )
      {
	if ( cx == 0 )
	  cx = LUT_ARRAY_LEN;
	cx--;
	break;
      }
      else if ( event == U8X8_MSG_GPIO_MENU_UP )
      {
	if ( cy == 0 )
	  cy = LUT_WAVE_CNT+3;
	cy--;
	break;
      }
    }
  }
  return 0;
}


/* send lut to the target eink device (given by u8x8_t *) */
void write_to_lut_register(u8x8_t *epaper)
{
  uint8_t i;
  uint8_t b;
  u8x8_cad_StartTransfer(epaper);
  
  u8x8_cad_SendCmd(epaper, 0x032);		


  for ( i = 0; i < LUT_ARRAY_LEN; i++ )
  {
    b = lut_level[0][i]; 
    b |= lut_level[1][i]<<2;
    b |= lut_level[2][i]<<4;
    b |= lut_level[3][i]<<6;
    u8x8_cad_SendArg(epaper, b);    
  }
  for ( i = 0; i < LUT_ARRAY_LEN/2; i++ )
  {
    b = lut_time[i*2 + 0];
    b |= lut_time[i*2 + 1] << 4;
    u8x8_cad_SendArg(epaper, b);    
  }

  u8x8_cad_EndTransfer(epaper);

}

/*================================================*/

/*================================================*/
/* epaper test */

uint8_t is_enable_hook;
uint8_t (*u8x8_d_original)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


/* the purpose of this hook function is to replace the REFRESH code by our own function */
uint8_t u8x8_d_test_hook(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  volatile uint32_t start, curr, end;
  volatile uint16_t v;
  if ( is_enable_hook )
  {
    if ( msg == U8X8_MSG_DISPLAY_REFRESH )
    {
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x022);		// program update sequence
      u8x8_cad_SendArg(u8x8, 0x004);		// define sequence
	// 0x0c4: clk -> CP -> LUT -> pattern display
      u8x8_cad_SendCmd(u8x8, 0x020);		// execute
      start =millis();
      end = start + 4000;
      //pinMode(A2, INPUT);
      delay(10);
      for(;;)
      {
	v = analogRead(BUSY_PIN);
	curr = millis();
	if ( v < 300 )
	  break;
	if ( curr > end )
	  break;
      }
      lut_measured_duration = curr-start;
      u8x8_cad_EndTransfer(u8x8);
      return 1;
    }
  }
  return u8x8_d_original(u8x8, msg, arg_int, arg_ptr);
}


void test_lut(u8g2_t *u8g2)
{

    u8g2_epaper.setPowerSave(0);

    u8g2_FirstPage(u8g2);
    do
    {
      u8g2_DrawStr(u8g2, 0, 20, "Test LUT, Page 1");
    } while( u8g2_NextPage(u8g2) );

    u8g2_epaper.firstPage();
    do {
    u8g2_epaper.setFont(u8g2_font_ncenB14_tr);
    u8g2_epaper.drawStr(0,20,"11111111111111111111111111");
    u8g2_epaper.drawStr(0,40,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    u8g2_epaper.drawBox(10, 60, 60, 30);
    u8g2_epaper.drawFrame(50, 70, 60, 40);
    
    u8g2_epaper.drawBox(110+10, 60, 60, 30);
    
    } while ( u8g2_epaper.nextPage() );

    delay(3000);

    u8g2_FirstPage(u8g2);
    do
    {
      u8g2_DrawStr(u8g2, 0, 20, "Test LUT, Page 2");
      u8g2_DrawStr(u8g2, 0, 40, u8x8_u16toa( lut_measured_duration, 4));
      
    } while( u8g2_NextPage(u8g2) );

    u8g2_epaper.firstPage();
    do {
    u8g2_epaper.setFont(u8g2_font_ncenB14_tr);
    u8g2_epaper.drawStr(0,20,"22222222222222222222222222");
    u8g2_epaper.drawStr(0,40,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    u8g2_epaper.drawFrame(10, 60, 60, 30);
    u8g2_epaper.drawBox(50, 70, 60, 40);
    
    u8g2_epaper.drawBox(150+10, 70, 60, 40);
    } while ( u8g2_epaper.nextPage() );

    delay(3000);

    u8g2_FirstPage(u8g2);
    do
    {
      u8g2_DrawStr(u8g2, 0, 20, "Test LUT, Page 3");
      u8g2_DrawStr(u8g2, 0, 40, u8x8_u16toa( lut_measured_duration, 4));
      
    } while( u8g2_NextPage(u8g2) );

    u8g2_epaper.firstPage();
    do {
    u8g2_epaper.setFont(u8g2_font_ncenB14_tr);
    u8g2_epaper.drawStr(0,20,"33333333333333333333333333");
    u8g2_epaper.drawStr(0,40,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    
    u8g2_epaper.drawBox(10, 60, 60, 30);
    u8g2_epaper.drawFrame(50, 70, 60, 40);
    
    u8g2_epaper.drawBox(110+10, 60, 60, 30);
    } while ( u8g2_epaper.nextPage() );

    u8g2_epaper.setPowerSave(1);

    delay(3000);

}


/*================================================*/
/* send lut to the target eink device (given by u8x8_t *) */
void show_lut_on_editor(void)
{
  uint8_t i;
  uint8_t b;
  u8g2_uint_t x, y;


  x = 0;
  y = 0;
  for ( i = 0; i < LUT_ARRAY_LEN; i++ )
  {
    b = lut_level[0][i]; 
    b |= lut_level[1][i]<<2;
    b |= lut_level[2][i]<<4;
    b |= lut_level[3][i]<<6;
    u8g2_editor.setCursor(x*20, y*8+8);    
    u8g2_editor.print(b, HEX);
    x++;
    if ( x >= 5 )
    {
      x = 0;
      y++;
    }
  }
  
  x = 0;
  y++;
  
  for ( i = 0; i < LUT_ARRAY_LEN/2; i++ )
  {
    b = lut_time[i*2 + 0];
    b |= lut_time[i*2 + 1] << 4;
    u8g2_editor.setCursor(x*20, y*8+8);    
    u8g2_editor.print(b, HEX);
    x++;
    if ( x >= 5 )
    {
      x = 0;
      y++;
    }
  }


}

/*================================================*/
/* arduino setup & loop */

const uint8_t LUTDefault_full[31] =
{
0x50, 0xAA, 0x55, 0xAA, 0x11, 
0x11, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0xFF, 0xFF, 0x3F, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00
};

// same as LUTDefault_full, but dubled size
const uint8_t w01[30] =
{
0x00, 0x00, 0x44, 0xaa, 0xaa, 
0x55, 0x55, 0xaa, 0xaa, 0x11, 
0x11, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x01, 0xff, 0xff, 0xff, 0xff, 
0x0f, 0x00, 0x00, 0x00, 0x00
};

// same as w01, but timing adjisted, should be identical to LUTDefault_full
const uint8_t w02[30] =
{
0x00, 0x00, 0x44, 0xaa, 0xaa, 
0x55, 0x55, 0xaa, 0xaa, 0x11, 
0x11, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x11, 0x5a, 0x5a, 0x5a, 0x5a, 
0x0a, 0x00, 0x00, 0x00, 0x00
};

// speed optimized, lesser flickering version
// ticks=71
// measued=1441ms
const uint8_t w03[30] =
{
0x00, 0x40, 0x04, 0xa6, 0xa8, 
0x65, 0x19, 0xaa, 0x98, 0x11, 
0x11, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x20, 0x26, 0x26, 0x26, 0xff, 
0x0f, 0x00, 0x00, 0x00, 0x00
};


// speed optimized, lesser flickering version
// ticks=71
// measued=1441ms
const uint8_t w04[30] =
{
0x00, 0x88, 0x88, 0x88, 0x98, 
0x99, 0x99, 0x99, 0x11, 0x11, 
0x11, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x77, 0x77, 0x77, 0x77, 0x77, 
0x00, 0x00, 0x00, 0x00, 0x00
};

// speed optimized, no (?) flickering version
const uint8_t w05[30] =
{
0xaa, 0x55, 0x55, 0x55, 0x99, 
0x99, 0x99, 0x99, 0x99, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x77, 0x77, 0x77, 0x77, 0x07, 
0x00, 0x00, 0x00, 0x00, 0x00
};

/*
  Duration: 1240 ms
  Initial white pulse:
    - Required for white-white so that white stays white (pulse can be as low as 2 ticks)
    - On black-black to ensure long term stbility (pulse should be 4 or higher for 
      long term stability)
    - Pulse should be short, to avoid flicker
    - Long enough for the black-black transition
  White pixel (black-white transition)
    - Starts at the beginning. Looks like a black pulse is not required.
    - White low level must stop before the black pixel are finished
  Black pixel (black-black and white-black)
    - Black (high level) is extended. If the black level is applied longer, the contrast
      gets better.
    - There must be no white pixel write otherwise contrast will be bad: Writing
      black is put at the end and starts in the middle
    
*/

/*
  U8X8_A(0x02),
  U8X8_A(0x02),
  U8X8_A(0x01),
  U8X8_A(0x11),
  U8X8_A(0x12),
  U8X8_A(0x12),
  U8X8_A(0x22),
  U8X8_A(0x22),
  U8X8_A(0x66),
  U8X8_A(0x69),
  U8X8_A(0x69),
  U8X8_A(0x59),
  U8X8_A(0x58),
  U8X8_A(0x99),
  U8X8_A(0x99),
  U8X8_A(0x88),
  U8X8_A(0x00),
  U8X8_A(0x00),
  U8X8_A(0x00),
  U8X8_A(0x00),

  U8X8_A(0xF8),
  U8X8_A(0xB4),
  U8X8_A(0x13),
  U8X8_A(0x51),
  U8X8_A(0x35),
  U8X8_A(0x51),
  U8X8_A(0x51),
  U8X8_A(0x19),
  U8X8_A(0x01),
  U8X8_A(0x00),
*/

const uint8_t w06[30] =
{
0x02, 0x02, 0x01, 0x11, 0x12, 
0x12, 0x12, 0x22, 0x22, 0x66, 
0x69, 0x59, 0x58, 0x99, 0x99, 
0x88, 0x00, 0x00, 0x00, 0x00, 

0xf8, 0xb4, 0x13, 0x51, 0x35, 
0x51, 0x51, 0xe9, 0x04, 0x00
};

void setup(void) 
{

  u8g2_editor.begin(/* menu_select_pin= */ 2, /* menu_next_pin= */ 4, /* menu_prev_pin= */ 7, /* menu_up_pin= */ 6, /* menu_down_pin= */ 5, /* menu_home_pin= */ 3);
  u8g2_editor.setFont(u8g2_font_5x7_mr);
  u8g2_editor.setFontMode(0);
  u8g2_editor.setFontDirection(0);
  
  
  u8g2_epaper.begin();
  u8g2_epaper.setPowerSave(0);
  
  u8x8_d_original = u8g2_epaper.getU8x8()->display_cb;		// get the original display function and..
  u8g2_epaper.getU8x8()->display_cb = u8x8_d_test_hook;	// ...replace it with our hook function

  init_lut();
  //read_lut(LUTDefault_full);
  read_lut(w06);
}



void loop(void) 
{
  uint8_t cmd;
  
  cmd = edit_lut(u8g2_editor.getU8g2());
  
  switch( cmd )
  {
    case 1:	/* test new lut */
      is_enable_hook = 1;
      u8g2_epaper.setPowerSave(0);
      write_to_lut_register(u8g2_epaper.getU8x8());
      test_lut(u8g2_editor.getU8g2());
      break;
    case 2:	/* test old, existing lut */
      is_enable_hook = 0;
      u8g2_epaper.initDisplay();
      u8g2_epaper.setPowerSave(0);
      test_lut(u8g2_editor.getU8g2());
      break;
    case 10:
      u8g2_editor.firstPage();
      do {
	show_lut_on_editor();
      } while ( u8g2_editor.nextPage() );
      while( u8g2_editor.getMenuEvent() == 0 )
	;

  }
}

