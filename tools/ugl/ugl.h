/*

  proc <name> 

*/


#ifndef _UGL_H
#define _UGL_H

#include <stdint.h>
#include <stdarg.h>


#define UGL_MAX_IDENTIFIER_LEN 63


/* ugl_error.c */
void ugl_err(const char *fmt, ...)  __attribute__ ((noreturn));
void ugl_plog(const char *fmt, ...);



/* ugl_arrays.c */
extern uint16_t ugl_bytecode_len;

void ugl_InitBytecode(void);
void ugl_AddBytecode(uint8_t x);
void ugl_ExecBytecode(void);
void ugl_ResolveSymbols(void);

int ugl_GetLabel(const char *name);
void ugl_SetLabelBytecodePos(const char *name, uint16_t bytecode_pos);
uint16_t ugl_GetLabelBytecodePos(int idx);


/* ugl_file.c */
extern int ugl_current_input_line;

int ugl_read_filename(const char *name);



#endif
