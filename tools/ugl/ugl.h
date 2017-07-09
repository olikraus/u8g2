/*

  proc <name> 

*/


#ifndef _UGL_H
#define _UGL_H

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>


#define UGL_MAX_IDENTIFIER_LEN 63


/* ugl_error.c */
extern int ugl_current_input_line;
extern int ugl_is_suppress_log;
void ugl_err(const char *fmt, ...)  __attribute__ ((noreturn));
void ugl_plog(const char *fmt, ...);
void ugl_glog(const char *fmt, ...);



/* ugl_arrays.c */
extern uint16_t ugl_bytecode_len;

void ugl_InitBytecode(void);
void ugl_AddBytecode(uint8_t x);
void ugl_ExecBytecode(void);
void ugl_ResolveSymbols(void);
void ugl_WriteBytecodeCArray(FILE *fp, const char *name);


int ugl_GetLabel(const char *name);
void ugl_SetLabelBytecodePos(const char *name, uint16_t bytecode_pos);
uint16_t ugl_GetLabelBytecodePos(int idx);


/* ugl_parse.c */

uint16_t uglStartNamelessProc(int args);
int ugl_read_line(const char **s);
int uglReadLine(const char **s);


/* ugl_main.c */

int ugl_read_fp(void);
int ugl_read_filename(const char *name);



#endif
