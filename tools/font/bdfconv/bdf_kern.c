/*

  bdf_kern.c
  
*/

#include "bdf_font.h"
#include <assert.h>

#define BDF_KERNING_MAX (1024*60)

/* the following tables contain the first encodings if they do contain any kernings */
uint16_t bdf_first_table_cnt;
uint16_t bdf_first_encoding_table[BDF_KERNING_MAX];
uint16_t bdf_index_to_second_table[BDF_KERNING_MAX];

/* the index from bdf_index_to_second_table can be used to jump into the following table */
uint16_t bdf_second_table_cnt;
uint16_t bdf_second_encoding_table[BDF_KERNING_MAX];
uint8_t bdf_kerning_values[BDF_KERNING_MAX];

/*
struct u8g2_kerning
{
  uint16_t first_table_cnt;
  uint16_t second_table_cnt;
  uint16_t *first_encoding_table;  
  uint16_t *index_to_second_table;
  uin16_t *second_encoding_table;
  uint8_t *kerning_values;
}
*/


static void bdf_write_uint16_array(FILE *fp, const char *pre, const char *post, uint16_t cnt, const uint16_t *a)
{
  uint16_t i;
  
  fprintf(fp, "static const uint16_t %s_%s[%u] = {\n  ", pre, post, cnt);
  for( i = 0; i < cnt; i++ )
  {
    fprintf(fp, "%u", a[i]);
    if ( i+1 < cnt )
    {
      fprintf(fp, ", ");
      if ( i % 16 == 0 && i > 0 )
      {
	fprintf(fp, "\n  ");
      }
    }
  }
  fprintf(fp, "};\n");
}

static void bdf_write_uint8_array(FILE *fp, const char *pre, const char *post, uint16_t cnt, const uint8_t *a)
{
  uint16_t i;
  
  fprintf(fp, "static const uint8_t %s_%s[%u] = {\n  ", pre, post, cnt);
  for( i = 0; i < cnt; i++ )
  {
    fprintf(fp, "%u", a[i]);
    if ( i+1 < cnt )
    {
      fprintf(fp, ", ");
      if ( i % 16 == 0 && i > 0 )
      {
	fprintf(fp, "\n  ");
      }
    }
  }
  fprintf(fp, "};\n");
}


void bdf_write_kerning_file(const char *kernfile, const char *name)
{
  FILE *fp;
  fp = fopen(kernfile, "w");
  
  fprintf(fp, "/* %s, Size: %u Bytes */\n", name, bdf_first_table_cnt*4 + bdf_second_table_cnt*3 + 4 + 8);  // size calculation for 16 bit controller
  bdf_write_uint16_array(fp, name, "first_encoding_table", bdf_first_table_cnt, bdf_first_encoding_table);
  bdf_write_uint16_array(fp, name, "index_to_second_table", bdf_first_table_cnt, bdf_index_to_second_table);
  bdf_write_uint16_array(fp, name, "second_encoding_table", bdf_second_table_cnt, bdf_second_encoding_table);
  bdf_write_uint8_array(fp, name, "kerning_values", bdf_second_table_cnt, bdf_kerning_values);
   
  
  fprintf(fp, "u8g2_kerning_t %s_k = {\n", name);
  fprintf(fp, "  %u, %u,\n", bdf_first_table_cnt, bdf_second_table_cnt);
  fprintf(fp, "  %s_%s,\n", name, "first_encoding_table");
  fprintf(fp, "  %s_%s,\n", name, "index_to_second_table");
  fprintf(fp, "  %s_%s,\n", name, "second_encoding_table");
  fprintf(fp, "  %s_%s};\n\n", name, "kerning_values");
  fclose(fp);
}

/*
  assumes 
    tga_set_font(font); 
  and
    tga_init((tga_get_char_width()+16)*3, ((tga_get_char_height()+8)*2));
  is called before
*/
static int bdf_is_glyph_overlap(uint8_t *font, uint16_t e1, uint16_t e2, uint8_t kerning_test, int is_save)
{
  unsigned int x, y;
  tga_clear();

  x = 8;
  y = tga_get_char_height();
  x += tga_draw_glyph(x, y, e1, 0);
  x -= kerning_test;
  tga_clear_pixel_intersection();
  x += tga_draw_glyph(x, y, e2, 0);

  if ( is_save )
  {
    //char buf[64];
    //sprintf(buf, "glyph_intersection_%u_%u_%u.tga", e1, e2, kerning_test);
    //tga_save(buf);
  }
  
  return tga_is_pixel_intersection();
}

unsigned bdf_calculate_kerning(uint8_t *font, uint16_t e1, uint16_t e2, uint8_t min_distance_in_per_cent_of_char_width)
{
  uint8_t upper_bound;
  uint8_t kerning;
  uint8_t min_distance_in_pixel;

  
  tga_set_font(font);
  tga_init((tga_get_char_width()+16)*3, ((tga_get_char_height()+8)*2));
  
  min_distance_in_pixel = ((unsigned)tga_get_char_width()*(unsigned)min_distance_in_per_cent_of_char_width) / 100;
  upper_bound = tga_get_char_width();
  for( kerning = 0; kerning < upper_bound; kerning++ )
  {
    if ( bdf_is_glyph_overlap(font, e1, e2, kerning, 0) != 0 )
      break;
  }
  if ( kerning >= upper_bound )
    kerning = 0; /* maybe "." compared against "-" */
  if ( kerning < min_distance_in_pixel )
    kerning = 0;
  else 
    kerning -= min_distance_in_pixel;
  
  if ( kerning != 0 )
  {
    bdf_is_glyph_overlap(font, e1, e2, kerning, 1);
    //printf("bdf_calculate_kerning %u %u ", e1, e2);
    //printf("result: %d\n", kerning);
  }
  return kerning;
}

void bdf_calculate_all_kerning(bf_t *bf, const char *filename, const char *fontname, uint8_t min_distance_in_per_cent_of_char_width)
{
  int first, second;
  bg_t *bg_first;
  bg_t *bg_second;
  uint8_t kerning;
  int is_first_encoding_added;

  
  bdf_first_table_cnt = 0;
  bdf_second_table_cnt = 0;
  
  for( first= 0; first < bf->glyph_cnt; first++ )
  {
    is_first_encoding_added = 0;
    bg_first = bf->glyph_list[first];
    if ( bg_first->target_data != NULL && bg_first->is_excluded_from_kerning == 0 )
    {
      for( second= 0; second < bf->glyph_cnt; second++ )
      {
        bg_second = bf->glyph_list[second];
        if ( bg_second->target_data != NULL && bg_second->is_excluded_from_kerning == 0 )
        {
          kerning = bdf_calculate_kerning(bf->target_data, bg_first->encoding, bg_second->encoding, min_distance_in_per_cent_of_char_width);
	  if ( kerning > 1 )
	  {
	    if ( is_first_encoding_added == 0 )
	    {
	      bdf_first_encoding_table[bdf_first_table_cnt] = bg_first->encoding;
	      bdf_index_to_second_table[bdf_first_table_cnt]  = bdf_second_table_cnt;
	      bdf_first_table_cnt++;
	      if (bdf_first_table_cnt > BDF_KERNING_MAX)
	      {
		      fprintf(stderr, "Kerning calculation aborted: bdf_first_table_cnt > BDF_KERNING_MAX\n");
		      return;
	      }
	      is_first_encoding_added = 1;
	    }
	    bdf_second_encoding_table[bdf_second_table_cnt] = bg_second->encoding;
	    bdf_kerning_values[bdf_second_table_cnt] = kerning;
	    bdf_second_table_cnt++;
	    if (bdf_second_table_cnt > BDF_KERNING_MAX)
	    {
		      fprintf(stderr, "Kerning calculation aborted: bdf_second_table_cnt > BDF_KERNING_MAX\n");
		      return;
	    }
	  }
        }
      }
    }
  }
  /* add a final entry for more easier calculation in u8g2 */
  bdf_first_encoding_table[bdf_first_table_cnt] = 0x0ffff;
  bdf_index_to_second_table[bdf_first_table_cnt]  = bdf_second_table_cnt;
  bdf_first_table_cnt++;
  
  bdf_write_kerning_file(filename, fontname);
}

