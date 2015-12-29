
#include "u8x8.h"
#include "u8g2.h"



extern uint8_t tga_is_transparent;

extern uint8_t tga_desc_fg_r;
extern uint8_t tga_desc_fg_g;
extern uint8_t tga_desc_fg_b;

extern uint8_t tga_desc_bg_r;
extern uint8_t tga_desc_bg_g;
extern uint8_t tga_desc_bg_b;

extern uint8_t tga_lcd_fg_r;
extern uint8_t tga_lcd_fg_g;
extern uint8_t tga_lcd_fg_b;

extern uint8_t tga_lcd_bg_r;
extern uint8_t tga_lcd_bg_g;
extern uint8_t tga_lcd_bg_b;

extern void tga_save_png(const char *name);

u8x8_t u8x8;
u8g2_t u8g2;
u8g2_t desc;

void ra(uint16_t tx, uint16_t ty, const char *s)
{
  int x,y, i, w;
  x = tx*3+64*3 + 0;
  y = ty*3+32*3 + 1;
  for( i = 0; i < 6; i++)
  {
    u8g2_DrawHVLine(&desc, x-2*i, y-i, 1+2*i, 1);
    u8g2_DrawHVLine(&desc, x-2*i-1, y-i, 1+2*i, 1);
  }
  u8g2_DrawHVLine(&desc, x-30, y-1, 28, 0);
  u8g2_DrawHVLine(&desc, x-30, y, 20, 0);
  u8g2_DrawHVLine(&desc, x-30, y+1, 28, 0);

  w = u8g2_GetStringWidth(&desc, s);
  u8g2_DrawStr(&desc, x-34-w, y+u8g2_GetAscent(&desc)/2, s);
  
  
}

/* vertical measure */
/* tx,ty: lower point */
void vm(uint16_t tx, uint16_t ty, uint16_t dy)
{
  char s[10];
  
  int x,y, i, w;
  x = tx*3+64*3 + 1;
  y = ty*3+32*3 +3;
  
  u8g2_DrawHVLine(&desc, x-3, y-1, 7, 0);
  u8g2_DrawHVLine(&desc, x, y-dy*3, dy*3, 1);
  u8g2_DrawHVLine(&desc, x-3, y-dy*3, 7, 0);

  sprintf(s, "%d", dy);
  w = u8g2_GetStringWidth(&desc, s);
  u8g2_DrawStr(&desc, x+6, y-(dy*3)/2+u8g2_GetAscent(&desc)/2, s);
}


int main(void)
{
  /*
  tga_desc_fg_r = 100;
  tga_desc_fg_g = 118;
  tga_desc_fg_b = 135;
  */

  tga_desc_fg_r = 76;
  tga_desc_fg_g = 0;
  tga_desc_fg_b = 205;

  tga_desc_bg_r = 255;
  tga_desc_bg_g = 255;
  tga_desc_bg_b = 255;

  tga_lcd_fg_r = 0;
  tga_lcd_fg_g = 0;
  tga_lcd_fg_b = 0;

  tga_lcd_bg_r = 250;
  tga_lcd_bg_g = 193;
  tga_lcd_bg_b = 50;

  /*
  u8x8_Setup_TGA_DESC(&desc);
  u8x8_InitDisplay(&desc);  
  u8x8_SetPowerSave(&desc, 0);
  u8x8_ClearDisplay(&desc);
  u8x8_SetFont(&desc, u8x8_font_amstrad_cpc_extended_r);
  u8x8_DrawString(&desc, 0, 0, "Description");
*/

  u8g2_SetupBuffer_TGA_DESC(&desc, &u8g2_cb_r0);
  u8g2_InitDisplay(&desc);  
  u8g2_SetPowerSave(&desc, 0);
  u8g2_ClearDisplay(&desc);


  u8g2_SetupBuffer_TGA_LCD(&u8g2, &u8g2_cb_r0);
  u8g2_InitDisplay(&u8g2);  
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_ClearDisplay(&u8g2);



  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tf);
    u8g2_DrawStr(&u8g2, 0, 15, "Hello World!");
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(0,15, "x=0, y=15");
  } while( u8g2_NextPage(&desc) );
  tga_is_transparent = 0;

  tga_save_png("u8g2_hello_world.png");

/*=========================================*/
  
  u8x8_ClearDisplay(u8g2_GetU8x8(&desc)); 
  
  u8g2_FirstPage(&u8g2);
  do
  {
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    u8g2_DrawStr(&u8g2, 5, 20, "A  gi");
  } while( u8g2_NextPage(&u8g2) );

  tga_is_transparent = 1;
  u8g2_FirstPage(&desc);
  do
  {
    u8g2_SetFont(&desc, u8g2_font_helvB18_tf);
    ra(5,20, "x=5, y=20");
    u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tf);
    vm(25,19, u8g2_GetAscent(&u8g2));
    
    /* descent usually is negative */
    vm(62,19-u8g2_GetDescent(&u8g2), -u8g2_GetDescent(&u8g2));
  } while( u8g2_NextPage(&desc) );
  tga_is_transparent = 0;

  tga_save_png("u8g2_ascent_descent.png");

/*=========================================*/

  u8x8_ClearDisplay(u8g2_GetU8x8(&desc));
  
  
  return 0;
}

