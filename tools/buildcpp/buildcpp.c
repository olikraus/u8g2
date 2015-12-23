/*
  OBSOLETE,
  replaced by codebuild
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSVLINELEN (1024*16)


char csvline[CSVLINELEN];
char elements[10][CSVLINELEN];

#define DEVICE elements[0]
#define PINSET elements[1]
#define CONSTRUCTOR elements[2]


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

char *get_args(const char *ps)
{
  static char s[1024];
  if ( strcmp(ps, "4WSWSPI") == 0 )
    sprintf(s, "clock, data, cs, dc, reset");
  else if ( strcmp(ps, "4WHWSPI") == 0 )
    sprintf(s, "cs, dc, reset");    
  else if ( strcmp(ps, "3WSWSPI") == 0 )
    sprintf(s, "clock, data, cs, reset");
  else if ( strcmp(ps, "SSDSWI2C") == 0 )
    sprintf(s, "clock,  data,  reset");
  else if ( strcmp(ps, "6800") == 0 )
    sprintf(s, "d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset");
  else if ( strcmp(ps, "8080") == 0 )
    sprintf(s, "d0, d1, d2, d3, d4, d5, d6, d7, enable, cs, dc, reset");
  else 
    sprintf(s, "... unknown pinset ...");
  return s;
}

void write_u8x8_md(FILE *fp)
{
  fprintf(fp, "| U8X8_%s(", CONSTRUCTOR);  
  fprintf(fp, "%s) |\n", get_args(PINSET));  
}


void write_class(const char *prefix, FILE *fp)
{
  fprintf(fp, "class %s_%s : public %s {\n", prefix, CONSTRUCTOR, prefix);
  fprintf(fp, "  public: %s_%s(", prefix, CONSTRUCTOR);
  
  if ( strcmp(PINSET, "4WSWSPI") == 0 )
    fprintf(fp, "uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE");
  else if ( strcmp(PINSET, "4WHWSPI") == 0 )
    fprintf(fp, "uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE");
  else if ( strcmp(PINSET, "3WSWSPI") == 0 )
    fprintf(fp, "uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE");
  else if ( strcmp(PINSET, "SSDSWI2C") == 0 )
    fprintf(fp, "uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE");
  else if ( strcmp(PINSET, "6800") == 0 )
    fprintf(fp, "uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE");
  else if ( strcmp(PINSET, "8080") == 0 )
    fprintf(fp, "uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE");
  else 
    fprintf(fp, "... unknown pinset ...");
  
  
  
  fprintf(fp, ") : %s() {\n", prefix);
  fprintf(fp, "    ");
  
  if ( strcmp(PINSET, "4WSWSPI") == 0 )
    fprintf(fp, "u8x8_Setup_4Wire_SW_SPI");
  else if ( strcmp(PINSET, "4WHWSPI") == 0 )
    fprintf(fp, "u8x8_Setup_4Wire_HW_SPI");
  else if ( strcmp(PINSET, "3WSWSPI") == 0 )
    fprintf(fp, "u8x8_Setup_3Wire_SW_SPI");
  else if ( strcmp(PINSET, "SSDSWI2C") == 0 )
    fprintf(fp, "u8x8_Setup_SSD13xx_SW_I2C");
  else if ( strcmp(PINSET, "6800") == 0 )
    fprintf(fp, "u8x8_Setup_8Bit_6800");
  else if ( strcmp(PINSET, "8080") == 0 )
    fprintf(fp, "u8x8_Setup_8Bit_8080");
  else   
    fprintf(fp, "... unknown pinset ...");
  
  
  fprintf(fp, "(getU8x8(), %s", DEVICE);

  fprintf(fp, ", %s", get_args(PINSET));

    /*
  if ( strcmp(PINSET, "4WSWSPI") == 0 )
    fprintf(fp, ", clock, data, cs, dc, reset");
  else if ( strcmp(PINSET, "4WHWSPI") == 0 )
    fprintf(fp, ", cs, dc, reset");    
  else if ( strcmp(PINSET, "3WSWSPI") == 0 )
    fprintf(fp, ", clock, data, cs, reset");
  else if ( strcmp(PINSET, "SSDSWI2C") == 0 )
    fprintf(fp, ", clock,  data,  reset");
  else if ( strcmp(PINSET, "6800") == 0 )
    fprintf(fp, ", d0,  d1,  d2,  d3,  d4,  d5,  d6,  d7,  enable,  cs,  dc,  reset");
  else if ( strcmp(PINSET, "8080") == 0 )
    fprintf(fp, ", d0,  d1,  d2,  d3,  d4,  d5,  d6,  d7,  enable,  cs,  dc,  reset");
  else 
    fprintf(fp, "... unknown pinset ...");
    */
  
  fprintf(fp, ");\n");

  
  fprintf(fp, "  }\n");
  fprintf(fp, "};\n");
  
}

void readcsv_createu8x8line(char *s)
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

void readcsv_createu8x8md(const char *name, const char *mdname)
{
  FILE *fp;
  FILE *md_fp;
  char *s;
  fp = fopen(name, "r");
  md_fp = fopen(mdname, "w");
  if ( md_fp != NULL )
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
      readcsv_createu8x8line(s);
      write_u8x8_md(md_fp);
    }
    fclose(fp);
    fclose(md_fp);
  }
  
}


void readcsv_create(const char *prefix, const char *name, const char *ctorname)
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
      readcsv_createu8x8line(s);
      write_class(prefix, ctor_fp);
    }
    fclose(fp);
    fclose(ctor_fp);
  }
  
}


int main(void)
{
  readcsv_create("U8X8", "display.csv", "ctor.h");
  
  //insert_into_file("../../cppsrc/U8x8lib.h", "ctor.h", "// constructor list start", "// constructor list end");
  
  readcsv_create("U8G2", "display.csv", "ctor.h");
  readcsv_createu8x8md("display.csv", "md");
  
  
  return 0;
}

