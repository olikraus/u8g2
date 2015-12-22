
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
    "ssd1306", 	16, 	8, 	"u8x8_cad_001",
    {
      { "128x64_noname" },
      { NULL }
    }
  },
  {
    "uc1701", 		13, 	8, 	"u8x8_cad_001",
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
  for( i = 0; i <= len; i++ )
    buf[i] = tolower(s[i]);
  return buf;
}

char *struppercase(const char *s)
{
  int i, len = strlen(s);
  static char buf[1024];
  for( i = 0; i <= len; i++ )
    buf[i] = toupper(s[i]);
  return buf;
}

/*===========================================*/

FILE *buf_code_fp;
FILE *buf_header_fp;
FILE *setup_code_fp;
FILE *setup_header_fp;


void do_setup_prototype(FILE *fp, int controller_idx, int display_idx, const char *postfix)
{
  fprintf(fp, "uint8_t *u8g2_Setup_");
  fprintf(fp, "%s_", strlowercase(controller_list[controller_idx].name));
  fprintf(fp, "%s_", strlowercase(controller_list[controller_idx].display_list[display_idx].name));
  fprintf(fp, "%s(uint8_t *u8g2, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb, const u8g2_cb_t *rotation)", postfix);
}

/*===========================================*/

void do_display(int controller_idx, int display_idx, const char *postfix)
{
  do_setup_prototype(setup_header_fp, controller_idx, display_idx, postfix);
  fprintf(setup_header_fp, ";\n");
  
  do_setup_prototype(setup_code_fp, controller_idx, display_idx, postfix);
  fprintf(setup_code_fp, "\n");
  fprintf(setup_code_fp, "{\n");
  fprintf(setup_code_fp, "  uint8_t tile_buf_height;\n");
  fprintf(setup_code_fp, "  uint8_t *buffer;\n");
  fprintf(setup_code_fp, "  u8g2_SetupDisplay(u8x8_d_");
  fprintf(setup_code_fp, "%s_", strlowercase(controller_list[controller_idx].name));
  fprintf(setup_code_fp, "%s, ", strlowercase(controller_list[controller_idx].display_list[display_idx].name));
  fprintf(setup_code_fp, "%s, ", controller_list[controller_idx].cad);
  fprintf(setup_code_fp, "byte_cb, gpio_and_delay_cb);\n");    
  fprintf(setup_code_fp, "  buffer = ");
  fprintf(setup_code_fp, "u8g2_m_%s_%d_%s(&tile_buf_height);\n", strlowercase(controller_list[controller_idx].name), controller_list[controller_idx].tile_width, postfix);
  fprintf(setup_code_fp, "  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, rotation);\n");
  fprintf(setup_code_fp, "}\n");
}

void do_controller_buffer_code(int idx, const char *postfix, int buf_len, int rows)
{
  int display_idx;
  FILE *fp = stdout;
  fprintf(buf_code_fp, "uint8_t *u8g2_m_%s_%d_%s(uint8_t *page_cnt)\n", 
    strlowercase(controller_list[idx].name), controller_list[idx].tile_width, postfix);
  fprintf(buf_code_fp, "{\n");
  fprintf(buf_code_fp, "  static uint8_t buf[%d];\n", buf_len);
  fprintf(buf_code_fp, "  *page_cnt = %d;\n", rows);
  fprintf(buf_code_fp, "  return buf;\n");
  fprintf(buf_code_fp, "}\n");
  
  fprintf(buf_header_fp, "uint8_t *u8g2_m_%s_%d_%s(uint8_t *page_cnt);\n", 
    strlowercase(controller_list[idx].name), controller_list[idx].tile_width, postfix);
  
  display_idx = 0;
  fprintf(setup_code_fp, "/* %s %s */\n", controller_list[idx].name, postfix);
  while( controller_list[idx].display_list[display_idx].name != NULL )
  {
    do_display(idx, display_idx, postfix);
    display_idx++;
  }
  
}



void do_controller_list(void)
{
  int i;
  for( i = 0; i < sizeof(controller_list)/sizeof(*controller_list); i++ )
  {
    puts(controller_list[i].name);
    fprintf(setup_code_fp, "/* %s */\n", controller_list[i].name);
    do_controller_buffer_code(i, "1", controller_list[i].tile_width*8, 1);
    do_controller_buffer_code(i, "2", controller_list[i].tile_width*8*2, 2);
    do_controller_buffer_code(i, "f", controller_list[i].tile_width*8*controller_list[i].tile_height, controller_list[i].tile_height);
  }
}

int main(void)
{
  buf_code_fp = fopen("u8g2_d_memory.c", "w");
  fprintf(buf_code_fp, "/* u8g2_d_memory.c */\n");
  fprintf(buf_code_fp, "/* generated code, codebuild, u8g2 project */\n");
  fprintf(buf_code_fp, "\n");
  fprintf(buf_code_fp, "#include \"u8g2.h\"\n");
  fprintf(buf_code_fp, "\n");
  
  buf_header_fp = fopen("u8g2_memory.h", "w");
  fprintf(buf_header_fp, "/* start of generated code, codebuild, u8g2 project */\n");

  setup_code_fp = fopen("u8g2_d_setup.c", "w");
  fprintf(setup_code_fp, "/* u8g2_d_setup.c */\n");
  fprintf(setup_code_fp, "/* generated code, codebuild, u8g2 project */\n");
  fprintf(setup_code_fp, "\n");
  fprintf(setup_code_fp, "#include \"u8g2.h\"\n");
  fprintf(setup_code_fp, "\n");

  setup_header_fp = fopen("u8g2_setup.h", "w");
  fprintf(setup_header_fp, "/* start of generated code, codebuild, u8g2 project */\n");
  
  do_controller_list();
  
  fprintf(buf_code_fp, "/* end of generated code */\n");
  fclose(buf_code_fp);
  
  fprintf(buf_header_fp, "/* end of generated code */\n");
  fclose(buf_header_fp);

  fprintf(setup_code_fp, "/* end of generated code */\n");
  fclose(setup_code_fp);

  fprintf(setup_header_fp, "/* end of generated code */\n");
  fclose(setup_header_fp);

  return 0;
}

