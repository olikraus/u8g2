/*

  u8g2.h
  
  call sequence
  
  u8g2_Setup_XYZ
    u8x8_Setup_XYZ
      u8x8_SetupDefaults(u8g2);
      assign u8x8 callbacks
      u8x8->display_cb(u8x8, U8X8_MSG_DISPLAY_SETUP, 0, NULL);  
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
  The following macro enables the HVLine speed optimization.
  It will consume about 40 bytes more in flash memory of the AVR.
*/
#define U8G2_HVLINE_SPEED_OPTIMIZATION

/*
  The following macro enables all four drawing directions for glyphs and strings.
  If this macro is not defined, than a string can be drawn only in horizontal direction.
*/
#define U8G2_WITH_FONT_ROTATION

/*
  The following macro activates the early intersection check with the current visible area.
  Clipping (and low level intersection calculation) may still happen and is controlled by U8G2_WITH_CLIPPING.
  This early intersection check mainly improves speed for the picture loop (u8g2_FirstPage/NextPage).
  With a full framebuffer in RAM and if most graphical elements are drawn within the visible area, then this
  macro can be commented to reduce code size.
*/
#define U8G2_WITH_INTERSECTION

/*
  U8glib V2 contains support for unicode plane 0 (Basic Multilingual Plane, BMP).
  The following macro activates this support. Deactivation would save some ROM.
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


/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================*/

typedef uint16_t u8g2_uint_t;	/* for pixel position only */

typedef struct u8g2_struct u8g2_t;
typedef struct u8g2_cb_struct u8g2_cb_t;

typedef void (*u8g2_update_dimension_cb)(u8g2_t *u8g2);
typedef void (*u8g2_draw_l90_cb)(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);



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
  int8_t  descent_g;
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
#ifdef U8G2_WITH_FONT_ROTATION  
  uint8_t dir;				/* direction */
#endif
};
typedef struct _u8g2_font_decode_t u8g2_font_decode_t;


struct u8g2_cb_struct
{
  u8g2_update_dimension_cb update;
  u8g2_draw_l90_cb draw_l90;
};

typedef u8g2_uint_t (*u8g2_font_calc_vref_fnptr)(u8g2_t *u8g2);


struct u8g2_struct
{
  u8x8_t u8x8;
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
  
  /* display dimensions in pixel for the user, calculated inu8g2_update_dimension_common(), used in u8g2_draw_hv_line_2dir() */
  u8g2_uint_t width;
  u8g2_uint_t height;
  
  /* ths is the clip box for the user to check if a specific box has an intersection */
  u8g2_uint_t user_x0;	/* left corner of the buffer */
  u8g2_uint_t user_x1;	/* right corner of the buffer (excluded) */
  u8g2_uint_t user_y0;
  u8g2_uint_t user_y1;
  
  /* information about the current font */
  const uint8_t *font;             /* current font for all text procedures */
  u8g2_font_calc_vref_fnptr font_calc_vref;
  u8g2_font_decode_t font_decode;		/* new font decode structure */
  u8g2_font_info_t font_info;			/* new font info structure */

  uint8_t font_height_mode;
  int8_t font_ref_ascent;
  int8_t font_ref_descent;

  uint8_t draw_color;		/* 0: clear pixel, 1: set pixel */
  
#ifdef U8G2_WITH_HVLINE_COUNT
  unsigned long hv_cnt;
#endif /* U8G2_WITH_HVLINE_COUNT */   
};

#define u8g2_GetU8x8(u8g2) ((u8x8_t *)(u8g2))

/*==========================================*/
/* u8g2_setup.c */

extern const u8g2_cb_t u8g2_cb_r0;
extern const u8g2_cb_t u8g2_cb_r1;
extern const u8g2_cb_t u8g2_cb_r2;
extern const u8g2_cb_t u8g2_cb_r3;

void u8g2_Setup(u8g2_t *u8g2, uint8_t *buf, uint8_t tile_buf_height, const u8g2_cb_t *u8g2_cb);



/*==========================================*/
/* u8g2_buffer.c */

void u8g2_SendBuffer(u8g2_t *u8g2);
void u8g2_ClearBuffer(u8g2_t *u8g2);

void u8g2_FirstPage(u8g2_t *u8g2);
uint8_t u8g2_NextPage(u8g2_t *u8g2);


/*==========================================*/
/* u8g2_hvline.c */
void u8g2_DrawHVLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir);



/*==========================================*/
/* u8g2_intersection.c */
#ifdef U8G2_WITH_INTERSECTION    
uint8_t u8g2_IsIntersection(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t x1, u8g2_uint_t y1);
#endif /* U8G2_WITH_INTERSECTION */


/*==========================================*/
/* u8g2_font.c */


#define U8G2_FONT_HEIGHT_MODE_TEXT 0
#define U8G2_FONT_HEIGHT_MODE_XTEXT 1
#define U8G2_FONT_HEIGHT_MODE_ALL 2

void u8g2_SetFont(u8g2_t *u8g2, const uint8_t  *font);
void u8g2_SetFontDirection(u8g2_t *u8g2, uint8_t dir);
u8g2_uint_t u8g2_DrawString(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *str);

/*==========================================*/
/* u8x8_d_sdl_128x64.c */
void u8g2_Setup_SDL_128x64(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
void u8g2_Setup_SDL_128x64_4(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);

/*==========================================*/
/* u8x8_d_tga.c */
void u8g2_Setup_TGA_DESC(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
void u8g2_Setup_TGA_LCD(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);


/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
}
#endif


#endif

