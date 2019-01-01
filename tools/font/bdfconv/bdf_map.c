/*
  bdf_map.c
  
  select and map glyphs
  
  Syntax:
    maplist := <mapcmd> { "," <mapcmd> }
    mapcmd := <default> | <maprange> | <exclude>
    default := "*"
    maprange := <range> [  ">" <num> ]
    exclude := "~" <range> 
    kern_exclude := "x" <range> 
    range := <num> [ "-" <num> ]
    num := <hexnum> | <decnum>
    hexnum := "$" <hexdigit> { <hexdigit> }
    decnum := <decdigit> { <decdigit> }
    decdigit := "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
    hexdigit := "a" | "b" | "c" | "d" | "e" | "f" | "A" | "B" | "C" | "D" | "E" | "F" | <decdigit>
    
    { }: zero, one ore more
    [ ]: zero or once
    |: alternative
  
*/

#include "bdf_font.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static long range_from;
static long range_to;
static int is_exclude;
static int is_kern_exclude;
static long map_to;

static void skip_space(const char **s)
{
  for(;;)
  {
    if ( **s == '\0' )
      break;
    if ( **s > ' ' )
      break;
    (*s)++;
  }
}

static long get_dec(const char **s)
{
  long v = 0;
  for(;;)
  {
    if ( (**s) >= '0' && (**s)  <= '9' )
    {
      v*=10;
      v+= (**s)-'0';
      (*s)++;
    }
    else
    {
      break;
    }
  }  
  skip_space(s);
  return v;
}

static long get_hex(const char **s)
{
  long v = 0;
  for(;;)
  {
    if ( (**s) >= '0' && (**s)  <= '9' )
    {
      v*=16;
      v+= (**s)-'0';
      (*s)++;
    }
    else if ( (**s) >= 'a' && (**s)  <= 'f' )
    {
      v*=16;
      v+= (**s)-'a'+10;
      (*s)++;
    }
    else if ( (**s) >= 'A' && (**s)  <= 'F' )
    {
      v*=16;
      v+= (**s)-'A'+10;
      (*s)++;
    }
    else
    {
      break;
    }
  }
  skip_space(s);
  return v;
}

static long get_num(const char **s)
{
  if ( (**s) != '$' )
    return get_dec(s);
  (*s)++;
  skip_space(s);
  return get_hex(s);
}

static long get_mul(const char **s)
{
  long v;
  v = get_num(s);
  while ( (**s) == '*' )
  {
    (*s)++;
    skip_space(s);
    v *= get_num(s);
  }
  return v;
}

static long get_add(const char **s)
{
  long v;
  v = get_mul(s);
  while ( (**s) == '+' )
  {
    (*s)++;
    skip_space(s);
    v += get_mul(s);
  }
  return v;
}

static long get_addsub(const char **s)
{
  long v;
  int op;
  v = get_mul(s);
  while ( (**s) == '+' || (**s) == '-' )
  {
    op = **s;
    (*s)++;
    skip_space(s);
    if ( op == '+' )
      v += get_mul(s);
    else
      v -= get_mul(s);      
  }
  return v;
}

static void get_range(const char **s)
{
  range_from = get_add(s);
  if ( **s != '-' )
  {
    range_to = range_from;
  }
  else
  {
    (*s)++;
    skip_space(s);
    range_to = get_add(s);
  }
}

static void map_cmd(const char **s)
{
  if ( **s == '*' )
  {
    range_from = 32;
    range_to = 255;
    map_to = 32;
    is_exclude = 0;
    is_kern_exclude = 0;
    
    (*s)++;
    skip_space(s);
  }
  else if ( **s == '~' )
  {
    is_exclude = 1;
    map_to = 0;	/*will be ignored */
    
    (*s)++;
    skip_space(s);
    get_range(s);
    
  }
  else if ( **s == 'x' )
  {
    is_kern_exclude = 1;
    map_to = 0;	/*will be ignored */
    
    (*s)++;
    skip_space(s);
    get_range(s);
  }
  else 
  {
    is_exclude = 0;
    get_range(s);
    map_to = range_from;
    if ( **s == '>')
    {
      (*s)++;
      skip_space(s);
      map_to = get_addsub(s);
    }
  }
}

void bf_map_cmd(bf_t *bf, const char **s)
{
  int i;
  bg_t *bg;
  static int is_log_disabled_for_single_glyphs = 0;
  
  if ( **s == ',' || **s == '\0' )
    return;
      
  map_cmd(s);

    
  if ( range_from != range_to )
  {
    bf_Log(bf, "Map: exclude=%d from=%ld/$%lx to=%ld/$%lx map=%ld/$%lx", is_exclude, range_from, range_from, range_to, range_to, map_to, map_to);
  }
  else if ( is_log_disabled_for_single_glyphs == 0 )
  {
    bf_Log(bf, "Map: exclude=%d from=%ld/$%lx to=%ld/$%lx map=%ld/$%lx (further single glyph logs disabled)", is_exclude, range_from, range_from, range_to, range_to, map_to, map_to);
    is_log_disabled_for_single_glyphs = 1;
  }
  
  
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    if ( bg->encoding >= range_from && bg->encoding <= range_to )
    {
      if ( is_kern_exclude != 0 )
      {
	      bg->is_excluded_from_kerning = 1;
      }
      else
      {
	      if ( is_exclude != 0 )
	      {
		bg->map_to = -1;
	      }
	      else
	      {
		bg->map_to = bg->encoding - range_from + map_to;
	      }
      }
    }
  }
  
}

void bf_map_list(bf_t *bf, const char **s)
{
  int i;
  bg_t *bg;
  
  /* exclude everything first */
  for( i = 0; i < bf->glyph_cnt; i++ )
  {
    bg = bf->glyph_list[i];
    bg->map_to = -1;
  }
  
  /* process the mapping list list */
  skip_space(s);
  for(;;)
  {
    bf_map_cmd(bf, s);
    if ( **s != ',' )
      break;
    (*s)++;
    skip_space(s);
  }
}

void bf_Map(bf_t *bf, const char *map_cmd_list)
{
  if ( strlen(map_cmd_list) < 1024 )
    bf_Log(bf, "Map: map_cmd_list='%s'", map_cmd_list);
  bf_map_list(bf, &map_cmd_list);
}

int bf_MapFile(bf_t *bf, const char *map_file_name)
{
  struct stat buf;
  char *s;
  FILE *fp;
  if ( map_file_name == NULL )
    return 1;
  if ( map_file_name[0] == '\0' )
    return 1;
  
  
  if ( stat(map_file_name, &buf) != 0 )
    return 0;
  fp = fopen(map_file_name, "r");
  if ( fp == NULL )
    return 0;
  s = malloc(buf.st_size+1);
  if ( s == NULL )
    return 0;
  fread(s, buf.st_size, 1, fp);
  s[buf.st_size] = '\0';
  fclose(fp);
  bf_Map(bf, s);
  free(s);
  return 1;
}

