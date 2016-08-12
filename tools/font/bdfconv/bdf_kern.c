/*

  bdf_kern.c
  
*/

#include "bdf_font.h"


#define BDF_KERNING_MAX (1024*32)

/* the following tables contain the first encodings if they do contain any kernings */
uint16_t bdf_first_table_cnt;
uint16_t bdf_first_encoding_table[BDF_KERNING_MAX];
uint16_t bdf_index_to_second_table[BDF_KERNING_MAX];

/* the index from bdf_index_to_second_table can be used to jump into the following table */
uint16_t bdf_second_table_cnt;
uint16_t bdf_second_encoding_table[BDF_KERNING_MAX];
uint8_t bdf_kerning_values[BDF_KERNING_MAX]



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
    char buf[64];
    sprintf(buf, "glyph_intersection_%u_%u_%u.tga", e1, e2, kerning_test);
    tga_save(buf);
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
  printf("bdf_calculate_kerning %u %u\n", e1, e2);
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
    printf("result: %d\n", kerning);
  }
  return kerning;
}

void bdf_calculate_all_kerning(bf_t *bf, uint8_t min_distance_in_per_cent_of_char_width)
{
  int first, second;
  bg_t *bg_first;
  bg_t *bg_second;

  for( first= 0; first < bf->glyph_cnt; first++ )
  {
    bg_first = bf->glyph_list[first];
    if ( bg_first->target_data != NULL && bg_first->is_excluded_from_kerning == 0 )
    {
      
      for( second= 0; second < bf->glyph_cnt; second++ )
      {
        bg_second = bf->glyph_list[second];
        if ( bg_second->target_data != NULL && bg_second->is_excluded_from_kerning == 0 )
        {
          bdf_calculate_kerning(bf->target_data, bg_first->encoding, bg_second->encoding, min_distance_in_per_cent_of_char_width);
          
        }
      }
      
    }
  }
}