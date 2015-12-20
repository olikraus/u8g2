
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===========================================*/

struct display
{
  const char *name;
};

struct controller
{
  const char *name;
  int tile_width;
  int tile_height;
  const char *cad;
  struct display display_list[10];	/* usually not used completly, but space does not matter much here */  
};

struct controller controller_list[] =
{
  {
    "SSD1306", 	16, 	8, 	"u8x8_cad_001",
    {
      { "128x64_noname" },
      { NULL }
    }
  },
  {
    "UC1701", 	9, 	8, 	"u8x8_cad_001",
    {
      { "dogs102" },
      { NULL }
    }
  }
};

/*===========================================*/

char *strlowercase(const char *s)
{
  int i, len = strlen(s);
  static char buf[1024];
  for( i = 0; i < len; i++ )
    buf[i] = tolower(s[i]);
  return buf;
}

/*===========================================*/

FILE *buf_code_fp;
FILE *buf_header_fp;
FILE *setup_code_fp;



/*===========================================*/

void do_controller_buffer_code(int idx, const char *postfix, int buf_len, int rows)
{
  int display_idx;
  FILE *fp = stdout;
  fprintf(buf_code_fp, "uint8_t *u8g2_m_%s_%s(uint8_t *page_cnt)\n", 
    strlowercase(controller_list[idx].name), postfix);
  fprintf(buf_code_fp, "{\n");
  fprintf(buf_code_fp, "  static uint8_t buf[%d];\n", buf_len);
  fprintf(buf_code_fp, "  *page_cnt = %d;\n", rows);
  fprintf(buf_code_fp, "  return buf;\n");
  fprintf(buf_code_fp, "}\n");
  
  fprintf(buf_header_fp, "uint8_t *u8g2_m_%s_%s(uint8_t *page_cnt);\n", 
    strlowercase(controller_list[idx].name), postfix);
  
  display_idx = 0;
  while( controller_list[idx].display_list[display_idx].name != NULL )
  {
    printf("%s\n", controller_list[idx].display_list[display_idx].name);
    display_idx++;
  }
  
}



void do_controller_list(void)
{
  int i;
  for( i = 0; i < sizeof(controller_list)/sizeof(*controller_list); i++ )
  {
    puts(controller_list[i].name);
    do_controller_buffer_code(i, "1", controller_list[i].tile_width*8, 1);
    do_controller_buffer_code(i, "2", controller_list[i].tile_width*8*2, 2);
    do_controller_buffer_code(i, "f", controller_list[i].tile_width*8*controller_list[i].tile_height, controller_list[i].tile_height);
  }
}

int main(void)
{
  buf_code_fp = fopen("u8g2_memory.c", "w");
  fprintf(buf_code_fp, "/* start of generated code, codebuild, u8g2 project */\n");
  
  buf_header_fp = fopen("u8g2_memory.h", "w");
  fprintf(buf_header_fp, "/* start of generated code, codebuild, u8g2 project */\n");

  setup_code_fp = fopen("u8g2_setup.c", "w");
  fprintf(buf_code_fp, "/* start of generated code, codebuild, u8g2 project */\n");
  
  do_controller_list();
  
  fprintf(buf_code_fp, "/* end of generated code */\n");
  fclose(buf_code_fp);
  
  fprintf(buf_header_fp, "/* end of generated code */\n");
  fclose(buf_header_fp);
  
  return 0;
}

