/*
  mapgen.c
  
  tile <ascii> <mapto> <top> <right> <bottom> <left>
  ":"<mapline>


    num := <hexnum> | <decnum> | <asciinum>
    asciinum := "'" <char>
    hexnum := "$" <hexdigit> { <hexdigit> }
    decnum := <decdigit> { <decdigit> }
    decdigit := "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
    hexdigit := "a" | "b" | "c" | "d" | "e" | "f" | "A" | "B" | "C" | "D" | "E" | "F" | <decdigit>


  The value 0 for "top", "right", "bottom" or "left" means match any.
  
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

#define MAP_SIZE_X 1024
#define MAP_SIZE_Y 1024
#define MAP_LINE_MAX 4096


uint8_t map[MAP_SIZE_Y][MAP_SIZE_X];
uint8_t map2[MAP_SIZE_Y][MAP_SIZE_X];
int map_curr_line = 0;
char map_name[MAP_LINE_MAX];
long map_width = 0;
long map_height = 0;


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

/* map a tile from map[][] to map2[][] */
/* called by map_all_tile */
int map_tile(int x, int y)
{
  int ascii, i, j;
  int cond[4];
  int is_condition_match;
  //int is_simple_match;
  int condition_match_cnt;
  int condition_match_max;
  int i_best;
  
  /* get the ascii version */
  ascii = map[y][x];
  cond[0] = 32;
  cond[1] = 32;
  cond[2] = 32;
  cond[3] = 32;
  
  if ( y > 0 ) cond[0] = map[y-1][x];
  if ( x+1 < map_width ) cond[1] = map[y][x+1];
  if ( y+1 < map_height ) cond[2] = map[y+1][x];
  if ( x > 0 ) cond[3] = map[y][x-1];
  
  /* find matching tile */
  condition_match_max = -1;
  i_best = -1;
  for( i = 0; i < tile_cnt; i++ )
  {
    if ( tile_list[i].ascii == ascii )
    {
      is_condition_match = 1;
      //is_simple_match = 1;
      condition_match_cnt = 0;
      for( j = 0; j < 4; j++ )
      {
	if ( tile_list[i].condition[j] != 0 )
	{
	  //is_simple_match = 0;
	  if ( tile_list[i].condition[j] != cond[j] )
	  {
	    is_condition_match = 0;
	  }
	  else
	  {
	    condition_match_cnt++;
	  }
	}
      }
      if ( is_condition_match )
      {
	if ( condition_match_max < condition_match_cnt )
	{
	  condition_match_max = condition_match_cnt;
	  i_best = i;
	}
      }
    }
  }
  if ( i_best < 0 )
  {
    printf("no tile mapping found for '%c' (x=%d, y=%d)\n", ascii, x, y);
    return 0;
  }
  //printf("tile mapping '%c' --> $%02x (x=%d, y=%d)\n", ascii, tile_list[i_best].map_to, x, y);
  map2[y][x] = tile_list[i_best].map_to;
  return 1;
}

int map_all_tiles(void)
{
  int x, y;
  for( y = 0; y < map_height; y++ )
    for( x = 0; x < map_width; x++ )
      if ( map_tile(x,y) == 0 )
	return 0;
  return 1;
}


void clear_map(void)
{
  int x, y;
  for( y = 0; y < MAP_SIZE_Y; y++ )
    for( x = 0; x < MAP_SIZE_X; x++ )
      map[y][x] =32;
  map_curr_line = 0;
}

void write_map(const char *filename)
{
  int x, y;
  FILE *fp;
  fp = fopen(filename, "w");
  for( y = 0; y < map_height; y++ )
  {
    fprintf(fp, "    \"");
    for( x = 0; x < map_width; x++ )
    {
      fprintf(fp, "\\x%02x", map2[y][x]);
    }
    fprintf(fp, "\"");
    if ( y+1 < map_height )
      fprintf(fp, ",");
    fprintf(fp, "\n");
  }
  fclose(fp);
}

int map_read_tile(const char **s)
{
  long ascii;
  int idx, i;
  ascii = get_num(s);
  if ( tile_cnt >= TILE_MAX )
  {
    printf("max number of tiles reached\n");
    return 0;
  }
  idx = tile_cnt;
  tile_list[idx].ascii = ascii;
  tile_cnt++;
    
  tile_list[idx].map_to = get_num(s);
  for( i = 0; i < 4; i++ )
  {
    tile_list[idx].condition[i] = get_num(s);
  }
  
  //printf("[%d] tile %c: ", idx, (int)ascii);
  //printf("map to $%02x\n", tile_list[idx].map_to);
    
  return 1;
}

int map_read_row(const char **s)
{
  int x = 0;
  //printf("line %d\n", map_curr_line);
  while ( **s >= ' ' )
  {
    if ( x > map_width )
    {
      printf("map '%s': Row '%d' too long\n", map_name, map_curr_line);
      return 0;
    }
    //printf("%d ", **s);
    map[map_curr_line][x] = **s;
    (*s)++;
    x++;
    
  }
  map_curr_line++;
  return 1;
}


int map_read_map_cmd(const char **s)
{
  
  /* get new map */
  strcpy(map_name, get_identifier(s));
  map_width = get_num(s);
  map_height = get_num(s);

  printf("map '%s' (%ld x %ld)\n", map_name, map_width, map_height);
  clear_map();
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
  
  if ( **s == ':' )
  {
    (*s)++;
    return map_read_row(s);
  }
  
  id = get_identifier(s);
  if ( strcmp(id, "tile") == 0 )
  {
     return map_read_tile(s);
  }
  else if ( strcmp(id, "map") == 0 )
  {
     return map_read_map_cmd(s);
  }
  else if ( strcmp(id, "endmap") == 0 )
  {
    /* write existing map */
    if ( map_width > 0 && map_height > 0 )
    {
      if ( map_all_tiles() )
	write_map("gm.c");
    }
    return 1;
  }
  else
  {
    printf("line %d: unkown command '%s'\n", map_curr_line, id);
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
  clear_map();
  map_read_filename("gm.map");
}
