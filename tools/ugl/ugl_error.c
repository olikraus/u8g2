

#include "ugl.h"
#include <stdio.h>
#include <stdlib.h>

void ugl_err(const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  vprintf(fmt, va);
  printf("\n");
  va_end(va);
  exit(1);
}

void ugl_plog(const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  printf("%d: ", ugl_current_input_line);
  vprintf(fmt, va);
  printf("\n");
  va_end(va);
}

/* generic log */
void ugl_glog(const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  vprintf(fmt, va);
  printf("\n");
  va_end(va);
}