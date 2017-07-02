

#include "ugl.h"

int main(void)
{
  ugl_InitBytecode();
  ugl_read_filename("test.ugl");
  ugl_ResolveSymbols();
  ugl_ExecBytecode();

}