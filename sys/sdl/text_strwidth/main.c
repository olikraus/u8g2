
#include "u8g2.h"
#include <stdio.h>

u8g2_t u8g2;

void draw(int x, int y)
{
	char *list[] = {"|", ">", "-|", "lazydog|", "lazydog!", "lazy|dog"};
	u8g2_uint_t fh, w;
	u8g2_DrawLine(&u8g2, 120, 0, 120, 63);

	u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
	//u8g2_SetFont(&u8g2, u8g2_font_helvB08_tf);
	u8g2_SetFontPosTop(&u8g2);
	fh = u8g2_GetFontAscent(&u8g2) - u8g2_GetFontDescent(&u8g2);

	int i;
	for (i = 0; i < sizeof(list)/sizeof(char*); i++) {
		w = u8g2_GetStrWidth(&u8g2, list[i]);
		u8g2_DrawStr(&u8g2, 120 - w, fh * i, list[i]);
		//u8g2.setCursor(0,fh * i);
		//u8g2.print(w);
	}	
}


int main(void)
{
  int x, y;
  int k;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  //u8g2_SetFont(&u8g2, u8g2_font_helvB18_tr);
  u8g2_SetFont(&u8g2, u8g2_font_helvB18_tf);
  
  x = 50;
  y = 30;
  
  for(;;)
  {
    
    u8g2_FirstPage(&u8g2);
    do
    {
	draw(x,y);
	/*
      u8g2_SetFontDirection(&u8g2, 0); 
      u8g2_DrawUTF8(&u8g2, x, y, "WAV");
      u8g2_SetFontDirection(&u8g2, 1); 
      u8g2_DrawUTF8(&u8g2, x, y, "WAV");
      u8g2_SetFontDirection(&u8g2, 2);
      u8g2_DrawUTF8(&u8g2, x, y, "WAV");
      u8g2_SetFontDirection(&u8g2, 3);
      u8g2_DrawUTF8(&u8g2, x, y, "WAV");
	    */
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
  
  return 0;
}

