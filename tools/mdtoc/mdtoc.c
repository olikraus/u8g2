
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define TOC_ENTRIES 512
#define TOC_ENTRY_SIZE 1024

char toc_start[] = "[tocstart]: # (toc start)";
char toc_end[] = "[tocend]: # (toc end)";

char toc[TOC_ENTRIES][TOC_ENTRY_SIZE];
int toc_level[TOC_ENTRIES];
int toc_cnt = 0;

void add_toc(int level, char *entry)
{
  strncpy( toc[toc_cnt], entry, TOC_ENTRY_SIZE);
  toc[toc_cnt][TOC_ENTRY_SIZE-1] = '\0';
  toc_level[toc_cnt] = level;
  toc_cnt++;
  printf("%d: %s\n", level, entry);
}

void read_md_fp(FILE *in, FILE *out)
{
  static char buf[TOC_ENTRY_SIZE];
  char *s;
  int is_inside_toc = 0;
  int is_start_of_file = 1;
  for(;;)
  {
    s = fgets(buf, TOC_ENTRY_SIZE, in);
    if ( s == NULL )
      return;
    if ( is_inside_toc != 0 )
    {
      if ( strcmp(s, toc_end) == 0 )
      {
	is_inside_toc = 0;
      }
    }
    else
    {
      if ( buf[0] == '\0' )
      {
	/* skip empty lines at the beginning of the file */
	if ( is_start_of_file == 0 )
	  fputs(buf, out);
      }
      else if ( strcmp(s, toc_start) == 0 )
      {
	is_start_of_file = 0;
	is_inside_toc = 1;
      }
      else if ( strncmp(s, "# ", 2) == 0 )
      {
	is_start_of_file = 0;
	add_toc(1, s+2);      
	fputs(buf, out);
      }
      else if ( strncmp(s, "## ", 3) == 0 )
      {
	is_start_of_file = 0;
	add_toc(2, s+3);
	fputs(buf, out);
      }
      else
      {
	is_start_of_file = 0;
	fputs(buf, out);
      }
    }
  }
}

void read_md(const char *in_name, const char *out_name)
{
  FILE *in;
  FILE *out;
  
  in = fopen(in_name, "r");
  if ( in == NULL )
  {
    perror(in_name);
    return;
  }
  
  out = fopen(out_name, "w");
  if ( out == NULL )
  {
    perror(out_name);
    return;
  }
  
  read_md_fp(in, out);
  
  fclose(in);
  fclose(out);
}

int main(int argc, char **argv)
{
  if ( argc <= 1 )
  {
    printf("%s [markdown files]\n", argv[0]);
    return 1;
  }
  argv++;
  while( *argv != NULL )
  {
    toc_cnt = 0;
    read_md(*argv, "tmp.md");
    argv++;
  }
}

