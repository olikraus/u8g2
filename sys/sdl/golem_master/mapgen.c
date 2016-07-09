/*
  mapgen.c
  
  tile <ascii> <mapto> <top> <right> <bottom> <left>
  map <mapline>


    num := <hexnum> | <decnum> | <asciinum>
    asciinum := "'" <char>
    hexnum := "$" <hexdigit> { <hexdigit> }
    decnum := <decdigit> { <decdigit> }
    decdigit := "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
    hexdigit := "a" | "b" | "c" | "d" | "e" | "f" | "A" | "B" | "C" | "D" | "E" | "F" | <decdigit>

  
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



struct tile_struct
{
  int ascii;
  int map_to;
  int condition[4];  
};
#define TILE_MAX 4096
struct tile_struct tile_list[TILE_MAX];

int tile_cnt = 0;


#define MAP_LINE_MAX 4096


FILE *map_fp;
char map_line[MAP_LINE_MAX];


static void skip_space(const char **s)
{
  for(;;)
  {
    if ( **s == '#' )
    {
      while( **s != '\0' )
	(*s)++;
      break;
    }
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

static long get_ascii(const char **s)
{
  long v = 0;
  v = **s;
  (*s)++;
  skip_space(s);
  return v;  
}


static long get_num(const char **s)
{
  if ( (**s) == '$' )
  {
    (*s)++;
    return get_hex(s);
  }
  if ( (**s) == '\'' )
  {
    (*s)++;
    return get_ascii(s);
  }
  
  return get_dec(s);
}

static const char *get_identifier(const char **s)
{
  static char buf[MAP_LINE_MAX];
  int c;
  int i = 0;
  buf[0] = '\0';
  for(;;)
  {
    c = **s;
    if ( c < 'A' )
      break;
    if ( i >= MAP_LINE_MAX-2 )
      break;
    buf[i++] = c;
    buf[i] = '\0';
    (*s)++;
  }
  skip_space(s);
  return buf;
}

/*============================================*/
int get_tile_idx_by_ascii(int ascii)
{
  int i;
  for( i = 0; i < tile_cnt; i++ )
  {
    if ( tile_list[i].ascii == ascii )
      return i;
  }
  return -1;
}



int map_read_tile(const char **s)
{
  long ascii;
  int idx, i;
  ascii = get_num(s);
  idx = get_tile_idx_by_ascii(ascii);
  if ( idx < 0 )
  {
    if ( tile_cnt >= TILE_MAX )
    {
      printf("max number of tiles reached\n");
      return 0;
    }
    idx = tile_cnt;
    tile_list[idx].ascii = ascii;
    tile_cnt++;
  }
  tile_list[idx].map_to = get_num(s);
  for( i = 0; i < 4; i++ )
  {
    tile_list[idx].condition[i] = get_num(s);
  }
  
  printf("tile %c: ", (int)ascii);
  printf("map to $%02x\n", tile_list[idx].map_to);
  
  
  return 1;
}


int map_read_line(const char **s)
{
  const char *id;
  skip_space(s);
  
  if ( **s == '#' )		/* comment (hmm handled by skip_space) */
    return 1;

  if ( **s == '\0' )		/* empty line */
    return 1;
  
  id = get_identifier(s);
  if ( strcmp(id, "tile") == 0 )
  {
     return map_read_tile(s);
  }
  else
  {
    printf("unkown command '%s'\n", id);
  }
  
  return 1;
}

int map_read_fp(void)
{
  const char *s;
  for(;;)
  {
    if ( fgets(map_line, MAP_LINE_MAX, map_fp) == NULL )
      break;
    s = &(map_line[0]);
    if ( map_read_line(&s) == 0 )
      return 0;
  }
  return 1;
}

int map_read_filename(const char *name)
{
  map_fp = fopen(name, "r");
  if ( map_fp == NULL )
    return 0;
  printf("file '%s'\n", name);
  if ( map_read_fp() == 0 )
    return fclose(map_fp), 0;
  fclose(map_fp);
  return 1;
}

int main(void)
{
  map_read_filename("gm.map");
}
