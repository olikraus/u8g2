
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#ifdef BUILD2
#include "u8g2.h"
#endif

extern const uint8_t *u8x8_font_list[] ;
extern char *u8x8_font_names[] ;
extern const uint8_t *u8g2_font_list[] ;
extern char *u8g2_font_names[] ;

const char convert_extra_options[] = "-flip";   /* issue 2080: convert seems to flip the tga picture, again do the flip */

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
  char *bdfconv_opt;
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
  { "Open Iconic", 	"fntgrpiconic", 		"../../../../u8g2.wiki/fntgrpiconic.md", 		"fntgrpiconic.pre" }, 		/* 22 */  
  { "Persian", 	"fntgrppersian", 		"../../../../u8g2.wiki/fntgrppersian.md", 		"fntgrppersian.pre" }, 		/* 23 */  
  { "Tom-Thumb", 	"fntgrptomthumb", 		"../../../../u8g2.wiki/fntgrptomthumb.md", 		"fntgrptomthumb.pre" }, 	/* 24 */  
  { "Extant", 	"fntgrpextant", 		"../../../../u8g2.wiki/fntgrpextant.md", 		"fntgrpextant.pre" }, 		/* 25 */  
  { "MistressEllipsis", "fntgrpmistressellipsis", "../../../../u8g2.wiki/fntgrpmistressellipsis.md", 		"fntgrpmistressellipsis.pre" }, 		/* 26 */ 
  { "JayWright", "fntgrpjaywright", "../../../../u8g2.wiki/fntgrpjaywright.md", 		"fntgrpjaywright.pre" }, 		/* 27 */ 
  { "Angel", "fntgrpangel", "../../../../u8g2.wiki/fntgrpangel.md", 		"fntgrpangel.pre" }, 		/* 28 */ 
  { "JosephKnightcom", "fntgrpjosephknightcom", "../../../../u8g2.wiki/fntgrpjosephknightcom.md", 		"fntgrpjosephknightcom.pre" }, 		/* 29 */ 
  { "ChristinaAntoinetteNeofotistou", "fntgrpchristinaneofotistou", "../../../../u8g2.wiki/fntgrpchristinaneofotistou.md", 		"fntgrpchristinaneofotistou.pre" }, 		/* 30 */ 
  { "Geoff", "fntgrpgeoff", "../../../../u8g2.wiki/fntgrpgeoff.md", 		"fntgrpgeoff.pre" }, 		/* 31 */ 
  { "Tulamide", "fntgrptulamide", "../../../../u8g2.wiki/fntgrptulamide.md", 		"fntgrptulamide.pre" }, 		/* 32 */ 
  { "GilesBooth", "fntgrpgilesbooth", "../../../../u8g2.wiki/fntgrpgilesbooth.md", 		"fntgrpgilesbooth.pre" }, 		/* 33 */ 
  { "bitfontmaker2", "fntgrpbitfontmaker2", "../../../../u8g2.wiki/fntgrpbitfontmaker2.md", 		"fntgrpbitfontmaker2.pre" }, 		/* 34 */ 
  { "JapanYoshi", "fntgrpjapanyoshi", "../../../../u8g2.wiki/fntgrpjapanyoshi.md", 		"fntgrpjapanyoshi.pre" }, 		/* 35 */ 
  { "Pentacom", "fntgrppentacom", "../../../../u8g2.wiki/fntgrppentacom.md", 		"fntgrppentacom.pre" }, 		/* 36 */ 
  { "dafont", "fntgrpdafont", "../../../../u8g2.wiki/fntgrpdafont.md", 		"fntgrpdafont.pre" }, 		/* 37 */ 
  { "lucida", "fntgrplucida", "../../../../u8g2.wiki/fntgrplucida.md", 		"fntgrplucida.pre" }, 		/* 38 */ 
  { "streamline", 	"fntgrpstreamline", 		"../../../../u8g2.wiki/fntgrpstreamline.md", 		"fntgrpstreamline.pre" }, 		/* 39 */  
  { "Mystery Quest", 	"fntgrpmysteryquest", 		"../../../../u8g2.wiki/fntgrpmysteryquest.md", 		"fntgrpmysteryquest.pre" }, 		/* 40 */  
  { "HasanKazan", "fntgrphasankazan", "../../../../u8g2.wiki/fntgrphasankazan.md", 		"fntgrphasankazan.pre" }, 		/* 41 */ 
  { "Integrated Mapping Ltd", "fntgrpim", "../../../../u8g2.wiki/fntgrpim.md", 		"fntgrpim.pre" }, 		/* 42 */ 
  { "Spleen", "fntgrpspleen", "../../../../u8g2.wiki/fntgrpspleen.md", 		"fntgrpspleen.pre" }, 		/* 43 */ 

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
#define MM_E	64	/* extended 32-701,7838,8364 fb00..fb07	 */ /* added euro sign */


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
	
  { 0,  0, "u8glib_4.bdf", 		"u8glib_4", 		0, 0, BM_T|BM_H, FM_C, MM_F|MM_R, "", "" },

  
  { 0,  0, "m2icon_5.bdf", 		"m2icon_5", 		0, 0, BM_T, FM_C, MM_F, "", ""},
  { 0,  0, "m2icon_7.bdf", 		"m2icon_7", 		0, 0, BM_T, FM_C, MM_F, "", ""},
  { 0,  0, "m2icon_9.bdf", 		"m2icon_9", 		0, 0, BM_T, FM_C, MM_F, "", ""},
  { 0,  0, "emoticons21.bdf", 	"emoticons21", 	0, 0, BM_T, FM_C, MM_R, "", ""},
  { 0,  0, "battery19.bdf", 		"battery19", 		0, 0, BM_T, FM_C, MM_N, "", ""},
  { 0,  0, "battery24.bdf", 		"battery24", 		0, 0, BM_T, FM_C, MM_R, "", ""},

  { 0,  0, "u8g2_squeezed_regular_6.bdf", 		"squeezed_r6", 		0, 0, BM_T, FM_C, MM_R|MM_N, "", ""},
  { 0,  0, "u8g2_squeezed_bold_6.bdf", 		"squeezed_b6", 		0, 0, BM_T, FM_C, MM_R|MM_N, "", ""},
  { 0,  0, "u8g2_squeezed_regular_7.bdf", 		"squeezed_r7", 		0, 0, BM_T, FM_C, MM_R|MM_N, "", ""},
  { 0,  0, "u8g2_squeezed_bold_7.bdf", 		"squeezed_b7", 		0, 0, BM_T, FM_C, MM_R|MM_N, "", ""},
  
  { 0,  0, "u8g2_percent_circle_25.bdf", 		"percent_circle_25", 		0, 0, BM_H, FM_C, MM_N, "", ""},
  
  { 0,  0, "freedoomr10r.bdf", 	"freedoomr10", 	0, 0, BM_T|BM_M, FM_C, MM_U, "", ""},
  { 0,  0, "freedoomr25n.bdf", 	"freedoomr25", 	0, 0, BM_T|BM_M, FM_C, MM_N, "", ""},

  { 0,  0, "7Segments_26x42.bdf",  "7Segments_26x42", 	0, 0, BM_M, FM_C, MM_N, "", ""},

  { 0,  0, "7_Seg_33x19.bdf",  "7_Seg_33x19", 	0, 0, BM_M, FM_C, MM_N, "", ""},
  { 0,  0, "7_Seg_41x21.bdf",  "7_Seg_41x21", 	0, 0, BM_M, FM_C, MM_N, "", ""},

  { 0,  0, "font_tiny5.bdf", 		"tiny5", 		0, 0, BM_T, FM_C, MM_R|MM_F, "", ""},
  { 0,  "-r 72 -p 8", "04B_03B_.TTF",	"04b_03b", 0, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 8", "04B_03__.TTF",	"04b_03", 0, 0, BM_T, FM_C, MM_R, "", "" },

  
    
  { 0,  0, "amstrad_cpc_extended.bdf", 	"amstrad_cpc_extended", 	2, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},
  
  { 0,  0, "cursor.bdf", 			"cursor", 			1, 0, BM_T, FM_C, MM_C, "0-223>32", "f" },
  { 0,  0, "cursor.bdf", 			"cursor", 			1, 0, BM_T, FM_C, MM_C, "0-80>32", "r" },
  { 0,  0, "micro.bdf", 			"micro", 			1, 0, BM_T|BM_M, FM_C, MM_R|MM_N, "", "" },		/* micro does not have the full set */
  { 0,  0, "4x6.bdf", 			"4x6", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "4x6.bdf", 			"4x6", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "5x7.bdf", 			"5x7", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "5x7.bdf", 			"5x7", 			1, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "5x7.bdf", 			"5x7", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "5x8.bdf", 			"5x8", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "5x8.bdf", 			"5x8", 			1, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "5x8.bdf", 			"5x8", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "6x10.bdf", 			"6x10", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "6x12.bdf", 			"6x12", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "6x12.bdf", 			"6x12", 			1, 0, BM_T|BM_M, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0,  0, "6x12.bdf", 			"6x12", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "6x13.bdf", 			"6x13", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "6x13.bdf", 			"6x13", 			1, 0, BM_T, FM_C, MM_C, "32-128,$590-$5ff,$fb1d-$fb4f", "_hebrew" },
  { 0,  0, "6x13.bdf", 			"6x13", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "6x13B.bdf", 			"6x13B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "6x13B.bdf", 			"6x13B", 			1, 0, BM_T, FM_C, MM_C, "32-128,$590-$5ff,$fb1d-$fb4f", "_hebrew" },
  { 0,  0, "6x13B.bdf", 			"6x13B", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "6x13O.bdf", 			"6x13O", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "7x13.bdf", 			"7x13", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "7x13.bdf", 			"7x13", 			1, 0, BM_T|BM_M, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0,  0, "7x13.bdf", 			"7x13", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "7x13B.bdf", 			"7x13B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "7x13O.bdf", 			"7x13O", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "7x14.bdf", 			"7x14", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "7x14B.bdf", 			"7x14B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "8x13.bdf", 			"8x13", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "8x13.bdf", 			"8x13", 			1, 0, BM_T|BM_M, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0,  0, "8x13.bdf", 			"8x13", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "8x13B.bdf", 			"8x13B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "8x13O.bdf", 			"8x13O", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },

  { 0,  0, "9x15.bdf", 			"9x15", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "9x15.bdf", 			"9x15", 			1, 0, BM_T|BM_M, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0,  0, "9x15.bdf", 			"9x15", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "9x15B.bdf", 			"9x15B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "9x18.bdf", 			"9x18", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "9x18B.bdf", 			"9x18B", 			1, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "10x20.bdf", 			"10x20", 			1, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "10x20.bdf", 			"10x20", 			1, 0, BM_T, FM_C, MM_C, "32-128,$370-$3ff", "_greek" },
  { 0,  0, "10x20.bdf", 			"10x20", 			1, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "10x20.bdf", 			"10x20", 			1, 35, BM_T, FM_C, MM_C, "32-128,$600-$6ff,$FB50-$FBB1,$FE70-$FEFF,x32-64,x91-96,x123-191,x247,x697-879,x32-$5ff", "_arabic" },

  { "-y -2 -th 1 -tv 2",  0, "8x13.bdf",	"8x13_1x2",	1, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y -2 -th 1 -tv 2",  0, "8x13B.bdf",	"8x13B_1x2",	1, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y -1 -x -1 -th 1 -tv 2",  0, "7x14.bdf",	"7x14_1x2",	1, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y -1 -x -1 -th 1 -tv 2",  0, "7x14B.bdf",	"7x14B_1x2",	1, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  
  
  { 0,  0, "siji_with_6x10.bdf", 	"siji", 			20, 0, BM_T, FM_C, MM_C, "32-128,$e000-$ffff", "_6x10" },
  { 0,  0, "waffle-10.bdf", 	"waffle", 			20, 0, BM_T, FM_C, MM_C, "$0020-$ffff", "_all" },
  
  { 0,  0, "tom-thumb.bdf", 	"tom_thumb_4x6", 		24, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  0, "tom-thumb.bdf", 	"tom_thumb_4x6", 		24, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },

  /* Mystery Quest  */
    { 0,  "-a -r 72 -p 24", "mystery-quest.regular.ttf",	"mystery_quest_24",	40, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
    { 0,  "-a -r 72 -p 28", "mystery-quest.regular.ttf",	"mystery_quest_28",	40, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
    { 0,  "-a -r 72 -p 32", "mystery-quest.regular.ttf",	"mystery_quest_32",	40, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
    { 0,  "-a -r 72 -p 36", "mystery-quest.regular.ttf",	"mystery_quest_36",	40, 0, BM_T, FM_C, MM_N, "", "" },
    { 0,  "-a -r 72 -p 42", "mystery-quest.regular.ttf",	"mystery_quest_42",	40, 0, BM_T, FM_C, MM_N, "", "" },
    { 0,  "-a -r 72 -p 48", "mystery-quest.regular.ttf",	"mystery_quest_48",	40, 0, BM_T, FM_C, MM_N, "", "" },
    { 0,  "-a -r 72 -p 56", "mystery-quest.regular.ttf",	"mystery_quest_56",	40, 0, BM_T, FM_C, MM_N, "", "" },


  
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
  
  { 0,  0, "t0-11-uni.bdf", 		"t0_11", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-11-uni.bdf", 		"t0_11", 			19, 0, BM_T, FM_C, MM_C, "$0020-$FFF0", "_all" },
  { 0,  0, "t0-11b-uni.bdf", 		"t0_11b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-12-uni.bdf", 		"t0_12", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-12b-uni.bdf", 		"t0_12b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-13-uni.bdf", 		"t0_13", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-13b-uni.bdf", 		"t0_13b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-14-uni.bdf", 		"t0_14", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-14b-uni.bdf", 		"t0_14b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-15-uni.bdf", 		"t0_15", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-15b-uni.bdf", 		"t0_15b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-16-uni.bdf", 		"t0_16", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-16b-uni.bdf", 		"t0_16b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-17-uni.bdf", 		"t0_17", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-17b-uni.bdf", 		"t0_17b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-18-uni.bdf", 		"t0_18", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-18b-uni.bdf", 		"t0_18b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-22-uni.bdf", 		"t0_22", 			19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "t0-22b-uni.bdf", 		"t0_22b", 		19, 0, BM_T|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },

  
  { 0,  0, "open_iconic_all_1x.bdf", "open_iconic_all_1x",  22, 0, BM_T, FM_C, MM_C, "32-400", "" },
  { 0,  0, "open_iconic_app_1x.bdf", "open_iconic_app_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_arrow_1x.bdf", "open_iconic_arrow_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_check_1x.bdf", "open_iconic_check_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_email_1x.bdf", "open_iconic_email_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_embedded_1x.bdf", "open_iconic_embedded_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_gui_1x.bdf", "open_iconic_gui_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_human_1x.bdf", "open_iconic_human_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_mime_1x.bdf", "open_iconic_mime_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_other_1x.bdf", "open_iconic_other_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_play_1x.bdf", "open_iconic_play_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_text_1x.bdf", "open_iconic_text_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_thing_1x.bdf", "open_iconic_thing_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_weather_1x.bdf", "open_iconic_weather_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_www_1x.bdf", "open_iconic_www_1x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },

  { "-th 1 -tv 1",  0, "open_iconic_arrow_1x.bdf",		"open_iconic_arrow_1x1",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 1 -tv 1",  0, "open_iconic_check_1x.bdf",		"open_iconic_check_1x1",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 1 -tv 1",  0, "open_iconic_embedded_1x.bdf",	"open_iconic_embedded_1x1",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 1 -tv 1",  0, "open_iconic_play_1x.bdf",		"open_iconic_play_1x1",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 1 -tv 1",  0, "open_iconic_thing_1x.bdf",		"open_iconic_thing_1x1",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 1 -tv 1",  0, "open_iconic_weather_1x.bdf",		"open_iconic_weather_1x1",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  

  
  { 0,  0, "open_iconic_all_2x.bdf", "open_iconic_all_2x",  22, 0, BM_T, FM_C, MM_C, "32-400", "" },
  { 0,  0, "open_iconic_app_2x.bdf", "open_iconic_app_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_arrow_2x.bdf", "open_iconic_arrow_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_check_2x.bdf", "open_iconic_check_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_email_2x.bdf", "open_iconic_email_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_embedded_2x.bdf", "open_iconic_embedded_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_gui_2x.bdf", "open_iconic_gui_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_human_2x.bdf", "open_iconic_human_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_mime_2x.bdf", "open_iconic_mime_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_other_2x.bdf", "open_iconic_other_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_play_2x.bdf", "open_iconic_play_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_text_2x.bdf", "open_iconic_text_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_thing_2x.bdf", "open_iconic_thing_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_weather_2x.bdf", "open_iconic_weather_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_www_2x.bdf", "open_iconic_www_2x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },  
  
  { "-th 2 -tv 2",  0, "open_iconic_arrow_2x.bdf",		"open_iconic_arrow_2x2",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 2 -tv 2",  0, "open_iconic_check_2x.bdf",		"open_iconic_check_2x2",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 2 -tv 2",  0, "open_iconic_embedded_2x.bdf",	"open_iconic_embedded_2x2",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 2 -tv 2",  0, "open_iconic_play_2x.bdf",		"open_iconic_play_2x2",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 2 -tv 2",  0, "open_iconic_thing_2x.bdf",		"open_iconic_thing_2x2",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 2 -tv 2",  0, "open_iconic_weather_2x.bdf",		"open_iconic_weather_2x2",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  
  
  { 0,  0, "open_iconic_all_4x.bdf", "open_iconic_all_4x",  22, 0, BM_T, FM_C, MM_C, "32-400", "" },
  { 0,  0, "open_iconic_app_4x.bdf", "open_iconic_app_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_arrow_4x.bdf", "open_iconic_arrow_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_check_4x.bdf", "open_iconic_check_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_email_4x.bdf", "open_iconic_email_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_embedded_4x.bdf", "open_iconic_embedded_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_gui_4x.bdf", "open_iconic_gui_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_human_4x.bdf", "open_iconic_human_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_mime_4x.bdf", "open_iconic_mime_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_other_4x.bdf", "open_iconic_other_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_play_4x.bdf", "open_iconic_play_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_text_4x.bdf", "open_iconic_text_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_thing_4x.bdf", "open_iconic_thing_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_weather_4x.bdf", "open_iconic_weather_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_www_4x.bdf", "open_iconic_www_4x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },

  { "-th 4 -tv 4",  0, "open_iconic_arrow_4x.bdf",		"open_iconic_arrow_4x4",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 4 -tv 4",  0, "open_iconic_check_4x.bdf",		"open_iconic_check_4x4",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 4 -tv 4",  0, "open_iconic_embedded_4x.bdf",	"open_iconic_embedded_4x4",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 4 -tv 4",  0, "open_iconic_play_4x.bdf",		"open_iconic_play_4x4",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 4 -tv 4",  0, "open_iconic_thing_4x.bdf",		"open_iconic_thing_4x4",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 4 -tv 4",  0, "open_iconic_weather_4x.bdf",		"open_iconic_weather_4x4",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  


  { 0,  0, "open_iconic_all_6x.bdf", "open_iconic_all_6x",  22, 0, BM_T, FM_C, MM_C, "32-400", "" },
  { 0,  0, "open_iconic_app_6x.bdf", "open_iconic_app_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_arrow_6x.bdf", "open_iconic_arrow_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_check_6x.bdf", "open_iconic_check_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_email_6x.bdf", "open_iconic_email_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_embedded_6x.bdf", "open_iconic_embedded_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_gui_6x.bdf", "open_iconic_gui_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_human_6x.bdf", "open_iconic_human_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_mime_6x.bdf", "open_iconic_mime_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_other_6x.bdf", "open_iconic_other_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_play_6x.bdf", "open_iconic_play_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_text_6x.bdf", "open_iconic_text_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_thing_6x.bdf", "open_iconic_thing_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_weather_6x.bdf", "open_iconic_weather_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_www_6x.bdf", "open_iconic_www_6x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },

  { 0,  0, "open_iconic_all_8x.bdf", "open_iconic_all_8x",  22, 0, BM_T, FM_C, MM_C, "32-400", "" },
  { 0,  0, "open_iconic_app_8x.bdf", "open_iconic_app_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_arrow_8x.bdf", "open_iconic_arrow_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_check_8x.bdf", "open_iconic_check_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_email_8x.bdf", "open_iconic_email_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_embedded_8x.bdf", "open_iconic_embedded_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_gui_8x.bdf", "open_iconic_gui_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_human_8x.bdf", "open_iconic_human_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_mime_8x.bdf", "open_iconic_mime_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_other_8x.bdf", "open_iconic_other_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_play_8x.bdf", "open_iconic_play_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_text_8x.bdf", "open_iconic_text_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_thing_8x.bdf", "open_iconic_thing_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_weather_8x.bdf", "open_iconic_weather_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },
  { 0,  0, "open_iconic_www_8x.bdf", "open_iconic_www_8x",  22, 0, BM_T, FM_C, MM_C, "32-300", "" },

  { "-th 8 -tv 8",  0, "open_iconic_arrow_8x.bdf",		"open_iconic_arrow_8x8",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 8 -tv 8",  0, "open_iconic_check_8x.bdf",		"open_iconic_check_8x8",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 8 -tv 8",  0, "open_iconic_embedded_8x.bdf",	"open_iconic_embedded_8x8",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 8 -tv 8",  0, "open_iconic_play_8x.bdf",		"open_iconic_play_8x8",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 8 -tv 8",  0, "open_iconic_thing_8x.bdf",		"open_iconic_thing_8x8",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  
  { "-th 8 -tv 8",  0, "open_iconic_weather_8x.bdf",		"open_iconic_weather_8x8",	22, 0, BM_8, FM_8, MM_C, "32-255", "" },  


  { 0, 0, "streamline_all.bdf", "streamline_all", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_building_real_estate.bdf", "streamline_building_real_estate", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_business.bdf", "streamline_business", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_coding_apps_websites.bdf", "streamline_coding_apps_websites", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_computers_devices_electronics.bdf", "streamline_computers_devices_electronics", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_content_files.bdf", "streamline_content_files", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_design.bdf", "streamline_design", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_ecology.bdf", "streamline_ecology", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_email.bdf", "streamline_email", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_entertainment_events_hobbies.bdf", "streamline_entertainment_events_hobbies", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_food_drink.bdf", "streamline_food_drink", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_hand_signs.bdf", "streamline_hand_signs", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_health_beauty.bdf", "streamline_health_beauty", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_action.bdf", "streamline_interface_essential_action", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_alert.bdf", "streamline_interface_essential_alert", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_audio.bdf", "streamline_interface_essential_audio", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_calendar.bdf", "streamline_interface_essential_calendar", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_chart.bdf", "streamline_interface_essential_chart", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_circle_triangle.bdf", "streamline_interface_essential_circle_triangle", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_cog.bdf", "streamline_interface_essential_cog", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_cursor.bdf", "streamline_interface_essential_cursor", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_dial_pad.bdf", "streamline_interface_essential_dial_pad", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_edit.bdf", "streamline_interface_essential_edit", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_expand_shrink.bdf", "streamline_interface_essential_expand_shrink", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_eye.bdf", "streamline_interface_essential_eye", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_file.bdf", "streamline_interface_essential_file", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_help.bdf", "streamline_interface_essential_help", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_hierarchy.bdf", "streamline_interface_essential_hierarchy", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_home_menu.bdf", "streamline_interface_essential_home_menu", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_id.bdf", "streamline_interface_essential_id", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_key_lock.bdf", "streamline_interface_essential_key_lock", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_link.bdf", "streamline_interface_essential_link", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_loading.bdf", "streamline_interface_essential_loading", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_login.bdf", "streamline_interface_essential_login", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_other.bdf", "streamline_interface_essential_other", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_paginate.bdf", "streamline_interface_essential_paginate", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_search.bdf", "streamline_interface_essential_search", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_setting.bdf", "streamline_interface_essential_setting", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_share.bdf", "streamline_interface_essential_share", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_text.bdf", "streamline_interface_essential_text", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_wifi.bdf", "streamline_interface_essential_wifi", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_interface_essential_zoom.bdf", "streamline_interface_essential_zoom", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_internet_network.bdf", "streamline_internet_network", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_logo.bdf", "streamline_logo", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_map_navigation.bdf", "streamline_map_navigation", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_money_payments.bdf", "streamline_money_payments", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_music_audio.bdf", "streamline_music_audio", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_pet_animals.bdf", "streamline_pet_animals", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_phone.bdf", "streamline_phone", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_photography.bdf", "streamline_photography", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_romance.bdf", "streamline_romance", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_school_science.bdf", "streamline_school_science", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_shopping_shipping.bdf", "streamline_shopping_shipping", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_social_rewards.bdf", "streamline_social_rewards", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_technology.bdf", "streamline_technology", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_transportation.bdf", "streamline_transportation", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_travel_wayfinding.bdf", "streamline_travel_wayfinding", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_users.bdf", "streamline_users", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_video_movies.bdf", "streamline_video_movies", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  { 0, 0, "streamline_weather.bdf", "streamline_weather", 39, 0, BM_T, FM_C, MM_C, "48-999", "" }, 
  
  
  { 0,  0, "profont10.bdf", 		"profont10", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "profont11.bdf", 		"profont11", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "profont12.bdf", 		"profont12", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "profont15.bdf", 		"profont15", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "profont17.bdf", 		"profont17", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "profont22.bdf", 		"profont22", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "profont29.bdf", 		"profont29", 		4, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  
  { "-y 0 -th 2 -tv 3",  0, "profont29.bdf",	"profont29_2x3",	4, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},

  
  /* Persian */
    { 0,  "-a -r 72 -p 10", "Samim.ttf",	"samim_10",						23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-a -r 72 -p 12", "Samim.ttf",	"samim_12",						23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-a -r 72 -p 14", "Samim.ttf",	"samim_14",						23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-a -r 72 -p 16", "Samim.ttf",	"samim_16",						23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },

    { 0,  "-a -r 72 -p 10", "Samim-FD.ttf",	"samim_fd_10",				23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-a -r 72 -p 12", "Samim-FD.ttf",	"samim_fd_12",				23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-a -r 72 -p 14", "Samim-FD.ttf",	"samim_fd_14",				23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-a -r 72 -p 16", "Samim-FD.ttf",	"samim_fd_16",				23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },

    { 0,  "-r 72 -p 10", "GanjNamehSans-Regular.ttf",	"ganj_nameh_sans10",	23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-r 72 -p 12", "GanjNamehSans-Regular.ttf",	"ganj_nameh_sans12",	23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-r 72 -p 14", "GanjNamehSans-Regular.ttf",	"ganj_nameh_sans14",	23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-r 72 -p 16", "GanjNamehSans-Regular.ttf",	"ganj_nameh_sans16",	23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },

    { 0,  "-r 72 -p 8", "IranianSansRegular.ttf",	"iranian_sans_8",			23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-r 72 -p 10", "IranianSansRegular.ttf",	"iranian_sans_10",			23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-r 72 -p 12", "IranianSansRegular.ttf",	"iranian_sans_12",			23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-r 72 -p 14", "IranianSansRegular.ttf",	"iranian_sans_14",			23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },
    { 0,  "-r 72 -p 16", "IranianSansRegular.ttf",	"iranian_sans_16",			23, 0, BM_T, FM_C, MM_C, "32-65500", "_all" },

    /* NBP */
  { 0,  "-r 72 -p 16", "MOZART_0.ttf",	"mozart_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "MOZART_0.ttf",	"mozart_nbp",			18, 0, BM_T|BM_H, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "glasstown_nbp.ttf",	"glasstown_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "glasstown_nbp.ttf",	"glasstown_nbp",	18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "shylock_nbp.ttf",	"shylock_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "shylock_nbp.ttf",	"shylock_nbp",			18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "RENT_0.ttf",	"roentgen_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "RENT_0.ttf",	"roentgen_nbp",			18, 0, BM_T|BM_H, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "CALIBRATE1.ttf",	"calibration_gothic_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "CALIBRATE1.ttf",	"calibration_gothic_nbp",	18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "smart_patrol_nbp.ttf",	"smart_patrol_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "prospero_bold_nbp.ttf", "prospero_bold_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "prospero_nbp.ttf",	"prospero_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "BALRG_0.ttf",	"balthasar_regular_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 32", "BALTT_0.ttf",	"balthasar_titling_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 8", "synchronizer_nbp.ttf",	"synchronizer_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "mercutio_basic.ttf", "mercutio_basic_nbp",	18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "mercutio_basic.ttf", "mercutio_basic_nbp",	18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "mercutio_sc.ttf", "mercutio_sc_nbp",		18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "mercutio_sc.ttf", "mercutio_sc_nbp",		18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "MIRANDA.ttf",	"miranda_nbp",		18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "nine0.ttf", "nine_by_five_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "nine0.ttf", "nine_by_five_nbp",			18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "ROSEN_0.ttf", "rosencrantz_nbp",		18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "ROSEN_0.ttf", "rosencrantz_nbp",		18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "GUILD_0.ttf", "guildenstern_nbp",		18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "GUILD_0.ttf", "guildenstern_nbp",		18, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "astra0.ttf",	"astragal_nbp",			18, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },

    /* Extant, 25 */
  { 0,  "-r 72 -p 16", "HabsburgChancery.ttf",	"habsburgchancery",	25, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "HabsburgChancery.ttf",	"habsburgchancery",	25, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "MissingPlanet.ttf",	"missingplanet",	25, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "MissingPlanet.ttf",	"missingplanet",	25, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "OrdinaryBasis.ttf",	"ordinarybasis",	25, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "OrdinaryBasis.ttf",	"ordinarybasis",	25, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "PixelMordred.ttf",	"pixelmordred",	25, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "PixelMordred.ttf",	"pixelmordred",	25, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "SecretaryHand.ttf",	"secretaryhand",	25, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "SecretaryHand.ttf",	"secretaryhand",	25, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "GarbageCan.ttf",	"garbagecan",	25, 0, BM_T, FM_C, MM_F|MM_R, "", "" },


  /* MistressEllipsis, 26 */
  { 0,  "-r 72 -p 16", "Beanstalk.ttf",	"beanstalk_mel",	26, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "Cube.ttf",	"cube_mel",	26, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "Mademoiselle.ttf",	"mademoiselle_mel",	26, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "PieceOfCake.ttf",	"pieceofcake_mel",	26, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "Press.ttf",	"press_mel",	26, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "RePress.ttf",	"repress_mel",	26, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "Sticker.ttf",	"sticker_mel",	26, 0, BM_T, FM_C, MM_R|MM_N, "", "" },


  /* JayWright, 27 */
  { 0,  "-r 72 -p 16", "CelibateMonk.ttf",	"celibatemonk",		27, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "DisrespectfulTeenager.ttf",	"disrespectfulteenager",	27, 0, BM_T, FM_C, MM_U, "", "" },
  { 0,  "-r 72 -p 16", "MichaelMouse.ttf",	"michaelmouse",		27, 0, BM_T, FM_C, MM_U, "", "" },
  { 0,  "-r 72 -p 16", "SandyForest.ttf",	"sandyforest",			27, 0, BM_T, FM_C, MM_R|MM_U|MM_N, "", "" },

  /* Angel, 28 */
  { 0,  "-r 72 -p 16", "CupcakeMeToYourLeader.ttf",	"cupcakemetoyourleader",	28, 0, BM_T, FM_C, MM_R|MM_U|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "OldWizard.ttf",			"oldwizard",				28, 0, BM_T, FM_C, MM_F|MM_R|MM_U|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "Squirrel.ttf",				"squirrel",				28, 0, BM_T, FM_C, MM_R|MM_U|MM_N, "", "" },
  
  /* JosephKnightcom, 29 */
  { 0,  "-r 72 -p 16", "DiodeSemiMono.ttf",	"diodesemimono",	29, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Questgiver.ttf",		"questgiver",		29, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Seraphimb1.ttf",		"seraphimb1",		29, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 9", "ResoledBold.ttf",		"resoledbold",		29, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 9", "ResoledMedium.ttf",	"resoledmedium",		29, 0, BM_T, FM_C, MM_R, "", "" },

  /* ChristinaAntoinetteNeofotistou, 30 */
  { 0,  "-r 72 -p 16", "JinxedWizards.ttf",	"jinxedwizards",	30, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "LastPriestess.ttf",	"lastpriestess",		30, 0, BM_T, FM_C, MM_U|MM_R, "", "" },

  /* Geoff, 31 */
  { 0,  "-r 72 -p 16", "BitCasual.ttf",			"bitcasual",	31, 0, BM_T, FM_C, MM_N|MM_U|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "BitCasual.ttf",			"bitcasual",	31, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "Koleeko.ttf",			"koleeko",	31, 0, BM_T, FM_C, MM_N|MM_U|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "TenFatGuys.ttf",		"tenfatguys",	31, 0, BM_T, FM_C, MM_N|MM_U|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "TenFatGuys.ttf",		"tenfatguys",	31, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "TenStamps.ttf",		"tenstamps",	31, 0, BM_M, FM_C, MM_N|MM_U|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "TenThinGuys.ttf",		"tenthinguys",	31, 0, BM_T, FM_C, MM_N|MM_U|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "TenThinGuys.ttf",		"tenthinguys",	31, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "TenThinnerGuys.ttf",	"tenthinnerguys",	31, 0, BM_T, FM_C, MM_N|MM_U|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "TenThinnerGuys.ttf",	"tenthinnerguys",	31, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "TwelveDings.ttf",		"twelvedings",	31, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },

  { 0,  "-r 72 -p 16", "Frigidaire.ttf",		"frigidaire",	31, 0, BM_M, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Lord.ttf",		"lord",	31, 0, BM_M, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Abel.ttf",		"abel",	31, 0, BM_M, FM_C, MM_R, "", "" },
 

  

  /* tulamide, 32 */
  { 0,  "-r 72 -p 16", "Fewture.ttf",			"fewture",	32, 0, BM_T, FM_C, MM_N|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "Halftone.ttf",			"halftone",	32, 0, BM_T, FM_C, MM_N|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "Nerhoe.ttf",			"nerhoe",	32, 0, BM_T, FM_C, MM_N|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "Oskool.ttf",			"oskool",	32, 0, BM_T, FM_C, MM_N|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "TinyTim.ttf",			"tinytim",	32, 0, BM_T, FM_C, MM_N|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "TooseOrnament.ttf",	"tooseornament",	32, 0, BM_T, FM_C, MM_N|MM_R|MM_F, "", "" },
  
  /* GilesBooth, 33*/
  { 0,  "-r 72 -p 16", "Bauhaus2015.ttf",			"bauhaus2015",	33, 0, BM_T, FM_C, MM_N|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "FindersKeepers.ttf",			"finderskeepers",	33, 0, BM_T, FM_C, MM_N|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "SirClivetheBold.ttf",			"sirclivethebold",	33, 0, BM_T, FM_C, MM_N|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "SirClive.ttf",				"sirclive",	33, 0, BM_T, FM_C, MM_N|MM_R, "", "" },
  
  /* bm2, 34*/
  { 0,  "-r 72 -p 16", "Adventurer.ttf",			"adventurer",	34, 0, BM_T, FM_C, MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "Adventurer.ttf",			"adventurer",	34, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "BracketedBabies.ttf",		"bracketedbabies",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Frikativ.ttf",				"frikativ",	34, 0, BM_T, FM_C, MM_F|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Frikativ.ttf",				"frikativ",	34, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "FancyPixels.ttf",			"fancypixels",	34, 0, BM_T, FM_C, MM_F|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "HEAVYBOTTOM.ttf",			"heavybottom",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "IconQuadPix.ttf",			"iconquadpix",	34, 0, BM_M, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "Tallpix.ttf",				"tallpix",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "botmaker.ttf",				"botmaker",	34, 0, BM_T, FM_C, MM_E, "", "" },
  { 0,  "-r 72 -p 16", "EfraneExtraCondensed.ttf",		"efraneextracondensed",	34, 0, BM_T, FM_C, MM_E, "", "" },
  
  /* Capital A 3 */
  { 0,  "-r 72 -p 16", "Minimal3x3.ttf",		                "minimal3x3",	34, 0, BM_T, FM_C, MM_U, "", "" },
  { 0,  "-r 72 -p 16", "3x3Basic.ttf",		                 "3x3basic",	34, 0, BM_T, FM_C, MM_R, "", "" },
  
  /* Capital A 4 */
  { 0,  "-r 72 -p 16", "tinyByGK.ttf",		                "tiny_gk",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "ThreePix.ttf",		                "threepix",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "eventhrees.ttf",		                 "eventhrees",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Fourmat.ttf",		                        "fourmat",	        34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  
  /* Capital A 5 */
  { 0,  "-r 72 -p 7", "tinyBySimon.ttf",		                "tiny_simon",	34, 0, BM_T, FM_C, MM_R, "", "" },  /* fixed, -p16 is wrong */
  { 0,  "-r 72 -p 7", "tinyBySimon.ttf",		                "tiny_simon",	34, 0, BM_M, FM_C, MM_R, "", "" },  /* fixed, -p16 is wrong */
  { 0,  "-r 72 -p 16", "smolFont.ttf",		                "smolfont",	34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "TinyUnicode.ttf",		                "tinyunicode",	34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "MicroPixel.ttf",		                "micropixel",	34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 6", "TinyPixie2.ttf",		                "tinypixie2",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Standardized3x5.ttf",		         "standardized3x5",	34, 0, BM_T, FM_C, MM_R, "", "" },  
  { 0,  "-r 72 -p 8", "fivepx.ttf",		                        "fivepx",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 10", "3x5.ttf",		                "3x5im",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 10", "3x5.ttf",		                "3x5im",	        34, 0, BM_M, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Wedge.ttf",		                        "wedge",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 10", "Kibibyte.ttf",		                "kibibyte",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "TinyFace.ttf",		                "tinyface",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },

  /* other */
  { 0,  "-r 72 -p 16", "SmallSimple.ttf",		                "smallsimple",	34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Simple1.ttf",		                "simple1",	34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },  
  { 0,  "-r 72 -p 16", "LikeMinecraft.ttf",		                "likeminecraft",	34, 0, BM_T, FM_C, MM_E, "", "" },
  { 0,  "-r 72 -p 16", "MedSans.ttf",		                "medsans",	        34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "HeiSans.ttf",		                        "heisans",	        34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "OriginalSans.ttf",		                "originalsans",	34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Minicute.ttf",		                "minicute",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Scrum.ttf",		                        "scrum",	        34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "StylishCharm.ttf",		                "stylishcharm",	34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "SisterSerif.ttf",		                "sisterserif",	        34, 0, BM_T, FM_C, MM_R, "", "" },

  { 0,  "-r 72 -p 16", "Princess.ttf",		                        "princess",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Dystopia.ttf",		                "dystopia",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },

  { 0,  "-r 72 -p 16", "LastApprenticeThin.ttf",                "lastapprenticethin",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "LastApprenticeBold.ttf",                "lastapprenticebold",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },

  { 0,  "-r 72 -p 16", "BPixel.ttf",		                        "bpixel",	                        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "BPixelDouble.ttf",		                "bpixeldouble",	        34, 0, BM_T, FM_C, MM_R, "", "" },
  

  { 0,  "-r 72 -p 16", "Mildras.ttf",		                "mildras",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },

  { 0,  "-r 72 -p 16", "MinuteConsole.ttf",		"minuteconsole",	        34, 0, BM_M, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "MinuteConsole.ttf",		"minuteconsole",	        34, 0, BM_T, FM_C, MM_R, "", "" },
  

  { 0,  "-r 72 -p 16", "BusDisplay11x5.ttf",		                "busdisplay11x5",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "BusDisplay8x5.ttf",		                "busdisplay8x5",	        34, 0, BM_T, FM_C, MM_R, "", "" },

  { 0,  "-r 72 -p 16", "Sticker100Complete.ttf",		                "sticker100complete",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },

  { 0,  "-r 72 -p 14", "DoomAlpha04.ttf",		                        "doomalpha04",	                        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },

  { 0,  "-r 72 -p 16", "GreenBloodSerif2.ttf",		                "greenbloodserif2",	        34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Eckpixel.ttf",		                "eckpixel",	        34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Elispe.ttf",		                "elispe",	        34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Neuecraft.ttf",		                "neuecraft",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },


  { 0,  "-r 72 -p 16", "8bitClassic.ttf",		                "8bitclassic",	                34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "LittleMissLoudonBold.ttf",		 "littlemissloudonbold",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Commodore64.ttf",		 "commodore64",	                34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "NEW3x9PixelFont.ttf",		 "new3x9pixelfont",	        34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },

  { 0,  "-r 72 -p 16", "SonicMania.ttf",		 "sonicmania",	        34, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "ByteSize.ttf",		 "bytesize",	        34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "PixzillaV1.ttf",		 "pixzillav1",	        34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Ciircle13.ttf",		 "ciircle13",	                34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "PxClassic.ttf",		 "pxclassic",	        34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },

  { 0,  "-r 72 -p 16", "MooseNooks.ttf",		 "moosenooks",	                34, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "TallPixelExtended.ttf",		 "tallpixelextended",	        34, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  
  
  /* JapanYoshi, 35*/
  { 0,  "-r 72 -p 16", "BBSesque.ttf",			"BBSesque",			35, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Born2bSportySlab.ttf",		"Born2bSportySlab",	35, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Born2bSportySlab.ttf",		"Born2bSportySlab",	35, 0, BM_T, FM_C, MM_C, "32-$ffff", "_all" },
  { 0,  "-r 72 -p 16", "Born2bSportyV2.ttf",		"Born2bSportyV2",		35, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "CursivePixel.ttf",			"CursivePixel",	35, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Engrish.ttf",				"Engrish",	35, 0, BM_T, FM_C, MM_F|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "ImpactBits.ttf",			"ImpactBits",	35, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "IPAandRUSLCD.ttf",			"IPAandRUSLCD",	35, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "PixelTheatre.ttf",			"PixelTheatre",	35, 0, BM_T, FM_C, MM_R|MM_E, "", "" },


  /* Pentacom, 36*/
  { 0,  "-r 72 -p 16", "HelvetiPixel.ttf",			"HelvetiPixel",			36, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "TimesNewPixel.ttf",			"TimesNewPixel",		36, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "BitTypeWriter.ttf",			"BitTypeWriter",		36, 0, BM_T, FM_C, MM_R	|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Georgia7px.ttf",			"Georgia7px",			36, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Wizzard.ttf",				"Wizzard",			36, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "HelvetiPixelOutline.ttf",		"HelvetiPixelOutline",	36, 0, BM_T, FM_C, MM_R|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "Untitled16PixelSansSerifBitmapTestFont.ttf",	"Untitled16PixelSansSerifBitmap",	36, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "UnnamedDOSFontIV.ttf",	"UnnamedDOSFontIV",	36, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "Terminal.ttf",	"Terminal",	36, 0, BM_T, FM_C, MM_R|MM_E, "", "" },

      /* HasanKazan, 41 */
  { 0,  "-r 72 -p 16", "NokiaLargeBold.ttf",	"NokiaLargeBold",	41, 0, BM_T, FM_C, MM_E|MM_F|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "NokiaSmallBold.ttf",	"NokiaSmallBold",	41, 0, BM_T, FM_C, MM_E|MM_F|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "NokiaSmallPlain.ttf",	"NokiaSmallPlain",	41, 0, BM_T, FM_C, MM_E|MM_F|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "12x6LED.ttf",	"12x6LED",	41, 0, BM_T, FM_C, MM_F|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "12x6LED.ttf",	"12x6LED",	41, 0, BM_M, FM_C, MM_N, "", "" },
  { 0,  "-r 72 -p 16", "9x6LED.ttf",	        "9x6LED",	41, 0, BM_T, FM_C, MM_F|MM_R, "", "" },
  { 0,  "-r 72 -p 16", "9x6LED.ttf",	        "9x6LED",	41, 0, BM_M, FM_C, MM_N, "", "" },

      /* Integrated Mapping Ltd, 42 */
  { 0,  0, "CalBlk36.bdf", 		"calblk36", 		42, 0, BM_T, FM_C, MM_R, "", "" },  
  { 0,  0, "CalLite24.bdf", 		"callite24", 		42, 0, BM_T, FM_C, MM_R, "", "" },  

        /* Spleen, https://github.com/olikraus/u8g2/issues/2015 , 43 */
  /* spleen-12x24.bdf  spleen-16x32.bdf  spleen-32x64.bdf  spleen-5x8.bdf  spleen-6x12.bdf  spleen-8x16.bdf */
  { 0,  0, "spleen-5x8.bdf", 		"spleen5x8", 		43, 0, BM_M, FM_C, MM_N|MM_U|MM_R|MM_F|MM_E, "", "" },  
  { 0,  0, "spleen-6x12.bdf", 		"spleen6x12", 		43, 0, BM_M, FM_C, MM_N|MM_U|MM_R|MM_F|MM_E, "", "" },  
  { 0,  0, "spleen-8x16.bdf", 		"spleen8x16", 		43, 0, BM_M, FM_C, MM_N|MM_U|MM_R|MM_F|MM_E, "", "" },  
  { 0,  0, "spleen-12x24.bdf", 		"spleen12x24", 		43, 0, BM_M, FM_C, MM_N|MM_U|MM_R|MM_F|MM_E, "", "" },  
  { 0,  0, "spleen-16x32.bdf", 		"spleen16x32", 		43, 0, BM_M, FM_C, MM_N|MM_U|MM_R|MM_F|MM_E, "", "" },  
  { 0,  0, "spleen-32x64.bdf", 		"spleen32x64", 		43, 0, BM_M, FM_C, MM_N|MM_U|MM_R|MM_F|MM_E, "", "" },  

//#endif   /* TMP */

  /* dafont, 37*/
  { 0,  "-r 72 -p 8", "nokiafc22.ttf",			"nokiafc22",		37, 0, BM_T, FM_C, MM_N|MM_U|MM_R|MM_F, "", "" },
  { 0,  "-a -r 72 -p 20", "VCR_OSD_MONO_1.001.ttf",	"VCR_OSD",		37, 0, BM_T|BM_M, FM_C, MM_N|MM_U|MM_R|MM_F, "", "" },
  { 0,  "-r 72 -p 16", "Pixellari.ttf",				"Pixellari",		37, 0, BM_T, FM_C, MM_N|MM_U|MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 10", "pixelpoiiz.ttf",				"pixelpoiiz",		37, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "DigitalDisco-Thin.ttf",		"DigitalDiscoThin",	37, 0, BM_T, FM_C, MM_N|MM_U|MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "DigitalDisco.ttf",			"DigitalDisco",		37, 0, BM_T, FM_C, MM_N|MM_U|MM_R|MM_F|MM_E, "", "" },
  { 0,  "-r 72 -p 16", "pearfont.ttf",				"pearfont",		37, 0, BM_T, FM_C, MM_R, "", "" },

//#ifdef TMP  
  
  /* thai fonts are not unicode encoded, so map the thai chars to their correct unicode position 128-255>3552 */
  { 0,  0, "etl14-thai.bdf", 		"etl14thai", 		17, 0, BM_T, FM_C, MM_C, "32-127,128-255>3552", "" },
  { 0,  0, "etl16-thai.bdf", 		"etl16thai", 		17, 0, BM_T, FM_C, MM_C, "32-127,128-255>3552", "" },
  { 0,  0, "etl24-thai.bdf", 		"etl24thai", 		17, 0, BM_T, FM_C, MM_C, "32-127,128-255>3552", "" },


  
  
  /* crox fonts are CP1251 encoded */
  { 0,  0, "win_crox1cb.bdf",		"crox1cb",		15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox1c.bdf",		"crox1c",			15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox1hb.bdf",		"crox1hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox1h.bdf",		"crox1h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox1tb.bdf",		"crox1tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox1t.bdf",		"crox1t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox2cb.bdf",		"crox2cb",		15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox2c.bdf",		"crox2c",			15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox2hb.bdf",		"crox2hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox2h.bdf",		"crox2h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox2tb.bdf",		"crox2tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox2t.bdf",		"crox2t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox3cb.bdf",		"crox3cb",		15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox3c.bdf",		"crox3c",			15,0,BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox3hb.bdf",		"crox3hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox3h.bdf",		"crox3h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox3tb.bdf",		"crox3tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox3t.bdf",		"crox3t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox4hb.bdf",		"crox4hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox4h.bdf",		"crox4h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox4tb.bdf",		"crox4tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox4t.bdf",		"crox4t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox5hb.bdf",		"crox5hb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox5h.bdf",		"crox5h",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox5tb.bdf",		"crox5tb",		15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "win_crox5t.bdf",		"crox5t",			15,0,BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },

  
  { 0,  0, "cu12.bdf", 			"cu12", 			3, 0, BM_T|BM_H|BM_M, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "cu12.bdf", 			"cu12", 			3, 0, BM_T|BM_H, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  { 0,  0, "cu12.bdf", 			"cu12", 			3, 0, BM_T, FM_C, MM_C, "32-128,$370-$3ff", "_greek" },
  { 0,  0, "cu12.bdf", 			"cu12", 			3, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "cu12.bdf", 			"cu12", 			3, 0, BM_T, FM_C, MM_C, "32-128,$f00-$fff", "_tibetan" },
  { 0,  0, "cu12.bdf", 			"cu12", 			3, 0, BM_T, FM_C, MM_C, "32-128,$590-$5ff,$fb1d-$fb4f", "_hebrew" },
  { 0,  0, "cu12.bdf", 			"cu12", 			3, 35, BM_T, FM_C, MM_C, "32-128,$600-$6ff,$FB50-$FBB1,$FE70-$FEFF,x32-64,x91-96,x123-191,x247,x697-879,x32-$5ff", "_arabic" },
  
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_R|MM_F|MM_E, "", "" },  
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-255", "_latin" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-701", "_extended" },

  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "9216-9471", "_72_73" },		// takeover from u8glib
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-127,9216-9471", "_0_72_73" },		

  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "9600-9727", "_75" },		
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-127,9600-9727", "_0_75" },		
  
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "9728-9855", "_76" },		// takeover from u8glib
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-127,9728-9855", "_0_76" },		
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "9856-9983", "_77" },		// takeover from u8glib
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-127,9856-9983", "_0_77" },		
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "9984-10240", "_78_79" },		// takeover from u8glib
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-127,9984-10240", "_0_78_79" },		
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "11008-11135", "_86" },		// takeover from u8glib
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-127,11008-11135", "_0_86" },		
  
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$370-$3ff", "_greek" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$590-$5ff,$fb1d-$fb4f", "_hebrew" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$980-$9ff", "_bengali" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$f00-$fff", "_tibetan" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$600-$6ff,$750-$77f,$fb50-$fdff,$fe70-$feff", "_urdu" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-127,$104-$107,$118-$119,$141-$144,$15a-$15b,$179-$17c,$d3,$f3", "_polish" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_C, "32-128,$900-$97f,$1cd0-$1cff,$a8e0-$a8ff", "_devanagari" },   /* Hindi, issue 584 */
  
  
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 35, BM_T, FM_C, MM_C, "32-128,$600-$6ff,$FB50-$FBB1,$FE70-$FEFF,x32-64,x91-96,x123-191,x247,x697-879,x32-$5ff", "_arabic" },

  /*
    $20a0-$20bf currency symbols
    $2103 Circle C-
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
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T|BM_H, FM_C, MM_C, "32-255,$20a0-$20bf,$2103,$2109,$2126,$2190-$21bb,$21d0-$21d9,$21e6-$21e9,$23e9-$23fa,$2580-$261f,$2654-$2667,$2680-$2685,$2713-$2718,$274f-$2752", "_symbols" },
  
  { 0,  0, "unifont_upper.bdf", 	"unifont", 		6, 0, BM_T, FM_C, MM_C, "$01f600-$01f64f>$20, $01f910-$01f92f>$70, $01f970-$01f971>$90, $01f973-$01f976>$92,$01f97a>$96, $01f9b8-$01f9b9>$97, $01f9d0-$01f9d6>$99", "_emoticons" },
  { 0,  0, "unifont_upper.bdf", 	"unifont", 		6, 0, BM_T, FM_C, MM_C, "$01f400-$01f43f>$20,$01f980-$01f9af>$60", "_animals" },
  { 0,  0, "unifont_upper.bdf", 	"unifont", 		6, 0, BM_T, FM_C, MM_C, "$01f030-$01f093>$20", "_domino" },
  { 0,  0, "unifont_upper.bdf", 	"unifont", 		6, 0, BM_T, FM_C, MM_C, "$01f0a0-$01f0f5>$20", "_cards" },
  { 0,  0, "unifont_upper.bdf", 	"unifont", 		6, 0, BM_T, FM_C, MM_C, "$01f310-$01f32c>$20", "_weather" },

  
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "chinese3.map", "_chinese3" },

  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "japanese3.map", "_japanese3" },

  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "korean1.map", "_korean1" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "korean2.map", "_korean2" },
  
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "vietnamese1.map", "_vietnamese1" },
  { 0,  0, "unifont.bdf", 		"unifont", 		6, 0, BM_T, FM_C, MM_M, "vietnamese2.map", "_vietnamese2" },
  
  { 0,  0, "gb16st.bdf", 		"gb16st", 		13, 0, BM_T, FM_C, MM_M, "chinese1.map", "_1" },
  { 0,  0, "gb16st.bdf", 		"gb16st", 		13, 0, BM_T, FM_C, MM_M, "chinese2.map", "_2" },
  { 0,  0, "gb16st.bdf", 		"gb16st", 		13, 0, BM_T, FM_C, MM_M, "chinese3.map", "_3" },

  { 0,  0, "gb24st.bdf", 		"gb24st", 		13, 0, BM_T, FM_C, MM_M, "chinese1.map", "_1" },
  { 0,  0, "gb24st.bdf", 		"gb24st", 		13, 0, BM_T, FM_C, MM_M, "chinese2.map", "_2" },
  { 0,  0, "gb24st.bdf", 		"gb24st", 		13, 0, BM_T, FM_C, MM_M, "chinese3.map", "_3" },

  { 0,  0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0,  0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0,  0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0,  0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0,  0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0,  0, "wenquanyi_9pt.bdf", 	"wqy12", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },

  { 0,  0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0,  0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0,  0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0,  0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0,  0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0,  0, "wenquanyi_10pt.bdf", 	"wqy13", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },

  { 0,  0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0,  0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0,  0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0,  0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0,  0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0,  0, "wenquanyi_13px.bdf", 	"wqy14", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },

  { 0,  0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0,  0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0,  0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0,  0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0,  0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0,  0, "wenquanyi_11pt.bdf", 	"wqy15", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },
  
  { 0,  0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "chinese1.map", "_chinese1" },
  { 0,  0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese2" },
  { 0,  0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "chinese2.map", "_chinese3" },
  { 0,  0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "gb2312.map", "_gb2312" },
  { 0,  0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "gb2312a.map", "_gb2312a" },
  { 0,  0, "wenquanyi_12pt.bdf", 	"wqy16", 		21, 0, BM_T, FM_C, MM_M, "gb2312b.map", "_gb2312b" },

  { 0,  0, "b10.bdf", 			"b10", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "b10.bdf", 			"b10", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "b10_b.bdf", 			"b10_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "b10_b.bdf", 			"b10_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  { 0,  0, "f10.bdf", 			"f10", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "f10.bdf", 			"f10", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "f10_b.bdf", 			"f10_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "f10_b.bdf", 			"f10_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  { 0,  0, "b12.bdf", 			"b12", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "b12.bdf", 			"b12", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "b12.bdf", 			"b12", 		16, 0, BM_T, FM_C, MM_M, "japanese3.map", "_japanese3" },
  { 0,  0, "b12_b.bdf", 			"b12_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "b12_b.bdf", 			"b12_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "b12_b.bdf", 			"b12_b", 		16, 0, BM_T, FM_C, MM_M, "japanese3.map", "_japanese3" },

    /* does not work, error in bdf 
  { 0,  0, "b14.bdf", 			"b14", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "b14.bdf", 			"b14", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "b14.bdf", 			"b14", 		16, 0, BM_T, FM_C, MM_M, "japanese3.map", "_japanese3" },
  { 0,  0, "b14_b.bdf", 			"b14_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "b14_b.bdf", 			"b14_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "b14_b.bdf", 			"b14_b", 		16, 0, BM_T, FM_C, MM_M, "japanese3.map", "_japanese3" },
  */

  { 0,  0, "f12.bdf", 			"f12", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "f12.bdf", 			"f12", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "f12_b.bdf", 			"f12_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "f12_b.bdf", 			"f12_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  { 0,  0, "b16.bdf", 			"b16", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "b16.bdf", 			"b16", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "b16.bdf", 			"b16", 		16, 0, BM_T, FM_C, MM_M, "japanese3.map", "_japanese3" },
  { 0,  0, "b16_b.bdf", 			"b16_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "b16_b.bdf", 			"b16_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "b16_b.bdf", 			"b16_b", 		16, 0, BM_T, FM_C, MM_M, "japanese3.map", "_japanese3" },

  { 0,  0, "f16.bdf", 			"f16", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "f16.bdf", 			"f16", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },
  { 0,  0, "f16_b.bdf", 			"f16_b", 		16, 0, BM_T, FM_C, MM_M, "japanese1.map", "_japanese1" },
  { 0,  0, "f16_b.bdf", 			"f16_b", 		16, 0, BM_T, FM_C, MM_M, "japanese2.map", "_japanese2" },

  
  

  { 0,  0, "ArtosSans-8.bdf",			"artossans8",			7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  { 0,  0, "ArtosSerif-8.bdf",			"artosserif8",			7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  { 0,  0, "Chroma48Medium-8.bdf",	"chroma48medium8",	7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  /* no lowercase */
  { 0,  0, "SaikyoSansBold-8.bdf",		"saikyosansbold8",		7, 0, BM_8, FM_C|FM_8, MM_U|MM_N, "" , ""},
  { 0,  0, "TorusSansBold-8.bdf",		"torussansbold8",		7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  { 0,  0, "VictoriaBold-8.bdf",		"victoriabold8",		7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  { 0,  0, "VictoriaMedium-8.bdf",		"victoriamedium8",		7, 0, BM_8, FM_C|FM_8, MM_R|MM_U|MM_N, "" , ""},
  
  { 0,  0, "courB08.bdf", 		"courB08", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courB10.bdf", 		"courB10", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courB12.bdf", 		"courB12", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courB14.bdf", 		"courB14", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courB18.bdf", 		"courB18", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courB24.bdf", 		"courB24", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courR08.bdf", 		"courR08", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courR10.bdf", 		"courR10", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courR12.bdf", 		"courR12", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courR14.bdf", 		"courR14", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courR18.bdf", 		"courR18", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "courR24.bdf", 		"courR24", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  
  { "-y -1 -th 2 -tv 3",  0, "courB18.bdf",	"courB18_2x3",	5, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y -1 -th 2 -tv 3",  0, "courR18.bdf",	"courR18_2x3",	5, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y -2 -th 3 -tv 4",  0, "courB24.bdf",	"courB24_3x4",	5, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y -2 -th 3 -tv 4",  0, "courR24.bdf",	"courR24_3x4",	5, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},


  { 0,  0, "helvB08.bdf", 		"helvB08", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvB10.bdf", 		"helvB10", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvB12.bdf", 		"helvB12", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvB14.bdf", 		"helvB14", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvB18.bdf", 		"helvB18", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvB24.bdf", 		"helvB24", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvR08.bdf", 		"helvR08", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvR10.bdf", 		"helvR10", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvR12.bdf", 		"helvR12", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvR14.bdf", 		"helvR14", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvR18.bdf", 		"helvR18", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "helvR24.bdf", 		"helvR24", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },

  { 0,  0, "ncenB08.bdf", 		"ncenB08", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenB10.bdf", 		"ncenB10", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenB12.bdf", 		"ncenB12", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenB14.bdf", 		"ncenB14", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenB18.bdf", 		"ncenB18", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenB24.bdf", 		"ncenB24", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenR08.bdf", 		"ncenR08", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenR10.bdf", 		"ncenR10", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenR12.bdf", 		"ncenR12", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenR14.bdf", 		"ncenR14", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenR18.bdf", 		"ncenR18", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "ncenR24.bdf", 		"ncenR24", 		5, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },

  { 0,  0, "timB08.bdf", 		"timB08", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timB10.bdf", 		"timB10", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timB12.bdf", 		"timB12", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timB14.bdf", 		"timB14", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timB18.bdf", 		"timB18", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timB24.bdf", 		"timB24", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timR08.bdf", 		"timR08", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timR10.bdf", 		"timR10", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timR12.bdf", 		"timR12", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timR14.bdf", 		"timR14", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timR18.bdf", 		"timR18", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  0, "timR24.bdf", 		"timR24", 		5, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  
  

  { 0,  0, "lubB08.bdf", 		"lubB08", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" }, 
  { 0,  0, "lubB10.bdf", 		"lubB10", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubB12.bdf", 		"lubB12", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubB14.bdf", 		"lubB14", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubB18.bdf", 		"lubB18", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubB19.bdf", 		"lubB19", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubB24.bdf", 		"lubB24", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubBI08.bdf", 		"lubBI08", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubBI10.bdf", 		"lubBI10", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubBI12.bdf", 		"lubBI12", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubBI14.bdf", 		"lubBI14", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubBI18.bdf", 		"lubBI18", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubBI19.bdf", 		"lubBI19", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubBI24.bdf", 		"lubBI24", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubI08.bdf", 		"lubI08", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubI10.bdf", 		"lubI10", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubI12.bdf", 		"lubI12", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubI14.bdf", 		"lubI14", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubI18.bdf", 		"lubI18", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubI19.bdf", 		"lubI19", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubI24.bdf", 		"lubI24", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBIS08.bdf", 		"luBIS08", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBIS10.bdf", 		"luBIS10", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBIS12.bdf", 		"luBIS12", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBIS14.bdf", 		"luBIS14", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBIS18.bdf", 		"luBIS18", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBIS19.bdf", 		"luBIS19", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBIS24.bdf", 		"luBIS24", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubR08.bdf", 		"lubR08", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubR10.bdf", 		"lubR10", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubR12.bdf", 		"lubR12", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubR14.bdf", 		"lubR14", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubR18.bdf", 		"lubR18", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubR19.bdf", 		"lubR19", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "lubR24.bdf", 		"lubR24", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBS08.bdf", 		"luBS08", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBS10.bdf", 		"luBS10", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBS12.bdf", 		"luBS12", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBS14.bdf", 		"luBS14", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBS18.bdf", 		"luBS18", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBS19.bdf", 		"luBS19", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luBS24.bdf", 		"luBS24", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luIS08.bdf", 		"luIS08", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luIS10.bdf", 		"luIS10", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luIS12.bdf", 		"luIS12", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luIS14.bdf", 		"luIS14", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luIS18.bdf", 		"luIS18", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luIS19.bdf", 		"luIS19", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luIS24.bdf", 		"luIS24", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luRS08.bdf", 		"luRS08", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luRS10.bdf", 		"luRS10", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luRS12.bdf", 		"luRS12", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luRS14.bdf", 		"luRS14", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luRS18.bdf", 		"luRS18", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luRS19.bdf", 		"luRS19", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  0, "luRS24.bdf", 		"luRS24", 		38, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  

  
  
  /* Adobe symb skipped... */


  
  { 0,  "-r 72 -p 8", "baby.ttf",			"baby",			2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 8", "blipfest_07.ttf",		"blipfest_07",		2, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 8", "chikita.ttf",			"chikita",			2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 8", "lucasfont_alternate.ttf",	"lucasfont_alternate",2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 8", "p01type.ttf",			"p01type",		2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 8", "pixelle_micro.ttf",		"pixelle_micro",	2, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "robot_de_niro.ttf",	"robot_de_niro",	2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 8", "trixel_square.ttf",		"trixel_square",	2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "haxrcorp4089.ttf",	"haxrcorp4089",	2, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "haxrcorp4089.ttf",	"haxrcorp4089",	2, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  
  { 0,  "-r 72 -p 24", "bubble.ttf",			"bubble",			2, 0, BM_T, FM_C, MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 36", "cardimon-pixel.ttf",	"cardimon_pixel",	2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 32", "maniac.ttf",			"maniac",			2, 0, BM_T, FM_C, MM_E|MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 32", "lucasarts-scumm-subtitle-roman-outline.ttf", "lucasarts_scumm_subtitle_o",	2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-r 72 -p 16", "lucasarts-scumm-subtitle-roman.ttf", "lucasarts_scumm_subtitle_r",	2, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },

  { "-y 1 -th 2 -tv 2",  "-r 72 -p 32", "lucasarts-scumm-subtitle-roman-outline.ttf",	"lucasarts_scumm_subtitle_o_2x2",	2, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y 0 -th 2 -tv 2",  "-r 72 -p 16", "lucasarts-scumm-subtitle-roman.ttf",	"lucasarts_scumm_subtitle_r_2x2",	2, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  
  { 0,  "-r 72 -p 24", "utopia24.ttf",			"utopia24",			2, 0, BM_T, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
  { 0,  "-r 72 -p 22", "m-c-kids-nes-credits-font.ttf", "m_c_kids_nes_credits_font",	2, 0, BM_T, FM_C, MM_R, "", "" },
  { 0,  "-r 72 -p 16", "chargen-92.ttf", "chargen_92",	2, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N|MM_E, "", "" },
 
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
  
  { 0,  "-a -r 72 -p 16", "FreeUniversal-Bold.ttf",	"fub11",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 20", "FreeUniversal-Bold.ttf",	"fub14",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 23", "FreeUniversal-Bold.ttf",	"fub17",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 27", "FreeUniversal-Bold.ttf",	"fub20",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 34", "FreeUniversal-Bold.ttf",	"fub25",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 40", "FreeUniversal-Bold.ttf",	"fub30",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 49", "FreeUniversal-Bold.ttf",	"fub35",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 58", "FreeUniversal-Bold.ttf",	"fub42",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 68", "FreeUniversal-Bold.ttf",	"fub49",			8, 0, BM_T, FM_C, MM_N, "", "" },

  { 0,  "-a -r 72 -p 16", "FreeUniversal-Bold.ttf", 	"fub11", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 20", "FreeUniversal-Bold.ttf", 	"fub14", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 23", "FreeUniversal-Bold.ttf", 	"fub17", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 27", "FreeUniversal-Bold.ttf", 	"fub20", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 34", "FreeUniversal-Bold.ttf", 	"fub25", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 40", "FreeUniversal-Bold.ttf", 	"fub30", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 49", "FreeUniversal-Bold.ttf", 	"fub35", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 58", "FreeUniversal-Bold.ttf", 	"fub42", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2031-$3000", "_symbol" },  // per mill sign does not fit
  { 0,  "-a -r 72 -p 68", "FreeUniversal-Bold.ttf", 	"fub49", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2031-$3000", "_symbol" },  // per mill sign does not fit

  
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

  { 0,  "-a -r 72 -p 15", "FreeUniversal-Regular.ttf",	"fur11",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 19", "FreeUniversal-Regular.ttf",	"fur14",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 23", "FreeUniversal-Regular.ttf",	"fur17",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 28", "FreeUniversal-Regular.ttf",	"fur20",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 34", "FreeUniversal-Regular.ttf",	"fur25",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 40", "FreeUniversal-Regular.ttf",	"fur30",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 48", "FreeUniversal-Regular.ttf",	"fur35",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 58", "FreeUniversal-Regular.ttf",	"fur42",			8, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 68", "FreeUniversal-Regular.ttf",	"fur49",			8, 0, BM_T, FM_C, MM_N, "", "" },

  { 0,  "-a -r 72 -p 16", "FreeUniversal-Regular.ttf", 	"fur11", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 19", "FreeUniversal-Regular.ttf", 	"fur14", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 23", "FreeUniversal-Regular.ttf", 	"fur17", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 28", "FreeUniversal-Regular.ttf", 	"fur20", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 34", "FreeUniversal-Regular.ttf", 	"fur25", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 40", "FreeUniversal-Regular.ttf", 	"fur30", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 48", "FreeUniversal-Regular.ttf", 	"fur35", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2030-$3000", "_symbol" },
  { 0,  "-a -r 72 -p 58", "FreeUniversal-Regular.ttf", 	"fur42", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2031-$3000", "_symbol" }, // per mill sign does not fit
  { 0,  "-a -r 72 -p 68", "FreeUniversal-Regular.ttf", 	"fur49", 			8, 0, BM_T, FM_C, MM_C, "32,35-57,$300-$3ff,$2031-$3000", "_symbol" },  // per mill sign does not fit
  
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
  { 0,  "-a -r 72 -p 25", "OldStandard-Regular.ttf",	"osb18",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 28", "OldStandard-Regular.ttf",	"osb21",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 34", "OldStandard-Regular.ttf",	"osb26",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 38", "OldStandard-Regular.ttf",	"osb29",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 48", "OldStandard-Regular.ttf",	"osb35",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 55", "OldStandard-Regular.ttf",	"osb41",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
*/
  
  { 0,  "-a -r 72 -p 25", "OldStandard-Bold.ttf",	"osb18",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 28", "OldStandard-Bold.ttf",	"osb21",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 34", "OldStandard-Bold.ttf",	"osb26",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 38", "OldStandard-Bold.ttf",	"osb29",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 48", "OldStandard-Bold.ttf",	"osb35",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 55", "OldStandard-Bold.ttf",	"osb41",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },

/*
./do_fontsize_a_v2.sh 26 ../ttf/os/OldStandard-Regular.ttf osr18
./do_fontsize_a_v2.sh 29 ../ttf/os/OldStandard-Regular.ttf osr21
./do_fontsize_a_v2.sh 36 ../ttf/os/OldStandard-Regular.ttf osr26
./do_fontsize_a_v2.sh 41 ../ttf/os/OldStandard-Regular.ttf osr29
./do_fontsize_a_v2.sh 49 ../ttf/os/OldStandard-Regular.ttf osr35
./do_fontsize_a_v2.sh 57 ../ttf/os/OldStandard-Regular.ttf osr41
*/

  { 0,  "-a -r 72 -p 26", "OldStandard-Regular.ttf",	"osr18",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 29", "OldStandard-Regular.ttf",	"osr21",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 36", "OldStandard-Regular.ttf",	"osr26",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 41", "OldStandard-Regular.ttf",	"osr29",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 49", "OldStandard-Regular.ttf",	"osr35",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 57", "OldStandard-Regular.ttf",	"osr41",			9, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  
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
  { 0,  "-a -r 72 -p 22", "inr.otf",	"inr16",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 26", "inr.otf",	"inr19",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 30", "inr.otf",	"inr21",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 33", "inr.otf",	"inr24",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 33", "inr.otf",	"inr24",			11, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  "-a -r 72 -p 36", "inr.otf",	"inr27",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 36", "inr.otf",	"inr27",			11, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  "-a -r 72 -p 40", "inr.otf",	"inr30",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 40", "inr.otf",	"inr30",			11, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  "-a -r 72 -p 44", "inr.otf",	"inr33",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 44", "inr.otf",	"inr33",			11, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  "-a -r 72 -p 51", "inr.otf",	"inr38",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 51", "inr.otf",	"inr38",			11, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  "-a -r 72 -p 57", "inr.otf",	"inr42",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 57", "inr.otf",	"inr42",			11, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  "-a -r 72 -p 62", "inr.otf",	"inr46",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 62", "inr.otf",	"inr46",			11, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  "-a -r 72 -p 67", "inr.otf",	"inr49",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 67", "inr.otf",	"inr49",			11, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  "-a -r 72 -p 72", "inr.otf",	"inr53",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 72", "inr.otf",	"inr53",			11, 0, BM_T, FM_C, MM_C, "32-128,$400-$52f", "_cyrillic" },
  { 0,  "-a -r 72 -p 78", "inr.otf",	"inr57",			11, 0, BM_M, FM_C, MM_N, "", "" },
  { 0,  "-a -r 72 -p 82", "inr.otf",	"inr62",			11, 0, BM_M, FM_C, MM_N, "", "" },
  
  { "-y -1 -x 1 -th 2 -tv 4",  "-a -r 72 -p 30", "inr.otf",	"inr21_2x4",	11, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y -1 -x 2 -th 3 -tv 6",  "-a -r 72 -p 44", "inr.otf",	"inr33_3x6",	11, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y 0 -x 3 -th 4 -tv 8",  "-a -r 72 -p 62", "inr.otf",		"inr46_4x8",	11, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  
 
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

  { 0,  "-a -r 72 -p 22", "inb.otf",	"inb16",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 25", "inb.otf",	"inb19",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 30", "inb.otf",	"inb21",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 33", "inb.otf",	"inb24",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 36", "inb.otf",	"inb27",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 40", "inb.otf",	"inb30",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 44", "inb.otf",	"inb33",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 51", "inb.otf",	"inb38",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 57", "inb.otf",	"inb42",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 62", "inb.otf",	"inb46",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 66", "inb.otf",	"inb49",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 71", "inb.otf",	"inb53",			11, 0, BM_M, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 78", "inb.otf",	"inb57",			11, 0, BM_M, FM_C, MM_N, "", "" },
  { 0,  "-a -r 72 -p 84", "inb.otf",	"inb63",			11, 0, BM_M, FM_C, MM_N, "", "" },
  
  { "-y -1 -x 1 -th 2 -tv 4",  "-a -r 72 -p 30", "inb.otf",	"inb21_2x4",	11, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y -1 -x 2 -th 3 -tv 6",  "-a -r 72 -p 44", "inb.otf",	"inb33_3x6",	11, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y 0 -x 3 -th 4 -tv 8",  "-a -r 72 -p 62", "inb.otf",	"inb46_4x8",	11, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  

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

  { 0,  "-a -r 72 -p 23", "Logisoso.ttf",	"logisoso16",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 27", "Logisoso.ttf",	"logisoso18",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 30", "Logisoso.ttf",	"logisoso20",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 32", "Logisoso.ttf",	"logisoso22",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 34", "Logisoso.ttf",	"logisoso24",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 38", "Logisoso.ttf",	"logisoso26",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 40", "Logisoso.ttf",	"logisoso28",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 43", "Logisoso.ttf",	"logisoso30",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 45", "Logisoso.ttf",	"logisoso32",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 49", "Logisoso.ttf",	"logisoso34",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 54", "Logisoso.ttf",	"logisoso38",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 60", "Logisoso.ttf",	"logisoso42",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 66", "Logisoso.ttf",	"logisoso46",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 71", "Logisoso.ttf",	"logisoso50",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 77", "Logisoso.ttf",	"logisoso54",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 83", "Logisoso.ttf",	"logisoso58",			10, 0, BM_T, FM_C, MM_F|MM_R|MM_N, "", "" },
  { 0,  "-a -r 72 -p 89", "Logisoso.ttf",	"logisoso62",			10, 0, BM_T, FM_C, MM_N, "", "" },
  { 0,  "-a -r 72 -p 110", "Logisoso.ttf",	"logisoso78",			10, 0, BM_T, FM_C, MM_N, "", "" },
  { 0,  "-a -r 72 -p 131", "Logisoso.ttf",	"logisoso92",			10, 0, BM_T, FM_C, MM_N, "", "" },

  { 0,  "-r 72 -p 8", "PressStart2P.ttf",	"pressstart2p",			12, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},
  { 0,  "-r 72 -p 8", "pcsenior.ttf",		"pcsenior",			12, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},


    /*
  PxPlus_IBM_CGAthin.ttf
  PxPlus_IBM_CGA.ttf
  PxPlus_TandyNew_TV.ttf
  */
  
  { 0,  "-r 72 -p 8", "PxPlus_IBM_CGAthin.ttf",	"pxplusibmcgathin",	14, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},
  { 0,  "-r 72 -p 8", "PxPlus_IBM_CGA.ttf",		"pxplusibmcga",		14, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},
  { 0,  "-r 72 -p 8", "PxPlus_TandyNew_TV.ttf",	"pxplustandynewtv",	14, 0, BM_8, FM_C|FM_8, MM_F|MM_R|MM_U|MM_N, "" , ""},  
  { 0,  "-r 72 -p 8", "PxPlus_TandyNew_TV.ttf",	"pxplustandynewtv",	14, 0, BM_T|BM_8, FM_C, MM_C, "32-$ffff", "_all" },

  
  { 0,  "-r 72 -p 16", "PxPlus_IBM_VGA9.ttf",	"pxplusibmvga9",	14, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "" , ""},
  { 0,  "-r 72 -p 16", "PxPlus_IBM_VGA9.ttf",	"pxplusibmvga9",	14, 0, BM_T|BM_M, FM_C, MM_C, "32-$ffff", "_all" },
  
  { 0,  "-r 72 -p 16", "PxPlus_IBM_VGA8.ttf",	"pxplusibmvga8",	14, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "" , ""},
  { 0,  "-r 72 -p 16", "PxPlus_IBM_VGA8.ttf",	"pxplusibmvga8",	14, 0, BM_T|BM_M, FM_C, MM_C, "32-$ffff", "_all" },
  
  { 0,  "-r 72 -p 16", "Px437_Wyse700a.ttf",	"px437wyse700a",	14, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "" , ""},
  { 0,  "-r 72 -p 16", "Px437_Wyse700b.ttf",	"px437wyse700b",	14, 0, BM_T|BM_M, FM_C, MM_F|MM_R|MM_N, "" , ""},

  { "-y -1 -th 2 -tv 2",  "-r 72 -p 16", "Px437_Wyse700a.ttf",	"px437wyse700a_2x2",	14, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},
  { "-y -1 -th 2 -tv 2",  "-r 72 -p 16", "Px437_Wyse700b.ttf",	"px437wyse700b_2x2",	14, 0, BM_8, FM_8, MM_F|MM_R|MM_N, "" , ""},


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
const char *fntlistallpath = "../../../../u8g2.wiki/fntlistallplain.md";
FILE *fntlistall;

const char *fntlist8path = "../../../../u8g2.wiki/fntlist8.md";
FILE *fntlist8;
const char *fntlist12path = "../../../../u8g2.wiki/fntlist12.md";
FILE *fntlist12;
const char *fntlist16path = "../../../../u8g2.wiki/fntlist16.md";
FILE *fntlist16;
const char *fntlist99path = "../../../../u8g2.wiki/fntlist99.md";
FILE *fntlist99;


const char *fntlistmonopath = "../../../../u8g2.wiki/fntlistmono.md";
FILE *fntlistmono;
const char *fntlist8x8path = "../../../../u8g2.wiki/fntlist8x8.md";
FILE *fntlist8x8;

#ifdef BUILD2

u8g2_t u8g2;
int u8x8_fnt_cnt = 0;
int u8g2_fnt_cnt = 0;

extern void do_font_loop(cbfn_t cb);

/*===================================================================*/
/* word cloud */

#define WORD_CLOUD_MAX_X 600


struct _box_t 
{
  uint32_t w, h;
};
typedef struct _box_t box_t;

struct _pos_t 
{
  uint32_t x, y;
};
typedef struct _pos_t pos_t;

struct _pbox_t		// placed box
{
  pos_t pos;
  int box_idx;
};
typedef struct _pbox_t pbox_t;

#define BOX_LIST_MAX 500
box_t box_list[BOX_LIST_MAX];
char box_word[BOX_LIST_MAX][64];
const uint8_t *box_font[BOX_LIST_MAX];
int box_cnt = 0;

int box_index[BOX_LIST_MAX];

#define PLACE_OPTION_MAX 1000
int place_option_cnt = 0;
pos_t place_option_list[PLACE_OPTION_MAX];

#define PLACED_BOX_MAX 1000
int placed_box_cnt = 0;
pbox_t placed_box_list[PLACED_BOX_MAX];
uint32_t placed_box_area_max_x;
uint32_t placed_box_area_max_y;
uint32_t placed_box_area_value;

/*============================================*/

int place_option_find(pos_t *p)
{
  int i;
  for( i = 0; i < place_option_cnt; i++ )
  {
    if ( place_option_list[i].x == p->x && place_option_list[i].y == p->y)
      return i;		// place option already exists
  }
  return -1;
}

void place_option_add(pos_t *p)
{
  int i;
  assert(place_option_cnt < PLACE_OPTION_MAX);
  
  if ( place_option_find(p) >= 0 )
      return;		// place option already exists
  
  // the placement option also must not be part of any existing box
  for( i = 2; i < placed_box_cnt; i++ )
  {
    if ( p->x >= placed_box_list[i].pos.x && p->x < placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w )
      if ( p->y >= placed_box_list[i].pos.y && p->y < placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h )
	return;		// place option would be inside an existing box
  }
  
  // place option does not exist, add new place option
  
  place_option_list[place_option_cnt] = *p;
  place_option_cnt++;
}

void place_option_by_xy(uint32_t x, uint32_t y)
{
  pos_t p;
  p.x = x;
  p.y = y;
  place_option_add(&p);
}

void place_option_delete(int idx)
{
  if ( idx < 0 || idx >= place_option_cnt )
    return;
  for( ; idx+1 < place_option_cnt; idx++ )
  {
    place_option_list[idx] = place_option_list[idx+1];
  }
  place_option_cnt--;
}

/*============================================*/

// p is copyied, b only the address is stored
pbox_t *placed_box_push(pos_t *p, int box_idx)
{
  assert(placed_box_cnt < PLACED_BOX_MAX);

  
  // place the new box
  placed_box_list[placed_box_cnt].pos = *p;
  placed_box_list[placed_box_cnt].box_idx = box_idx;
  placed_box_cnt++;
  return placed_box_list+placed_box_cnt-1;
}

void placed_box_pop(void)
{
  assert(placed_box_cnt > 0);
  placed_box_cnt--;
}

uint32_t placed_box_calculate_max_area(void)
{
  int i;
  placed_box_area_max_y = 0;
  placed_box_area_max_x = 0;
  for( i = 2; i < placed_box_cnt; i++ )
  {
    
    if ( placed_box_area_max_y < placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h )
      placed_box_area_max_y = placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h;
    if ( placed_box_area_max_x < placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w )
      placed_box_area_max_x = placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w;
  }
  
  if ( placed_box_area_max_x > WORD_CLOUD_MAX_X )
    // weight of y is higher, so this should give a more wider picture
    placed_box_area_value = placed_box_area_max_x*5 + placed_box_area_max_y ;
  else
    // weight of y is higher, so this should give a more wider picture
    placed_box_area_value = placed_box_area_max_x + placed_box_area_max_y *5;
  return placed_box_area_value;
}

/*============================================*/

int is_intersection(pos_t *p1, box_t *b1, pos_t *p2, box_t *b2) 
{ 
    pos_t q1 = *p1;
    pos_t q2 = *p2;
    q1.x += b1->w;
    q1.y += b1->h;
    q2.x += b2->w;
    q2.y += b2->h;
  
    if ( p1->x >= q2.x || q1.x <= p2->x )
      return 0;
    if ( p1->y >= q2.y || q1.y <= p2->y )
      return 0;
  
    return 1; 
} 

int is_intersection_with_placed_box(pos_t *p1, box_t *b1, int idx) 
{
  return is_intersection(p1, b1, &(placed_box_list[idx].pos),  box_list+placed_box_list[idx].box_idx );
}

int is_any_intersection(pos_t *p1, box_t *b1)
{
  int i;
  // start with the third box, because the first two are boundary boxes
  for( i = 2; i < placed_box_cnt; i++ )
  {
    if ( is_intersection_with_placed_box(p1, b1, i) != 0 )
      return 1;
  }
  return 0;
}


// add new place options by a give pbox
void place_option_by_pbox(pbox_t *pbox)
{
  int i, found_i;
  uint32_t y, x;
  uint32_t max_y, max_x;
  
  // from the lower left corner, search towards the left until another box is hit.
  y = pbox->pos.y + box_list[pbox->box_idx].h;
  max_x = 0;
  found_i = -1;
  for( i = 0; i < placed_box_cnt; i++ )
  {
    if ( placed_box_list+i != pbox ) // do not check ourself
    {
      if ( placed_box_list[i].pos.x <= pbox->pos.x )
      {
	if ( y >=  placed_box_list[i].pos.y && y < placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h )
	{
	  // left box found, but is it max?
	  // must use >= here, because the initial boxes have zero area
	  if ( max_x <= placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w )
	  {
	    max_x = placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w;
	    found_i = i;
	  }
	}
      }
    }
  }
  if (found_i >= 0)
    place_option_by_xy(max_x, y);
  

  // from the upper right corner, search towards the top until another box is hit.
  x = pbox->pos.x + box_list[pbox->box_idx].w;
  max_y = 0;
  found_i = -1;
  for( i = 0; i < placed_box_cnt; i++ )
  {
    if ( placed_box_list+i != pbox ) // do not check ourself
    {
      if ( placed_box_list[i].pos.y <= pbox->pos.y )
      {
	if ( x >=  placed_box_list[i].pos.x && x < placed_box_list[i].pos.x + box_list[placed_box_list[i].box_idx].w )
	{
	  // upper box found, but is it the next ?
	  // must use >= here, because the initial boxes have zero area
	  if ( max_y <= placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h )
	  {
	    max_y = placed_box_list[i].pos.y + box_list[placed_box_list[i].box_idx].h;
	    found_i = i;
	  }
	}
      }
    }
  }
  if (found_i >= 0)
    place_option_by_xy(x, max_y);

}

void add_all_place_options(void)
{
  int i;
  //place_option_cnt = 0;	// clear the place option list
  for( i = 2; i < placed_box_cnt; i++ )
  {
    place_option_by_pbox(placed_box_list+i);
  }
}

void show(void)
{
  pos_t p;
  box_t b;
  int i;
  b.w = 1;
  b.h = 1;
  for( p.y = 0; p.y < 30; p.y++ )
  {
    for( p.x = 0; p.x < 60; p.x++ )
    {

      for( i = 0; i < place_option_cnt; i++ )
      {
	if ( place_option_list[i].x == p.x &&  place_option_list[i].y == p.y )
	{
	  break;
	}
      }
      if ( i < place_option_cnt )
      {
	printf("*");
      }
      else
      {
	
	for( i = 0; i < placed_box_cnt; i++ )
	{
	  if ( is_intersection_with_placed_box(&p, &b, i) != 0 )
	    break;
	}
	if ( i < placed_box_cnt )
	{
	  printf("%c", i+'a');
	}
	else
	{
	  printf(" ");
	}
      }
    }
    printf("\n");
  }
}

void init(void)
{
  pos_t p;
  

  box_list[0].w = 0x0ffffffff;
  box_list[0].h = 0;

  box_list[1].w = 0;
  box_list[1].h = 0x0ffffffff;
  
  box_cnt = 2;
  place_option_cnt = 0;
  placed_box_cnt = 0;
 
  p.x = 0;
  p.y = 0;  
  placed_box_push(&p, 0);

  p.x = 0;
  p.y = 0;  
  placed_box_push(&p, 1);

  // create one place option at the upper left
  place_option_by_xy(0, 0);
  
}

void do_best_place(int box_idx)
{
  int i;
  int found_i;
  pbox_t *pbox;
  uint32_t value;
  uint32_t lowest_value;
  
  found_i = -1;
  lowest_value = 0x0ffffffff;
  for( i = 0; i < place_option_cnt; i++ )
  {
    // check whether this placement would generate an intersection
    if ( is_any_intersection(place_option_list+i, box_list+box_idx) == 0 )
    {
      // place the box at the position
      pbox = placed_box_push(place_option_list+i, box_idx);
      value = placed_box_calculate_max_area();
      /*
      if ( value == 0x0ffffffff )
      {
	value = pbox->pos.y;
      }
      else
      */
      {
	value *= 8;
	value += pbox->pos.x;
	value += pbox->pos.y;
      }
      
      // greedy algorithm: We search for the lowest area increase
      if ( lowest_value > value )
      {
	lowest_value = value ;
	found_i = i;
      }
      
      placed_box_pop();
    }
  }

  if ( found_i >= 0 )
  {
    
    // now permanently place the box
    pbox = placed_box_push(place_option_list+found_i, box_idx);

    // delete the position from the place option list
    place_option_delete(found_i);
    
    // calculate new place options with the new box
    //place_option_by_pbox(pbox);
 
    // recalculate all place options
    add_all_place_options();
  }
  else
  {
    //assert(found_i >= 0 );
  }
}


/*
  Linear Congruential Generator (LCG)
  z = (a*z + c) % m;  
  m = 256 (8 Bit)
  
  for period:
  a-1: dividable by 2
  a-1: multiple of 4
  c: not dividable by 2
  
  c = 17
  a-1 = 64 --> a = 65
*/
uint8_t cloud_z = 127;	// start value
uint8_t cloud_rnd(void) {
/*
  cloud_z++;
  cloud_z = (uint8_t)((uint16_t)65*(uint16_t)cloud_z + (uint16_t)17);
  return (uint8_t)cloud_z;
  */
  return rand() & 255;
}




//u8g2_IsAllValidUTF8

char *cloud_utf8[] = 
{
  "µC"
  "Numérique"
  "像素",
  "屏幕",
  "图形",
  "制作",
  "Ψηφιακή",
  "κατασκευάζω",
  "Οθόνη",
   "Γραφικά",
   "アイコン、",
   "作る",
   "ビットマップ",
   "キャラクター、",  
  "전자", "엔지니어", "장치", "하드웨어",
   "Ingeniør", "Skærm",
  "Gerät",
  "Sprzęt",
  "Računar", "Ugrađen",
  "Grafică", 
  "экран",
   "Графика",
   "Значок",
   "Фонт", "екран", "рачунар", "уграђен",
   "พิกเซล",
   "หน้าจอ",
   "กราฟิก",
   "Näyttö",
   "ĂăĚěŇň",
   "ÄäÖöÜü",
   "Ææ",
   "E=mc²",
  "Pixel",
  "Screen",
  "Graphics",
  "Icon",
  "Bitmap",
  "Character",
  "Glyph",
  "Font",
  "Display",
  "Computer",
  "Embedded",
  "Electronics", 
  "Engineer",
  "Device",
  "Hardware",
  "Software",
  "Science",
  "Digital",
  "Arduino",
  "U8g2",
  "DIY"
};

char *cloud_str[] = 
{
  "Pixel",
  "Screen",
  "Graphics",
  "Icon",
  "Bitmap",
  "Character",
  "Glyph",
  "Font",
  "Display",
  "Computer",
  "Embedded",
  "Electronics", 
  "Engineer",
  "Device",
  "Hardware",
  "Software",
  "Science",
  "Digital",
  "Arduino",
  "U8g2",
  "DIY"
};

char *cloud_simple[] = 
{
  "U8g2",
  "Abc",
  "XYZ",
  "AaBb",
  "Xy"
};



void cloud_add(u8g2_t *u8g2, const uint8_t *font, char *word)
{
  u8g2_uint_t extra = 8;
  u8g2_SetFont(u8g2, font);
  box_list[box_cnt].w = u8g2_GetUTF8Width(u8g2, word) + extra; 
  box_list[box_cnt].h = u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2) + extra;
  strcpy(box_word[box_cnt], word); 
  //puts(word);
  box_font[box_cnt] = font;
  box_cnt++;
}

void cloud_auto_add(u8g2_t *u8g2, const uint8_t *font)
{
  int i, n, cnt;
  
  u8g2_SetFont(u8g2, font);
  
  
  if ( u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2) > 30 )
  {
    n = sizeof(cloud_simple)/sizeof(*cloud_simple);
    
    cnt = 0;
    for( i = 0; i < n; i++ )
    {
      if ( u8g2_IsAllValidUTF8(u8g2, cloud_simple[i]) != 0 )
      {
	cnt++;
      }
    }
    
    if ( cnt > 0 )
    {
      
      cnt = cloud_rnd() % cnt;
      
      for( i = 0; i < n; i++ )
      {
	if ( u8g2_IsAllValidUTF8(u8g2, cloud_simple[i]) != 0 )
	{
	  if ( cnt == 0 )
	    break;
	  cnt--;
	}
      }
    }
    
    if ( i < n )
    {
      cloud_add(u8g2, font, cloud_simple[i]);
      return;
    }
    
  }
  
  n = sizeof(cloud_utf8)/sizeof(*cloud_utf8);
  //printf("n=%d\n", n);
  
  cnt = 0;
  for( i = 0; i < n; i++ )
  {
    if ( u8g2_IsAllValidUTF8(u8g2, cloud_utf8[i]) != 0 )
    {
      cnt++;
    }
  }
  
  if ( cnt > 0 )
  {
    cnt = cloud_rnd() % cnt;
    
    for( i = 0; i < n; i++ )
    {
      if ( u8g2_IsAllValidUTF8(u8g2, cloud_utf8[i]) != 0 )
      {
	if ( cnt == 0 )
	  break;
	cnt--;
      }
    }
  }
  else
  {
    i = n;
  }
  
  if ( i < n )
  {
    cloud_add(u8g2, font, cloud_utf8[i]);
    return;
  }
  
  n = sizeof(cloud_str)/sizeof(*cloud_str);
  //printf("n=%d\n", n);
  i = cloud_rnd() % n;
  cloud_add(u8g2, font, cloud_str[i]);
  
}

void cloud_init(void)
{
  init();  
}

void cloud_calculate(uint8_t z)
{
  int i;
  int a, b, t;
  
  cloud_z = z;
  srand(z);
  
  for( i = 0; i < box_cnt; i++ )
  {
    box_index[i] = i;
  }
  
  for ( i = 0; i < box_cnt / 2; i++ )
  {
    a = cloud_rnd() % (box_cnt-2);
    a += 2;
    b = cloud_rnd() % (box_cnt-2);
    b += 2;
    t = box_index[a];
    box_index[a] = box_index[b];
    box_index[b] = t;
  }
  
  for( i = 2; i < box_cnt; i++ )
    do_best_place(box_index[i]);  
}


void cloud_draw(u8g2_t *u8g2)
{
  int i;
  for( i = 2; i < placed_box_cnt; i++ )
  {


	u8g2_SetFont(u8g2, box_font[placed_box_list[i].box_idx]);
	u8g2_SetFontMode(u8g2, 1);
	u8g2_DrawUTF8(u8g2, 
	  placed_box_list[i].pos.x, 
	  placed_box_list[i].pos.y, 
	  box_word[placed_box_list[i].box_idx]);	
  }
}





/*===================================================================*/


void overview_draw_line(int i, uint16_t encoding_start, uint16_t x, uint16_t y, uint16_t w, uint16_t glyphs_per_line)
{
  int j;

  u8g2_SetFont(&u8g2, u8g2_font_list[u8g2_fnt_cnt]);
  u8g2_SetFontDirection(&u8g2, 0);
  for( j = 0; j < glyphs_per_line; j++)
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
  uint16_t glyphs_per_line = 16;
  int size;
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
  
  size = u8g2_GetFontSize(u8g2_font_list[u8g2_fnt_cnt]);
  if ( size > 100000 )
    glyphs_per_line = 32;
    
  sprintf(s, "Font Data Size: %d Bytes", size);
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
    encoding = line*glyphs_per_line;
    if ( is_overview_line_empty(encoding) == 0 )
    {
      u8g2_SetFont(&u8g2, u8g2_font_7x13_tr);    
      sprintf(s, "%5d/%04x ", encoding, encoding);
      
      x = u8g2_DrawStr(&u8g2, 0, y, s);
      overview_draw_line(i, encoding, x, y, cw+1, glyphs_per_line);
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
    int cw;
    int ch;
    printf("8x8 font overview picture %s\n", target_font_identifier);  
    u8g2_SetupBuffer_TGA(&u8g2, &u8g2_cb_r0);
    u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
    //u8x8_ClearDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetFont(u8g2_GetU8x8(&u8g2), u8x8_font_amstrad_cpc_extended_r);
    u8x8_DrawString(u8g2_GetU8x8(&u8g2), 0, 0, target_font_identifier);
    u8x8_SetFont(u8g2_GetU8x8(&u8g2), u8x8_font_list[u8x8_fnt_cnt]);
    cw = u8x8_GetFontCharWidth(u8g2_GetU8x8(&u8g2));
    ch = u8x8_GetFontCharHeight(u8g2_GetU8x8(&u8g2));
    
    {
      uint8_t x;
      uint8_t y;
      static char s[32];
      for( y = 0; y < 16; y++ )
      {
	u8x8_SetFont(u8g2_GetU8x8(&u8g2), u8x8_font_amstrad_cpc_extended_r);
	sprintf(s, "%3d/%02x ", y*16, y*16);
	u8x8_DrawString(u8g2_GetU8x8(&u8g2), 0, (y+2)*ch, s);
  	u8x8_SetFont(u8g2_GetU8x8(&u8g2), u8x8_font_list[u8x8_fnt_cnt]);
	for( x = 0; x < 16; x++ )
	{
	    u8x8_DrawGlyph(u8g2_GetU8x8(&u8g2), x*(cw+1)+7,(y+2)*ch, y*16+x);
	}
      }
    }
    
    if ( mm != MM_N )
      u8x8_DrawString(u8g2_GetU8x8(&u8g2), 0,(16+2)*ch, "The quick brown fox jumps over the lazy dog.");

    tga_save("font.tga");
    
    /* remove date info, see https://legacy.imagemagick.org/discourse-server/viewtopic.php?t=12230 */
    sprintf(convert_cmd, "convert font.tga +set modify-date +set create-date -trim %s %s.png", convert_extra_options, target_font_identifier );
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
    
     /* remove date info, see https://legacy.imagemagick.org/discourse-server/viewtopic.php?t=12230 */
   sprintf(convert_cmd, "convert font.tga +set modify-date +set create-date -trim %s %s.png", convert_extra_options, target_font_identifier );
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
    
     /* remove date info, see https://legacy.imagemagick.org/discourse-server/viewtopic.php?t=12230 */
    sprintf(convert_cmd, "convert font.tga +set modify-date +set create-date -trim %s %s_short.png", convert_extra_options, target_font_identifier );
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
    
    /* remove date info, see https://legacy.imagemagick.org/discourse-server/viewtopic.php?t=12230 */
    sprintf(convert_cmd, "convert font.tga +set modify-date +set create-date -trim %s %s_short.png", convert_extra_options, target_font_identifier );
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
          
          if ( current_capital_A_size <= 8 )
          {
            fprintf(fntlist8, "\n## %d Pixel Height\n\n", current_capital_A_size);
          }
          else if ( current_capital_A_size <= 12 )
          {
            fprintf(fntlist12, "\n## %d Pixel Height\n\n", current_capital_A_size);
          }
          else if ( current_capital_A_size <= 16 )
          {
            fprintf(fntlist16, "\n## %d Pixel Height\n\n", current_capital_A_size);
          }
          else
          {
            fprintf(fntlist99, "\n## %d Pixel Height\n\n", current_capital_A_size);
          }
          
	  printf("listall: == %d ==\n", current_capital_A_size);
	}
	font_found_for_this_size = 1;
	//fprintf(fntlistall, "![fntpic/%s_short.png](fntpic/%s_short.png) ", target_font_identifier, target_font_identifier);
	fprintf(fntlistall, "%s ", target_font_identifier);
	fprintf(fntlistall, " [%s](%s)\n\n", gi[fi[i].group].groupname, gi[fi[i].group].reference);
        
          if ( current_capital_A_size <= 8 )
          {
            fprintf(fntlist8, "![fntpic/%s_short.png](fntpic/%s_short.png) ", target_font_identifier, target_font_identifier);
            fprintf(fntlist8, "%s ", target_font_identifier);
            fprintf(fntlist8, " [%s](%s)\n\n", gi[fi[i].group].groupname, gi[fi[i].group].reference);
          }
          else if ( current_capital_A_size <= 12 )
          {
            fprintf(fntlist12, "![fntpic/%s_short.png](fntpic/%s_short.png) ", target_font_identifier, target_font_identifier);
            fprintf(fntlist12, "%s ", target_font_identifier);
            fprintf(fntlist12, " [%s](%s)\n\n", gi[fi[i].group].groupname, gi[fi[i].group].reference);
          }
          else if ( current_capital_A_size <= 16 )
          {
            fprintf(fntlist16, "![fntpic/%s_short.png](fntpic/%s_short.png) ", target_font_identifier, target_font_identifier);
            fprintf(fntlist16, "%s ", target_font_identifier);
            fprintf(fntlist16, " [%s](%s)\n\n", gi[fi[i].group].groupname, gi[fi[i].group].reference);
          }
          else
          {
            fprintf(fntlist99, "![fntpic/%s_short.png](fntpic/%s_short.png) ", target_font_identifier, target_font_identifier);
            fprintf(fntlist99, "%s ", target_font_identifier);
            fprintf(fntlist99, " [%s](%s)\n\n", gi[fi[i].group].groupname, gi[fi[i].group].reference);
          }
        
        
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
  
  file_copy("fntlist8.pre", fntlist8path);
  fntlist8 = fopen(fntlist8path, "r+");
  fseek(fntlist8, 0L, SEEK_END);
  
  file_copy("fntlist12.pre", fntlist12path);
  fntlist12 = fopen(fntlist12path, "r+");
  fseek(fntlist12, 0L, SEEK_END);
  
  file_copy("fntlist16.pre", fntlist16path);
  fntlist16 = fopen(fntlist16path, "r+");
  fseek(fntlist16, 0L, SEEK_END);

  file_copy("fntlist99.pre", fntlist99path);
  fntlist99 = fopen(fntlist99path, "r+");
  fseek(fntlist99, 0L, SEEK_END);

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
  
  fclose(fntlist99);
  fclose(fntlist16);
  fclose(fntlist12);
  fclose(fntlist8);
  
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
  
  if ( fi[i].bdfconv_opt != 0 )
  {
    strcat(bdf_cmd, fi[i].bdfconv_opt);    
    strcat(bdf_cmd, " ");
  }
  
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
  if ( mm == MM_E ) strcat(bdf_cmd, " -m '32-701>32,7838,8320-8329,8364,64256-64263'");  /* added subscript 0-9 8320-8329 */
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

#ifdef BUILD2

void generate_font_group_word_cloud(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms)
{
  static int _i = 0;
  static int _fm = 0;
  static int _bm = 0;
  
  if ( fi[i].group == current_font_group_index && fm == FM_C )
  {
    if ( strcmp( current_font_name, fi[i].name ) != 0 )
    {
      strcpy(current_font_name, fi[i].name);
      
      //fprintf(current_md_file, "\n");
      //fprintf(current_md_file, "## %s\n", current_font_name);
      //printf("## %s\n", current_font_name);      
    }
    else
    {
      if ( _i == i && _fm == fm && _bm == bm )
      {
      }
      else
      {
	 // printf( "group %s, %s, font %s %s\n", gi[current_font_group_index].groupname, gi[current_font_group_index].reference, fi[i].name, target_font_identifier);
	//fprintf(current_md_file, "\n");
      }
	
    }
    //fprintf(current_md_file, "![fntpic/%s.png](fntpic/%s.png)\n", target_font_identifier, target_font_identifier);

    if ( _i == i )
    {
      //
    }
    else
    {
      //u8g2_font_list[u8g2_fnt_cnt]
      cloud_auto_add(&u8g2, u8g2_font_list[u8g2_fnt_cnt]);

      //printf( "%d: group %s, %s, font %s %s %d %s\n", i, gi[current_font_group_index].groupname, gi[current_font_group_index].reference, fi[i].name, target_font_identifier, u8g2_fnt_cnt, u8g2_font_names[u8g2_fnt_cnt]);
    }
    
    _i = i;
    _fm = fm;
    _bm = bm;
    
  }

  if ( fm == FM_C ) 
  {
    u8g2_fnt_cnt++;
  }

}
#endif

void gen_font(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms, cbfn_t cb )
{
  
  if ( fm == FM_8 )
    if ( bm != BM_8 )
      return;
  
  strcpy(target_font_identifier, fms);
  strcat(target_font_identifier, "_font_");
  strcat(target_font_identifier, fi[i].name);
  if ( bms[0] != '\0' || mms[0] != '\0' )
  {
    strcat(target_font_identifier, "_");
    strcat(target_font_identifier, bms);
    strcat(target_font_identifier, mms);
  }
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
#ifdef BUILD2
    u8g2_fnt_cnt = 0;
    u8x8_fnt_cnt = 0;
#endif
    do_font_loop(cb);
    fclose(current_md_file);
  }
}

void do_font_groups_wc(cbfn_t cb)
{
  int cnt;
  cnt = sizeof(gi)/sizeof(*gi);
  for( current_font_group_index = 0; current_font_group_index < cnt; current_font_group_index++ )
  {
#ifdef BUILD2
    u8g2_fnt_cnt = 0;
    u8x8_fnt_cnt = 0;

    u8g2_SetupBuffer_TGA(&u8g2, &u8g2_cb_r0);
    u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
    u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
    u8g2_SetFontPosTop(&u8g2);

    cloud_init();

    do_font_loop(cb);

    cloud_calculate(4);
    u8g2_FirstPage(&u8g2);
    do
    {
      cloud_draw(&u8g2);
      
    } while( u8g2_NextPage(&u8g2) );


    tga_save("font.tga");    
     /* remove date info, see https://legacy.imagemagick.org/discourse-server/viewtopic.php?t=12230 */
   sprintf(convert_cmd, "convert font.tga +set modify-date +set create-date -trim %s %s_word_cloud.png", convert_extra_options, gi[current_font_group_index].reference );
    system(convert_cmd);

    
    printf("Group end %s\n", gi[current_font_group_index].reference);
#endif
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

  do_font_groups_wc(generate_font_group_word_cloud);
  
#endif

  return 0;
}
