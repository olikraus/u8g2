

#ifndef _BDF_GLYPH_H
#define _BDF_GLYPH_H
#include <stdint.h>
#include <stddef.h>

/* forward def */
typedef struct _bdf_font_struct bf_t;


struct _bbx_struct
{
  long w;
  long h;
  long x;
  long y;
};
typedef struct _bbx_struct bbx_t;


struct _bdf_glyph_struct
{
  bf_t *bf;
  
  long encoding;
  long map_to;
  
  long dwidth_x;
  long dwidth_y;

  bbx_t bbx;				/* x,y are delta x,y from glyph, w,h start wth bitmap dimension, but may be reduced */
  long shift_x;				/* extra shift for BDF_BBX_MODE_HEIGHT alignment */

  uint8_t *bitmap_data;
  int bitmap_width;			/* the physical width within "bitmap_data", will be larger than or equal to bbx.w */
  int bitmap_height;
  
  //int actual_bitmap_width;	/* identical to bbx.w, maybe replace actual_bitmap_width by bbx.w */
  //int actual_bitmap_height;	/* identical to bbx.h, maybe replace actual_bitmap_height by bbx.h */
  
  uint8_t *target_data;
  int target_max;
  int target_cnt;
  int target_bit_pos;
  
  /* rle data */
  
  unsigned rle_bits_per_0;
  unsigned rle_bits_per_1;
  
  int rle_is_first;
  unsigned rle_bitcnt;
  unsigned rle_last_0;
  unsigned rle_last_1;
  
  long width_deviation;  /* filled by bf_copy_bbx_and_update_shift for statistics */
  
};
typedef struct _bdf_glyph_struct bg_t;

bg_t *bg_Open(void);
void bg_Close(bg_t *bg);
int bg_AddTargetData(bg_t *bg, uint8_t data);
void bg_ClearTargetData(bg_t *bg);
int bg_AddTargetBits(bg_t *bg, unsigned cnt, unsigned val);
int bg_FlushTargetBits(bg_t *bg);

int bg_SetBitmapSizeInBytes(bg_t *bg, size_t bytes);
int bg_SetBitmapSize(bg_t *bg, int w, int h);
void bg_SetBitmapPixel(bg_t *bg, int x, int y, int value);
int bg_GetBitmapPixel(bg_t *bg, int x, int y);
int bg_GetBBXPixel(bg_t *bg, int x, int y);

void bg_ShowBitmap(bg_t *bg, bbx_t *bbx);
void bg_ReduceGlyph(bg_t *bg);
int bg_Max(bg_t *bg, bbx_t *max);		/* returns idx which describes glyph expansion */

/* bdf_rle.c */
int bg_rle_compress(bg_t *bg, bbx_t *bbx, unsigned rle_bits_per_0, unsigned rle_bits_per_1, int is_output);


#endif

