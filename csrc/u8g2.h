/*

  u8g2.h

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


  call sequence
  
  u8g2_SetupBuffer_XYZ
    u8x8_Setup_XYZ
      u8x8_SetupDefaults(u8g2);
      assign u8x8 callbacks
      u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SETUP_MEMORY, 0, NULL);  
    setup tile buffer
    
  
  Arduino Uno Text Example
>	FONT_ROTATION	INTERSECTION	CLIPPING	text	   	data		bss		dec		hex	
>																		8700
>	x				x				x			7450	104		1116	8670	21de
>	-				x				x			7132	104		1115	8351	209f
>	x				-				x			7230	104		1116	8450	2102
>	-				-				x			7010	104		1115	8229	2025
>	-				-				-			6880	104		1115	8099	1fa3
  
  
*/


#ifndef _U8G2_H
#define _U8G2_H

#include "u8x8.h"


/*
  The following macro enables 16 Bit mode. 
  Without defining this macro all calulations are done with 8 Bit (1 Byte) variables.
  Especially on AVR architecture, this will save some space. 
  If this macro is defined, then U8g2 will switch to 16 Bit mode.
  Use 16 Bit mode for any display with more than 240 pixel in one 
  direction.
*/
//#define U8G2_16BIT


/*
  The following macro enables a special check and optimization
  in the HVLine procedure for lines with one pixel length.
  It enabled, it will consume about 60 bytes in flash memory of the AVR and
  there will be a speed improvement of about 20% for any single pixel draw.
*/
#define U8G2_WITH_ONE_PIXEL_OPTIMIZATION


/*
  The following macro enables the HVLine speed optimization.
  It will consume about 40 bytes more in flash memory of the AVR.
  HVLine procedures are also used by the text drawing functions.
*/
#define U8G2_HVLINE_SPEED_OPTIMIZATION

/*
  The following macro enables all four drawing directions for glyphs and strings.
  If this macro is not defined, than a string can be drawn only in horizontal direction.
*/
#define U8G2_WITH_FONT_ROTATION

/*
  The following macro activates the early intersection check with the current visible area.
  Clipping (and low level intersection calculation) will still happen and is controlled by U8G2_WITH_CLIPPING.
  This early intersection check only improves speed for the picture loop (u8g2_FirstPage/NextPage).
  With a full framebuffer in RAM and if most graphical elements are drawn within the visible area, then this
  macro can be commented to reduce code size.
*/
#define U8G2_WITH_INTERSECTION

/*
  U8glib V2 contains support for unicode plane 0 (Basic Multilingual Plane, BMP).
  The following macro activates this support. Deactivation would save some ROM.
  This definition also defines the behavior of the expected string encoding.
  If the following macro is defined, then the DrawUTF8 function is enabled and 
  the string argument for this function is assumed 
  to be UTF-8 encoded.
  If the following macro is not defined, then all strings in the c-code are assumed 
  to be ISO 8859-1/CP1252 encoded. 
  Independently from this macro, the Arduino print function never accepts UTF-8
  strings.
  
  This macro does not affect the u8x8 string draw function.
  u8x8 has also two function, one for pure strings and one for UTF8
  
  Conclusion:
    U8G2_WITH_UNICODE defined
      - C-Code Strings must be UTF-8 encoded
      - Full support of all 65536 glyphs of the unicode basic multilingual plane
      - Up to 65536 glyphs of the font file can be used.
    U8G2_WITH_UNICODE not defined
      - C-Code Strings are assumbed to be ISO 8859-1/CP1252 encoded
      - Only character values 0 to 255 are supported in the font file.
*/
#define U8G2_WITH_UNICODE


/*
  Internal performance test for the effect of enabling U8G2_WITH_INTERSECTION
  Should not be defined for production code
*/
//#define U8G2_WITH_HVLINE_COUNT

/*
  Defining the following variable adds the clipping and check procedures agains the display boundaries.
  Clipping procedures are mandatory for the picture loop (u8g2_FirstPage/NextPage).
  Clipping procedures will also allow strings to exceed the display boundaries.
  On the other side, without clipping, all the setting of pixels must happen within the display boundaries.
  
  WARNING: Adding a comment in front of the following macro or removing the following line
  may lead to memory faults if you write any pixel outside the display boundary.
*/
#define U8G2_WITH_CLIPPING




/*==========================================*/


#ifdef __GNUC__
#  define U8G2_NOINLINE __attribute__((noinline))
#else
#  define U8G2_NOINLINE
#endif

#define U8G2_FONT_SECTION(name) U8X8_FONT_SECTION(name) 


/* the macro U8G2_USE_LARGE_FONTS disables large fonts (>32K) */
/* it can be enabled for those uC supporting larger arrays */
#if defined(__arm__) || defined(__arc__) || defined(ESP8266) || defined(ESP_PLATFORM)
#define U8G2_USE_LARGE_FONTS
#endif

/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================*/

#ifdef U8G2_16BIT
typedef uint16_t u8g2_uint_t;	/* for pixel position only */
typedef int16_t u8g2_int_t;		/* introduced for circle calculation */
typedef int32_t u8g2_long_t;		/* introduced for ellipse calculation */
#else
typedef uint8_t u8g2_uint_t;		/* for pixel position only */
typedef int8_t u8g2_int_t;		/* introduced for circle calculation */
typedef int16_t u8g2_long_t;		/* introduced for ellipse calculation */
#endif


typedef struct u8g2_struct u8g2_t;
typedef struct u8g2_cb_struct u8g2_cb_t;

typedef void (*u8g2_update_dimension_cb)(u8g2_t *u8g2);
typedef void (*u8g2_draw_l90_cb)(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);
typedef void (*u8g2_draw_ll_hvline_cb)(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);

typedef uint8_t (*u8g2_get_kerning_cb)(u8g2_t *u8g2, uint16_t e1, uint16_t e2);


/* from ucglib... */
struct _u8g2_font_info_t
{
  /* offset 0 */
  uint8_t glyph_cnt;
  uint8_t bbx_mode;
  uint8_t bits_per_0;
  uint8_t bits_per_1;
  
  /* offset 4 */
  uint8_t bits_per_char_width;
  uint8_t bits_per_char_height;		
  uint8_t bits_per_char_x;
  uint8_t bits_per_char_y;
  uint8_t bits_per_delta_x;
  
  /* offset 9 */
  int8_t max_char_width;
  int8_t max_char_height; /* overall height, NOT ascent. Instead ascent = max_char_height + y_offset */
  int8_t x_offset;
  int8_t y_offset;
  
  /* offset 13 */
  int8_t  ascent_A;
  int8_t  descent_g;	/* usually a negative value */
  int8_t  ascent_para;
  int8_t  descent_para;
    
  /* offset 17 */
  uint16_t start_pos_upper_A;
  uint16_t start_pos_lower_a; 
  
  /* offset 21 */
#ifdef U8G2_WITH_UNICODE  
  uint16_t start_pos_unicode;
#endif
};
typedef struct _u8g2_font_info_t u8g2_font_info_t;

/* from ucglib... */
struct _u8g2_font_decode_t
{
  const uint8_t *decode_ptr;			/* pointer to the compressed data */
  
  u8g2_uint_t target_x;
  u8g2_uint_t target_y;
  
  int8_t x;						/* local coordinates, (0,0) is upper left */
  int8_t y;
  int8_t glyph_width;	
  int8_t glyph_height;

  uint8_t decode_bit_pos;			/* bitpos inside a byte of the compressed data */
  uint8_t is_transparent;
  uint8_t fg_color;
  uint8_t bg_color;
#ifdef U8G2_WITH_FONT_ROTATION  
  uint8_t dir;				/* direction */
#endif
};
typedef struct _u8g2_font_decode_t u8g2_font_decode_t;

struct _u8g2_kerning_t
{
  uint16_t first_table_cnt;
  uint16_t second_table_cnt;
  const uint16_t *first_encoding_table;  
  const uint16_t *index_to_second_table;
  const uint16_t *second_encoding_table;
  const uint8_t *kerning_values;
};
typedef struct _u8g2_kerning_t u8g2_kerning_t;


struct u8g2_cb_struct
{
  u8g2_update_dimension_cb update;
  u8g2_draw_l90_cb draw_l90;
};

typedef u8g2_uint_t (*u8g2_font_calc_vref_fnptr)(u8g2_t *u8g2);


struct u8g2_struct
{
  u8x8_t u8x8;
  u8g2_draw_ll_hvline_cb ll_hvline;	/* low level hvline procedure */
  const u8g2_cb_t *cb;		/* callback drawprocedures, can be replaced for rotation */
  
  /* the following variables must be assigned during u8g2 setup */
  uint8_t *tile_buf_ptr;	/* ptr to memory area with u8g2.display_info->tile_width * 8 * tile_buf_height bytes */
  uint8_t tile_buf_height;	/* height of the tile memory area in tile rows */
  uint8_t tile_curr_row;	/* current row for picture loop */
  
  /* dimension of the buffer in pixel */
  u8g2_uint_t pixel_buf_width;		/* equal to tile_buf_height*8 */
  u8g2_uint_t pixel_buf_height;		/* u8g2.display_info->tile_width*8 */
  u8g2_uint_t pixel_curr_row;		/* u8g2.tile_curr_row*8 */
  
  /* the following variables are set by the update dimension callback */
  /* this is clipbox after rotation for the hvline procedures */
  //u8g2_uint_t buf_x0;	/* left corner of the buffer */
  //u8g2_uint_t buf_x1;	/* right corner of the buffer (excluded) */
  u8g2_uint_t buf_y0;
  u8g2_uint_t buf_y1;
  
  /* display dimensions in pixel for the user, calculated in u8g2_update_dimension_common(), used in u8g2_draw_hv_line_2dir() */
  u8g2_uint_t width;
  u8g2_uint_t height;
  
  /* ths is the clip box for the user to check if a specific box has an intersection */
  /* use u8g2_IsIntersection from u8g2_intersection.c to test against this intersection */
  /* boundary values are part of the box so that they can be used with u8g2_IsIntersection */
  u8g2_uint_t user_x0;	/* left corner of the buffer */
  u8g2_uint_t user_x1;	/* right corner of the buffer (excluded) */
  u8g2_uint_t user_y0;	/* upper edge of the buffer */
  u8g2_uint_t user_y1;	/* lower edge of the buffer (excluded) */
  
  /* information about the current font */
  const uint8_t *font;             /* current font for all text procedures */
  // removed: const u8g2_kerning_t *kerning;		/* can be NULL */
  // removed: u8g2_get_kerning_cb get_kerning_cb;
  
  u8g2_font_calc_vref_fnptr font_calc_vref;
  u8g2_font_decode_t font_decode;		/* new font decode structure */
  u8g2_font_info_t font_info;			/* new font info structure */

  uint8_t font_height_mode;
  int8_t font_ref_ascent;
  int8_t font_ref_descent;
  
  int8_t glyph_x_offset;		/* set by u8g2_GetGlyphWidth as a side effect */
  
  uint8_t bitmap_transparency;	/* black pixels will be treated as transparent (not drawn) */

  uint8_t draw_color;		/* 0: clear pixel, 1: set pixel, modified and restored by font procedures */
					/* draw_color can be used also directly by the user API */
					
  uint8_t is_auto_page_clear; 		/* set to 0 to disable automatic page clear in firstPage() and nextPage() */
  
#ifdef U8G2_WITH_HVLINE_COUNT
  unsigned long hv_cnt;
#endif /* U8G2_WITH_HVLINE_COUNT */   
};

#define u8g2_GetU8x8(u8g2) ((u8x8_t *)(u8g2))

#ifdef U8X8_WITH_USER_PTR
#define u8g2_GetUserPtr(u8g2) ((u8g2_GetU8x8(u8g2))->user_ptr)
#define u8g2_SetUserPtr(u8g2, p) ((u8g2_GetU8x8(u8g2))->user_ptr = (p))
#endif


#define u8g2_SetAutoPageClear(u8g2, mode) ((u8g2)->is_auto_page_clear = (mode))

/*==========================================*/
/* u8x8 wrapper */

#define u8g2_SetupDisplay(u8g2, display_cb, cad_cb, byte_cb, gpio_and_delay_cb) \
  u8x8_Setup(u8g2_GetU8x8(u8g2), (display_cb), (cad_cb), (byte_cb), (gpio_and_delay_cb))

#define u8g2_InitDisplay(u8g2) u8x8_InitDisplay(u8g2_GetU8x8(u8g2))
#define u8g2_SetPowerSave(u8g2, is_enable) u8x8_SetPowerSave(u8g2_GetU8x8(u8g2), (is_enable))
#define u8g2_SetFlipMode(u8g2, mode) u8x8_SetFlipMode(u8g2_GetU8x8(u8g2), (mode))
#define u8g2_SetContrast(u8g2, value) u8x8_SetContrast(u8g2_GetU8x8(u8g2), (value))
//#define u8g2_ClearDisplay(u8g2) u8x8_ClearDisplay(u8g2_GetU8x8(u8g2))  obsolete, can not be used in all cases
void u8g2_ClearDisplay(u8g2_t *u8g2);

#define u8g2_GetDisplayHeight(u8g2) ((u8g2)->height)
#define u8g2_GetDisplayWidth(u8g2) ((u8g2)->width)
#define u8g2_GetDrawColor(u8g2) ((u8g2)->draw_color)

#define u8g2_SetI2CAddress(u8g2, address) ((u8g2_GetU8x8(u8g2))->i2c_address = (address))
#define u8g2_GetI2CAddress(u8g2)   u8x8_GetI2CAddress(u8g2_GetU8x8(u8g2))

#ifdef U8X8_USE_PINS 
#define u8g2_SetMenuSelectPin(u8g2, val) u8x8_SetMenuSelectPin(u8g2_GetU8x8(u8g2), (val)) 
#define u8g2_SetMenuNextPin(u8g2, val) u8x8_SetMenuNextPin(u8g2_GetU8x8(u8g2), (val))
#define u8g2_SetMenuPrevPin(u8g2, val) u8x8_SetMenuPrevPin(u8g2_GetU8x8(u8g2), (val))
#define u8g2_SetMenuHomePin(u8g2, val) u8x8_SetMenuHomePin(u8g2_GetU8x8(u8g2), (val))
#define u8g2_SetMenuUpPin(u8g2, val) u8x8_SetMenuUpPin(u8g2_GetU8x8(u8g2), (val))
#define u8g2_SetMenuDownPin(u8g2, val) u8x8_SetMenuDownPin(u8g2_GetU8x8(u8g2), (val))
#endif

/*==========================================*/
/* u8g2_setup.c */

extern const u8g2_cb_t u8g2_cb_r0;
extern const u8g2_cb_t u8g2_cb_r1;
extern const u8g2_cb_t u8g2_cb_r2;
extern const u8g2_cb_t u8g2_cb_r3;
extern const u8g2_cb_t u8g2_cb_mirror;

#define U8G2_R0	(&u8g2_cb_r0)
#define U8G2_R1	(&u8g2_cb_r1)
#define U8G2_R2	(&u8g2_cb_r2)
#define U8G2_R3	(&u8g2_cb_r3)
#define U8G2_MIRROR	(&u8g2_cb_mirror)
/*
  u8g2:			A new, not yet initialized u8g2 memory areay
  buf:			Memory are of size tile_buf_height*<width of the display in pixel>
  tile_buf_height:	Number of full lines
  ll_hvline_cb:		one of:
    u8g2_ll_hvline_vertical_top_lsb
    u8g2_ll_hvline_horizontal_right_lsb
  u8g2_cb			U8G2_R0 .. U8G2_R3
      
*/

void u8g2_SetupBuffer(u8g2_t *u8g2, uint8_t *buf, uint8_t tile_buf_height, u8g2_draw_ll_hvline_cb ll_hvline_cb, const u8g2_cb_t *u8g2_cb);
void u8g2_SetDisplayRotation(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);

/*==========================================*/
/* u8g2_d_memory.c generated code start */
uint8_t *u8g2_m_ssd1305_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1305_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1305_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_sh1106_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_sh1106_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_sh1106_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_8_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_8_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_8_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_12_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_12_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1306_12_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1309_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1309_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1309_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1325_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1325_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1325_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1327_12_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1327_12_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1327_12_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1329_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1329_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1329_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ld7032_8_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ld7032_8_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ld7032_8_f(uint8_t *page_cnt);
uint8_t *u8g2_m_st7920_24_1(uint8_t *page_cnt);
uint8_t *u8g2_m_st7920_24_2(uint8_t *page_cnt);
uint8_t *u8g2_m_st7920_24_f(uint8_t *page_cnt);
uint8_t *u8g2_m_st7920_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_st7920_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_st7920_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ls013b7dh03_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ls013b7dh03_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ls013b7dh03_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1701_13_1(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1701_13_2(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1701_13_f(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1701_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1701_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1701_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_pcd8544_11_1(uint8_t *page_cnt);
uint8_t *u8g2_m_pcd8544_11_2(uint8_t *page_cnt);
uint8_t *u8g2_m_pcd8544_11_f(uint8_t *page_cnt);
uint8_t *u8g2_m_pcf8812_12_1(uint8_t *page_cnt);
uint8_t *u8g2_m_pcf8812_12_2(uint8_t *page_cnt);
uint8_t *u8g2_m_pcf8812_12_f(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1604_24_1(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1604_24_2(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1604_24_f(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1608_30_1(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1608_30_2(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1608_30_f(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1610_20_1(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1610_20_2(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1610_20_f(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1611_30_1(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1611_30_2(uint8_t *page_cnt);
uint8_t *u8g2_m_uc1611_30_f(uint8_t *page_cnt);
uint8_t *u8g2_m_st7565_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_st7565_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_st7565_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_st7565_17_1(uint8_t *page_cnt);
uint8_t *u8g2_m_st7565_17_2(uint8_t *page_cnt);
uint8_t *u8g2_m_st7565_17_f(uint8_t *page_cnt);
uint8_t *u8g2_m_st7567_17_1(uint8_t *page_cnt);
uint8_t *u8g2_m_st7567_17_2(uint8_t *page_cnt);
uint8_t *u8g2_m_st7567_17_f(uint8_t *page_cnt);
uint8_t *u8g2_m_st7588_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_st7588_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_st7588_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_nt7534_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_nt7534_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_nt7534_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ist3020_24_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ist3020_24_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ist3020_24_f(uint8_t *page_cnt);
uint8_t *u8g2_m_sbn1661_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_sbn1661_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_sbn1661_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_sed1520_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_sed1520_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_sed1520_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ks0108_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ks0108_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ks0108_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ks0108_24_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ks0108_24_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ks0108_24_f(uint8_t *page_cnt);
uint8_t *u8g2_m_lc7981_20_1(uint8_t *page_cnt);
uint8_t *u8g2_m_lc7981_20_2(uint8_t *page_cnt);
uint8_t *u8g2_m_lc7981_20_f(uint8_t *page_cnt);
uint8_t *u8g2_m_lc7981_30_1(uint8_t *page_cnt);
uint8_t *u8g2_m_lc7981_30_2(uint8_t *page_cnt);
uint8_t *u8g2_m_lc7981_30_f(uint8_t *page_cnt);
uint8_t *u8g2_m_t6963_30_1(uint8_t *page_cnt);
uint8_t *u8g2_m_t6963_30_2(uint8_t *page_cnt);
uint8_t *u8g2_m_t6963_30_f(uint8_t *page_cnt);
uint8_t *u8g2_m_t6963_32_1(uint8_t *page_cnt);
uint8_t *u8g2_m_t6963_32_2(uint8_t *page_cnt);
uint8_t *u8g2_m_t6963_32_f(uint8_t *page_cnt);
uint8_t *u8g2_m_t6963_16_1(uint8_t *page_cnt);
uint8_t *u8g2_m_t6963_16_2(uint8_t *page_cnt);
uint8_t *u8g2_m_t6963_16_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1322_32_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1322_32_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1322_32_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1606_22_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1606_22_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1606_22_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1607_25_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1607_25_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ssd1607_25_f(uint8_t *page_cnt);
uint8_t *u8g2_m_sed1330_30_1(uint8_t *page_cnt);
uint8_t *u8g2_m_sed1330_30_2(uint8_t *page_cnt);
uint8_t *u8g2_m_sed1330_30_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ra8835_30_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ra8835_30_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ra8835_30_f(uint8_t *page_cnt);
uint8_t *u8g2_m_ra8835_40_1(uint8_t *page_cnt);
uint8_t *u8g2_m_ra8835_40_2(uint8_t *page_cnt);
uint8_t *u8g2_m_ra8835_40_f(uint8_t *page_cnt);
uint8_t *u8g2_m_max7219_4_1(uint8_t *page_cnt);
uint8_t *u8g2_m_max7219_4_2(uint8_t *page_cnt);
uint8_t *u8g2_m_max7219_4_f(uint8_t *page_cnt);
uint8_t *u8g2_m_a2printer_48_1(uint8_t *page_cnt);
uint8_t *u8g2_m_a2printer_48_2(uint8_t *page_cnt);
uint8_t *u8g2_m_a2printer_48_f(uint8_t *page_cnt);

/* u8g2_d_memory.c generated code end */

/*==========================================*/
/* u8g2_d_setup.c generated code start */
void u8g2_Setup_ssd1305_128x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1305_128x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1305_128x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1305_i2c_128x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1305_i2c_128x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1305_i2c_128x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sh1106_i2c_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x32_univision_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x32_univision_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_128x32_univision_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x32_univision_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x32_univision_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_128x32_univision_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x48_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x48_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x48_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x48_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x48_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x48_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_64x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x32_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x32_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_64x32_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_96x16_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_96x16_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_96x16_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_96x16_er_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_96x16_er_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1306_i2c_96x16_er_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_128x64_noname2_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_128x64_noname2_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_128x64_noname2_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_i2c_128x64_noname2_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_i2c_128x64_noname2_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_i2c_128x64_noname2_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_128x64_noname0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_128x64_noname0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_128x64_noname0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_i2c_128x64_noname0_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_i2c_128x64_noname0_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1309_i2c_128x64_noname0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1325_nhd_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1325_nhd_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1325_nhd_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1325_i2c_nhd_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1325_i2c_nhd_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1325_i2c_nhd_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1327_seeed_96x96_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1327_seeed_96x96_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1327_seeed_96x96_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1327_i2c_seeed_96x96_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1327_i2c_seeed_96x96_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1327_i2c_seeed_96x96_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1329_128x96_noname_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1329_128x96_noname_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1329_128x96_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ld7032_60x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ld7032_60x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ld7032_60x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ld7032_i2c_60x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ld7032_i2c_60x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ld7032_i2c_60x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_p_192x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_p_192x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_p_192x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_192x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_192x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_192x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_s_192x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_s_192x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_s_192x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_p_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_p_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_p_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_s_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_s_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7920_s_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ls013b7dh03_128x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ls013b7dh03_128x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ls013b7dh03_128x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1701_ea_dogs102_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1701_ea_dogs102_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1701_ea_dogs102_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1701_mini12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1701_mini12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1701_mini12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_pcd8544_84x48_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_pcd8544_84x48_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_pcd8544_84x48_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_pcf8812_96x65_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_pcf8812_96x65_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_pcf8812_96x65_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1604_jlx19264_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1604_jlx19264_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1604_jlx19264_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1604_i2c_jlx19264_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1604_i2c_jlx19264_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1604_i2c_jlx19264_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_erc24064_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_erc24064_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_erc24064_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_i2c_erc24064_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_i2c_erc24064_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_i2c_erc24064_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_i2c_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_i2c_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1608_i2c_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1610_ea_dogxl160_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1610_ea_dogxl160_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1610_ea_dogxl160_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1610_i2c_ea_dogxl160_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1610_i2c_ea_dogxl160_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1610_i2c_ea_dogxl160_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_ea_dogm240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_ea_dogm240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_ea_dogm240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_i2c_ea_dogm240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_i2c_ea_dogm240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_i2c_ea_dogm240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_ea_dogxl240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_ea_dogxl240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_ea_dogxl240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_i2c_ea_dogxl240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_i2c_ea_dogxl240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_uc1611_i2c_ea_dogxl240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_ea_dogm128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_64128n_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_zolen_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_lm6059_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_erc12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_nhd_c12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_ea_dogm128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_64128n_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_zolen_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_lm6059_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_erc12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_nhd_c12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_ea_dogm128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_64128n_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_zolen_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_lm6059_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_erc12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_nhd_c12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_nhd_c12832_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_nhd_c12832_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_nhd_c12832_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_ea_dogm132_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_ea_dogm132_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7565_ea_dogm132_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7567_pi_132x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7567_pi_132x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7567_pi_132x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7588_jlx12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7588_jlx12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7588_jlx12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7588_i2c_jlx12864_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7588_i2c_jlx12864_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_st7588_i2c_jlx12864_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_nt7534_tg12864r_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_nt7534_tg12864r_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_nt7534_tg12864r_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ist3020_erc19264_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ist3020_erc19264_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ist3020_erc19264_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sbn1661_122x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sbn1661_122x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sbn1661_122x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sed1520_122x32_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sed1520_122x32_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sed1520_122x32_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ks0108_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ks0108_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ks0108_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ks0108_erm19264_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ks0108_erm19264_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ks0108_erm19264_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_lc7981_160x80_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_lc7981_160x80_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_lc7981_160x80_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_lc7981_160x160_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_lc7981_160x160_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_lc7981_160x160_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_lc7981_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_lc7981_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_lc7981_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_240x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_240x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_240x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_256x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_256x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_256x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_128x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_128x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_t6963_128x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1322_nhd_256x64_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1322_nhd_256x64_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1322_nhd_256x64_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1606_172x72_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1606_172x72_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1606_172x72_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1607_200x200_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1607_200x200_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ssd1607_200x200_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sed1330_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sed1330_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_sed1330_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ra8835_nhd_240x128_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ra8835_nhd_240x128_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ra8835_nhd_240x128_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ra8835_320x240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ra8835_320x240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_ra8835_320x240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_max7219_32x8_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_max7219_32x8_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_max7219_32x8_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_a2printer_384x240_1(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_a2printer_384x240_2(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);
void u8g2_Setup_a2printer_384x240_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);

/* u8g2_d_setup.c generated code end */

/*==========================================*/
/* u8g2_buffer.c */

void u8g2_SendBuffer(u8g2_t *u8g2);
void u8g2_ClearBuffer(u8g2_t *u8g2);

void u8g2_SetPageCurrTileRow(u8g2_t *u8g2, uint8_t row) U8G2_NOINLINE;
void u8g2_FirstPage(u8g2_t *u8g2);
uint8_t u8g2_NextPage(u8g2_t *u8g2);

#define u8g2_GetBufferPtr(u8g2) ((u8g2)->tile_buf_ptr)
#define u8g2_GetBufferTileHeight(u8g2)	((u8g2)->tile_buf_height)
#define u8g2_GetBufferTileWidth(u8g2)	(u8g2_GetU8x8(u8g2)->display_info->tile_width)
/* the following variable is only valid after calling u8g2_FirstPage */
#define u8g2_GetPageCurrTileRow(u8g2) ((u8g2)->tile_curr_row)

/*==========================================*/
/* u8g2_ll_hvline.c */
/*
  x,y		Upper left position of the line within the local buffer (not the display!)
  len		length of the line in pixel, len must not be 0
  dir		0: horizontal line (left to right)
		1: vertical line (top to bottom)
  asumption: 
    all clipping done
*/

/* SSD13xx, UC17xx, UC16xx */
void u8g2_ll_hvline_vertical_top_lsb(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);
/* ST7920 */
void u8g2_ll_hvline_horizontal_right_lsb(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);


/*==========================================*/
/* u8g2_hvline.c */

/* u8g2_DrawHVLine does not use u8g2_IsIntersection */
void u8g2_DrawHVLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);

/* the following three function will do an intersection test of this is enabled with U8G2_WITH_INTERSECTION */
void u8g2_DrawHLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len);
void u8g2_DrawVLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len);
void u8g2_DrawPixel(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y);
void u8g2_SetDrawColor(u8g2_t *u8g2, uint8_t color) U8G2_NOINLINE;  /* u8g: u8g_SetColorIndex(u8g_t *u8g, uint8_t idx); */


/*==========================================*/
/* u8g2_bitmap.c */
void u8g2_SetBitmapMode(u8g2_t *u8g2, uint8_t is_transparent);
void u8g2_DrawHorizontalBitmap(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, const uint8_t *b);
void u8g2_DrawBitmap(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t cnt, u8g2_uint_t h, const uint8_t *bitmap);
void u8g2_DrawXBM(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, const uint8_t *bitmap);
void u8g2_DrawXBMP(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, const uint8_t *bitmap);	/* assumes bitmap in PROGMEM */


/*==========================================*/
/* u8g2_intersection.c */
#ifdef U8G2_WITH_INTERSECTION    
uint8_t u8g2_IsIntersection(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t x1, u8g2_uint_t y1);
#endif /* U8G2_WITH_INTERSECTION */



/*==========================================*/
/* u8g2_circle.c */
#define U8G2_DRAW_UPPER_RIGHT 0x01
#define U8G2_DRAW_UPPER_LEFT  0x02
#define U8G2_DRAW_LOWER_LEFT 0x04
#define U8G2_DRAW_LOWER_RIGHT  0x08
#define U8G2_DRAW_ALL (U8G2_DRAW_UPPER_RIGHT|U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_LOWER_RIGHT|U8G2_DRAW_LOWER_LEFT)
void u8g2_DrawCircle(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t option);
void u8g2_DrawDisc(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t option);
void u8g2_DrawEllipse(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rx, u8g2_uint_t ry, uint8_t option);
void u8g2_DrawFilledEllipse(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rx, u8g2_uint_t ry, uint8_t option);

/*==========================================*/
/* u8g2_line.c */
void u8g2_DrawLine(u8g2_t *u8g2, u8g2_uint_t x1, u8g2_uint_t y1, u8g2_uint_t x2, u8g2_uint_t y2);


/*==========================================*/
/* u8g2_box.c */
void u8g2_DrawBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h);
void u8g2_DrawFrame(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h);
void u8g2_DrawRBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r);
void u8g2_DrawRFrame(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r);


/*==========================================*/
/* u8g2_polygon.c */
void u8g2_ClearPolygonXY(void);
void u8g2_AddPolygonXY(u8g2_t *u8g2, int16_t x, int16_t y);
void u8g2_DrawPolygon(u8g2_t *u8g2);
void u8g2_DrawTriangle(u8g2_t *u8g2, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);



/*==========================================*/
/* u8g2_kerning.c */
//uint8_t u8g2_GetNullKerning(u8g2_t *u8g2, uint16_t e1, uint16_t e2);
uint8_t u8g2_GetKerning(u8g2_t *u8g2, u8g2_kerning_t *kerning, uint16_t e1, uint16_t e2);
uint8_t u8g2_GetKerningByTable(u8g2_t *u8g2, const uint16_t *kt, uint16_t e1, uint16_t e2);


/*==========================================*/
/* u8g2_font.c */

size_t u8g2_GetFontSize(const uint8_t *font_arg);

#define U8G2_FONT_HEIGHT_MODE_TEXT 0
#define U8G2_FONT_HEIGHT_MODE_XTEXT 1
#define U8G2_FONT_HEIGHT_MODE_ALL 2

void u8g2_SetFont(u8g2_t *u8g2, const uint8_t  *font);
void u8g2_SetFontMode(u8g2_t *u8g2, uint8_t is_transparent);

uint8_t u8g2_IsGlyph(u8g2_t *u8g2, uint16_t requested_encoding);
int8_t u8g2_GetGlyphWidth(u8g2_t *u8g2, uint16_t requested_encoding);
u8g2_uint_t u8g2_DrawGlyph(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, uint16_t encoding);
int8_t u8g2_GetStrX(u8g2_t *u8g2, const char *s);	/* for u8g compatibility */

void u8g2_SetFontDirection(u8g2_t *u8g2, uint8_t dir);
u8g2_uint_t u8g2_DrawStr(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *str);
u8g2_uint_t u8g2_DrawUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *str);
u8g2_uint_t u8g2_DrawExtendedUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, uint8_t to_left, u8g2_kerning_t *kerning, const char *str);
u8g2_uint_t u8g2_DrawExtUTF8(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, uint8_t to_left, const uint16_t *kerning_table, const char *str);

#define u8g2_GetMaxCharHeight(u8g2) ((u8g2)->font_info.max_char_height)
#define u8g2_GetMaxCharWidth(u8g2) ((u8g2)->font_info.max_char_width)
#define u8g2_GetAscent(u8g2) ((u8g2)->font_ref_ascent)
#define u8g2_GetDescent(u8g2) ((u8g2)->font_ref_descent)
#define u8g2_GetFontAscent(u8g2) ((u8g2)->font_ref_ascent)
#define u8g2_GetFontDescent(u8g2) ((u8g2)->font_ref_descent)

u8g2_uint_t u8g2_GetStrWidth(u8g2_t *u8g2, const char *s);
u8g2_uint_t u8g2_GetUTF8Width(u8g2_t *u8g2, const char *str);

void u8g2_SetFontPosBaseline(u8g2_t *u8g2);
void u8g2_SetFontPosBottom(u8g2_t *u8g2);
void u8g2_SetFontPosTop(u8g2_t *u8g2);
void u8g2_SetFontPosCenter(u8g2_t *u8g2);

void u8g2_SetFontRefHeightText(u8g2_t *u8g2);
void u8g2_SetFontRefHeightExtendedText(u8g2_t *u8g2);
void u8g2_SetFontRefHeightAll(u8g2_t *u8g2);



/*==========================================*/
/* u8g2_selection_list.c */
void u8g2_DrawUTF8Line(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, const char *s, uint8_t border_size, uint8_t is_invert);
u8g2_uint_t u8g2_DrawUTF8Lines(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t line_height, const char *s);
uint8_t u8g2_UserInterfaceSelectionList(u8g2_t *u8g2, const char *title, uint8_t start_pos, const char *sl);

/*==========================================*/
/* u8g2_message.c */
uint8_t u8g2_UserInterfaceMessage(u8g2_t *u8g2, const char *title1, const char *title2, const char *title3, const char *buttons);

/*==========================================*/
/* u8g2_input_value.c */
uint8_t u8g2_UserInterfaceInputValue(u8g2_t *u8g2, const char *title, const char *pre, uint8_t *value, uint8_t lo, uint8_t hi, uint8_t digits, const char *post);


/*==========================================*/
/* u8x8_d_sdl_128x64.c */
void u8g2_SetupBuffer_SDL_128x64(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
void u8g2_SetupBuffer_SDL_128x64_4(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);

/*==========================================*/
/* u8x8_d_tga.c */
void u8g2_SetupBuffer_TGA_DESC(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
void u8g2_SetupBuffer_TGA_LCD(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);

/*==========================================*/
/* u8x8_d_utf8.c */
/* 96x32 stdout */
void u8g2_SetupBuffer_Utf8(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);




/*==========================================*/
/* itoa procedures */
#define u8g2_u8toa u8x8_u8toa
#define u8g2_u16toa u8x8_u16toa


/*==========================================*/

/* start font list */
extern const uint8_t u8g2_font_u8glib_4_tf[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_tf");
extern const uint8_t u8g2_font_u8glib_4_tr[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_tr");
extern const uint8_t u8g2_font_u8glib_4_hf[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_hf");
extern const uint8_t u8g2_font_u8glib_4_hr[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_hr");
extern const uint8_t u8g2_font_m2icon_5_tf[] U8G2_FONT_SECTION("u8g2_font_m2icon_5_tf");
extern const uint8_t u8g2_font_m2icon_7_tf[] U8G2_FONT_SECTION("u8g2_font_m2icon_7_tf");
extern const uint8_t u8g2_font_m2icon_9_tf[] U8G2_FONT_SECTION("u8g2_font_m2icon_9_tf");
extern const uint8_t u8g2_font_freedoomr10_tu[] U8G2_FONT_SECTION("u8g2_font_freedoomr10_tu");
extern const uint8_t u8g2_font_freedoomr10_mu[] U8G2_FONT_SECTION("u8g2_font_freedoomr10_mu");
extern const uint8_t u8g2_font_freedoomr25_tn[] U8G2_FONT_SECTION("u8g2_font_freedoomr25_tn");
extern const uint8_t u8g2_font_freedoomr25_mn[] U8G2_FONT_SECTION("u8g2_font_freedoomr25_mn");
extern const uint8_t u8g2_font_cursor_tf[] U8G2_FONT_SECTION("u8g2_font_cursor_tf");
extern const uint8_t u8g2_font_cursor_tr[] U8G2_FONT_SECTION("u8g2_font_cursor_tr");
extern const uint8_t u8g2_font_micro_tr[] U8G2_FONT_SECTION("u8g2_font_micro_tr");
extern const uint8_t u8g2_font_micro_tn[] U8G2_FONT_SECTION("u8g2_font_micro_tn");
extern const uint8_t u8g2_font_micro_mr[] U8G2_FONT_SECTION("u8g2_font_micro_mr");
extern const uint8_t u8g2_font_micro_mn[] U8G2_FONT_SECTION("u8g2_font_micro_mn");
extern const uint8_t u8g2_font_4x6_tf[] U8G2_FONT_SECTION("u8g2_font_4x6_tf");
extern const uint8_t u8g2_font_4x6_tr[] U8G2_FONT_SECTION("u8g2_font_4x6_tr");
extern const uint8_t u8g2_font_4x6_tn[] U8G2_FONT_SECTION("u8g2_font_4x6_tn");
extern const uint8_t u8g2_font_4x6_mf[] U8G2_FONT_SECTION("u8g2_font_4x6_mf");
extern const uint8_t u8g2_font_4x6_mr[] U8G2_FONT_SECTION("u8g2_font_4x6_mr");
extern const uint8_t u8g2_font_4x6_mn[] U8G2_FONT_SECTION("u8g2_font_4x6_mn");
extern const uint8_t u8g2_font_5x7_tf[] U8G2_FONT_SECTION("u8g2_font_5x7_tf");
extern const uint8_t u8g2_font_5x7_tr[] U8G2_FONT_SECTION("u8g2_font_5x7_tr");
extern const uint8_t u8g2_font_5x7_tn[] U8G2_FONT_SECTION("u8g2_font_5x7_tn");
extern const uint8_t u8g2_font_5x7_mf[] U8G2_FONT_SECTION("u8g2_font_5x7_mf");
extern const uint8_t u8g2_font_5x7_mr[] U8G2_FONT_SECTION("u8g2_font_5x7_mr");
extern const uint8_t u8g2_font_5x7_mn[] U8G2_FONT_SECTION("u8g2_font_5x7_mn");
extern const uint8_t u8g2_font_5x8_tf[] U8G2_FONT_SECTION("u8g2_font_5x8_tf");
extern const uint8_t u8g2_font_5x8_tr[] U8G2_FONT_SECTION("u8g2_font_5x8_tr");
extern const uint8_t u8g2_font_5x8_tn[] U8G2_FONT_SECTION("u8g2_font_5x8_tn");
extern const uint8_t u8g2_font_5x8_mf[] U8G2_FONT_SECTION("u8g2_font_5x8_mf");
extern const uint8_t u8g2_font_5x8_mr[] U8G2_FONT_SECTION("u8g2_font_5x8_mr");
extern const uint8_t u8g2_font_5x8_mn[] U8G2_FONT_SECTION("u8g2_font_5x8_mn");
extern const uint8_t u8g2_font_6x10_tf[] U8G2_FONT_SECTION("u8g2_font_6x10_tf");
extern const uint8_t u8g2_font_6x10_tr[] U8G2_FONT_SECTION("u8g2_font_6x10_tr");
extern const uint8_t u8g2_font_6x10_tn[] U8G2_FONT_SECTION("u8g2_font_6x10_tn");
extern const uint8_t u8g2_font_6x10_mf[] U8G2_FONT_SECTION("u8g2_font_6x10_mf");
extern const uint8_t u8g2_font_6x10_mr[] U8G2_FONT_SECTION("u8g2_font_6x10_mr");
extern const uint8_t u8g2_font_6x10_mn[] U8G2_FONT_SECTION("u8g2_font_6x10_mn");
extern const uint8_t u8g2_font_6x12_tf[] U8G2_FONT_SECTION("u8g2_font_6x12_tf");
extern const uint8_t u8g2_font_6x12_tr[] U8G2_FONT_SECTION("u8g2_font_6x12_tr");
extern const uint8_t u8g2_font_6x12_tn[] U8G2_FONT_SECTION("u8g2_font_6x12_tn");
extern const uint8_t u8g2_font_6x12_te[] U8G2_FONT_SECTION("u8g2_font_6x12_te");
extern const uint8_t u8g2_font_6x12_mf[] U8G2_FONT_SECTION("u8g2_font_6x12_mf");
extern const uint8_t u8g2_font_6x12_mr[] U8G2_FONT_SECTION("u8g2_font_6x12_mr");
extern const uint8_t u8g2_font_6x12_mn[] U8G2_FONT_SECTION("u8g2_font_6x12_mn");
extern const uint8_t u8g2_font_6x12_me[] U8G2_FONT_SECTION("u8g2_font_6x12_me");
extern const uint8_t u8g2_font_6x12_t_symbols[] U8G2_FONT_SECTION("u8g2_font_6x12_t_symbols");
extern const uint8_t u8g2_font_6x13_tf[] U8G2_FONT_SECTION("u8g2_font_6x13_tf");
extern const uint8_t u8g2_font_6x13_tr[] U8G2_FONT_SECTION("u8g2_font_6x13_tr");
extern const uint8_t u8g2_font_6x13_tn[] U8G2_FONT_SECTION("u8g2_font_6x13_tn");
extern const uint8_t u8g2_font_6x13_te[] U8G2_FONT_SECTION("u8g2_font_6x13_te");
extern const uint8_t u8g2_font_6x13_mf[] U8G2_FONT_SECTION("u8g2_font_6x13_mf");
extern const uint8_t u8g2_font_6x13_mr[] U8G2_FONT_SECTION("u8g2_font_6x13_mr");
extern const uint8_t u8g2_font_6x13_mn[] U8G2_FONT_SECTION("u8g2_font_6x13_mn");
extern const uint8_t u8g2_font_6x13_me[] U8G2_FONT_SECTION("u8g2_font_6x13_me");
extern const uint8_t u8g2_font_6x13_t_hebrew[] U8G2_FONT_SECTION("u8g2_font_6x13_t_hebrew");
extern const uint8_t u8g2_font_6x13B_tf[] U8G2_FONT_SECTION("u8g2_font_6x13B_tf");
extern const uint8_t u8g2_font_6x13B_tr[] U8G2_FONT_SECTION("u8g2_font_6x13B_tr");
extern const uint8_t u8g2_font_6x13B_tn[] U8G2_FONT_SECTION("u8g2_font_6x13B_tn");
extern const uint8_t u8g2_font_6x13B_mf[] U8G2_FONT_SECTION("u8g2_font_6x13B_mf");
extern const uint8_t u8g2_font_6x13B_mr[] U8G2_FONT_SECTION("u8g2_font_6x13B_mr");
extern const uint8_t u8g2_font_6x13B_mn[] U8G2_FONT_SECTION("u8g2_font_6x13B_mn");
extern const uint8_t u8g2_font_6x13B_t_hebrew[] U8G2_FONT_SECTION("u8g2_font_6x13B_t_hebrew");
extern const uint8_t u8g2_font_6x13O_tf[] U8G2_FONT_SECTION("u8g2_font_6x13O_tf");
extern const uint8_t u8g2_font_6x13O_tr[] U8G2_FONT_SECTION("u8g2_font_6x13O_tr");
extern const uint8_t u8g2_font_6x13O_tn[] U8G2_FONT_SECTION("u8g2_font_6x13O_tn");
extern const uint8_t u8g2_font_6x13O_mf[] U8G2_FONT_SECTION("u8g2_font_6x13O_mf");
extern const uint8_t u8g2_font_6x13O_mr[] U8G2_FONT_SECTION("u8g2_font_6x13O_mr");
extern const uint8_t u8g2_font_6x13O_mn[] U8G2_FONT_SECTION("u8g2_font_6x13O_mn");
extern const uint8_t u8g2_font_7x13_tf[] U8G2_FONT_SECTION("u8g2_font_7x13_tf");
extern const uint8_t u8g2_font_7x13_tr[] U8G2_FONT_SECTION("u8g2_font_7x13_tr");
extern const uint8_t u8g2_font_7x13_tn[] U8G2_FONT_SECTION("u8g2_font_7x13_tn");
extern const uint8_t u8g2_font_7x13_te[] U8G2_FONT_SECTION("u8g2_font_7x13_te");
extern const uint8_t u8g2_font_7x13_mf[] U8G2_FONT_SECTION("u8g2_font_7x13_mf");
extern const uint8_t u8g2_font_7x13_mr[] U8G2_FONT_SECTION("u8g2_font_7x13_mr");
extern const uint8_t u8g2_font_7x13_mn[] U8G2_FONT_SECTION("u8g2_font_7x13_mn");
extern const uint8_t u8g2_font_7x13_me[] U8G2_FONT_SECTION("u8g2_font_7x13_me");
extern const uint8_t u8g2_font_7x13_t_symbols[] U8G2_FONT_SECTION("u8g2_font_7x13_t_symbols");
extern const uint8_t u8g2_font_7x13B_tf[] U8G2_FONT_SECTION("u8g2_font_7x13B_tf");
extern const uint8_t u8g2_font_7x13B_tr[] U8G2_FONT_SECTION("u8g2_font_7x13B_tr");
extern const uint8_t u8g2_font_7x13B_tn[] U8G2_FONT_SECTION("u8g2_font_7x13B_tn");
extern const uint8_t u8g2_font_7x13B_mf[] U8G2_FONT_SECTION("u8g2_font_7x13B_mf");
extern const uint8_t u8g2_font_7x13B_mr[] U8G2_FONT_SECTION("u8g2_font_7x13B_mr");
extern const uint8_t u8g2_font_7x13B_mn[] U8G2_FONT_SECTION("u8g2_font_7x13B_mn");
extern const uint8_t u8g2_font_7x13O_tf[] U8G2_FONT_SECTION("u8g2_font_7x13O_tf");
extern const uint8_t u8g2_font_7x13O_tr[] U8G2_FONT_SECTION("u8g2_font_7x13O_tr");
extern const uint8_t u8g2_font_7x13O_tn[] U8G2_FONT_SECTION("u8g2_font_7x13O_tn");
extern const uint8_t u8g2_font_7x13O_mf[] U8G2_FONT_SECTION("u8g2_font_7x13O_mf");
extern const uint8_t u8g2_font_7x13O_mr[] U8G2_FONT_SECTION("u8g2_font_7x13O_mr");
extern const uint8_t u8g2_font_7x13O_mn[] U8G2_FONT_SECTION("u8g2_font_7x13O_mn");
extern const uint8_t u8g2_font_7x14_tf[] U8G2_FONT_SECTION("u8g2_font_7x14_tf");
extern const uint8_t u8g2_font_7x14_tr[] U8G2_FONT_SECTION("u8g2_font_7x14_tr");
extern const uint8_t u8g2_font_7x14_tn[] U8G2_FONT_SECTION("u8g2_font_7x14_tn");
extern const uint8_t u8g2_font_7x14_mf[] U8G2_FONT_SECTION("u8g2_font_7x14_mf");
extern const uint8_t u8g2_font_7x14_mr[] U8G2_FONT_SECTION("u8g2_font_7x14_mr");
extern const uint8_t u8g2_font_7x14_mn[] U8G2_FONT_SECTION("u8g2_font_7x14_mn");
extern const uint8_t u8g2_font_7x14B_tf[] U8G2_FONT_SECTION("u8g2_font_7x14B_tf");
extern const uint8_t u8g2_font_7x14B_tr[] U8G2_FONT_SECTION("u8g2_font_7x14B_tr");
extern const uint8_t u8g2_font_7x14B_tn[] U8G2_FONT_SECTION("u8g2_font_7x14B_tn");
extern const uint8_t u8g2_font_7x14B_mf[] U8G2_FONT_SECTION("u8g2_font_7x14B_mf");
extern const uint8_t u8g2_font_7x14B_mr[] U8G2_FONT_SECTION("u8g2_font_7x14B_mr");
extern const uint8_t u8g2_font_7x14B_mn[] U8G2_FONT_SECTION("u8g2_font_7x14B_mn");
extern const uint8_t u8g2_font_8x13_tf[] U8G2_FONT_SECTION("u8g2_font_8x13_tf");
extern const uint8_t u8g2_font_8x13_tr[] U8G2_FONT_SECTION("u8g2_font_8x13_tr");
extern const uint8_t u8g2_font_8x13_tn[] U8G2_FONT_SECTION("u8g2_font_8x13_tn");
extern const uint8_t u8g2_font_8x13_te[] U8G2_FONT_SECTION("u8g2_font_8x13_te");
extern const uint8_t u8g2_font_8x13_mf[] U8G2_FONT_SECTION("u8g2_font_8x13_mf");
extern const uint8_t u8g2_font_8x13_mr[] U8G2_FONT_SECTION("u8g2_font_8x13_mr");
extern const uint8_t u8g2_font_8x13_mn[] U8G2_FONT_SECTION("u8g2_font_8x13_mn");
extern const uint8_t u8g2_font_8x13_me[] U8G2_FONT_SECTION("u8g2_font_8x13_me");
extern const uint8_t u8g2_font_8x13_t_symbols[] U8G2_FONT_SECTION("u8g2_font_8x13_t_symbols");
extern const uint8_t u8g2_font_8x13B_tf[] U8G2_FONT_SECTION("u8g2_font_8x13B_tf");
extern const uint8_t u8g2_font_8x13B_tr[] U8G2_FONT_SECTION("u8g2_font_8x13B_tr");
extern const uint8_t u8g2_font_8x13B_tn[] U8G2_FONT_SECTION("u8g2_font_8x13B_tn");
extern const uint8_t u8g2_font_8x13B_mf[] U8G2_FONT_SECTION("u8g2_font_8x13B_mf");
extern const uint8_t u8g2_font_8x13B_mr[] U8G2_FONT_SECTION("u8g2_font_8x13B_mr");
extern const uint8_t u8g2_font_8x13B_mn[] U8G2_FONT_SECTION("u8g2_font_8x13B_mn");
extern const uint8_t u8g2_font_8x13O_tf[] U8G2_FONT_SECTION("u8g2_font_8x13O_tf");
extern const uint8_t u8g2_font_8x13O_tr[] U8G2_FONT_SECTION("u8g2_font_8x13O_tr");
extern const uint8_t u8g2_font_8x13O_tn[] U8G2_FONT_SECTION("u8g2_font_8x13O_tn");
extern const uint8_t u8g2_font_8x13O_mf[] U8G2_FONT_SECTION("u8g2_font_8x13O_mf");
extern const uint8_t u8g2_font_8x13O_mr[] U8G2_FONT_SECTION("u8g2_font_8x13O_mr");
extern const uint8_t u8g2_font_8x13O_mn[] U8G2_FONT_SECTION("u8g2_font_8x13O_mn");
extern const uint8_t u8g2_font_9x15_tf[] U8G2_FONT_SECTION("u8g2_font_9x15_tf");
extern const uint8_t u8g2_font_9x15_tr[] U8G2_FONT_SECTION("u8g2_font_9x15_tr");
extern const uint8_t u8g2_font_9x15_tn[] U8G2_FONT_SECTION("u8g2_font_9x15_tn");
extern const uint8_t u8g2_font_9x15_te[] U8G2_FONT_SECTION("u8g2_font_9x15_te");
extern const uint8_t u8g2_font_9x15_mf[] U8G2_FONT_SECTION("u8g2_font_9x15_mf");
extern const uint8_t u8g2_font_9x15_mr[] U8G2_FONT_SECTION("u8g2_font_9x15_mr");
extern const uint8_t u8g2_font_9x15_mn[] U8G2_FONT_SECTION("u8g2_font_9x15_mn");
extern const uint8_t u8g2_font_9x15_me[] U8G2_FONT_SECTION("u8g2_font_9x15_me");
extern const uint8_t u8g2_font_9x15_t_symbols[] U8G2_FONT_SECTION("u8g2_font_9x15_t_symbols");
extern const uint8_t u8g2_font_9x15B_tf[] U8G2_FONT_SECTION("u8g2_font_9x15B_tf");
extern const uint8_t u8g2_font_9x15B_tr[] U8G2_FONT_SECTION("u8g2_font_9x15B_tr");
extern const uint8_t u8g2_font_9x15B_tn[] U8G2_FONT_SECTION("u8g2_font_9x15B_tn");
extern const uint8_t u8g2_font_9x15B_mf[] U8G2_FONT_SECTION("u8g2_font_9x15B_mf");
extern const uint8_t u8g2_font_9x15B_mr[] U8G2_FONT_SECTION("u8g2_font_9x15B_mr");
extern const uint8_t u8g2_font_9x15B_mn[] U8G2_FONT_SECTION("u8g2_font_9x15B_mn");
extern const uint8_t u8g2_font_9x18_tf[] U8G2_FONT_SECTION("u8g2_font_9x18_tf");
extern const uint8_t u8g2_font_9x18_tr[] U8G2_FONT_SECTION("u8g2_font_9x18_tr");
extern const uint8_t u8g2_font_9x18_tn[] U8G2_FONT_SECTION("u8g2_font_9x18_tn");
extern const uint8_t u8g2_font_9x18_mf[] U8G2_FONT_SECTION("u8g2_font_9x18_mf");
extern const uint8_t u8g2_font_9x18_mr[] U8G2_FONT_SECTION("u8g2_font_9x18_mr");
extern const uint8_t u8g2_font_9x18_mn[] U8G2_FONT_SECTION("u8g2_font_9x18_mn");
extern const uint8_t u8g2_font_9x18B_tf[] U8G2_FONT_SECTION("u8g2_font_9x18B_tf");
extern const uint8_t u8g2_font_9x18B_tr[] U8G2_FONT_SECTION("u8g2_font_9x18B_tr");
extern const uint8_t u8g2_font_9x18B_tn[] U8G2_FONT_SECTION("u8g2_font_9x18B_tn");
extern const uint8_t u8g2_font_9x18B_mf[] U8G2_FONT_SECTION("u8g2_font_9x18B_mf");
extern const uint8_t u8g2_font_9x18B_mr[] U8G2_FONT_SECTION("u8g2_font_9x18B_mr");
extern const uint8_t u8g2_font_9x18B_mn[] U8G2_FONT_SECTION("u8g2_font_9x18B_mn");
extern const uint8_t u8g2_font_10x20_tf[] U8G2_FONT_SECTION("u8g2_font_10x20_tf");
extern const uint8_t u8g2_font_10x20_tr[] U8G2_FONT_SECTION("u8g2_font_10x20_tr");
extern const uint8_t u8g2_font_10x20_tn[] U8G2_FONT_SECTION("u8g2_font_10x20_tn");
extern const uint8_t u8g2_font_10x20_te[] U8G2_FONT_SECTION("u8g2_font_10x20_te");
extern const uint8_t u8g2_font_10x20_mf[] U8G2_FONT_SECTION("u8g2_font_10x20_mf");
extern const uint8_t u8g2_font_10x20_mr[] U8G2_FONT_SECTION("u8g2_font_10x20_mr");
extern const uint8_t u8g2_font_10x20_mn[] U8G2_FONT_SECTION("u8g2_font_10x20_mn");
extern const uint8_t u8g2_font_10x20_me[] U8G2_FONT_SECTION("u8g2_font_10x20_me");
extern const uint8_t u8g2_font_10x20_t_greek[] U8G2_FONT_SECTION("u8g2_font_10x20_t_greek");
extern const uint8_t u8g2_font_10x20_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_10x20_t_cyrillic");
extern const uint8_t u8g2_font_10x20_t_arabic[] U8G2_FONT_SECTION("u8g2_font_10x20_t_arabic");
extern const uint8_t u8g2_font_profont10_tf[] U8G2_FONT_SECTION("u8g2_font_profont10_tf");
extern const uint8_t u8g2_font_profont10_tr[] U8G2_FONT_SECTION("u8g2_font_profont10_tr");
extern const uint8_t u8g2_font_profont10_tn[] U8G2_FONT_SECTION("u8g2_font_profont10_tn");
extern const uint8_t u8g2_font_profont10_mf[] U8G2_FONT_SECTION("u8g2_font_profont10_mf");
extern const uint8_t u8g2_font_profont10_mr[] U8G2_FONT_SECTION("u8g2_font_profont10_mr");
extern const uint8_t u8g2_font_profont10_mn[] U8G2_FONT_SECTION("u8g2_font_profont10_mn");
extern const uint8_t u8g2_font_profont11_tf[] U8G2_FONT_SECTION("u8g2_font_profont11_tf");
extern const uint8_t u8g2_font_profont11_tr[] U8G2_FONT_SECTION("u8g2_font_profont11_tr");
extern const uint8_t u8g2_font_profont11_tn[] U8G2_FONT_SECTION("u8g2_font_profont11_tn");
extern const uint8_t u8g2_font_profont11_mf[] U8G2_FONT_SECTION("u8g2_font_profont11_mf");
extern const uint8_t u8g2_font_profont11_mr[] U8G2_FONT_SECTION("u8g2_font_profont11_mr");
extern const uint8_t u8g2_font_profont11_mn[] U8G2_FONT_SECTION("u8g2_font_profont11_mn");
extern const uint8_t u8g2_font_profont12_tf[] U8G2_FONT_SECTION("u8g2_font_profont12_tf");
extern const uint8_t u8g2_font_profont12_tr[] U8G2_FONT_SECTION("u8g2_font_profont12_tr");
extern const uint8_t u8g2_font_profont12_tn[] U8G2_FONT_SECTION("u8g2_font_profont12_tn");
extern const uint8_t u8g2_font_profont12_mf[] U8G2_FONT_SECTION("u8g2_font_profont12_mf");
extern const uint8_t u8g2_font_profont12_mr[] U8G2_FONT_SECTION("u8g2_font_profont12_mr");
extern const uint8_t u8g2_font_profont12_mn[] U8G2_FONT_SECTION("u8g2_font_profont12_mn");
extern const uint8_t u8g2_font_profont15_tf[] U8G2_FONT_SECTION("u8g2_font_profont15_tf");
extern const uint8_t u8g2_font_profont15_tr[] U8G2_FONT_SECTION("u8g2_font_profont15_tr");
extern const uint8_t u8g2_font_profont15_tn[] U8G2_FONT_SECTION("u8g2_font_profont15_tn");
extern const uint8_t u8g2_font_profont15_mf[] U8G2_FONT_SECTION("u8g2_font_profont15_mf");
extern const uint8_t u8g2_font_profont15_mr[] U8G2_FONT_SECTION("u8g2_font_profont15_mr");
extern const uint8_t u8g2_font_profont15_mn[] U8G2_FONT_SECTION("u8g2_font_profont15_mn");
extern const uint8_t u8g2_font_profont17_tf[] U8G2_FONT_SECTION("u8g2_font_profont17_tf");
extern const uint8_t u8g2_font_profont17_tr[] U8G2_FONT_SECTION("u8g2_font_profont17_tr");
extern const uint8_t u8g2_font_profont17_tn[] U8G2_FONT_SECTION("u8g2_font_profont17_tn");
extern const uint8_t u8g2_font_profont17_mf[] U8G2_FONT_SECTION("u8g2_font_profont17_mf");
extern const uint8_t u8g2_font_profont17_mr[] U8G2_FONT_SECTION("u8g2_font_profont17_mr");
extern const uint8_t u8g2_font_profont17_mn[] U8G2_FONT_SECTION("u8g2_font_profont17_mn");
extern const uint8_t u8g2_font_profont22_tf[] U8G2_FONT_SECTION("u8g2_font_profont22_tf");
extern const uint8_t u8g2_font_profont22_tr[] U8G2_FONT_SECTION("u8g2_font_profont22_tr");
extern const uint8_t u8g2_font_profont22_tn[] U8G2_FONT_SECTION("u8g2_font_profont22_tn");
extern const uint8_t u8g2_font_profont22_mf[] U8G2_FONT_SECTION("u8g2_font_profont22_mf");
extern const uint8_t u8g2_font_profont22_mr[] U8G2_FONT_SECTION("u8g2_font_profont22_mr");
extern const uint8_t u8g2_font_profont22_mn[] U8G2_FONT_SECTION("u8g2_font_profont22_mn");
extern const uint8_t u8g2_font_profont29_tf[] U8G2_FONT_SECTION("u8g2_font_profont29_tf");
extern const uint8_t u8g2_font_profont29_tr[] U8G2_FONT_SECTION("u8g2_font_profont29_tr");
extern const uint8_t u8g2_font_profont29_tn[] U8G2_FONT_SECTION("u8g2_font_profont29_tn");
extern const uint8_t u8g2_font_profont29_mf[] U8G2_FONT_SECTION("u8g2_font_profont29_mf");
extern const uint8_t u8g2_font_profont29_mr[] U8G2_FONT_SECTION("u8g2_font_profont29_mr");
extern const uint8_t u8g2_font_profont29_mn[] U8G2_FONT_SECTION("u8g2_font_profont29_mn");
extern const uint8_t u8g2_font_amstrad_cpc_extended_8f[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_extended_8f");
extern const uint8_t u8g2_font_amstrad_cpc_extended_8r[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_extended_8r");
extern const uint8_t u8g2_font_amstrad_cpc_extended_8n[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_extended_8n");
extern const uint8_t u8g2_font_amstrad_cpc_extended_8u[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_extended_8u");
extern const uint8_t u8g2_font_mozart_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_mozart_nbp_tf");
extern const uint8_t u8g2_font_mozart_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_mozart_nbp_tr");
extern const uint8_t u8g2_font_mozart_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_mozart_nbp_tn");
extern const uint8_t u8g2_font_mozart_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_mozart_nbp_t_all");
extern const uint8_t u8g2_font_glasstown_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_glasstown_nbp_tf");
extern const uint8_t u8g2_font_glasstown_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_glasstown_nbp_tr");
extern const uint8_t u8g2_font_glasstown_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_glasstown_nbp_tn");
extern const uint8_t u8g2_font_glasstown_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_glasstown_nbp_t_all");
extern const uint8_t u8g2_font_shylock_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_shylock_nbp_tf");
extern const uint8_t u8g2_font_shylock_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_shylock_nbp_tr");
extern const uint8_t u8g2_font_shylock_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_shylock_nbp_tn");
extern const uint8_t u8g2_font_shylock_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_shylock_nbp_t_all");
extern const uint8_t u8g2_font_roentgen_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_roentgen_nbp_tf");
extern const uint8_t u8g2_font_roentgen_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_roentgen_nbp_tr");
extern const uint8_t u8g2_font_roentgen_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_roentgen_nbp_tn");
extern const uint8_t u8g2_font_roentgen_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_roentgen_nbp_t_all");
extern const uint8_t u8g2_font_calibration_gothic_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_calibration_gothic_nbp_tf");
extern const uint8_t u8g2_font_calibration_gothic_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_calibration_gothic_nbp_tr");
extern const uint8_t u8g2_font_calibration_gothic_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_calibration_gothic_nbp_tn");
extern const uint8_t u8g2_font_calibration_gothic_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_calibration_gothic_nbp_t_all");
extern const uint8_t u8g2_font_smart_patrol_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_smart_patrol_nbp_tf");
extern const uint8_t u8g2_font_smart_patrol_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_smart_patrol_nbp_tr");
extern const uint8_t u8g2_font_smart_patrol_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_smart_patrol_nbp_tn");
extern const uint8_t u8g2_font_prospero_bold_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_prospero_bold_nbp_tf");
extern const uint8_t u8g2_font_prospero_bold_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_prospero_bold_nbp_tr");
extern const uint8_t u8g2_font_prospero_bold_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_prospero_bold_nbp_tn");
extern const uint8_t u8g2_font_prospero_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_prospero_nbp_tf");
extern const uint8_t u8g2_font_prospero_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_prospero_nbp_tr");
extern const uint8_t u8g2_font_prospero_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_prospero_nbp_tn");
extern const uint8_t u8g2_font_balthasar_regular_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_balthasar_regular_nbp_tf");
extern const uint8_t u8g2_font_balthasar_regular_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_balthasar_regular_nbp_tr");
extern const uint8_t u8g2_font_balthasar_regular_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_balthasar_regular_nbp_tn");
extern const uint8_t u8g2_font_balthasar_titling_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_balthasar_titling_nbp_tf");
extern const uint8_t u8g2_font_balthasar_titling_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_balthasar_titling_nbp_tr");
extern const uint8_t u8g2_font_balthasar_titling_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_balthasar_titling_nbp_tn");
extern const uint8_t u8g2_font_synchronizer_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_synchronizer_nbp_tf");
extern const uint8_t u8g2_font_synchronizer_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_synchronizer_nbp_tr");
extern const uint8_t u8g2_font_synchronizer_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_synchronizer_nbp_tn");
extern const uint8_t u8g2_font_mercutio_basic_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_mercutio_basic_nbp_tf");
extern const uint8_t u8g2_font_mercutio_basic_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_mercutio_basic_nbp_tr");
extern const uint8_t u8g2_font_mercutio_basic_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_mercutio_basic_nbp_tn");
extern const uint8_t u8g2_font_mercutio_basic_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_mercutio_basic_nbp_t_all");
extern const uint8_t u8g2_font_mercutio_sc_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_mercutio_sc_nbp_tf");
extern const uint8_t u8g2_font_mercutio_sc_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_mercutio_sc_nbp_tr");
extern const uint8_t u8g2_font_mercutio_sc_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_mercutio_sc_nbp_tn");
extern const uint8_t u8g2_font_mercutio_sc_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_mercutio_sc_nbp_t_all");
extern const uint8_t u8g2_font_miranda_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_miranda_nbp_tf");
extern const uint8_t u8g2_font_miranda_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_miranda_nbp_tr");
extern const uint8_t u8g2_font_miranda_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_miranda_nbp_tn");
extern const uint8_t u8g2_font_nine_by_five_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_nine_by_five_nbp_tf");
extern const uint8_t u8g2_font_nine_by_five_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_nine_by_five_nbp_tr");
extern const uint8_t u8g2_font_nine_by_five_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_nine_by_five_nbp_tn");
extern const uint8_t u8g2_font_nine_by_five_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_nine_by_five_nbp_t_all");
extern const uint8_t u8g2_font_rosencrantz_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_rosencrantz_nbp_tf");
extern const uint8_t u8g2_font_rosencrantz_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_rosencrantz_nbp_tr");
extern const uint8_t u8g2_font_rosencrantz_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_rosencrantz_nbp_tn");
extern const uint8_t u8g2_font_rosencrantz_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_rosencrantz_nbp_t_all");
extern const uint8_t u8g2_font_guildenstern_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_guildenstern_nbp_tf");
extern const uint8_t u8g2_font_guildenstern_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_guildenstern_nbp_tr");
extern const uint8_t u8g2_font_guildenstern_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_guildenstern_nbp_tn");
extern const uint8_t u8g2_font_guildenstern_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_guildenstern_nbp_t_all");
extern const uint8_t u8g2_font_astragal_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_astragal_nbp_tf");
extern const uint8_t u8g2_font_astragal_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_astragal_nbp_tr");
extern const uint8_t u8g2_font_astragal_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_astragal_nbp_tn");
extern const uint8_t u8g2_font_etl14thai_t[] U8G2_FONT_SECTION("u8g2_font_etl14thai_t");
extern const uint8_t u8g2_font_etl16thai_t[] U8G2_FONT_SECTION("u8g2_font_etl16thai_t");
extern const uint8_t u8g2_font_etl24thai_t[] U8G2_FONT_SECTION("u8g2_font_etl24thai_t");
extern const uint8_t u8g2_font_crox1cb_tf[] U8G2_FONT_SECTION("u8g2_font_crox1cb_tf");
extern const uint8_t u8g2_font_crox1cb_tr[] U8G2_FONT_SECTION("u8g2_font_crox1cb_tr");
extern const uint8_t u8g2_font_crox1cb_tn[] U8G2_FONT_SECTION("u8g2_font_crox1cb_tn");
extern const uint8_t u8g2_font_crox1cb_mf[] U8G2_FONT_SECTION("u8g2_font_crox1cb_mf");
extern const uint8_t u8g2_font_crox1cb_mr[] U8G2_FONT_SECTION("u8g2_font_crox1cb_mr");
extern const uint8_t u8g2_font_crox1cb_mn[] U8G2_FONT_SECTION("u8g2_font_crox1cb_mn");
extern const uint8_t u8g2_font_crox1c_tf[] U8G2_FONT_SECTION("u8g2_font_crox1c_tf");
extern const uint8_t u8g2_font_crox1c_tr[] U8G2_FONT_SECTION("u8g2_font_crox1c_tr");
extern const uint8_t u8g2_font_crox1c_tn[] U8G2_FONT_SECTION("u8g2_font_crox1c_tn");
extern const uint8_t u8g2_font_crox1c_mf[] U8G2_FONT_SECTION("u8g2_font_crox1c_mf");
extern const uint8_t u8g2_font_crox1c_mr[] U8G2_FONT_SECTION("u8g2_font_crox1c_mr");
extern const uint8_t u8g2_font_crox1c_mn[] U8G2_FONT_SECTION("u8g2_font_crox1c_mn");
extern const uint8_t u8g2_font_crox1hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox1hb_tf");
extern const uint8_t u8g2_font_crox1hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox1hb_tr");
extern const uint8_t u8g2_font_crox1hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox1hb_tn");
extern const uint8_t u8g2_font_crox1h_tf[] U8G2_FONT_SECTION("u8g2_font_crox1h_tf");
extern const uint8_t u8g2_font_crox1h_tr[] U8G2_FONT_SECTION("u8g2_font_crox1h_tr");
extern const uint8_t u8g2_font_crox1h_tn[] U8G2_FONT_SECTION("u8g2_font_crox1h_tn");
extern const uint8_t u8g2_font_crox1tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox1tb_tf");
extern const uint8_t u8g2_font_crox1tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox1tb_tr");
extern const uint8_t u8g2_font_crox1tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox1tb_tn");
extern const uint8_t u8g2_font_crox1t_tf[] U8G2_FONT_SECTION("u8g2_font_crox1t_tf");
extern const uint8_t u8g2_font_crox1t_tr[] U8G2_FONT_SECTION("u8g2_font_crox1t_tr");
extern const uint8_t u8g2_font_crox1t_tn[] U8G2_FONT_SECTION("u8g2_font_crox1t_tn");
extern const uint8_t u8g2_font_crox2cb_tf[] U8G2_FONT_SECTION("u8g2_font_crox2cb_tf");
extern const uint8_t u8g2_font_crox2cb_tr[] U8G2_FONT_SECTION("u8g2_font_crox2cb_tr");
extern const uint8_t u8g2_font_crox2cb_tn[] U8G2_FONT_SECTION("u8g2_font_crox2cb_tn");
extern const uint8_t u8g2_font_crox2cb_mf[] U8G2_FONT_SECTION("u8g2_font_crox2cb_mf");
extern const uint8_t u8g2_font_crox2cb_mr[] U8G2_FONT_SECTION("u8g2_font_crox2cb_mr");
extern const uint8_t u8g2_font_crox2cb_mn[] U8G2_FONT_SECTION("u8g2_font_crox2cb_mn");
extern const uint8_t u8g2_font_crox2c_tf[] U8G2_FONT_SECTION("u8g2_font_crox2c_tf");
extern const uint8_t u8g2_font_crox2c_tr[] U8G2_FONT_SECTION("u8g2_font_crox2c_tr");
extern const uint8_t u8g2_font_crox2c_tn[] U8G2_FONT_SECTION("u8g2_font_crox2c_tn");
extern const uint8_t u8g2_font_crox2c_mf[] U8G2_FONT_SECTION("u8g2_font_crox2c_mf");
extern const uint8_t u8g2_font_crox2c_mr[] U8G2_FONT_SECTION("u8g2_font_crox2c_mr");
extern const uint8_t u8g2_font_crox2c_mn[] U8G2_FONT_SECTION("u8g2_font_crox2c_mn");
extern const uint8_t u8g2_font_crox2hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox2hb_tf");
extern const uint8_t u8g2_font_crox2hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox2hb_tr");
extern const uint8_t u8g2_font_crox2hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox2hb_tn");
extern const uint8_t u8g2_font_crox2h_tf[] U8G2_FONT_SECTION("u8g2_font_crox2h_tf");
extern const uint8_t u8g2_font_crox2h_tr[] U8G2_FONT_SECTION("u8g2_font_crox2h_tr");
extern const uint8_t u8g2_font_crox2h_tn[] U8G2_FONT_SECTION("u8g2_font_crox2h_tn");
extern const uint8_t u8g2_font_crox2tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox2tb_tf");
extern const uint8_t u8g2_font_crox2tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox2tb_tr");
extern const uint8_t u8g2_font_crox2tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox2tb_tn");
extern const uint8_t u8g2_font_crox2t_tf[] U8G2_FONT_SECTION("u8g2_font_crox2t_tf");
extern const uint8_t u8g2_font_crox2t_tr[] U8G2_FONT_SECTION("u8g2_font_crox2t_tr");
extern const uint8_t u8g2_font_crox2t_tn[] U8G2_FONT_SECTION("u8g2_font_crox2t_tn");
extern const uint8_t u8g2_font_crox3cb_tf[] U8G2_FONT_SECTION("u8g2_font_crox3cb_tf");
extern const uint8_t u8g2_font_crox3cb_tr[] U8G2_FONT_SECTION("u8g2_font_crox3cb_tr");
extern const uint8_t u8g2_font_crox3cb_tn[] U8G2_FONT_SECTION("u8g2_font_crox3cb_tn");
extern const uint8_t u8g2_font_crox3cb_mf[] U8G2_FONT_SECTION("u8g2_font_crox3cb_mf");
extern const uint8_t u8g2_font_crox3cb_mr[] U8G2_FONT_SECTION("u8g2_font_crox3cb_mr");
extern const uint8_t u8g2_font_crox3cb_mn[] U8G2_FONT_SECTION("u8g2_font_crox3cb_mn");
extern const uint8_t u8g2_font_crox3c_tf[] U8G2_FONT_SECTION("u8g2_font_crox3c_tf");
extern const uint8_t u8g2_font_crox3c_tr[] U8G2_FONT_SECTION("u8g2_font_crox3c_tr");
extern const uint8_t u8g2_font_crox3c_tn[] U8G2_FONT_SECTION("u8g2_font_crox3c_tn");
extern const uint8_t u8g2_font_crox3c_mf[] U8G2_FONT_SECTION("u8g2_font_crox3c_mf");
extern const uint8_t u8g2_font_crox3c_mr[] U8G2_FONT_SECTION("u8g2_font_crox3c_mr");
extern const uint8_t u8g2_font_crox3c_mn[] U8G2_FONT_SECTION("u8g2_font_crox3c_mn");
extern const uint8_t u8g2_font_crox3hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox3hb_tf");
extern const uint8_t u8g2_font_crox3hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox3hb_tr");
extern const uint8_t u8g2_font_crox3hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox3hb_tn");
extern const uint8_t u8g2_font_crox3h_tf[] U8G2_FONT_SECTION("u8g2_font_crox3h_tf");
extern const uint8_t u8g2_font_crox3h_tr[] U8G2_FONT_SECTION("u8g2_font_crox3h_tr");
extern const uint8_t u8g2_font_crox3h_tn[] U8G2_FONT_SECTION("u8g2_font_crox3h_tn");
extern const uint8_t u8g2_font_crox3tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox3tb_tf");
extern const uint8_t u8g2_font_crox3tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox3tb_tr");
extern const uint8_t u8g2_font_crox3tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox3tb_tn");
extern const uint8_t u8g2_font_crox3t_tf[] U8G2_FONT_SECTION("u8g2_font_crox3t_tf");
extern const uint8_t u8g2_font_crox3t_tr[] U8G2_FONT_SECTION("u8g2_font_crox3t_tr");
extern const uint8_t u8g2_font_crox3t_tn[] U8G2_FONT_SECTION("u8g2_font_crox3t_tn");
extern const uint8_t u8g2_font_crox4hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox4hb_tf");
extern const uint8_t u8g2_font_crox4hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox4hb_tr");
extern const uint8_t u8g2_font_crox4hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox4hb_tn");
extern const uint8_t u8g2_font_crox4h_tf[] U8G2_FONT_SECTION("u8g2_font_crox4h_tf");
extern const uint8_t u8g2_font_crox4h_tr[] U8G2_FONT_SECTION("u8g2_font_crox4h_tr");
extern const uint8_t u8g2_font_crox4h_tn[] U8G2_FONT_SECTION("u8g2_font_crox4h_tn");
extern const uint8_t u8g2_font_crox4tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox4tb_tf");
extern const uint8_t u8g2_font_crox4tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox4tb_tr");
extern const uint8_t u8g2_font_crox4tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox4tb_tn");
extern const uint8_t u8g2_font_crox4t_tf[] U8G2_FONT_SECTION("u8g2_font_crox4t_tf");
extern const uint8_t u8g2_font_crox4t_tr[] U8G2_FONT_SECTION("u8g2_font_crox4t_tr");
extern const uint8_t u8g2_font_crox4t_tn[] U8G2_FONT_SECTION("u8g2_font_crox4t_tn");
extern const uint8_t u8g2_font_crox5hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox5hb_tf");
extern const uint8_t u8g2_font_crox5hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox5hb_tr");
extern const uint8_t u8g2_font_crox5hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox5hb_tn");
extern const uint8_t u8g2_font_crox5h_tf[] U8G2_FONT_SECTION("u8g2_font_crox5h_tf");
extern const uint8_t u8g2_font_crox5h_tr[] U8G2_FONT_SECTION("u8g2_font_crox5h_tr");
extern const uint8_t u8g2_font_crox5h_tn[] U8G2_FONT_SECTION("u8g2_font_crox5h_tn");
extern const uint8_t u8g2_font_crox5tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox5tb_tf");
extern const uint8_t u8g2_font_crox5tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox5tb_tr");
extern const uint8_t u8g2_font_crox5tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox5tb_tn");
extern const uint8_t u8g2_font_crox5t_tf[] U8G2_FONT_SECTION("u8g2_font_crox5t_tf");
extern const uint8_t u8g2_font_crox5t_tr[] U8G2_FONT_SECTION("u8g2_font_crox5t_tr");
extern const uint8_t u8g2_font_crox5t_tn[] U8G2_FONT_SECTION("u8g2_font_crox5t_tn");
extern const uint8_t u8g2_font_cu12_tf[] U8G2_FONT_SECTION("u8g2_font_cu12_tf");
extern const uint8_t u8g2_font_cu12_tr[] U8G2_FONT_SECTION("u8g2_font_cu12_tr");
extern const uint8_t u8g2_font_cu12_tn[] U8G2_FONT_SECTION("u8g2_font_cu12_tn");
extern const uint8_t u8g2_font_cu12_te[] U8G2_FONT_SECTION("u8g2_font_cu12_te");
extern const uint8_t u8g2_font_cu12_hf[] U8G2_FONT_SECTION("u8g2_font_cu12_hf");
extern const uint8_t u8g2_font_cu12_hr[] U8G2_FONT_SECTION("u8g2_font_cu12_hr");
extern const uint8_t u8g2_font_cu12_hn[] U8G2_FONT_SECTION("u8g2_font_cu12_hn");
extern const uint8_t u8g2_font_cu12_he[] U8G2_FONT_SECTION("u8g2_font_cu12_he");
extern const uint8_t u8g2_font_cu12_mf[] U8G2_FONT_SECTION("u8g2_font_cu12_mf");
extern const uint8_t u8g2_font_cu12_mr[] U8G2_FONT_SECTION("u8g2_font_cu12_mr");
extern const uint8_t u8g2_font_cu12_mn[] U8G2_FONT_SECTION("u8g2_font_cu12_mn");
extern const uint8_t u8g2_font_cu12_me[] U8G2_FONT_SECTION("u8g2_font_cu12_me");
extern const uint8_t u8g2_font_cu12_t_symbols[] U8G2_FONT_SECTION("u8g2_font_cu12_t_symbols");
extern const uint8_t u8g2_font_cu12_t_greek[] U8G2_FONT_SECTION("u8g2_font_cu12_t_greek");
extern const uint8_t u8g2_font_cu12_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_cu12_t_cyrillic");
extern const uint8_t u8g2_font_cu12_t_hebrew[] U8G2_FONT_SECTION("u8g2_font_cu12_t_hebrew");
extern const uint8_t u8g2_font_cu12_t_arabic[] U8G2_FONT_SECTION("u8g2_font_cu12_t_arabic");
extern const uint8_t u8g2_font_unifont_t_latin[] U8G2_FONT_SECTION("u8g2_font_unifont_t_latin");
extern const uint8_t u8g2_font_unifont_t_extended[] U8G2_FONT_SECTION("u8g2_font_unifont_t_extended");
extern const uint8_t u8g2_font_unifont_t_greek[] U8G2_FONT_SECTION("u8g2_font_unifont_t_greek");
extern const uint8_t u8g2_font_unifont_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_unifont_t_cyrillic");
extern const uint8_t u8g2_font_unifont_t_hebrew[] U8G2_FONT_SECTION("u8g2_font_unifont_t_hebrew");
extern const uint8_t u8g2_font_unifont_t_bengali[] U8G2_FONT_SECTION("u8g2_font_unifont_t_bengali");
extern const uint8_t u8g2_font_unifont_t_urdu[] U8G2_FONT_SECTION("u8g2_font_unifont_t_urdu");
extern const uint8_t u8g2_font_unifont_t_polish[] U8G2_FONT_SECTION("u8g2_font_unifont_t_polish");
extern const uint8_t u8g2_font_unifont_t_arabic[] U8G2_FONT_SECTION("u8g2_font_unifont_t_arabic");
extern const uint8_t u8g2_font_unifont_t_chinese1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_chinese1");
extern const uint8_t u8g2_font_unifont_t_chinese2[] U8G2_FONT_SECTION("u8g2_font_unifont_t_chinese2");
extern const uint8_t u8g2_font_unifont_t_chinese3[] U8G2_FONT_SECTION("u8g2_font_unifont_t_chinese3");
extern const uint8_t u8g2_font_unifont_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_japanese1");
extern const uint8_t u8g2_font_unifont_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_unifont_t_japanese2");
extern const uint8_t u8g2_font_unifont_t_korean1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_korean1");
extern const uint8_t u8g2_font_unifont_t_korean2[] U8G2_FONT_SECTION("u8g2_font_unifont_t_korean2");
extern const uint8_t u8g2_font_gb16st_t_1[] U8G2_FONT_SECTION("u8g2_font_gb16st_t_1");
extern const uint8_t u8g2_font_gb16st_t_2[] U8G2_FONT_SECTION("u8g2_font_gb16st_t_2");
extern const uint8_t u8g2_font_gb16st_t_3[] U8G2_FONT_SECTION("u8g2_font_gb16st_t_3");
extern const uint8_t u8g2_font_gb24st_t_1[] U8G2_FONT_SECTION("u8g2_font_gb24st_t_1");
extern const uint8_t u8g2_font_gb24st_t_2[] U8G2_FONT_SECTION("u8g2_font_gb24st_t_2");
extern const uint8_t u8g2_font_gb24st_t_3[] U8G2_FONT_SECTION("u8g2_font_gb24st_t_3");
extern const uint8_t u8g2_font_b10_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b10_t_japanese1");
extern const uint8_t u8g2_font_b10_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b10_t_japanese2");
extern const uint8_t u8g2_font_b10_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b10_b_t_japanese1");
extern const uint8_t u8g2_font_b10_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b10_b_t_japanese2");
extern const uint8_t u8g2_font_f10_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f10_t_japanese1");
extern const uint8_t u8g2_font_f10_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f10_t_japanese2");
extern const uint8_t u8g2_font_f10_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f10_b_t_japanese1");
extern const uint8_t u8g2_font_f10_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f10_b_t_japanese2");
extern const uint8_t u8g2_font_b12_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b12_t_japanese1");
extern const uint8_t u8g2_font_b12_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b12_t_japanese2");
extern const uint8_t u8g2_font_b12_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b12_b_t_japanese1");
extern const uint8_t u8g2_font_b12_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b12_b_t_japanese2");
extern const uint8_t u8g2_font_f12_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f12_t_japanese1");
extern const uint8_t u8g2_font_f12_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f12_t_japanese2");
extern const uint8_t u8g2_font_f12_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f12_b_t_japanese1");
extern const uint8_t u8g2_font_f12_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f12_b_t_japanese2");
extern const uint8_t u8g2_font_b16_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b16_t_japanese1");
extern const uint8_t u8g2_font_b16_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b16_t_japanese2");
extern const uint8_t u8g2_font_b16_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b16_b_t_japanese1");
extern const uint8_t u8g2_font_b16_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b16_b_t_japanese2");
extern const uint8_t u8g2_font_f16_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f16_t_japanese1");
extern const uint8_t u8g2_font_f16_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f16_t_japanese2");
extern const uint8_t u8g2_font_f16_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f16_b_t_japanese1");
extern const uint8_t u8g2_font_f16_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f16_b_t_japanese2");
extern const uint8_t u8g2_font_unifont_t_symbols[] U8G2_FONT_SECTION("u8g2_font_unifont_t_symbols");
extern const uint8_t u8g2_font_artossans8_8r[] U8G2_FONT_SECTION("u8g2_font_artossans8_8r");
extern const uint8_t u8g2_font_artossans8_8n[] U8G2_FONT_SECTION("u8g2_font_artossans8_8n");
extern const uint8_t u8g2_font_artossans8_8u[] U8G2_FONT_SECTION("u8g2_font_artossans8_8u");
extern const uint8_t u8g2_font_artosserif8_8r[] U8G2_FONT_SECTION("u8g2_font_artosserif8_8r");
extern const uint8_t u8g2_font_artosserif8_8n[] U8G2_FONT_SECTION("u8g2_font_artosserif8_8n");
extern const uint8_t u8g2_font_artosserif8_8u[] U8G2_FONT_SECTION("u8g2_font_artosserif8_8u");
extern const uint8_t u8g2_font_chroma48medium8_8r[] U8G2_FONT_SECTION("u8g2_font_chroma48medium8_8r");
extern const uint8_t u8g2_font_chroma48medium8_8n[] U8G2_FONT_SECTION("u8g2_font_chroma48medium8_8n");
extern const uint8_t u8g2_font_chroma48medium8_8u[] U8G2_FONT_SECTION("u8g2_font_chroma48medium8_8u");
extern const uint8_t u8g2_font_saikyosansbold8_8n[] U8G2_FONT_SECTION("u8g2_font_saikyosansbold8_8n");
extern const uint8_t u8g2_font_saikyosansbold8_8u[] U8G2_FONT_SECTION("u8g2_font_saikyosansbold8_8u");
extern const uint8_t u8g2_font_torussansbold8_8r[] U8G2_FONT_SECTION("u8g2_font_torussansbold8_8r");
extern const uint8_t u8g2_font_torussansbold8_8n[] U8G2_FONT_SECTION("u8g2_font_torussansbold8_8n");
extern const uint8_t u8g2_font_torussansbold8_8u[] U8G2_FONT_SECTION("u8g2_font_torussansbold8_8u");
extern const uint8_t u8g2_font_victoriabold8_8r[] U8G2_FONT_SECTION("u8g2_font_victoriabold8_8r");
extern const uint8_t u8g2_font_victoriabold8_8n[] U8G2_FONT_SECTION("u8g2_font_victoriabold8_8n");
extern const uint8_t u8g2_font_victoriabold8_8u[] U8G2_FONT_SECTION("u8g2_font_victoriabold8_8u");
extern const uint8_t u8g2_font_victoriamedium8_8r[] U8G2_FONT_SECTION("u8g2_font_victoriamedium8_8r");
extern const uint8_t u8g2_font_victoriamedium8_8n[] U8G2_FONT_SECTION("u8g2_font_victoriamedium8_8n");
extern const uint8_t u8g2_font_victoriamedium8_8u[] U8G2_FONT_SECTION("u8g2_font_victoriamedium8_8u");
extern const uint8_t u8g2_font_courB08_tf[] U8G2_FONT_SECTION("u8g2_font_courB08_tf");
extern const uint8_t u8g2_font_courB08_tr[] U8G2_FONT_SECTION("u8g2_font_courB08_tr");
extern const uint8_t u8g2_font_courB08_tn[] U8G2_FONT_SECTION("u8g2_font_courB08_tn");
extern const uint8_t u8g2_font_courB10_tf[] U8G2_FONT_SECTION("u8g2_font_courB10_tf");
extern const uint8_t u8g2_font_courB10_tr[] U8G2_FONT_SECTION("u8g2_font_courB10_tr");
extern const uint8_t u8g2_font_courB10_tn[] U8G2_FONT_SECTION("u8g2_font_courB10_tn");
extern const uint8_t u8g2_font_courB12_tf[] U8G2_FONT_SECTION("u8g2_font_courB12_tf");
extern const uint8_t u8g2_font_courB12_tr[] U8G2_FONT_SECTION("u8g2_font_courB12_tr");
extern const uint8_t u8g2_font_courB12_tn[] U8G2_FONT_SECTION("u8g2_font_courB12_tn");
extern const uint8_t u8g2_font_courB14_tf[] U8G2_FONT_SECTION("u8g2_font_courB14_tf");
extern const uint8_t u8g2_font_courB14_tr[] U8G2_FONT_SECTION("u8g2_font_courB14_tr");
extern const uint8_t u8g2_font_courB14_tn[] U8G2_FONT_SECTION("u8g2_font_courB14_tn");
extern const uint8_t u8g2_font_courB18_tf[] U8G2_FONT_SECTION("u8g2_font_courB18_tf");
extern const uint8_t u8g2_font_courB18_tr[] U8G2_FONT_SECTION("u8g2_font_courB18_tr");
extern const uint8_t u8g2_font_courB18_tn[] U8G2_FONT_SECTION("u8g2_font_courB18_tn");
extern const uint8_t u8g2_font_courB24_tf[] U8G2_FONT_SECTION("u8g2_font_courB24_tf");
extern const uint8_t u8g2_font_courB24_tr[] U8G2_FONT_SECTION("u8g2_font_courB24_tr");
extern const uint8_t u8g2_font_courB24_tn[] U8G2_FONT_SECTION("u8g2_font_courB24_tn");
extern const uint8_t u8g2_font_courR08_tf[] U8G2_FONT_SECTION("u8g2_font_courR08_tf");
extern const uint8_t u8g2_font_courR08_tr[] U8G2_FONT_SECTION("u8g2_font_courR08_tr");
extern const uint8_t u8g2_font_courR08_tn[] U8G2_FONT_SECTION("u8g2_font_courR08_tn");
extern const uint8_t u8g2_font_courR10_tf[] U8G2_FONT_SECTION("u8g2_font_courR10_tf");
extern const uint8_t u8g2_font_courR10_tr[] U8G2_FONT_SECTION("u8g2_font_courR10_tr");
extern const uint8_t u8g2_font_courR10_tn[] U8G2_FONT_SECTION("u8g2_font_courR10_tn");
extern const uint8_t u8g2_font_courR12_tf[] U8G2_FONT_SECTION("u8g2_font_courR12_tf");
extern const uint8_t u8g2_font_courR12_tr[] U8G2_FONT_SECTION("u8g2_font_courR12_tr");
extern const uint8_t u8g2_font_courR12_tn[] U8G2_FONT_SECTION("u8g2_font_courR12_tn");
extern const uint8_t u8g2_font_courR14_tf[] U8G2_FONT_SECTION("u8g2_font_courR14_tf");
extern const uint8_t u8g2_font_courR14_tr[] U8G2_FONT_SECTION("u8g2_font_courR14_tr");
extern const uint8_t u8g2_font_courR14_tn[] U8G2_FONT_SECTION("u8g2_font_courR14_tn");
extern const uint8_t u8g2_font_courR18_tf[] U8G2_FONT_SECTION("u8g2_font_courR18_tf");
extern const uint8_t u8g2_font_courR18_tr[] U8G2_FONT_SECTION("u8g2_font_courR18_tr");
extern const uint8_t u8g2_font_courR18_tn[] U8G2_FONT_SECTION("u8g2_font_courR18_tn");
extern const uint8_t u8g2_font_courR24_tf[] U8G2_FONT_SECTION("u8g2_font_courR24_tf");
extern const uint8_t u8g2_font_courR24_tr[] U8G2_FONT_SECTION("u8g2_font_courR24_tr");
extern const uint8_t u8g2_font_courR24_tn[] U8G2_FONT_SECTION("u8g2_font_courR24_tn");
extern const uint8_t u8g2_font_helvB08_tf[] U8G2_FONT_SECTION("u8g2_font_helvB08_tf");
extern const uint8_t u8g2_font_helvB08_tr[] U8G2_FONT_SECTION("u8g2_font_helvB08_tr");
extern const uint8_t u8g2_font_helvB08_tn[] U8G2_FONT_SECTION("u8g2_font_helvB08_tn");
extern const uint8_t u8g2_font_helvB08_te[] U8G2_FONT_SECTION("u8g2_font_helvB08_te");
extern const uint8_t u8g2_font_helvB10_tf[] U8G2_FONT_SECTION("u8g2_font_helvB10_tf");
extern const uint8_t u8g2_font_helvB10_tr[] U8G2_FONT_SECTION("u8g2_font_helvB10_tr");
extern const uint8_t u8g2_font_helvB10_tn[] U8G2_FONT_SECTION("u8g2_font_helvB10_tn");
extern const uint8_t u8g2_font_helvB10_te[] U8G2_FONT_SECTION("u8g2_font_helvB10_te");
extern const uint8_t u8g2_font_helvB12_tf[] U8G2_FONT_SECTION("u8g2_font_helvB12_tf");
extern const uint8_t u8g2_font_helvB12_tr[] U8G2_FONT_SECTION("u8g2_font_helvB12_tr");
extern const uint8_t u8g2_font_helvB12_tn[] U8G2_FONT_SECTION("u8g2_font_helvB12_tn");
extern const uint8_t u8g2_font_helvB12_te[] U8G2_FONT_SECTION("u8g2_font_helvB12_te");
extern const uint8_t u8g2_font_helvB14_tf[] U8G2_FONT_SECTION("u8g2_font_helvB14_tf");
extern const uint8_t u8g2_font_helvB14_tr[] U8G2_FONT_SECTION("u8g2_font_helvB14_tr");
extern const uint8_t u8g2_font_helvB14_tn[] U8G2_FONT_SECTION("u8g2_font_helvB14_tn");
extern const uint8_t u8g2_font_helvB14_te[] U8G2_FONT_SECTION("u8g2_font_helvB14_te");
extern const uint8_t u8g2_font_helvB18_tf[] U8G2_FONT_SECTION("u8g2_font_helvB18_tf");
extern const uint8_t u8g2_font_helvB18_tr[] U8G2_FONT_SECTION("u8g2_font_helvB18_tr");
extern const uint8_t u8g2_font_helvB18_tn[] U8G2_FONT_SECTION("u8g2_font_helvB18_tn");
extern const uint8_t u8g2_font_helvB18_te[] U8G2_FONT_SECTION("u8g2_font_helvB18_te");
extern const uint8_t u8g2_font_helvB24_tf[] U8G2_FONT_SECTION("u8g2_font_helvB24_tf");
extern const uint8_t u8g2_font_helvB24_tr[] U8G2_FONT_SECTION("u8g2_font_helvB24_tr");
extern const uint8_t u8g2_font_helvB24_tn[] U8G2_FONT_SECTION("u8g2_font_helvB24_tn");
extern const uint8_t u8g2_font_helvB24_te[] U8G2_FONT_SECTION("u8g2_font_helvB24_te");
extern const uint8_t u8g2_font_helvR08_tf[] U8G2_FONT_SECTION("u8g2_font_helvR08_tf");
extern const uint8_t u8g2_font_helvR08_tr[] U8G2_FONT_SECTION("u8g2_font_helvR08_tr");
extern const uint8_t u8g2_font_helvR08_tn[] U8G2_FONT_SECTION("u8g2_font_helvR08_tn");
extern const uint8_t u8g2_font_helvR08_te[] U8G2_FONT_SECTION("u8g2_font_helvR08_te");
extern const uint8_t u8g2_font_helvR10_tf[] U8G2_FONT_SECTION("u8g2_font_helvR10_tf");
extern const uint8_t u8g2_font_helvR10_tr[] U8G2_FONT_SECTION("u8g2_font_helvR10_tr");
extern const uint8_t u8g2_font_helvR10_tn[] U8G2_FONT_SECTION("u8g2_font_helvR10_tn");
extern const uint8_t u8g2_font_helvR10_te[] U8G2_FONT_SECTION("u8g2_font_helvR10_te");
extern const uint8_t u8g2_font_helvR12_tf[] U8G2_FONT_SECTION("u8g2_font_helvR12_tf");
extern const uint8_t u8g2_font_helvR12_tr[] U8G2_FONT_SECTION("u8g2_font_helvR12_tr");
extern const uint8_t u8g2_font_helvR12_tn[] U8G2_FONT_SECTION("u8g2_font_helvR12_tn");
extern const uint8_t u8g2_font_helvR12_te[] U8G2_FONT_SECTION("u8g2_font_helvR12_te");
extern const uint8_t u8g2_font_helvR14_tf[] U8G2_FONT_SECTION("u8g2_font_helvR14_tf");
extern const uint8_t u8g2_font_helvR14_tr[] U8G2_FONT_SECTION("u8g2_font_helvR14_tr");
extern const uint8_t u8g2_font_helvR14_tn[] U8G2_FONT_SECTION("u8g2_font_helvR14_tn");
extern const uint8_t u8g2_font_helvR14_te[] U8G2_FONT_SECTION("u8g2_font_helvR14_te");
extern const uint8_t u8g2_font_helvR18_tf[] U8G2_FONT_SECTION("u8g2_font_helvR18_tf");
extern const uint8_t u8g2_font_helvR18_tr[] U8G2_FONT_SECTION("u8g2_font_helvR18_tr");
extern const uint8_t u8g2_font_helvR18_tn[] U8G2_FONT_SECTION("u8g2_font_helvR18_tn");
extern const uint8_t u8g2_font_helvR18_te[] U8G2_FONT_SECTION("u8g2_font_helvR18_te");
extern const uint8_t u8g2_font_helvR24_tf[] U8G2_FONT_SECTION("u8g2_font_helvR24_tf");
extern const uint8_t u8g2_font_helvR24_tr[] U8G2_FONT_SECTION("u8g2_font_helvR24_tr");
extern const uint8_t u8g2_font_helvR24_tn[] U8G2_FONT_SECTION("u8g2_font_helvR24_tn");
extern const uint8_t u8g2_font_helvR24_te[] U8G2_FONT_SECTION("u8g2_font_helvR24_te");
extern const uint8_t u8g2_font_ncenB08_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB08_tf");
extern const uint8_t u8g2_font_ncenB08_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB08_tr");
extern const uint8_t u8g2_font_ncenB08_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB08_tn");
extern const uint8_t u8g2_font_ncenB08_te[] U8G2_FONT_SECTION("u8g2_font_ncenB08_te");
extern const uint8_t u8g2_font_ncenB10_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB10_tf");
extern const uint8_t u8g2_font_ncenB10_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB10_tr");
extern const uint8_t u8g2_font_ncenB10_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB10_tn");
extern const uint8_t u8g2_font_ncenB10_te[] U8G2_FONT_SECTION("u8g2_font_ncenB10_te");
extern const uint8_t u8g2_font_ncenB12_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB12_tf");
extern const uint8_t u8g2_font_ncenB12_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB12_tr");
extern const uint8_t u8g2_font_ncenB12_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB12_tn");
extern const uint8_t u8g2_font_ncenB12_te[] U8G2_FONT_SECTION("u8g2_font_ncenB12_te");
extern const uint8_t u8g2_font_ncenB14_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB14_tf");
extern const uint8_t u8g2_font_ncenB14_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB14_tr");
extern const uint8_t u8g2_font_ncenB14_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB14_tn");
extern const uint8_t u8g2_font_ncenB14_te[] U8G2_FONT_SECTION("u8g2_font_ncenB14_te");
extern const uint8_t u8g2_font_ncenB18_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB18_tf");
extern const uint8_t u8g2_font_ncenB18_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB18_tr");
extern const uint8_t u8g2_font_ncenB18_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB18_tn");
extern const uint8_t u8g2_font_ncenB18_te[] U8G2_FONT_SECTION("u8g2_font_ncenB18_te");
extern const uint8_t u8g2_font_ncenB24_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB24_tf");
extern const uint8_t u8g2_font_ncenB24_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB24_tr");
extern const uint8_t u8g2_font_ncenB24_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB24_tn");
extern const uint8_t u8g2_font_ncenB24_te[] U8G2_FONT_SECTION("u8g2_font_ncenB24_te");
extern const uint8_t u8g2_font_ncenR08_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR08_tf");
extern const uint8_t u8g2_font_ncenR08_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR08_tr");
extern const uint8_t u8g2_font_ncenR08_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR08_tn");
extern const uint8_t u8g2_font_ncenR08_te[] U8G2_FONT_SECTION("u8g2_font_ncenR08_te");
extern const uint8_t u8g2_font_ncenR10_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR10_tf");
extern const uint8_t u8g2_font_ncenR10_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR10_tr");
extern const uint8_t u8g2_font_ncenR10_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR10_tn");
extern const uint8_t u8g2_font_ncenR10_te[] U8G2_FONT_SECTION("u8g2_font_ncenR10_te");
extern const uint8_t u8g2_font_ncenR12_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR12_tf");
extern const uint8_t u8g2_font_ncenR12_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR12_tr");
extern const uint8_t u8g2_font_ncenR12_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR12_tn");
extern const uint8_t u8g2_font_ncenR12_te[] U8G2_FONT_SECTION("u8g2_font_ncenR12_te");
extern const uint8_t u8g2_font_ncenR14_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR14_tf");
extern const uint8_t u8g2_font_ncenR14_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR14_tr");
extern const uint8_t u8g2_font_ncenR14_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR14_tn");
extern const uint8_t u8g2_font_ncenR14_te[] U8G2_FONT_SECTION("u8g2_font_ncenR14_te");
extern const uint8_t u8g2_font_ncenR18_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR18_tf");
extern const uint8_t u8g2_font_ncenR18_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR18_tr");
extern const uint8_t u8g2_font_ncenR18_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR18_tn");
extern const uint8_t u8g2_font_ncenR18_te[] U8G2_FONT_SECTION("u8g2_font_ncenR18_te");
extern const uint8_t u8g2_font_ncenR24_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR24_tf");
extern const uint8_t u8g2_font_ncenR24_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR24_tr");
extern const uint8_t u8g2_font_ncenR24_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR24_tn");
extern const uint8_t u8g2_font_ncenR24_te[] U8G2_FONT_SECTION("u8g2_font_ncenR24_te");
extern const uint8_t u8g2_font_timB08_tf[] U8G2_FONT_SECTION("u8g2_font_timB08_tf");
extern const uint8_t u8g2_font_timB08_tr[] U8G2_FONT_SECTION("u8g2_font_timB08_tr");
extern const uint8_t u8g2_font_timB08_tn[] U8G2_FONT_SECTION("u8g2_font_timB08_tn");
extern const uint8_t u8g2_font_timB10_tf[] U8G2_FONT_SECTION("u8g2_font_timB10_tf");
extern const uint8_t u8g2_font_timB10_tr[] U8G2_FONT_SECTION("u8g2_font_timB10_tr");
extern const uint8_t u8g2_font_timB10_tn[] U8G2_FONT_SECTION("u8g2_font_timB10_tn");
extern const uint8_t u8g2_font_timB12_tf[] U8G2_FONT_SECTION("u8g2_font_timB12_tf");
extern const uint8_t u8g2_font_timB12_tr[] U8G2_FONT_SECTION("u8g2_font_timB12_tr");
extern const uint8_t u8g2_font_timB12_tn[] U8G2_FONT_SECTION("u8g2_font_timB12_tn");
extern const uint8_t u8g2_font_timB14_tf[] U8G2_FONT_SECTION("u8g2_font_timB14_tf");
extern const uint8_t u8g2_font_timB14_tr[] U8G2_FONT_SECTION("u8g2_font_timB14_tr");
extern const uint8_t u8g2_font_timB14_tn[] U8G2_FONT_SECTION("u8g2_font_timB14_tn");
extern const uint8_t u8g2_font_timB18_tf[] U8G2_FONT_SECTION("u8g2_font_timB18_tf");
extern const uint8_t u8g2_font_timB18_tr[] U8G2_FONT_SECTION("u8g2_font_timB18_tr");
extern const uint8_t u8g2_font_timB18_tn[] U8G2_FONT_SECTION("u8g2_font_timB18_tn");
extern const uint8_t u8g2_font_timB24_tf[] U8G2_FONT_SECTION("u8g2_font_timB24_tf");
extern const uint8_t u8g2_font_timB24_tr[] U8G2_FONT_SECTION("u8g2_font_timB24_tr");
extern const uint8_t u8g2_font_timB24_tn[] U8G2_FONT_SECTION("u8g2_font_timB24_tn");
extern const uint8_t u8g2_font_timR08_tf[] U8G2_FONT_SECTION("u8g2_font_timR08_tf");
extern const uint8_t u8g2_font_timR08_tr[] U8G2_FONT_SECTION("u8g2_font_timR08_tr");
extern const uint8_t u8g2_font_timR08_tn[] U8G2_FONT_SECTION("u8g2_font_timR08_tn");
extern const uint8_t u8g2_font_timR10_tf[] U8G2_FONT_SECTION("u8g2_font_timR10_tf");
extern const uint8_t u8g2_font_timR10_tr[] U8G2_FONT_SECTION("u8g2_font_timR10_tr");
extern const uint8_t u8g2_font_timR10_tn[] U8G2_FONT_SECTION("u8g2_font_timR10_tn");
extern const uint8_t u8g2_font_timR12_tf[] U8G2_FONT_SECTION("u8g2_font_timR12_tf");
extern const uint8_t u8g2_font_timR12_tr[] U8G2_FONT_SECTION("u8g2_font_timR12_tr");
extern const uint8_t u8g2_font_timR12_tn[] U8G2_FONT_SECTION("u8g2_font_timR12_tn");
extern const uint8_t u8g2_font_timR14_tf[] U8G2_FONT_SECTION("u8g2_font_timR14_tf");
extern const uint8_t u8g2_font_timR14_tr[] U8G2_FONT_SECTION("u8g2_font_timR14_tr");
extern const uint8_t u8g2_font_timR14_tn[] U8G2_FONT_SECTION("u8g2_font_timR14_tn");
extern const uint8_t u8g2_font_timR18_tf[] U8G2_FONT_SECTION("u8g2_font_timR18_tf");
extern const uint8_t u8g2_font_timR18_tr[] U8G2_FONT_SECTION("u8g2_font_timR18_tr");
extern const uint8_t u8g2_font_timR18_tn[] U8G2_FONT_SECTION("u8g2_font_timR18_tn");
extern const uint8_t u8g2_font_timR24_tf[] U8G2_FONT_SECTION("u8g2_font_timR24_tf");
extern const uint8_t u8g2_font_timR24_tr[] U8G2_FONT_SECTION("u8g2_font_timR24_tr");
extern const uint8_t u8g2_font_timR24_tn[] U8G2_FONT_SECTION("u8g2_font_timR24_tn");
extern const uint8_t u8g2_font_baby_tf[] U8G2_FONT_SECTION("u8g2_font_baby_tf");
extern const uint8_t u8g2_font_baby_tr[] U8G2_FONT_SECTION("u8g2_font_baby_tr");
extern const uint8_t u8g2_font_baby_tn[] U8G2_FONT_SECTION("u8g2_font_baby_tn");
extern const uint8_t u8g2_font_blipfest_07_tr[] U8G2_FONT_SECTION("u8g2_font_blipfest_07_tr");
extern const uint8_t u8g2_font_blipfest_07_tn[] U8G2_FONT_SECTION("u8g2_font_blipfest_07_tn");
extern const uint8_t u8g2_font_chikita_tf[] U8G2_FONT_SECTION("u8g2_font_chikita_tf");
extern const uint8_t u8g2_font_chikita_tr[] U8G2_FONT_SECTION("u8g2_font_chikita_tr");
extern const uint8_t u8g2_font_chikita_tn[] U8G2_FONT_SECTION("u8g2_font_chikita_tn");
extern const uint8_t u8g2_font_lucasfont_alternate_tf[] U8G2_FONT_SECTION("u8g2_font_lucasfont_alternate_tf");
extern const uint8_t u8g2_font_lucasfont_alternate_tr[] U8G2_FONT_SECTION("u8g2_font_lucasfont_alternate_tr");
extern const uint8_t u8g2_font_lucasfont_alternate_tn[] U8G2_FONT_SECTION("u8g2_font_lucasfont_alternate_tn");
extern const uint8_t u8g2_font_p01type_tf[] U8G2_FONT_SECTION("u8g2_font_p01type_tf");
extern const uint8_t u8g2_font_p01type_tr[] U8G2_FONT_SECTION("u8g2_font_p01type_tr");
extern const uint8_t u8g2_font_p01type_tn[] U8G2_FONT_SECTION("u8g2_font_p01type_tn");
extern const uint8_t u8g2_font_pixelle_micro_tr[] U8G2_FONT_SECTION("u8g2_font_pixelle_micro_tr");
extern const uint8_t u8g2_font_pixelle_micro_tn[] U8G2_FONT_SECTION("u8g2_font_pixelle_micro_tn");
extern const uint8_t u8g2_font_robot_de_niro_tf[] U8G2_FONT_SECTION("u8g2_font_robot_de_niro_tf");
extern const uint8_t u8g2_font_robot_de_niro_tr[] U8G2_FONT_SECTION("u8g2_font_robot_de_niro_tr");
extern const uint8_t u8g2_font_robot_de_niro_tn[] U8G2_FONT_SECTION("u8g2_font_robot_de_niro_tn");
extern const uint8_t u8g2_font_trixel_square_tf[] U8G2_FONT_SECTION("u8g2_font_trixel_square_tf");
extern const uint8_t u8g2_font_trixel_square_tr[] U8G2_FONT_SECTION("u8g2_font_trixel_square_tr");
extern const uint8_t u8g2_font_trixel_square_tn[] U8G2_FONT_SECTION("u8g2_font_trixel_square_tn");
extern const uint8_t u8g2_font_haxrcorp4089_tr[] U8G2_FONT_SECTION("u8g2_font_haxrcorp4089_tr");
extern const uint8_t u8g2_font_haxrcorp4089_tn[] U8G2_FONT_SECTION("u8g2_font_haxrcorp4089_tn");
extern const uint8_t u8g2_font_haxrcorp4089_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_haxrcorp4089_t_cyrillic");
extern const uint8_t u8g2_font_fub11_tf[] U8G2_FONT_SECTION("u8g2_font_fub11_tf");
extern const uint8_t u8g2_font_fub11_tr[] U8G2_FONT_SECTION("u8g2_font_fub11_tr");
extern const uint8_t u8g2_font_fub11_tn[] U8G2_FONT_SECTION("u8g2_font_fub11_tn");
extern const uint8_t u8g2_font_fub14_tf[] U8G2_FONT_SECTION("u8g2_font_fub14_tf");
extern const uint8_t u8g2_font_fub14_tr[] U8G2_FONT_SECTION("u8g2_font_fub14_tr");
extern const uint8_t u8g2_font_fub14_tn[] U8G2_FONT_SECTION("u8g2_font_fub14_tn");
extern const uint8_t u8g2_font_fub17_tf[] U8G2_FONT_SECTION("u8g2_font_fub17_tf");
extern const uint8_t u8g2_font_fub17_tr[] U8G2_FONT_SECTION("u8g2_font_fub17_tr");
extern const uint8_t u8g2_font_fub17_tn[] U8G2_FONT_SECTION("u8g2_font_fub17_tn");
extern const uint8_t u8g2_font_fub20_tf[] U8G2_FONT_SECTION("u8g2_font_fub20_tf");
extern const uint8_t u8g2_font_fub20_tr[] U8G2_FONT_SECTION("u8g2_font_fub20_tr");
extern const uint8_t u8g2_font_fub20_tn[] U8G2_FONT_SECTION("u8g2_font_fub20_tn");
extern const uint8_t u8g2_font_fub25_tf[] U8G2_FONT_SECTION("u8g2_font_fub25_tf");
extern const uint8_t u8g2_font_fub25_tr[] U8G2_FONT_SECTION("u8g2_font_fub25_tr");
extern const uint8_t u8g2_font_fub25_tn[] U8G2_FONT_SECTION("u8g2_font_fub25_tn");
extern const uint8_t u8g2_font_fub30_tf[] U8G2_FONT_SECTION("u8g2_font_fub30_tf");
extern const uint8_t u8g2_font_fub30_tr[] U8G2_FONT_SECTION("u8g2_font_fub30_tr");
extern const uint8_t u8g2_font_fub30_tn[] U8G2_FONT_SECTION("u8g2_font_fub30_tn");
extern const uint8_t u8g2_font_fub35_tf[] U8G2_FONT_SECTION("u8g2_font_fub35_tf");
extern const uint8_t u8g2_font_fub35_tr[] U8G2_FONT_SECTION("u8g2_font_fub35_tr");
extern const uint8_t u8g2_font_fub35_tn[] U8G2_FONT_SECTION("u8g2_font_fub35_tn");
extern const uint8_t u8g2_font_fub42_tf[] U8G2_FONT_SECTION("u8g2_font_fub42_tf");
extern const uint8_t u8g2_font_fub42_tr[] U8G2_FONT_SECTION("u8g2_font_fub42_tr");
extern const uint8_t u8g2_font_fub42_tn[] U8G2_FONT_SECTION("u8g2_font_fub42_tn");
extern const uint8_t u8g2_font_fub49_tn[] U8G2_FONT_SECTION("u8g2_font_fub49_tn");
extern const uint8_t u8g2_font_fur11_tf[] U8G2_FONT_SECTION("u8g2_font_fur11_tf");
extern const uint8_t u8g2_font_fur11_tr[] U8G2_FONT_SECTION("u8g2_font_fur11_tr");
extern const uint8_t u8g2_font_fur11_tn[] U8G2_FONT_SECTION("u8g2_font_fur11_tn");
extern const uint8_t u8g2_font_fur14_tf[] U8G2_FONT_SECTION("u8g2_font_fur14_tf");
extern const uint8_t u8g2_font_fur14_tr[] U8G2_FONT_SECTION("u8g2_font_fur14_tr");
extern const uint8_t u8g2_font_fur14_tn[] U8G2_FONT_SECTION("u8g2_font_fur14_tn");
extern const uint8_t u8g2_font_fur17_tf[] U8G2_FONT_SECTION("u8g2_font_fur17_tf");
extern const uint8_t u8g2_font_fur17_tr[] U8G2_FONT_SECTION("u8g2_font_fur17_tr");
extern const uint8_t u8g2_font_fur17_tn[] U8G2_FONT_SECTION("u8g2_font_fur17_tn");
extern const uint8_t u8g2_font_fur20_tf[] U8G2_FONT_SECTION("u8g2_font_fur20_tf");
extern const uint8_t u8g2_font_fur20_tr[] U8G2_FONT_SECTION("u8g2_font_fur20_tr");
extern const uint8_t u8g2_font_fur20_tn[] U8G2_FONT_SECTION("u8g2_font_fur20_tn");
extern const uint8_t u8g2_font_fur25_tf[] U8G2_FONT_SECTION("u8g2_font_fur25_tf");
extern const uint8_t u8g2_font_fur25_tr[] U8G2_FONT_SECTION("u8g2_font_fur25_tr");
extern const uint8_t u8g2_font_fur25_tn[] U8G2_FONT_SECTION("u8g2_font_fur25_tn");
extern const uint8_t u8g2_font_fur30_tf[] U8G2_FONT_SECTION("u8g2_font_fur30_tf");
extern const uint8_t u8g2_font_fur30_tr[] U8G2_FONT_SECTION("u8g2_font_fur30_tr");
extern const uint8_t u8g2_font_fur30_tn[] U8G2_FONT_SECTION("u8g2_font_fur30_tn");
extern const uint8_t u8g2_font_fur35_tf[] U8G2_FONT_SECTION("u8g2_font_fur35_tf");
extern const uint8_t u8g2_font_fur35_tr[] U8G2_FONT_SECTION("u8g2_font_fur35_tr");
extern const uint8_t u8g2_font_fur35_tn[] U8G2_FONT_SECTION("u8g2_font_fur35_tn");
extern const uint8_t u8g2_font_fur42_tf[] U8G2_FONT_SECTION("u8g2_font_fur42_tf");
extern const uint8_t u8g2_font_fur42_tr[] U8G2_FONT_SECTION("u8g2_font_fur42_tr");
extern const uint8_t u8g2_font_fur42_tn[] U8G2_FONT_SECTION("u8g2_font_fur42_tn");
extern const uint8_t u8g2_font_fur49_tn[] U8G2_FONT_SECTION("u8g2_font_fur49_tn");
extern const uint8_t u8g2_font_osb18_tf[] U8G2_FONT_SECTION("u8g2_font_osb18_tf");
extern const uint8_t u8g2_font_osb18_tr[] U8G2_FONT_SECTION("u8g2_font_osb18_tr");
extern const uint8_t u8g2_font_osb18_tn[] U8G2_FONT_SECTION("u8g2_font_osb18_tn");
extern const uint8_t u8g2_font_osb21_tf[] U8G2_FONT_SECTION("u8g2_font_osb21_tf");
extern const uint8_t u8g2_font_osb21_tr[] U8G2_FONT_SECTION("u8g2_font_osb21_tr");
extern const uint8_t u8g2_font_osb21_tn[] U8G2_FONT_SECTION("u8g2_font_osb21_tn");
extern const uint8_t u8g2_font_osb26_tf[] U8G2_FONT_SECTION("u8g2_font_osb26_tf");
extern const uint8_t u8g2_font_osb26_tr[] U8G2_FONT_SECTION("u8g2_font_osb26_tr");
extern const uint8_t u8g2_font_osb26_tn[] U8G2_FONT_SECTION("u8g2_font_osb26_tn");
extern const uint8_t u8g2_font_osb29_tf[] U8G2_FONT_SECTION("u8g2_font_osb29_tf");
extern const uint8_t u8g2_font_osb29_tr[] U8G2_FONT_SECTION("u8g2_font_osb29_tr");
extern const uint8_t u8g2_font_osb29_tn[] U8G2_FONT_SECTION("u8g2_font_osb29_tn");
extern const uint8_t u8g2_font_osb35_tf[] U8G2_FONT_SECTION("u8g2_font_osb35_tf");
extern const uint8_t u8g2_font_osb35_tr[] U8G2_FONT_SECTION("u8g2_font_osb35_tr");
extern const uint8_t u8g2_font_osb35_tn[] U8G2_FONT_SECTION("u8g2_font_osb35_tn");
extern const uint8_t u8g2_font_osb41_tf[] U8G2_FONT_SECTION("u8g2_font_osb41_tf");
extern const uint8_t u8g2_font_osb41_tr[] U8G2_FONT_SECTION("u8g2_font_osb41_tr");
extern const uint8_t u8g2_font_osb41_tn[] U8G2_FONT_SECTION("u8g2_font_osb41_tn");
extern const uint8_t u8g2_font_osr18_tf[] U8G2_FONT_SECTION("u8g2_font_osr18_tf");
extern const uint8_t u8g2_font_osr18_tr[] U8G2_FONT_SECTION("u8g2_font_osr18_tr");
extern const uint8_t u8g2_font_osr18_tn[] U8G2_FONT_SECTION("u8g2_font_osr18_tn");
extern const uint8_t u8g2_font_osr21_tf[] U8G2_FONT_SECTION("u8g2_font_osr21_tf");
extern const uint8_t u8g2_font_osr21_tr[] U8G2_FONT_SECTION("u8g2_font_osr21_tr");
extern const uint8_t u8g2_font_osr21_tn[] U8G2_FONT_SECTION("u8g2_font_osr21_tn");
extern const uint8_t u8g2_font_osr26_tf[] U8G2_FONT_SECTION("u8g2_font_osr26_tf");
extern const uint8_t u8g2_font_osr26_tr[] U8G2_FONT_SECTION("u8g2_font_osr26_tr");
extern const uint8_t u8g2_font_osr26_tn[] U8G2_FONT_SECTION("u8g2_font_osr26_tn");
extern const uint8_t u8g2_font_osr29_tf[] U8G2_FONT_SECTION("u8g2_font_osr29_tf");
extern const uint8_t u8g2_font_osr29_tr[] U8G2_FONT_SECTION("u8g2_font_osr29_tr");
extern const uint8_t u8g2_font_osr29_tn[] U8G2_FONT_SECTION("u8g2_font_osr29_tn");
extern const uint8_t u8g2_font_osr35_tf[] U8G2_FONT_SECTION("u8g2_font_osr35_tf");
extern const uint8_t u8g2_font_osr35_tr[] U8G2_FONT_SECTION("u8g2_font_osr35_tr");
extern const uint8_t u8g2_font_osr35_tn[] U8G2_FONT_SECTION("u8g2_font_osr35_tn");
extern const uint8_t u8g2_font_osr41_tf[] U8G2_FONT_SECTION("u8g2_font_osr41_tf");
extern const uint8_t u8g2_font_osr41_tr[] U8G2_FONT_SECTION("u8g2_font_osr41_tr");
extern const uint8_t u8g2_font_osr41_tn[] U8G2_FONT_SECTION("u8g2_font_osr41_tn");
extern const uint8_t u8g2_font_inr16_mf[] U8G2_FONT_SECTION("u8g2_font_inr16_mf");
extern const uint8_t u8g2_font_inr16_mr[] U8G2_FONT_SECTION("u8g2_font_inr16_mr");
extern const uint8_t u8g2_font_inr16_mn[] U8G2_FONT_SECTION("u8g2_font_inr16_mn");
extern const uint8_t u8g2_font_inr19_mf[] U8G2_FONT_SECTION("u8g2_font_inr19_mf");
extern const uint8_t u8g2_font_inr19_mr[] U8G2_FONT_SECTION("u8g2_font_inr19_mr");
extern const uint8_t u8g2_font_inr19_mn[] U8G2_FONT_SECTION("u8g2_font_inr19_mn");
extern const uint8_t u8g2_font_inr21_mf[] U8G2_FONT_SECTION("u8g2_font_inr21_mf");
extern const uint8_t u8g2_font_inr21_mr[] U8G2_FONT_SECTION("u8g2_font_inr21_mr");
extern const uint8_t u8g2_font_inr21_mn[] U8G2_FONT_SECTION("u8g2_font_inr21_mn");
extern const uint8_t u8g2_font_inr24_mf[] U8G2_FONT_SECTION("u8g2_font_inr24_mf");
extern const uint8_t u8g2_font_inr24_mr[] U8G2_FONT_SECTION("u8g2_font_inr24_mr");
extern const uint8_t u8g2_font_inr24_mn[] U8G2_FONT_SECTION("u8g2_font_inr24_mn");
extern const uint8_t u8g2_font_inr27_mf[] U8G2_FONT_SECTION("u8g2_font_inr27_mf");
extern const uint8_t u8g2_font_inr27_mr[] U8G2_FONT_SECTION("u8g2_font_inr27_mr");
extern const uint8_t u8g2_font_inr27_mn[] U8G2_FONT_SECTION("u8g2_font_inr27_mn");
extern const uint8_t u8g2_font_inr30_mf[] U8G2_FONT_SECTION("u8g2_font_inr30_mf");
extern const uint8_t u8g2_font_inr30_mr[] U8G2_FONT_SECTION("u8g2_font_inr30_mr");
extern const uint8_t u8g2_font_inr30_mn[] U8G2_FONT_SECTION("u8g2_font_inr30_mn");
extern const uint8_t u8g2_font_inr33_mf[] U8G2_FONT_SECTION("u8g2_font_inr33_mf");
extern const uint8_t u8g2_font_inr33_mr[] U8G2_FONT_SECTION("u8g2_font_inr33_mr");
extern const uint8_t u8g2_font_inr33_mn[] U8G2_FONT_SECTION("u8g2_font_inr33_mn");
extern const uint8_t u8g2_font_inr38_mf[] U8G2_FONT_SECTION("u8g2_font_inr38_mf");
extern const uint8_t u8g2_font_inr38_mr[] U8G2_FONT_SECTION("u8g2_font_inr38_mr");
extern const uint8_t u8g2_font_inr38_mn[] U8G2_FONT_SECTION("u8g2_font_inr38_mn");
extern const uint8_t u8g2_font_inr42_mf[] U8G2_FONT_SECTION("u8g2_font_inr42_mf");
extern const uint8_t u8g2_font_inr42_mr[] U8G2_FONT_SECTION("u8g2_font_inr42_mr");
extern const uint8_t u8g2_font_inr42_mn[] U8G2_FONT_SECTION("u8g2_font_inr42_mn");
extern const uint8_t u8g2_font_inr46_mf[] U8G2_FONT_SECTION("u8g2_font_inr46_mf");
extern const uint8_t u8g2_font_inr46_mr[] U8G2_FONT_SECTION("u8g2_font_inr46_mr");
extern const uint8_t u8g2_font_inr46_mn[] U8G2_FONT_SECTION("u8g2_font_inr46_mn");
extern const uint8_t u8g2_font_inr49_mf[] U8G2_FONT_SECTION("u8g2_font_inr49_mf");
extern const uint8_t u8g2_font_inr49_mr[] U8G2_FONT_SECTION("u8g2_font_inr49_mr");
extern const uint8_t u8g2_font_inr49_mn[] U8G2_FONT_SECTION("u8g2_font_inr49_mn");
extern const uint8_t u8g2_font_inr53_mf[] U8G2_FONT_SECTION("u8g2_font_inr53_mf");
extern const uint8_t u8g2_font_inr53_mr[] U8G2_FONT_SECTION("u8g2_font_inr53_mr");
extern const uint8_t u8g2_font_inr53_mn[] U8G2_FONT_SECTION("u8g2_font_inr53_mn");
extern const uint8_t u8g2_font_inr57_mn[] U8G2_FONT_SECTION("u8g2_font_inr57_mn");
extern const uint8_t u8g2_font_inr62_mn[] U8G2_FONT_SECTION("u8g2_font_inr62_mn");
extern const uint8_t u8g2_font_inb16_mf[] U8G2_FONT_SECTION("u8g2_font_inb16_mf");
extern const uint8_t u8g2_font_inb16_mr[] U8G2_FONT_SECTION("u8g2_font_inb16_mr");
extern const uint8_t u8g2_font_inb16_mn[] U8G2_FONT_SECTION("u8g2_font_inb16_mn");
extern const uint8_t u8g2_font_inb19_mf[] U8G2_FONT_SECTION("u8g2_font_inb19_mf");
extern const uint8_t u8g2_font_inb19_mr[] U8G2_FONT_SECTION("u8g2_font_inb19_mr");
extern const uint8_t u8g2_font_inb19_mn[] U8G2_FONT_SECTION("u8g2_font_inb19_mn");
extern const uint8_t u8g2_font_inb21_mf[] U8G2_FONT_SECTION("u8g2_font_inb21_mf");
extern const uint8_t u8g2_font_inb21_mr[] U8G2_FONT_SECTION("u8g2_font_inb21_mr");
extern const uint8_t u8g2_font_inb21_mn[] U8G2_FONT_SECTION("u8g2_font_inb21_mn");
extern const uint8_t u8g2_font_inb24_mf[] U8G2_FONT_SECTION("u8g2_font_inb24_mf");
extern const uint8_t u8g2_font_inb24_mr[] U8G2_FONT_SECTION("u8g2_font_inb24_mr");
extern const uint8_t u8g2_font_inb24_mn[] U8G2_FONT_SECTION("u8g2_font_inb24_mn");
extern const uint8_t u8g2_font_inb27_mf[] U8G2_FONT_SECTION("u8g2_font_inb27_mf");
extern const uint8_t u8g2_font_inb27_mr[] U8G2_FONT_SECTION("u8g2_font_inb27_mr");
extern const uint8_t u8g2_font_inb27_mn[] U8G2_FONT_SECTION("u8g2_font_inb27_mn");
extern const uint8_t u8g2_font_inb30_mf[] U8G2_FONT_SECTION("u8g2_font_inb30_mf");
extern const uint8_t u8g2_font_inb30_mr[] U8G2_FONT_SECTION("u8g2_font_inb30_mr");
extern const uint8_t u8g2_font_inb30_mn[] U8G2_FONT_SECTION("u8g2_font_inb30_mn");
extern const uint8_t u8g2_font_inb33_mf[] U8G2_FONT_SECTION("u8g2_font_inb33_mf");
extern const uint8_t u8g2_font_inb33_mr[] U8G2_FONT_SECTION("u8g2_font_inb33_mr");
extern const uint8_t u8g2_font_inb33_mn[] U8G2_FONT_SECTION("u8g2_font_inb33_mn");
extern const uint8_t u8g2_font_inb38_mf[] U8G2_FONT_SECTION("u8g2_font_inb38_mf");
extern const uint8_t u8g2_font_inb38_mr[] U8G2_FONT_SECTION("u8g2_font_inb38_mr");
extern const uint8_t u8g2_font_inb38_mn[] U8G2_FONT_SECTION("u8g2_font_inb38_mn");
extern const uint8_t u8g2_font_inb42_mf[] U8G2_FONT_SECTION("u8g2_font_inb42_mf");
extern const uint8_t u8g2_font_inb42_mr[] U8G2_FONT_SECTION("u8g2_font_inb42_mr");
extern const uint8_t u8g2_font_inb42_mn[] U8G2_FONT_SECTION("u8g2_font_inb42_mn");
extern const uint8_t u8g2_font_inb46_mf[] U8G2_FONT_SECTION("u8g2_font_inb46_mf");
extern const uint8_t u8g2_font_inb46_mr[] U8G2_FONT_SECTION("u8g2_font_inb46_mr");
extern const uint8_t u8g2_font_inb46_mn[] U8G2_FONT_SECTION("u8g2_font_inb46_mn");
extern const uint8_t u8g2_font_inb49_mf[] U8G2_FONT_SECTION("u8g2_font_inb49_mf");
extern const uint8_t u8g2_font_inb49_mr[] U8G2_FONT_SECTION("u8g2_font_inb49_mr");
extern const uint8_t u8g2_font_inb49_mn[] U8G2_FONT_SECTION("u8g2_font_inb49_mn");
extern const uint8_t u8g2_font_inb53_mf[] U8G2_FONT_SECTION("u8g2_font_inb53_mf");
extern const uint8_t u8g2_font_inb53_mr[] U8G2_FONT_SECTION("u8g2_font_inb53_mr");
extern const uint8_t u8g2_font_inb53_mn[] U8G2_FONT_SECTION("u8g2_font_inb53_mn");
extern const uint8_t u8g2_font_inb57_mn[] U8G2_FONT_SECTION("u8g2_font_inb57_mn");
extern const uint8_t u8g2_font_inb63_mn[] U8G2_FONT_SECTION("u8g2_font_inb63_mn");
extern const uint8_t u8g2_font_logisoso16_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso16_tf");
extern const uint8_t u8g2_font_logisoso16_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso16_tr");
extern const uint8_t u8g2_font_logisoso16_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso16_tn");
extern const uint8_t u8g2_font_logisoso18_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso18_tf");
extern const uint8_t u8g2_font_logisoso18_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso18_tr");
extern const uint8_t u8g2_font_logisoso18_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso18_tn");
extern const uint8_t u8g2_font_logisoso20_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso20_tf");
extern const uint8_t u8g2_font_logisoso20_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso20_tr");
extern const uint8_t u8g2_font_logisoso20_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso20_tn");
extern const uint8_t u8g2_font_logisoso22_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso22_tf");
extern const uint8_t u8g2_font_logisoso22_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso22_tr");
extern const uint8_t u8g2_font_logisoso22_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso22_tn");
extern const uint8_t u8g2_font_logisoso24_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso24_tf");
extern const uint8_t u8g2_font_logisoso24_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso24_tr");
extern const uint8_t u8g2_font_logisoso24_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso24_tn");
extern const uint8_t u8g2_font_logisoso26_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso26_tf");
extern const uint8_t u8g2_font_logisoso26_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso26_tr");
extern const uint8_t u8g2_font_logisoso26_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso26_tn");
extern const uint8_t u8g2_font_logisoso28_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso28_tf");
extern const uint8_t u8g2_font_logisoso28_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso28_tr");
extern const uint8_t u8g2_font_logisoso28_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso28_tn");
extern const uint8_t u8g2_font_logisoso30_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso30_tf");
extern const uint8_t u8g2_font_logisoso30_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso30_tr");
extern const uint8_t u8g2_font_logisoso30_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso30_tn");
extern const uint8_t u8g2_font_logisoso32_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso32_tf");
extern const uint8_t u8g2_font_logisoso32_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso32_tr");
extern const uint8_t u8g2_font_logisoso32_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso32_tn");
extern const uint8_t u8g2_font_logisoso34_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso34_tf");
extern const uint8_t u8g2_font_logisoso34_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso34_tr");
extern const uint8_t u8g2_font_logisoso34_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso34_tn");
extern const uint8_t u8g2_font_logisoso38_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso38_tf");
extern const uint8_t u8g2_font_logisoso38_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso38_tr");
extern const uint8_t u8g2_font_logisoso38_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso38_tn");
extern const uint8_t u8g2_font_logisoso42_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso42_tf");
extern const uint8_t u8g2_font_logisoso42_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso42_tr");
extern const uint8_t u8g2_font_logisoso42_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso42_tn");
extern const uint8_t u8g2_font_logisoso46_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso46_tf");
extern const uint8_t u8g2_font_logisoso46_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso46_tr");
extern const uint8_t u8g2_font_logisoso46_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso46_tn");
extern const uint8_t u8g2_font_logisoso50_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso50_tf");
extern const uint8_t u8g2_font_logisoso50_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso50_tr");
extern const uint8_t u8g2_font_logisoso50_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso50_tn");
extern const uint8_t u8g2_font_logisoso54_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso54_tf");
extern const uint8_t u8g2_font_logisoso54_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso54_tr");
extern const uint8_t u8g2_font_logisoso54_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso54_tn");
extern const uint8_t u8g2_font_logisoso58_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso58_tf");
extern const uint8_t u8g2_font_logisoso58_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso58_tr");
extern const uint8_t u8g2_font_logisoso58_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso58_tn");
extern const uint8_t u8g2_font_logisoso62_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso62_tn");
extern const uint8_t u8g2_font_logisoso78_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso78_tn");
extern const uint8_t u8g2_font_logisoso92_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso92_tn");
extern const uint8_t u8g2_font_pressstart2p_8f[] U8G2_FONT_SECTION("u8g2_font_pressstart2p_8f");
extern const uint8_t u8g2_font_pressstart2p_8r[] U8G2_FONT_SECTION("u8g2_font_pressstart2p_8r");
extern const uint8_t u8g2_font_pressstart2p_8n[] U8G2_FONT_SECTION("u8g2_font_pressstart2p_8n");
extern const uint8_t u8g2_font_pressstart2p_8u[] U8G2_FONT_SECTION("u8g2_font_pressstart2p_8u");
extern const uint8_t u8g2_font_pcsenior_8f[] U8G2_FONT_SECTION("u8g2_font_pcsenior_8f");
extern const uint8_t u8g2_font_pcsenior_8r[] U8G2_FONT_SECTION("u8g2_font_pcsenior_8r");
extern const uint8_t u8g2_font_pcsenior_8n[] U8G2_FONT_SECTION("u8g2_font_pcsenior_8n");
extern const uint8_t u8g2_font_pcsenior_8u[] U8G2_FONT_SECTION("u8g2_font_pcsenior_8u");
extern const uint8_t u8g2_font_pxplusibmcgathin_8f[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcgathin_8f");
extern const uint8_t u8g2_font_pxplusibmcgathin_8r[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcgathin_8r");
extern const uint8_t u8g2_font_pxplusibmcgathin_8n[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcgathin_8n");
extern const uint8_t u8g2_font_pxplusibmcgathin_8u[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcgathin_8u");
extern const uint8_t u8g2_font_pxplusibmcga_8f[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcga_8f");
extern const uint8_t u8g2_font_pxplusibmcga_8r[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcga_8r");
extern const uint8_t u8g2_font_pxplusibmcga_8n[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcga_8n");
extern const uint8_t u8g2_font_pxplusibmcga_8u[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcga_8u");
extern const uint8_t u8g2_font_pxplustandynewtv_8f[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8f");
extern const uint8_t u8g2_font_pxplustandynewtv_8r[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8r");
extern const uint8_t u8g2_font_pxplustandynewtv_8n[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8n");
extern const uint8_t u8g2_font_pxplustandynewtv_8u[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8u");
extern const uint8_t u8g2_font_pxplustandynewtv_t_all[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_t_all");
extern const uint8_t u8g2_font_pxplustandynewtv_8_all[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8_all");
extern const uint8_t u8g2_font_pxplusibmvga9_tf[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_tf");
extern const uint8_t u8g2_font_pxplusibmvga9_tr[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_tr");
extern const uint8_t u8g2_font_pxplusibmvga9_tn[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_tn");
extern const uint8_t u8g2_font_pxplusibmvga9_mf[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_mf");
extern const uint8_t u8g2_font_pxplusibmvga9_mr[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_mr");
extern const uint8_t u8g2_font_pxplusibmvga9_mn[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_mn");
extern const uint8_t u8g2_font_pxplusibmvga9_t_all[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_t_all");
extern const uint8_t u8g2_font_pxplusibmvga9_m_all[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_m_all");
extern const uint8_t u8g2_font_pxplusibmvga8_tf[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_tf");
extern const uint8_t u8g2_font_pxplusibmvga8_tr[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_tr");
extern const uint8_t u8g2_font_pxplusibmvga8_tn[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_tn");
extern const uint8_t u8g2_font_pxplusibmvga8_mf[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_mf");
extern const uint8_t u8g2_font_pxplusibmvga8_mr[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_mr");
extern const uint8_t u8g2_font_pxplusibmvga8_mn[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_mn");
extern const uint8_t u8g2_font_pxplusibmvga8_t_all[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_t_all");
extern const uint8_t u8g2_font_pxplusibmvga8_m_all[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_m_all");
extern const uint8_t u8g2_font_px437wyse700a_tf[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_tf");
extern const uint8_t u8g2_font_px437wyse700a_tr[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_tr");
extern const uint8_t u8g2_font_px437wyse700a_tn[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_tn");
extern const uint8_t u8g2_font_px437wyse700a_mf[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_mf");
extern const uint8_t u8g2_font_px437wyse700a_mr[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_mr");
extern const uint8_t u8g2_font_px437wyse700a_mn[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_mn");
extern const uint8_t u8g2_font_px437wyse700b_tf[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_tf");
extern const uint8_t u8g2_font_px437wyse700b_tr[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_tr");
extern const uint8_t u8g2_font_px437wyse700b_tn[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_tn");
extern const uint8_t u8g2_font_px437wyse700b_mf[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_mf");
extern const uint8_t u8g2_font_px437wyse700b_mr[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_mr");
extern const uint8_t u8g2_font_px437wyse700b_mn[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_mn");

/* end font list */

/*==========================================*/
/* u8g font mapping, might be incomplete.... */


#define u8g_font_10x20   u8g2_font_10x20_tf
#define u8g_font_10x20r   u8g2_font_10x20_tr
#define u8g_font_4x6   u8g2_font_4x6_tf
#define u8g_font_4x6r   u8g2_font_4x6_tr
#define u8g_font_5x7   u8g2_font_5x7_tf
#define u8g_font_5x7r   u8g2_font_5x7_tr
#define u8g_font_5x8   u8g2_font_5x8_tf
#define u8g_font_5x8r   u8g2_font_5x8_tr
#define u8g_font_6x10   u8g2_font_6x10_tf
#define u8g_font_6x10r   u8g2_font_6x10_tr
#define u8g_font_6x12   u8g2_font_6x12_tf
#define u8g_font_6x12r   u8g2_font_6x12_tr
#define u8g_font_6x13B   u8g2_font_6x13B_tf
#define u8g_font_6x13Br   u8g2_font_6x13B_tr
#define u8g_font_6x13   u8g2_font_6x13_tf
#define u8g_font_6x13r   u8g2_font_6x13_tr
#define u8g_font_6x13O   u8g2_font_6x13O_tf
#define u8g_font_6x13Or   u8g2_font_6x13O_tr
#define u8g_font_7x13B   u8g2_font_7x13B_tf
#define u8g_font_7x13Br   u8g2_font_7x13B_tr
#define u8g_font_7x13   u8g2_font_7x13_tf
#define u8g_font_7x13r   u8g2_font_7x13_tr
#define u8g_font_7x13O   u8g2_font_7x13O_tf
#define u8g_font_7x13Or   u8g2_font_7x13O_tr
#define u8g_font_7x14B   u8g2_font_7x14B_tf
#define u8g_font_7x14Br   u8g2_font_7x14B_tr
#define u8g_font_7x14   u8g2_font_7x14_tf
#define u8g_font_7x14r   u8g2_font_7x14_tr
#define u8g_font_8x13B   u8g2_font_8x13B_tf
#define u8g_font_8x13Br   u8g2_font_8x13B_tr
#define u8g_font_8x13   u8g2_font_8x13_tf
#define u8g_font_8x13r   u8g2_font_8x13_tr
#define u8g_font_8x13O   u8g2_font_8x13O_tf
#define u8g_font_8x13Or   u8g2_font_8x13O_tr
#define u8g_font_9x15B   u8g2_font_9x15B_tf
#define u8g_font_9x15Br   u8g2_font_9x15B_tr
#define u8g_font_9x15   u8g2_font_9x15_tf
#define u8g_font_9x15r   u8g2_font_9x15_tr
#define u8g_font_9x18B   u8g2_font_9x18B_tf
#define u8g_font_9x18   u8g2_font_9x18_tf
#define u8g_font_9x18Br   u8g2_font_9x18B_tr
#define u8g_font_9x18r   u8g2_font_9x18_tr
#define u8g_font_cu12   u8g2_font_cu12_tf
#define u8g_font_micro   u8g2_font_micro_tf
#define u8g_font_unifont   u8g2_font_unifont_t_latin
#define u8g_font_unifontr   u8g2_font_unifont_t_latin
#define u8g_font_courB08   u8g2_font_courB08_tf
#define u8g_font_courB08r   u8g2_font_courB08_tr
#define u8g_font_courB10   u8g2_font_courB10_tf
#define u8g_font_courB10r   u8g2_font_courB10_tr
#define u8g_font_courB12   u8g2_font_courB12_tf
#define u8g_font_courB12r   u8g2_font_courB12_tr
#define u8g_font_courB14   u8g2_font_courB14_tf
#define u8g_font_courB14r   u8g2_font_courB14_tr
#define u8g_font_courB18   u8g2_font_courB18_tf
#define u8g_font_courB18r   u8g2_font_courB18_tr
#define u8g_font_courB24   u8g2_font_courB24_tf
#define u8g_font_courB24r   u8g2_font_courB24_tr
#define u8g_font_courB24n   u8g2_font_courB24_tn
#define u8g_font_courR08   u8g2_font_courR08_tf
#define u8g_font_courR08r   u8g2_font_courR08_tr
#define u8g_font_courR10   u8g2_font_courR10_tf
#define u8g_font_courR10r   u8g2_font_courR10_tr
#define u8g_font_courR12   u8g2_font_courR12_tf
#define u8g_font_courR12r   u8g2_font_courR12_tr
#define u8g_font_courR14   u8g2_font_courR14_tf
#define u8g_font_courR14r   u8g2_font_courR14_tr
#define u8g_font_courR18   u8g2_font_courR18_tf
#define u8g_font_courR18r   u8g2_font_courR18_tr
#define u8g_font_courR24   u8g2_font_courR24_tf
#define u8g_font_courR24r   u8g2_font_courR24_tr
#define u8g_font_courR24n   u8g2_font_courR24_tn
#define u8g_font_helvB08   u8g2_font_helvB08_tf
#define u8g_font_helvB08r   u8g2_font_helvB08_tr
#define u8g_font_helvB08n   u8g2_font_helvB08_tn
#define u8g_font_helvB10   u8g2_font_helvB10_tf
#define u8g_font_helvB10r   u8g2_font_helvB10_tr
#define u8g_font_helvB10n   u8g2_font_helvB10_tn
#define u8g_font_helvB12   u8g2_font_helvB12_tf
#define u8g_font_helvB12r   u8g2_font_helvB12_tr
#define u8g_font_helvB12n   u8g2_font_helvB12_tn
#define u8g_font_helvB14   u8g2_font_helvB14_tf
#define u8g_font_helvB14r   u8g2_font_helvB14_tr
#define u8g_font_helvB14n   u8g2_font_helvB14_tn
#define u8g_font_helvB18   u8g2_font_helvB18_tf
#define u8g_font_helvB18r   u8g2_font_helvB18_tr
#define u8g_font_helvB18n   u8g2_font_helvB18_tn
#define u8g_font_helvB24   u8g2_font_helvB24_tf
#define u8g_font_helvB24r   u8g2_font_helvB24_tr
#define u8g_font_helvB24n   u8g2_font_helvB24_tn
#define u8g_font_helvR08   u8g2_font_helvR08_tf
#define u8g_font_helvR08r   u8g2_font_helvR08_tr
#define u8g_font_helvR08n   u8g2_font_helvR08_tn
#define u8g_font_helvR10   u8g2_font_helvR10_tf
#define u8g_font_helvR10r   u8g2_font_helvR10_tr
#define u8g_font_helvR10n   u8g2_font_helvR10_tn
#define u8g_font_helvR12   u8g2_font_helvR12_tf
#define u8g_font_helvR12r   u8g2_font_helvR12_tr
#define u8g_font_helvR12n   u8g2_font_helvR12_tn
#define u8g_font_helvR14   u8g2_font_helvR14_tf
#define u8g_font_helvR14r   u8g2_font_helvR14_tr
#define u8g_font_helvR14n   u8g2_font_helvR14_tn
#define u8g_font_helvR18   u8g2_font_helvR18_tf
#define u8g_font_helvR18r   u8g2_font_helvR18_tr
#define u8g_font_helvR18n   u8g2_font_helvR18_tn
#define u8g_font_helvR24   u8g2_font_helvR24_tf
#define u8g_font_helvR24r   u8g2_font_helvR24_tr
#define u8g_font_helvR24n   u8g2_font_helvR24_tn
#define u8g_font_ncenB08   u8g2_font_ncenB08_tf
#define u8g_font_ncenB08r   u8g2_font_ncenB08_tr
#define u8g_font_ncenB10   u8g2_font_ncenB10_tf
#define u8g_font_ncenB10r   u8g2_font_ncenB10_tr
#define u8g_font_ncenB12   u8g2_font_ncenB12_tf
#define u8g_font_ncenB12r   u8g2_font_ncenB12_tr
#define u8g_font_ncenB14   u8g2_font_ncenB14_tf
#define u8g_font_ncenB14r   u8g2_font_ncenB14_tr
#define u8g_font_ncenB18   u8g2_font_ncenB18_tf
#define u8g_font_ncenB18r   u8g2_font_ncenB18_tr
#define u8g_font_ncenB24   u8g2_font_ncenB24_tf
#define u8g_font_ncenB24r   u8g2_font_ncenB24_tr
#define u8g_font_ncenB24n   u8g2_font_ncenB24_tn
#define u8g_font_ncenR08   u8g2_font_ncenR08_tf
#define u8g_font_ncenR08r   u8g2_font_ncenR08_tr
#define u8g_font_ncenR10   u8g2_font_ncenR10_tf
#define u8g_font_ncenR10r   u8g2_font_ncenR10_tr
#define u8g_font_ncenR12   u8g2_font_ncenR12_tf
#define u8g_font_ncenR12r   u8g2_font_ncenR12_tr
#define u8g_font_ncenR14   u8g2_font_ncenR14_tf
#define u8g_font_ncenR14r   u8g2_font_ncenR14_tr
#define u8g_font_ncenR18   u8g2_font_ncenR18_tf
#define u8g_font_ncenR18r   u8g2_font_ncenR18_tr
#define u8g_font_ncenR24   u8g2_font_ncenR24_tf
#define u8g_font_ncenR24r   u8g2_font_ncenR24_tr
#define u8g_font_ncenR24n   u8g2_font_ncenR24_tn
#define u8g_font_timB08   u8g2_font_timB08_tf
#define u8g_font_timB08r   u8g2_font_timB08_tr
#define u8g_font_timB10   u8g2_font_timB10_tf
#define u8g_font_timB10r   u8g2_font_timB10_tr
#define u8g_font_timB12   u8g2_font_timB12_tf
#define u8g_font_timB12r   u8g2_font_timB12_tr
#define u8g_font_timB14   u8g2_font_timB14_tf
#define u8g_font_timB14r   u8g2_font_timB14_tr
#define u8g_font_timB18   u8g2_font_timB18_tf
#define u8g_font_timB18r   u8g2_font_timB18_tr
#define u8g_font_timB24   u8g2_font_timB24_tf
#define u8g_font_timB24r   u8g2_font_timB24_tr
#define u8g_font_timB24n   u8g2_font_timB24_tn
#define u8g_font_timR08   u8g2_font_timR08_tf
#define u8g_font_timR08r   u8g2_font_timR08_tr
#define u8g_font_timR10   u8g2_font_timR10_tf
#define u8g_font_timR10r   u8g2_font_timR10_tr
#define u8g_font_timR12   u8g2_font_timR12_tf
#define u8g_font_timR12r   u8g2_font_timR12_tr
#define u8g_font_timR14   u8g2_font_timR14_tf
#define u8g_font_timR14r   u8g2_font_timR14_tr
#define u8g_font_timR18   u8g2_font_timR18_tf
#define u8g_font_timR18r   u8g2_font_timR18_tr
#define u8g_font_timR24   u8g2_font_timR24_tf
#define u8g_font_timR24r   u8g2_font_timR24_tr
#define u8g_font_timR24n   u8g2_font_timR24_tn
#define u8g_font_p01type   u8g2_font_p01type_tf
#define u8g_font_p01typer   u8g2_font_p01type_tr
#define u8g_font_lucasfont_alternate   u8g2_font_lucasfont_alternate_tf
#define u8g_font_lucasfont_alternater   u8g2_font_lucasfont_alternate_tr
#define u8g_font_chikita   u8g2_font_chikita_tf
#define u8g_font_chikitar   u8g2_font_chikita_tr
#define u8g_font_pixelle_micro   u8g2_font_pixelle_micro_tf
#define u8g_font_pixelle_micror   u8g2_font_pixelle_micro_tr
#define u8g_font_trixel_square   u8g2_font_trixel_square_tf
#define u8g_font_trixel_squarer   u8g2_font_trixel_square_tr
#define u8g_font_robot_de_niro   u8g2_font_robot_de_niro_tf
#define u8g_font_robot_de_niror   u8g2_font_robot_de_niro_tr
#define u8g_font_baby   u8g2_font_baby_tf
#define u8g_font_babyr   u8g2_font_baby_tr
#define u8g_font_blipfest_07   u8g2_font_blipfest_07_tr
#define u8g_font_blipfest_07r   u8g2_font_blipfest_07_tr
#define u8g_font_blipfest_07n   u8g2_font_blipfest_07_tn
#define u8g_font_profont10   u8g2_font_profont10_tf
#define u8g_font_profont10r   u8g2_font_profont10_tr
#define u8g_font_profont11   u8g2_font_profont11_tf
#define u8g_font_profont11r   u8g2_font_profont11_tr
#define u8g_font_profont12   u8g2_font_profont12_tf
#define u8g_font_profont12r   u8g2_font_profont12_tr
#define u8g_font_profont15   u8g2_font_profont15_tf
#define u8g_font_profont15r   u8g2_font_profont15_tr
#define u8g_font_profont17   u8g2_font_profont17_tf
#define u8g_font_profont17r   u8g2_font_profont17_tr
#define u8g_font_profont22   u8g2_font_profont22_tf
#define u8g_font_profont22r   u8g2_font_profont22_tr
#define u8g_font_profont29   u8g2_font_profont29_tf
#define u8g_font_profont29r   u8g2_font_profont29_tr


/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
}
#endif


#endif

