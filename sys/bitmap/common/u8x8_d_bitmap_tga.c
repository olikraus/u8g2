#include "stdlib.h"	/* malloc */
#include "stdint.h"	/* uint16_t */
#include "stdio.h"	/* FILE */
#include "u8g2.h"

static void tga_write_byte(FILE *fp, uint8_t byte)
{
  fputc(byte, fp);
}

static void tga_write_word(FILE *fp, uint16_t word)
{
  tga_write_byte(fp, word&255);
  tga_write_byte(fp, word>>8);
}

void u8x8_SaveBitmapTGA(u8x8_t *u8x8, const char *name)
{
  FILE *fp;
  uint16_t x, y;

  uint16_t pixel_width = u8x8->display_info->pixel_width;
  uint16_t pixel_height = u8x8->display_info->pixel_height;
  
  fp = fopen(name, "wb");
  if ( fp != NULL )
  {
    tga_write_byte(fp, 0);		/* no ID */
    tga_write_byte(fp, 0);		/* no color map */
    tga_write_byte(fp, 2);		/* uncompressed true color */
    tga_write_word(fp, 0);		
    tga_write_word(fp, 0);		
    tga_write_byte(fp, 0);		
    tga_write_word(fp, 0);		/* x origin */
    tga_write_word(fp, 0);		/* y origin */
    tga_write_word(fp, pixel_width);		/* width */
    tga_write_word(fp, pixel_height);		/* height */
    tga_write_byte(fp, 24);		/* color depth */
    tga_write_byte(fp, 0);	

    for( y = 0; y < pixel_height; y++ )
    {
      for( x = 0; x < pixel_width; x++ )
      {
	if ( u8x8_GetBitmapPixel(u8x8, x, pixel_height-y-1) == 0 )
	{
	  tga_write_byte(fp, 255);		/* R */
	  tga_write_byte(fp, 255);		/* G */
	  tga_write_byte(fp, 255);		/* B */
	}
	else
	{
	  tga_write_byte(fp, 0);		/* R */
	  tga_write_byte(fp, 0);		/* G */
	  tga_write_byte(fp, 0);		/* B */
	}
      }
    }
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    tga_write_word(fp, 0);
    fwrite("TRUEVISION-XFILE.", 18, 1, fp);
    fclose(fp);
  }
}
