/* 
  
  bmf2bdf.c
  
  Converter for bmf file format to bdf: http://bmf.php5.cz/
  
  Copyright (c) 2017, olikraus@gmail.com
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

*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* http://bmf.php5.cz/index.php?page=format */


uint8_t threshold = 128; // gray level threshold

FILE *bdf_fp = NULL;

int16_t lineHeight;
int16_t sizeOver;
int16_t sizeUnder;
int16_t addSpace;
int16_t sizeInner;
int16_t usedColors;
int16_t highestColor;
int16_t alphaBits;
int16_t extraPalettes;
int16_t reserved;
int16_t numColors;
uint8_t titleLength;
uint16_t asciiChars;
uint8_t whichChar;
uint8_t palette[256*3];
char title[256+2];
int8_t tablo[5];
uint8_t bitmap[256*256];


void write_bdf_header(void)
{
  fprintf(bdf_fp, "STARTFONT 2.1\n");
  fprintf(bdf_fp, "FONT");
  fprintf(bdf_fp, " \"%s\"\n", title);
    
  fprintf(bdf_fp, "SIZE 16 75 75\n");
  fprintf(bdf_fp, "FONTBOUNDINGBOX 16 16 0 0\n");
  fprintf(bdf_fp, "STARTPROPERTIES 3\n");

  fprintf(bdf_fp, "COPYRIGHT \"http://bmf.php5.cz\"");

  fprintf(bdf_fp, "FONT_ASCENT 0\n");
  fprintf(bdf_fp, "FONT_DESCENT 0\n");
  
  fprintf(bdf_fp, "ENDPROPERTIES\n");
  fprintf(bdf_fp, "CHARS %d\n", asciiChars);
  
}

uint8_t get_pixel(int x, int y)
{
  int width, height;
  uint16_t idx;
  width = (uint8_t)tablo[0];    
  height = (uint8_t)tablo[1];
  if ( x < 0 )
    return 0;
  if ( x >= width )
    return 0;
  if ( y < 0 )
    return 0;
  if ( y >= height )
    return 0;
  idx = bitmap[x+y*width];
  
  //uint8_t intensity = (palette[(idx-1)*3]+palette[(idx-1)*3+1]+palette[(idx-1)*3+2])/3;
  
  if ( idx == 0 )
    return 0;
  return 1;
}

uint8_t get_pixel_byte(int x, int y)
{
  uint8_t b = 0;
  int i;
  for( i = 0; i < 8; i++ )
  {
      b <<= 1;
      b |= get_pixel(x + i,y);
  }
  return b;
}

void write_4bit(int x)
{
  if ( bdf_fp == NULL )
    return;
  if ( x < 10 )
    fprintf(bdf_fp, "%c", x + '0' );
  else
    fprintf(bdf_fp, "%c", x - 10 + 'a'  );    
}

void write_byte(int x)
{
  write_4bit(x >> 4);
  write_4bit(x & 15);
}

void write_bdf_bitmap(uint32_t encoding)
{
  int x, y;
  int width, height;
  width = (uint8_t)tablo[0];    
  height = (uint8_t)tablo[1];
  
  if ( bdf_fp == NULL )
    return;
  
  fprintf(bdf_fp, "STARTCHAR %u\n", encoding);
  fprintf(bdf_fp, "ENCODING %u\n", encoding);
  fprintf(bdf_fp, "DWIDTH %d 0\n", tablo[4]);
  fprintf(bdf_fp, "BBX %d %d 0 0\n", width, height);
  fprintf(bdf_fp, "BITMAP\n");
  
  for( y = 0; y < height; y++ )
  {
    x = 0;
    while( x < width )
    {
      write_byte(get_pixel_byte(x,y));
      x+=8;
    }
    fprintf(bdf_fp, "\n"  );    
  }  
  fprintf(bdf_fp, "ENDCHAR\n");
  encoding++;
}

uint16_t readWord(FILE *file)
{
  uint16_t l, h;
  l = (uint8_t)fgetc(file);
  h = (uint8_t)fgetc(file);
  return h*256+l; 
}

int processBMF(const char *filename)
{
  FILE *file = fopen(filename, "rb");
  static int8_t buffer[18];
  int8_t version;
  uint8_t w, h;
  //uint8_t x, y;
  uint16_t bitmapSize;
  int i;
  
  if ( fread(buffer, 17, 1, file) != 1 )
    return fclose(file), 0;
  version = buffer[4];
  if (strncmp((char *)buffer, "\xE1\xE6\xD5\x1A", 4) !=  0 || (version != 0x11 && version != 0x12)) 
  {
    return fclose(file), 0; // not a BMF, version 1.1 or 1.2
  }
  lineHeight = buffer[5];
  sizeOver = buffer[6];
  sizeUnder = buffer[7];
  addSpace = buffer[8];
  sizeInner = buffer[9];
  usedColors = buffer[10];
  highestColor = buffer[11];
  alphaBits = buffer[12];
  extraPalettes = buffer[13];
  reserved = buffer[14];
  numColors = buffer[16];
  //printf("numColors=%d\n", (int)numColors);
  if ( fread(palette, numColors * 3, 1, file) != 1 )
    return fclose(file), 0;
  
  for (i = 0; i < numColors * 3; i++) 
  {
    palette[i] *= 4; // stretch 0..63 values to 0..255
  }
  
  /*
  for (i = 0; i < numColors ; i++) 
  {
    uint8_t intensity = (palette[i*3]+palette[i*3+1]+palette[i*3+2])/3;
    printf("color %d: %d %d %d --> %d\n", i, 
      palette[i*3], palette[i*3+1], palette[i*3+2], intensity);
  }
  */
  
  titleLength = (uint8_t)fgetc(file);
  if ( fread(title, titleLength, 1, file) != 1 )
    return fclose(file), 0;
  title[titleLength] = '\0';
  //printf("title=%s\n", title);
  asciiChars = readWord(file);
  
  //printf("asciiChars=%d\n", (int)asciiChars);

  
  write_bdf_header();
  for (i = 0; i < asciiChars; i++) 
  {
    whichChar = (uint8_t)fgetc(file);
    //printf("whichChar=%d\n", (int)whichChar);
    if ( fread(tablo, 5, 1, file) != 1 )
      return fclose(file), 0;
    w = tablo[0];    
    h = tablo[1];
    bitmapSize = ((uint16_t)w*(uint16_t)h);
    if ( bitmapSize > 0  ) 
    {
      if ( fread(bitmap, bitmapSize, 1, file) != 1 )
        return fclose(file), 0;
    }
    /*
    for( y = 0; y < h; y++)
    {
      for( x = 0; x < w; x++)
      {
        uint16_t idx = bitmap[x+y*w];
        if ( idx == 0 )  // color 0 is transparent
        {
          printf(" ");
        }
        else
        {
          uint8_t intensity = (palette[(idx-1)*3]+palette[(idx-1)*3+1]+palette[(idx-1)*3+2])/3;
          if ( intensity < threshold )
          {
            printf(" ");
          }
          else
          {
            printf("*");
          }
        }
      }
      printf("\n");
    }
    */
    
    write_bdf_bitmap(whichChar);
    
  }
  /*
  if (version >= 0x12) { // next goes for version 1.2 only
    unicodeChars = fread(file, 4);
    for (i = 0; i < unicodeChars; i++) {
      whichChar = fread(file, 4);
      tablo[whichChar] = fread(file, 5);
      if (bitmapSize = tablo[whichChar][0] * tablo[whichChar][1]) {
        bitmaps[whichChar] = fread(file, bitmapSize);
      }
    }
    numKerningPairs = fread(file, 4);
    kerningPairs = [];
    for (i = 0; i < numKerningPairs; i++) {
      fread(file, buffer, 10);
      kerningPairs []= buffer;
    }
  }
  */
  fclose(file);
  return 1;
}

void help(void)
{
  puts("bmf2bdf <file.bmf>");
}

int main(int argc, char **argv)
{
  bdf_fp = stdout;
  if ( argc < 2 )
  {
    help();
    return 0;
  }
  processBMF(argv[1]);
}

