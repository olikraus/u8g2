#include "u8x8.h"

/*
https://www.streamlinehq.com/icons/pixel?search=&icon=ico_94BhV342TAIclS4E
Business_Product_Startup_1__Streamline_Pixel
Made by Streamline
24x24 XBM
*/
static unsigned char startup_xbm[] = {
   0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x24, 0x00, 0x00, 0x42, 0x00,
   0x00, 0xff, 0x00, 0x00, 0x81, 0x00, 0x80, 0x18, 0x01, 0x80, 0x34, 0x01,
   0x80, 0x3c, 0x01, 0xc0, 0x18, 0x03, 0xa0, 0x00, 0x05, 0x90, 0x40, 0x09,
   0x90, 0x00, 0x09, 0xd0, 0x40, 0x0b, 0xb0, 0x00, 0x0d, 0x10, 0xff, 0x08,
   0x00, 0x42, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x80, 0xa5, 0x01,
   0x6c, 0x24, 0x36, 0x12, 0x42, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };




     

   

u8x8_t u8x8;

uint8_t *get_tile_from_xbm(uint8_t tx, uint8_t ty, uint8_t xbm_byte_width, const unsigned char *xbm)
{
  static uint8_t d[8];
  uint8_t mask = 1;
  uint8_t i;
  const uint8_t *p = xbm;
  p += tx;
  p += xbm_byte_width*ty*8;
  
  for( i = 0; i < 8; i++ )
    d[i] = 0;
  for( i = 0; i < 8; i++ )
  {
    if  ( *p & 1 ) d[0] |= mask;
    if  ( *p & 2 ) d[1] |= mask;
    if  ( *p & 4 ) d[2] |= mask;
    if  ( *p & 8 ) d[3] |= mask;
    if  ( *p & 16 ) d[4] |= mask;
    if  ( *p & 32 ) d[5] |= mask;
    if  ( *p & 64 ) d[6] |= mask;
    if  ( *p & 128 ) d[7] |= mask;
    mask <<= 1;
    p += xbm_byte_width;
  }
  return d;
}
   
void u8x8_draw_xbm(uint8_t tx, uint8_t ty, uint8_t xbm_width, uint8_t xbm_height, const unsigned char *xbm)
{
  uint8_t x, y;
  uint8_t *tile;
  for( y = 0; y < xbm_height/8; y++ )
  {
    for( x = 0; x < xbm_width/8; x++ )
    {
      tile = get_tile_from_xbm(x, y, xbm_width/8, xbm);
      u8x8_DrawTile(&u8x8, tx+x, ty+y, 1, tile);
      // u8x8.drawTile(tx+x, ty+y, 1, tile);
    }
  }
}


int main(void)
{
  u8x8_Setup_SDL_128x64(&u8x8);
  u8x8_InitDisplay(&u8x8);  

  u8x8_draw_xbm(1, 1, 24, 24, startup_xbm);

  while( u8g_sdl_get_key() < 0 )
    ;
  
  
  return 0;
}

