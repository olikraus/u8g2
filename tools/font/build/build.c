
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BM_T	1	/* Transparent = build mode 0 proportional */
#define BM_H	2	/* Common Height = build mode 1 */
#define BM_M	4	/* Monospace = build mode 2 */
#define BM_8	8	/* 8x8 = build mode 3 */
#define FM_C	1	/* u8g2 compressed font */
#define FM_8	2	/* u8x8 uncompressed font */
#define MM_F	1	/* full */
#define MM_R	2	/* reduced */
#define MM_N	4	/* numbers */

/*
    fprintf(out_fp, "const uint8_t %s[%d] U8X8_FONT_SECTION(\"%s\") \n", fontname, bf->target_cnt, fontname);
    fprintf(out_fp, "const uint8_t %s[%d] U8G2_FONT_SECTION(\"%s\") \n", fontname, bf->target_cnt, fontname);

*/


struct groupinfo
{
  char *groupname;
};

struct fontinfo
{
  int is_ttf;		/* 0 = bdf, 1= ttf */
  char *filename;	/* filename including extension */
  char *name;
  int group;		/* group-index */
  int	pt;			/* point size, ttf only */
  int build_mode;	/* Or'd BM_T, BM_H, BM_M, BM_8 */
  int font_mode;	/* Or'd FM_C and FM_8 */
  int map_mode;	/* Or'd MM_F, FM_N and FM_R */
};

typedef void (*cbfn_t)(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms);



struct groupinfo gi[] = {
  { "X11" },
};

struct fontinfo fi[] = {
  { 0, "6x10.bdf", "6x10", 0, 1, BM_T|BM_M|BM_8, FM_C|FM_8, MM_F|MM_R|MM_N }
};

char *bdf_path = "../bdf/";
char *bdfconv_path = "../bdfconv/bdfconv";
FILE *u8g2_font_list_fp;
FILE *u8x8_font_list_fp;


char target_font_identifier[1024];
char bdf_cmd[2048];

void bdfconv(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms)
{
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
  if ( mm == MM_N ) strcat(bdf_cmd, " -m '32,42-58>32'");

  strcat(bdf_cmd, " ");
  strcat(bdf_cmd, bdf_path);
  strcat(bdf_cmd, fi[i].filename);


  strcat(bdf_cmd, " -n ");
  strcat(bdf_cmd, target_font_identifier);

  strcat(bdf_cmd, " -o ");
  //strcat(bdf_cmd, target_font_identifier);
  strcat(bdf_cmd, "font");
  strcat(bdf_cmd, ".c");
  
  if ( fm == FM_8 ) 
    strcat(bdf_cmd, " && cat font.c >>u8x8_fonts.c");
  else
    strcat(bdf_cmd, " && cat font.c >>u8g2_fonts.c");

  printf("%s\n", bdf_cmd);
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
  
}

void gen_font(int i, int fm, char *fms, int bm, char *bms, int mm, char *mms, cbfn_t cb )
{
  
  if ( fm == FM_8 )
    if ( bm != BM_8 )
      return;
  
  strcpy(target_font_identifier, fms);
  strcat(target_font_identifier, "_");
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
    map_font(i, fm, fms, BM_8, "8", cb);
}

void process_font(int i, cbfn_t cb)
{
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

int main(void)
{
  unlink("u8x8_fonts.c");
  unlink("u8g2_fonts.c");
  
  do_font_loop(bdfconv);
  
  u8g2_font_list_fp = fopen("u8g2_font_list.c", "w");
  u8x8_font_list_fp  = fopen("u8x8_font_list.c", "w");
  fprintf(u8g2_font_list_fp, "/* u8g2_font_list.c */\n");
  fprintf(u8x8_font_list_fp, "/* u8x8_font_list.c */\n");
  fprintf(u8g2_font_list_fp, "#include \"u8g2.h\"\n");
  fprintf(u8x8_font_list_fp, "#include \"u8g2.h\"\n");
  fprintf(u8g2_font_list_fp, "uint8_t *u8g2_font_list[] = {\n");
  fprintf(u8x8_font_list_fp, "uint8_t *u8x8_font_list[] = {\n");
  do_font_loop(fontlist_identifier);
  fprintf(u8g2_font_list_fp, "  NULL\n};\n");
  fprintf(u8x8_font_list_fp, "  NULL\n};\n");

  fprintf(u8g2_font_list_fp, "uint8_t *u8g2_font_names[] = {\n");
  fprintf(u8x8_font_list_fp, "uint8_t *u8x8_font_names[] = {\n");
  do_font_loop(fontlist_name);
  fprintf(u8g2_font_list_fp, "  NULL\n};\n");
  fprintf(u8x8_font_list_fp, "  NULL\n};\n");
  
  fclose(u8g2_font_list_fp);
  fclose(u8x8_font_list_fp);
  return 0;
}
