

#include "ugl.h"
#include "ugl_bc.h"
#include <stdio.h>

#define UGL_MAX_INPUT_LINE_LEN 1024
FILE *ugl_input_fp;
char ugl_input_line[UGL_MAX_INPUT_LINE_LEN];


int ugl_read_fp(void)
{
  const char *s;
  ugl_current_input_line = 0;
  for(;;)
  {
    if ( fgets(ugl_input_line, UGL_MAX_INPUT_LINE_LEN, ugl_input_fp) == NULL )
      break;
    ugl_current_input_line++;
    s = &(ugl_input_line[0]);
    if ( ugl_read_line(&s) == 0 )
      return 0;
  }
  return 1;
}

int ugl_read_filename(const char *name)
{
  ugl_input_fp = fopen(name, "r");
  if ( ugl_input_fp == NULL )
    return 0;
  printf("file '%s'\n", name);
  if ( ugl_read_fp() == 0 )
    return fclose(ugl_input_fp), 0;
  fclose(ugl_input_fp);
  return 1;
}

extern uint8_t ugl_bytecode_array[];
void ugl_ExecBytecode(void)
{
  bc_t bc;
  bc_exec(&bc, ugl_bytecode_array, 0);
}

int main(void)
{
  ugl_InitBytecode();
  ugl_read_filename("test.ugl");
  ugl_ResolveSymbols();
  ugl_ExecBytecode();
  ugl_WriteBytecodeCArray(stdout, "code");
  
}