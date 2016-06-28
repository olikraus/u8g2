
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define STR_MAX_LEN 1024


struct _pbm_struct
{
  
  uint16_t w;
  uint16_t h;
  uint16_t width_in_bytes;
  uint8_t *bitmap;
  char line[STR_MAX_LEN];
  uint8_t is_ascii;
  FILE *fp;
};
typedef struct _pbm_struct pbm_t;


void pbm_SetPixel(pbm_t *pbm, uint16_t x, uint16_t y, uint8_t value)
{
  uint8_t *p;
  uint8_t mask;
  p = pbm->bitmap;
  p += pbm->width_in_bytes * y;
  p += x/8;
  mask = 1<<(x&7);

  if ( value == 0 )
    *p &= ~mask;
  else
    *p |= mask;
}

uint8_t pbm_GetPixel(pbm_t *pbm, uint16_t x, uint16_t y)
{
  uint8_t *p;
  uint8_t mask;
  p = pbm->bitmap;
  p += pbm->width_in_bytes * y;
  p += x/8;
  mask = 1<<(x&7);

  if (  (*p & mask) != 0 )
    return 1;
  return 0;
}

/*=================================================*/
void skip_space(const char **s)
{
  for(;;)
  {
    if ( **s == '\0' )
      return;
    if ( **s > 32 )
      return;
    (*s)++;
  }
}

int get_int(const char **s) 
{
  int x = 0;
  for(;;)
  {
    if ( **s < '0' || **s > '9' )
      break;
    x = x*10 + (((int)**s) - '0' );
    (*s)++;    
  }
  skip_space(s);
  return x;
}

const char *get_identifier(const char **s) 
{
  static char buf[STR_MAX_LEN];
  int i = 0;
  for(;;)
  {
    if ( **s <=' '  )
      break;
    if ( i+2 < STR_MAX_LEN )
    {
      buf[i] = **s;
      i++;
    }
    (*s)++;    
  }
  buf[i] = '\0';
  skip_space(s);
  return buf;
}

/*=================================================*/

int pbm_ReadLine(pbm_t *pbm)
{
  if ( fgets(pbm->line, STR_MAX_LEN, pbm->fp) == NULL )
    return 0;
  return 1;
}

int pbm_ReadFP(pbm_t *pbm)
{
  const char *s = pbm->line;
  if ( pbm_ReadLine(pbm) == 0 )
    return 0;
  
  if ( strncmp(pbm->line, "P1", 2) == 0 )
    pbm->is_ascii = 1;
  else if ( strncmp(pbm->line, "P4", 2) == 0 )
    pbm->is_ascii = 0;
  else
    return 0;
  
  for(;;)
  {
    if ( pbm_ReadLine(pbm) == 0 )
      return 0;
    if ( pbm->line[0] != '#' )
      break;
  }

  skip_space(&s);
  pbm->w = get_int(&s);
  pbm->h = get_int(&s);
  
  assert( pbm->w < 10000);
  assert( pbm->h < 10000);

  pbm->width_in_bytes = (pbm->w+7)/8;
  pbm->bitmap = (uint8_t *)malloc(pbm->width_in_bytes * pbm->h);
  assert( pbm->bitmap != NULL );
  
  
  if ( pbm->is_ascii != 0 )
  {
    uint16_t x,y;
    int c;
    x = 0;
    y = 0;
    for(;;)
    {
      c = fgetc(pbm->fp);
      if ( c == EOF )
	break;
      if ( c == '1' || c == '0' )
      {
	pbm_SetPixel(pbm, x, y, c == '1');
	x++;
	if ( x >= pbm->w )
	{
	  y++;
	  x = 0;
	}
      }
    }
  }
  return 1;
}

uint8_t pbm_GetByte(pbm_t *pbm, uint16_t x, uint16_t y, uint8_t cnt)
{
  uint8_t b;
  uint8_t i;
  
  b = 0;
  for( i = 0; i < cnt; i++ )
  {
    if ( pbm_GetPixel(pbm, x+i, y) != 0 )
    {
      b |= 1<<(7-i);
    }
  }
  return b;
}

uint8_t *pbm_GetBytes(pbm_t *pbm, uint16_t x, uint16_t y, uint16_t cnt)
{
  uint8_t i;
  static uint8_t buf[1024];
  i = 0;
  while( cnt > 8 )
  {
    buf[i]  = pbm_GetByte(pbm, x, y, 8);
    x+=8;
    i++;
    cnt -= 8;
  }
  buf[i]  = pbm_GetByte(pbm, x, y, cnt);
  return buf;
}

void pbm_WriteHexBytes(pbm_t *pbm, uint16_t x, uint16_t y, uint16_t cnt, FILE *fp)
{
  uint8_t i;
  uint8_t *p;
  p = pbm_GetBytes(pbm, x, y, cnt);
  cnt = (cnt+7)/8;
  for( i = 0; i < cnt; i++)
    fprintf(fp, "%02x", p[i]);
  fprintf(fp, "\n");
}

void pbm_WriteHexBitmap(pbm_t *pbm, uint16_t x, uint16_t y, uint16_t w, uint16_t h, FILE *fp)
{
  uint16_t i;
  for( i = 0; i < h; i++ )
  {
    pbm_WriteHexBytes(pbm, x, y+i, w, fp);
  }  
}

void pbm_WriteGlyph(pbm_t *pbm, uint16_t e, uint16_t x, uint16_t y, uint16_t w, uint16_t h, FILE *fp)
{
  fprintf(fp, "STARTCHAR %u\n", e);
  fprintf(fp, "ENCODING %u\n", e);
  fprintf(fp, "SWIDTH %u 0\n", w*72);		// ???
  fprintf(fp, "DWIDTH %u 0\n", w);
  fprintf(fp, "BBX %u %u 0 0\n", w, h);
  fprintf(fp, "BITMAP\n");
  pbm_WriteHexBitmap(pbm, x, y, w, h, fp);
  fprintf(fp, "ENDCHAR\n");
}

void pbm_WriteFontStart(FILE *fp, const char *s)
{
  fprintf(fp, "STARTFONT 2.1\n");
  fprintf(fp, "FONT %s\n", s);
  fprintf(fp, "SIZE 16 75 75\n");
  fprintf(fp, "FONTBOUNDINGBOX 16 16 0 0\n");
	
  fprintf(fp, "STARTPROPERTIES 3\n");
  fprintf(fp, "COPYRIGHT \"CC-BY-3.0\"\n");
  fprintf(fp, "FONT_ASCENT 0\n");
  fprintf(fp, "FONT_DESCENT 0\n");
  fprintf(fp, "ENDPROPERTIES\n");
  fprintf(fp, "CHARS 235\n");		// hard coded :-(
	
}

void pbm_WriteFontEnd(FILE *fp)
{
  fprintf(fp, "ENDFONT\n");
}

void pbm_Show(pbm_t *pbm)
{
  uint16_t x, y;
  for( y = 0; y < pbm->h; y++ )
  {
    for( x = 0; x < pbm->w; x++ )
    {
      printf("%c", pbm_GetPixel(pbm, x, y) == 0 ? '0' : '1');
    }
    puts("");
    pbm_WriteHexBytes(pbm, 0, y, pbm->w, stdout);
  }
}





int pbm_ReadFilename(pbm_t *pbm, const char *name)
{
  pbm->fp = fopen(name, "r");
  pbm->is_ascii = 0;
  if ( pbm->fp == NULL )
    return 0;
  
  pbm_ReadFP(pbm);
  fclose(pbm->fp);
  
  return 1;
}



/*=================================================*/

void outline(pbm_t *pbm, FILE *fp, uint16_t e, uint16_t y, uint16_t cnt)
{
  uint16_t i;
  for(i = 0; i < cnt; i++ )
    pbm_WriteGlyph(pbm, i+e, 32+i*16, y, 16, 16, fp);
}

void out(pbm_t *pbm, FILE *fp, const char *s)
{
  uint16_t e;
  pbm_WriteFontStart(fp, s);
  outline(pbm, fp, 0x01, 320, 8);	/* The Unliving */
  outline(pbm, fp, 0x10, 496, 12);	/* Traps&Devices */
  outline(pbm, fp, 0x1c, 272, 4);	/* Trollkind */
  outline(pbm, fp, 0x20, 96-32, 1);	/* Space */
  outline(pbm, fp, 0x21, 784, 15);	/* Magic */
  
  e = 0x030;
  outline(pbm, fp, e, 96, 13);	/* Characters */
  e+= 13;
  outline(pbm, fp, e, 112, 13);	/* Characters */
  e+= 13;
  outline(pbm, fp, e, 128, 7);	/* Characters */
  e+= 7;
  outline(pbm, fp, e, 224, 13);	/* Fauna */
  e+= 13;
  outline(pbm, fp, e, 368, 8);	/* Creatures */
  e+= 8;
  outline(pbm, fp, e, 416, 16);	/* Building */
  e+= 16;
  outline(pbm, fp, e, 432, 7);	/* Building */
  e+= 7;
  outline(pbm, fp, e, 448, 12);	/* Building */
  e+= 12;
  outline(pbm, fp, e, 544, 13);	/* Outerworld */
  e+= 13;
  outline(pbm, fp, e, 592, 16);	/* Exploration */
  e+= 16;
  outline(pbm, fp, e, 608, 5);	/* Exploration */
  e+= 5;
  outline(pbm, fp, e, 656, 16);	/* Food&Drink */
  e+= 16;
  outline(pbm, fp, e, 672, 6);	/* Food&Drink */
  e+= 6;
  outline(pbm, fp, e, 720, 16);	/* Outfit */
  e+= 16;
  outline(pbm, fp, e, 736, 11);	/* Outfit */
  e+= 11;
  
  //outline(pbm, fp, 400, 832, 6);	/* Music */
  outline(pbm, fp, e, 880, 16);	/* Symbols */
  e+= 16;
  outline(pbm, fp, e, 896, 7);	/* Symbols */
  e+= 7;
  
  
  pbm_WriteFontEnd(fp);
  
  
}


int main(int argc, char **argv)
{
  pbm_t pbm;
  if ( argc <= 1 )
  {
    printf("%s <pbm ascii file>\n", argv[0]);
  }
  else
  {
    pbm_ReadFilename(&pbm, argv[1]);
    out(&pbm, stdout, argv[1]);
    //printf("Size: %d x %d\n", pbm.w, pbm.h);
    //pbm_Show(&pbm);
  }
  return 0;
}


