
#include "u8g2.h"
#include <stdio.h>

u8g2_t u8g2;


static const uint16_t helv_first_encoding_table[50] = {
  65, 67, 70, 75, 76, 80, 84, 86, 87, 88, 89, 97, 98, 99, 101, 103, 104, 
  107, 109, 110, 111, 112, 113, 114, 115, 117, 118, 119, 120, 121, 122, 192, 193, 
  194, 195, 196, 197, 199, 215, 221, 222, 224, 230, 231, 232, 242, 248, 249, 254, 
  65535};
static const uint16_t helv_index_to_second_table[50] = {
  0, 7, 9, 19, 39, 47, 56, 92, 101, 109, 112, 135, 138, 141, 142, 145, 146, 
  150, 151, 154, 158, 161, 164, 165, 169, 172, 173, 176, 177, 178, 181, 182, 189, 
  196, 203, 210, 217, 224, 226, 229, 252, 255, 258, 261, 262, 265, 268, 269, 270, 
  273};
static const uint16_t helv_second_encoding_table[273] = {
  84, 86, 87, 89, 121, 221, 253, 121, 253, 65, 74, 122, 192, 193, 194, 195, 196, 
  197, 198, 67, 71, 79, 81, 116, 117, 118, 119, 121, 199, 210, 211, 212, 213, 
  214, 249, 250, 251, 252, 253, 84, 86, 87, 89, 118, 121, 221, 253, 65, 74, 
  192, 193, 194, 195, 196, 197, 198, 65, 74, 97, 99, 100, 101, 103, 109, 110, 
  111, 112, 113, 114, 115, 117, 118, 119, 120, 121, 122, 192, 193, 194, 195, 196, 
  197, 198, 215, 225, 230, 231, 233, 243, 248, 250, 253, 65, 74, 192, 193, 194, 
  195, 196, 197, 198, 65, 192, 193, 194, 195, 196, 197, 198, 118, 121, 253, 65, 
  74, 97, 99, 100, 101, 103, 111, 113, 192, 193, 194, 195, 196, 197, 198, 225, 
  229, 230, 231, 233, 243, 248, 84, 89, 221, 84, 89, 221, 84, 84, 89, 221, 
  84, 84, 86, 89, 221, 84, 84, 89, 221, 84, 86, 89, 221, 84, 89, 221, 
  84, 89, 221, 84, 74, 84, 88, 90, 84, 89, 221, 84, 84, 88, 90, 84, 
  84, 84, 88, 90, 84, 84, 86, 87, 89, 121, 221, 253, 84, 86, 87, 89, 
  121, 221, 253, 84, 86, 87, 89, 121, 221, 253, 84, 86, 87, 89, 121, 221, 
  253, 84, 86, 87, 89, 121, 221, 253, 84, 86, 87, 89, 121, 221, 253, 121, 
  253, 84, 89, 221, 65, 74, 97, 99, 100, 101, 103, 111, 113, 192, 193, 194, 
  195, 196, 197, 198, 225, 229, 230, 231, 233, 243, 248, 84, 88, 90, 84, 89, 
  221, 84, 89, 221, 84, 84, 89, 221, 84, 89, 221, 84, 84, 84, 89, 221};
static const uint8_t helv_kerning_values[273] = {
  2, 2, 1, 3, 1, 3, 1, 1, 1, 1, 5, 3, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 3, 2, 3, 1, 3, 1, 3, 3, 3, 1, 5, 
  1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 
  2, 3, 2, 3, 2, 3, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 
  2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 
  2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 
  3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 1, 
  1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 1, 1, 
  3, 3, 1, 2, 2, 2, 3, 1, 1, 3, 1, 2, 2, 2, 1, 1, 
  2, 1, 1, 3, 1, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 
  2, 2, 1, 1, 2, 2, 2, 1, 3, 1, 3, 1, 2, 2, 1, 3, 
  1, 3, 1, 2, 2, 1, 3, 1, 3, 1, 2, 2, 1, 3, 1, 3, 
  1, 2, 2, 1, 3, 1, 3, 1, 2, 2, 1, 3, 1, 3, 1, 1, 
  1, 3, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 
  3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 
  1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1};
u8g2_kerning_t helv = {
  50, 273,
  helv_first_encoding_table,
  helv_index_to_second_table,
  helv_second_encoding_table,
  helv_kerning_values};

int main(void)
{
  int x, y;
  int k;
  int i;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  //u8g2_SetFont(&u8g2, u8g2_font_helvB18_tr);
  u8g2_SetFont(&u8g2, u8g2_font_helvB18_tf);
  
  x = 50;
  y = 30;

  
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
    
    u8g2.kerning = &helv;
    u8g2.get_kerning_cb = u8g2_GetKerning;
    
    u8g2_FirstPage(&u8g2);
    i = 0;
    do
    {
      u8g2_SetFontDirection(&u8g2, 0); 
      u8g2_DrawUTF8(&u8g2, x, y, "WAV");
      u8g2_SetFontDirection(&u8g2, 1); 
      u8g2_DrawUTF8(&u8g2, x, y, "WAV");
      u8g2_SetFontDirection(&u8g2, 2);
      u8g2_DrawUTF8(&u8g2, x, y, "WAV");
      u8g2_SetFontDirection(&u8g2, 3);
      u8g2_DrawUTF8(&u8g2, x, y, "WAV");
      if ( i == 1 )
      {
	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y0, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x0, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y1-1, 1, 0);
	u8g2_DrawHVLine(&u8g2, u8g2.user_x1-1, u8g2.user_y0, 1, 0);

      }
      
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

