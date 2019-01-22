
#include "u8g2.h"
#include <stdio.h>

u8g2_t u8g2;

/*
  draw unicode for https://en.wikipedia.org/wiki/Devanagari
  Adjust the glyph position as good as possible for the unicode font.

  Report missing or wrong glyph adjustments here:
  https://github.com/olikraus/u8g2/issues/584
  
  precondition: 
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_devanagari);
    u8g2_SetFontMode(&u8g2, 1);
    Font direction command is NOT supported
*/
u8g2_uint_t u8g2_draw_unifont_devanagari(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, const char *str)
{
  uint16_t e;
  u8g2_uint_t delta, sum;
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
      
      switch(e)
      {
	case 0x093e: x-= 12; break;
	case 0x093f: x-= 19; break;
	case 0x0941: x-= 14; break;
	case 0x0947: x-= 12; break;
	case 0x094d: x-= 10; break;
      }
      delta = u8g2_DrawGlyph(u8g2, x, y, e);    
      switch(e)
      {
	case 0x094d: x-= 8; break;
      }
      
      x += delta;
      sum += delta;    
    }
  }
  return sum;
}


int main(void)
{
  int x, y, o;
  int k;
  int i;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  //u8g2_SetFont(&u8g2, u8g2_font_helvB18_tr);
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_devanagari);
  u8g2_SetFontMode(&u8g2, 1);
  
  x = 50;
  y = 30;
  o = 0;

  
  for(;;)
  {
#ifdef U8G2_WITH_HVLINE_COUNT
    u8g2.hv_cnt = 0UL;
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
    
    u8g2_FirstPage(&u8g2);
    i = 0;
    do
    {
      u8g2_SetFontDirection(&u8g2, 0); 
      
      /* अविनाश */
      /* &#x0905;&#x0935;&#x093F;&#x0928;&#x093E;&#x0936; */
      /*
      o = 0;
      o += u8g2_DrawGlyph(&u8g2, x+o, y, 0x0905);
      o += u8g2_DrawGlyph(&u8g2, x+o, y, 0x0935);
      o -=19;	// offset 19 for 0x093F S
      o += u8g2_DrawGlyph(&u8g2, x+o, y, 0x093F);
      o += u8g2_DrawGlyph(&u8g2, x+o, y, 0x0928);
      o -=12;	// offset 19 for 0x093e 
      o += u8g2_DrawGlyph(&u8g2, x+o, y, 0x093e);
      o += u8g2_DrawGlyph(&u8g2, x+o, y, 0x0936);
      */
      
      
      //u8g2_draw_devanagari(&u8g2, x, y+16, "अविनाश");
      
      /* नमस्ते दुनिया Hello World */
      /*
	&#x0928;&#x092E;&#x0938;&#x094D;&#x0924;&#x0947; 
	&#x0926;&#x0941;&#x0928;&#x093F;&#x092F;&#x093E;
      */
      u8g2_draw_unifont_devanagari(&u8g2, x, y+16, "नमस्ते दुनिया");
      
      i++;
      

    } while( u8g2_NextPage(&u8g2) );
#ifdef U8G2_WITH_HVLINE_COUNT
    printf("hv cnt: %ld\n", u8g2.hv_cnt);
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
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

