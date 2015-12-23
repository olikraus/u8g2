
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===========================================*/

#define COM_4WSPI		0x0001
#define COM_3WSPI		0x0002
#define COM_6800		0x0004
#define COM_8080		0x0008
#define COM_SSDI2C	0x0010

struct interface
{
  const char *interface_name;		/* SW_SPI */
  const char *setpin_function;		/* u8x8_SetPin_4Wire_SW_SPI */
  const char *arduino_com_procedure;	/* u8x8_byte_4wire_sw_spi */
  const char *arduino_gpio_procedure;	/* u8x8_gpio_and_delay_arduino */
  const char *pins_with_type;		/* uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE */
  const char *pins_plain;			/* clock, data, cs, dc, reset */
};


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
  unsigned com;
  struct display display_list[10];	/* usually not used completly, but space does not matter much here */  
};

struct controller controller_list[] =
{
  {
    "ssd1306", 	16, 	8, 	"u8x8_cad_001", COM_4WSPI|COM_3WSPI|COM_6800|COM_8080|COM_8080|COM_SSDI2C,
    {
      { "128x64_noname" },
      { NULL }
    }
  },
  {
    "uc1701", 		13, 	8, 	"u8x8_cad_001", COM_4WSPI|COM_3WSPI|COM_6800|COM_8080|COM_8080,
    {
      { "dogs102" },
      { NULL }
    }
  }
};

struct interface interface_list[] = 
{
  /* 0 */
  {
    "4W_SW_SPI",
    "u8x8_SetPin_4Wire_SW_SPI",
    "u8x8_byte_4wire_sw_spi",
    "u8x8_gpio_and_delay_arduino",
    "uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE",
    "clock, data, cs, dc, reset"
  },
  /* 1 */
  {
    "4W_HW_SPI",
    "u8x8_SetPin_4Wire_HW_SPI",
    "u8x8_byte_arduino_hw_spi",
    "u8x8_gpio_and_delay_arduino",   
    "uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE",
    "cs, dc, reset"
  },  
  /* 2 */
  {
    "6800",
    "u8x8_SetPin_8Bit_6800",
    "u8x8_byte_8bit_6800mode",
    "u8x8_gpio_and_delay_arduino",   
    "uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE",
    "d0,  d1,  d2,  d3,  d4,  d5,  d6,  d7,  enable,  cs,  dc,  reset"
  },
  /* 3 */
  {
    "8080",
    "u8x8_SetPin_8Bit_8080",
    "u8x8_byte_8bit_8080mode",
    "u8x8_gpio_and_delay_arduino",   
    "uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t enable, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE",
    "d0,  d1,  d2,  d3,  d4,  d5,  d6,  d7,  enable,  cs,  dc,  reset"
  },
  /* 4 */
  {
    "3W_SW_SPI",
    "u8x8_SetPin_3Wire_SW_SPI",
    "u8x8_byte_3wire_sw_spi",
    "u8x8_gpio_and_delay_arduino",
    "uint8_t clock, uint8_t data, uint8_t cs, uint8_t reset = U8X8_PIN_NONE",
    "clock, data, cs, reset"
  },
  /* 5 */
  {
    "3W_HW_SPI",
    "",
    "",
    "",   
    "uint8_t cs, uint8_t reset = U8X8_PIN_NONE",
    "cs, reset"
  },  
  /* 6 */
  {
    "SW_I2C",
    "u8x8_SetPin_SSD13xx_SW_I2C",
    "u8x8_byte_ssd13xx_sw_i2c",
    "u8x8_gpio_and_delay_arduino",
    "uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE",
    "clock,  data,  reset"
  },
  /* 7 */
  {
    "HW_I2C",
    "",
    "",
    "",   
    "uint8_t reset = U8X8_PIN_NONE",
    "reset"
  },  
  
  
};


/*===========================================*/

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
  Insert file "insertname" between lines "start_line" and "end_line" of file "filename"
*/
int insert_into_file(const char *filename, const char *insertname, const char *start_line, const char *end_line)
{
  int ch;
  static char line[1024*4];
  const char *tmpname = "tmp.h";
  FILE *source_fp;
  FILE *dest_fp;
  FILE *insert_fp;
  
  if ( file_copy(filename, tmpname) == 0 )
    return 0;

  source_fp = fopen(tmpname, "r");
  dest_fp = fopen(filename, "w");
  insert_fp = fopen(insertname, "r");

  if ( source_fp == NULL || dest_fp == NULL || insert_fp == NULL )
    return 0;
  
  for(;;)
  {
    if ( fgets(line, 1024*4, source_fp) == NULL )
      break;
    if ( strncmp(line, start_line, strlen(start_line)) == 0 )
    {
      fputs(line, dest_fp);

      while( ( ch = fgetc(insert_fp) ) != EOF )
	fputc(ch, dest_fp);
      
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

  fclose(insert_fp);
  fclose(source_fp);
  fclose(dest_fp);
  
  unlink(tmpname);

  return 1;
}

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
FILE *u8g2_cpp_header_fp;


void do_setup_prototype(FILE *fp, int controller_idx, int display_idx, const char *postfix)
{
  fprintf(fp, "void u8g2_Setup_");
  fprintf(fp, "%s_", strlowercase(controller_list[controller_idx].name));
  fprintf(fp, "%s_", strlowercase(controller_list[controller_idx].display_list[display_idx].name));
  fprintf(fp, "%s(u8g2_t *u8g2, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb, const u8g2_cb_t *rotation)", postfix);
}

/*===========================================*/
/*
class U8G2_SSD1306_128x64_NONAME_1_SW_SPI : public U8G2
{
  public:
    U8G2_SSD1306_128x64_NONAME_1_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE)
    {
      u8g2_Setup_ssd1306_128x64_noname_1(&u8g2, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_arduino, rotation);
      u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
    }
};
*/

void do_display_interface(int controller_idx, int display_idx, const char *postfix, int interface_idx)
{
  FILE *fp = u8g2_cpp_header_fp;
  fprintf(fp, "class U8G2_");
  fprintf(fp, "%s_", struppercase(controller_list[controller_idx].name));
  fprintf(fp, "%s_", struppercase(controller_list[controller_idx].display_list[display_idx].name));
  fprintf(fp, "%s_", struppercase(postfix));
  fprintf(fp, "%s", struppercase(interface_list[interface_idx].interface_name));  
  fprintf(fp, " : public U8G2 {\n");
  fprintf(fp, "  public: ");
  fprintf(fp, "U8G2_");
  fprintf(fp, "%s_", struppercase(controller_list[controller_idx].name));
  fprintf(fp, "%s_", struppercase(controller_list[controller_idx].display_list[display_idx].name));
  fprintf(fp, "%s_", struppercase(postfix));
  fprintf(fp, "%s", struppercase(interface_list[interface_idx].interface_name));  
  fprintf(fp, "(const u8g2_cb_t *rotation, ");
  fprintf(fp, "%s) : U8G2() {\n", interface_list[interface_idx].pins_with_type);
  fprintf(fp, "    ");
  fprintf(fp, "u8g2_Setup_");
  fprintf(fp, "%s_", strlowercase(controller_list[controller_idx].name));
  fprintf(fp, "%s_", strlowercase(controller_list[controller_idx].display_list[display_idx].name));
  fprintf(fp, "%s", postfix);
  fprintf(fp, "(&u8g2, ");
  fprintf(fp, "%s, ", interface_list[interface_idx].arduino_com_procedure);  
  fprintf(fp, "%s, ", interface_list[interface_idx].arduino_gpio_procedure);  
  fprintf(fp, "rotation);\n");
  fprintf(fp, "    ");
  fprintf(fp, "%s(getU8x8(), ", interface_list[interface_idx].setpin_function);  
  fprintf(fp, "%s);\n", interface_list[interface_idx].pins_plain);
  fprintf(fp, "  }\n");
  fprintf(fp, "};\n");
  
  
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
  fprintf(setup_code_fp, "  uint8_t *buf;\n");
  fprintf(setup_code_fp, "  u8g2_SetupDisplay(u8g2, u8x8_d_");
  fprintf(setup_code_fp, "%s_", strlowercase(controller_list[controller_idx].name));
  fprintf(setup_code_fp, "%s, ", strlowercase(controller_list[controller_idx].display_list[display_idx].name));
  fprintf(setup_code_fp, "%s, ", controller_list[controller_idx].cad);
  fprintf(setup_code_fp, "byte_cb, gpio_and_delay_cb);\n");    
  fprintf(setup_code_fp, "  buf = ");
  fprintf(setup_code_fp, "u8g2_m_%s_%d_%s(&tile_buf_height);\n", strlowercase(controller_list[controller_idx].name), controller_list[controller_idx].tile_width, postfix);
  fprintf(setup_code_fp, "  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, rotation);\n");
  fprintf(setup_code_fp, "}\n");
  
  /* generate interfaces for this display */
  if ( controller_list[controller_idx].com & COM_4WSPI )
  {
    do_display_interface(controller_idx, display_idx, postfix, 0);		/* SW SPI */
    do_display_interface(controller_idx, display_idx, postfix, 1);		/* HW SPI */
  }
  if ( controller_list[controller_idx].com & COM_3WSPI )
  {
    do_display_interface(controller_idx, display_idx, postfix, 4);		/* 3wire SW SPI */
    //do_display_interface(controller_idx, display_idx, postfix, 5);		/* 3wire HW SPI (not implemented) */
  }
  if ( controller_list[controller_idx].com & COM_6800 )
  {
    do_display_interface(controller_idx, display_idx, postfix, 2);		/* 6800 mode */    
  }
  if ( controller_list[controller_idx].com & COM_8080 )
  {
    do_display_interface(controller_idx, display_idx, postfix, 3);		/* 8080 mode */    
  }
  if ( controller_list[controller_idx].com & COM_SSDI2C )
  {
    do_display_interface(controller_idx, display_idx, postfix, 6);		/* SW SPI */
    //do_display_interface(controller_idx, display_idx, postfix, 7);		/* HW SPI (not yet implemented) */
  }
  
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
  //fprintf(buf_header_fp, "/* start of generated code, codebuild, u8g2 project */\n");

  setup_code_fp = fopen("u8g2_d_setup.c", "w");
  fprintf(setup_code_fp, "/* u8g2_d_setup.c */\n");
  fprintf(setup_code_fp, "/* generated code, codebuild, u8g2 project */\n");
  fprintf(setup_code_fp, "\n");
  fprintf(setup_code_fp, "#include \"u8g2.h\"\n");
  fprintf(setup_code_fp, "\n");

  setup_header_fp = fopen("u8g2_setup.h", "w");
  //fprintf(setup_header_fp, "/* start of generated code, codebuild, u8g2 project */\n");
  
  u8g2_cpp_header_fp = fopen("U8g2lib.h", "w");
  fprintf(u8g2_cpp_header_fp, "/* generated code (codebuild), u8g2 project */\n");
  
  
  do_controller_list();
  
  fprintf(buf_code_fp, "/* end of generated code */\n");
  fclose(buf_code_fp);
  
  //fprintf(buf_header_fp, "/* end of generated code */\n");
  fclose(buf_header_fp);

  fprintf(setup_code_fp, "/* end of generated code */\n");
  fclose(setup_code_fp);

  //fprintf(setup_header_fp, "/* end of generated code */\n");
  fclose(setup_header_fp);

  fclose(u8g2_cpp_header_fp);

  system("cp u8g2_d_memory.c ../../csrc/.");
  system("cp u8g2_d_setup.c ../../csrc/.");

  insert_into_file("../../csrc/u8g2.h", "u8g2_memory.h", "/* u8g2_d_memory.c generated code start */", "/* u8g2_d_memory.c generated code end */");
  insert_into_file("../../csrc/u8g2.h", "u8g2_setup.h", "/* u8g2_d_setup.c generated code start */", "/* u8g2_d_setup.c generated code end */");
  insert_into_file("../../cppsrc/U8g2lib.h", "U8g2lib.h", "/* Arduino constructor list start */", "/* Arduino constructor list end */");

  return 0;
}

