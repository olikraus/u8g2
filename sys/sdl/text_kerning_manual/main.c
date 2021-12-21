
#include "u8g2.h"
#include <stdio.h>

u8g2_t u8g2;



const uint16_t distance_adjust_table[] = 
{
	/* first char, second char, gap reduction value */
	0x09AE, 0x9BE, 12, /* reduce distance between ম  and া  by 12 */
        /* add more pairs here... */
  0x09B8, 0x09CB, 12, //for স and ো
  0x09A8, 0x09BE, 12, //for ন and া
  0x09AE, 0x09BF, 14, //for ম and ি
  0x09AC, 0x09BE, 12, //for ব and া
  0x09BE, 0x0982, 12, //for া and ং
  0x09B2, 0x09BE, 12, //for ল and া
  0x09AC, 0x09CD, 12, //for ব and ্
  0x09CD, 0x09A6, 12, //for ্ and দ	
	/* this line terminates the table */
	0xffff, 0xffff, 0xffff
};

/* get distance from the distance table */
uint16_t get_distance_adjust(uint16_t e1, uint16_t e2)
{
  uint16_t i;
  i = 0;
  for(;;)
  {
    if ( distance_adjust_table[i] == 0x0ffff )
      break;
    if ( distance_adjust_table[i] == e1 && distance_adjust_table[i+1] == e2 )
      return distance_adjust_table[i+2];
    i+=3;
  }
  return 0;
}

u8g2_uint_t draw_string(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *str)
{
  uint16_t e_prev = 0x0ffff;
  uint16_t e;
  u8g2_uint_t delta, adjust, sum;
  
  delta = 0;
  adjust = 0;
  u8g2->u8x8.next_cb = u8x8_utf8_next;
  
  u8x8_utf8_init(u8g2_GetU8x8(u8g2));
  sum = 0;
  for(;;)
  {
    e = u8g2->u8x8.next_cb(u8g2_GetU8x8(u8g2), (uint8_t)*str);
    if ( e == 0x0ffff )
      break;
    str++;
    if ( e != 0x0fffe )
    {
      delta = u8g2_GetGlyphWidth(u8g2, e);	    
      adjust = get_distance_adjust(e_prev, e);
      e_prev = e;
      u8g2_DrawGlyph(u8g2, x-adjust, y, e);
      x += delta-adjust;
      sum += delta-adjust;    
    }
  }
  return sum;
}




int main(void)
{
  int x, y;
  int k;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  //u8g2_SetFont(&u8g2, u8g2_font_helvB18_tr);
  
  x = 20;
  y = 30;

  
  for(;;)
  {
    u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_bengali);
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_DrawUTF8(&u8g2, 0, y, "সোনার");
      draw_string(&u8g2, 0, y+15, "সোনার");
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

