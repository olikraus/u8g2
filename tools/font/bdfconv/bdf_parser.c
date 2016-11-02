

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "bdf_font.h"

static int bf_curr(bf_t * bf)
{
  if ( bf->line_pos >= BDF_LINE_LEN )
    return '\0';
  return bf->line_buf[bf->line_pos];
}

static void bf_next(bf_t *bf)
{
  if ( bf_curr(bf) == '\0' )
    return;
  if ( bf->line_pos >= BDF_LINE_LEN )
    return;
  bf->line_pos++;
}

static void bf_skipspace(bf_t *bf)
{
  int c;
  
  for(;;)
  {
    c = bf_curr(bf);
    if ( c == '\0' )
      return;
    if ( c > ' ' )
      return;
    bf_next(bf);
  }
}

static const char *bf_get_identifier(bf_t *bf)
{
  static char buf[BDF_LINE_LEN];
  int c;
  int i = 0;
  buf[0] = '\0';
  for(;;)
  {
    c = bf_curr(bf);
    if ( c <= ' ' )
      break;
    if ( i >= BDF_LINE_LEN-2 )
      break;
    buf[i++] = c;
    buf[i] = '\0';
    bf_next(bf);
  }
  bf_skipspace(bf);
  return buf;
}

static const char *bf_get_string(bf_t *bf)
{
  static char buf[BDF_LINE_LEN];
  int i = 0;
  int c;

  buf[0] = '\0';
  
  if ( bf_curr(bf) != '\"' )
    return bf_get_identifier(bf);

  bf_next(bf);
  
  for(;;)
  {
    c = bf_curr(bf);
    if ( c == '\0'  )
      break;
    if ( c == '\"'  )
      break;
    if ( i >= BDF_LINE_LEN-2 )
      break;
    buf[i++] = c;
    buf[i] = '\0';
    bf_next(bf);
  }
  if ( bf_curr(bf) == '\"' )
    bf_next(bf);
  
  bf_skipspace(bf);
  return buf;
}

static const char *bf_get_eol_string(bf_t *bf)
{
  static char buf[BDF_LINE_LEN];
  int i = 0;
  int c;

  buf[0] = '\0';
  
  if ( bf_curr(bf) == '\"' )
    return bf_get_string(bf);
  //bf_next(bf);
  
  for(;;)
  {
    c = bf_curr(bf);
    if ( c == '\0'  )
      break;
    if ( c == '\n' || c == '\r' )
      break;
    if ( i >= BDF_LINE_LEN-2 )
      break;
    buf[i++] = c;
    buf[i] = '\0';
    bf_next(bf);
  }
  
  bf_skipspace(bf);
  return buf;
}

static long bf_get_long(bf_t *bf)
{
  int c;
  long v = 0;
  long sign = 1;

  if ( bf_curr(bf) == '-' )
  {
    sign = -1;
    bf_next(bf);
    bf_skipspace(bf);
  }

  for(;;)
  {
    c = bf_curr(bf);
    if ( c < '0' )
      break;
    if ( c > '9' )
      break;
    v *= 10L;
    v += (long)(c-'0');
    bf_next(bf);
  }
  bf_skipspace(bf);
  
  return v*sign;
}

/*
static long bf_get_mul(bf_t *bf)
{
  long v;
  v = bf_get_long(bf);
  if ( bf_curr(bf) == '*' )
  {
    bf_next(bf);
    bf_skipspace(bf);
    v *= bf_get_long(bf);    
  }
  return v;
}

static long bf_get_add(bf_t *bf)
{
  long v;
  v = bf_get_mul(bf);
  if ( bf_curr(bf) == '+' )
  {
    bf_next(bf);
    bf_skipspace(bf);
    v += bf_get_mul(bf);    
  }
  return v;
}
*/


static int bf_add_glyph_to_list(bf_t *bf)
{
  bg_t *bg;
  bf->glyph_pos = bf_AddGlyph(bf);
  if ( bf->glyph_pos < 0 )
  {
    bf_Error(bf, "Can not add glyph (memory error?)");
    return 0;
  }
  
  bg = bf->glyph_list[bf->glyph_pos];
  
  /* copy the values from the parser to the glyph record */
  bg->encoding = bf->encoding;
  bg->dwidth_x = bf->dwidth_x;
  bg->dwidth_y = bf->dwidth_y;
  bg->bbx.w = bf->bbx_w;
  bg->bbx.h = bf->bbx_h;
  bg->bbx.x = bf->bbx_x;
  bg->bbx.y = bf->bbx_y;
  
  if ( bg_SetBitmapSize(bg, bf->bbx_w, bf->bbx_h) == 0 )
  {
    bf_Error(bf, "Can not create bitmap (memory error?)");
    return 0;
  }
  
  bg->bitmap_width = bf->bbx_w;
  bg->bitmap_height = bf->bbx_h;
  
  // bg->actual_bitmap_width = bf->bbx_w;
  // bg->actual_bitmap_height = bf->bbx_h;
  
  //bf_Log(bf, "BDF File: Create glyph glyph_pos=%d bbx_w=%d bbx_h=%d bbx_x=%d bbx_y=%d", bf->glyph_pos, bf->bbx_w, bf->bbx_h, bf->bbx_x, bf->bbx_y);

  
  return 1;
}

static void bf_set_pixel_by_halfbyte(bf_t *bf, int halfbyte)
{
  int i;
  bg_t *bg;
  
  assert( bf->glyph_list != NULL );
  
  bg = bf->glyph_list[bf->glyph_pos];
  
  for ( i = 0; i < 4; i++ )
  {
    if ( (halfbyte & 8) != 0 )
    {
      bg_SetBitmapPixel(bg, bf->bitmap_x, bf->bitmap_y, 1);      
    }
    halfbyte<<=1;
    bf->bitmap_x++;
  }
}

static void bf_set_pixel_by_hex_char(bf_t *bf, int hex)
{
  if ( hex >= '0' && hex <= '9' )
    return bf_set_pixel_by_halfbyte(bf, hex-'0');
  if ( hex >= 'a' && hex <= 'f' )
    return bf_set_pixel_by_halfbyte(bf, hex-'a'+10);
  if ( hex >= 'A' && hex <= 'F' )
    return bf_set_pixel_by_halfbyte(bf, hex-'A'+10);
  return bf_set_pixel_by_halfbyte(bf, 0);
}

static int bf_parse_line(bf_t *bf)
{
  static char cmd[BDF_LINE_LEN];
  
  bf->line_pos = 0;
  bf_skipspace(bf);
  
  if ( bf->is_bitmap_parsing == 0 )
  {
    /* regular command */
    bf->bitmap_y = 0;
    strcpy(cmd, bf_get_string(bf));
    
    if ( strcmp(cmd, "" ) == 0 )
    {
      return 1;
    }
    else if ( strcmp(cmd, "STARTFONT" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "FONT" ) == 0 )	/* args: s */
    {      
      if ( bf->str_font != NULL )
	free(bf->str_font);
      bf->str_font = strdup(bf_get_eol_string(bf));
    }
    else if ( strcmp(cmd, "SIZE" ) == 0 )	/* args: lll */
    {      
    }
    else if ( strcmp(cmd, "FONTBOUNDINGBOX" ) == 0 )	/* args: llll */
    {      
    }
    else if ( strcmp(cmd, "STARTPROPERTIES" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "FONTNAME_REGISTRY" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "FOUNDRY" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "FAMILY_NAME" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "WEIGHT_NAME" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "SLANT" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "SETWIDTH_NAME" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "ADD_STYLE_NAME" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "PIXEL_SIZE" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "POINT_SIZE" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "RESOLUTION_X" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "RESOLUTION_Y" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "SPACING" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "AVERAGE_WIDTH" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "CHARSET_REGISTRY" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "CHARSET_ENCODING" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "DESTINATION" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "COPYRIGHT" ) == 0 )	/* args: s */
    {      
      if ( bf->str_copyright != NULL )
	free(bf->str_copyright);
      bf->str_copyright = strdup(bf_get_eol_string(bf));
    }
    else if ( strcmp(cmd, "_XMBDFED_INFO" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "CAP_HEIGHT" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "X_HEIGHT" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "WEIGHT" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "QUAD_WIDTH" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "DEFAULT_CHAR" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "FONT_DESCENT" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "FONT_ASCENT" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "ENDPROPERTIES" ) == 0 )	/* args: - */
    {      
    }
    else if ( strcmp(cmd, "CHARS" ) == 0 )	/* args: l */
    {      
    }
    else if ( strcmp(cmd, "STARTCHAR" ) == 0 )	/* args: s */
    {      
    }
    else if ( strcmp(cmd, "ENCODING" ) == 0 )		/* args: l */
    {      
      bf->encoding = bf_get_long(bf);
      //bf_Log(bf, "BDF File: ENCODING %d", bf->encoding);
    }
    else if ( strcmp(cmd, "SWIDTH" ) == 0 )		/* args: ll */
    {      
    }
    else if ( strcmp(cmd, "DWIDTH" ) == 0 )		/* args: ll */
    {      
      bf->dwidth_x = bf_get_long(bf);
      bf->dwidth_y = bf_get_long(bf);
  
    }
    else if ( strcmp(cmd, "BBX" ) == 0 )		/* args: llll */
    {      
      bf->bbx_w = bf_get_long(bf);
      bf->bbx_h = bf_get_long(bf);
      bf->bbx_x = bf_get_long(bf);
      bf->bbx_y = bf_get_long(bf);
    }
    else if ( strcmp(cmd, "BITMAP" ) == 0 )		/* args: - */
    {
      if ( bf_add_glyph_to_list(bf) == 0 )
	return 0;
      bf->is_bitmap_parsing = 1;
    }
    else if ( strcmp(cmd, "ENDCHAR" ) == 0 )		/* args: - */
    {
      bf_Error(bf, "Unexpected ENDCHAR found");
      return 0;
    }
  }
  else
  {
    /* a line of the glyph bitmap */
    int c;
    strcpy(cmd, bf_get_string(bf));
    if ( cmd[0] != '\0' )
    {
      if ( strcmp(cmd, "ENDCHAR" ) == 0 )		/* args: - */
      {
	bf->is_bitmap_parsing = 0;
	/* bg_ShowBitmap(bf->glyph_list[bf->glyph_pos]); */
      }
      else
      {
	bf->line_pos = 0;
	bf_skipspace(bf);
	
	
	bf->bitmap_x = 0;

	
	for(;;)
	{
	  c = bf_curr(bf);
	  if ( c < '0' )
	    break;
	  bf_set_pixel_by_hex_char(bf, c);
	  bf_next(bf);
	}
	
	bf->bitmap_y++;
      }
    }
  }
  return 1;
}

static int bf_parse_file(bf_t *bf)
{
  long line_cnt = 0;
  bf->is_bitmap_parsing = 0;
  for(;;)
  {
    line_cnt++;
    if ( fgets(bf->line_buf, BDF_LINE_LEN, bf->fp) == NULL )
      break;
    if ( bf_parse_line(bf) == 0 )
    {
      bf_Error(bf, "perse error in line %ld", line_cnt);
      return 0;
    }    
  }
  return 1;
}

int bf_ParseFile(bf_t *bf, const char *name)
{
  int r;
  bf->fp = fopen(name, "r");
  if ( bf->fp == NULL )
  {
    bf_Error(bf, "Can not open bdf file '%s'", name);
    return 0;
  }
  r = bf_parse_file(bf);
  fclose(bf->fp);
  bf_Log(bf, "Parse File %s: %d glyph(s) found", name, bf->glyph_cnt);
  return r;
}

