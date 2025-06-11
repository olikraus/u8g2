

#include <stdlib.h>
#include <stdarg.h>
#include "bdf_font.h"

void bf_Error(bf_t *bf, char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  vprintf(fmt, va);
  printf("\n");
  va_end(va);
}

void bf_Log(bf_t *bf, char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  if ( bf->is_verbose != 0 )
  {
    vprintf(fmt, va);
    printf("\n");
  }
  va_end(va);
}

/*
  bf_Open(0, BDF_BBX_MODE_MINIMAL)
*/
bf_t *bf_Open(int is_verbose, int bbx_mode)
{
  bf_t *bf;
  bf = (bf_t *)malloc(sizeof(bf_t));
  if ( bf != NULL )
  {
    bf->is_verbose = is_verbose;
    bf->glyph_list = NULL;
    bf->glyph_cnt = 0;
    bf->glyph_max = 0;
    bf->str_font = NULL;		/* argument for FONT in bdf file */
    bf->str_copyright = NULL;	/* argument for COPYRIGHT in bdf file */
    bf->target_data = NULL;
    bf->target_max = 0;
    bf->target_cnt = 0;
    bf->selected_glyphs = 0;
    
    bf->enc_w = 0;
    bf->enc_h = 0;
    bf->enc_x = 0;
    bf->enc_y = 0;
    
    bf->bbx_mode = bbx_mode;
    bf->tile_h_size = 1;
    bf->tile_v_size = 1;
        
    return bf;
  }
  return NULL;
}

void bf_Clear(bf_t *bf)
{
  int i;
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    free( bf->glyph_list[i] );
  }
  bf->glyph_cnt = 0;
  
  if ( bf->str_font != NULL )
    free(bf->str_font);
  bf->str_font = NULL;
  if ( bf->str_copyright != NULL )
    free(bf->str_copyright);
  bf->str_copyright = NULL;
}

void bf_Close(bf_t *bf)
{
  bf_Clear(bf);
  if ( bf->glyph_list != NULL )
    free(bf->glyph_list);
  if ( bf->target_data != NULL )
    free(bf->target_data);
  bf->glyph_list = NULL;
  bf->glyph_max = 0;
  free(bf);
}

static int bf_extend(bf_t *bf)
{
  int extend = 16;
  void *ptr;
  if ( bf->glyph_list == NULL )
  {
    ptr = malloc(extend*sizeof(bg_t *));
    bf->glyph_max = 0;
  }
  else
  {
    ptr = realloc(bf->glyph_list, (bf->glyph_max + extend)*sizeof(bg_t *));
  }
  if ( ptr == NULL )
    return 0;
  bf->glyph_max += extend;
  bf->glyph_list = (bg_t **)ptr;
  return 1;
}

/* returns glyph position or -1 */
int bf_AddGlyph(bf_t *bf)
{
    while( bf->glyph_max <= bf->glyph_cnt )
      if ( bf_extend(bf) == 0 )
	return -1;
    bf->glyph_list[bf->glyph_cnt] = bg_Open();
    if ( bf->glyph_list[bf->glyph_cnt] == NULL )
      return -1;
    bf->glyph_list[bf->glyph_cnt]->bf = bf;
    bf->glyph_cnt++;
    return bf->glyph_cnt-1;
}


static int bf_extend_target_data(bf_t *bf)
{
  int extend = 16;
  int i;
  void *ptr;
  if ( bf->target_data == NULL )
  {
    ptr = malloc(extend*sizeof(uint8_t));
    bf->target_max = 0;
  }
  else
  {
    ptr = realloc(bf->target_data, (bf->target_max + extend)*sizeof(uint8_t));
  }
  if ( ptr == NULL )
    return 0;
  bf->target_data = (uint8_t *)ptr;
  for( i = bf->target_max; i < bf->target_max + extend; i++ )
      bf->target_data[i] = 0;
  bf->target_max += extend;
  return 1;
}

int bf_AddTargetData(bf_t *bf, uint8_t data)
{
  while( bf->target_max <= bf->target_cnt )
    if ( bf_extend_target_data(bf) == 0 )
      return -1;
  bf->target_data[bf->target_cnt] = data;
  bf->target_cnt++;
  return bf->target_cnt-1;
}

void bf_ClearTargetData(bf_t *bf)
{
  int i;
  for( i = 0; i < bf->target_max; i++ )
    bf->target_data[i] = 0;
  bf->target_cnt = 0;
}

void bf_CalculateSelectedNumberOfGlyphs(bf_t *bf)
{
  int i;
  bg_t *bg;
  bf->selected_glyphs = 0;
  
  
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 )
    {
      bf->selected_glyphs++;
    }
  }
}


void bf_ReduceAllGlyph(bf_t *bf)
{
  int i;
  bg_t *bg;
  int red_x, red_y;
  
  bf_Log(bf, "Reduce: Start");
  
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 )
    {
      bg_ReduceGlyph(bg);
      //bg_ShowBitmap(bg, &(bg->bbx));
      red_x = bg->bitmap_width - bg->bbx.w;
      red_y = bg->bitmap_height - bg->bbx.h;
      if ( red_x > 0 || red_y > 0 )
      {
	//bf_Log(bf, "Reduce: Encoding %ld, x by %d, y by %d", bg->encoding, red_x, red_y);
      }
    }
  }
  bf_Log(bf, "Reduce: End");

}

void bf_ShowAllGlyphs(bf_t *bf, bbx_t *bbx)
{
  int i;
  bg_t *bg;
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 )
    {
      bg_ShowBitmap(bg, bbx); 
    }
  }
}

int bf_GetIndexByEncoding(bf_t *bf, long encoding)
{
  int i;
  bg_t *bg;
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->encoding == encoding )
      return i;
  }
  return -1;
}

void bf_CalculateMaxBBX(bf_t *bf)
{
  int i;
  int is_first = 1;
  int enc_idx;
  bg_t *bg;
  
  
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 )
    {
      if ( is_first != 0 )
      {
	bf->max = bg->bbx;
	bf->enc_x = bg->encoding;
	bf->enc_y = bg->encoding;
	bf->enc_w = bg->encoding;
	bf->enc_h = bg->encoding;
	is_first = 0;
      }
      else
      {
	enc_idx = bg_Max(bg, &(bf->max));
	switch(enc_idx)
	{
	  case 1:
	    bf->enc_w = bg->encoding;
	    break;
	  case 2:
	    bf->enc_h = bg->encoding;
	    break;
	  case 3:
	    bf->enc_x = bg->encoding;
	    break;
	  case 4:
	    bf->enc_y = bg->encoding;
	    break;
	}
      }
    }
  }
  
  if ( bf->bbx_mode == BDF_BBX_MODE_M8 )
  {
    /* old 
    bf->max.w = ( bf->max.w + 7 ) & ~7;
    bf->max.h = ( bf->max.h + 7 ) & ~7;
    */
    bf->max.w = 8;
    bf->max.h = 7;
  }

  
  bf_Log(bf, "CalculateMaxBBX: x=%ld, y=%ld, w=%ld, h=%ld", bf->max.x, bf->max.y, bf->max.w, bf->max.h);
  bf_Log(bf, "CalculateMaxBBX: Encodings x=%ld, y=%ld, w=%ld, h=%ld", bf->enc_x, bf->enc_y, bf->enc_w, bf->enc_h);
}

void bf_CalculateMinMaxDWidth(bf_t *bf)
{
  int i;
  bg_t *bg;
  
  bf->dx_min = 0x07fff;
  bf->dx_max = -0x07fff;
  
  bf->x_min = 0x07fff;
  bf->x_max = -0x07fff;
  
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 )
    {
      if ( bf->dx_min > bg->dwidth_x )
	bf->dx_min = bg->dwidth_x;
      if ( bf->dx_max < bg->dwidth_x )
	bf->dx_max = bg->dwidth_x;

      if ( bf->x_min > bg->bbx.x )
	bf->x_min = bg->bbx.x;
      if ( bf->x_max < bg->bbx.x )
	bf->x_max = bg->bbx.x;
      
    }
  }
  bf_Log(bf, "bf_CalculateMinMaxDWidth: dx_min=%ld, dx_max=%ld", bf->dx_min, bf->dx_max);
  bf_Log(bf, "bf_CalculateMinMaxDWidth: x_min=%ld, x_max=%ld", bf->x_min, bf->x_max);
  if ( bf->dx_min == bf->dx_max && bf->x_min >= 0 )
  {
    bf_Log(bf, "bf_CalculateMinMaxDWidth: Monospaced font.");	/* not sufficient: also bbx.x must be >= 0 for all glyphs */
    /* for a monospaced font, dx must be identical to bbx.w */
  }
}

int get_unsigned_bit_size(unsigned long v)
{
  int i = 0;
  while( v != 0 )
  {
    v = v / 2;
    i++;
  }
  return i;
}

int get_signed_bit_size(long v)
{
  if ( v < 0 )
    return get_unsigned_bit_size(-v-1) + 1;
  return get_unsigned_bit_size(v) + 1;
}

/*
#define BDF_BBX_MODE_MINIMAL 0
#define BDF_BBX_MODE_HEIGHT 1
#define BDF_BBX_MODE_MAX 2
#define BDF_BBX_MODE_M8 3
*/
void bf_copy_bbx_and_update_shift(bf_t *bf, bbx_t *target_bbx, bg_t *bg)
{
      /* modifing the following code requires update ind bdf_rle.c also */
      if ( bf->bbx_mode == BDF_BBX_MODE_MINIMAL )	// mode 0
      {
	*target_bbx = bg->bbx;	
      }
      else if ( bf->bbx_mode == BDF_BBX_MODE_MAX )	// mode 2 (monospace)
      {
	*target_bbx = bf->max;	
	target_bbx->x = 0;
	if ( bg->bbx.x < 0 )
	  bg->shift_x = bg->bbx.x;
	if ( target_bbx->w < bg->dwidth_x )
	  target_bbx->w = bg->dwidth_x;
      }
      else if ( bf->bbx_mode == BDF_BBX_MODE_5X7 )	// mode 4 (5x7 displays)
      {
	*target_bbx = bf->max;	
	target_bbx->x = 0;
	if ( bg->bbx.x < 0 )
	  bg->shift_x = bg->bbx.x;
        target_bbx->w = 5;
      }
      else if ( bf->bbx_mode == BDF_BBX_MODE_M8 )
      {
	/* old
	target_bbx->w = bf->max.w;
	if ( target_bbx->w < bg->dwidth_x )
	  target_bbx->w = bg->dwidth_x;
	target_bbx->w = (target_bbx->w+7) & ~7;
	target_bbx->h = (bf->max.h+7) & ~7;
	*/
	target_bbx->w = 8;
	target_bbx->h = 8;
	target_bbx->x = bf->max.x;
	target_bbx->y = bf->max.y;
	target_bbx->x = 0;
	if ( bg->bbx.x < 0 )
	  bg->shift_x = bg->bbx.x;
      }
      else
      {

	*target_bbx = bf->max;
	target_bbx->w = bg->bbx.w;	
	target_bbx->x = bg->bbx.x;	
	target_bbx->x = 0;
	if ( bg->bbx.x < 0 )
	{
	  /* e.g. "j" */
	  target_bbx->w -= bg->bbx.x;
	  bg->shift_x = bg->bbx.x;
	}
	else
	{
	  /* e.g. "B" */
	  target_bbx->w += bg->bbx.x;
	  //bg->shift_x = bg->bbx.x;
	}
	if ( target_bbx->w < bg->dwidth_x )
	  target_bbx->w = bg->dwidth_x;
      }
  
  bg->width_deviation = target_bbx->w - bg->bbx.w;
}

void bf_CalculateMaxBitFieldSize(bf_t *bf)
{
  int i;
  bg_t *bg;
  int bs;
  bbx_t local_bbx;
  bf->bbx_x_max_bit_size = 0;
  bf->bbx_y_max_bit_size = 0;
  bf->bbx_w_max_bit_size = 0;
  bf->bbx_h_max_bit_size = 0;
  bf->dx_max_bit_size = 0;
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 )
    {
      
      bf_copy_bbx_and_update_shift(bf, &local_bbx, bg);
#ifdef OLD_CLODE      
      /* modifing the following code requires update ind bdf_rle.c also */
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
      bs = get_unsigned_bit_size(local_bbx.w);
      if ( bf->bbx_w_max_bit_size < bs )
	bf->bbx_w_max_bit_size = bs;
      
      bs = get_unsigned_bit_size(local_bbx.h);
      if ( bf->bbx_h_max_bit_size < bs )
	bf->bbx_h_max_bit_size = bs;
      
      //printf("%ld ", local_bbx.x);
      bs = get_signed_bit_size(local_bbx.x);
      if ( bf->bbx_x_max_bit_size < bs )
	bf->bbx_x_max_bit_size = bs;
      //printf("%d:%d ",(int)local_bbx->x, (int)bs);

      bs = get_signed_bit_size(local_bbx.y);
      if ( bf->bbx_y_max_bit_size < bs )
	bf->bbx_y_max_bit_size = bs;

      if ( bf->bbx_mode == BDF_BBX_MODE_MINIMAL )
      {
	bs = get_signed_bit_size(bg->dwidth_x);
      }
      else if ( bf->bbx_mode == BDF_BBX_MODE_MAX )
      {
	bs = get_signed_bit_size(local_bbx.w);
      }
      else
      {
	bs = get_signed_bit_size(local_bbx.w);
      }
      if ( bf->dx_max_bit_size < bs )
	bf->dx_max_bit_size = bs;
      
    }
  }
  bf_Log(bf, "bf_CalculateMaxBitFieldSize: bbx.x=%d, bbx.y=%d, bbx.w=%d, bbx.h=%d, dwidth=%d", 
      bf->bbx_x_max_bit_size, bf->bbx_y_max_bit_size, bf->bbx_w_max_bit_size, bf->bbx_h_max_bit_size, bf->dx_max_bit_size);
  
}

void bf_ShowMonospaceStatistics(bf_t *bf)
{
  int i;
  bg_t *bg;
  long cnt = 0;
  long max = 0;
  long sum = 0;
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->map_to >= 0 )
    {
      if ( max < bg->width_deviation )
	max = bg->width_deviation;
      sum += bg->width_deviation;
      cnt++;
    }
  }
  if ( cnt == 0 )
    cnt++;
  bf_Log(bf, "Monospace Statistics: Max width extention %ld, average width extention %ld.%ld", max, sum/cnt, (sum*10/cnt) % 10 );
  /*
    Monospaced font: average width extention does not differ much between -b 1 and --b 2
    Variable width font: big difference for the average width extention between modes 1 and 2.
  
    Examples:
  
      ./bdfconv -b 1 -v ../bdf/profont12.bdf
	Monospace Statistics: Max width extention 6, average width extention 1.7
      ./bdfconv -b 2 -v ../bdf/profont12.bdf
	Monospace Statistics: Max width extention 6, average width extention 1.7
      --> profont12.bdf is a monospaced font  
  
      ./bdfconv -b 1 -v ../bdf/helvR12.bdf
	Monospace Statistics: Max width extention 6, average width extention 1.9
      ./bdfconv -b 2 -v ../bdf/helvR12.bdf
	Monospace Statistics: Max width extention 15, average width extention 8.0
      --> helvR12.bdf is not a monospaced font  
  
  */
}


int bf_WriteUCGCByFP(bf_t *bf, FILE *out_fp, const char *fontname, const char *indent)
{
  int i;
  int bytes_per_line = 16;
  
  fprintf(out_fp, "/*\n");
  fprintf(out_fp, "  Fontname: %s\n", bf->str_font);
  fprintf(out_fp, "  Copyright: %s\n", bf->str_copyright);
  fprintf(out_fp, "  Glyphs: %d/%d\n", (int)bf->selected_glyphs, (int)bf->glyph_cnt );
  fprintf(out_fp, "  BBX Build Mode: %d\n", (int)bf->bbx_mode);
  fprintf(out_fp, "*/\n");
  fprintf(out_fp, "#include \"ucg.h\"\n");  
  fprintf(out_fp, "const ucg_fntpgm_uint8_t %s[%d] UCG_FONT_SECTION(\"%s\") = {\n", fontname, bf->target_cnt, fontname);
  fprintf(out_fp, "  ");
    
  for( i = 0; i < bf->target_cnt; i++ )
  {
    fprintf(out_fp, "%d", bf->target_data[i]);
    if ( i+1 != bf->target_cnt )
      fprintf(out_fp, ",");
    if ( (i+1) % bytes_per_line == 0 )
      fprintf(out_fp, "\n%s", indent);    
  }

  fprintf(out_fp, "};\n");  
  return 1;
}

int bf_WriteU8G2CByFP(bf_t *bf, FILE *out_fp, const char *fontname, const char *indent)
{
  int i;
  int bytes_per_line = 32;
  int extra1;
  
  fprintf(out_fp, "/*\n");
  fprintf(out_fp, "  Fontname: %s\n", bf->str_font);
  fprintf(out_fp, "  Copyright: %s\n", bf->str_copyright);
  fprintf(out_fp, "  Glyphs: %d/%d\n", (int)bf->selected_glyphs, (int)bf->glyph_cnt );
  fprintf(out_fp, "  BBX Build Mode: %d\n", (int)bf->bbx_mode);
  fprintf(out_fp, "*/\n");

  if ( bf->target_data[bf->target_cnt-1] == 0 )
    extra1 = 0;
  else
    extra1 = 1;
  
  if ( bf->target_cnt-1+extra1 > 32760 )
  {
    fprintf(out_fp, "#ifdef U8G2_USE_LARGE_FONTS\n");    
  }
  
  if ( bf->bbx_mode == 3 )	// maybe better check for the font_format
  {
    //fprintf(out_fp, "#include \"u8x8.h\"\n");  
    fprintf(out_fp, "const uint8_t %s[%d] U8X8_FONT_SECTION(\"%s\") = \n", fontname, bf->target_cnt+extra1, fontname);
  }
  else
  {
    //fprintf(out_fp, "#include \"u8g2.h\"\n");  
    fprintf(out_fp, "const uint8_t %s[%d] U8G2_FONT_SECTION(\"%s\") = \n", fontname, bf->target_cnt+extra1, fontname);
  }
  fprintf(out_fp, "%s\"", indent);
    
  for( i = 0; i < bf->target_cnt-1+extra1; i++ )
  {
    if ( bf->target_data[i] < 32 || bf->target_data[i] == '\"' || bf->target_data[i] == '\\'  || bf->target_data[i] == '?' || ( bf->target_data[i] >= '0' && bf->target_data[i] <= '9' ))
    {
      fprintf(out_fp, "\\%o", bf->target_data[i]);
      //fprintf(out_fp, "\\x%02x", bf->target_data[i]);
    }
    else if ( bf->target_data[i] < 127 )		/* issue 482, do not output ASCII char 127, instead use octal code for 127 */
    {
      fprintf(out_fp, "%c", bf->target_data[i]);
    }
    else
    {
      fprintf(out_fp, "\\%o", bf->target_data[i]);
    }
    if ( (i+1) % bytes_per_line == 0 )
      fprintf(out_fp, "\"\n%s\"", indent); 
  }

  fprintf(out_fp, "\";\n");  

  if ( bf->target_cnt-1+extra1 > 32760 )
  {
    fprintf(out_fp, "#endif /* U8G2_USE_LARGE_FONTS */\n");    
  }
  
  return 1;
}

int bf_WriteUCGCByFilename(bf_t *bf, const char *filename, const char *fontname, const char *indent)
{
  FILE *fp;
  fp = fopen(filename, "wb");
  if ( fp == NULL )
  {
    bf_Log(bf, "bf_WriteUCGCByFilename: Open error '%s'", filename);
    return 0;
  }
  
  bf_WriteUCGCByFP(bf, fp, fontname, indent);
  bf_Log(bf, "bf_WriteUCGCByFilename: Write file '%s'", filename);
  
  fclose(fp);
  return 1;
}

/* called from main() */
int bf_WriteU8G2CByFilename(bf_t *bf, const char *filename, const char *fontname, const char *indent)
{
  FILE *fp;
  fp = fopen(filename, "wb");
  if ( fp == NULL )
  {
    bf_Log(bf, "bf_WriteU8G2CByFilename: Open error '%s'", filename);
    return 0;
  }
  
  bf_WriteU8G2CByFP(bf, fp, fontname, indent);
  bf_Log(bf, "bf_WriteU8G2CByFilename: Write file '%s'", filename);
  
  fclose(fp);
  return 1;
}


/*
  xo, yo: offset for 8x8 fonts (font_format==2)
  called from main()
*/
bf_t *bf_OpenFromFile(const char *bdf_filename, int is_verbose, int bbx_mode, const char *map_str, const char *map_file_name, const char *utf8_file_name, int font_format, int xo, int yo, int th, int tv)
{
  bf_t *bf;

  bf = bf_Open(is_verbose, bbx_mode);
  if ( bf != NULL )
  {
    bf->tile_h_size = th;
    bf->tile_v_size = tv;
    
    if ( bf_ParseFile(bf, bdf_filename) != 0 )
    {
      if ( map_file_name[0] != '\0' ) 
      {
	bf_MapFile(bf, map_file_name);  // bdf_map.c
      }
      else
      {
	bf_Map(bf, map_str);     // bdf_map.c
      }
      if ( utf8_file_name[0] != '\0' )
      {
	bf_Utf8File(bf, utf8_file_name);
      }
      bf_CalculateSelectedNumberOfGlyphs(bf);
      
      bf_ReduceAllGlyph(bf);
      bf_CalculateMaxBBX(bf);
      //bf_ShowAllGlyphs(bf, &(bf->max));
      bf_CalculateMinMaxDWidth(bf);
      
      /* issue 669 */
      if ( bf->bbx_mode == BDF_BBX_MODE_MAX )
	if ( bf->max.w < bf->dx_max )
	  bf->max.w = bf->dx_max;
      
      bf_CalculateMaxBitFieldSize(bf);  
      
      
      if ( font_format == 0 || font_format == 1 )
      {
	bf_RLECompressAllGlyphs(bf);
      }
      else
      {
	bf_Generate8x8Font(bf, xo, yo);	/* bdf_8x8.c */
      }
      
      if ( bf->bbx_mode != BDF_BBX_MODE_MINIMAL )
	bf_ShowMonospaceStatistics(bf);	/* Show stats only for none minimal mode. For minimal mode it will always be zero */
    
      return bf;
    }
    bf_Close(bf);
  }
  return NULL;
}

