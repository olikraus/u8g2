
#include "u8g2.h"
#include <stdio.h>

u8g2_t u8g2;


/*
  Linear Congruential Generator (LCG)
  z = (a*z + c) % m;  
  m = 256*256 (16 Bit)
  
  for period:
  a-1: dividable by 2
  a-1: multiple of 4
  c: not dividable by 2
  
  c = 17
  a-1 = 64 --> a = 65
*/
uint16_t rndz = 127;	// start value
uint16_t lcg(void) {
  rndz = ((uint16_t)65*(uint16_t)rndz + (uint16_t)17);
  return rndz;
}


#define ELEMENTS 7

/*
wolframalpha.com: plot ((0,0), (25, 10), (50,38), (80, 100), (100,128), (114, 137), (127, 128))
*/
int16_t xa[ELEMENTS] = { 0, 25, 50, 80, 100, 114, 127 } ;
int16_t ya[ELEMENTS] = { 0, 10, 38, 100 , 128, 137, 128};


uint16_t get_pos(uint16_t x)
{
  int i = 1;
  if ( x > xa[ELEMENTS-1] )
    return ya[ELEMENTS-1];
  while( xa[i] < x )
    i++;
  return ((x-xa[i-1]) * (ya[i]-ya[i-1])) / (xa[i]-xa[i-1]) + ya[i-1];
}

void u8g2_draw_mech_counter(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, int16_t step, const char *prev, const char *next)
{
    u8g2_uint_t h = u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2);
    u8g2_uint_t shift = 0;

    if ( step >= 0 )
    {
      shift = (get_pos(step)*h)/128; 
    }
    
    u8g2_DrawStr(u8g2, x, y+shift, prev);
    u8g2_DrawStr(u8g2, x, y-h+shift, next);
}

void u8g2_draw_counter(u8g2_t *u8g2, uint16_t rnd, u8g2_uint_t x, u8g2_uint_t dx, u8g2_uint_t y, uint8_t step, const char *prev, const char *next)
{
  uint8_t i;
  
  char p[2] = "0";
  char n[2] = "0";
  
  for( i = 0; i < 4; i++ )
  {
    p[0] = prev[i];
    n[0] = next[i];
    u8g2_draw_mech_counter(u8g2, x, y, step + (rnd & 31), p, n);
    rnd >>= 2;
    x += dx;
  }
}

int main(void)
{
  int x, y;
  int k;
  int i;
  int step = 0;
  uint16_t rnd;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, u8g2_font_helvB18_tn);
  
  x = 2;
  y = 28;

  rnd = lcg();
  
  for(;;)
  {
#ifdef U8G2_WITH_HVLINE_COUNT
    u8g2.hv_cnt = 0UL;
#endif /* U8G2_WITH_HVLINE_COUNT */   
    
    /*
    u8g2_ClearBuffer(&u8g2);
    
      u8g2_SetFontDirection(&u8g2, 0);
      u8g2_DrawStr(&u8g2, x, y, "ABC");
      u8g2_SetFontDirection(&u8g2, 1);
      u8g2_DrawStr(&u8g2, x, y, "abc");
      u8g2_SetFontDirection(&u8g2, 2);
      u8g2_DrawStr(&u8g2, x, y, "abc");
      u8g2_SetFontDirection(&u8g2, 3);
      u8g2_DrawStr(&u8g2, x, y, "abc");
    
    u8g2_SendBuffer(&u8g2);
    */
    
    u8g2_FirstPage(&u8g2);
    i = 0;
    do
    {
      u8g2_SetFont(&u8g2, u8g2_font_mystery_quest_32_tr);
      //u8g2_DrawStr(&u8g2, x,y, "234");
      
      u8g2_draw_counter(&u8g2, rnd, x, 15, y, step, "2345", "9176");
      
      u8g2_SetFont(&u8g2, u8g2_font_mystery_quest_24_tr);
      u8g2_DrawStr(&u8g2, x+74,y-2, "RPM");
      
      //u8g2_SetFont(&u8g2, u8g2_font_mystery_quest_32_tr);
      //u8g2_draw_mech_counter(&u8g2, x+48, y, step, "5","6");
      
     // u8g2_SetFont(&u8g2, u8g2_font_mystery_quest_32_tr);
      //u8g2_DrawStr(&u8g2, x,y+32, "3210");
      //u8g2_SetFont(&u8g2, u8g2_font_mystery_quest_24_tr);
      //u8g2_DrawStr(&u8g2, x+74,y+30, "mV");
      
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

    if ( k == ' ' ) step += 3;

    if ( step > 160 )   
    {
          rnd = lcg();
          step = 0;
    }  
    
    if ( k == 'q' ) break;
    
    //printf("%u\n", lcg());
    
  }
  
  //u8x8_Set8x8Font(u8g2_GetU8x8(&u8g2), bdf_font);
  //u8x8_Draw8x8String(u8g2_GetU8x8(&u8g2), 0, 0, "Hello World!");
  
  
  
  
  
  
  return 0;
}

