/*
  
    ugl_file.c
    
    microcontroller game language
    
    bytecode
      return from procedure... all procedure leave a 16 bit value on the stack
      jump	probably only for if/loop
      pop stack and jump on zero
      pop stack and jump on not zero
      call buildin procedure
      call bytecode procedure
      call toplevel buildin procedure
      call toplevel bytecode procedure
      push constant 16 bit value on stack
      pop constant value from stack
      
      
      
  proc <name>		Starts a new procedure. Procedures must be terminated with "endproc"
  endproc			End a procedure
  locals <num>	Define the specified number of local variables, acces with arg(n), where n is between no_of_args+1 and no_of_args+<num>
*/
#include <stdio.h>
#include <string.h>
#include "ugl.h"
#include "ugl_bc.h"


long ugl_current_local_variables = 0;
long ugl_current_args = 0;


#define UGL_MAX_INDENT 64

#define UGL_INDENT_TYPE_PROC 0
#define UGL_INDENT_TYPE_IF 1
#define UGL_INDENT_TYPE_LOOP 2

int ugl_indent_level;
int ugl_indent_type[UGL_MAX_INDENT];
char ugl_indent_identifier[UGL_MAX_INDENT][UGL_MAX_IDENTIFIER_LEN+1];

struct ugl_buildin_cmd_struct
{
  int code;
  const char *name;
  int args;
};

struct ugl_buildin_cmd_struct ugl_buildin_cmd_list[] = {
  { /* code=*/ 0, 	/* name=*/ "nop", 		/* args=*/ 0},
  { /* code=*/ 1, 	/* name=*/ "return", 	/* args=*/ 1 },	/* assign the return value of a user defined function */
  { /* code=*/ 2, 	/* name=*/ "a", 		/* args=*/ 1 },	/* return the value of the n-th argument of a user defined function */
  { /* code=*/ 3, 	/* name=*/ "a", 		/* args=*/ 2 }, /* reassign the value of the n-th argument of a user defined function */
  { /* code=*/ 4, 	/* name=*/ "add", 		/* args=*/ 2 },
  { /* code=*/ 5, 	/* name=*/ "print", 		/* args=*/ 1 },
  { /* code=*/ 6, 	/* name=*/ "setPos", 	/* args=*/ 2 },
  { /* code=*/ 7, 	/* name=*/ "setItemPos", /* args=*/ 1 },
  { /* code=*/ 8, 	/* name=*/ "setItemDir",  /* args=*/ 2 },
};



void ugl_IncIndent(int type)
{
  if ( ugl_indent_level > UGL_MAX_INDENT )
    ugl_err("max indent level reached");
  ugl_indent_type[ugl_indent_level] = type;
  ugl_indent_level++;
}

void ugl_DecIndent(int type)
{
  if ( ugl_indent_level == 0 )
    ugl_err("can not decrement indent level, requested type = %d", type);
  ugl_indent_level--;
  if (ugl_indent_type[ugl_indent_level] != type )
    ugl_err("block end mismatch, %d != %d", ugl_indent_type[ugl_indent_level] , type );
}

static void skip_space(const char **s)
{
  for(;;)
  {
    if ( **s == '#' )
    {
      while( **s != '\0' )
	(*s)++;
      break;
    }
    if ( **s == '\0' )
      break;
    if ( **s > ' ' )
      break;
    (*s)++;
  }
}

static long get_dec(const char **s)
{
  long v = 0;
  for(;;)
  {
    if ( (**s) >= '0' && (**s)  <= '9' )
    {
      v*=10;
      v+= (**s)-'0';
      (*s)++;
    }
    else
    {
      break;
    }
  }  
  skip_space(s);
  return v;
}

static long get_hex(const char **s)
{
  long v = 0;
  for(;;)
  {
    if ( (**s) >= '0' && (**s)  <= '9' )
    {
      v*=16;
      v+= (**s)-'0';
      (*s)++;
    }
    else if ( (**s) >= 'a' && (**s)  <= 'f' )
    {
      v*=16;
      v+= (**s)-'a'+10;
      (*s)++;
    }
    else if ( (**s) >= 'A' && (**s)  <= 'F' )
    {
      v*=16;
      v+= (**s)-'A'+10;
      (*s)++;
    }
    else
    {
      break;
    }
  }
  skip_space(s);
  return v;
}

static long get_ascii(const char **s)
{
  long v = 0;
  v = **s;
  (*s)++;
  skip_space(s);
  return v;  
}


static long get_num(const char **s)
{
  if ( (**s) == '$' )
  {
    (*s)++;
    return get_hex(s);
  }
  if ( (**s) == '\'' )
  {
    (*s)++;
    return get_ascii(s);
  }
  
  return get_dec(s);
}

static const char *get_identifier(const char **s)
{
  static char buf[UGL_MAX_IDENTIFIER_LEN+1];
  int c;
  int i = 0;
  buf[0] = '\0';
  for(;;)
  {
    c = **s;
    if ( c < 'A' )
      break;
    if ( i > UGL_MAX_IDENTIFIER_LEN )
      break;
    buf[i++] = c;
    buf[i] = '\0';
    (*s)++;
  }
  skip_space(s);
  return buf;
}

/*======================================================*/
/* code generator sub procedures */

void ugl_bytecode_buildin_procedure(const char *name, int idx, int is_toplevel)
{
  ugl_plog("BC %scall buildin '%s'", is_toplevel?"toplevel ":"", name);
  
  if ( is_toplevel == 0 )
  {
    ugl_AddBytecode(BC_CMD_CALL_BUILDIN | ((idx&0x0f00)>>4));
    ugl_AddBytecode(idx&0xff);    
  }
  else
  {
    ugl_AddBytecode(BC_CMD_CALL_BUILDIN_POP_STACK | ((idx&0x0f00)>>4));
    ugl_AddBytecode(idx&0xff);        
  }
}

void ugl_bytecode_call_procedure(const char *name, int is_toplevel, int arg_cnt)
{
  uint16_t idx;
  
  ugl_plog("BC call %sbytecode '%s' with %d arg(s)", is_toplevel?"toplevel ":"", name, arg_cnt);  
  idx = ugl_GetLabel(name);

  ugl_AddBytecode(BC_CMD_CALL_PROCEDURE | (arg_cnt<<4));
  
  ugl_AddBytecode(idx>>8);    
  ugl_AddBytecode(idx&0x0ff);

  if ( is_toplevel != 0 )
  {
    ugl_AddBytecode(BC_CMD_POP_ARG_STACK );		// remove the return value from the stack
  }
  
#ifdef NOTUSED
  if ( is_toplevel != 0 )
  {
    arg_cnt++;		// one more element on the stack (return value) has to be removed, if this is called from top level
  }
  
  while( arg_cnt > 16 )
  {
    ugl_AddBytecode(BC_CMD_POP_ARG_STACK | 0x0f0);
    arg_cnt -= 16;
  }
  if ( arg_cnt > 0 )
  {
    arg_cnt--;
    ugl_AddBytecode(BC_CMD_POP_ARG_STACK | (arg_cnt<<4));    
  }
#endif
}

void ugl_bytecode_reserve_arg_stack(int arg_cnt)
{
  while( arg_cnt > 16 )
  {
    ugl_AddBytecode(BC_CMD_PUSH_ARG_STACK | 0x0f0);
    arg_cnt -= 16;
  }
  if ( arg_cnt > 0 )
  {
    arg_cnt--;
    ugl_AddBytecode(BC_CMD_PUSH_ARG_STACK | (arg_cnt<<4));    
  }
}

void ugl_bytecode_remove_arg_stack(int arg_cnt)
{
  while( arg_cnt > 16 )
  {
    ugl_AddBytecode(BC_CMD_POP_ARG_STACK | 0x0f0);
    arg_cnt -= 16;
  }
  if ( arg_cnt > 0 )
  {
    arg_cnt--;
    ugl_AddBytecode(BC_CMD_POP_ARG_STACK | (arg_cnt<<4));    
  }
}

void ugl_bytecode_constant_value(long num)
{  
  ugl_plog("BC constant value %ld", num);  
 
  if ( num == 0 )
  {
    ugl_AddBytecode(BC_CMD_LOAD_0);
  }
  else if ( num == 1 )
  {
    ugl_AddBytecode(BC_CMD_LOAD_1);
  }
  else if ( num <= 0x0fff )
  {
    ugl_AddBytecode(BC_CMD_LOAD_12BIT | ((num&0x0f00)>>4));
    ugl_AddBytecode(num&0xff);    
  }
  else
  {
    ugl_AddBytecode(BC_CMD_LOAD_16BIT );    
    ugl_AddBytecode(num>>8);    
    ugl_AddBytecode(num&0x0ff);
  }  
}

void ugl_bytecode_return_from_procedure(void)
{
    ugl_plog("BC return from procedure");
    ugl_AddBytecode(BC_CMD_RETURN_FROM_PROCEDURE );    
}

void ugl_bytecode_branch(const char *s)
{
  int idx;
  idx = ugl_GetLabel(s);
  ugl_plog("BC branch, label %s, idx=%d", s, idx);
  
  ugl_AddBytecode(BC_CMD_BRANCH | ((idx&0x0f00)>>4));
  ugl_AddBytecode(idx&0xff);    
  
  
}

void ugl_bytecode_jmp_no_zero(const char *s)
{
  int idx;
  idx = ugl_GetLabel(s);
  ugl_plog("BC jump on not zero, label idx=%d", idx);
  
  ugl_AddBytecode(BC_CMD_JUMP_NOT_ZERO );
  ugl_AddBytecode(idx>>8);    
  ugl_AddBytecode(idx&0x0ff);
}

void ugl_bytecode_jmp_zero(const char *s)
{
  int idx;
  idx = ugl_GetLabel(s);
  ugl_plog("BC jump on zero, label idx=%d", idx);
  
  ugl_AddBytecode(BC_CMD_JUMP_ZERO );
  ugl_AddBytecode(idx>>8);    
  ugl_AddBytecode(idx&0x0ff);
}


/*======================================================*/

int ugl_is_buildin_cmd(const char *name)
{
  int i, cnt;
  cnt = sizeof(ugl_buildin_cmd_list)/sizeof(*ugl_buildin_cmd_list);
  for( i = 0; i < cnt; i++ )
  {
    if ( strcmp(ugl_buildin_cmd_list[i].name, name) == 0 )
      return 1;
  }
  return 0;
}

void ugl_call_proc(const char *name, int is_toplevel, int arg_cnt)
{
  int i, cnt;
  int ii;
  cnt = sizeof(ugl_buildin_cmd_list)/sizeof(*ugl_buildin_cmd_list);
  ii = cnt;
  for( i = 0; i < cnt; i++ )
  {
    if ( strcmp(ugl_buildin_cmd_list[i].name, name) == 0 )
      ii = i;
    if ( strcmp(ugl_buildin_cmd_list[i].name, name) == 0 && ugl_buildin_cmd_list[i].args == arg_cnt)
      break;
  }
  if ( i < cnt )
  {
    ugl_bytecode_buildin_procedure(name, i, is_toplevel);
  }
  else
  {
    if ( ii != cnt )
    {
      ugl_err("Buildin procedure '%s' expects differnt number of args", name);
    }
    else
    {
      ugl_bytecode_call_procedure(name, is_toplevel, arg_cnt);
    }
  }
}

void ugl_parse_proc(const char **s, const char *id, int is_toplevel)
{
  char procname[UGL_MAX_IDENTIFIER_LEN];
  int arg_cnt = 0;
  ugl_plog("parse procedure '%s'", id);
  strcpy(procname, id);
  if ( ugl_is_buildin_cmd(id) == 0 )
  {
    /* if this is a buildin cmd, then do nothing: buildin code takes care on the return value */
    /* for custom procedures push a value on the arg stack for the return value, but do this only if this is not a toplevel procedure */
    ugl_bytecode_constant_value(0);		/* return value will be 0 by default */
  }
  
  if ( **s == '(' )
  {
    const char *name;
    for(;;)
    {
      (*s)++;
      skip_space(s);
      
      if ( **s == ')' )
	break;
      
      if ( (**s >= '0' && **s <= '9') || **s == '$' || **s == '\'' )
      {
	ugl_bytecode_constant_value(get_num(s));
      }
      else
      {
	name = get_identifier(s);
	ugl_parse_proc(s, name, 0);
      }
      arg_cnt++;
      if ( **s != ',' )
	break;
    }
    if ( **s != ')' )
      ugl_err("missing ')'");    
    (*s)++;
    skip_space(s);
    ugl_call_proc(procname, is_toplevel, arg_cnt);
  }
  else
  {
    ugl_call_proc(procname, is_toplevel, 0);
  }
}

uint16_t  uglStartNamelessProc(int args)
{
    ugl_current_local_variables = 0;
    ugl_current_args = args;
    if ( ugl_indent_level != 0 )
      ugl_err("nested procedures not allowed");
    ugl_IncIndent(UGL_INDENT_TYPE_PROC);
    return ugl_bytecode_len;
}

int ugl_read_line(const char **s)
{
  const char *id;
  
  skip_space(s);
  if ( **s == '\0' )
    return 1;
  
  
  id = get_identifier(s);
  
  if ( strcmp(id, "proc") == 0 )
  {
    const char *name = get_identifier(s);
    long args = get_num(s);
    
    ugl_plog("start procedure '%s' (args=%ld)", name, args);
    
    ugl_GetLabel(name);	/* create a label for the procedure name */
    ugl_SetLabelBytecodePos(name, uglStartNamelessProc(args)); /* and set the label for it */

    
  }
  else if ( strcmp(id, "endproc") == 0 )
  {
    //ugl_bytecode_remove_arg_stack(ugl_current_local_variables);
    ugl_DecIndent(UGL_INDENT_TYPE_PROC);        
    ugl_bytecode_return_from_procedure();
    ugl_current_local_variables = 0;
    ugl_current_args = 0;
  }
  else if ( strcmp(id, "locals") == 0 )
  {
    long n = get_num(s);
    if ( ugl_current_local_variables != 0 )
      ugl_err("only one 'locals' command allowed");
    if ( ugl_indent_level != 1 )
      ugl_err("'locals': only toplevel call allowed");	/* it must not be inside loops or ifs */
    ugl_current_local_variables = n;
    ugl_bytecode_reserve_arg_stack(ugl_current_local_variables);
  }
  else if ( strcmp(id, "if" ) == 0 )
  {
    ugl_parse_proc(s, get_identifier(s), 0);
    sprintf(ugl_indent_identifier[ugl_indent_level], ".if%d", ugl_bytecode_len);    /* use the current bytecode position as unique identifier */
    ugl_bytecode_jmp_zero(ugl_indent_identifier[ugl_indent_level]);
    ugl_IncIndent(UGL_INDENT_TYPE_IF);    
    
  }
  else if ( strcmp(id, "else" ) == 0 )
  {
    char s[UGL_MAX_IDENTIFIER_LEN+1];
    sprintf(s, ".else%d", ugl_bytecode_len);    /* use the current bytecode position as unique identifier */    
    ugl_bytecode_branch(s);    
    
    ugl_SetLabelBytecodePos(ugl_indent_identifier[ugl_indent_level-1], ugl_bytecode_len);
    strcpy(ugl_indent_identifier[ugl_indent_level-1], s);
  }
  else if ( strcmp(id, "endif" ) == 0 )
  {
    ugl_DecIndent(UGL_INDENT_TYPE_IF);    
    ugl_SetLabelBytecodePos(ugl_indent_identifier[ugl_indent_level], ugl_bytecode_len);
  }
  else 
  {
    ugl_parse_proc(s, id, 1);
  }
  return 1;
}

/* returns 0 if "endproc" is found */
int uglReadLine(const char **s)
{
  ugl_read_line(s);
  if ( ugl_indent_level == 0 )
    return 0;
  return 1;  
}
