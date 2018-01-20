
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "bdf_font.h"
#include "fd.h"

/*================================================*/

int get_str_arg(char ***argv, int c, char **result)
{
  if ( (**argv)[0] == '-' )
  {
    if ( (**argv)[1] == c )
    {
      if ( (**argv)[2] == '\0' )
      {
	(*argv)++;
	*result = **argv;
      }
      else
      {
	*result = (**argv)+2;
      }
      (*argv)++;
      return 1;      
    }
  }
  return 0;
}


int get_num_arg(char ***argv, int c, unsigned long *result)
{
  if ( (**argv)[0] == '-' )
  {
    if ( (**argv)[1] == c )
    {
      
      if ( (**argv)[2] == '\0' )
      {
	if ( *((*argv)+1) != NULL )
	{
	  (*argv)++;
	  *result = strtoul((**argv), NULL, 10);
	}
      }
      else
      {
	*result = strtoul((**argv)+2, NULL, 10);
      }
      (*argv)++;
      return 1;      
    }
  }
  return 0;
}

int is_arg(char ***argv, int c)
{
  if ( (**argv)[0] == '-' )
  {
    if ( (**argv)[1] == c )
    {
      (*argv)++;
      return 1;      
    }
  }
  return 0;
}

/*================================================*/
void help(void)
{
  printf("bdfconv [options] filename\n");
  printf("-h          Display this help\n");
  printf("-v          Print log messages\n");
  printf("-b <n>      Font build mode, 0: proportional, 1: common height, 2: monospace, 3: multiple of 8\n");
  printf("-f <n>      Font format, 0: ucglib font, 1: u8g2 font, 2: u8g2 uncompressed 8x8 font (enforces -b 3)\n");
  printf("-m 'map'    Unicode ASCII mapping\n");
  printf("-M 'mapfile'    Read Unicode ASCII mapping from file 'mapname'\n");
  printf("-o <file>   C output font file\n");
  printf("-k <file>   C output file with kerning information\n");	
  printf("-p <%%>    Minimum distance for kerning in percent of the global char width (lower values: Smaller gaps, more data)\n");	

  printf("-n <name>   C indentifier (font name)\n");
  printf("-d <file>   Overview picture: Enable generation of bdf.tga and assign BDF font <file> for description\n");
  printf("-l <margin> Overview picture: Set left margin\n");
  printf("-g <glyphs> Overview picture: Set glyphs per line (default: 16)\n");
  printf("-a          Overview picture: Additional font information (background, orange&blue dot)\n");
  printf("-t          Overview picture: Test string (Woven silk pyjamas exchanged for blue quartz.)\n");
  printf("-r          Runtime test\n");
  printf("\n");

  printf("map := <mapcmd> { \",\" <mapcmd> }\n");
  printf("mapcmd := <default> | <maprange> | <exclude> | <exclude-kerning>\n");
  printf("default := \"*\"\n");
  printf("maprange := <range> [  \">\" <addexpr> ]        Move specified glyph <range> to target code <num>\n");
  printf("exclude := \"~\" <range> \n");
  printf("exclude-kerning:= \"x\" <range> \n");
  printf("range := <addexpr> [ \"-\" <addexpr> ]          Select glyphs within specified range\n");
  printf("addexpr := <mulexpr> [ \"+\" <mulexpr> ]\n");
  printf("mulexpr := <num> [ \"*\" <num> ]\n");
  printf("num := <hexnum> | <decnum>\n");
  printf("hexnum := \"$\" <hexdigit> { <hexdigit> }\n");
  printf("decnum := <decdigit> { <decdigit> }\n");
  printf("decdigit := \"0\" | \"1\" | \"2\" | \"3\" | \"4\" | \"5\" | \"6\" | \"7\" | \"8\" | \"9\"\n");
  printf("hexdigit := \"a\" | \"b\" | \"c\" | \"d\" | \"e\" | \"f\" | \"A\" | \"B\" | \"C\" | \"D\" | \"E\" | \"F\" | <decdigit>\n");
    
  printf("{ } zero, one ore more, [ ] zero or once, | alternative\n");

  printf("example:\n");
  printf("  -m '32-255'     select gylphs from encoding 32 to 255\n");
  printf("  -m '32-255,~64' select gylphs from encoding 32 to 255, exclude '@'\n");
  printf("  -m '32,48-57'   select space, '1', '2', ... '9'\n");
  printf("build modes:\n");
  printf(" -b 0: Most compact, glyph bitmap is minimal\n");
  printf(" -b 1: Like -b 0, but glyph bitmap is extended to the height of the largest glyph within the selected glyph list.\n");
  printf("       Also the width of the gylphs is extended to cover the delta x advance.\n");
  printf(" -b 2: Like -b 1, but glyph width is set to the width of the largest glyph within the selected gylph list.\n");
  printf(" -b 3: Like -b 2, but width and height are forced to be a multiple of 8.\n");

}

/*================================================*/

unsigned long left_margin = 1;
unsigned long build_bbx_mode = 0;
unsigned long font_format = 0;
unsigned long min_distance_in_per_cent_of_char_width = 25;
unsigned long cmdline_glyphs_per_line = 16;
int font_picture_extra_info = 0;
int font_picture_test_string = 0;
int runtime_test = 0;
char *c_filename = NULL;
char *k_filename = NULL;
char *target_fontname = "bdf_font";

/*================================================*/

unsigned tga_get_line_height(bf_t *bf_desc_font, bf_t *bf)
{
  unsigned h;
  tga_set_font(bf_desc_font->target_data);
  h = tga_get_char_height();
  tga_set_font(bf->target_data);
  if ( h < tga_get_char_height() )
    return tga_get_char_height();
  return h;
}

unsigned tga_draw_font_line(unsigned y, long enc_start, bf_t *bf_desc_font, bf_t *bf, long glyphs_per_line)
{
  long i;
  unsigned x;
  int is_empty;
  char pre[32];
  
  is_empty = 1;
  for( i = 0; i< 16 && is_empty != 0; i++ )
  {
    if ( tga_get_glyph_data(i+enc_start) != NULL )
	is_empty = 0;
  }
  
  if ( is_empty != 0 )
    return 0;
  
  sprintf(pre, "%5ld/0x%04lx", enc_start, enc_start);
  
  x = left_margin;
  if ( bf_desc_font != NULL )
  {
    if ( bf_desc_font->target_data != NULL )
    {
      tga_set_font(bf_desc_font->target_data);
      x += tga_draw_string(x, y, pre, 0, 0);
    }
  }
  x += 4;
  
  tga_set_font(bf->target_data);
  for( i = 0; i< glyphs_per_line; i++ )
  {
    tga_draw_glyph(x + (tga_get_char_width()+2)*i,y,enc_start+i, font_picture_extra_info);
  }

  return left_margin + x + (tga_get_char_width()+2)*glyphs_per_line;
}

unsigned tga_draw_font_info(unsigned y, const char *fontname, bf_t *bf_desc_font, bf_t *bf)
{
  unsigned x;
  int cap_a, cap_a_height;
  static char s[256];
  
  cap_a_height = 0;
  cap_a = bf_GetIndexByEncoding(bf, 'A');
  if ( cap_a >= 0 )
  {
    cap_a_height = bf->glyph_list[cap_a]->bbx.h+bf->glyph_list[cap_a]->bbx.y;
  }
  
  if ( bf_desc_font != NULL )
  {
    if ( bf_desc_font->target_data != NULL )
    {
      
      tga_set_font(bf_desc_font->target_data);
      
      y +=  tga_get_char_height()+1;
      x = left_margin;
      x += tga_draw_string(x, y, fontname, 0, 0);

      y +=  tga_get_char_height()+1;      
      sprintf(s, "Max width %u, max height %u", tga_get_char_width(), tga_get_char_height());
      x = left_margin;
      x += tga_draw_string(x, y, s, 0, 0);

      y +=  tga_get_char_height()+1;      
      sprintf(s, "'A' height %d, font size %d ", cap_a_height, bf->target_cnt);
      x = left_margin;
      x += tga_draw_string(x, y, s, 0, 0);
      return (tga_get_char_height()+1)*3;
    }
  }
  return 0;
}


unsigned tga_draw_font(unsigned y, const char *fontname, bf_t *bf_desc_font, bf_t *bf, long glyphs_per_line)
{
  long i;
  unsigned x, xmax;
  xmax = 0;
  
  bf_Log(bf, "Draw TGA, line height %d", tga_get_line_height(bf_desc_font, bf));
  
  y += tga_draw_font_info( y, fontname, bf_desc_font, bf);
  
  y +=   tga_get_line_height(bf_desc_font, bf)+1;
  
  
  
  for( i = 0; i <= 0x0ffff; i+=glyphs_per_line )
  {
    x = tga_draw_font_line(y, i, bf_desc_font, bf, glyphs_per_line);
    if ( x > 0 )
    {
      if ( xmax < x )
	xmax = x;
      y +=  tga_get_line_height(bf_desc_font, bf)+1;
      //bf_Log(bf, "Draw TGA, encoding %04x, y %d", i, y);
    }
  }
  
  bf_Log(bf, "Draw TGA, xmax %d", xmax);
  
  tga_set_font(bf->target_data);
  
  //tga_draw_string(0, y, "Woven silk pyjamas exchanged for blue quartz", 1, xmax);
  //y += tga_get_char_height()+1;
  if ( font_picture_test_string != 0 )
  {
    tga_draw_string(left_margin, y, "Woven silk pyjamas exchanged for blue quartz.", 0, xmax);
    y +=  tga_get_line_height(bf_desc_font, bf)+1;
  }
  return y;
}


/*================================================*/
/* main */

int main(int argc, char **argv)
{
  bf_t *bf_desc_font;
  bf_t *bf;
  char *bdf_filename = NULL;
  int is_verbose = 0;
  char *map_str ="*";
  char *map_filename ="";
  char *desc_font_str = "";
  unsigned y;
  
  argv++;
  /*
  if ( *argv == NULL )
  {
    help();
    exit(1);
  }
  */
  for(;;)
  {
    if ( *argv == NULL )
      break;
    if ( is_arg(&argv, 'h') != 0 )
    {
      help();
      exit(1);
    }
    else if ( is_arg(&argv, 'v') != 0 )
    {
      is_verbose = 1;
    }
    else if ( is_arg(&argv, 'a') != 0 )
    {
      font_picture_extra_info = 1;
    }
    else if ( is_arg(&argv, 't') != 0 )
    {
      font_picture_test_string = 1;
    }
    else if ( is_arg(&argv, 'r') != 0 )
    {
      runtime_test = 1;
    }    
    else if ( get_num_arg(&argv, 'g', &cmdline_glyphs_per_line) != 0 )
    {
    }
    else if ( get_num_arg(&argv, 'b', &build_bbx_mode) != 0 )
    {
    }
    else if ( get_num_arg(&argv, 'f', &font_format) != 0 )
    {
    }
    else if ( get_num_arg(&argv, 'l', &left_margin) != 0 )
    {
    }
    else if ( get_num_arg(&argv, 'p', &min_distance_in_per_cent_of_char_width) != 0 )
    {
    }
    else if ( get_str_arg(&argv, 'd', &desc_font_str) != 0 )
    {      
    }
    else if ( get_str_arg(&argv, 'o', &c_filename) != 0 )
    {      
    }
    else if ( get_str_arg(&argv, 'n', &target_fontname) != 0 )
    {      
    }
    else if ( get_str_arg(&argv, 'm', &map_str) != 0 )
    {      
    }
    else if ( get_str_arg(&argv, 'k', &k_filename) != 0 )
    {      
    }
    else if ( get_str_arg(&argv, 'M', &map_filename) != 0 )
    {      
    }
    else
    {
      bdf_filename = *argv;
      argv++;
    }
  }
  
  if ( bdf_filename  == NULL )
  {
    help();
    exit(1);
  }

  bf_desc_font = NULL;
  if ( desc_font_str[0] != '\0' )
  {
    bf_desc_font = bf_OpenFromFile(desc_font_str, 0, BDF_BBX_MODE_MINIMAL, "*", "", 0);	/* assume format 0 for description */
    if ( bf_desc_font == NULL )
    {
      exit(1);
    }
  }

  if ( font_format == 2 )
  {
    build_bbx_mode = BDF_BBX_MODE_M8;
    /* issue the following log message later, when there is a valid bf object */
    /* bf_Log(bf, "Font mode 1: BBX mode set to 3"); */
  }
  
  bf = bf_OpenFromFile(bdf_filename, is_verbose, build_bbx_mode, map_str, map_filename, font_format);
  
  if ( bf == NULL )
  {
    exit(1);
  }

  if ( font_format == 2 )
  {
    /* now generate the log message */
    bf_Log(bf, "Note: For font format 1 BBX mode has been set to 3");
  }

  if ( bf_desc_font != NULL )
  {
    tga_init(1024, 1024*12);
    if ( target_fontname[0] != '\0' )
      y = tga_draw_font(0, target_fontname, bf_desc_font, bf, cmdline_glyphs_per_line);
    else
      y = tga_draw_font(0, bdf_filename, bf_desc_font, bf, cmdline_glyphs_per_line);
    
    if ( runtime_test != 0 )
    {
      long i;
      clock_t c = clock();
      fd_t fd;
      fd_init(&fd);
      fd_set_font(&fd, bf->target_data);
      for( i = 0; i < 10000; i++ )
	fd_draw_string(&fd, left_margin, y, "Woven silk pyjamas exchanged for blue quartz.");
      bf_Log(bf, "Runtime test: %.2lf sec", (double)(clock()-c)/(double)CLOCKS_PER_SEC);
    }
    
    tga_set_pixel(1, 1, 0, 0, 0);

    tga_save("bdf.tga");
  }

  
  
  if ( c_filename != NULL )
  {
    /* write the encoded data in bf->target_data */
    if ( font_format == 0 )
    {
      bf_WriteUCGCByFilename(bf, c_filename, target_fontname, "  ");
    }
    else
    {
      /* font format >= 1 are for u8g2 */
      bf_WriteU8G2CByFilename(bf, c_filename, target_fontname, "  ");
    }
  }

  if ( k_filename != NULL )
  {
    bdf_calculate_all_kerning(bf, k_filename, target_fontname, min_distance_in_per_cent_of_char_width);
  }


  
  bf_Close(bf);
  return 0;
}
