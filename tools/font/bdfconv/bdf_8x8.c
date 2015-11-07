/*

  bdf_8x8.c
  
  unencoded font format for 8x8 pixel font
  
  offset 	bytes	desc
  0		1		first char
  1		2		last char
  2		n		font data, n = (last char - first char + 1)*8
  
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bdf_font.h"


int bg_8x8_convert(bg_t *bg, bbx_t *bbx)
{
  int x;
  int y;
  int d;

  if ( bbx == NULL )
    bbx = &(bg->bbx);  
  
  bg_ClearTargetData(bg);
  
  if ( bbx->w != 8 )
    return 0;
  if ( bbx->h != 8 )
    return 0;
  

  /*
  for( y = bbx->y+bbx->h-1; y >= bbx->y; y--)
  {
    d = 0;
    for( x = bbx->x; x < bbx->x + bbx->w; x++)
    {
      d <<= 1;
      if ( bg_GetBBXPixel(bg, x, y) != 0 )
      {
	d++;
      }
    }
    if ( bg_AddTargetData(bg, d) < 0 )
      return 0;
 }
  */

  for( x = bbx->x; x < bbx->x + bbx->w; x++)
  {
    d = 0;
    for( y = bbx->y+bbx->h-1; y >= bbx->y; y--)
    {
      d >>= 1;
      if ( bg_GetBBXPixel(bg, x, y) != 0 )
      {
	d |= 128;
      }
    }
    if ( bg_AddTargetData(bg, d) < 0 )
      return 0;
 }
  
  return 1;
}


void bf_Generate8x8Font(bf_t *bf)
{
  int i, j, k;
  bg_t *bg;
  int first, last;
  bbx_t local_bbx;
  int is_glyph_written;

  /* Step 1: Generate 8x8 bitmap data */
  
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 )
    {
      bf_copy_bbx_and_update_shift(bf, &local_bbx, bg);
      
      if ( local_bbx.w != 8 )
      {
	bf_Log(bf, "Generate8x8Font: Error, glyph width is not 8, width=%d, encoding=%d", local_bbx.w, bg->encoding);
      }
      else
      {
	if ( local_bbx.h != 8 )
	{
	  bf_Log(bf, "Generate8x8Font: Error, glyph height is not 8, height=%d, encoding=%d", local_bbx.h, bg->encoding);
	}
	else
	{
	  if ( bg_8x8_convert(bg, &local_bbx) == 0 )
	  {
	    bf_Log(bf, "Generate8x8Font: Error, 8x8 conversion, encoding=%d", bg->target_cnt, bg->encoding);
	  }
	}
      }
    }
  }

  /* Step 2: Calculate first and last char */
  
  first = -1;
  last = -1;
  
  for( j = 0; j < 256; j++ )
  {
    for( i = 0; i < bf->glyph_cnt; i++ )
    {
      bg = bf->glyph_list[i];
      if ( bg->map_to == j )
      {
	if ( bg->target_data != NULL )
	{
	  
	  if ( bg->target_cnt != 8)
	  {
	    bf_Log(bf, "Generate8x8Font: Error, glyph size is not 8, size=%d, encoding=%d", bg->target_cnt, bg->encoding);
	  }
	  else
	  {
	    if ( first < 0 )
	      first = j;
	    last = j;
	  }
	}
      }
    }
  }
  
  /* Step 3: Write font data */

  bf_AddTargetData(bf, first);
  bf_AddTargetData(bf, last);

  
  for( j = first; j <= last; j++ )
  {
    is_glyph_written = 0;
    for( i = 0; i < bf->glyph_cnt; i++ )
    {
      bg = bf->glyph_list[i];
      if ( bg->map_to == j )
      {
	if ( bg->target_data != NULL )
	{
	  
	  if ( bg->target_cnt == 8)
	  {
	    for( k = 0; k < bg->target_cnt; k++ )
	    {
	      bf_AddTargetData(bf, bg->target_data[k]);
	    }
	    is_glyph_written = 1;
	  }
	}
      }
    }
    if (  is_glyph_written == 0 )
    {
      for( k = 0; k < 8; k++ )
      {
	bf_AddTargetData(bf, 0);
      }
    }

  }

  bf_Log(bf, "Generate8x8Font: Font size %d", bf->target_cnt);
}