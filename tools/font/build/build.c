
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#ifdef BUILD2
#include "u8g2.h"
#endif

extern const uint8_t *u8x8_font_list[] ;
extern char *u8x8_font_names[] ;
extern const uint8_t *u8g2_font_list[] ;
extern char *u8g2_font_names[] ;

#ifdef BUILD2
extern void u8g2_SetupBuffer_TGA(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
extern void tga_save(const char *name);
#endif


/*===================================*/

int add_to_str(char **dest, const char *s)
{
  void *t;
  
  if ( *dest == NULL )
  {
    *dest = strdup(s);
    if ( *dest == NULL )
      return 0;
  }
  else
  {
    t = realloc(*dest, strlen(*dest) + strlen(s) + 1);
    if ( t == NULL )
      return 0;
    *dest = (char *)t;
    strcat(*dest, s);
  }
  
  return 1;
}

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
  Insert "text" between lines "start_line" and "end_line" of file "filename"
*/
int insert_into_file(const char *filename, const char *text, const char *start_line, const char *end_line)
{
  static char line[1024*4];
  const char *tmpname = "tmp.h";
  FILE *source_fp;
  FILE *dest_fp;
  
  if ( file_copy(filename, tmpname) == 0 )
    return 0;

  source_fp = fopen(tmpname, "r");
  dest_fp = fopen(filename, "w");

  if ( source_fp == NULL || dest_fp == NULL )
    return 0;
  
  for(;;)
  {
    if ( fgets(line, 1024*4, source_fp) == NULL )
      break;
    if ( strncmp(line, start_line, strlen(start_line)) == 0 )
    {
      fputs(line, dest_fp);
      fputs(text, dest_fp);
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

  fclose(source_fp);
  fclose(dest_fp);
  
  unlink(tmpname);

  return 1;
}

/*===================================*/


struct groupinfo
{
  char *groupname;
  char *reference;
  char *mdfile;
  char *mdprefixfile;
};

struct fontinfo
{
  //int is_ttf;		/* 0 = bdf, 1= ttf */
  char *ttf_opt;		/* 0 or "-r 72 -p 8" */
  char *filename;	/* filename including extension */
  char *name;
  int group;		/* group-index */
  int	kerning_min_distance_per_cent;			/* 0: do not generate kerning file */
  int build_mode;	/* Or'd BM_T, BM_H, BM_M, BM_8 */
  int font_mode;	/* Or'd FM_C and FM_8 */
  int map_mode;	/* Or'd MM_F, FM_N and FM_R */
  char *map_custom;	/* e.g. 32,42-58>32 */
  char *map_custom_postfix;
};

typedef void (*cbfn_t)(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms);



struct groupinfo gi[] = {
  { "U8glib", 		"fntgrpu8g", 		"../../../../u8g2.wiki/fntgrpu8g.md", 			"fntgrpu8g.pre" },
  { "X11", 		"fntgrpx11", 		"../../../../u8g2.wiki/fntgrpx11.md", 			"fntgrpx11.pre" },
  { "Fontstruct", 	"fntgrpfontstruct", 	"../../../../u8g2.wiki/fntgrpfontstruct.md", 	"fntgrpfontstruct.pre" },		/* 2 */
  { "cu12", 		"fntgrpcu12", 		"../../../../u8g2.wiki/fntgrpcu12.md", 		"fntgrpcu12.pre" },
  { "Profont", 		"fntgrpprofont", 	"../../../../u8g2.wiki/fntgrpprofont.md", 		"fntgrpprofont.pre" },		/* 4 */
  { "Adobe X11", 	"fntgrpadobex11", 	"../../../../u8g2.wiki/fntgrpadobex11.md", 	"fntgrpadobex11.pre" },
  { "Unifont", 		"fntgrpunifont", 	"../../../../u8g2.wiki/fntgrpunifont.md", 		"fntgrpunifont.pre" }, 		/* 6 */
  { "Open Game Art", "fntgrpopengameart", 	"../../../../u8g2.wiki/fntgrpopengameart.md", 	"fntgrpopengameart.pre" }, 		/* 7 */
  { "Free Universal", "fntgrpfreeuniversal", 	"../../../../u8g2.wiki/fntgrpfreeuniversal.md", 	"fntgrpfreeuniversal.pre" }, 		/* 8 */
  { "Old Standard", 	"fntgrpoldstandard", 	"../../../../u8g2.wiki/fntgrpoldstandard.md", 	"fntgrpoldstandard.pre" }, 		/* 9 */
  { "Logisoso", 		"fntgrplogisoso", 		"../../../../u8g2.wiki/fntgrplogisoso.md", 		"fntgrplogisoso.pre" }, 		/* 10 */
  { "Inconsolata", 	"fntgrpinconsolata", 	"../../../../u8g2.wiki/fntgrpinconsolata.md", 	"fntgrpinconsolata.pre" }, 		/* 11 */
  { "Codeman38", 	"fntgrpcodeman38",	"../../../../u8g2.wiki/fntgrpcodeman38.md",	"fntgrpcodeman38.pre" },		/* 12 */
  { "Academia Sinica","fntgrpacademiasinica",	"../../../../u8g2.wiki/fntgrpacademiasinica.md",	"fntgrpacademiasinica.pre" },		/* 13 */
  { "Oldschool PC Fonts","fntgrpoldschoolpcfonts",	"../../../../u8g2.wiki/fntgrpoldschoolpcfonts.md",	"fntgrpoldschoolpcfonts.pre" },  /* 14 */		/* 14 */
  { "crox", 		"fntgrpcrox", 		"../../../../u8g2.wiki/fntgrpcrox.md", 			"fntgrpcrox.pre" }, 		/* 15 */
  { "efont", 		"fntgrpefont", 		"../../../../u8g2.wiki/fntgrpefont.md", 		"fntgrpefont.pre" }, 		/* 16 */
  { "Tlwg (Thai-Fonts)","fntgrptlwg", 		"../../../../u8g2.wiki/fntgrptlwg.md", 		"fntgrptlwg.pre" }, 		/* 17 */
  { "NBP", 		"fntgrpnbp", 		"../../../../u8g2.wiki/fntgrpnbp.md", 			"fntgrpnbp.pre" }, 		/* 18 */
  { "UW ttyp0", 	"fntgrpttyp0", 		"../../../../u8g2.wiki/fntgrpttyp0.md", 		"fntgrpttyp0.pre" }, 		/* 19 */
  { "Siji Icon Font", 	"fntgrpsiji", 		"../../../../u8g2.wiki/fntgrpsiji.md", 		"fntgrpsiji.pre" }, 		/* 20 */
  { "Wqy (Chinese Font)", 	"fntgrpwqy", 		"../../../../u8g2.wiki/fntgrpwqy.md", 		"fntgrpwqy.pre" }, 		/* 21 */  
};

#define BM_T	1	/* Transparent = build mode 0 proportional */
#define BM_H	2	/* Common Height = build mode 1 */
#define BM_M	4	/* Monospace = build mode 2 */
#define BM_8	8	/* 8x8 = build mode 3 */
#define FM_C	1	/* u8g2 compressed font */
#define FM_8	2	/* u8x8 uncompressed font */
#define MM_F	1	/* full */
#define MM_R	2	/* reduced */
#define MM_U	4	/* uppercase */
#define MM_N	8	/* numbers */
#define MM_C	16	/* custom */
#define MM_M	32	/* map file */
#define MM_E	64	/* extended */


/*
Greek 	$370-$3ff		_greek
Cyrillic	$400-$52f 	_cyrillic
Armenian	0530–058F
Hebrew	0590–05FF
Thai		0E00–0E7F
Georgian	10A0–10FF
Latin Extended Additional	1E00–1EFF
Greek Extended	1F00–1FFF
*/

struct fontinfo fi[] ={
	
  { 0, "u8glib_4.bdf", 		"u8glib_4", 		0, 0, BM_T|BM_H, FM_C, MM_F|MM_R, "", "" },
  
  
  { 0, "m2icon_5.bdf", 		"m2icon_5", 		0, 0, BM_T, FM_C, MM_F, "", ""},
  { 0, "m2icon_7.bdf", 		"m2icon_7", 		0, 0, BM_T, FM_C, MM_F, "", ""},
  { 0, "m2icon_9.bdf", 		"m2icon_9", 		0, 0, BM_T, FM_C, MM_F, "", ""},
  
  { 0, "freedoomr10r.bdf", 	"freedoomr10", 	0, 0, BM_T|BM_M, FM_C, MM_U, "", ""},
  { 0, "freedoomr25n.bdf", 	"freedoomr25", 	0, 0, BM_T|BM_M, FM_C, MM_N, "", ""},

  { 0, "amstrad_cpc_extended.bdf", 	"amstrad_cpc_extended", 	2, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},
  
  { 0, "cursor.bdf", 			"cursor", 			1, 0, BM_T, FM_C, MM_C, "0-223>32", "f" },
  { 0, "cursor.bdf", 			"cursor", 			1, 0, BM_T, FM_C, MM_C, "0-80>32", "r" },
  { 0, "micro.bdf", 			"micro", 			1, 0, BM_T|BM_M, FM_C, MM_R|MM_N, "", "" },		/* micro does not have the full set */
  { 0, "4x6.bdf", 			"4x6", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "4x6.bdf", 			"4x6", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "5x7.bdf", 			"5x7", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "5x7.bdf", 			"5x7", 			1, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "", "" },
  { 0, "5x7.bdf", 			"5x7", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "5x8.bdf", 			"5x8", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "5x8.bdf", 			"5x8", 			1, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "", "" },
  { 0, "5x8.bdf", 			"5x8", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "6x10.bdf", 			"6x10", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "6x12.bdf", 			"6x12", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "6x12.bdf", 			"6x12", 			1, 0, BM_T|BM_M, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0, "6x13.bdf", 			"6x13", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "6x13.bdf", 			"6x13", 			1, 0, BM_T, FM_C, MM_C, "32-128,$590-$5ff,$fb1d-$fb4f", "_hebrew" },
  { 0, "6x13.bdf", 			"6x13", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "6x13B.bdf", 			"6x13B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "6x13B.bdf", 			"6x13B", 			1, 0, BM_T, FM_C, MM_C, "32-128,$590-$5ff,$fb1d-$fb4f", "_hebrew" },
  { 0, "6x13B.bdf", 			"6x13B", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "6x13O.bdf", 			"6x13O", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "7x13.bdf", 			"7x13", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "7x13.bdf", 			"7x13", 			1, 0, BM_T|BM_M, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0, "7x13.bdf", 			"7x13", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "7x13B.bdf", 			"7x13B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "7x13O.bdf", 			"7x13O", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "7x14.bdf", 			"7x14", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "7x14B.bdf", 			"7x14B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
//#ifdef TMP
  { 0, "8x13.bdf", 			"8x13", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "8x13.bdf", 			"8x13", 			1, 0, BM_T|BM_M, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0, "8x13.bdf", 			"8x13", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "8x13B.bdf", 			"8x13B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "8x13O.bdf", 			"8x13O", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },

  
  { 0, "9x15.bdf", 			"9x15", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "9x15.bdf", 			"9x15", 			1, 0, BM_T|BM_M, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0, "9x15.bdf", 			"9x15", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "9x15B.bdf", 			"9x15B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "9x18.bdf", 			"9x18", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "9x18B.bdf", 			"9x18B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "10x20.bdf", 			"10x20", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "10x20.bdf", 			"10x20", 			1, 0, BM_T, FM_C, MM_C, "32-128,$370-$3ff", "_greek" },
  { 0, "10x20.bdf", 			"10x20", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "10x20.bdf", 			"10x20", 			1, 35, BM_T, FM_C, MM_C, "32-128,$600-$6ff,$FB50-$FBB1,$FE70-$FEFF,x32-64,x91-96,x123-191,x247,x697-879,x32-$5ff", "_arabic" },

  { 0, "siji_with_6x10.bdf", 	"siji", 			20, 0, BM_T, FM_C, MM_C, "32-128,$e000-$ffff", "_6x10" },
  { 0, "siji_with_6x10.bdf", 	"siji", 			20, 0, BM_T, FM_C, MM_C, "$e000-$ffff", "" },
  
  
/*
t0 includes the following pages/language support:
Greek 	$370-$3ff		_greek
Cyrillic	$400-$52f 	_cyrillic
Armenian	0530–058F
Hebrew	0590–05FF
Thai		0E00–0E7F
Georgian	10A0–10FF
Latin Extended Additional	1E00–1EFF
Greek Extended	1F00–1FFF
*/
  
  { 0, "t0-11-uni.bdf", 		"t0_11", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-11-uni.bdf", 		"t0_11", 			19, 0, BM_T, FM_C, MM_C, "$0020-$FFF0", "_all" },
  { 0, "t0-11b-uni.bdf", 		"t0_11b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-12-uni.bdf", 		"t0_12", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-12b-uni.bdf", 		"t0_12b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-13-uni.bdf", 		"t0_13", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-13b-uni.bdf", 		"t0_13b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-14-uni.bdf", 		"t0_14", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-14b-uni.bdf", 		"t0_14b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-15-uni.bdf", 		"t0_15", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-15b-uni.bdf", 		"t0_15b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-16-uni.bdf", 		"t0_16", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-16b-uni.bdf", 		"t0_16b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-17-uni.bdf", 		"t0_17", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-17b-uni.bdf", 		"t0_17b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-18-uni.bdf", 		"t0_18", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-18b-uni.bdf", 		"t0_18b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-22-uni.bdf", 		"t0_22", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "t0-22b-uni.bdf", 		"t0_22b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },

  
  { 0, "profont10.bdf", 		"profont10", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "profont11.bdf", 		"profont11", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "profont12.bdf", 		"profont12", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "profont15.bdf", 		"profont15", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "profont17.bdf", 		"profont17", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "profont22.bdf", 		"profont22", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "profont29.bdf", 		"profont29", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },

  { "-r 72 -p 16", "MOZART_0.ttf",	"mozart_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "MOZART_0.ttf",	"mozart_nbp",			18, 0, BM_T|BM_H, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "glasstown_nbp.ttf",	"glasstown_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "glasstown_nbp.ttf",	"glasstown_nbp",	18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "shylock_nbp.ttf",	"shylock_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "shylock_nbp.ttf",	"shylock_nbp",			18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "RENT_0.ttf",	"roentgen_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "RENT_0.ttf",	"roentgen_nbp",			18, 0, BM_T|BM_H, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "CALIBRATE1.ttf",	"calibration_gothic_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "CALIBRATE1.ttf",	"calibration_gothic_nbp",	18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "smart_patrol_nbp.ttf",	"smart_patrol_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "prospero_bold_nbp.ttf", "prospero_bold_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "prospero_nbp.ttf",	"prospero_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "BALRG_0.ttf",	"balthasar_regular_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 32", "BALTT_0.ttf",	"balthasar_titling_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 8", "synchronizer_nbp.ttf",	"synchronizer_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "mercutio_basic.ttf", "mercutio_basic_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "mercutio_basic.ttf", "mercutio_basic_nbp",	18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "mercutio_sc.ttf", "mercutio_sc_nbp",		18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "mercutio_sc.ttf", "mercutio_sc_nbp",		18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "MIRANDA.ttf",	"miranda_nbp",		18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "nine0.ttf", "nine_by_five_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "nine0.ttf", "nine_by_five_nbp",			18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "ROSEN_0.ttf", "rosencrantz_nbp",		18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "ROSEN_0.ttf", "rosencrantz_nbp",		18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "GUILD_0.ttf", "guildenstern_nbp",		18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "GUILD_0.ttf", "guildenstern_nbp",		18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { "-r 72 -p 16", "astra0.ttf",	"astragal_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  

  
  /* thai fonts are not unicode encoded, so map the thai chars to their correct unicode position 128-255>3552 */
  { 0, "etl14-thai.bdf", 		"etl14thai", 		17, 0, BM_T, FM_C, MM_C, "32-127,128-255>3552", "" },
  { 0, "etl16-thai.bdf", 		"etl16thai", 		17, 0, BM_T, FM_C, MM_C, "32-127,128-255>3552", "" },
  { 0, "etl24-thai.bdf", 		"etl24thai", 		17, 0, BM_T, FM_C, MM_C, "32-127,128-255>3552", "" },

  
  
  /* crox fonts are CP1251 encoded */
  { 0, "win_crox1cb.bdf",		"crox1cb",		15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox1c.bdf",		"crox1c",			15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox1hb.bdf",		"crox1hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox1h.bdf",		"crox1h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox1tb.bdf",		"crox1tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox1t.bdf",		"crox1t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox2cb.bdf",		"crox2cb",		15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox2c.bdf",		"crox2c",			15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox2hb.bdf",		"crox2hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox2h.bdf",		"crox2h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox2tb.bdf",		"crox2tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox2t.bdf",		"crox2t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox3cb.bdf",		"crox3cb",		15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox3c.bdf",		"crox3c",			15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox3hb.bdf",		"crox3hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox3h.bdf",		"crox3h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox3tb.bdf",		"crox3tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox3t.bdf",		"crox3t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox4hb.bdf",		"crox4hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox4h.bdf",		"crox4h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox4tb.bdf",		"crox4tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox4t.bdf",		"crox4t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox5hb.bdf",		"crox5hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox5h.bdf",		"crox5h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox5tb.bdf",		"crox5tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "win_crox5t.bdf",		"crox5t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },

  
  { 0, "cu12.bdf", 			"cu12", 			3, 0, BM_T|BM_H|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "cu12.bdf", 			"cu12", 			3, 0, BM_T|BM_H, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0, "cu12.bdf", 			"cu12", 			3, 0, BM_T, FM_C, MM_C, "32-128,$370-$3ff", "_greek" },
  { 0, "cu12.bdf", 			"cu12", 			3, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "cu12.bdf", 			"cu12", 			3, 0, BM_T, FM_C, MM_C, "32-128,$f00-$fff", "_tibetan" },
  { 0, "cu12.bdf", 			"cu12", 			3, 0, BM_T, FM_C, MM_C, "32-128,$590-$5ff,$fb1d-$fb4f", "_hebrew" },
  { 0, "cu12.bdf", 			"cu12", 			3, 35, BM_T, FM_C, MM_C, "32-128,$600-$6ff,$FB50-$FBB1,$FE70-$FEFF,x32-64,x91-96,x123-191,x247,x697-879,x32-$5ff", "_arabic" },
  
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-255", "_latin" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-701", "_extended" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$370-$3ff", "_greek" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$590-$5ff,$fb1d-$fb4f", "_hebrew" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$980-$9ff", "_bengali" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$f00-$fff", "_tibetan" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$600-$6ff,$750-$77f,$fb50-$fdff,$fe70-$feff", "_urdu" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-127,$104-$107,$118-$119,$141-$144,$15a-$15b,$179-$17c,$d3,$f3", "_polish" },
  
  
  { 0, "unifont.bdf", 		"unifont", 		6, 35, BM_T, FM_C, MM_C, "32-128,$600-$6ff,$FB50-$FBB1,$FE70-$FEFF,x32-64,x91-96,x123-191,x247,x697-879,x32-$5ff", "_arabic" },


  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "chinese3.map", "_chinese3" },

  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "korean1.map", "_korean1" },
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "korean2.map", "_korean2" },
  
  
  { 0, "gb16st.bdf", 		"gb16st", 		13, 0, BM_T, FM_C, MM_M, "chinese1.map", "_1" },
  { 0, "gb16st.bdf", 		"gb16st", 		13, 0, BM_T, FM_C, MM_M, "chinese2.map", "_2" },
  { 0, "gb16st.bdf", 		"gb16st", 		13, 0, BM_T, FM_C, MM_M, "chinese3.map", "_3" },

  { 0, "gb24st.bdf", 		"gb24st", 		13, 0, BM_T, FM_C, MM_M, "chinese1.map", "_1" },
  { 0, "gb24st.bdf", 		"gb24st", 		13, 0, BM_T, FM_C, MM_M, "chinese2.map", "_2" },
  { 0, "gb24st.bdf", 		"gb24st", 		13, 0, BM_T, FM_C, MM_M, "chinese3.map", "_3" },

  { 0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },

  { 0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },

  { 0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },

  { 0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },
  
  { 0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },

//#endif   /* TMP */
  { 0, "b10.bdf", 			"b10", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "b10.bdf", 			"b10", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
//#ifdef TMP
  { 0, "b10_b.bdf", 			"b10_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "b10_b.bdf", 			"b10_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  { 0, "f10.bdf", 			"f10", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "f10.bdf", 			"f10", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0, "f10_b.bdf", 			"f10_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "f10_b.bdf", 			"f10_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  { 0, "b12.bdf", 			"b12", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "b12.bdf", 			"b12", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0, "b12_b.bdf", 			"b12_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "b12_b.bdf", 			"b12_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  { 0, "f12.bdf", 			"f12", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "f12.bdf", 			"f12", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0, "f12_b.bdf", 			"f12_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "f12_b.bdf", 			"f12_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  { 0, "b16.bdf", 			"b16", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "b16.bdf", 			"b16", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0, "b16_b.bdf", 			"b16_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "b16_b.bdf", 			"b16_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  { 0, "f16.bdf", 			"f16", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "f16.bdf", 			"f16", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0, "f16_b.bdf", 			"f16_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0, "f16_b.bdf", 			"f16_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  
  
  /*
    $20a0-$20bf currency symbols
    $2103 Circle C
    $2109 Circle F
    $2126 Ohm
    $2190-$21bb single arrow
    $21d0-$21d9 double arrow
    $21e6-$21e9 double arrow
    $23e9-$23fa  music player symbols
    $2580-$261f  various symbols
    $2654-$2667 chess and cards
    $2680-$2685 dice
    $2713-$2718 yes/no
    $274f-$2752 3d box
    $1f600-$1f64f emoticons
  */
  { 0, "unifont.bdf", 		"unifont", 		6, 0, BM_T|BM_H, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  

  { 0, "ArtosSans-8.bdf",			"artossans8",			7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  { 0, "ArtosSerif-8.bdf",			"artosserif8",			7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  { 0, "Chroma48Medium-8.bdf",	"chroma48medium8",	7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  /* no lowercase */
  { 0, "SaikyoSansBold-8.bdf",		"saikyosansbold8",		7, 0, BM_8, FM_C|FM_8, MM_U|MM_N, "" , ""},
  { 0, "TorusSansBold-8.bdf",		"torussansbold8",		7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  { 0, "VictoriaBold-8.bdf",		"victoriabold8",		7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  { 0, "VictoriaMedium-8.bdf",		"victoriamedium8",		7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  
  { 0, "courB08.bdf", 		"courB08", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courB10.bdf", 		"courB10", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courB12.bdf", 		"courB12", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courB14.bdf", 		"courB14", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courB18.bdf", 		"courB18", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courB24.bdf", 		"courB24", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courR08.bdf", 		"courR08", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courR10.bdf", 		"courR10", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courR12.bdf", 		"courR12", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courR14.bdf", 		"courR14", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courR18.bdf", 		"courR18", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "courR24.bdf", 		"courR24", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },

  { 0, "helvB08.bdf", 		"helvB08", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvB10.bdf", 		"helvB10", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvB12.bdf", 		"helvB12", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvB14.bdf", 		"helvB14", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvB18.bdf", 		"helvB18", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvB24.bdf", 		"helvB24", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvR08.bdf", 		"helvR08", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvR10.bdf", 		"helvR10", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvR12.bdf", 		"helvR12", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvR14.bdf", 		"helvR14", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvR18.bdf", 		"helvR18", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "helvR24.bdf", 		"helvR24", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },

  { 0, "ncenB08.bdf", 		"ncenB08", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenB10.bdf", 		"ncenB10", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenB12.bdf", 		"ncenB12", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenB14.bdf", 		"ncenB14", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenB18.bdf", 		"ncenB18", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenB24.bdf", 		"ncenB24", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenR08.bdf", 		"ncenR08", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenR10.bdf", 		"ncenR10", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenR12.bdf", 		"ncenR12", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenR14.bdf", 		"ncenR14", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenR18.bdf", 		"ncenR18", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0, "ncenR24.bdf", 		"ncenR24", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },

  { 0, "timB08.bdf", 		"timB08", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timB10.bdf", 		"timB10", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timB12.bdf", 		"timB12", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timB14.bdf", 		"timB14", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timB18.bdf", 		"timB18", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timB24.bdf", 		"timB24", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timR08.bdf", 		"timR08", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timR10.bdf", 		"timR10", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timR12.bdf", 		"timR12", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timR14.bdf", 		"timR14", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timR18.bdf", 		"timR18", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0, "timR24.bdf", 		"timR24", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  
  /* Adobe symb skipped... */


  
  { "-r 72 -p 8", "baby.ttf",			"baby",			2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 8", "blipfest_07.ttf",		"blipfest_07",		2, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { "-r 72 -p 8", "chikita.ttf",			"chikita",			2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 8", "lucasfont_alternate.ttf",	"lucasfont_alternate",2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 8", "p01type.ttf",			"p01type",		2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 8", "pixelle_micro.ttf",		"pixelle_micro",	2, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "robot_de_niro.ttf",	"robot_de_niro",	2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 8", "trixel_square.ttf",		"trixel_square",	2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "haxrcorp4089.ttf",	"haxrcorp4089",	2, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { "-r 72 -p 16", "haxrcorp4089.ttf",	"haxrcorp4089",	2, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },

/*
  Free Universal Bold
  
  ./do_fontsize_a_v2.sh 16 ../ttf/fu/FreeUniversal-Bold.ttf fub11
  ./do_fontsize_a_v2.sh 20 ../ttf/fu/FreeUniversal-Bold.ttf fub14
  ./do_fontsize_a_v2.sh 23 ../ttf/fu/FreeUniversal-Bold.ttf fub17
  ./do_fontsize_a_v2.sh 27 ../ttf/fu/FreeUniversal-Bold.ttf fub20
  ./do_fontsize_a_v2.sh 34 ../ttf/fu/FreeUniversal-Bold.ttf fub25
  ./do_fontsize_a_v2.sh 40 ../ttf/fu/FreeUniversal-Bold.ttf fub30
  ./do_fontsize_a_v2.sh 49 ../ttf/fu/FreeUniversal-Bold.ttf fub35
  ./do_fontsize_a_v2.sh 58 ../ttf/fu/FreeUniversal-Bold.ttf fub42
  ./do_fontsize_a_v2.sh 68 ../ttf/fu/FreeUniversal-Bold.ttf fub49
*/
  
  { "-a -r 72 -p 16", "FreeUniversal-Bold.ttf",	"fub11",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 20", "FreeUniversal-Bold.ttf",	"fub14",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 23", "FreeUniversal-Bold.ttf",	"fub17",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 27", "FreeUniversal-Bold.ttf",	"fub20",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 34", "FreeUniversal-Bold.ttf",	"fub25",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 40", "FreeUniversal-Bold.ttf",	"fub30",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 49", "FreeUniversal-Bold.ttf",	"fub35",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 58", "FreeUniversal-Bold.ttf",	"fub42",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 68", "FreeUniversal-Bold.ttf",	"fub49",			8, 0, BM_T, FM_C, MM_N, "", "" },

  { "-a -r 72 -p 16", "FreeUniversal-Bold.ttf", 	"fub11", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 20", "FreeUniversal-Bold.ttf", 	"fub14", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 23", "FreeUniversal-Bold.ttf", 	"fub17", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 27", "FreeUniversal-Bold.ttf", 	"fub20", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 34", "FreeUniversal-Bold.ttf", 	"fub25", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 40", "FreeUniversal-Bold.ttf", 	"fub30", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 49", "FreeUniversal-Bold.ttf", 	"fub35", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 58", "FreeUniversal-Bold.ttf", 	"fub42", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2031-$3000", "_symbol" },  // per mill sign does not fit
  { "-a -r 72 -p 68", "FreeUniversal-Bold.ttf", 	"fub49", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2031-$3000", "_symbol" },  // per mill sign does not fit

  
/*  
  Free Universal Regular
  
./do_fontsize_a_v2.sh 15 ../ttf/fu/FreeUniversal-Regular.ttf fur11
./do_fontsize_a_v2.sh 19 ../ttf/fu/FreeUniversal-Regular.ttf fur14
./do_fontsize_a_v2.sh 23 ../ttf/fu/FreeUniversal-Regular.ttf fur17
./do_fontsize_a_v2.sh 28 ../ttf/fu/FreeUniversal-Regular.ttf fur20
./do_fontsize_a_v2.sh 34 ../ttf/fu/FreeUniversal-Regular.ttf fur25
./do_fontsize_a_v2.sh 40 ../ttf/fu/FreeUniversal-Regular.ttf fur30
./do_fontsize_a_v2.sh 48 ../ttf/fu/FreeUniversal-Regular.ttf fur35
./do_fontsize_a_v2.sh 58 ../ttf/fu/FreeUniversal-Regular.ttf fur42
./do_fontsize_a_v2.sh 68 ../ttf/fu/FreeUniversal-Regular.ttf fur49
*/

  { "-a -r 72 -p 15", "FreeUniversal-Regular.ttf",	"fur11",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 19", "FreeUniversal-Regular.ttf",	"fur14",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 23", "FreeUniversal-Regular.ttf",	"fur17",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 28", "FreeUniversal-Regular.ttf",	"fur20",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 34", "FreeUniversal-Regular.ttf",	"fur25",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 40", "FreeUniversal-Regular.ttf",	"fur30",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 48", "FreeUniversal-Regular.ttf",	"fur35",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 58", "FreeUniversal-Regular.ttf",	"fur42",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 68", "FreeUniversal-Regular.ttf",	"fur49",			8, 0, BM_T, FM_C, MM_N, "", "" },

  { "-a -r 72 -p 16", "FreeUniversal-Regular.ttf", 	"fur11", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 19", "FreeUniversal-Regular.ttf", 	"fur14", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 23", "FreeUniversal-Regular.ttf", 	"fur17", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 28", "FreeUniversal-Regular.ttf", 	"fur20", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 34", "FreeUniversal-Regular.ttf", 	"fur25", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 40", "FreeUniversal-Regular.ttf", 	"fur30", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 48", "FreeUniversal-Regular.ttf", 	"fur35", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { "-a -r 72 -p 58", "FreeUniversal-Regular.ttf", 	"fur42", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2031-$3000", "_symbol" }, // per mill sign does not fit
  { "-a -r 72 -p 68", "FreeUniversal-Regular.ttf", 	"fur49", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2031-$3000", "_symbol" },  // per mill sign does not fit
  
/*
./do_fontsize_a_v2.sh 25 ../ttf/os/OldStandard-Bold.ttf osb18
./do_fontsize_a_v2.sh 28 ../ttf/os/OldStandard-Bold.ttf osb21
./do_fontsize_a_v2.sh 34 ../ttf/os/OldStandard-Bold.ttf osb26
./do_fontsize_a_v2.sh 38 ../ttf/os/OldStandard-Bold.ttf osb29
./do_fontsize_a_v2.sh 48 ../ttf/os/OldStandard-Bold.ttf osb35
./do_fontsize_a_v2.sh 55 ../ttf/os/OldStandard-Bold.ttf osb41
*/
/*
  bugfix 14 Jul 2017, this is the old (and wrong) code:
  { "-a -r 72 -p 25", "OldStandard-Regular.ttf",	"osb18",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 28", "OldStandard-Regular.ttf",	"osb21",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 34", "OldStandard-Regular.ttf",	"osb26",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 38", "OldStandard-Regular.ttf",	"osb29",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 48", "OldStandard-Regular.ttf",	"osb35",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 55", "OldStandard-Regular.ttf",	"osb41",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
*/
  
  { "-a -r 72 -p 25", "OldStandard-Bold.ttf",	"osb18",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 28", "OldStandard-Bold.ttf",	"osb21",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 34", "OldStandard-Bold.ttf",	"osb26",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 38", "OldStandard-Bold.ttf",	"osb29",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 48", "OldStandard-Bold.ttf",	"osb35",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 55", "OldStandard-Bold.ttf",	"osb41",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },

/*
./do_fontsize_a_v2.sh 26 ../ttf/os/OldStandard-Regular.ttf osr18
./do_fontsize_a_v2.sh 29 ../ttf/os/OldStandard-Regular.ttf osr21
./do_fontsize_a_v2.sh 36 ../ttf/os/OldStandard-Regular.ttf osr26
./do_fontsize_a_v2.sh 41 ../ttf/os/OldStandard-Regular.ttf osr29
./do_fontsize_a_v2.sh 49 ../ttf/os/OldStandard-Regular.ttf osr35
./do_fontsize_a_v2.sh 57 ../ttf/os/OldStandard-Regular.ttf osr41
*/

  { "-a -r 72 -p 26", "OldStandard-Regular.ttf",	"osr18",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 29", "OldStandard-Regular.ttf",	"osr21",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 36", "OldStandard-Regular.ttf",	"osr26",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 41", "OldStandard-Regular.ttf",	"osr29",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 49", "OldStandard-Regular.ttf",	"osr35",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 57", "OldStandard-Regular.ttf",	"osr41",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  
/*

inconsolata

./do_fontsize_a_mono_v2.sh 22 ../ttf/in/inr.otf inr16
./do_fontsize_a_mono_v2.sh 26 ../ttf/in/inr.otf inr19
./do_fontsize_a_mono_v2.sh 30 ../ttf/in/inr.otf inr21
./do_fontsize_a_mono_v2.sh 33 ../ttf/in/inr.otf inr24
./do_fontsize_a_mono_v2.sh 36 ../ttf/in/inr.otf inr27
./do_fontsize_a_mono_v2.sh 40 ../ttf/in/inr.otf inr30
./do_fontsize_a_mono_v2.sh 44 ../ttf/in/inr.otf inr33
./do_fontsize_a_mono_v2.sh 51 ../ttf/in/inr.otf inr38
./do_fontsize_a_mono_v2.sh 57 ../ttf/in/inr.otf inr42
./do_fontsize_a_mono_v2.sh 62 ../ttf/in/inr.otf inr46
./do_fontsize_a_mono_v2.sh 67 ../ttf/in/inr.otf inr49
./do_fontsize_a_mono_v2.sh  72 ../ttf/in/inr.otf inr53
./do_fontsize_a_mono_v2.sh  78 ../ttf/in/inr.otf inr57
./do_fontsize_a_mono_v2.sh  82 ../ttf/in/inr.otf inr62
*/
  { "-a -r 72 -p 22", "inr.otf",	"inr16",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 26", "inr.otf",	"inr19",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 30", "inr.otf",	"inr21",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 33", "inr.otf",	"inr24",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 36", "inr.otf",	"inr27",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 40", "inr.otf",	"inr30",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 44", "inr.otf",	"inr33",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 51", "inr.otf",	"inr38",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 57", "inr.otf",	"inr42",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 62", "inr.otf",	"inr46",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 67", "inr.otf",	"inr49",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 72", "inr.otf",	"inr53",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 78", "inr.otf",	"inr57",			11, 0, BM_M, FM_C, MM_N, "", "" },
  { "-a -r 72 -p 82", "inr.otf",	"inr62",			11, 0, BM_M, FM_C, MM_N, "", "" },
  
/*

./do_fontsize_a_mono_v2.sh 22 ../ttf/in/inb.otf inb16
./do_fontsize_a_mono_v2.sh 25 ../ttf/in/inb.otf inb19
./do_fontsize_a_mono_v2.sh 30 ../ttf/in/inb.otf inb21
./do_fontsize_a_mono_v2.sh 33 ../ttf/in/inb.otf inb24
./do_fontsize_a_mono_v2.sh 36 ../ttf/in/inb.otf inb27
./do_fontsize_a_mono_v2.sh 40 ../ttf/in/inb.otf inb30
./do_fontsize_a_mono_v2.sh 44 ../ttf/in/inb.otf inb33
./do_fontsize_a_mono_v2.sh 51 ../ttf/in/inb.otf inb38
./do_fontsize_a_mono_v2.sh 57 ../ttf/in/inb.otf inb42
./do_fontsize_a_mono_v2.sh 62 ../ttf/in/inb.otf inb46
./do_fontsize_a_mono_v2.sh 66 ../ttf/in/inb.otf inb49
./do_fontsize_a_mono_v2.sh  71 ../ttf/in/inb.otf inb53
./do_fontsize_a_mono_v2.sh  78 ../ttf/in/inb.otf inb57
./do_fontsize_a_mono_v2.sh  84 ../ttf/in/inb.otf inb63	
*/

  { "-a -r 72 -p 22", "inb.otf",	"inb16",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 25", "inb.otf",	"inb19",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 30", "inb.otf",	"inb21",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 33", "inb.otf",	"inb24",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 36", "inb.otf",	"inb27",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 40", "inb.otf",	"inb30",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 44", "inb.otf",	"inb33",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 51", "inb.otf",	"inb38",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 57", "inb.otf",	"inb42",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 62", "inb.otf",	"inb46",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 66", "inb.otf",	"inb49",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 71", "inb.otf",	"inb53",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 78", "inb.otf",	"inb57",			11, 0, BM_M, FM_C, MM_N, "", "" },
  { "-a -r 72 -p 84", "inb.otf",	"inb63",			11, 0, BM_M, FM_C, MM_N, "", "" },

/*
  Logisoso

./do_fontsize_a_v2.sh 23 ../ttf/log/Logisoso.ttf logisoso16
./do_fontsize_a_v2.sh 27 ../ttf/log/Logisoso.ttf logisoso18
./do_fontsize_a_v2.sh 30 ../ttf/log/Logisoso.ttf logisoso20
./do_fontsize_a_v2.sh 32 ../ttf/log/Logisoso.ttf logisoso22
./do_fontsize_a_v2.sh 34 ../ttf/log/Logisoso.ttf logisoso24
./do_fontsize_a_v2.sh 38 ../ttf/log/Logisoso.ttf logisoso26
./do_fontsize_a_v2.sh 40 ../ttf/log/Logisoso.ttf logisoso28
./do_fontsize_a_no_64_v2.sh 43 ../ttf/log/Logisoso.ttf logisoso30
./do_fontsize_a_no_64_v2.sh 45 ../ttf/log/Logisoso.ttf logisoso32
./do_fontsize_a_no_64_v2.sh 49 ../ttf/log/Logisoso.ttf logisoso34
./do_fontsize_a_no_64_v2.sh 54 ../ttf/log/Logisoso.ttf logisoso38
./do_fontsize_a_no_64_v2.sh 60 ../ttf/log/Logisoso.ttf logisoso42
./do_fontsize_a_no_64_v2.sh 66 ../ttf/log/Logisoso.ttf logisoso46
./do_fontsize_a_no_64_v2.sh 71 ../ttf/log/Logisoso.ttf logisoso50
./do_fontsize_a_no_64_v2.sh  77 ../ttf/log/Logisoso.ttf logisoso54
./do_fontsize_a_no_64_v2.sh  83 ../ttf/log/Logisoso.ttf logisoso58
./do_fontsize_a_no_64_v2.sh  89 ../ttf/log/Logisoso.ttf logisoso62

*/

  { "-a -r 72 -p 23", "Logisoso.ttf",	"logisoso16",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 27", "Logisoso.ttf",	"logisoso18",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 30", "Logisoso.ttf",	"logisoso20",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 32", "Logisoso.ttf",	"logisoso22",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 34", "Logisoso.ttf",	"logisoso24",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 38", "Logisoso.ttf",	"logisoso26",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 40", "Logisoso.ttf",	"logisoso28",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 43", "Logisoso.ttf",	"logisoso30",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 45", "Logisoso.ttf",	"logisoso32",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 49", "Logisoso.ttf",	"logisoso34",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 54", "Logisoso.ttf",	"logisoso38",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 60", "Logisoso.ttf",	"logisoso42",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 66", "Logisoso.ttf",	"logisoso46",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 71", "Logisoso.ttf",	"logisoso50",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 77", "Logisoso.ttf",	"logisoso54",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 83", "Logisoso.ttf",	"logisoso58",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { "-a -r 72 -p 89", "Logisoso.ttf",	"logisoso62",			10, 0, BM_T, FM_C, MM_N, "", "" },
  { "-a -r 72 -p 110", "Logisoso.ttf",	"logisoso78",			10, 0, BM_T, FM_C, MM_N, "", "" },
  { "-a -r 72 -p 131", "Logisoso.ttf",	"logisoso92",			10, 0, BM_T, FM_C, MM_N, "", "" },

  { "-r 72 -p 8", "PressStart2P.ttf",	"pressstart2p",			12, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},
  { "-r 72 -p 8", "pcsenior.ttf",		"pcsenior",			12, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},


    /*
  PxPlus_IBM_CGAthin.ttf
  PxPlus_IBM_CGA.ttf
  PxPlus_TandyNew_TV.ttf
  */
  
  { "-r 72 -p 8", "PxPlus_IBM_CGAthin.ttf",	"pxplusibmcgathin",	14, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},
  { "-r 72 -p 8", "PxPlus_IBM_CGA.ttf",		"pxplusibmcga",		14, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},
  { "-r 72 -p 8", "PxPlus_TandyNew_TV.ttf",	"pxplustandynewtv",	14, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},  
  { "-r 72 -p 8", "PxPlus_TandyNew_TV.ttf",	"pxplustandynewtv",	14, 0, BM_T|BM_8, FM_C, MM_C, "32-$ffff", "_all" },

  
  { "-r 72 -p 16", "PxPlus_IBM_VGA9.ttf",	"pxplusibmvga9",	14, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "" , ""},
  { "-r 72 -p 16", "PxPlus_IBM_VGA9.ttf",	"pxplusibmvga9",	14, 0, BM_T|BM_M, FM_C, MM_C, "32-$ffff", "_all" },
  
  { "-r 72 -p 16", "PxPlus_IBM_VGA8.ttf",	"pxplusibmvga8",	14, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "" , ""},
  { "-r 72 -p 16", "PxPlus_IBM_VGA8.ttf",	"pxplusibmvga8",	14, 0, BM_T|BM_M, FM_C, MM_C, "32-$ffff", "_all" },
  
  { "-r 72 -p 16", "Px437_Wyse700a.ttf",	"px437wyse700a",	14, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "" , ""},
  { "-r 72 -p 16", "Px437_Wyse700b.ttf",	"px437wyse700b",	14, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "" , ""},
  
//#endif /* TMP */

};




char *bdf_path = "../bdf/";
char *bdfconv_path = "../bdfconv/bdfconv";
char *otf2bdf_path = "../otf2bdf/otf2bdf";
FILE *u8g2_font_list_fp;
FILE *u8x8_font_list_fp;
FILE *keywords_fp;
char *u8g2_prototypes = NULL;
char *u8x8_prototypes = NULL;
char *u8g2_fonts_filename = "../../../csrc/u8g2_fonts.c";
char *u8x8_fonts_filename = "../../../csrc/u8x8_fonts.c";

char target_font_identifier[1024];
char otf_cmd[2048];
char bdf_cmd[2048];
char font_prototype[2048];
char tga_filename[2048];
char convert_cmd[2048];
int current_font_group_index;
char current_font_name[256] = "";
FILE *current_md_file;
int current_capital_A_size;
const char *fntlistallpath = "../../../../u8g2.wiki/fntlistall.md";
FILE *fntlistall;
const char *fntlistmonopath = "../../../../u8g2.wiki/fntlistmono.md";
FILE *fntlistmono;
const char *fntlist8x8path = "../../../../u8g2.wiki/fntlist8x8.md";
FILE *fntlist8x8;

#ifdef BUILD2

u8g2_t u8g2;
int u8x8_fnt_cnt = 0;
int u8g2_fnt_cnt = 0;

extern void do_font_loop(cbfn_t cb);


void overview_draw_line(int i, uint16_t encoding_start, uint16_t x, uint16_t y, uint16_t w)
{
  int j;

  u8g2_SetFont(&u8g2, u8g2_font_list[u8g2_fnt_cnt]);
  u8g2_SetFontDirection(&u8g2, 0);
  for( j = 0; j < 16; j++)
  {
    if ( u8g2_IsGlyph(&u8g2, encoding_start + j) != 0 )
    {
      u8g2_DrawGlyph(&u8g2, x+j*w, y, encoding_start + j);
    }
  }
}

int is_overview_line_empty(uint16_t encoding_start)
{
  int j;
  for( j = 0; j < 16; j++)
  {
    if ( u8g2_IsGlyph(&u8g2, encoding_start + j) != 0 )
    {
      return 0;
    }
  }
  return 1;
}

void overview_draw_table(int i, uint16_t x, uint16_t y)
{
  int cw, ch;
  int line, h;
  uint16_t encoding;
  static char s[256];
  
  h = 13;
  
  u8g2_SetFont(&u8g2, u8g2_font_list[u8g2_fnt_cnt]);
  u8g2_SetFontDirection(&u8g2, 0);
  u8g2_SetFontMode(&u8g2, 1);

  ch = u8g2_GetMaxCharHeight(&u8g2);
  cw = u8g2_GetMaxCharWidth(&u8g2);
  
  sprintf(s, "BBX Width %d, Height %d, Capital A %d", 
    u8g2_GetMaxCharWidth(&u8g2), 
    u8g2_GetMaxCharHeight(&u8g2),
    u8g2_GetAscent(&u8g2));
    
  u8g2_SetFont(&u8g2, u8g2_font_7x13_tr);    
  u8g2_DrawStr(&u8g2, 0, h, u8g2_font_names[u8g2_fnt_cnt]);
  u8g2_DrawStr(&u8g2, 0, h*2, s);
  
  sprintf(s, "Font Data Size: %d Bytes", u8g2_GetFontSize(u8g2_font_list[u8g2_fnt_cnt]));
  u8g2_DrawStr(&u8g2, 0, h*3, s);

  u8g2_SetFont(&u8g2, u8g2_font_list[u8g2_fnt_cnt]);
  u8g2_SetFontDirection(&u8g2, 0);

  ch = u8g2_GetMaxCharHeight(&u8g2);
  cw = u8g2_GetMaxCharWidth(&u8g2);
  if ( ch < h )
    ch = h;
  y = h*3+ch+1;
  
  line = 0;
  for(;;)
  {
    encoding = line*16;
    if ( is_overview_line_empty(encoding) == 0 )
    {
      u8g2_SetFont(&u8g2, u8g2_font_7x13_tr);    
      sprintf(s, "%5d/%04x ", encoding, encoding);
      
      x = u8g2_DrawStr(&u8g2, 0, y, s);
      overview_draw_line(i, encoding, x, y, cw+1);
      y += ch;
    }
    line++;
    if ( line > 0xfff )
      break;
  }
  if ( u8g2_IsGlyph(&u8g2, 'A') != 0 && u8g2_IsGlyph(&u8g2, 'z') != 0 )
  {
    y++;
    // y -= ch;
    u8g2_SetFont(&u8g2, u8g2_font_list[u8g2_fnt_cnt]);
    u8g2_SetFontDirection(&u8g2, 0); 
    //y += u8g2_GetMaxCharHeight(&u8g2);
    u8g2_DrawStr(&u8g2, 0, y, "The quick brown fox");
    y += u8g2_GetMaxCharHeight(&u8g2);
    u8g2_DrawStr(&u8g2, 0, y, "jumps over the lazy dog.");
  }
  //u8g2_DrawStr(&u8g2, 0, y, "Woven silk pyjamas exchanged for blue quartz"); 

}



void overviewpic(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms)
{
  int cw, ch;
  if ( fm == FM_8 ) 
  {
  
    printf("8x8 font overview picture %s\n", target_font_identifier);  
    u8g2_SetupBuffer_TGA(&u8g2, &u8g2_cb_r0);
    u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
    //u8x8_ClearDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetFont(u8g2_GetU8x8(&u8g2), u8x8_font_amstrad_cpc_extended_r);
    u8x8_DrawString(u8g2_GetU8x8(&u8g2), 0, 0, target_font_identifier);
    u8x8_SetFont(u8g2_GetU8x8(&u8g2), u8x8_font_list[u8x8_fnt_cnt]);
    {
      uint8_t x;
      uint8_t y;
      static char s[32];
      for( y = 0; y < 16; y++ )
      {
	u8x8_SetFont(u8g2_GetU8x8(&u8g2), u8x8_font_amstrad_cpc_extended_r);
	sprintf(s, "%3d/%02x ", y*16, y*16);
	u8x8_DrawString(u8g2_GetU8x8(&u8g2), 0, y+2, s);
  	u8x8_SetFont(u8g2_GetU8x8(&u8g2), u8x8_font_list[u8x8_fnt_cnt]);
	for( x = 0; x < 16; x++ )
	{
	    u8x8_DrawGlyph(u8g2_GetU8x8(&u8g2), x*2+7,y+2, y*16+x);
	}
      }
    }
    
    if ( mm != MM_N )
      u8x8_DrawString(u8g2_GetU8x8(&u8g2), 0,16+2, "The quick brown fox jumps over the lazy dog.");

    tga_save("font.tga");
    
    sprintf(convert_cmd, "convert font.tga -trim %s.png", target_font_identifier );
    system(convert_cmd);
    u8x8_fnt_cnt++;
  }
  else if ( fm == FM_C ) 
  {
    printf("overview picture %s\n", target_font_identifier);
    u8g2_SetupBuffer_TGA(&u8g2, &u8g2_cb_r0);
    u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
    //u8g2_SetFont(&u8g2, u8g2_font_helvB14_tr);

    
    u8g2_FirstPage(&u8g2);
    do
    {

      overview_draw_table(i, 0, 60);

      //overviewline(i, '@', 0, 80, cw);      
    } while( u8g2_NextPage(&u8g2) );

    strcpy(tga_filename, target_font_identifier);
    strcat(tga_filename, ".tga");
    
    tga_save("font.tga");
    
    sprintf(convert_cmd, "convert font.tga -trim %s.png", target_font_identifier );
    system(convert_cmd);

    u8g2_fnt_cnt++;
  }
}

void overviewshortpic(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms)
{
  int cw, ch;
  if ( fm == FM_8 )
  {
    printf("8x8 font short overview picture %s\n", target_font_identifier);  
    u8g2_SetupBuffer_TGA(&u8g2, &u8g2_cb_r0);
    u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
    //u8x8_ClearDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetFont(u8g2_GetU8x8(&u8g2), u8x8_font_list[u8x8_fnt_cnt]);

    if ( mm == MM_N )
      u8x8_DrawString(u8g2_GetU8x8(&u8g2), 0, 0, "1234567890");
    else if ( mm == MM_U )
      u8x8_DrawString(u8g2_GetU8x8(&u8g2), 0, 0, "ABCDEF 123");
    else
      u8x8_DrawString(u8g2_GetU8x8(&u8g2), 0, 0, "Abcdefg 123");
    
    tga_save("font.tga");
    
    sprintf(convert_cmd, "convert font.tga -trim %s_short.png", target_font_identifier );
    system(convert_cmd);

    u8x8_fnt_cnt++;
  }
  else if ( fm == FM_C ) 
  {
    printf("short overview picture %s\n", target_font_identifier);
    u8g2_SetupBuffer_TGA(&u8g2, &u8g2_cb_r0);
    u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
    //u8g2_SetFont(&u8g2, u8g2_font_helvB14_tr);

    u8g2_SetFont(&u8g2, u8g2_font_list[u8g2_fnt_cnt]);
    u8g2_SetFontDirection(&u8g2, 0);
    ch = u8g2_GetMaxCharHeight(&u8g2);
    cw = u8g2_GetMaxCharWidth(&u8g2);

    u8g2_FirstPage(&u8g2);
    do
    {

      if ( mm == MM_N )
	u8g2_DrawStr(&u8g2, 0, ch, "1234567890");
      else if ( mm == MM_U )
	u8g2_DrawStr(&u8g2, 0, ch, "ABCDEF 123");
      else
	u8g2_DrawStr(&u8g2, 0, ch, "Abcdefg 123");
    
    } while( u8g2_NextPage(&u8g2) );

    tga_save("font.tga");
    
    sprintf(convert_cmd, "convert font.tga -trim %s_short.png", target_font_identifier );
    system(convert_cmd);

    u8g2_fnt_cnt++;
  }
}

int font_found_for_this_size = 0;
int mfont_found_for_this_size = 0;

void generate_font_list(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms)
{

  if ( fm == FM_8 ) 
  {
    if ( current_capital_A_size == 8 )
    {
      fprintf(fntlist8x8, "![fntpic/%s_short.png](fntpic/%s_short.png) ", target_font_identifier, target_font_identifier);
      fprintf(fntlist8x8, "%s ", target_font_identifier);
      fprintf(fntlist8x8, " [%s](%s)\n\n", gi[fi[i].group].groupname, gi[fi[i].group].reference);
  
      u8x8_fnt_cnt++;
    }
  }
  else if ( fm == FM_C ) 
  {
    u8g2_SetupBuffer_TGA(&u8g2, &u8g2_cb_r0);
    u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
    if ( u8g2_font_list[u8g2_fnt_cnt] != NULL )
    {
      u8g2_SetFont(&u8g2, u8g2_font_list[u8g2_fnt_cnt]);
      u8g2_SetFontDirection(&u8g2, 0);
      if ( current_capital_A_size == u8g2.font_info.ascent_A )
      {
	if ( font_found_for_this_size == 0 ) 
	{
	  fprintf(fntlistall, "\n## %d Pixel Height\n\n", current_capital_A_size);
	  printf("listall: == %d ==\n", current_capital_A_size);
	}
	font_found_for_this_size = 1;
	fprintf(fntlistall, "![fntpic/%s_short.png](fntpic/%s_short.png) ", target_font_identifier, target_font_identifier);
	fprintf(fntlistall, "%s ", target_font_identifier);
	fprintf(fntlistall, " [%s](%s)\n\n", gi[fi[i].group].groupname, gi[fi[i].group].reference);
	//printf("%d: %s %s\n", current_capital_A_size, target_font_identifier, gi[fi[i].group].groupname);
      }
	
      if ( bm == BM_M || bm == BM_8 )
      {
	if ( current_capital_A_size == u8g2.font_info.ascent_A )
	{
	  if ( mfont_found_for_this_size == 0 ) 
	  {
	    fprintf(fntlistmono, "\n## %d Pixel Height\n\n", current_capital_A_size);
	    printf("listmono: == %d ==\n", current_capital_A_size);
	  }
	  mfont_found_for_this_size = 1;
	  fprintf(fntlistmono, "![fntpic/%s_short.png](fntpic/%s_short.png) ", target_font_identifier, target_font_identifier);
	  fprintf(fntlistmono, "%s ", target_font_identifier);
	  fprintf(fntlistmono, " [%s](%s)\n\n", gi[fi[i].group].groupname, gi[fi[i].group].reference);
	  
	}
      }
      u8g2_fnt_cnt++;
    }
  }
}

void do_font_list(cbfn_t cb)
{

  file_copy("fntlistall.pre", fntlistallpath);
  fntlistall = fopen(fntlistallpath, "r+");
  fseek(fntlistall, 0L, SEEK_END);
  fntlistmono = fopen(fntlistmonopath, "w");
  fntlist8x8 = fopen(fntlist8x8path, "w");
  fprintf(fntlistall, "# All U8g2 Fonts, Capital A Height\n\n");
  fprintf(fntlistmono, "# Monospaced and 8x8 Fonts for U8g2, Capital A Height\n\n");
  fprintf(fntlist8x8, "# Fonts for U8x8\n\n");
  for( current_capital_A_size = 2; current_capital_A_size < 100; current_capital_A_size++ )
  {
    font_found_for_this_size = 0;
    mfont_found_for_this_size = 0;
    u8x8_fnt_cnt = 0;
    u8g2_fnt_cnt = 0;
    do_font_loop(cb);
  }
  fclose(fntlistall);
  fclose(fntlistmono);
  fclose(fntlist8x8);
}


#endif

void bdfconv(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms)
{
  
  if ( fi[i].ttf_opt != NULL )
  {
    strcpy(otf_cmd, otf2bdf_path);
    strcat(otf_cmd, " ");
    strcat(otf_cmd, fi[i].ttf_opt);
    strcat(otf_cmd, " ../ttf/");
    strcat(otf_cmd, fi[i].filename);
    strcat(otf_cmd, " -o tmp.bdf");
    printf("%s\n", otf_cmd);
    system(otf_cmd);
  }
  
  
  strcpy(bdf_cmd, bdfconv_path);
  strcat(bdf_cmd, " ");

  if ( fm == FM_C ) strcat(bdf_cmd, " -f 1");
  if ( fm == FM_8 ) strcat(bdf_cmd, " -f 2");
  
  if ( bm == BM_T ) strcat(bdf_cmd, " -b 0");
  if ( bm == BM_H ) strcat(bdf_cmd, " -b 1");
  if ( bm == BM_M ) strcat(bdf_cmd, " -b 2");
  if ( bm == BM_8 ) strcat(bdf_cmd, " -b 3");
  
  if ( mm == MM_F ) strcat(bdf_cmd, " -m '32-255>32'");
  if ( mm == MM_R ) strcat(bdf_cmd, " -m '32-127>32'");
  if ( mm == MM_N ) strcat(bdf_cmd, " -m '32,42-58>42'");
  if ( mm == MM_U ) strcat(bdf_cmd, " -m '32-95>32'");
  if ( mm == MM_E ) strcat(bdf_cmd, " -m '32-701>32,7838'");
  if ( mm == MM_C ) 
  {
    strcat(bdf_cmd, " -m '");
    strcat(bdf_cmd, fi[i].map_custom);
    strcat(bdf_cmd, "'");
  }
  if ( mm == MM_M ) 
  {
    strcat(bdf_cmd, " -M '");
    strcat(bdf_cmd, fi[i].map_custom);
    strcat(bdf_cmd, "'");
  }

  strcat(bdf_cmd, " ");
  if ( fi[i].ttf_opt != NULL )
  {
    strcat(bdf_cmd, "tmp.bdf");
  }
  else
  {
    strcat(bdf_cmd, bdf_path);
    strcat(bdf_cmd, fi[i].filename);
  }

  strcat(bdf_cmd, " -n ");
  strcat(bdf_cmd, target_font_identifier);

  strcat(bdf_cmd, " -o ");
  //strcat(bdf_cmd, target_font_identifier);
  strcat(bdf_cmd, "font");
  strcat(bdf_cmd, ".c");
  
  
  /*
  if ( fi[i].kerning_min_distance_per_cent > 0 )
  {
    strcat(bdf_cmd, " -k ");
    strcat(bdf_cmd, target_font_identifier);
    strcat(bdf_cmd, "_k.c");
	  
    strcat(bdf_cmd, " -p ");
    sprintf(bdf_cmd+strlen(bdf_cmd), "%d", fi[i].kerning_min_distance_per_cent);
	
  }
  */
  
/*
    fprintf(out_fp, "const uint8_t %s[%d] U8X8_FONT_SECTION(\"%s\") \n", fontname, bf->target_cnt, fontname);
    fprintf(out_fp, "const uint8_t %s[%d] U8G2_FONT_SECTION(\"%s\") \n", fontname, bf->target_cnt, fontname);

*/
  strcpy(font_prototype, "extern const uint8_t ");
  strcat(font_prototype, target_font_identifier);
  strcat(font_prototype, "[]");

  if ( fm == FM_8 ) 
  {
    strcat(bdf_cmd, " && cat font.c >>");
    strcat(bdf_cmd, u8x8_fonts_filename);
    strcat(font_prototype, " U8X8_FONT_SECTION(\"");    
    strcat(font_prototype, target_font_identifier);
    strcat(font_prototype, "\");\n");
    add_to_str(&u8x8_prototypes, font_prototype);
  }
  else
  {    
    strcat(bdf_cmd, " && cat font.c >>");
    strcat(bdf_cmd, u8g2_fonts_filename);
    strcat(font_prototype, " U8G2_FONT_SECTION(\"");    
    strcat(font_prototype, target_font_identifier);
    strcat(font_prototype, "\");\n");
    add_to_str(&u8g2_prototypes, font_prototype);
  }

  printf("%s\n", bdf_cmd);
  system(bdf_cmd);
  
  strcpy(bdf_cmd, "cp font.c ./single_font_files/");
  strcat(bdf_cmd, target_font_identifier);
  strcat(bdf_cmd, ".c");
  system(bdf_cmd);
  
}

void fontlist_identifier(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms)
{
  if ( fm == FM_C ) 
  {
    fprintf(u8g2_font_list_fp, "  %s,\n", target_font_identifier);
  }
  if ( fm == FM_8 ) 
  {
    fprintf(u8x8_font_list_fp, "  %s,\n", target_font_identifier);
  }
  
}

void fontlist_name(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms)
{
  if ( fm == FM_C ) 
  {
    fprintf(u8g2_font_list_fp, "  \"%s\",\n", target_font_identifier);
  }
  if ( fm == FM_8 ) 
  {
    fprintf(u8x8_font_list_fp, "  \"%s\",\n", target_font_identifier);
  }

  fprintf(keywords_fp, "%s\tLITERAL1\n", target_font_identifier);
  
  
}

void generate_font_group_md(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms)
{
  static int _i = 0;
  static int _fm = 0;
  static int _bm = 0;
  
  if ( fi[i].group == current_font_group_index )
  {
    if ( strcmp( current_font_name, fi[i].name ) != 0 )
    {
      strcpy(current_font_name, fi[i].name);
      
      fprintf(current_md_file, "\n");
      fprintf(current_md_file, "## %s\n", current_font_name);
      printf("## %s\n", current_font_name);      
    }
    else
    {
      if ( _i == i && _fm == fm && _bm == bm )
      {
      }
      else
      {
	fprintf(current_md_file, "\n");
      }
    }
    fprintf(current_md_file, "![fntpic/%s.png](fntpic/%s.png)\n", target_font_identifier, target_font_identifier);
    
    _i = i;
    _fm = fm;
    _bm = bm;
    printf( "%s %s\n", gi[current_font_group_index].mdfile, target_font_identifier);
  }
  
}


void gen_font(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms, cbfn_t cb )
{
  
  if ( fm == FM_8 )
    if ( bm != BM_8 )
      return;
  
  strcpy(target_font_identifier, fms);
  strcat(target_font_identifier, "_font_");
  strcat(target_font_identifier, fi[i].name);
  strcat(target_font_identifier, "_");
  strcat(target_font_identifier, bms);
  strcat(target_font_identifier, mms);
    
  cb(i, fm,fms,bm,bms,mm,mms);
}

void map_font(int i, int fm, char *fms, int bm, char *bms, cbfn_t cb)
{
  if ( (fi[i].map_mode & MM_F) != 0 )
    gen_font(i, fm, fms, bm, bms, MM_F, "f", cb);
  if ( (fi[i].map_mode & MM_R) != 0 )
    gen_font(i, fm, fms, bm, bms, MM_R, "r", cb);
  if ( (fi[i].map_mode & MM_N) != 0 )
    gen_font(i, fm, fms, bm, bms, MM_N, "n", cb);
  if ( (fi[i].map_mode & MM_U) != 0 )
    gen_font(i, fm, fms, bm, bms, MM_U, "u", cb);
  if ( (fi[i].map_mode & MM_C) != 0 )
    gen_font(i, fm, fms, bm, bms, MM_C, fi[i].map_custom_postfix, cb);
  if ( (fi[i].map_mode & MM_M) != 0 )
    gen_font(i, fm, fms, bm, bms, MM_M, fi[i].map_custom_postfix, cb);
  if ( (fi[i].map_mode & MM_E) != 0 )
    gen_font(i, fm, fms, bm, bms, MM_E, "e", cb);
}

void build_font(int i, int fm, char *fms, cbfn_t cb)
{
  if ( (fi[i].build_mode & BM_T) != 0 )
    map_font(i, fm, fms, BM_T, "t", cb);
  if ( (fi[i].build_mode & BM_H) != 0 )
    map_font(i, fm, fms, BM_H, "h", cb);
  if ( (fi[i].build_mode & BM_M) != 0 )
    map_font(i, fm, fms, BM_M, "m", cb);
  if ( (fi[i].build_mode & BM_8) != 0 )
  {
    if ( fm == FM_8 ) 
      map_font(i, fm, fms, BM_8, "", cb);
    else
      map_font(i, fm, fms, BM_8, "8", cb);
  }
}

void process_font(int i, cbfn_t cb)
{
  FILE *fp;
  static char s[1024];
 
  if ( fi[i].ttf_opt == 0 )
  {
    strcpy(s,bdf_path);
  }
  else
  {
    strcpy(s,"../ttf/");
  }
  strcat(s, fi[i].filename);
  fp = fopen(s, "r");
  if ( fp == NULL )
  {
    printf("font %s missing\n",s );
    return;
  }
  fclose(fp);
  
  if ( (fi[i].font_mode & FM_C) != 0 )
    build_font(i, FM_C, "u8g2", cb);
  if ( (fi[i].font_mode & FM_8) != 0 )
    build_font(i, FM_8, "u8x8", cb);
}

void do_font_loop(cbfn_t cb)
{
  int i, cnt;
  cnt = sizeof(fi)/sizeof(*fi);
  for( i = 0; i < cnt; i++ )
  {
    process_font(i, cb);
  }
}

void do_font_groups(cbfn_t cb)
{
  int cnt;
  cnt = sizeof(gi)/sizeof(*gi);
  for( current_font_group_index = 0; current_font_group_index < cnt; current_font_group_index++ )
  {
    file_copy(gi[current_font_group_index].mdprefixfile, gi[current_font_group_index].mdfile);
    current_md_file = fopen(gi[current_font_group_index].mdfile, "a");
    fprintf(current_md_file, "\n");
    strcpy(current_font_name, ".");    
    do_font_loop(cb);
    fclose(current_md_file);
  }
}



int main(void)
{
  //unlink(u8x8_fonts_filename);
  //unlink(u8g2_fonts_filename);

#ifndef BUILD2
  
  if ( file_copy("u8x8_fonts.pre", u8x8_fonts_filename) == 0 )
    return 0;
  if ( file_copy("u8g2_fonts.pre", u8g2_fonts_filename) == 0 )
    return 0;
  if ( file_copy("keywords.pre", "keywords.txt") == 0 )
    return 0;
  
  
  do_font_loop(bdfconv);
  
  u8g2_font_list_fp = fopen("u8g2_font_list.c", "w");
  u8x8_font_list_fp  = fopen("u8x8_font_list.c", "w");
  keywords_fp  = fopen("keywords.txt", "a");
  fprintf(u8g2_font_list_fp, "/* u8g2_font_list.c */\n");
  fprintf(u8x8_font_list_fp, "/* u8x8_font_list.c */\n");
  fprintf(u8g2_font_list_fp, "#include \"u8g2.h\"\n");
  fprintf(u8x8_font_list_fp, "#include \"u8g2.h\"\n");
  fprintf(u8g2_font_list_fp, "const uint8_t *u8g2_font_list[] = {\n");
  fprintf(u8x8_font_list_fp, "const uint8_t *u8x8_font_list[] = {\n");
  do_font_loop(fontlist_identifier);
  fprintf(u8g2_font_list_fp, "  NULL\n};\n");
  fprintf(u8x8_font_list_fp, "  NULL\n};\n");

  fprintf(u8g2_font_list_fp, "char *u8g2_font_names[] = {\n");
  fprintf(u8x8_font_list_fp, "char *u8x8_font_names[] = {\n");
  do_font_loop(fontlist_name);
  fprintf(u8g2_font_list_fp, "  NULL\n};\n");
  fprintf(u8x8_font_list_fp, "  NULL\n};\n");
  
  fclose(u8g2_font_list_fp);
  fclose(u8x8_font_list_fp);
  fclose(keywords_fp);

  printf("update u8g2.h\n");
  insert_into_file("../../../csrc/u8g2.h", u8g2_prototypes, "/* start font list */", "/* end font list */");
  printf("update u8x8.h\n");
  insert_into_file("../../../csrc/u8x8.h", u8x8_prototypes, "/* start font list */", "/* end font list */");

  unlink("font.c");
  
  
  do_font_groups(generate_font_group_md);
  
#endif

#ifdef BUILD2
  u8g2_fnt_cnt = 0;
  u8x8_fnt_cnt = 0;
  do_font_loop(overviewpic);
  u8g2_fnt_cnt = 0;
  u8x8_fnt_cnt = 0;
  do_font_loop(overviewshortpic);
  do_font_list(generate_font_list);

  
#endif

  return 0;
}
