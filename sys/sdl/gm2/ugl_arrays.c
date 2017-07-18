

#include "ugl.h"
#include "ugl_bc.h"
#include <string.h>
#include <stdio.h>

/* arrays & variables */


#define UGL_MAX_BYTECODE_LEN (63*1024)
uint8_t ugl_bytecode_array[UGL_MAX_BYTECODE_LEN];
uint16_t ugl_bytecode_len;

#define UGL_MAX_LABEL_CNT 128
char ugl_label_name[UGL_MAX_LABEL_CNT][UGL_MAX_IDENTIFIER_LEN+1];
uint16_t ugl_label_bytecode_pos[UGL_MAX_LABEL_CNT];  	/* position in the bytecode array */
int ugl_label_cnt;

/* procedures */

void ugl_AddBytecode(uint8_t x)
{
  ugl_bytecode_array[ugl_bytecode_len] = x;
  ugl_bytecode_len++;
}

void ugl_InitBytecode(void)
{
  ugl_bytecode_len = 0;
  ugl_AddBytecode(BC_CMD_RETURN_FROM_PROCEDURE);
}

/*
extern uint8_t ugl_bytecode_array[];
void ugl_ExecBytecode(void)
{
  bc_t bc;
  bc_exec(&bc, ugl_bytecode_array, 0);
}
*/


void ugl_CheckForAllLabelsDefined(void)
{
  int i;
  for( i = 0; i < ugl_label_cnt; i++ )
  {
      if ( ugl_label_bytecode_pos[ugl_label_cnt] == 0x0ffff )
      {
	ugl_err("undefined label '%s'", ugl_label_name[i] );
      }
  }
}

void ugl_ResolveSymbols(void)
{
  uint8_t *code = ugl_bytecode_array;
  uint8_t *dest = ugl_bytecode_array+ugl_bytecode_len;
  uint16_t val;
  uint8_t cmd;
  
  ugl_CheckForAllLabelsDefined();
  
  ugl_glog("Resolve start=%p, end=%p", code, dest);
  ugl_glog("Resolve bytecode len=%d", ugl_bytecode_len);

  while( code < dest )
  {
    cmd = *code;
    ugl_glog("Resolve pos=%p, cmd=%02x", code, cmd);
    code++;
    val = cmd;
    val &=0x0f0;	/* put upper four bit as upper 4bit of the 12bit value into val */
    val <<= 4;
    switch(cmd&15)
    {
      case BC_CMD_LOAD_12BIT:
	code++;
	break;
      case BC_CMD_CALL_BUILDIN:
	code++;
	break;
      case BC_CMD_CALL_BUILDIN_POP_STACK:
	code++;
	break;
      case BC_CMD_BRANCH:
	val |= *code;
	code++;
	ugl_glog("Resolve BRANCH '%s' (idx=%d)", ugl_label_name[val], val);
	val = ugl_GetLabelBytecodePos(val);
	val = (val - (uint16_t)(code - ugl_bytecode_array));
	ugl_glog("Resolve BRANCH delta=0x%03x", val);
	*(code-2) &= 0x0f;
	*(code-2) |= (val >> 4) & 0x0f0;
	*(code-1) = val & 255;
	break;
      case BC_CMD_POP_ARG_STACK:
	break;
      case BC_CMD_PUSH_ARG_STACK:
	break;
      case BC_CMD_CALL_PROCEDURE:
	    val = code[0];
	    val <<= 8;
	    val |= code[1];
	  
	    ugl_glog("Resolve CALL Procedre '%s'  pos=%u", ugl_label_name[val], ugl_GetLabelBytecodePos(val));

	    val = ugl_GetLabelBytecodePos(val);

	    *code = val>>8;
	    code++;
	    *code = val&255;
	    code++;
	    break;
      
      default: /* assume 0x0f, extended command */
	switch( cmd )
	{	  
	  case BC_CMD_LOAD_0:
	    break;
	  case BC_CMD_LOAD_1:
	    break;
	  case BC_CMD_LOAD_16BIT:
	    code++;
	    code++;
	    break;
	  case BC_CMD_RETURN_FROM_PROCEDURE:
	    break;
	  case BC_CMD_JUMP_NOT_ZERO:
	  case BC_CMD_JUMP_ZERO:
	    val = code[0];
	    val <<= 8;
	    val |= code[1];
	  
	  
	    ugl_glog("Resolve JUMP '%s'", ugl_label_name[val]);

	    val = ugl_GetLabelBytecodePos(val);
	  
	    *code = val>>8;
	    code++;
	    *code = val&255;
	    code++;
	
	    break;
#ifdef NOT_USED
	  case BC_CMD_CALL_PROCEDURE:	    
	    val = code[0];
	    val <<= 8;
	    val |= code[1];
	  
	    ugl_glog("Resolve CALL Procedre '%s'  pos=%u", ugl_label_name[val], ugl_GetLabelBytecodePos(val));

	    val = ugl_GetLabelBytecodePos(val);

	    *code = val>>8;
	    code++;
	    *code = val&255;
	    code++;
	    
	    break;
#endif
/*
	  case BC_CMD_POP_ARG_STACK:
	    break;
*/
	  default:
	    ugl_err("Resolve: Unexpected command");
	    break;
	}
	break;
    } /* switch() */
  } /* while */
}

 



static int ugl_FindLabel(const char *name)
{
  int i;
  for( i = 0; i < ugl_label_cnt; i++ )
  {
    if (strcmp(name, ugl_label_name[i] ) == 0 )
      return i;
  }
  return -1;
}

static int ugl_AddLabel(const char *name)
{
  strcpy(ugl_label_name[ugl_label_cnt], name);
  ugl_label_bytecode_pos[ugl_label_cnt] = 0x0ffff;
  ugl_label_cnt++;
  if ( ugl_label_cnt >= UGL_MAX_LABEL_CNT )
    ugl_err("max number of labels reached, label=%s", name);
  return ugl_label_cnt-1;
}

int ugl_GetLabel(const char *name)
{
  int idx;
  idx = ugl_FindLabel(name);
  if ( idx >= 0 )
    return idx;
  return ugl_AddLabel(name);
}

void ugl_SetLabelBytecodePos(const char *name, uint16_t bytecode_pos)
{
  int idx;
  idx = ugl_GetLabel(name);
  if ( ugl_label_bytecode_pos[idx] != 0x0ffff )
    ugl_err("double definition of label '%s'", name);
  ugl_label_bytecode_pos[idx] = bytecode_pos;
}

uint16_t ugl_GetLabelBytecodePos(int idx)
{
  if ( ugl_label_bytecode_pos[idx] == 0x0ffff )
    ugl_err("undefined label '%s'", ugl_label_name[idx]);
  return ugl_label_bytecode_pos[idx];
}

void ugl_WriteBytecodeCArray(FILE *fp, const char *name)
{
  uint16_t i;
  fprintf(fp, "unsigned char %s[] =\n  \"", name);

  i = 0;
  while ( i < ugl_bytecode_len )
  {
    fprintf(fp, "\\x%02x", ugl_bytecode_array[i]);
    if ( i+1 == ugl_bytecode_len )
    {
      break;
    }
    if ( (i & 0x0f) == 0x0f )
    {
      fprintf(fp, "\"\n  \"");
    }
    i++;
  }
  fprintf(fp, "\";\n\n"); 
}