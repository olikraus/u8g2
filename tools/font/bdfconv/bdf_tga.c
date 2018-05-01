/*
  
  bdf_tga.c
  
  
  
  Modes:
    BDF_BBX_MODE_MINIMAL 0
    BDF_BBX_MODE_MAX 1
    BDF_BBX_MODE_HEIGHT 2
  
  For all modes, default reference should be the baseline. 
  This is required for mode 0, but may be optional for 1 and 2
  
  If (x,y) is the user provided baseline point for the glyph, then 
  the decoding mus tbe start at
    (x..., y-h-descent)
    
  
  BDF_BBX_MODE_MINIMAL
    - exact space as intended by the font author
    - glyphs my overlap ("mj" with osb18)
    
  BDF_BBX_MODE_MAX
    - extra space may be added
    - glyphs do not overlap
  
  BDF_BBX_MODE_HEIGHT
    - extra space may be added
    - glyphs do not overlap
    
  
*/


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "fd.h"


static uint16_t tga_width;
static uint16_t tga_height;
static uint16_t tga_used_height;
static uint8_t *tga_data = NULL;

static uint8_t *tga_font;
static int glyph_cnt;
static int bits_per_0;
static int bits_per_1;
static int bits_per_char_width;
static int bits_per_char_height;
static int bits_per_char_x;
static int bits_per_char_y;
static int bits_per_delta_x;
static int char_width;
static int char_height;
static int char_descent;
static unsigned unicode_start_pos;
static int tga_pixel_intersection;


int tga_get_char_width(void)
{
    return char_width;
}

int tga_get_char_height(void)
{
    return char_height;
}

int tga_init(uint16_t w, uint16_t h)
{
  tga_width = 0;
  tga_height = 0;
  tga_used_height = 0;
  tga_pixel_intersection = 0;
  if ( tga_data != NULL )
    free(tga_data);
  tga_data = (uint8_t *)malloc((size_t)w*(size_t)h*3);
  if ( tga_data == NULL )
    return 0;
  tga_width = w;
  tga_height = h;
  memset(tga_data, 255, tga_width*tga_height*3);
  return 1;
}

void tga_clear(void)
{
  memset(tga_data, 255, tga_width*tga_height*3);
}

void tga_set_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t *p;
  if ( y>= tga_height)
    return;
  if ( x>= tga_width)
    return;
  if ( tga_used_height < y )
    tga_used_height = y;
  
  p = tga_data + (tga_height-y-1)*(size_t)tga_width*3 + (size_t)x*3;
  
  if ( p[0] != 255 || p[1] != 255 || p[2] != 255 )
	  tga_pixel_intersection = 1;
  
  //printf("tga_set_pixel %d %d\n", x, y);
  
  *p++ = b;
  *p++ = g;
  *p++ = r;
}

int tga_is_pixel_intersection(void)
{
	return tga_pixel_intersection;
}

void tga_clear_pixel_intersection(void)
{
	tga_pixel_intersection = 0;
}

void tga_write_byte(FILE *fp, uint8_t byte)
{
  fputc(byte, fp);
}

void tga_write_word(FILE *fp, uint16_t word)
{
  tga_write_byte(fp, word&255);
  tga_write_byte(fp, word>>8);
}

void tga_save(const char *name)
{
  FILE *fp;
  fp = fopen(name, "wb");
  if ( fp != NULL )
  {
    tga_write_byte(fp, 0);		/* no ID */
    tga_write_byte(fp, 0);		/* no color map */
    tga_write_byte(fp, 2);		/* uncompressed true color */
    tga_write_word(fp, 0);		
    tga_write_word(fp, 0);		
    tga_write_byte(fp, 0);		
    tga_write_word(fp, 0);		/* x origin */
    tga_write_word(fp, 0);		/* y origin */
    tga_write_word(fp, tga_width);		/* width */
    tga_write_word(fp, tga_used_height+1);		/* height */
    tga_write_byte(fp, 24);		/* color depth */
    tga_write_byte(fp, 0);		
    fwrite(tga_data + (tga_height - (tga_used_height+1))*tga_width*3 , tga_width*3, tga_used_height+1, fp);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    fwrite("TRUEVISION-XFILE.", 18, 1, fp);
    fclose(fp);
  }
}

/*
  font data:

  offset	bytes	description
  0		1		glyph_cnt		number of glyphs
  1		1		bbx_mode	0: proportional, 1: common height, 2: monospace, 3: multiple of 8
  2		1		bits_per_0	glyph rle parameter
  3		1		bits_per_1	glyph rle parameter

  4		1		bits_per_char_width		glyph rle parameter
  5		1		bits_per_char_height	glyph rle parameter
  6		1		bits_per_char_x		glyph rle parameter
  7		1		bits_per_char_y		glyph rle parameter
  8		1		bits_per_delta_x		glyph rle parameter

  9		1		max_char_width
  10		1		max_char_height
  11		1		x offset
  12		1		y offset (descent)
  
  13		1		ascent (capital A)
  14		1		descent (lower g)
  15		1		ascent '('
  16		1		descent ')'
  
  17		1		start pos 'A' high byte
  18		1		start pos 'A' low byte

  19		1		start pos 'a' high byte
  20		1		start pos 'a' low byte
  
  21		1		start pos unicode high byte
  22		1		start pos unicode low byte

*/

void tga_set_font(uint8_t *font)
{
    glyph_cnt = *font++;
    font++;		/* bbx mode */
    bits_per_0 = *font++;
    bits_per_1 = *font++;
    bits_per_char_width = *font++;
    bits_per_char_height = *font++;
    bits_per_char_x = *font++;
    bits_per_char_y = *font++;
    bits_per_delta_x = *font++;
    char_width = *font++;
    char_height = *font++;
    font++;	/* x offset */
    char_descent = *(int8_t *)font;
    font++;
  
    font++;
    font++;
    font++;
    font++;

    font++;
    font++;
    font++;
    font++;
    
    unicode_start_pos = *font++;
    unicode_start_pos <<= 8;
    unicode_start_pos |= *font++;
  
    tga_font = font;
  
}

uint8_t *tga_get_glyph_data(uint16_t encoding)
{
  uint8_t *font = tga_font;
  if ( encoding <= 255 )
  {
    for(;;)
    {
      if ( font[1] == 0 )
	break;
      if ( font[0] == encoding )
      {
	return font;
      }
      font += font[1];
    }
  }
  else
  {
    uint16_t e;
    uint8_t *unicode_lookup_table;
    font += unicode_start_pos;
    unicode_lookup_table = font;
    
    
    /* search for the glyph start in the unicode lookup table */
    do
    {
      font += ((unicode_lookup_table[0]<<8)|unicode_lookup_table[1]);
      unicode_lookup_table+=2;
      e = (unicode_lookup_table[0]<<8)|unicode_lookup_table[1];
      unicode_lookup_table+=2;
    } while( e < encoding );
    
    /* continue with the search in the font */
    for(;;)
    {
      e = ((font[0]<<8)|font[1]);
      if ( e == 0 )
	break;
      if ( e == encoding )
      {
	return font;
      }
      font += font[2];
    }
  }
  return NULL;
}


/* font decode */
struct tga_fd_struct
{
  unsigned target_x;
  unsigned target_y;
  unsigned is_transparent;
  
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
typedef struct tga_fd_struct tga_fd_t;

/* increment x and consider line wrap (inc y)*/
/* old procedure */
void tga_fd_inc(tga_fd_t *f)
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


unsigned tga_fd_get_unsigned_bits(tga_fd_t *f, unsigned cnt)
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
int tga_fd_get_signed_bits(tga_fd_t *t, int cnt)
{
  return (int)tga_fd_get_unsigned_bits(t, cnt) - ((1<<cnt)>>1);
}


void tga_fd_draw_fg_pixel(tga_fd_t *f, unsigned cnt)
{
  //printf("%d ", cnt);
  /* cnt can be zero */
  while( cnt > 0 )
  {
    cnt--;
    tga_set_pixel(f->target_x+f->x+cnt, f->target_y+f->y, 0,0,0);
  }
}

void tga_fd_draw_bg_pixel(tga_fd_t *f, unsigned cnt)
{
  //printf("%d ", cnt);
  /* cnt can be zero */
  while( cnt > 0 )
  {
    cnt--;
    if ( f->is_transparent == 0 )
      tga_set_pixel(f->target_x+f->x+cnt, f->target_y+f->y, 0x0e8,0x0e8,0x0e8);
  }
}

void  tga_draw_hline(unsigned x,unsigned y, unsigned cnt,  unsigned is_foreground)
{
  while( cnt > 0 )
  {
    cnt--;
    if ( is_foreground == 0 )
      tga_set_pixel(x+cnt, y, 0x0e8,0x0e8,0x0e8);
    else
      tga_set_pixel(x+cnt, y, 255,0,0);
  }
}


void tga_fd_draw_pixel(tga_fd_t *f, unsigned cnt, unsigned is_foreground)
{
  if ( is_foreground )
  {
    tga_fd_draw_fg_pixel(f, cnt);
  }
  else
  {
    tga_fd_draw_bg_pixel(f, cnt);
  }
}

void tga_fd_decode_len(tga_fd_t *f, unsigned len, unsigned is_foreground)
{
  unsigned cnt, rem;
  cnt = len;
  for(;;)
  {
    rem = f->glyph_width;
    rem -= f->x;
    if ( cnt < rem )
      break;
    tga_fd_draw_pixel(f,rem, is_foreground);
    cnt -= rem;
    f->x = 0;
    f->y++;
  }
  tga_fd_draw_pixel(f, cnt, is_foreground);
  f->x += cnt;
}

unsigned tga_fd_decode(tga_fd_t *f, uint8_t *glyph_data, int is_unicode)
{
  unsigned a, b;
  //unsigned cnt, rem;
  int x, y;
  unsigned d = 0;
    
  f->decode_ptr = glyph_data;
  f->decode_bit_pos = 0;
  
  f->decode_ptr += 1;
  f->decode_ptr += 1;
  if ( is_unicode != 0 )
    f->decode_ptr += 1;
    
  
  f->glyph_width = tga_fd_get_unsigned_bits(f, bits_per_char_width);
  f->glyph_height = tga_fd_get_unsigned_bits(f, bits_per_char_height);
  x = tga_fd_get_signed_bits(f, bits_per_char_x);
  y = tga_fd_get_signed_bits(f, bits_per_char_y);
  d = tga_fd_get_signed_bits(f, bits_per_delta_x);
  
  if ( f->glyph_width > 0 )
  {
    
    f->target_x += x;
    f->target_y -= f->glyph_height ;
    f->target_y -=y ;
        
    /* reset local x/y position */
    f->x = 0;
    f->y = 0;
    
    /* decode glyph */
    for(;;)
    {
      a = tga_fd_get_unsigned_bits(f, bits_per_0);
      b = tga_fd_get_unsigned_bits(f, bits_per_1);
      do
      {
	tga_fd_decode_len(f, a, 0);
	tga_fd_decode_len(f, b, 1);
      } while( tga_fd_get_unsigned_bits(f, 1) != 0 );

      if ( f->y >= f->glyph_height )
	break;
    }
  }
  return d;
}


unsigned tga_draw_glyph(unsigned x, unsigned y, uint16_t encoding, int is_hints)
{
  unsigned dx = 0;
  tga_fd_t f;
  f.target_x = x;
  f.target_y = y;
  f.is_transparent = !is_hints;
  uint8_t *glyph_data = tga_get_glyph_data(encoding);	/* better skip the first 2 or 3 bytes */
  if ( glyph_data != NULL )
  {
    dx = tga_fd_decode(&f, glyph_data, encoding >= 255 ? 1 : 0);
    if ( is_hints )
    {
      tga_set_pixel(x+dx, y, 28,133,240);	/* orange: reference point */
      tga_set_pixel(x, y, 255,164,0);	/* blue: delta x (width) for this glyph */
    }
  }
  return dx;
}

unsigned tga_draw_string(unsigned x, unsigned y, const char *s, int is_hints, unsigned max_dx)
{
  unsigned dx = 0;
  while( *s != '\0' )
  {
    dx += tga_draw_glyph(x+dx,y,*s, is_hints);
    if ( max_dx > 0 )
      if ( dx > max_dx )
	break;
    s++;
  }
  return dx;
}


