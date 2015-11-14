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
  This definition also defines the behavior of the expected string encoding.
  If the following macro is defined, then all strings in the c-code are assumed 
  to be UTF-8 encoded.
  If the following macro is not defined, then all strings in the c-code are assumed 
  to be ISO 8859-1/CP1252 encoded. Note: This is NOT supported by the Arduino 
  IDE. However note, that the chars 0 to 127 have identical encoding for UTF-8
  and ISO 8859-1.
  
  This macro does not affect the u8x8 string draw function.
  u8x8 always assumes ISO 8859-1/CP1252 encoding.
  
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

size_t u8g2_GetFontSize(const uint8_t *font_arg);

#define U8G2_FONT_HEIGHT_MODE_TEXT 0
#define U8G2_FONT_HEIGHT_MODE_XTEXT 1
#define U8G2_FONT_HEIGHT_MODE_ALL 2

void u8g2_SetFont(u8g2_t *u8g2, const uint8_t  *font);

uint8_t u8g2_IsGlyph(u8g2_t *u8g2, uint16_t requested_encoding);
int8_t u8g2_GetGlyphWidth(u8g2_t *u8g2, uint16_t requested_encoding);
u8g2_uint_t u8g2_DrawGlyph(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, uint16_t encoding);

void u8g2_SetFontDirection(u8g2_t *u8g2, uint8_t dir);
u8g2_uint_t u8g2_DrawString(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *str);
#define u8g2_GetMaxCharHeight(u8g2) ((u8g2)->font_info.max_char_height)
#define u8g2_GetMaxCharWidth(u8g2) ((u8g2)->font_info.max_char_width)
#define u8g2_GetAscent(u8g2) ((u8g2)->font_info.ascent_A)
#define u8g2_GetDescent(u8g2) ((u8g2)->font_info.descent_g)


/*==========================================*/
/* u8x8_d_sdl_128x64.c */
void u8g2_Setup_SDL_128x64(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
void u8g2_Setup_SDL_128x64_4(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);

/*==========================================*/
/* u8x8_d_tga.c */
void u8g2_Setup_TGA_DESC(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
void u8g2_Setup_TGA_LCD(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);




/*==========================================*/

/* start font list */
extern const uint8_t u8g2_font_u8glib_4_tf[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_tf");
extern const uint8_t u8g2_font_u8glib_4_tr[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_tr");
extern const uint8_t u8g2_font_u8glib_4_hf[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_hf");
extern const uint8_t u8g2_font_u8glib_4_hr[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_hr");
extern const uint8_t u8g2_font_m2icon_5_tf[] U8G2_FONT_SECTION("u8g2_font_m2icon_5_tf");
extern const uint8_t u8g2_font_m2icon_7_tf[] U8G2_FONT_SECTION("u8g2_font_m2icon_7_tf");
extern const uint8_t u8g2_font_m2icon_9_tf[] U8G2_FONT_SECTION("u8g2_font_m2icon_9_tf");
extern const uint8_t u8g2_font_cursor_tf[] U8G2_FONT_SECTION("u8g2_font_cursor_tf");
extern const uint8_t u8g2_font_cursor_tr[] U8G2_FONT_SECTION("u8g2_font_cursor_tr");
extern const uint8_t u8g2_font_micro_tf[] U8G2_FONT_SECTION("u8g2_font_micro_tf");
extern const uint8_t u8g2_font_micro_tr[] U8G2_FONT_SECTION("u8g2_font_micro_tr");
extern const uint8_t u8g2_font_micro_tn[] U8G2_FONT_SECTION("u8g2_font_micro_tn");
extern const uint8_t u8g2_font_micro_mf[] U8G2_FONT_SECTION("u8g2_font_micro_mf");
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
extern const uint8_t u8g2_font_6x12_mf[] U8G2_FONT_SECTION("u8g2_font_6x12_mf");
extern const uint8_t u8g2_font_6x12_mr[] U8G2_FONT_SECTION("u8g2_font_6x12_mr");
extern const uint8_t u8g2_font_6x12_mn[] U8G2_FONT_SECTION("u8g2_font_6x12_mn");
extern const uint8_t u8g2_font_6x13_tf[] U8G2_FONT_SECTION("u8g2_font_6x13_tf");
extern const uint8_t u8g2_font_6x13_tr[] U8G2_FONT_SECTION("u8g2_font_6x13_tr");
extern const uint8_t u8g2_font_6x13_tn[] U8G2_FONT_SECTION("u8g2_font_6x13_tn");
extern const uint8_t u8g2_font_6x13_mf[] U8G2_FONT_SECTION("u8g2_font_6x13_mf");
extern const uint8_t u8g2_font_6x13_mr[] U8G2_FONT_SECTION("u8g2_font_6x13_mr");
extern const uint8_t u8g2_font_6x13_mn[] U8G2_FONT_SECTION("u8g2_font_6x13_mn");
extern const uint8_t u8g2_font_6x13B_tf[] U8G2_FONT_SECTION("u8g2_font_6x13B_tf");
extern const uint8_t u8g2_font_6x13B_tr[] U8G2_FONT_SECTION("u8g2_font_6x13B_tr");
extern const uint8_t u8g2_font_6x13B_tn[] U8G2_FONT_SECTION("u8g2_font_6x13B_tn");
extern const uint8_t u8g2_font_6x13B_mf[] U8G2_FONT_SECTION("u8g2_font_6x13B_mf");
extern const uint8_t u8g2_font_6x13B_mr[] U8G2_FONT_SECTION("u8g2_font_6x13B_mr");
extern const uint8_t u8g2_font_6x13B_mn[] U8G2_FONT_SECTION("u8g2_font_6x13B_mn");
extern const uint8_t u8g2_font_6x13O_tf[] U8G2_FONT_SECTION("u8g2_font_6x13O_tf");
extern const uint8_t u8g2_font_6x13O_tr[] U8G2_FONT_SECTION("u8g2_font_6x13O_tr");
extern const uint8_t u8g2_font_6x13O_tn[] U8G2_FONT_SECTION("u8g2_font_6x13O_tn");
extern const uint8_t u8g2_font_6x13O_mf[] U8G2_FONT_SECTION("u8g2_font_6x13O_mf");
extern const uint8_t u8g2_font_6x13O_mr[] U8G2_FONT_SECTION("u8g2_font_6x13O_mr");
extern const uint8_t u8g2_font_6x13O_mn[] U8G2_FONT_SECTION("u8g2_font_6x13O_mn");
extern const uint8_t u8g2_font_7x13_tf[] U8G2_FONT_SECTION("u8g2_font_7x13_tf");
extern const uint8_t u8g2_font_7x13_tr[] U8G2_FONT_SECTION("u8g2_font_7x13_tr");
extern const uint8_t u8g2_font_7x13_tn[] U8G2_FONT_SECTION("u8g2_font_7x13_tn");
extern const uint8_t u8g2_font_7x13_mf[] U8G2_FONT_SECTION("u8g2_font_7x13_mf");
extern const uint8_t u8g2_font_7x13_mr[] U8G2_FONT_SECTION("u8g2_font_7x13_mr");
extern const uint8_t u8g2_font_7x13_mn[] U8G2_FONT_SECTION("u8g2_font_7x13_mn");
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
extern const uint8_t u8g2_font_8x13_mf[] U8G2_FONT_SECTION("u8g2_font_8x13_mf");
extern const uint8_t u8g2_font_8x13_mr[] U8G2_FONT_SECTION("u8g2_font_8x13_mr");
extern const uint8_t u8g2_font_8x13_mn[] U8G2_FONT_SECTION("u8g2_font_8x13_mn");
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
extern const uint8_t u8g2_font_9x15_mf[] U8G2_FONT_SECTION("u8g2_font_9x15_mf");
extern const uint8_t u8g2_font_9x15_mr[] U8G2_FONT_SECTION("u8g2_font_9x15_mr");
extern const uint8_t u8g2_font_9x15_mn[] U8G2_FONT_SECTION("u8g2_font_9x15_mn");
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
extern const uint8_t u8g2_font_10x20_mf[] U8G2_FONT_SECTION("u8g2_font_10x20_mf");
extern const uint8_t u8g2_font_10x20_mr[] U8G2_FONT_SECTION("u8g2_font_10x20_mr");
extern const uint8_t u8g2_font_10x20_mn[] U8G2_FONT_SECTION("u8g2_font_10x20_mn");
extern const uint8_t u8g2_font_profont10_mf[] U8G2_FONT_SECTION("u8g2_font_profont10_mf");
extern const uint8_t u8g2_font_profont10_mr[] U8G2_FONT_SECTION("u8g2_font_profont10_mr");
extern const uint8_t u8g2_font_profont10_mn[] U8G2_FONT_SECTION("u8g2_font_profont10_mn");
extern const uint8_t u8g2_font_profont11_mf[] U8G2_FONT_SECTION("u8g2_font_profont11_mf");
extern const uint8_t u8g2_font_profont11_mr[] U8G2_FONT_SECTION("u8g2_font_profont11_mr");
extern const uint8_t u8g2_font_profont11_mn[] U8G2_FONT_SECTION("u8g2_font_profont11_mn");
extern const uint8_t u8g2_font_profont12_mf[] U8G2_FONT_SECTION("u8g2_font_profont12_mf");
extern const uint8_t u8g2_font_profont12_mr[] U8G2_FONT_SECTION("u8g2_font_profont12_mr");
extern const uint8_t u8g2_font_profont12_mn[] U8G2_FONT_SECTION("u8g2_font_profont12_mn");
extern const uint8_t u8g2_font_profont15_mf[] U8G2_FONT_SECTION("u8g2_font_profont15_mf");
extern const uint8_t u8g2_font_profont15_mr[] U8G2_FONT_SECTION("u8g2_font_profont15_mr");
extern const uint8_t u8g2_font_profont15_mn[] U8G2_FONT_SECTION("u8g2_font_profont15_mn");
extern const uint8_t u8g2_font_profont17_mf[] U8G2_FONT_SECTION("u8g2_font_profont17_mf");
extern const uint8_t u8g2_font_profont17_mr[] U8G2_FONT_SECTION("u8g2_font_profont17_mr");
extern const uint8_t u8g2_font_profont17_mn[] U8G2_FONT_SECTION("u8g2_font_profont17_mn");
extern const uint8_t u8g2_font_profont22_mf[] U8G2_FONT_SECTION("u8g2_font_profont22_mf");
extern const uint8_t u8g2_font_profont22_mr[] U8G2_FONT_SECTION("u8g2_font_profont22_mr");
extern const uint8_t u8g2_font_profont22_mn[] U8G2_FONT_SECTION("u8g2_font_profont22_mn");
extern const uint8_t u8g2_font_profont29_mf[] U8G2_FONT_SECTION("u8g2_font_profont29_mf");
extern const uint8_t u8g2_font_profont29_mr[] U8G2_FONT_SECTION("u8g2_font_profont29_mr");
extern const uint8_t u8g2_font_profont29_mn[] U8G2_FONT_SECTION("u8g2_font_profont29_mn");
extern const uint8_t u8g2_font_amstrad_cpc_8f[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_8f");
extern const uint8_t u8g2_font_amstrad_cpc_8r[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_8r");
extern const uint8_t u8g2_font_amstrad_cpc_8n[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_8n");
extern const uint8_t u8g2_font_cu12_tf[] U8G2_FONT_SECTION("u8g2_font_cu12_tf");
extern const uint8_t u8g2_font_cu12_tr[] U8G2_FONT_SECTION("u8g2_font_cu12_tr");
extern const uint8_t u8g2_font_cu12_tn[] U8G2_FONT_SECTION("u8g2_font_cu12_tn");
extern const uint8_t u8g2_font_cu12_hf[] U8G2_FONT_SECTION("u8g2_font_cu12_hf");
extern const uint8_t u8g2_font_cu12_hr[] U8G2_FONT_SECTION("u8g2_font_cu12_hr");
extern const uint8_t u8g2_font_cu12_hn[] U8G2_FONT_SECTION("u8g2_font_cu12_hn");
extern const uint8_t u8g2_font_cu12_mf[] U8G2_FONT_SECTION("u8g2_font_cu12_mf");
extern const uint8_t u8g2_font_cu12_mr[] U8G2_FONT_SECTION("u8g2_font_cu12_mr");
extern const uint8_t u8g2_font_cu12_mn[] U8G2_FONT_SECTION("u8g2_font_cu12_mn");
extern const uint8_t u8g2_font_courB08_tf[] U8G2_FONT_SECTION("u8g2_font_courB08_tf");
extern const uint8_t u8g2_font_courB08_tr[] U8G2_FONT_SECTION("u8g2_font_courB08_tr");
extern const uint8_t u8g2_font_courB08_tn[] U8G2_FONT_SECTION("u8g2_font_courB08_tn");
extern const uint8_t u8g2_font_courB08_mf[] U8G2_FONT_SECTION("u8g2_font_courB08_mf");
extern const uint8_t u8g2_font_courB08_mr[] U8G2_FONT_SECTION("u8g2_font_courB08_mr");
extern const uint8_t u8g2_font_courB08_mn[] U8G2_FONT_SECTION("u8g2_font_courB08_mn");
extern const uint8_t u8g2_font_courB10_tf[] U8G2_FONT_SECTION("u8g2_font_courB10_tf");
extern const uint8_t u8g2_font_courB10_tr[] U8G2_FONT_SECTION("u8g2_font_courB10_tr");
extern const uint8_t u8g2_font_courB10_tn[] U8G2_FONT_SECTION("u8g2_font_courB10_tn");
extern const uint8_t u8g2_font_courB10_mf[] U8G2_FONT_SECTION("u8g2_font_courB10_mf");
extern const uint8_t u8g2_font_courB10_mr[] U8G2_FONT_SECTION("u8g2_font_courB10_mr");
extern const uint8_t u8g2_font_courB10_mn[] U8G2_FONT_SECTION("u8g2_font_courB10_mn");
extern const uint8_t u8g2_font_courB12_tf[] U8G2_FONT_SECTION("u8g2_font_courB12_tf");
extern const uint8_t u8g2_font_courB12_tr[] U8G2_FONT_SECTION("u8g2_font_courB12_tr");
extern const uint8_t u8g2_font_courB12_tn[] U8G2_FONT_SECTION("u8g2_font_courB12_tn");
extern const uint8_t u8g2_font_courB12_mf[] U8G2_FONT_SECTION("u8g2_font_courB12_mf");
extern const uint8_t u8g2_font_courB12_mr[] U8G2_FONT_SECTION("u8g2_font_courB12_mr");
extern const uint8_t u8g2_font_courB12_mn[] U8G2_FONT_SECTION("u8g2_font_courB12_mn");
extern const uint8_t u8g2_font_courB14_tf[] U8G2_FONT_SECTION("u8g2_font_courB14_tf");
extern const uint8_t u8g2_font_courB14_tr[] U8G2_FONT_SECTION("u8g2_font_courB14_tr");
extern const uint8_t u8g2_font_courB14_tn[] U8G2_FONT_SECTION("u8g2_font_courB14_tn");
extern const uint8_t u8g2_font_courB14_mf[] U8G2_FONT_SECTION("u8g2_font_courB14_mf");
extern const uint8_t u8g2_font_courB14_mr[] U8G2_FONT_SECTION("u8g2_font_courB14_mr");
extern const uint8_t u8g2_font_courB14_mn[] U8G2_FONT_SECTION("u8g2_font_courB14_mn");
extern const uint8_t u8g2_font_courB18_tf[] U8G2_FONT_SECTION("u8g2_font_courB18_tf");
extern const uint8_t u8g2_font_courB18_tr[] U8G2_FONT_SECTION("u8g2_font_courB18_tr");
extern const uint8_t u8g2_font_courB18_tn[] U8G2_FONT_SECTION("u8g2_font_courB18_tn");
extern const uint8_t u8g2_font_courB18_mf[] U8G2_FONT_SECTION("u8g2_font_courB18_mf");
extern const uint8_t u8g2_font_courB18_mr[] U8G2_FONT_SECTION("u8g2_font_courB18_mr");
extern const uint8_t u8g2_font_courB18_mn[] U8G2_FONT_SECTION("u8g2_font_courB18_mn");
extern const uint8_t u8g2_font_courB24_tf[] U8G2_FONT_SECTION("u8g2_font_courB24_tf");
extern const uint8_t u8g2_font_courB24_tr[] U8G2_FONT_SECTION("u8g2_font_courB24_tr");
extern const uint8_t u8g2_font_courB24_tn[] U8G2_FONT_SECTION("u8g2_font_courB24_tn");
extern const uint8_t u8g2_font_courB24_mf[] U8G2_FONT_SECTION("u8g2_font_courB24_mf");
extern const uint8_t u8g2_font_courB24_mr[] U8G2_FONT_SECTION("u8g2_font_courB24_mr");
extern const uint8_t u8g2_font_courB24_mn[] U8G2_FONT_SECTION("u8g2_font_courB24_mn");
extern const uint8_t u8g2_font_courR08_tf[] U8G2_FONT_SECTION("u8g2_font_courR08_tf");
extern const uint8_t u8g2_font_courR08_tr[] U8G2_FONT_SECTION("u8g2_font_courR08_tr");
extern const uint8_t u8g2_font_courR08_tn[] U8G2_FONT_SECTION("u8g2_font_courR08_tn");
extern const uint8_t u8g2_font_courR08_mf[] U8G2_FONT_SECTION("u8g2_font_courR08_mf");
extern const uint8_t u8g2_font_courR08_mr[] U8G2_FONT_SECTION("u8g2_font_courR08_mr");
extern const uint8_t u8g2_font_courR08_mn[] U8G2_FONT_SECTION("u8g2_font_courR08_mn");
extern const uint8_t u8g2_font_courR10_tf[] U8G2_FONT_SECTION("u8g2_font_courR10_tf");
extern const uint8_t u8g2_font_courR10_tr[] U8G2_FONT_SECTION("u8g2_font_courR10_tr");
extern const uint8_t u8g2_font_courR10_tn[] U8G2_FONT_SECTION("u8g2_font_courR10_tn");
extern const uint8_t u8g2_font_courR10_mf[] U8G2_FONT_SECTION("u8g2_font_courR10_mf");
extern const uint8_t u8g2_font_courR10_mr[] U8G2_FONT_SECTION("u8g2_font_courR10_mr");
extern const uint8_t u8g2_font_courR10_mn[] U8G2_FONT_SECTION("u8g2_font_courR10_mn");
extern const uint8_t u8g2_font_courR12_tf[] U8G2_FONT_SECTION("u8g2_font_courR12_tf");
extern const uint8_t u8g2_font_courR12_tr[] U8G2_FONT_SECTION("u8g2_font_courR12_tr");
extern const uint8_t u8g2_font_courR12_tn[] U8G2_FONT_SECTION("u8g2_font_courR12_tn");
extern const uint8_t u8g2_font_courR12_mf[] U8G2_FONT_SECTION("u8g2_font_courR12_mf");
extern const uint8_t u8g2_font_courR12_mr[] U8G2_FONT_SECTION("u8g2_font_courR12_mr");
extern const uint8_t u8g2_font_courR12_mn[] U8G2_FONT_SECTION("u8g2_font_courR12_mn");
extern const uint8_t u8g2_font_courR14_tf[] U8G2_FONT_SECTION("u8g2_font_courR14_tf");
extern const uint8_t u8g2_font_courR14_tr[] U8G2_FONT_SECTION("u8g2_font_courR14_tr");
extern const uint8_t u8g2_font_courR14_tn[] U8G2_FONT_SECTION("u8g2_font_courR14_tn");
extern const uint8_t u8g2_font_courR14_mf[] U8G2_FONT_SECTION("u8g2_font_courR14_mf");
extern const uint8_t u8g2_font_courR14_mr[] U8G2_FONT_SECTION("u8g2_font_courR14_mr");
extern const uint8_t u8g2_font_courR14_mn[] U8G2_FONT_SECTION("u8g2_font_courR14_mn");
extern const uint8_t u8g2_font_courR18_tf[] U8G2_FONT_SECTION("u8g2_font_courR18_tf");
extern const uint8_t u8g2_font_courR18_tr[] U8G2_FONT_SECTION("u8g2_font_courR18_tr");
extern const uint8_t u8g2_font_courR18_tn[] U8G2_FONT_SECTION("u8g2_font_courR18_tn");
extern const uint8_t u8g2_font_courR18_mf[] U8G2_FONT_SECTION("u8g2_font_courR18_mf");
extern const uint8_t u8g2_font_courR18_mr[] U8G2_FONT_SECTION("u8g2_font_courR18_mr");
extern const uint8_t u8g2_font_courR18_mn[] U8G2_FONT_SECTION("u8g2_font_courR18_mn");
extern const uint8_t u8g2_font_courR24_tf[] U8G2_FONT_SECTION("u8g2_font_courR24_tf");
extern const uint8_t u8g2_font_courR24_tr[] U8G2_FONT_SECTION("u8g2_font_courR24_tr");
extern const uint8_t u8g2_font_courR24_tn[] U8G2_FONT_SECTION("u8g2_font_courR24_tn");
extern const uint8_t u8g2_font_courR24_mf[] U8G2_FONT_SECTION("u8g2_font_courR24_mf");
extern const uint8_t u8g2_font_courR24_mr[] U8G2_FONT_SECTION("u8g2_font_courR24_mr");
extern const uint8_t u8g2_font_courR24_mn[] U8G2_FONT_SECTION("u8g2_font_courR24_mn");
extern const uint8_t u8g2_font_helvB08_tf[] U8G2_FONT_SECTION("u8g2_font_helvB08_tf");
extern const uint8_t u8g2_font_helvB08_tr[] U8G2_FONT_SECTION("u8g2_font_helvB08_tr");
extern const uint8_t u8g2_font_helvB08_tn[] U8G2_FONT_SECTION("u8g2_font_helvB08_tn");
extern const uint8_t u8g2_font_helvB08_hf[] U8G2_FONT_SECTION("u8g2_font_helvB08_hf");
extern const uint8_t u8g2_font_helvB08_hr[] U8G2_FONT_SECTION("u8g2_font_helvB08_hr");
extern const uint8_t u8g2_font_helvB08_hn[] U8G2_FONT_SECTION("u8g2_font_helvB08_hn");
extern const uint8_t u8g2_font_helvB10_tf[] U8G2_FONT_SECTION("u8g2_font_helvB10_tf");
extern const uint8_t u8g2_font_helvB10_tr[] U8G2_FONT_SECTION("u8g2_font_helvB10_tr");
extern const uint8_t u8g2_font_helvB10_tn[] U8G2_FONT_SECTION("u8g2_font_helvB10_tn");
extern const uint8_t u8g2_font_helvB10_hf[] U8G2_FONT_SECTION("u8g2_font_helvB10_hf");
extern const uint8_t u8g2_font_helvB10_hr[] U8G2_FONT_SECTION("u8g2_font_helvB10_hr");
extern const uint8_t u8g2_font_helvB10_hn[] U8G2_FONT_SECTION("u8g2_font_helvB10_hn");
extern const uint8_t u8g2_font_helvB12_tf[] U8G2_FONT_SECTION("u8g2_font_helvB12_tf");
extern const uint8_t u8g2_font_helvB12_tr[] U8G2_FONT_SECTION("u8g2_font_helvB12_tr");
extern const uint8_t u8g2_font_helvB12_tn[] U8G2_FONT_SECTION("u8g2_font_helvB12_tn");
extern const uint8_t u8g2_font_helvB12_hf[] U8G2_FONT_SECTION("u8g2_font_helvB12_hf");
extern const uint8_t u8g2_font_helvB12_hr[] U8G2_FONT_SECTION("u8g2_font_helvB12_hr");
extern const uint8_t u8g2_font_helvB12_hn[] U8G2_FONT_SECTION("u8g2_font_helvB12_hn");
extern const uint8_t u8g2_font_helvB14_tf[] U8G2_FONT_SECTION("u8g2_font_helvB14_tf");
extern const uint8_t u8g2_font_helvB14_tr[] U8G2_FONT_SECTION("u8g2_font_helvB14_tr");
extern const uint8_t u8g2_font_helvB14_tn[] U8G2_FONT_SECTION("u8g2_font_helvB14_tn");
extern const uint8_t u8g2_font_helvB14_hf[] U8G2_FONT_SECTION("u8g2_font_helvB14_hf");
extern const uint8_t u8g2_font_helvB14_hr[] U8G2_FONT_SECTION("u8g2_font_helvB14_hr");
extern const uint8_t u8g2_font_helvB14_hn[] U8G2_FONT_SECTION("u8g2_font_helvB14_hn");
extern const uint8_t u8g2_font_helvB18_tf[] U8G2_FONT_SECTION("u8g2_font_helvB18_tf");
extern const uint8_t u8g2_font_helvB18_tr[] U8G2_FONT_SECTION("u8g2_font_helvB18_tr");
extern const uint8_t u8g2_font_helvB18_tn[] U8G2_FONT_SECTION("u8g2_font_helvB18_tn");
extern const uint8_t u8g2_font_helvB18_hf[] U8G2_FONT_SECTION("u8g2_font_helvB18_hf");
extern const uint8_t u8g2_font_helvB18_hr[] U8G2_FONT_SECTION("u8g2_font_helvB18_hr");
extern const uint8_t u8g2_font_helvB18_hn[] U8G2_FONT_SECTION("u8g2_font_helvB18_hn");
extern const uint8_t u8g2_font_helvB24_tf[] U8G2_FONT_SECTION("u8g2_font_helvB24_tf");
extern const uint8_t u8g2_font_helvB24_tr[] U8G2_FONT_SECTION("u8g2_font_helvB24_tr");
extern const uint8_t u8g2_font_helvB24_tn[] U8G2_FONT_SECTION("u8g2_font_helvB24_tn");
extern const uint8_t u8g2_font_helvB24_hf[] U8G2_FONT_SECTION("u8g2_font_helvB24_hf");
extern const uint8_t u8g2_font_helvB24_hr[] U8G2_FONT_SECTION("u8g2_font_helvB24_hr");
extern const uint8_t u8g2_font_helvB24_hn[] U8G2_FONT_SECTION("u8g2_font_helvB24_hn");
extern const uint8_t u8g2_font_helvR08_tf[] U8G2_FONT_SECTION("u8g2_font_helvR08_tf");
extern const uint8_t u8g2_font_helvR08_tr[] U8G2_FONT_SECTION("u8g2_font_helvR08_tr");
extern const uint8_t u8g2_font_helvR08_tn[] U8G2_FONT_SECTION("u8g2_font_helvR08_tn");
extern const uint8_t u8g2_font_helvR08_hf[] U8G2_FONT_SECTION("u8g2_font_helvR08_hf");
extern const uint8_t u8g2_font_helvR08_hr[] U8G2_FONT_SECTION("u8g2_font_helvR08_hr");
extern const uint8_t u8g2_font_helvR08_hn[] U8G2_FONT_SECTION("u8g2_font_helvR08_hn");
extern const uint8_t u8g2_font_helvR10_tf[] U8G2_FONT_SECTION("u8g2_font_helvR10_tf");
extern const uint8_t u8g2_font_helvR10_tr[] U8G2_FONT_SECTION("u8g2_font_helvR10_tr");
extern const uint8_t u8g2_font_helvR10_tn[] U8G2_FONT_SECTION("u8g2_font_helvR10_tn");
extern const uint8_t u8g2_font_helvR10_hf[] U8G2_FONT_SECTION("u8g2_font_helvR10_hf");
extern const uint8_t u8g2_font_helvR10_hr[] U8G2_FONT_SECTION("u8g2_font_helvR10_hr");
extern const uint8_t u8g2_font_helvR10_hn[] U8G2_FONT_SECTION("u8g2_font_helvR10_hn");
extern const uint8_t u8g2_font_helvR12_tf[] U8G2_FONT_SECTION("u8g2_font_helvR12_tf");
extern const uint8_t u8g2_font_helvR12_tr[] U8G2_FONT_SECTION("u8g2_font_helvR12_tr");
extern const uint8_t u8g2_font_helvR12_tn[] U8G2_FONT_SECTION("u8g2_font_helvR12_tn");
extern const uint8_t u8g2_font_helvR12_hf[] U8G2_FONT_SECTION("u8g2_font_helvR12_hf");
extern const uint8_t u8g2_font_helvR12_hr[] U8G2_FONT_SECTION("u8g2_font_helvR12_hr");
extern const uint8_t u8g2_font_helvR12_hn[] U8G2_FONT_SECTION("u8g2_font_helvR12_hn");
extern const uint8_t u8g2_font_helvR14_tf[] U8G2_FONT_SECTION("u8g2_font_helvR14_tf");
extern const uint8_t u8g2_font_helvR14_tr[] U8G2_FONT_SECTION("u8g2_font_helvR14_tr");
extern const uint8_t u8g2_font_helvR14_tn[] U8G2_FONT_SECTION("u8g2_font_helvR14_tn");
extern const uint8_t u8g2_font_helvR14_hf[] U8G2_FONT_SECTION("u8g2_font_helvR14_hf");
extern const uint8_t u8g2_font_helvR14_hr[] U8G2_FONT_SECTION("u8g2_font_helvR14_hr");
extern const uint8_t u8g2_font_helvR14_hn[] U8G2_FONT_SECTION("u8g2_font_helvR14_hn");
extern const uint8_t u8g2_font_helvR18_tf[] U8G2_FONT_SECTION("u8g2_font_helvR18_tf");
extern const uint8_t u8g2_font_helvR18_tr[] U8G2_FONT_SECTION("u8g2_font_helvR18_tr");
extern const uint8_t u8g2_font_helvR18_tn[] U8G2_FONT_SECTION("u8g2_font_helvR18_tn");
extern const uint8_t u8g2_font_helvR18_hf[] U8G2_FONT_SECTION("u8g2_font_helvR18_hf");
extern const uint8_t u8g2_font_helvR18_hr[] U8G2_FONT_SECTION("u8g2_font_helvR18_hr");
extern const uint8_t u8g2_font_helvR18_hn[] U8G2_FONT_SECTION("u8g2_font_helvR18_hn");
extern const uint8_t u8g2_font_helvR24_tf[] U8G2_FONT_SECTION("u8g2_font_helvR24_tf");
extern const uint8_t u8g2_font_helvR24_tr[] U8G2_FONT_SECTION("u8g2_font_helvR24_tr");
extern const uint8_t u8g2_font_helvR24_tn[] U8G2_FONT_SECTION("u8g2_font_helvR24_tn");
extern const uint8_t u8g2_font_helvR24_hf[] U8G2_FONT_SECTION("u8g2_font_helvR24_hf");
extern const uint8_t u8g2_font_helvR24_hr[] U8G2_FONT_SECTION("u8g2_font_helvR24_hr");
extern const uint8_t u8g2_font_helvR24_hn[] U8G2_FONT_SECTION("u8g2_font_helvR24_hn");

/* end font list */


/*==========================================*/
/* C++ compatible */

#ifdef __cplusplus
}
#endif


#endif

