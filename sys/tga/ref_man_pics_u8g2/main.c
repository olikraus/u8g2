/*
  
  main.c
  
  Generate pictures for the u8g2 reference manual.
  
  
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "u8x8.h"
#include "u8g2.h"



extern uint8_t tga_is_transparent;

extern uint8_t tga_desc_fg_r;
extern uint8_t tga_desc_fg_g;
extern uint8_t tga_desc_fg_b;

extern uint8_t tga_desc_bg_r;
extern uint8_t tga_desc_bg_g;
extern uint8_t tga_desc_bg_b;

extern uint8_t tga_lcd_fg_r;
extern uint8_t tga_lcd_fg_g;
extern uint8_t tga_lcd_fg_b;

extern uint8_t tga_lcd_bg_r;
extern uint8_t tga_lcd_bg_g;
extern uint8_t tga_lcd_bg_b;

extern void tga_save_png(const char *name);

u8x8_t u8x8;
u8g2_t u8g2;
u8g2_t desc;

#define u8g2_logo_97x51_width 97
#define u8g2_logo_97x51_height 51
static unsigned char u8g2_logo_97x51_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x3c, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00,
   0x00, 0x00, 0x3c, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
   0x03, 0x00, 0x00, 0x3c, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0x03, 0x00, 0x00, 0x3c, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0xcf, 0x07, 0x00, 0x00, 0x3c, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x80, 0x83, 0x07, 0x00, 0x00, 0x3c, 0x80, 0x07, 0xf8, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x81, 0x07, 0x00, 0x00, 0x3c, 0x80, 0x07, 0xfc, 0x03,
   0x1c, 0x00, 0x3e, 0x1c, 0xc0, 0x03, 0x00, 0x00, 0x3c, 0x80, 0x07, 0xff,
   0x07, 0x7f, 0x80, 0xff, 0x3f, 0xe0, 0x01, 0x00, 0x00, 0x3c, 0x80, 0x07,
   0xff, 0x8f, 0xff, 0xc1, 0xff, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x3c, 0x80,
   0x87, 0xff, 0xdf, 0xff, 0xc1, 0xc3, 0x07, 0x7c, 0x00, 0x00, 0x00, 0x3c,
   0x80, 0x87, 0x0f, 0xfe, 0xff, 0xe3, 0x81, 0x03, 0x1e, 0x00, 0x00, 0x00,
   0x3c, 0x80, 0xc7, 0x07, 0xfc, 0xe3, 0xe3, 0x81, 0x07, 0x0f, 0x00, 0x00,
   0x00, 0x3c, 0x80, 0xc7, 0x07, 0xf8, 0xc1, 0xe7, 0x81, 0x87, 0xff, 0x07,
   0x00, 0x00, 0x3c, 0x80, 0xc7, 0x03, 0xf0, 0x80, 0xe7, 0xc3, 0x87, 0xff,
   0x07, 0x00, 0x00, 0x3c, 0x80, 0xc7, 0x03, 0x70, 0x80, 0xc7, 0xe7, 0x83,
   0xff, 0x07, 0x00, 0x00, 0x3c, 0x80, 0xc7, 0x03, 0x78, 0x80, 0xc7, 0xff,
   0x03, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x80, 0xc7, 0x03, 0xf8, 0xc0, 0x87,
   0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x80, 0xc7, 0x07, 0xfc, 0xc1,
   0xc7, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0xc0, 0x87, 0x0f, 0xfe,
   0xff, 0xe3, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xc0, 0x83, 0xff,
   0xdf, 0xff, 0xe3, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xf1, 0x03,
   0xff, 0x8f, 0xff, 0xe1, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff,
   0x01, 0xfe, 0x0f, 0xff, 0xc0, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0xff, 0x00, 0xfc, 0x03, 0x7c, 0xc0, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x3f, 0x00, 0xf8, 0x01, 0x00, 0xe0, 0x01, 0x1e, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x1e, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x1e, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xf0, 0xc7, 0x0f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xe0, 0xff,
   0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xc0,
   0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x24, 0x20, 0x00,
   0x00, 0x08, 0x46, 0x02, 0x00, 0x80, 0xc0, 0x40, 0x00, 0x0c, 0x6e, 0x6a,
   0xc0, 0xa4, 0x48, 0x04, 0xaa, 0xac, 0x8c, 0xaa, 0xac, 0x00, 0x6a, 0xa4,
   0xaa, 0x20, 0xea, 0xa4, 0x64, 0x66, 0xaa, 0x46, 0x4a, 0x8a, 0x00, 0x4c,
   0xa4, 0xaa, 0x20, 0xaa, 0xa2, 0x44, 0x2a, 0xaa, 0x28, 0xaa, 0x4c, 0x00,
   0xe8, 0xa8, 0x6c, 0xc4, 0xa4, 0x42, 0xee, 0x2a, 0xcc, 0x26, 0x6c, 0xe8,
   0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00 };


void ra(uint16_t tx, uint16_t ty, const char *s)
{
  int x,y, i, w;
  x = tx*3+64*3 + 0;
  y = ty*3+32*3 + 1;
  for( i = 0; i < 6; i++)
  {
    u8g2_DrawHVLine(&desc, x-2*i, y-i, 1+2*i, 1);
    u8g2_DrawHVLine(&desc, x-2*i-1, y-i, 1+2*i, 1);
  }
  u8g2_DrawHVLine(&desc, x-30, y-1, 28, 0);
  u8g2_DrawHVLine(&desc, x-30, y, 20, 0);
  u8g2_DrawHVLine(&desc, x-30, y+1, 28, 0);

  w = u8g2_GetStrWidth(&desc, s);
  u8g2_DrawStr(&desc, x-34-w, y+u8g2_GetAscent(&desc)/2, s);
  
  
}

/* vertical measure */
/* tx,ty: lower point */
void vm(uint16_t tx, uint16_t ty, uint16_t dy)
{
  char s[10];
  
  int x,y, i, w;
  x = tx*3+64*3 + 1;
  y = ty*3+32*3 +3;
  
  u8g2_DrawHVLine(&desc, x-3, y-1, 7, 0);
  u8g2_DrawHVLine(&desc, x, y-dy*3, dy*3, 1);
  u8g2_DrawHVLine(&desc, x-3, y-dy*3, 7, 0);

  sprintf(s, "%d", dy);
  w = u8g2_GetStrWidth(&desc, s);
  u8g2_DrawStr(&desc, x+6, y-(dy*3)/2+u8g2_GetAscent(&desc)/2, s);
}

/* horizontal measure */
/* tx,ty: left point */
void hm(uint16_t tx, uint16_t ty, uint16_t dx)
{
  char s[10];
  
  int x,y, i, w;
  x = tx*3+64*3 + 1;
  y = ty*3+32*3 +3;
  
  u8g2_DrawHVLine(&desc, x-1, y-3, 7, 1);
  u8g2_DrawHVLine(&desc, x, y, dx*3-1, 0);
  u8g2_DrawHVLine(&desc, x+(dx-1)*3+1, y-3, 7, 1);

  sprintf(s, "%d", dx);
  w = u8g2_GetStrWidth(&desc, s);
  u8g2_DrawStr(&desc, x+(dx*3)/2-w/2, y+6+u8g2_GetAscent(&desc), s);
}

int main(void)
{
  /*
  tga_desc_fg_r = 100;
  tga_desc_fg_g = 118;
  tga_desc_fg_b = 135;
  */

  tga_desc_fg_r = 76;
  tga_desc_fg_g = 0;
  tga_desc_fg_b = 205;

  tga_desc_bg_r = 255;
  tga_desc_bg_g = 255;
  tga_desc_bg_b = 255;

  tga_lcd_fg_r = 0;
  tga_lcd_fg_g = 0;
  tga_lcd_fg_b = 0;

  tga_lcd_bg_r = 250;
  tga_lcd_bg_g = 193;
  tga_lcd_bg_b = 50;

  /*
  u8x8_Setup_TGA_DESC(&desc);
  u8x8_InitDisplay(&desc);  
  u8x8_SetPowerSave(&desc, 0);
  u8x8_ClearDisplay(&desc);
  u8x8_SetFont(&desc, u8x8_font_amstrad_cpc_extended_r);
  u8x8_DrawString(&desc, 0, 0, "Description");
*/

  u8g2_SetupBuffer_TGA_DESC(&desc, &u8g2_cb_r0);
  u8g2_InitDisplay(&desc);  
  u8g2_SetPowerSave(&desc, 0);
  u8g2_ClearDisplay(&desc);


  u8g2_SetupBuffer_TGA_LCD(&u8g2, &u8g2_cb_r0);
  u8g2_InitDisplay(&u8g2);  
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_ClearDisplay(&u8g2);



  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tf);
    u8g2_DrawStr(&u8g2, 0, 15, "Hello World!");
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(0,15, "x=0, y=15");
  } while( u8g2_NextPage(&desc) );
  tga_is_transparent = 0;

  tga_save_png("u8g2_hello_world.png");

/*=========================================*/
/* u8g2_ascent_descent.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    u8g2_DrawStr(&u8g2, 5, 20, "A  gi");
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(5,20, "x=5, y=20");
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    vm(25,19, u8g2_GetAscent(&u8g2));
    
    /* descent usually is negative */
    vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  tga_is_transparent = 0;

  tga_save_png("u8g2_ascent_descent.png");

  /*=========================================*/
  /* u8g2_pos_center.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    u8g2_DrawStr(&u8g2, 5, 20, "Agi");
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(5,20, "x=5, y=20");
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    /* descent usually is negative */
    vm(48,19+u8g2.font_calc_vref(&u8g2)-u8g2_GetDescent(&u8g2), u8g2_GetAscent(&u8g2)-u8g2_GetDescent(&u8g2));
    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_pos_center.png");

/*=========================================*/

  u8x8_ClearDisplay(u8g2_GetU8x8(&desc));
  
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 5, 20, 0x2603);
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(5,20, "x=5, y=20");
  } while( u8g2_NextPage(&desc) );
  tga_is_transparent = 0;

  tga_save_png("u8g2_snowman_glyph.png");

/*=========================================*/

  u8x8_ClearDisplay(u8g2_GetU8x8(&desc));
  
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawUTF8(&u8g2, 5, 20, "Snowman: ☃");
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(5,20, "x=5, y=20");
  } while( u8g2_NextPage(&desc) );
  tga_is_transparent = 0;

  tga_save_png("u8g2_snowman_utf8.png");

/*=========================================*/

  u8x8_ClearDisplay(u8g2_GetU8x8(&desc));
  
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFontMode(&u8g2, 1);	/* transparent */
    
    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(&u8g2, 0, 30, "U");
    
    u8g2_SetFontDirection(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_inb30_mr);
    u8g2_DrawStr(&u8g2, 21, 8,"8");
        
    u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 51,30,"g");
    u8g2_DrawStr(&u8g2, 67,30,"\xb2");
    
    u8g2_DrawHLine(&u8g2, 2, 35, 47);
    u8g2_DrawHLine(&u8g2, 3, 36, 47);
    u8g2_DrawVLine(&u8g2, 45, 32, 12);
    u8g2_DrawVLine(&u8g2, 46, 33, 12);
    
    u8g2_SetFont(&u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(&u8g2, 1,54,"github.com/olikraus/u8g2");
    
  } while( u8g2_NextPage(&u8g2) );

  u8g2_SetFontDirection(&u8g2, 0);

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
  } while( u8g2_NextPage(&desc) );
  tga_is_transparent = 0;

  tga_save_png("u8g2_logo.png");

  /*=========================================*/
  /* u8g2_box.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    u8g2_DrawBox(&u8g2, 3, 7, 25, 15);
    //u8g2_DrawFrame(&u8g2, 60, 40, 16, 9);
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(3,7, "x=3, y=7");
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    /* descent usually is negative */
    vm(3+25+2,7+15-1, 15);
    hm(3, 7+15+1, 25);

    //ra(60,40, "x=60, y=40");
    //vm(60+16+2,40+9-1, 9);
    //hm(60, 40+9+1, 16);
    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_box.png");

  /*=========================================*/
  /* u8g2_frame.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    u8g2_DrawFrame(&u8g2, 3, 7, 25, 15);
    //u8g2_DrawFrame(&u8g2, 60, 40, 16, 9);
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(3,7, "x=3, y=7");
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    /* descent usually is negative */
    vm(3+25+2,7+15-1, 15);
    hm(3, 7+15+1, 25);

    //ra(60,40, "x=60, y=40");
    //vm(60+16+2,40+9-1, 9);
    //hm(60, 40+9+1, 16);
    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_frame.png");

  
  /*=========================================*/
  /* u8g2_c_frame.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);

  {
    char *s = "Cage";
    u8g2_uint_t w = u8g2_GetStrWidth(&u8g2, s);
    u8g2_uint_t h = u8g2_GetAscent(&u8g2)-u8g2_GetDescent(&u8g2);
    u8g2_uint_t x = 5;
    u8g2_uint_t y = 11;
    
    u8g2_SetFontPosBaseline(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawStr(&u8g2, x, y, s);
      u8g2_DrawFrame(&u8g2, x-1,y-u8g2_GetAscent(&u8g2)-1, w+2, h+2);
    } while( u8g2_NextPage(&u8g2) );

    tga_is_transparent = 1;
    u8g2_FirstPage(&desc);
    do
    {
      u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
      ra(x,y, "x=5, y=11");
      u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
      /* descent usually is negative */
      vm(x-1+w+2+2,y-9+h+2-1, h+2);
      hm(x-1, y-8+h+2+1+10, w+2);
      hm(x, y-8+h+2+1, w);

      //ra(60,40, "x=60, y=40");
      //vm(60+16+2,40+9-1, 9);
      //hm(60, 40+9+1, 16);
      
      //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
    } while( u8g2_NextPage(&desc) );

  }
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_c_frame.png");


  /*=========================================*/
  /* u8g2_button1.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);

  {
    char *s = "Btn";
    u8g2_uint_t w = u8g2_GetStrWidth(&u8g2, s);
    u8g2_uint_t h = u8g2_GetAscent(&u8g2)-u8g2_GetDescent(&u8g2);
    u8g2_uint_t x = 62;
    u8g2_uint_t y = 20;
    
    u8g2_SetFontPosBaseline(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawButtonUTF8(&u8g2, x, y, U8G2_BTN_BW2, 0,  2,  2, s );
    } while( u8g2_NextPage(&u8g2) );

    tga_is_transparent = 1;
    u8g2_FirstPage(&desc);
    do
    {
      u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
      ra(x,y, "x=62, y=20");
      u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
      /* descent usually is negative */
      vm(x-1+w+2+4,y-9+h+2-1+1, h+4);
      hm(x-2, y-8+h+2+3, w+4);
      //hm(x, y-8+h+2+3, w);

      //ra(60,40, "x=60, y=40");
      //vm(60+16+2,40+9-1, 9);
      //hm(60, 40+9+1, 16);
      
      //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
    } while( u8g2_NextPage(&desc) );

  }
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_button1.png");
  

  /*=========================================*/
  /* u8g2_button2.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);

  {
    char *s = "Btn";
    u8g2_uint_t w = u8g2_GetStrWidth(&u8g2, s);
    u8g2_uint_t h = u8g2_GetAscent(&u8g2)-u8g2_GetDescent(&u8g2);
    u8g2_uint_t x = 62;
    u8g2_uint_t y = 20;
    
    u8g2_SetFontPosBaseline(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawButtonUTF8(&u8g2, x, y, U8G2_BTN_HCENTER|U8G2_BTN_BW2, 34,  2,  2, s );
    } while( u8g2_NextPage(&u8g2) );

    tga_is_transparent = 1;
    u8g2_FirstPage(&desc);
    do
    {
      u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
      ra(x,y, "x=62, y=20");
      u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
      /* descent usually is negative */
      vm(x-1+w+2+5,y-9+h+2-1+1, h+4);
      hm(x-19, y-8+h+2+3,34+4);
      //hm(x, y-8+h+2+3, w);

      //ra(60,40, "x=60, y=40");
      //vm(60+16+2,40+9-1, 9);
      //hm(60, 40+9+1, 16);
      
      //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
    } while( u8g2_NextPage(&desc) );

  }
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_button2.png");

  /*=========================================*/
  /* u8g2_button3.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);

  {
    char *s = "Btn";
    u8g2_uint_t w = u8g2_GetStrWidth(&u8g2, s);
    u8g2_uint_t h = u8g2_GetAscent(&u8g2)-u8g2_GetDescent(&u8g2);
    u8g2_uint_t x = 62;
    u8g2_uint_t y = 20;
    
    u8g2_SetFontPosBaseline(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawButtonUTF8(&u8g2, x, y, U8G2_BTN_SHADOW1|U8G2_BTN_HCENTER|U8G2_BTN_BW2, 34,  2,  2, s );
    } while( u8g2_NextPage(&u8g2) );

    tga_is_transparent = 1;
    u8g2_FirstPage(&desc);
    do
    {
      u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
      ra(x,y, "x=62, y=20");
      u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
      /* descent usually is negative */
      vm(x-1+w+2+8,y-9+h+2-1+1, h+4);
      hm(x-19, y-8+h+2+6,34+4);
      //hm(x, y-8+h+2+3, w);

      //ra(60,40, "x=60, y=40");
      //vm(60+16+2,40+9-1, 9);
      //hm(60, 40+9+1, 16);
      
      //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
    } while( u8g2_NextPage(&desc) );

  }
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_button3.png");

  /*=========================================*/
  /* u8g2_inv_button1.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);

  {
    char *s = "Btn";
    u8g2_uint_t w = u8g2_GetStrWidth(&u8g2, s);
    u8g2_uint_t h = u8g2_GetAscent(&u8g2)-u8g2_GetDescent(&u8g2);
    u8g2_uint_t x = 62;
    u8g2_uint_t y = 20;
    
    u8g2_SetFontPosBaseline(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawButtonUTF8(&u8g2, x, y, U8G2_BTN_INV, 0,  2,  2, s );
    } while( u8g2_NextPage(&u8g2) );

    tga_is_transparent = 1;
    u8g2_FirstPage(&desc);
    do
    {
      u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
      ra(x,y, "x=62, y=20");
      u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
      /* descent usually is negative */
      vm(x-1+w+2+4,y-9+h+2-1+1, h+4);
      hm(x-2, y-8+h+2+3, w+4);
      //hm(x, y-8+h+2+3, w);

      //ra(60,40, "x=60, y=40");
      //vm(60+16+2,40+9-1, 9);
      //hm(60, 40+9+1, 16);
      
      //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
    } while( u8g2_NextPage(&desc) );

  }
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_inv_button1.png");
  

  /*=========================================*/
  /* u8g2_inv_button2.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);

  {
    char *s = "Btn";
    u8g2_uint_t w = u8g2_GetStrWidth(&u8g2, s);
    u8g2_uint_t h = u8g2_GetAscent(&u8g2)-u8g2_GetDescent(&u8g2);
    u8g2_uint_t x = 62;
    u8g2_uint_t y = 20;
    
    u8g2_SetFontPosBaseline(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawButtonUTF8(&u8g2, x, y, U8G2_BTN_INV|U8G2_BTN_BW2, 0,  2,  2, s );
    } while( u8g2_NextPage(&u8g2) );

    tga_is_transparent = 1;
    u8g2_FirstPage(&desc);
    do
    {
      u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
      ra(x,y, "x=62, y=20");
      u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
      /* descent usually is negative */
      vm(x-1+w+2+4,y-9+h+2-1+1, h+4);
      hm(x-2, y-8+h+2+3, w+4);
      //hm(x, y-8+h+2+3, w);

      //ra(60,40, "x=60, y=40");
      //vm(60+16+2,40+9-1, 9);
      //hm(60, 40+9+1, 16);
      
      //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
    } while( u8g2_NextPage(&desc) );

  }
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_inv_button2.png");
  

  /*=========================================*/
  /* u8g2_inv_button3.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);

  {
    char *s = "Btn";
    u8g2_uint_t w = u8g2_GetStrWidth(&u8g2, s);
    u8g2_uint_t h = u8g2_GetAscent(&u8g2)-u8g2_GetDescent(&u8g2);
    u8g2_uint_t x = 5;
    u8g2_uint_t y = 20;
    
    u8g2_SetFontPosBaseline(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawButtonUTF8(&u8g2, x, y, U8G2_BTN_INV, u8g2_GetDisplayWidth(&u8g2)-x*2,  x,  2, s );
    } while( u8g2_NextPage(&u8g2) );

    tga_is_transparent = 1;
    u8g2_FirstPage(&desc);
    do
    {
      u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
      ra(x,y, "x=5, y=20");
      u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
      /* descent usually is negative */
      vm(x-1+w+2+4,y-9+h+2-1+1, h+4);
      //hm(x-2, y-8+h+2+3, w+4);
      //hm(x, y-8+h+2+3, w);

      //ra(60,40, "x=60, y=40");
      //vm(60+16+2,40+9-1, 9);
      //hm(60, 40+9+1, 16);
      
      //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
    } while( u8g2_NextPage(&desc) );

  }
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_inv_button3.png");
  
  
  /*=========================================*/
  /* u8g2_ellipse.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    //u8g2_DrawEllipse(&u8g2, 20, 25, 15, 10, U8G2_DRAW_UPPER_RIGHT|U8G2_DRAW_LOWER_LEFT|U8G2_DRAW_LOWER_RIGHT);
    u8g2_DrawEllipse(&u8g2, 20, 25, 15, 10, U8G2_DRAW_ALL);
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(20,25, "x=20, y=25");
    /* descent usually is negative */
    vm(20+15+1, 25-1, 10);
    vm(20+15+1+13, 25+10, 21);
    hm(20+1, 25+10+1, 15);
    hm(20-15, 25+10+1+11, 31);

  
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_ellipse.png");

  /*=========================================*/
  /* u8g2_circle.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    //u8g2_DrawEllipse(&u8g2, 20, 25, 15, 10, U8G2_DRAW_UPPER_RIGHT|U8G2_DRAW_LOWER_LEFT|U8G2_DRAW_LOWER_RIGHT);
    u8g2_DrawCircle(&u8g2, 20, 25, 10, U8G2_DRAW_ALL);
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(20,25, "x=20, y=25");
    /* descent usually is negative */
    vm(20+15+1, 25-1, 10);
    vm(20+15+1+13, 25+10, 21);
    hm(20+1, 25+10+1, 10);
    hm(20-10, 25+10+1+11, 21);

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_circle.png");

  /*=========================================*/
  /* u8g2_rframe.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    //u8g2_DrawEllipse(&u8g2, 20, 25, 15, 10, U8G2_DRAW_UPPER_RIGHT|U8G2_DRAW_LOWER_LEFT|U8G2_DRAW_LOWER_RIGHT);
    u8g2_DrawRFrame(&u8g2, 20, 15, 30, 22, 7);
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(20,15, "x=20, y=15");
    /* descent usually is negative */
    vm(20+30+1, 15+22-1, 7);
    vm(20+30+1+8, 15+22-1, 22);
    hm(20+30-7, 15+22+1, 7);
    hm(20, 15+22+1+11, 30);

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_rframe.png");


  /*=========================================*/
  /* u8g2_triangle.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    u8g2_DrawTriangle(&u8g2, 20, 5, 27, 50, 5, 32);
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(20,5, "x=20, y=5");
    ra(27,50, "x=27, y=50");
    ra(5,32, "x=5, y=32");

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_triangle.png");

  /*=========================================*/
  /* u8g2_line.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_DrawLine(&u8g2, 20, 5, 5, 32);
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(20,5, "x=20, y=5");
    ra(5,32, "x=5, y=32");

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_line.png");

  /*=========================================*/
  /* u8g2_font_direction.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tf);
    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 15, 20, "Abc");
    u8g2_SetFontDirection(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 15, 20, "Abc");
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(15,20, "x=15, y=20");

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);

  tga_save_png("u8g2_font_direction.png");

  
  /*=========================================*/
  /* u8g2_xor.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontMode(&u8g2, 1);
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawBox(&u8g2, 22, 2, 35, 50);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tf);
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 5, 18, "abcd");
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 5, 33, "abcd");
    u8g2_SetDrawColor(&u8g2, 2);
    u8g2_DrawStr(&u8g2, 5, 48, "abcd");
  } while( u8g2_NextPage(&u8g2) );
  u8g2_SetDrawColor(&u8g2, 1);

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(5,15, "color 0");
    ra(5,30, "color 1");
    ra(5,45, "color 2");

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);

  tga_save_png("u8g2_xor.png");


/*=========================================*/
/* u8g2_bitmap_solid.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontMode(&u8g2, 1);
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetBitmapMode(&u8g2, 0);
    u8g2_DrawXBM(&u8g2, 4,3, u8g2_logo_97x51_width, u8g2_logo_97x51_height,  u8g2_logo_97x51_bits);
    u8g2_DrawXBM(&u8g2, 12,11, u8g2_logo_97x51_width, u8g2_logo_97x51_height,  u8g2_logo_97x51_bits);
  } while( u8g2_NextPage(&u8g2) );
  u8g2_SetDrawColor(&u8g2, 1);

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(4,3, "(4, 3)");
    ra(12,11, "(12, 11)");

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);

  tga_save_png("u8g2_bitmap_solid.png");


  /*=========================================*/
/* u8g2_bitmap_transparent.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontMode(&u8g2, 1);
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetBitmapMode(&u8g2, 1);
    u8g2_DrawXBM(&u8g2, 4,3, u8g2_logo_97x51_width, u8g2_logo_97x51_height,  u8g2_logo_97x51_bits);
    u8g2_DrawXBM(&u8g2, 12,11, u8g2_logo_97x51_width, u8g2_logo_97x51_height,  u8g2_logo_97x51_bits);
  } while( u8g2_NextPage(&u8g2) );
  u8g2_SetDrawColor(&u8g2, 1);

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(4,3, "(4, 3)");
    ra(12,11, "(12, 11)");

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);

  tga_save_png("u8g2_bitmap_transparent.png");


  /*=========================================*/
  /* u8g2_color.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_uint_t x,y;
    u8g2_SetDrawColor(&u8g2, 1);
    for( y = 0; y < u8g2_GetDisplayHeight(&u8g2); y++ )
      for( x = 0; x < u8g2_GetDisplayWidth(&u8g2); x++ )
	if ( (x ^ y) & 1 )
	  u8g2_DrawPixel(&u8g2, x, y);
        
    u8g2_SetFont(&u8g2, u8g2_font_9x15B_mf);
    
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 3, 20, "Color = 1");
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 3, 40, "Color = 0");
  } while( u8g2_NextPage(&u8g2) );

  u8g2_SetDrawColor(&u8g2, 1);

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    ra(3,20, "x=3, y=20");
    ra(3,40, "x=3, y=40");

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_color.png");

  
  /*=========================================*/
  /* u8g2_fontmode.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosCenter(&u8g2);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_uint_t x,y;
    u8g2_SetDrawColor(&u8g2, 1);
    for( y = 0; y < u8g2_GetDisplayHeight(&u8g2); y++ )
      for( x = 0; x < u8g2_GetDisplayWidth(&u8g2); x++ )
	if ( (x ^ y) & 1 )
	  u8g2_DrawPixel(&u8g2, x, y);
        
    u8g2_SetFont(&u8g2, u8g2_font_8x13B_mf);
    
    u8g2_SetFontMode(&u8g2, 0);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 3, 15, "Color=1, Mode 0");
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 3, 30, "Color=0, Mode 0");

    u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 3, 45, "Color=1, Mode 1");
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 3, 60, "Color=0, Mode 1");
  } while( u8g2_NextPage(&u8g2) );

  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_SetFontMode(&u8g2, 1);

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    ra(3,15, "x=3, y=15");
    ra(3,30, "x=3, y=30");
    ra(3,45, "x=3, y=45");
    ra(3,60, "x=3, y=60");

    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);

  tga_save_png("u8g2_fontmode.png");

  /*=========================================*/
  /* u8g2_message.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
  u8g2_SetFontRefHeightAll(&u8g2);  /* this will add some extra space for the text inside the buttons */
  
  u8g2_UserInterfaceMessage(&u8g2, "Title1", "Title2", "Title3", " Ok \n Cancel ");
  
  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    /*
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(20,5, "x=20, y=5");
    ra(27,50, "x=27, y=50");
    ra(5,32, "x=5, y=32");
    */
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontRefHeightText(&u8g2);

  tga_save_png("u8g2_message.png");

  /*=========================================*/
  /* u8g2_selection_list.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
  u8g2_SetFontRefHeightAll(&u8g2);  /* this will add some extra space for the text inside the buttons */
  
  u8g2_UserInterfaceSelectionList(&u8g2, "Title", 1, "abcdef\nghijkl\nmnopqr");
  
  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    /*
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(20,5, "x=20, y=5");
    ra(27,50, "x=27, y=50");
    ra(5,32, "x=5, y=32");
    */
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontRefHeightText(&u8g2);

  tga_save_png("u8g2_selection_list.png");

  /*=========================================*/
  /* u8g2_input_value.png */
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
  u8g2_SetFontRefHeightAll(&u8g2);  /* this will add some extra space for the text inside the buttons */
  
  {
    uint8_t v = 4;
    u8g2_UserInterfaceInputValue(&u8g2, "Select Voltage", "DAC= ", &v, 0, 5, 1, " V");
  }
  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    /*
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(20,5, "x=20, y=5");
    ra(27,50, "x=27, y=50");
    ra(5,32, "x=5, y=32");
    */
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontRefHeightText(&u8g2);

  tga_save_png("u8g2_input_value.png");

  /*=========================================*/
  /* u8g2_clip_window.png */

  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 

  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFont(&u8g2, u8g2_font_ncenB24_tf);
  u8g2_SetClipWindow(&u8g2, 10, 10, 85, 30);
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_DrawStr(&u8g2, 3, 32, "U8g2");
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(10,10, "(10, 10)");
    ra(85,30, "(85, 30)");
    ra(3,32, "(3, 32)");
    hm(10, 35, 75);
    vm(88, 29, 20);
    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_SetMaxClipWindow(&u8g2);

  tga_save_png("u8g2_clip_window.png");
  
  /*=========================================*/
  /* u8g2_tile_row.png */

  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  
  u8g2_FirstPage(&u8g2);
  do
  {
  } while( u8g2_NextPage(&u8g2) );
  

  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetDrawColor(&u8g2, 1);
  
  u8g2_SetBufferCurrTileRow(&u8g2, 0);
  u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);
  u8g2_DrawStr(&u8g2, 2, 8, "abcdefg");
  
  u8g2_SetBufferCurrTileRow(&u8g2, 2);
  u8g2_SendBuffer(&u8g2);
  u8g2_SetBufferCurrTileRow(&u8g2, 4);
  u8g2_SendBuffer(&u8g2);

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(2,8, "(2, 8)");
    ra(2,24, "(2, 24)");
    ra(2,40, "(2, 40)");
    //hm(10, 35, 75);
    //vm(88, 29, 20);
    
    //vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  
  tga_is_transparent = 0;
  u8g2_SetFontPosBaseline(&u8g2);
  u8g2_SetFontDirection(&u8g2, 0);

  tga_save_png("u8g2_tile_row.png");
  
  /*=========================================*/
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc));

  return 0;
}

