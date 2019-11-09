/*

  mdtoc.c
  
  Markdown TOC Generator

  LPC804 PLU Project
  
  Copyright (c) 2015, olikraus@gmail.com
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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



#define TOC_ENTRIES 512
#define TOC_ENTRY_SIZE 1024


char toc_start[] = "[tocstart]: # (toc start)";
char toc_end[] = "[tocend]: # (toc end)";

char toc[TOC_ENTRIES][TOC_ENTRY_SIZE];
int toc_level[TOC_ENTRIES];
int toc_cnt = 0;
int existing_toc_found = 0;

char *getentry(const char *s)
{
  static char buf[TOC_ENTRY_SIZE];
  char *t = buf;
  for(;;)
  {
    if ( *s == '\0' )
      break;
    if ( *s >= ' ' )
    {
      *t++ = *s++;
    }
    else
    {
      s++;
    }
  }
  *t = '\0';
  return buf;
}

char *getref(const char *s)
{
  static char buf[TOC_ENTRY_SIZE];
  char *t = buf;
  for(;;)
  {
    if ( *s == '\0' )
      break;
    if ( *s == ' ' )
    {
      *t++ = '-';
      s++;
    }
    else if ( *s >= '0' && *s <= '9' )
    {
      *t++ = *s++;
    }
    else if ( *s < 'A' )
    {
      s++;
    }
    else
    {
      *t = tolower(*s);
      t++;
      s++;
    }
  }
  *t = '\0';
  return buf;
}

void add_toc(int level, char *entry)
{
  if ( toc_cnt >= TOC_ENTRIES )
    return;
  strncpy( toc[toc_cnt], entry, TOC_ENTRY_SIZE);
  toc[toc_cnt][TOC_ENTRY_SIZE-1] = '\0';
  toc_level[toc_cnt] = level;
  toc_cnt++;
  // printf("%d: %s [%s](#%s)\n", level, entry, getentry(entry), getref(entry));
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
      if ( strncmp(s, toc_end, strlen(toc_end)) == 0 )
      {
	is_inside_toc = 0;
      }
    }
    else
    {
      if ( buf[0] == '\0' || buf[0] == '\n' )
      {
	/* skip empty lines at the beginning of the file */
	if ( is_start_of_file == 0 )
	  fputs(buf, out);
      }
      else if ( strncmp(s, toc_start, strlen(toc_start)) == 0 )
      {
	existing_toc_found = 1;
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

void copy_to_md_fp(FILE *tmp, FILE *out)
{
  static char buf[TOC_ENTRY_SIZE];
  char *s;
  int i, j;
  fprintf(out, "\n");
  fprintf(out, "%s\n", toc_start);
  fprintf(out, "\n");
  for( i = 0; i < toc_cnt; i++ )
  {
    for( j = 0; j < toc_level[i]; j++)
      fprintf(out, "  ");
    fprintf(out, "* [%s](#%s)\n", getentry(toc[i]), getref(toc[i]));
  }
  fprintf(out, "\n");
  fprintf(out, "%s\n", toc_end);
  fprintf(out, "\n");

  for(;;)
  {
    s = fgets(buf, TOC_ENTRY_SIZE, tmp);
    if ( s == NULL )
      return;
    fputs(buf, out);
  }
}

void copy_to_md(const char *tmp_name, const char *out_name)
{
  FILE *tmp;
  FILE *out;
  
  tmp = fopen(tmp_name, "r");
  if ( tmp == NULL )
  {
    perror(tmp_name);
    return;
  }
  
  out = fopen(out_name, "w");
  if ( out == NULL )
  {
    perror(out_name);
    return;
  }
  
  copy_to_md_fp(tmp, out);
  
  fclose(tmp);
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
    if ( strstr( *argv, "_Footer.md") == NULL &&
  	  strstr( *argv, "_Sidebar.md") == NULL )
    {
	
      toc_cnt = 0;
      existing_toc_found = 0;
      read_md(*argv, "tmp.md");
      if ( toc_cnt != 0 )
      {
	copy_to_md("tmp.md", *argv);
      }
      if ( toc_cnt == 0 )
      {
	printf("%s: nothing changed (no headings found)\n", *argv);
      }
      else
      {
	if ( existing_toc_found == 0 )
	{
	  if ( toc_cnt == 1 )
	  {
	    printf("%s: added %d toc entry\n", *argv, toc_cnt);
	  }
	  else
	  {
	    printf("%s: added %d toc entries\n", *argv, toc_cnt);
	  }
	}
	else
	{
	  if ( toc_cnt == 1 )
	  {
	    printf("%s: replaced %d toc entry\n", *argv, toc_cnt);
	  }
	  else
	  {
	    printf("%s: replaced %d toc entries\n", *argv, toc_cnt);
	  }
	}
      } /* no toc required */
    } /* skip system .md files */
    argv++;
  }
  return 0;
}

