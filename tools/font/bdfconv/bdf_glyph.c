

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bdf_glyph.h"


bg_t *bg_Open(void)
{
  bg_t *bg;
  
  bg = (bg_t *)malloc(sizeof(bg_t));
  if ( bg != NULL )
  {
    bg->encoding = 0;
    bg->map_to = -1;
    bg->bitmap_data = NULL;
    bg->target_data = NULL;
    bg->target_max = 0;
    bg->target_cnt = 0;
    bg->shift_x = 0;
    
    return bg;
  }
  return NULL;
}


void bg_Close(bg_t *bg)
{
  if ( bg->bitmap_data != NULL )
    free(bg->bitmap_data);
  if ( bg->target_data != NULL )
    free(bg->target_data);
  bg->bitmap_data = NULL;
  bg->target_data =NULL;
  bg->target_max = 0;
  bg->target_cnt = 0;
  bg->target_bit_pos = 0;
  bg->encoding = 0;
  free(bg);
}

static int bg_extend_target_data(bg_t *bg)
{
  int extend = 16;
  int i;
  void *ptr;
  if ( bg->target_data == NULL )
  {
    ptr = malloc(extend*sizeof(uint8_t));
    bg->target_max = 0;
  }
  else
  {
    ptr = realloc(bg->target_data, (bg->target_max + extend)*sizeof(uint8_t));
  }
  if ( ptr == NULL )
    return 0;
  bg->target_data = (uint8_t *)ptr;
  for( i = bg->target_max; i < bg->target_max + extend; i++ )
      bg->target_data[i] = 0;
  bg->target_max += extend;
  return 1;
}

int bg_AddTargetData(bg_t *bg, uint8_t data)
{
  while( bg->target_max <= bg->target_cnt )
    if ( bg_extend_target_data(bg) == 0 )
      return -1;
  bg->target_data[bg->target_cnt] = data;
  bg->target_cnt++;
  return bg->target_cnt-1;
}

void bg_ClearTargetData(bg_t *bg)
{
  int i;
  for( i = 0; i < bg->target_max; i++ )
    bg->target_data[i] = 0;
  bg->target_cnt = 0;
  bg->target_bit_pos = 0;
}

/*
  Desc:
    Output a field to the target bitstream. The field size in bits is given by "cnt" and
    the value of the field is "val".
  Args:
    cnt:	Fieldsize in bits, must be lower or equal to 8
    val:	The value (content) of the field. Side condition: val < (1<<cnt) && val >= 0
*/
int bg_AddTargetBits(bg_t *bg, unsigned cnt, unsigned val)
{
  
  assert( val < (1<<cnt) );
  
  while( bg->target_max <= bg->target_cnt+1 )
    if ( bg_extend_target_data(bg) == 0 )
      return 0;
  
  bg->target_data[bg->target_cnt] |= (val << bg->target_bit_pos);
  
  if ( bg->target_bit_pos+cnt >= 8 )
  {
    bg->target_cnt++;
    
    val >>= 8-bg->target_bit_pos;
    bg->target_data[bg->target_cnt] = val;
    
    bg->target_bit_pos+=cnt;
    bg->target_bit_pos-=8;
  }
  else
  {
    bg->target_bit_pos+=cnt;
  }
  
  return 1;
}

int bg_FlushTargetBits(bg_t *bg)
{
  while( bg->target_bit_pos != 0 )
    if ( bg_AddTargetBits(bg, 1, 0) == 0 )
      return 0;
  return 1;
}


int bg_SetBitmapSizeInBytes(bg_t *bg, size_t bytes)
{
  if ( bg->bitmap_data != NULL )
    free(bg->bitmap_data);
  bg->bitmap_data = (uint8_t *)malloc(bytes);
  if ( bg->bitmap_data == NULL )
    return 0;
  memset(bg->bitmap_data, 0, bytes);  
  return 1;
}

int bg_SetBitmapSize(bg_t *bg, int w, int h)
{
  return bg_SetBitmapSizeInBytes(bg, w*h);
}

void bg_SetBitmapPixel(bg_t *bg, int x, int y, int value)
{
  assert( x < bg->bitmap_width );
  assert( y < bg->bitmap_height );
  assert( x >= 0 );
  assert( y >= 0 );
  bg->bitmap_data[y*bg->bitmap_width + x] = value;
}

int bg_GetBitmapPixel(bg_t *bg, int x, int y)
{
  if ( x >= bg->bitmap_width )
    return 0;
  
  if ( y >= bg->bitmap_height )
    return 0;
  if ( x < 0 )
    return 0;
  if ( y < 0 )
    return 0;
  
  return bg->bitmap_data[y*bg->bitmap_width + x];
}

/* 
  Return a pixel with the provided bbx 
  Coordinates are within the bbx. 
*/
int bg_GetBBXPixel(bg_t *bg, int x, int y)
{
  /* glyph rectangle */
  long glyph_x0, glyph_x1, glyph_y0, glyph_y1;
  
  /* local bitmap coordinates */
  long bitmap_x, bitmap_y;
  
  /* perform x offset alignment (used in BDF_BBX_MODE_HEIGHT mode only)*/
  x += bg->shift_x;
  
  /* calculate the rectangle for the glyph */
  glyph_x0 = bg->bbx.x;
  glyph_y0 = bg->bbx.y;
  glyph_x1 = bg->bbx.x+bg->bbx.w;
  glyph_y1 = bg->bbx.y+bg->bbx.h;

  if ( x < glyph_x0 )
    return 0;
  if ( x >= glyph_x1 )
    return 0;
  
  if ( y < glyph_y0 )
    return 0;
  if ( y >= glyph_y1 )
    return 0;
  
  bitmap_x = x - glyph_x0;
  bitmap_y = bg->bbx.h - 1 - ( y - glyph_y0);
  
  return bg_GetBitmapPixel( bg, bitmap_x, bitmap_y );
  
}


void bg_ShowBitmap(bg_t *bg, bbx_t *bbx)
{
  int x, y;
  
  if ( bbx == NULL )
    bbx = &(bg->bbx);
  
  printf("Encoding %ld, mapped to %ld, w=%ld, h=%ld, x=%ld, y=%ld\n", bg->encoding, bg->map_to, bg->bbx.w, bg->bbx.h, bg->bbx.x, bg->bbx.y);
  for( y = bbx->y+bbx->h-1; y >= bbx->y; y--)
  {
    printf("%03d ", y);
    for( x = bbx->x; x < bbx->x + bbx->w; x++)
    {
      if ( bg_GetBBXPixel(bg, x, y) == 0 )
      {
	printf(" .");
      }
      else
      {
	printf(" #");
      }
    }
    
    printf("\n");
  }
}

int bg_IsColZero(bg_t *bg, int x)
{
  int y;
  for( y = 0; y < bg->bitmap_height; y++ )
  {
    if ( bg_GetBitmapPixel(bg, x, y) != 0 )
      return 0;
  }
  return 1;
}

int bg_IsRowZero(bg_t *bg, int y)
{
  int x;
  for( x = 0; x < bg->bitmap_width; x++ )
  {
    if ( bg_GetBitmapPixel(bg, x, y) != 0 )
      return 0;
  }
  return 1;
}

void bg_DeleteFirstCol(bg_t *bg)
{
  int x,y;
  for( y = 0; y < bg->bitmap_height; y++ )
    for( x = 0; x+1 < bg->bitmap_width; x++ )
    {
      bg_SetBitmapPixel(bg, x, y, bg_GetBitmapPixel(bg, x+1, y));
    }
}

void bg_DeleteFirstRow(bg_t *bg)
{
  int x,y;
  for( y = 0; y+1 < bg->bitmap_height; y++ )
    for( x = 0; x < bg->bitmap_width; x++ )
    {
      bg_SetBitmapPixel(bg, x, y, bg_GetBitmapPixel(bg, x, y+1));
    }
}

void bg_ReduceGlyph(bg_t *bg)
{
  /* assign bitmap dimension (should be done already) */
  
  //bg->bbx.w = bg->bitmap_width;
  //bg->bbx.h = bg->bitmap_height;
  
  /* do not assign x,y because they are already set correctly */
  
  
  while( bg->bbx.w > 0 )
  {
    if ( bg_IsColZero(bg, bg->bbx.w-1) == 0 )
      break;
    bg->bbx.w--;
  }

  while( bg->bbx.h > 0 )
  {
    if ( bg_IsRowZero(bg, bg->bbx.h-1) == 0 )
      break;
    bg->bbx.y++;
    bg->bbx.h--;
  }
  
  while( bg->bbx.w > 0)
  {
    if ( bg_IsColZero(bg, 0) == 0 )
      break;
    bg_DeleteFirstCol(bg);
    bg->bbx.x++;
    bg->bbx.w--;
  }
  
  while( bg->bbx.h > 0 )
  {
    if ( bg_IsRowZero(bg, 0) == 0 )
      break;
    bg_DeleteFirstRow(bg);
    bg->bbx.h--;
  }
  
}

/*
  maximize the provided bbx so that the bbxof the glyph completly is covered by the max bbx
*/
int bg_Max(bg_t *bg, bbx_t *max)
{
  int r = 0;
  
  /* glyph rectangle */
  long glyph_x0, glyph_x1, glyph_y0, glyph_y1;
  
  /* max rectangle */
  long max_x0, max_x1, max_y0, max_y1;

  //  printf("Encoding %ld, mapped to %ld, w=%ld, h=%ld, x=%ld, y=%ld\n", bg->encoding, bg->map_to, bg->bbx.w, bg->bbx.h, bg->bbx.x, bg->bbx.y);

  
  /* calculate the rectangle for the glyph */
  glyph_x0 = bg->bbx.x;
  glyph_y0 = bg->bbx.y;
  glyph_x1 = bg->bbx.x+bg->bbx.w;
  glyph_y1 = bg->bbx.y+bg->bbx.h;

  /* calculate the rectangle for the max bbx */  
  max_x0 = max->x;
  max_y0 = max->y;
  max_x1 = max->x+max->w;
  max_y1 = max->y+max->h;

  /* maximize the max rectrangle so that the glyph rectangle full fits inside */
  if ( max_x0 > glyph_x0 )
  {
    max_x0 = glyph_x0;
    r = 3;
  }
  /* 28 Mar dwidth_x and x0???? */
  if ( max_x0 > bg->dwidth_x )	/* include dwidth_x into the box */
  {
    max_x0 = bg->dwidth_x;  /* is this correct??? */
    r = 3;
  }
  
  
  if ( max_x1 < glyph_x1 )
  {
    r = 1;
    max_x1 = glyph_x1;
  }
  
  if ( max_y0 > glyph_y0 )
  {
    r = 4;
    max_y0 = glyph_y0;
  }
  if ( max_y1 < glyph_y1 )
  {
    r = 2;
    max_y1 = glyph_y1;
  }
  
  /* reconstruct the max bbx from the max rectangle */
  max->x = max_x0;
  max->y = max_y0;
  max->w = max_x1 - max->x;
  max->h = max_y1 - max->y;

  return r;
}