/* 
  
  png2bin.c
  
  Convert a png file to the internal file format, suitable for 
  https://github.com/olikraus/u8g2/tree/master/sys/arduino/u8g2_page_buffer/LoadFromSD
  
  Copyright (c) 2019, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  


  http://www.libpng.org/pub/png/libpng-manual.txt 

*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#define PNG_DEBUG 3
#include <png.h>

/*================================================*/
int is_invert = 0;
int is_16bit = 0;
int is_preview = 0;


/*================================================*/

int width, height;

png_byte color_type;
png_byte bit_depth;
uint8_t min, max;

png_structp png_ptr;
png_infop info_ptr;
png_bytep * row_pointers;

void print_info(const char* file_name)
{
  printf("%s: png_get_image_width(png_ptr, info_ptr)=%d\n", file_name, png_get_image_width(png_ptr, info_ptr));
  printf("%s: png_get_image_height(png_ptr, info_ptr)=%d\n", file_name, png_get_image_height(png_ptr, info_ptr));

  /*
#define PNG_COLOR_MASK_PALETTE    1
#define PNG_COLOR_MASK_COLOR      2
#define PNG_COLOR_MASK_ALPHA      4
  
#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_PALETTE  (PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_PALETTE)
#define PNG_COLOR_TYPE_RGB        (PNG_COLOR_MASK_COLOR)
#define PNG_COLOR_TYPE_RGB_ALPHA  (PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_ALPHA)
#define PNG_COLOR_TYPE_GRAY_ALPHA (PNG_COLOR_MASK_ALPHA)
*/
  
  printf("%s: png_get_color_type(png_ptr, info_ptr)=%d\n", file_name, png_get_color_type(png_ptr, info_ptr));
  printf("%s: png_get_bit_depth(png_ptr, info_ptr)=%d\n", file_name, png_get_bit_depth(png_ptr, info_ptr));
  printf("%s: png_get_rowbytes(png_ptr, info_ptr)=%lu\n", file_name, png_get_rowbytes(png_ptr, info_ptr));
  printf("%s: range: %d .. %d\n", file_name, min, max);
  
}

void print_short_info(const char* file_name)
{
  printf("%s rgb=%s alpha=%s w=%d h=%d\n", 
    file_name, 
    color_type&2?"no":"yes", 
    color_type&4?"no":"yes", 
    width, height);
}

uint8_t get_gray(int x, int y)
{
  
  uint8_t *p;
  uint8_t g;
  if ( color_type & PNG_COLOR_MASK_ALPHA )    
  {
    p = row_pointers[y] + x*4;
    g = ((int)p[0] + (int)p[1] + (int)p[2])/3;  
    //g = ((int)g+(int)p[3])/2;
    return g;
  }
  p = row_pointers[y] + x*3;
  return ((int)p[0] + (int)p[1] + (int)p[2])/3;  
}

uint8_t get_pixel(int x, int y)
{
  if ( x < 0 )
    return 0;
  if ( x >= width )
    return 0;
  if ( y < 0 )
    return 0;
  if ( y >= height )
    return 0;
  if ( get_gray(x, y) > 128/2 )
    return is_invert == 0 ? 1 : 0;
  return is_invert == 0 ? 0 : 1;
}

uint8_t get_pixel_byte(int x, int y)
{
  uint8_t b = 0;
  int i;
  for( i = 0; i < 8; i++ )
  {
      b |= get_pixel(x + i,y)<<i;  // lsb contains the leftmost pixel
  }
  return b;
}



void write_bdf_bitmap(const char *filename)
{
  int x, y;
  FILE *bin_fp;
  bin_fp = fopen(filename, "wb");
  
  if ( bin_fp == NULL )
  {
    perror(filename);
    return;
  }
  
  if ( is_16bit )
  {
    fputc(width>>8, bin_fp);
    fputc(width&255, bin_fp);
    fputc(height>>8, bin_fp);
    fputc(height&255, bin_fp);
  }
  else
  {
    fputc(width, bin_fp);
    fputc(height, bin_fp);
  }
  
  
  for( y = 0; y < height; y++ )
  {
    x = 0;
    while( x < width )
    {
      fputc(get_pixel_byte(x,y), bin_fp);
      x+=8;
    }
  }  
  
  fclose(bin_fp);
}


void show_ascii(void)
{
  int x, y;
  for( y = 0; y < height; y++ )
  {
    for( x = 0; x < width; x++ )
    {
      if ( is_invert )
      {
	if ( get_gray(x, y) <= 128/2 )
	  printf("#");
	else
	  printf(".");
      }
      else
      {
	if ( get_gray(x, y) > 128/2 )
	  printf("#");
	else
	  printf(".");
      }
    }
    printf("\n");
  }
}


void calc_min_max(void)
{
  int x, y;
  uint8_t val;
  min = 255;
  max = 0;
  for( y = 0; y < height; y++ )
  {
    for( x = 0; x < width; x++ )
    {
      val = get_gray(x, y);
      if ( max < val )
	max = val;
      if ( min > val )
	min = val;
    }
  }
}

int read_png_file(const char* file_name, const char* bin_name)
{
        unsigned char header[8];    // 8 is the maximum size that can be checked

        /* open file and test for it being a png */
        FILE *fp = fopen(file_name, "rb");
        if (fp == NULL)
	{
	  perror(file_name);
	  return 0;	/* open error */
	}
        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8))
	  return fclose(fp), 0;		/* not a png file error */

        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png_ptr == NULL)
	  return fclose(fp), 0;		/* alloc read struct */

        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL )
	  return fclose(fp), 0;		/* alloc info struct */

        if (setjmp(png_jmpbuf(png_ptr)))
	{
	    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	    return fclose(fp), 0;		/* library error */	  
	}

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);
        png_read_png(png_ptr, info_ptr, 
	  PNG_TRANSFORM_SCALE_16|
	  //PNG_TRANSFORM_STRIP_ALPHA|
	  PNG_TRANSFORM_PACKING|
	  //PNG_TRANSFORM_INVERT_ALPHA |
	  //PNG_TRANSFORM_INVERT_MONO | 
	  PNG_TRANSFORM_GRAY_TO_RGB
	  , NULL);

        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	row_pointers = png_get_rows(png_ptr, info_ptr);
	      
	//calc_min_max();
	//print_info(file_name);
	print_short_info(file_name);
	if ( is_preview )
	  show_ascii();
	write_bdf_bitmap(bin_name);
	
	 
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	fclose(fp);
	
	return 1;
	
}

void help(void)
{
  printf("png2bin [options] png-file bin-file\n");
  printf("Convert a png image to a binary \"XBM\" image, prefixed by hieght and length value.\n");
  printf("png-file format: RGB, gray, 1-bit index are supported, alpha channel is ignored, \n");
  printf("options:\n");
  printf("  -i         Invert image\n");
  printf("  -p         Preview image as ASCII art\n");
  printf("  -2         Two byte heigh/length value (16 bit values instead of 8 bit values, high byte first)\n");
  
}


int main(int argc, char **argv)
{
  if ( argc < 3 )
  {
    help();
    return 0;
  }
  argc--; argv++;
  for(;;)
  {
    if ( argv[0][0] == '-' )
    {
      if ( strcmp(argv[0], "-i") == 0 )
      {
	argc--; argv++;
	is_invert = 1;	
      }
      if ( strcmp(argv[0], "-2") == 0 )
      {
	argc--; argv++;
	is_16bit = 1;
      }
      if ( strcmp(argv[0], "-p") == 0 )
      {
	argc--; argv++;
	is_preview = 1;
      }
    }      
    else
    {
      break;
    }
  }

  
  if ( argv[0] != NULL && argv[1] != NULL )
  {
    if ( strcmp(argv[0], argv[1]) == 0 )
    {
      printf("source and destination file must be different\n");
    }
    else
    {
      read_png_file(argv[0], argv[1]);
    }
  }
  else
  {
    help();
  }
  return 0;
}


