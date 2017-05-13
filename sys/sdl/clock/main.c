/*

  main.c
  
*/

#include "gui.h"
#include <stdio.h>

  
u8g2_t u8g2;



int main(void)
{
  int x, y;
  int k;
  int i;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  
  gui_Init(&u8g2, 0);
  
  x = 50;
  y = 50;

  
  for(;;)
  {
    u8g2_FirstPage(&u8g2);
    i = 0;
    do
    {
      gui_Draw();
      i++;
      
    } while( u8g2_NextPage(&u8g2) );
   
    
    
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 'n' )
      gui_Next();

    if ( k == 's' )
      gui_Select();
    
    if ( k == 'm' )
    {
      gui_data.mo++;
      if ( gui_data.mo >= 10 )
      {
	gui_data.mo = 0;
	gui_data.mt++;
	if ( gui_data.mt >= 60 )
	{
	  gui_data.h++;
	  if ( gui_data.h >= 24 )
	  {
	    gui_data.h = 0;
	    gui_data.day++;
	  }
	}
      }
      gui_SignalTimeChange();
    }

    if ( k == 'h' )
    {
      gui_data.h++;
      if ( gui_data.h >= 24 )
      {
	gui_data.h = 0;
	gui_data.day++;
      }
      gui_SignalTimeChange();
    }
    
    if ( k == 273 ) y -= 1;
    if ( k == 274 ) y += 1;
    if ( k == 276 ) x -= 1;
    if ( k == 275 ) x += 1;
    
    if ( k == 'q' ) break;
    
  }
  
  //u8x8_Set8x8Font(u8g2_GetU8x8(&u8g2), bdf_font);
  //u8x8_Draw8x8String(u8g2_GetU8x8(&u8g2), 0, 0, "Hello World!");
  
  
  
  
  
  
  return 0;
}

