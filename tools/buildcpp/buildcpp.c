

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSVLINELEN (1024*16)


char csvline[CSVLINELEN];
char elements[10][CSVLINELEN];

#define DEVICE elements[0]
#define PINSET elements[1]
#define CONSTRUCTOR elements[2]
#define SETUP elements[3]


/*
  copy file from source_file_name to dest_file_name
*/
int file_copy(const char *source_file_name, const char *dest_file_name)
{
  int ch;
  FILE *source_fp;
  FILE *dest_fp;
  
  source_fp = fopen(source_file_name, "r");
  dest_fp = fopen(dest_file_name, "w");
  
  if ( source_fp == NULL || dest_fp == NULL )
    return 0;
  
  while( ( ch = fgetc(source_fp) ) != EOF )
    fputc(ch, dest_fp);
  
  fclose(source_fp);
  fclose(dest_fp);
  
  return 1;
}


/*
  Insert file "insertname" between lines "start_line" and "end_line" of file "filename"
*/
int insert_into_file(const char *filename, const char *insertname, const char *start_line, const char *end_line)
{
  int ch;
  static char line[1024*4];
  const char *tmpname = "tmp.h";
  FILE *source_fp;
  FILE *dest_fp;
  FILE *insert_fp;
  
  if ( file_copy(filename, tmpname) == 0 )
    return 0;

  source_fp = fopen(tmpname, "r");
  dest_fp = fopen(filename, "w");
  insert_fp = fopen(insertname, "r");

  if ( source_fp == NULL || dest_fp == NULL || insert_fp == NULL )
    return 0;
  
  for(;;)
  {
    if ( fgets(line, 1024*4, source_fp) == NULL )
      break;
    if ( strncmp(line, start_line, strlen(start_line)) == 0 )
    {
      fputs(line, dest_fp);

      while( ( ch = fgetc(insert_fp) ) != EOF )
	fputc(ch, dest_fp);
      
      fputs("\n", dest_fp);
      
      for(;;)
      {
	if ( fgets(line, 1024*4, source_fp) == NULL )
	  break;
	if ( strncmp(line, end_line, strlen(end_line)) == 0 )
	{
	  fputs(line, dest_fp);
	  break;
	}
      }
    }
    else
    {
      fputs(line, dest_fp);
    }
  }

  fclose(insert_fp);
  fclose(source_fp);
  fclose(dest_fp);
  
  unlink(tmpname);

  return 1;
}

/*
class U8X8_SSD1306_128X64_4W_SW_SPI : public U8X8
{
  public:
    U8X8_SSD1306_128X64_4W_SW_SPI(uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8X8()
    {
      u8x8_Setup_4Wire_SW_SPI(&u8x8, u8x8_d_ssd1306_128x64_noname, clock, data, cs, dc, reset);
    }
};
*/




void write_u8x8(const char *prefix, FILE *fp)
{
  fprintf(fp, "class %s_%s : public %s {\n", prefix, CONSTRUCTOR, prefix);
  fprintf(fp, "  public: %s_%s(", prefix, CONSTRUCTOR);
  if ( strcmp(PINSET, "4WSPI") == 0 )
    fprintf(fp, "uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE");
  else 
    fprintf(fp, "... unknown pinset ...");
  fprintf(fp, ") : %s() {\n", prefix);
  fprintf(fp, "    %s(getU8x8(), %s", SETUP, DEVICE);
  if ( strcmp(PINSET, "4WSPI") == 0 )
    fprintf(fp, ", clock, data, cs, dc, reset");
  else 
    fprintf(fp, "... unknown pinset ...");
  fprintf(fp, ");\n");

  
  fprintf(fp, "  }\n");
  fprintf(fp, "}\n");
  
}

void readcsvline(char *s)
{
  char *t;
  int i = 0;
  t = strtok(s, ",\n\r");
  while( t != NULL )
  {
    while( *t == ' ' || *t == '\t' )
      t++;
    //printf("%s\n", t);
    strcpy(elements[i], t);
    i++;
    if ( i >= 10 )
      break;
    t = strtok(NULL, ",\n\r");
  }
}


void readcsv(const char *name, const char *ctorname)
{
  FILE *fp;
  FILE *ctor_fp;
  char *s;
  fp = fopen(name, "r");
  ctor_fp = fopen(ctorname, "w");
  if ( fp != NULL )
  {
    for(;;)
    {
      s = fgets(csvline, CSVLINELEN, fp);
      if ( s == NULL )
	break;
      if ( s[0] == '\0' )
	continue;
      if ( s[0] == '\r' )
	continue;
      if ( s[0] == '\n' )
	continue;
      if ( s[0] == '#' )
	continue;
      readcsvline(s);
      write_u8x8("U8X8", ctor_fp);
    }
    fclose(fp);
    fclose(ctor_fp);
  }
  
}


int main(void)
{
  readcsv("display.csv", "ctor.h");
  
  
  insert_into_file("../../cppsrc/U8x8lib.h", "ctor.h", "// constructor list start", "// constructor list end");
  return 0;
}

