#include "u8x8.h"

u8x8_t u8x8;


void show(uint8_t *buf)
{
  uint8_t i, j;
  for( i = 0; i < 8; i++ )
  {
    printf("%02x ", buf[i]);
    for( j = 0; j < 8; j++ )
    {
      if ( buf[i] & (1<<(7-j)) )
        printf("*");
      else
        printf(".");
    }
    printf("\n");
  }
}

/*
static uint8_t *rotate270(uint8_t *buf)
{
  static uint8_t rbuf[8];
  uint8_t i, h;
  uint8_t *p;
  for( i = 0; i < 8; i++)
    rbuf[i] = 0;
  for( i = 0; i < 8; i++ )
  {
    h = buf[i];
    p = rbuf;
    *p<<=1; *p |= (h&1); h >>= 1; p++;
    *p<<=1; *p |= (h&1); h >>= 1; p++;
    *p<<=1; *p |= (h&1); h >>= 1; p++;
    *p<<=1; *p |= (h&1); h >>= 1; p++;
    *p<<=1; *p |= (h&1); h >>= 1; p++;
    *p<<=1; *p |= (h&1); h >>= 1; p++;
    *p<<=1; *p |= (h&1); h >>= 1; p++;
    *p<<=1; *p |= (h&1); h >>= 1; p++;
  }
  return rbuf;
}
*/


static uint8_t *rotate90(uint8_t *buf)
{
  static uint8_t rbuf[8];
  uint8_t i, h;
  uint8_t *p;
  uint8_t j;
  for( i = 0; i < 8; i++)
    rbuf[i] = 0;
  for( i = 0; i < 8; i++ )
  {
    h = buf[i];
    p = rbuf;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
    *p>>=1; *p |= (h&128); h <<= 1; p++;
  }
  return rbuf;
}

void u8x8_draw_glyph_90(u8x8_t *u8x8, uint8_t x, uint8_t y, uint8_t encoding)
{
  static uint8_t buf[8];
  u8x8_get_glyph_data(u8x8, encoding, buf, 0);
  u8x8_DrawTile(u8x8, x, y, 1, rotate90(buf));
}

void u8x8_draw_string_90(u8x8_t *u8x8, uint8_t x, uint8_t y, const char *s)
{
  while(*s != '\0')
    u8x8_draw_glyph_90(u8x8, x, y++, *s++); 
}

int main(void)
{
  
  u8x8_Setup_SDL_128x64(&u8x8);
  u8x8_InitDisplay(&u8x8);  

  u8x8_SetFont(&u8x8, u8x8_font_victoriabold8_r);
  
  
  u8x8_DrawString(&u8x8, 1, 1, "Abcd");
  u8x8_draw_string_90(&u8x8, 1,2, "Abcd");
  //u8x8_draw_glyph_90(&u8x8, 1, 4, 'B'); 


  while( u8g_sdl_get_key() < 0 )
    ;
  
  
  return 0;
}

