
#define BM_T	1	/* Transparent = build mode 0 proportional */
#define BM_H	2	/* Common Height = build mode 1 */
#define BM_M	4	/* Monospace = build mode 2 */
#define BM_8	8	/* 8x8 = build mode 3 */
#define FM_C	1	/* u8g2 compressed font */
#define FM_8	2	/* u8x8 uncompressed font */

struct groupinfo
{
  char *groupname;
};

struct fontinfo
{
  int is_ttf;		/* 0 = bdf, 1= ttf */
  char *filename;	/* filename including extension */
  int	pt;			/* point size, ttf only */
  int build_mode;	/* Or'd BM_T, BM_H, BM_M, BM_8 */
  int font_mode;	/* Or'd FM_C and FM_8 */
  int group;		/* group-index */
};


struct fontinfo
{
}