/* 
  
  bmf2bdf.c
  
  Converter for bmf font file format to bdf file format 
    bmf: http://bmf.php5.cz/
    bdf: https://en.wikipedia.org/wiki/Glyph_Bitmap_Distribution_Format
  
  Copyright (c) 2022, olikraus@gmail.com
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

  Features
    - Convert .bmf to .bdf, result will be written to stdout
    - Converts DOS codepage to Unicode (-u option)
    - Convert monospaced proportional font back to none-monospaced font (-p)
    - Optional glyph spacing with -x
    - Optional baseline adjustment with -y
    - Brightness threshold with -l
    - Generated BDF can be modified with gbdfed and FontForge
      (http://sofia.nmsu.edu/~mleisher/Software/gbdfed/, https://github.com/andrewshadura/gbdfed)
    
  Example:
    http://bmf.php5.cz/index.php?font=goth16
      bmf2bdf -u -p -y -2 goth16.bmf > goth16.bdf
    or
      bmf2bdf -u -p -x 1 -y -2 goth16.bmf > goth16.bdf

  
  

*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* http://bmf.php5.cz/index.php?page=format */


//uint8_t threshold = 128; // gray level threshold

FILE *bdf_fp = NULL;

/*
  option "-p"
  default: Use the shift value from bmf format for the BDF DWIDTH property
  "-p": Calculate the bdf DWIDTH from width, relx and addSpace vales. 
    This will force a none-fixed = proportional font
*/
int optionForceProportional = 0;

/*
  option "-u"
  default: No glyph number mapping
  "-u": Apply dos to unicode translation 
*/
int optionUnicodeMapping = 0;

/*
  option "-x #"
  default: Do not add any extra space
  "-x #": Add # pixel extra space between chars, might be useful for -p
*/
int optionAddExtraSpace = 0;

/*
  option "-y #"
  default: Do not shift chars vertically
  "-y #": Vertical shift of all glyphs (useful to adjust baseline, # can be negative);
*/
int optionShiftY = 0;

/*
  options "-l #"  "-h #"
  default: 0 ... 255
  Specified the low and high gray color range for pixel which should appear as set.
*/
int optionGrayLow = 0;
int optionGrayHigh = 255;

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
#define BITMAP_SIZE (256*256)
uint8_t bitmap[BITMAP_SIZE];
uint32_t dos2unicode[256];

int totalGlyphSize = 0;
int cntGlyphSize = 0;
int averageGlyphSize = 0;


void write_bdf_header(void)
{
  fprintf(bdf_fp, "STARTFONT 2.1\n");
  fprintf(bdf_fp, "FONT");
  fprintf(bdf_fp, " \"%s\"\n", title);
    
  fprintf(bdf_fp, "SIZE 16 75 75\n");
  fprintf(bdf_fp, "FONTBOUNDINGBOX 16 16 0 0\n");
  fprintf(bdf_fp, "STARTPROPERTIES 3\n");

  fprintf(bdf_fp, "COPYRIGHT \"http://bmf.php5.cz\"\n");

  fprintf(bdf_fp, "FONT_ASCENT %d\n", -sizeOver);
  fprintf(bdf_fp, "FONT_DESCENT %d\n", sizeUnder);
  
  fprintf(bdf_fp, "ENDPROPERTIES\n");
  fprintf(bdf_fp, "CHARS %d\n", asciiChars);
  
}

uint8_t get_pixel(int x, int y)
{
  int width, height;
  int intensity;
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

  if ( idx == 0 )
    return 0;
  
  intensity = (palette[(idx-1)*3]+palette[(idx-1)*3+1]+palette[(idx-1)*3+2])/3;
  if ( intensity >= optionGrayLow && intensity <= optionGrayHigh )
    return 1;
  return 0;
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
  int width, height, relx, rely;
  int dwidth;
  width = (uint8_t)tablo[0];    
  height = (uint8_t)tablo[1];
  relx = (int8_t)tablo[2];
  rely = (int8_t)tablo[3];
  
  dwidth = tablo[4]+addSpace;
  if ( optionForceProportional )
  {
    if ( width == 0 )
      dwidth = averageGlyphSize  + relx + addSpace;
    else
      dwidth = width + relx + addSpace;
  }
  dwidth+=optionAddExtraSpace;
  
  if ( bdf_fp == NULL )
    return;
  
  fprintf(bdf_fp, "STARTCHAR %u\n", encoding);
  fprintf(bdf_fp, "ENCODING %u\n", encoding);
  fprintf(bdf_fp, "DWIDTH %d 0\n", dwidth);
  //fprintf(bdf_fp, "BBX %d %d %d %d\n", width, height, relx, sizeOver+rely);
  fprintf(bdf_fp, "BBX %d %d %d %d\n", 
    width, height, relx, -(sizeOver+height+rely)+optionShiftY);
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

int processBMF(const char *filename, int isAnalyze)
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

  if ( isAnalyze == 0 )
  {
    write_bdf_header();
  }
  for (i = 0; i < asciiChars; i++) 
  {
    memset(bitmap, 0, BITMAP_SIZE);
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
    
    if ( isAnalyze )
    {
      if ( (whichChar >= 'A' && whichChar <= 'Z') || (whichChar >= 'a' && whichChar <= 'z') )
      {
        totalGlyphSize += w;
        cntGlyphSize++;
        averageGlyphSize = ((totalGlyphSize*2) / (cntGlyphSize*3));
      }
    }
    else
    {
      if ( optionUnicodeMapping )
      {
        write_bdf_bitmap(dos2unicode[whichChar]);
      }
      else
      {
        write_bdf_bitmap(whichChar);
      }
      //write_bdf_bitmap(whichChar);
    }
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
  if ( !isAnalyze )
  {
    fprintf(bdf_fp, "ENDFONT\n");
  }
  return 1;
}

void initDos2Unicode(void)
{
  dos2unicode[0x00]=0x0000; //NULL
  dos2unicode[0x01]=0x263A; //START OF HEADING
  dos2unicode[0x02]=0x263B; //START OF TEXT
  dos2unicode[0x03]=0x2665; //END OF TEXT
  dos2unicode[0x04]=0x2666; //END OF TRANSMISSION
  dos2unicode[0x05]=0x2663; //ENQUIRY
  dos2unicode[0x06]=0x2660; //ACKNOWLEDGE
  dos2unicode[0x07]=0x2022; //BELL
  dos2unicode[0x08]=0x25D8; //BACKSPACE
  dos2unicode[0x09]=0x25CB; //HORIZONTAL TABULATION
  dos2unicode[0x0a]=0x25D9; //LINE FEED
  dos2unicode[0x0b]=0x2642; //VERTICAL TABULATION
  dos2unicode[0x0c]=0x2640; //FORM FEED
  dos2unicode[0x0d]=0x266A; //CARRIAGE RETURN
  dos2unicode[0x0e]=0x266B; //SHIFT OUT
  dos2unicode[0x0f]=0x263C; //SHIFT IN
  dos2unicode[0x10]=0x25BA; //DATA LINK ESCAPE
  dos2unicode[0x11]=0x25C4; //DEVICE CONTROL ONE
  dos2unicode[0x12]=0x2195; //DEVICE CONTROL TWO
  dos2unicode[0x13]=0x203C; //DEVICE CONTROL THREE
  dos2unicode[0x14]=0x00B6; //DEVICE CONTROL FOUR
  dos2unicode[0x15]=0x00A7; //NEGATIVE ACKNOWLEDGE
  dos2unicode[0x16]=0x25AC; //SYNCHRONOUS IDLE
  dos2unicode[0x17]=0x21A8; //END OF TRANSMISSION BLOCK
  dos2unicode[0x18]=0x2191; //CANCEL
  dos2unicode[0x19]=0x2192; //END OF MEDIUM
  dos2unicode[0x1a]=0x2190; //SUBSTITUTE
  dos2unicode[0x1b]=0x221F; //ESCAPE
  dos2unicode[0x1c]=0x2194; //FILE SEPARATOR
  dos2unicode[0x1d]=0x25B2; //GROUP SEPARATOR
  dos2unicode[0x1e]=0x25BC; //RECORD SEPARATOR
  dos2unicode[0x1f]=0x001f; //UNIT SEPARATOR
  dos2unicode[0x20]=0x0020; //SPACE
  dos2unicode[0x21]=0x0021; //EXCLAMATION MARK
  dos2unicode[0x22]=0x0022; //QUOTATION MARK
  dos2unicode[0x23]=0x0023; //NUMBER SIGN
  dos2unicode[0x24]=0x0024; //DOLLAR SIGN
  dos2unicode[0x25]=0x0025; //PERCENT SIGN
  dos2unicode[0x26]=0x0026; //AMPERSAND
  dos2unicode[0x27]=0x0027; //APOSTROPHE
  dos2unicode[0x28]=0x0028; //LEFT PARENTHESIS
  dos2unicode[0x29]=0x0029; //RIGHT PARENTHESIS
  dos2unicode[0x2a]=0x002a; //ASTERISK
  dos2unicode[0x2b]=0x002b; //PLUS SIGN
  dos2unicode[0x2c]=0x002c; //COMMA
  dos2unicode[0x2d]=0x002d; //HYPHEN-MINUS
  dos2unicode[0x2e]=0x002e; //FULL STOP
  dos2unicode[0x2f]=0x002f; //SOLIDUS
  dos2unicode[0x30]=0x0030; //DIGIT ZERO
  dos2unicode[0x31]=0x0031; //DIGIT ONE
  dos2unicode[0x32]=0x0032; //DIGIT TWO
  dos2unicode[0x33]=0x0033; //DIGIT THREE
  dos2unicode[0x34]=0x0034; //DIGIT FOUR
  dos2unicode[0x35]=0x0035; //DIGIT FIVE
  dos2unicode[0x36]=0x0036; //DIGIT SIX
  dos2unicode[0x37]=0x0037; //DIGIT SEVEN
  dos2unicode[0x38]=0x0038; //DIGIT EIGHT
  dos2unicode[0x39]=0x0039; //DIGIT NINE
  dos2unicode[0x3a]=0x003a; //COLON
  dos2unicode[0x3b]=0x003b; //SEMICOLON
  dos2unicode[0x3c]=0x003c; //LESS-THAN SIGN
  dos2unicode[0x3d]=0x003d; //EQUALS SIGN
  dos2unicode[0x3e]=0x003e; //GREATER-THAN SIGN
  dos2unicode[0x3f]=0x003f; //QUESTION MARK
  dos2unicode[0x40]=0x0040; //COMMERCIAL AT
  dos2unicode[0x41]=0x0041; //LATIN CAPITAL LETTER A
  dos2unicode[0x42]=0x0042; //LATIN CAPITAL LETTER B
  dos2unicode[0x43]=0x0043; //LATIN CAPITAL LETTER C
  dos2unicode[0x44]=0x0044; //LATIN CAPITAL LETTER D
  dos2unicode[0x45]=0x0045; //LATIN CAPITAL LETTER E
  dos2unicode[0x46]=0x0046; //LATIN CAPITAL LETTER F
  dos2unicode[0x47]=0x0047; //LATIN CAPITAL LETTER G
  dos2unicode[0x48]=0x0048; //LATIN CAPITAL LETTER H
  dos2unicode[0x49]=0x0049; //LATIN CAPITAL LETTER I
  dos2unicode[0x4a]=0x004a; //LATIN CAPITAL LETTER J
  dos2unicode[0x4b]=0x004b; //LATIN CAPITAL LETTER K
  dos2unicode[0x4c]=0x004c; //LATIN CAPITAL LETTER L
  dos2unicode[0x4d]=0x004d; //LATIN CAPITAL LETTER M
  dos2unicode[0x4e]=0x004e; //LATIN CAPITAL LETTER N
  dos2unicode[0x4f]=0x004f; //LATIN CAPITAL LETTER O
  dos2unicode[0x50]=0x0050; //LATIN CAPITAL LETTER P
  dos2unicode[0x51]=0x0051; //LATIN CAPITAL LETTER Q
  dos2unicode[0x52]=0x0052; //LATIN CAPITAL LETTER R
  dos2unicode[0x53]=0x0053; //LATIN CAPITAL LETTER S
  dos2unicode[0x54]=0x0054; //LATIN CAPITAL LETTER T
  dos2unicode[0x55]=0x0055; //LATIN CAPITAL LETTER U
  dos2unicode[0x56]=0x0056; //LATIN CAPITAL LETTER V
  dos2unicode[0x57]=0x0057; //LATIN CAPITAL LETTER W
  dos2unicode[0x58]=0x0058; //LATIN CAPITAL LETTER X
  dos2unicode[0x59]=0x0059; //LATIN CAPITAL LETTER Y
  dos2unicode[0x5a]=0x005a; //LATIN CAPITAL LETTER Z
  dos2unicode[0x5b]=0x005b; //LEFT SQUARE BRACKET
  dos2unicode[0x5c]=0x005c; //REVERSE SOLIDUS
  dos2unicode[0x5d]=0x005d; //RIGHT SQUARE BRACKET
  dos2unicode[0x5e]=0x005e; //CIRCUMFLEX ACCENT
  dos2unicode[0x5f]=0x005f; //LOW LINE
  dos2unicode[0x60]=0x0060; //GRAVE ACCENT
  dos2unicode[0x61]=0x0061; //LATIN SMALL LETTER A
  dos2unicode[0x62]=0x0062; //LATIN SMALL LETTER B
  dos2unicode[0x63]=0x0063; //LATIN SMALL LETTER C
  dos2unicode[0x64]=0x0064; //LATIN SMALL LETTER D
  dos2unicode[0x65]=0x0065; //LATIN SMALL LETTER E
  dos2unicode[0x66]=0x0066; //LATIN SMALL LETTER F
  dos2unicode[0x67]=0x0067; //LATIN SMALL LETTER G
  dos2unicode[0x68]=0x0068; //LATIN SMALL LETTER H
  dos2unicode[0x69]=0x0069; //LATIN SMALL LETTER I
  dos2unicode[0x6a]=0x006a; //LATIN SMALL LETTER J
  dos2unicode[0x6b]=0x006b; //LATIN SMALL LETTER K
  dos2unicode[0x6c]=0x006c; //LATIN SMALL LETTER L
  dos2unicode[0x6d]=0x006d; //LATIN SMALL LETTER M
  dos2unicode[0x6e]=0x006e; //LATIN SMALL LETTER N
  dos2unicode[0x6f]=0x006f; //LATIN SMALL LETTER O
  dos2unicode[0x70]=0x0070; //LATIN SMALL LETTER P
  dos2unicode[0x71]=0x0071; //LATIN SMALL LETTER Q
  dos2unicode[0x72]=0x0072; //LATIN SMALL LETTER R
  dos2unicode[0x73]=0x0073; //LATIN SMALL LETTER S
  dos2unicode[0x74]=0x0074; //LATIN SMALL LETTER T
  dos2unicode[0x75]=0x0075; //LATIN SMALL LETTER U
  dos2unicode[0x76]=0x0076; //LATIN SMALL LETTER V
  dos2unicode[0x77]=0x0077; //LATIN SMALL LETTER W
  dos2unicode[0x78]=0x0078; //LATIN SMALL LETTER X
  dos2unicode[0x79]=0x0079; //LATIN SMALL LETTER Y
  dos2unicode[0x7a]=0x007a; //LATIN SMALL LETTER Z
  dos2unicode[0x7b]=0x007b; //LEFT CURLY BRACKET
  dos2unicode[0x7c]=0x007c; //VERTICAL LINE
  dos2unicode[0x7d]=0x007d; //RIGHT CURLY BRACKET
  dos2unicode[0x7e]=0x007e; //TILDE
  dos2unicode[0x7f]=0x007f; //DELETE
  dos2unicode[0x80]=0x00c7; //LATIN CAPITAL LETTER C WITH CEDILLA
  dos2unicode[0x81]=0x00fc; //LATIN SMALL LETTER U WITH DIAERESIS
  dos2unicode[0x82]=0x00e9; //LATIN SMALL LETTER E WITH ACUTE
  dos2unicode[0x83]=0x00e2; //LATIN SMALL LETTER A WITH CIRCUMFLEX
  dos2unicode[0x84]=0x00e4; //LATIN SMALL LETTER A WITH DIAERESIS
  dos2unicode[0x85]=0x00e0; //LATIN SMALL LETTER A WITH GRAVE
  dos2unicode[0x86]=0x00e5; //LATIN SMALL LETTER A WITH RING ABOVE
  dos2unicode[0x87]=0x00e7; //LATIN SMALL LETTER C WITH CEDILLA
  dos2unicode[0x88]=0x00ea; //LATIN SMALL LETTER E WITH CIRCUMFLEX
  dos2unicode[0x89]=0x00eb; //LATIN SMALL LETTER E WITH DIAERESIS
  dos2unicode[0x8a]=0x00e8; //LATIN SMALL LETTER E WITH GRAVE
  dos2unicode[0x8b]=0x00ef; //LATIN SMALL LETTER I WITH DIAERESIS
  dos2unicode[0x8c]=0x00ee; //LATIN SMALL LETTER I WITH CIRCUMFLEX
  dos2unicode[0x8d]=0x00ec; //LATIN SMALL LETTER I WITH GRAVE
  dos2unicode[0x8e]=0x00c4; //LATIN CAPITAL LETTER A WITH DIAERESIS
  dos2unicode[0x8f]=0x00c5; //LATIN CAPITAL LETTER A WITH RING ABOVE
  dos2unicode[0x90]=0x00c9; //LATIN CAPITAL LETTER E WITH ACUTE
  dos2unicode[0x91]=0x00e6; //LATIN SMALL LIGATURE AE
  dos2unicode[0x92]=0x00c6; //LATIN CAPITAL LIGATURE AE
  dos2unicode[0x93]=0x00f4; //LATIN SMALL LETTER O WITH CIRCUMFLEX
  dos2unicode[0x94]=0x00f6; //LATIN SMALL LETTER O WITH DIAERESIS
  dos2unicode[0x95]=0x00f2; //LATIN SMALL LETTER O WITH GRAVE
  dos2unicode[0x96]=0x00fb; //LATIN SMALL LETTER U WITH CIRCUMFLEX
  dos2unicode[0x97]=0x00f9; //LATIN SMALL LETTER U WITH GRAVE
  dos2unicode[0x98]=0x00ff; //LATIN SMALL LETTER Y WITH DIAERESIS
  dos2unicode[0x99]=0x00d6; //LATIN CAPITAL LETTER O WITH DIAERESIS
  dos2unicode[0x9a]=0x00dc; //LATIN CAPITAL LETTER U WITH DIAERESIS
  dos2unicode[0x9b]=0x00a2; //CENT SIGN
  dos2unicode[0x9c]=0x00a3; //POUND SIGN
  dos2unicode[0x9d]=0x00a5; //YEN SIGN
  dos2unicode[0x9e]=0x20a7; //PESETA SIGN
  dos2unicode[0x9f]=0x0192; //LATIN SMALL LETTER F WITH HOOK
  dos2unicode[0xa0]=0x00e1; //LATIN SMALL LETTER A WITH ACUTE
  dos2unicode[0xa1]=0x00ed; //LATIN SMALL LETTER I WITH ACUTE
  dos2unicode[0xa2]=0x00f3; //LATIN SMALL LETTER O WITH ACUTE
  dos2unicode[0xa3]=0x00fa; //LATIN SMALL LETTER U WITH ACUTE
  dos2unicode[0xa4]=0x00f1; //LATIN SMALL LETTER N WITH TILDE
  dos2unicode[0xa5]=0x00d1; //LATIN CAPITAL LETTER N WITH TILDE
  dos2unicode[0xa6]=0x00aa; //FEMININE ORDINAL INDICATOR
  dos2unicode[0xa7]=0x00ba; //MASCULINE ORDINAL INDICATOR
  dos2unicode[0xa8]=0x00bf; //INVERTED QUESTION MARK
  dos2unicode[0xa9]=0x2310; //REVERSED NOT SIGN
  dos2unicode[0xaa]=0x00ac; //NOT SIGN
  dos2unicode[0xab]=0x00bd; //VULGAR FRACTION ONE HALF
  dos2unicode[0xac]=0x00bc; //VULGAR FRACTION ONE QUARTER
  dos2unicode[0xad]=0x00a1; //INVERTED EXCLAMATION MARK
  dos2unicode[0xae]=0x00ab; //LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
  dos2unicode[0xaf]=0x00bb; //RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
  dos2unicode[0xb0]=0x2591; //LIGHT SHADE
  dos2unicode[0xb1]=0x2592; //MEDIUM SHADE
  dos2unicode[0xb2]=0x2593; //DARK SHADE
  dos2unicode[0xb3]=0x2502; //BOX DRAWINGS LIGHT VERTICAL
  dos2unicode[0xb4]=0x2524; //BOX DRAWINGS LIGHT VERTICAL AND LEFT
  dos2unicode[0xb5]=0x2561; //BOX DRAWINGS VERTICAL SINGLE AND LEFT DOUBLE
  dos2unicode[0xb6]=0x2562; //BOX DRAWINGS VERTICAL DOUBLE AND LEFT SINGLE
  dos2unicode[0xb7]=0x2556; //BOX DRAWINGS DOWN DOUBLE AND LEFT SINGLE
  dos2unicode[0xb8]=0x2555; //BOX DRAWINGS DOWN SINGLE AND LEFT DOUBLE
  dos2unicode[0xb9]=0x2563; //BOX DRAWINGS DOUBLE VERTICAL AND LEFT
  dos2unicode[0xba]=0x2551; //BOX DRAWINGS DOUBLE VERTICAL
  dos2unicode[0xbb]=0x2557; //BOX DRAWINGS DOUBLE DOWN AND LEFT
  dos2unicode[0xbc]=0x255d; //BOX DRAWINGS DOUBLE UP AND LEFT
  dos2unicode[0xbd]=0x255c; //BOX DRAWINGS UP DOUBLE AND LEFT SINGLE
  dos2unicode[0xbe]=0x255b; //BOX DRAWINGS UP SINGLE AND LEFT DOUBLE
  dos2unicode[0xbf]=0x2510; //BOX DRAWINGS LIGHT DOWN AND LEFT
  dos2unicode[0xc0]=0x2514; //BOX DRAWINGS LIGHT UP AND RIGHT
  dos2unicode[0xc1]=0x2534; //BOX DRAWINGS LIGHT UP AND HORIZONTAL
  dos2unicode[0xc2]=0x252c; //BOX DRAWINGS LIGHT DOWN AND HORIZONTAL
  dos2unicode[0xc3]=0x251c; //BOX DRAWINGS LIGHT VERTICAL AND RIGHT
  dos2unicode[0xc4]=0x2500; //BOX DRAWINGS LIGHT HORIZONTAL
  dos2unicode[0xc5]=0x253c; //BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL
  dos2unicode[0xc6]=0x255e; //BOX DRAWINGS VERTICAL SINGLE AND RIGHT DOUBLE
  dos2unicode[0xc7]=0x255f; //BOX DRAWINGS VERTICAL DOUBLE AND RIGHT SINGLE
  dos2unicode[0xc8]=0x255a; //BOX DRAWINGS DOUBLE UP AND RIGHT
  dos2unicode[0xc9]=0x2554; //BOX DRAWINGS DOUBLE DOWN AND RIGHT
  dos2unicode[0xca]=0x2569; //BOX DRAWINGS DOUBLE UP AND HORIZONTAL
  dos2unicode[0xcb]=0x2566; //BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL
  dos2unicode[0xcc]=0x2560; //BOX DRAWINGS DOUBLE VERTICAL AND RIGHT
  dos2unicode[0xcd]=0x2550; //BOX DRAWINGS DOUBLE HORIZONTAL
  dos2unicode[0xce]=0x256c; //BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL
  dos2unicode[0xcf]=0x2567; //BOX DRAWINGS UP SINGLE AND HORIZONTAL DOUBLE
  dos2unicode[0xd0]=0x2568; //BOX DRAWINGS UP DOUBLE AND HORIZONTAL SINGLE
  dos2unicode[0xd1]=0x2564; //BOX DRAWINGS DOWN SINGLE AND HORIZONTAL DOUBLE
  dos2unicode[0xd2]=0x2565; //BOX DRAWINGS DOWN DOUBLE AND HORIZONTAL SINGLE
  dos2unicode[0xd3]=0x2559; //BOX DRAWINGS UP DOUBLE AND RIGHT SINGLE
  dos2unicode[0xd4]=0x2558; //BOX DRAWINGS UP SINGLE AND RIGHT DOUBLE
  dos2unicode[0xd5]=0x2552; //BOX DRAWINGS DOWN SINGLE AND RIGHT DOUBLE
  dos2unicode[0xd6]=0x2553; //BOX DRAWINGS DOWN DOUBLE AND RIGHT SINGLE
  dos2unicode[0xd7]=0x256b; //BOX DRAWINGS VERTICAL DOUBLE AND HORIZONTAL SINGLE
  dos2unicode[0xd8]=0x256a; //BOX DRAWINGS VERTICAL SINGLE AND HORIZONTAL DOUBLE
  dos2unicode[0xd9]=0x2518; //BOX DRAWINGS LIGHT UP AND LEFT
  dos2unicode[0xda]=0x250c; //BOX DRAWINGS LIGHT DOWN AND RIGHT
  dos2unicode[0xdb]=0x2588; //FULL BLOCK
  dos2unicode[0xdc]=0x2584; //LOWER HALF BLOCK
  dos2unicode[0xdd]=0x258c; //LEFT HALF BLOCK
  dos2unicode[0xde]=0x2590; //RIGHT HALF BLOCK
  dos2unicode[0xdf]=0x2580; //UPPER HALF BLOCK
  dos2unicode[0xe0]=0x03b1; //GREEK SMALL LETTER ALPHA
  dos2unicode[0xe1]=0x00df; //LATIN SMALL LETTER SHARP S
  dos2unicode[0xe2]=0x0393; //GREEK CAPITAL LETTER GAMMA
  dos2unicode[0xe3]=0x03c0; //GREEK SMALL LETTER PI
  dos2unicode[0xe4]=0x03a3; //GREEK CAPITAL LETTER SIGMA
  dos2unicode[0xe5]=0x03c3; //GREEK SMALL LETTER SIGMA
  dos2unicode[0xe6]=0x00b5; //MICRO SIGN
  dos2unicode[0xe7]=0x03c4; //GREEK SMALL LETTER TAU
  dos2unicode[0xe8]=0x03a6; //GREEK CAPITAL LETTER PHI
  dos2unicode[0xe9]=0x0398; //GREEK CAPITAL LETTER THETA
  dos2unicode[0xea]=0x03a9; //GREEK CAPITAL LETTER OMEGA
  dos2unicode[0xeb]=0x03b4; //GREEK SMALL LETTER DELTA
  dos2unicode[0xec]=0x221e; //INFINITY
  dos2unicode[0xed]=0x03c6; //GREEK SMALL LETTER PHI
  dos2unicode[0xee]=0x03b5; //GREEK SMALL LETTER EPSILON
  dos2unicode[0xef]=0x2229; //INTERSECTION
  dos2unicode[0xf0]=0x2261; //IDENTICAL TO
  dos2unicode[0xf1]=0x00b1; //PLUS-MINUS SIGN
  dos2unicode[0xf2]=0x2265; //GREATER-THAN OR EQUAL TO
  dos2unicode[0xf3]=0x2264; //LESS-THAN OR EQUAL TO
  dos2unicode[0xf4]=0x2320; //TOP HALF INTEGRAL
  dos2unicode[0xf5]=0x2321; //BOTTOM HALF INTEGRAL
  dos2unicode[0xf6]=0x00f7; //DIVISION SIGN
  dos2unicode[0xf7]=0x2248; //ALMOST EQUAL TO
  dos2unicode[0xf8]=0x00b0; //DEGREE SIGN
  dos2unicode[0xf9]=0x2219; //BULLET OPERATOR
  dos2unicode[0xfa]=0x00b7; //MIDDLE DOT
  dos2unicode[0xfb]=0x221a; //SQUARE ROOT
  dos2unicode[0xfc]=0x207f; //SUPERSCRIPT LATIN SMALL LETTER N
  dos2unicode[0xfd]=0x00b2; //SUPERSCRIPT TWO
  dos2unicode[0xfe]=0x25a0; //BLACK SQUARE
  dos2unicode[0xff]=0x00a0; //NO-BREAK SPACE

}

void help(void)
{
  puts("bmf2bdf [options] <file.bmf>");
  puts("Options:");
  puts(" -l #   Lower edge of the gray level range which maps bmf colors to black  (0..255, defaults to 0)");
  puts(" -h #   Upper edge of the gray level range which maps bmf colors to black  (0..255, defaults to 255)");
  puts(" -p     Ignore the BMF shift value and try to recalculate it (creates a proportional font)");
  puts(" -u     Apply DOS/ASCII to unicode translation");
  puts(" -x #   Add # pixel of extra space after each glyph (increases DWIDTH by one, defaults to 0)");
  puts(" -y #   Shift all glyphs by # pixel (# can be negative)");
  puts("Hints:");
  puts(" Usually '-u' is required for proper glyph encoding.");
  puts(" Use '-x 1' to increase the gap between chars.");
  puts(" Use '-l 127' to get an improved color mapping for multicolor .bmf fonts.");
  puts(" Use '-p' for monospaced .bmf fonts which seem to be proportional fonts.");
  puts("Example:");
  puts(" bmf2bdf -u -p -l 128 -x 1 font.bms > font.bdf");
}

int main(int argc, char **argv)
{
  char *bmf_name = NULL;
  bdf_fp = stdout;
  if ( argc < 2 )
  {
    help();
    return 0;
  }
  
  argv++;
  argc--;
  while( argv != NULL && argc > 0)
  {
    if ( (*argv)[0] != '-' )
    {
      bmf_name = *argv;
    }
    else if ( strcmp(*argv, "-p") == 0 )
    {
      optionForceProportional = 1;
    }
    else if ( strcmp(*argv, "-u") == 0 )
    {
      optionUnicodeMapping = 1;
    }
    else if ( strcmp(*argv, "-x") == 0 )
    {
      argv++;
      argc--;
      optionAddExtraSpace = atoi(*argv);
    }
    else if ( strcmp(*argv, "-y") == 0 )
    {
      argv++;
      argc--;
      optionShiftY = atoi(*argv);
    }
    else if ( strcmp(*argv, "-l") == 0 )
    {
      argv++;
      argc--;
      optionGrayLow = atoi(*argv);
    }
    else if ( strcmp(*argv, "-h") == 0 )
    {
      argv++;
      argc--;
      optionGrayHigh = atoi(*argv);
    }
    else
    {
      fprintf(stderr, "Wrong option %s\n", *argv);
      return 1;
    }
    argv++;
    argc--;
  }

  
  if ( bmf_name != NULL )
  {
    initDos2Unicode();
    processBMF(bmf_name, 1);
    processBMF(bmf_name, 0);
  }
  return 0;
}

