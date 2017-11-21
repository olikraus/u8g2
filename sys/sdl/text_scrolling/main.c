
#include "u8g2.h"
#include <stdio.h>
#include <string.h>


/* not finished */
  
u8g2_t u8g2;

#define MAX_CHARS 30

u8g2_uint_t char_offset[MAX_CHARS+1]; 	// one more to store the end of the string
u8g2_uint_t curr_index;
u8g2_uint_t curr_offset;
u8g2_uint_t str_len;

/* UTF8 is not supported for "s" */
void init_text(const char *s)
{
  u8g2_uint_t i;
  str_len = strlen(s);			// calculate number of chars
  if ( str_len > MAX_CHARS )		// restrict the no of chars to the size of the offset table
    str_len = MAX_CHARS;
  char_offset[0] = 0;			// start with 0
  for( i = 0; i < str_len; i++ )
  {
    char_offset[i+1] = char_offset[i];
    char_offset[i+1] += u8g2_GetGlyphWidth(&u8g2, s[i]);	// assume s[i] is the glyph encoding --> no UTF8 support 
  }  
  curr_index = 0;
  curr_offset = 0;
}

void set_offset(u8g2_uint_t offset)
{
  for(;;)
  {
    if ( char_offset[curr_index] >= offset && offset < char_offset[curr_index+1] )
      break;
    
  }
}

void draw_text(void)
{
}


int main(void)
{
  int x, y;
  int k;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_helvB18_tn);
  
  x = 50;
  y = 30;

  
  for(;;)
  {
    
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_DrawStr(&u8g2, x, y, "123");
      u8g2_SetFontDirection(&u8g2, 1);
      u8g2_DrawStr(&u8g2, x, y, "123");
      u8g2_SetFontDirection(&u8g2, 2);
      u8g2_DrawStr(&u8g2, x, y, "123");
      u8g2_SetFontDirection(&u8g2, 3);
      u8g2_DrawStr(&u8g2, x, y, "123");
    } while( u8g2_NextPage(&u8g2) );
    
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 273 ) y -= 7;
    if ( k == 274 ) y += 7;
    if ( k == 276 ) x -= 7;
    if ( k == 275 ) x += 7;
    
    if ( k == 'e' ) y -= 1;
    if ( k == 'x' ) y += 1;
    if ( k == 's' ) x -= 1;
    if ( k == 'd' ) x += 1;
    if ( k == 'q' ) break;
    
  }
  
  //u8x8_Set8x8Font(u8g2_GetU8x8(&u8g2), bdf_font);
  //u8x8_Draw8x8String(u8g2_GetU8x8(&u8g2), 0, 0, "Hello World!");
  
  
  
  
  
  
  return 0;
}

