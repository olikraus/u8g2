/*

  test procedure for a draw arc procedure

  screenshot conversion:
  convert -flip -resize 256x128 screenshot.tga screenshot.png

*/


#include "u8g2.h"
#include <stdio.h>

#ifndef NO_SDL
#include "SDL.h"
#endif
  
u8g2_t u8g2;

// #define HIDE_INSTRUCTIONS

void spinner_animation(uint8_t animation_enabled, uint8_t* start, uint8_t* end)
{
  const uint8_t big_increment = 10;
  const uint8_t small_increment = 5;
  static uint8_t grow = 0, initialize = 1;
  static uint8_t prev_arc_length;

  if(!animation_enabled)
  {
    grow = 0;
    initialize = 1;
  }
  else
  {
    do
    {
      // Decrement angles
      *start = (256 + *start - (grow ? big_increment : small_increment)) % 256;
      *end = (256 + *end - (grow ? small_increment : big_increment)) % 256;

      // Get arc length
      uint8_t arc_length = (256 + *end - *start) % 256;
      if(initialize)
      {
        prev_arc_length = arc_length;
        initialize = 0;
      }

      // Check if arc overflows (grown above 255 or shrunk below 0)
      if((grow && arc_length < prev_arc_length) || (!grow && arc_length > prev_arc_length))
      {
        // invert start and end angles, and invert animation
        uint8_t tmp = *start;
        *start = *end;
        *end = tmp;
        grow = !grow;
        arc_length = grow ? 0 : 255;
      }

      prev_arc_length = arc_length; // update arc length for next iteration

    } while (*start == *end); // prevent drawing when arc length is 0 (arc function would draw a full circle)
  }
}


int main(void)
{
  int k;
  char buffer[256];

  uint8_t animation_enabled = 1;
  const uint8_t desired_fps = 30;
  uint32_t last_ticks = 0;

  u8g2_uint_t x = 30;
  u8g2_uint_t y = 32;
  
  uint8_t rad_in = 18;
  uint8_t rad_out = 20;
  
  uint8_t start = 0;
  uint8_t end = 220;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  
  u8x8_ConnectBitmapToU8x8(u8g2_GetU8x8(&u8g2));		/* connect to bitmap */
  
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);

  u8g2_SetFont(&u8g2, u8g2_font_tiny5_tf);
  u8g2_SetFontPosCenter(&u8g2);
  u8g2_SetFontMode(&u8g2, 1);
  

  for(;;)
  {
    #ifndef NO_SDL

      // Wait for next frame with constant FPS
      if (SDL_GetTicks() - last_ticks < 1000/desired_fps) continue;
      last_ticks = SDL_GetTicks();

      // Get current key press
      k = u8g_sdl_get_key();
      switch(k) {
        case ' ': animation_enabled = !animation_enabled; break;
        case 'y': rad_in -= 1; break;
        case 'u': rad_in += 1; break;
        case 'h': rad_out -= 1; break;
        case 'j': rad_out += 1; break;
        case 'i': start -= 1; break;
        case 'o': start += 1; break;
        case 'k': end -= 1; break;
        case 'l': end += 1; break;
        case 'e': case 273: y -= 1; break;
        case 'x': case 274: y += 1; break;
        case 's': case 276: x -= 1; break;
        case 'd': case 275: x += 1; break;
        case 'q': return 0;
        case 't': u8x8_SaveBitmapTGA(u8g2_GetU8x8(&u8g2), "screenshot.tga"); break;
      }
      if(k == ' ') printf(animation_enabled ? "play\n" : "pause\n");
      else if(k > 0) printf("x=%d, y=%d, rad_in=%d, rad_out=%d, start=%d, end=%d\n", x, y, rad_in, rad_out, start, end);

      // Animate if enabled
      spinner_animation(animation_enabled, &start, &end);
    #endif
    
    // Draw elements
    u8g2_FirstPage(&u8g2);
    do {
      
      // Draw arc for each radius
      for(u8g2_uint_t rad = rad_in; rad <= rad_out; rad++)
      {
        u8g2_DrawArc(&u8g2, x, y, rad, start, end);
      }
      

      // Draw instructions
      #ifndef HIDE_INSTRUCTIONS
        // separator
        u8g2_DrawVLine(&u8g2, 62, 0, 64);

        // anim
        u8g2_DrawTriangle(&u8g2, 66, 2, 66, 8, 71, 5);
        u8g2_DrawLine(&u8g2, 72, 8, 76, 2);
        u8g2_DrawBox(&u8g2, 79, 3, 2, 5);
        u8g2_DrawBox(&u8g2, 82, 3, 2, 5);
        u8g2_DrawUTF8(&u8g2, 87, 6, ":");
        (k == ' ' ? u8g2_DrawBox : u8g2_DrawFrame)(&u8g2, 90, 1, 35, 9);
        u8g2_DrawUTF8(&u8g2, 95, 6, "SPACE");

        // + / - icons
        u8g2_DrawHLine(&u8g2, 66, 17, 9);
        u8g2_DrawHLine(&u8g2, 117, 17, 9);
        u8g2_DrawVLine(&u8g2, 121, 13, 9);

        // rad_in
        (k == 'y' ? u8g2_DrawBox : u8g2_DrawFrame)(&u8g2, 66, 25, 9, 9);
        u8g2_DrawUTF8(&u8g2, 69, 30, "Y");
        (k == 'u' ? u8g2_DrawBox : u8g2_DrawFrame)(&u8g2, 117, 25, 9, 9);
        u8g2_DrawUTF8(&u8g2, 120, 30, "U");
        snprintf(buffer, sizeof(buffer), "IN:%d", rad_in);
        u8g2_DrawUTF8(&u8g2, 78, 30, buffer);

        // rad_out
        (k == 'h' ? u8g2_DrawBox : u8g2_DrawFrame)(&u8g2, 66, 35, 9, 9);
        u8g2_DrawUTF8(&u8g2, 69, 40, "H");
        (k == 'j' ? u8g2_DrawBox : u8g2_DrawFrame)(&u8g2, 117, 35, 9, 9);
        u8g2_DrawUTF8(&u8g2, 120, 40, "J");
        snprintf(buffer, sizeof(buffer), "OUT:%d", rad_out);
        u8g2_DrawUTF8(&u8g2, 78, 40, buffer);

        // start
        (k == 'i' ? u8g2_DrawBox : u8g2_DrawFrame)(&u8g2, 66, 45, 9, 9);
        u8g2_DrawUTF8(&u8g2, 69, 50, "I");
        (k == 'o' ? u8g2_DrawBox : u8g2_DrawFrame)(&u8g2, 117, 45, 9, 9);
        u8g2_DrawUTF8(&u8g2, 120, 50, "O");
        snprintf(buffer, sizeof(buffer), "START:%d", start);
        u8g2_DrawUTF8(&u8g2, 78, 50, buffer);

        // end
        (k == 'k' ? u8g2_DrawBox : u8g2_DrawFrame)(&u8g2, 66, 55, 9, 9);
        u8g2_DrawUTF8(&u8g2, 69, 60, "K");
        (k == 'l' ? u8g2_DrawBox : u8g2_DrawFrame)(&u8g2, 117, 55, 9, 9);
        u8g2_DrawUTF8(&u8g2, 120, 60, "L");
        snprintf(buffer, sizeof(buffer), "END:%d", end);
        u8g2_DrawUTF8(&u8g2, 78, 60, buffer);
      #endif

    } while( u8g2_NextPage(&u8g2) );
    
  }
  
  return 0;
}

