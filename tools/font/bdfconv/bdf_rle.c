/*

  bdf_rle.c
  
  run length glyph encoding
  
  
  size comparison with old and new font format
  4x6		1500	1469
  4x6r		734		726
  6x10		1866	2009
  6x10r		889		971
  7x13B		2172	2253
  7x13Br		1041	1017
  9x15		2959	2649
  9x15r		1427	1242
  10x20		3453	3053
  10x20r		1667	1417	85%
  courB12		3959	3312
  courB12r	1857	1538
  courB24		10502      6661
  courB24r	4775	3015	63%
  helvB24		10931	6904	63%
  helvB24r	4992	3166	63%
  logisoso50r	14375	5248	36%
  
  
*/

/* font information */
/*
    glyph_cnt = *font++;
    bits_per_0 = *font++;
    bits_per_1 = *font++;
    bits_per_char_width = *font++;
    bits_per_char_height = *font++;
    bits_per_char_x = *font++;
    bits_per_char_y = *font++;
    bits_per_delta_x = *font++;

*/

  /* apply glyph information */
  /*  
    ~		encoding			unsigned, 1 or 2 byte (high byte first in two byte version)
    ~		total size			unsigned, 1 byte
    ~             BBX width                                       unsigned	5
    ~             BBX height                                      unsigned	5
    ~             BBX xoffset                                    signed		2
    ~             BBX yoffset                                    signed		5
    ~             DWIDTH                                          unsigned		3


*/

#define BDF_RLE_FONT_GLYPH_START 23

/* max glyphs count is around 7500, 7500/100 = 75 */

#define UNICODE_GLYPHS_PER_LOOKUP_TABLE_ENTRY 100

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "bdf_font.h"

#ifdef OLD_CODE

#define SCREEN_W 140
#define SCREEN_H 140

uint8_t screen_buf[SCREEN_H][SCREEN_W];
unsigned screen_max_y;

void screen_init(void)
{
  unsigned x, y;
  screen_max_y = 0;
  for( y = 0; y < SCREEN_H; y++ )
  {
    for( x = 0; x < SCREEN_W; x++ )
    {
      screen_buf[y][x] = '.';
    }
  }
}

void screen_set_pixel(unsigned x, unsigned y, uint8_t c)
{
  if ( screen_max_y < y)
    screen_max_y = y;
  screen_buf[y][x] = c;
}

void screen_show(void)
{
  unsigned x, y;
  printf("\n");
  for( y = 0; y <= screen_max_y; y++ )
  {
    for( x = 0; x < SCREEN_W; x++ )
    {
      printf("%c", screen_buf[y][x]);
    }
    printf("\n");
  }
}

/*===================================================*/

/* font decode */
struct fd_struct
{
  unsigned x;						/* local coordinates, (0,0) is upper left */
  unsigned y;
  unsigned glyph_width;	
  unsigned glyph_height;

  
  const uint8_t *decode_ptr;			/* pointer to the compressed data */
  unsigned decode_bit_pos;			/* bitpos inside a byte of the compressed data */
  
  uint8_t bbx_x_max_bit_size;
  uint8_t bbx_y_max_bit_size;
  uint8_t bbx_w_max_bit_size;
  uint8_t bbx_h_max_bit_size;
  uint8_t dx_max_bit_size;
  
};
typedef struct fd_struct fd_t;

/* increment x and consider line wrap (inc y)*/
static void fd_inc(fd_t *f)
{
  unsigned x = f->x;
  x++;
  if ( x == f->glyph_width )
  {
    x = 0;
    f->y++;
  }
  f->x = x;
}


static unsigned fd_get_unsigned_bits(fd_t *f, unsigned cnt)
{
  unsigned val;
  unsigned bit_pos = f->decode_bit_pos;
  
  val = *(f->decode_ptr);
  
  val >>= bit_pos;
  if ( bit_pos + cnt >= 8 )
  {
    f->decode_ptr++;
    val |= *(f->decode_ptr) << (8-bit_pos);
    bit_pos -= 8;
  }
  val &= (1U<<cnt)-1;
  bit_pos += cnt;
  
  f->decode_bit_pos = bit_pos;
  return val;
}

/*
    2 bit --> cnt = 2
      -2,-1,0. 1

    3 bit --> cnt = 3
      -2,-1,0. 1
      -4,-3,-2,-1,0,1,2,3

      if ( x < 0 )
	r = bits(x-1)+1;
    else
	r = bits(x)+1;

*/
static int fd_get_signed_bits(fd_t *t, int cnt)
{
  return (int)fd_get_unsigned_bits(t, cnt) - ((1<<cnt)>>1);
}


static void fd_draw_pixel(fd_t *f)
{
  screen_set_pixel(f->x, f->y, '#');
}

static void fd_decode(bg_t *bg, bbx_t *bbx, fd_t *f, unsigned rle_bits_per_0, unsigned rle_bits_per_1)
{
  unsigned a, b;
  unsigned i;
  
  screen_init();

  if ( bbx == NULL )
    bbx = &(bg->bbx);
  
  /* init decode algorithm */
  f->decode_ptr = bg->target_data;
  f->decode_bit_pos = 0;
  f->glyph_width = bbx->w;
  f->glyph_height = bbx->h;

  
  /* read glyph info */
  f->decode_ptr += 2;
  
  fd_get_unsigned_bits(f, f->bbx_w_max_bit_size);
  fd_get_unsigned_bits(f, f->bbx_h_max_bit_size);
  fd_get_signed_bits(f, f->bbx_x_max_bit_size);
  fd_get_signed_bits(f, f->bbx_y_max_bit_size);
  fd_get_signed_bits(f, f->dx_max_bit_size);
  
  /* reset local x/y position */
  f->x = 0;
  f->y = 0;
  
  //puts("");
  
  /* decode glyph */
  for(;;)
  {
    a = fd_get_unsigned_bits(f, rle_bits_per_0);
    b = fd_get_unsigned_bits(f, rle_bits_per_1);
    //printf("[%u %u]", a, b);
    do
    {
      for( i = 0; i < a; i++ )
      {
	fd_inc(f);
      }

      for( i = 0; i < b; i++ )
      {	
	fd_draw_pixel(f);
	fd_inc(f);
      }
      
    } while( fd_get_unsigned_bits(f, 1) != 0 );
    
    if ( f->y >= f->glyph_height )
      break;
  }
  
  screen_show();
}

#endif 

/*===================================================*/


/*
  Desc:
    Output a and b to the stream.
    a and b must fit to the target size in bits.
    Additionally a repeat code r (one bit) is generated:
    It may look like this:
      r = 0: 0aaaabb
    or 
      r = 1: 1
    If r is 0, then the number of zeros (a) and ones (b) will follow and both
    values must be stored as in the decoder.
    If r os 1, then the number of zeros and ones is repeated once
  Args:
    a: number of 0 bits, log2(a) must be smaller or equal to the fieldsize
    b: number of 1 bits, log2(b) must be smaller or equal to the fieldsize
*/

static void bg_err(const char *s)
{
  puts(s);
}

static void bg_init_rle(bg_t *bg, unsigned rle_bits_per_0, unsigned rle_bits_per_1)
{
  bg->rle_bitcnt = 0;
  bg->rle_is_first = 1;
  bg->rle_bits_per_0 = rle_bits_per_0;
  bg->rle_bits_per_1 = rle_bits_per_1;
  bg->rle_last_0 = 0;
  bg->rle_last_1 = 1;
  bg_ClearTargetData(bg);
}
  
static int bg_01_rle(bg_t *bg, unsigned a, unsigned b)
{
  if ( bg->rle_is_first == 0 && bg->rle_last_0 == a && bg->rle_last_1 == b )
  {
    bg->rle_bitcnt++;
    if ( bg_AddTargetBits(bg, 1, 1) == 0 )
      return bg_err("error in bg_01_rle 1 0"), 0;
  }
  else
  {   
    if ( bg->rle_is_first == 0 )
    {
      if ( bg_AddTargetBits(bg, 1, 0) == 0 )
	return bg_err("error in bg_01_rle 1 0"), 0;
      bg->rle_bitcnt++;
    }
    if ( bg_AddTargetBits(bg, bg->rle_bits_per_0, a) == 0 )
      return bg_err("error in bg_01_rle 1 a"), 0;
    if ( bg_AddTargetBits(bg, bg->rle_bits_per_1, b) == 0 )
      return bg_err("error in bg_01_rle 1 b"), 0;
    
  
    /*
    if ( bg->encoding == ' ' )
    {
      printf("[%u %u]", a, b);
    }
    */	
    
    
    bg->rle_is_first = 0;
    bg->rle_bitcnt +=bg->rle_bits_per_0;
    bg->rle_bitcnt +=bg->rle_bits_per_1;
    bg->rle_last_0 = a;
    bg->rle_last_1 = b;
  }
  return 1;
}

/*
  Desc:
    Write the number of zeros and ones to the bit stream.
    There is no restriction on the size of a and b.
  Args:
    a: number of 0 bits
    b: number of 1 bits
  
*/
static int bg_prepare_01_rle(bg_t *bg, unsigned a, unsigned b)
{
  //printf("[%u %u]", a, b);
  while( a >= (1<<bg->rle_bits_per_0) -1 )
  {
    if ( bg_01_rle(bg, (1<<bg->rle_bits_per_0) -1, 0) == 0 )
      return 0;
    a -= (1<<bg->rle_bits_per_0) -1;
  }
  while( b >= (1<<bg->rle_bits_per_1) -1 )
  {
    if ( bg_01_rle(bg, a, (1<<bg->rle_bits_per_1) -1) == 0 )
      return 0;
    a = 0;
    b -= (1<<bg->rle_bits_per_1) -1;
  }
  if ( a != 0 || b != 0 )
    if ( bg_01_rle(bg, a, b) == 0 )
      return 0;
  return 1;
}


int bg_rle_compress(bg_t *bg, bbx_t *bbx, unsigned rle_bits_per_0, unsigned rle_bits_per_1, int is_output)
{
  int x;
  int y;
  int i;
  int bd_is_one;	/* bit delta */
  int bd_curr_len;
  int bd_max_len;
  int bd_chg_cnt;
  
  static int bd_list[1024*2];

  if ( bbx == NULL )
    bbx = &(bg->bbx);

  bg_init_rle(bg, rle_bits_per_0, rle_bits_per_1);
  

  /* step 0: output initial information */
  //printf("%ld %ld\n", (long)bg->encoding, (long)bg->map_to);
  if ( bg->map_to <= 255 )
  {
    if ( bg_AddTargetData(bg, bg->map_to) < 0 )
      return bg_err("error in bg_rle_compress"), 0;
  }
  else
  {
    if ( bg_AddTargetData(bg, bg->map_to >> 8) < 0 )
      return bg_err("error in bg_rle_compress"), 0;
    if ( bg_AddTargetData(bg, bg->map_to & 255 ) < 0 )
      return bg_err("error in bg_rle_compress"), 0;
  }
  /* size, will be added later */
  if ( bg_AddTargetData(bg, 0) < 0 )
    return bg_err("error in bg_rle_compress"), 0;
  // if ( bbx->w == 0 && bbx->h == 0 )
  // {
  //   printf("blank char: enc=%ld\n", bg->encoding);
  // }
  // w & h is 0 for the space glyphe (encoding 32)
  if ( bg_AddTargetBits(bg, bg->bf->bbx_w_max_bit_size, bbx->w) == 0 )
    return bg_err("error in bg_rle_compress"), 0;
  if ( bg_AddTargetBits(bg, bg->bf->bbx_h_max_bit_size, bbx->h) == 0 )
    return bg_err("error in bg_rle_compress"), 0;
  if ( bg_AddTargetBits(bg, bg->bf->bbx_x_max_bit_size, bbx->x + (1<<(bg->bf->bbx_x_max_bit_size-1))) == 0 )
    return bg_err("error in bg_rle_compress"), 0;
  if ( bg_AddTargetBits(bg, bg->bf->bbx_y_max_bit_size, bbx->y + (1<<(bg->bf->bbx_y_max_bit_size-1))) == 0 )
    return bg_err("error in bg_rle_compress"), 0;

  if ( bg->bf->bbx_mode == BDF_BBX_MODE_MINIMAL )
  {
    if ( bg_AddTargetBits(bg, bg->bf->dx_max_bit_size, bg->dwidth_x + (1<<(bg->bf->dx_max_bit_size-1))) == 0 )
      return bg_err("error in bg_rle_compress"), 0;
  }
  else if ( bg->bf->bbx_mode == BDF_BBX_MODE_MAX )
  {
    if ( bg_AddTargetBits(bg, bg->bf->dx_max_bit_size, bbx->w+ (1<<(bg->bf->dx_max_bit_size-1))) == 0 )
      return bg_err("error in bg_rle_compress"), 0;
  }
  else
  {
    if ( bg_AddTargetBits(bg, bg->bf->dx_max_bit_size, bbx->w+ (1<<(bg->bf->dx_max_bit_size-1))) == 0 )
      return bg_err("error in bg_rle_compress"), 0;
  }
  
  bd_is_one = 0;
  bd_curr_len = 0;
  bd_max_len = 0;
  bd_chg_cnt = 0;

  /* step 1: build array with pairs of a (number of zero bits) and b (number of one bits) */

  for( y = bbx->y+bbx->h-1; y >= bbx->y; y--)
  {
    for( x = bbx->x; x < bbx->x + bbx->w; x++)
    {
      if ( bg_GetBBXPixel(bg, x, y) == 0 )
      {
	if ( bd_is_one != 0 )
	{
	  bd_list[bd_chg_cnt] = bd_curr_len;
	  bd_is_one = 0;
	  bd_chg_cnt++;
	  bd_curr_len = 0;
	}
	bd_curr_len++;	
      }
      else
      {
	if ( bd_is_one == 0 )
	{
	  bd_list[bd_chg_cnt] = bd_curr_len;
	  bd_is_one = 1;
	  bd_chg_cnt++;
	  bd_curr_len = 0;
	}
	bd_curr_len++;	
      }
      
      if ( bd_max_len < bd_curr_len )
	bd_max_len = bd_curr_len;      
    }
  }
  
 
  bd_list[bd_chg_cnt] = bd_curr_len;
  bd_chg_cnt++;
  
  if ( (bd_chg_cnt & 1) == 1 )
  {
    assert(bd_is_one == 0);
    bd_list[bd_chg_cnt] = 0;
    bd_chg_cnt++;
  }
  
  //printf("01 pairs = %d\n", bd_chg_cnt/2);
  
  /* step 2: convert the array into bit stream */
  
  //if ( bg->encoding == ' '  )
  //  printf("Encoding list, pairs = %d\n", bd_chg_cnt/2);
  
  for( i = 0; i < bd_chg_cnt; i+=2 )
  {
    //if ( bg->encoding == ' ' )
    //  printf("(%d %d)", bd_list[i], bd_list[i+1]);
    
    if ( bg_prepare_01_rle(bg, bd_list[i], bd_list[i+1]) == 0 )
      return 0;
  }
  

  //if ( bg->encoding == 'B'  )
  //  printf("\nEncoding list end\n");
  
  if ( bg_AddTargetBits(bg, 1, 0) == 0 )		// ensure that there is a 0 bit at the end. This will simplify decoding loop
    return 0;

  if ( bg_FlushTargetBits(bg) == 0 )			// finish the last byte and update bg->target_cnt 
    return 0;


  if ( bg->map_to <= 255 )
  {
    bg->target_data[1] = bg->target_cnt;
  }
  else
  {
    bg->target_data[2] = bg->target_cnt;
  }
  
  /*
  {
    fd_t f;
    f.bbx_x_max_bit_size = bg->bf->bbx_x_max_bit_size;
    f.bbx_y_max_bit_size = bg->bf->bbx_y_max_bit_size;
    f.bbx_w_max_bit_size = bg->bf->bbx_w_max_bit_size;
    f.bbx_h_max_bit_size = bg->bf->bbx_h_max_bit_size;
    f.dx_max_bit_size = bg->bf->dx_max_bit_size;
  
    fd_decode(bg, bbx, &f, rle_bits_per_0, rle_bits_per_1);
  }
  */
  
  return 1;
}

unsigned long bf_RLECompressAllGlyphsWithFieldSize(bf_t *bf, int rle_0, int rle_1, int is_output)
{
  int i;
  bg_t *bg;
  unsigned long total_bits = 0;
  bbx_t local_bbx;
  
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 )
    {
      bf_copy_bbx_and_update_shift(bf, &local_bbx, bg);
#ifdef OLD_CLODE
      /* modifing the following code requires update ind bdf_font.c also */

      if ( bf->bbx_mode == BDF_BBX_MODE_MINIMAL )
      {
	local_bbx = bg->bbx;	
      }
      else if ( bf->bbx_mode == BDF_BBX_MODE_MAX )
      {
	local_bbx = bf->max;	
	local_bbx.x = 0;
	if ( bg->bbx.x < 0 )
	  bg->shift_x = bg->bbx.x;
	if ( local_bbx.w < bg->dwidth_x )
	  local_bbx.w = bg->dwidth_x;
      }
      else if ( bf->bbx_mode == BDF_BBX_MODE_M8 )
      {
	local_bbx.w = bf->max.w;
	if ( local_bbx.w < bg->dwidth_x )
	  local_bbx.w = bg->dwidth_x;
	local_bbx.w = (local_bbx.w+7) & ~7;
	local_bbx.h = (bf->max.h+7) & ~7;
	local_bbx.x = bf->max.x;
	local_bbx.y = bf->max.y;
	local_bbx.x = 0;
	if ( bg->bbx.x < 0 )
	  bg->shift_x = bg->bbx.x;
      }
      else
      {
	local_bbx = bf->max;
	local_bbx.w = bg->bbx.w;	
	local_bbx.x = bg->bbx.x;
		
	local_bbx.x = 0;
	if ( bg->bbx.x < 0 )
	{
	  /* e.g. "j" */
	  local_bbx.w -= bg->bbx.x;
	  bg->shift_x = bg->bbx.x;
	}
	else
	{
	  /* e.g. "B" */
	  local_bbx.w += bg->bbx.x;
	  //bg->shift_x = bg->bbx.x;
	}
	if ( local_bbx.w < bg->dwidth_x )
	  local_bbx.w = bg->dwidth_x;
	
	
      }
#endif

      bg_rle_compress(bg, &local_bbx, rle_0, rle_1, is_output); 
      total_bits += bg->target_cnt*8+bg->target_bit_pos;
      if ( is_output != 0 )
      {
	bf_Log(bf, "RLE Compress: Encoding %ld bits %u/%u", bg->encoding, bg->rle_bitcnt, bg->target_cnt*8+bg->target_bit_pos);  
      }
    }
  }
  //bf_Log(bf, "RLE Compress: zero bits %d, one bits %d, total bit size %lu", rle_0, rle_1, total_bits);
  return total_bits;
}


unsigned bf_RLE_get_glyph_data(bf_t *bf, uint8_t encoding)
{
  uint8_t *font = bf->target_data;
  font += BDF_RLE_FONT_GLYPH_START;
  for(;;)
  {
    if ( font[1] == 0 )
      break;
    if ( font[0] == encoding )
    {
      return (font-bf->target_data)-BDF_RLE_FONT_GLYPH_START;
    }
    font += font[1];
  }
  return 0;
}


void bf_RLECompressAllGlyphs(bf_t *bf)
{
  int i, j;
  bg_t *bg;
  
  int rle_0, rle_1;
  int best_rle_0=0, best_rle_1= 0;
  unsigned long total_bits = 0;
  unsigned long min_total_bits = 0xffffffff;
  
  int idx_cap_a;
  int idx_cap_a_ascent;
  int idx_1;
  int idx_1_ascent;
  int idx_g;
  int idx_g_descent;
  int idx_para;
  int idx_para_ascent;
  int idx_para_descent;
  
  unsigned pos;
  unsigned ascii_glyphs;
  unsigned unicode_start_pos;
  unsigned unicode_lookup_table_len;  
  uint32_t unicode_lookup_table_start;
  uint32_t unicode_last_delta;
  uint32_t unicode_last_target_cnt;
  unsigned unicode_lookup_table_pos;
  unsigned unicode_lookup_table_glyph_cnt;
  
  idx_cap_a_ascent = 0;
  idx_cap_a = bf_GetIndexByEncoding(bf, 'A');
  if ( idx_cap_a >= 0 )
  {
    idx_cap_a_ascent = bf->glyph_list[idx_cap_a]->bbx.h+bf->glyph_list[idx_cap_a]->bbx.y;
  }

  idx_1_ascent = 0;
  idx_1 = bf_GetIndexByEncoding(bf, '1');
  if ( idx_1 >= 0 )
  {
    idx_1_ascent = bf->glyph_list[idx_1]->bbx.h+bf->glyph_list[idx_1]->bbx.y;
  }

  idx_g_descent = 0;
  idx_g = bf_GetIndexByEncoding(bf, 'g');
  if ( idx_g >= 0 )
  {
    idx_g_descent = bf->glyph_list[idx_g]->bbx.y;
  }


  idx_para_ascent = 0;
  idx_para = bf_GetIndexByEncoding(bf, '(');
  if ( idx_para >= 0 )
  {
    idx_para_ascent = bf->glyph_list[idx_para]->bbx.h+bf->glyph_list[idx_para]->bbx.y;
    idx_para_descent = bf->glyph_list[idx_para]->bbx.y;
  }
  else
  {
    idx_para_ascent = idx_cap_a_ascent;
    if ( idx_para_ascent == 0 )
      idx_para_ascent = idx_1_ascent;
      
    idx_para_descent = idx_g_descent;
  }

  
  for( rle_0 = 2; rle_0 < 9; rle_0++ )
  {
    for( rle_1 = 2; rle_1 < 7; rle_1++ )
    {
      total_bits = bf_RLECompressAllGlyphsWithFieldSize(bf, rle_0, rle_1, 0);
      if ( min_total_bits > total_bits )
      {
	min_total_bits = total_bits;
	best_rle_0 = rle_0;
	best_rle_1 = rle_1;
      }
      
    }
  }
  bf_Log(bf, "RLE Compress: best zero bits %d, one bits %d, total bit size %lu", best_rle_0, best_rle_1, min_total_bits);
  bf_RLECompressAllGlyphsWithFieldSize(bf, best_rle_0, best_rle_1, 0);


  bf_ClearTargetData(bf);

  /*
    glyph_cnt = *font++;
    bits_per_0 = *font++;
    bits_per_1 = *font++;
    bits_per_char_width = *font++;
    bits_per_char_height = *font++;
    bits_per_char_x = *font++;
    bits_per_char_y = *font++;
    bits_per_delta_x = *font++;
  */

  bf_Log(bf, "RLE Compress: Font code generation, selected glyphs=%d, total glyphs=%d", bf->selected_glyphs, bf->glyph_cnt);
  
  /* 0 */
  bf_AddTargetData(bf, bf->selected_glyphs);
  bf_AddTargetData(bf, bf->bbx_mode);
  bf_AddTargetData(bf, best_rle_0);
  bf_AddTargetData(bf, best_rle_1);

  /* 4 */
  bf_AddTargetData(bf, bf->bbx_w_max_bit_size);
  bf_AddTargetData(bf, bf->bbx_h_max_bit_size);
  bf_AddTargetData(bf, bf->bbx_x_max_bit_size);
  bf_AddTargetData(bf, bf->bbx_y_max_bit_size);
  bf_AddTargetData(bf, bf->dx_max_bit_size);
  
  /* 9 */
  bf_AddTargetData(bf, bf->max.w);
  bf_AddTargetData(bf, bf->max.h);
  bf_AddTargetData(bf, bf->max.x);
  bf_AddTargetData(bf, bf->max.y);

  /* 13 */
  if ( idx_cap_a_ascent > 0 )
    bf_AddTargetData(bf, idx_cap_a_ascent);
  else
    bf_AddTargetData(bf, idx_1_ascent);
  bf_AddTargetData(bf, idx_g_descent);
    
  /* 15 */
  bf_AddTargetData(bf, idx_para_ascent);
  bf_AddTargetData(bf, idx_para_descent);

  /* 17 */
  bf_AddTargetData(bf, 0);	/* start pos 'A', high/low */
  bf_AddTargetData(bf, 0);

  /* 19 */
  bf_AddTargetData(bf, 0);	/* start pos 'a', high/low */
  bf_AddTargetData(bf, 0);

  /* 21 */
  bf_AddTargetData(bf, 0);	/* start pos unicode, high/low */
  bf_AddTargetData(bf, 0);

  /* assumes, that map_to is sorted */

  ascii_glyphs = 0;
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 && bg->map_to <= 255L )
    {
      if ( bg->target_data != NULL )
      {

	if ( bg->target_cnt >= 255 )
	{
	  bf_Error(bf, "RLE Compress: Error, glyph too large, encoding=%ld cnt=%d", (long)bg->encoding, (int)bg->target_cnt);
	  exit(1);
	}

	for( j = 0; j < bg->target_cnt; j++ )
	{
	  bf_AddTargetData(bf, bg->target_data[j]);
	}
	ascii_glyphs++;		/* calculate the numner of ascii glyphs, this is required later for the unicode index table */
      }
    }
  }
  /* add empty glyph as end of font marker for the ASCII part (chars from 0 to 255) */
  bf_AddTargetData(bf, 0);
  bf_AddTargetData(bf, 0);
  
  unicode_start_pos = bf->target_cnt-BDF_RLE_FONT_GLYPH_START;
  /* 
  1 May 2018: Unicode lookup table 
  */
  bf_Log(bf, "RLE Compress: ASCII gylphs=%d, Unicode glyphs=%d", ascii_glyphs, bf->selected_glyphs-ascii_glyphs);
  unicode_lookup_table_len = (bf->selected_glyphs-ascii_glyphs) / UNICODE_GLYPHS_PER_LOOKUP_TABLE_ENTRY;
  //if ( unicode_lookup_table_len > 1 )		
  //  unicode_lookup_table_len--;			
  bf_Log(bf, "RLE Compress: Glyphs per unicode lookup table entry=%d", UNICODE_GLYPHS_PER_LOOKUP_TABLE_ENTRY);
  unicode_lookup_table_start = bf->target_cnt;
  
  /* write n-1 entries */
  for( i = 1; i < unicode_lookup_table_len; i++ )
  {
    bf_AddTargetData(bf, 0);	/* offset */
    bf_AddTargetData(bf, 0);
    bf_AddTargetData(bf, 0);	/* encoding */
    bf_AddTargetData(bf, 0);
  }
  /* the last entry is special, it contains the encoding 0xffff */
  bf_AddTargetData(bf, 0);	/* offset */
  bf_AddTargetData(bf, 4);	/* default, if the table has only one entry, then just skip the table */
  bf_AddTargetData(bf, 0xff);	/* encoding */
  bf_AddTargetData(bf, 0xff);
  
  

  unicode_lookup_table_pos = 0;
  unicode_lookup_table_glyph_cnt = 0;
  unicode_last_delta = bf->target_cnt-unicode_lookup_table_start;   /* should be 4 if unicode_lookup_table_len == 0 */
  unicode_last_target_cnt = bf->target_cnt;
  /* now write chars with code >= 256 from the BMP */
  /* assumes, that map_to is sorted */
  for( i = 0; i < bf->glyph_cnt; i++ )
  { 
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 256 )
    {
      if ( bg->target_data != NULL )
      {

	if ( bg->target_cnt >= 255 )
	{
	  bf_Error(bf, "RLE Compress: Error, glyph too large, encoding=%ld", (long)bg->encoding);
	  exit(1);
	}

	for( j = 0; j < bg->target_cnt; j++ )
	{
	  bf_AddTargetData(bf, bg->target_data[j]);
	}
	
	/* update the unicode lookup table entry counter */
	unicode_lookup_table_glyph_cnt++;
	if  ( unicode_lookup_table_glyph_cnt  > UNICODE_GLYPHS_PER_LOOKUP_TABLE_ENTRY )
	{
	  /* ensure, that there is a table entry available */
	  if ( unicode_lookup_table_pos < unicode_lookup_table_len )
	  {
	    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4+0] = unicode_last_delta>>8;
	    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4+1] = unicode_last_delta&255;
	    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4+2] |= bg->encoding>>8;		// ensure to keep the 0x0ffff encoding at the end
	    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4+3] |= bg->encoding&255;		// ensure to keep the 0x0ffff encoding at the end 
	    
	    unicode_lookup_table_pos++;
	    unicode_lookup_table_glyph_cnt = 0;
	    unicode_last_delta = bf->target_cnt - unicode_last_target_cnt;
	    unicode_last_target_cnt = bf->target_cnt;
	  }
	}	
      }
    }
  }

  /* write pending block to the unicode lookup table, ensure, that there is a table entry available */
  if ( unicode_lookup_table_pos < unicode_lookup_table_len )
  {
    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4+0] = unicode_last_delta>>8;
    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4+1] = unicode_last_delta&255;
    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4+2] = 0xff;	
    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4+3] = 0xff;	
    unicode_lookup_table_pos++;
  }
  
  /* add empty encoding as end of font marker (note: this differs from the ASCII section) */
  bf_AddTargetData(bf, 0);
  bf_AddTargetData(bf, 0);

  bf_Log(bf, "RLE Compress: Unicode lookup table len=%d, written entries=%d", unicode_lookup_table_len, unicode_lookup_table_pos);
  bf_Log(bf, "RLE Compress: Unicode lookup table first entry: delta=%d, encoding=%d", 
    bf->target_data[unicode_lookup_table_start+0]*256+bf->target_data[unicode_lookup_table_start+1], 
    bf->target_data[unicode_lookup_table_start+2]*256+bf->target_data[unicode_lookup_table_start+3]);

  bf_Log(bf, "RLE Compress: Unicode lookup table last entry: delta=%d, encoding=%d", 
    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4-4+0]*256+bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4-4+1], 
    bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4-4+2]*256+bf->target_data[unicode_lookup_table_start+unicode_lookup_table_pos*4-4+3]);
  
  assert(unicode_lookup_table_len == unicode_lookup_table_pos );		// ensure that all table entries are filled
  
  pos = bf_RLE_get_glyph_data(bf, 'A');
  bf->target_data[17] = pos >> 8;
  bf->target_data[18] = pos & 255;
  
  pos = bf_RLE_get_glyph_data(bf, 'a');
  bf->target_data[19] = pos >> 8;
  bf->target_data[20] = pos & 255;

  bf->target_data[21] = unicode_start_pos >> 8;
  bf->target_data[22] = unicode_start_pos & 255;
  
  bf_Log(bf, "RLE Compress: 'A' pos = %u, 'a' pos = %u", bf_RLE_get_glyph_data(bf, 'A'), bf_RLE_get_glyph_data(bf, 'a'));
  
  bf_Log(bf, "RLE Compress: Font size %d", bf->target_cnt);
  
}



