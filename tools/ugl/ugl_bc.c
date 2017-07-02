/*

  ugl_bc.c

*/

#include "ugl_bc.h"
#include <stdio.h>

void bc_push_on_stack(bc_t *bc, uint16_t val)
{
  bc->stack[bc->stack_pointer]  = val;
  bc->stack_pointer++;
}

uint16_t bc_pop_from_stack(bc_t *bc)
{
  bc->stack_pointer--;
  return bc->stack[bc->stack_pointer] ;
}

void bc_duplicate_stack_top_value(bc_t *bc)
{
  bc->stack[bc->stack_pointer] = bc->stack[bc->stack_pointer-1]; 
  bc->stack_pointer++;
}

void bc_exec(bc_t *bc, uint8_t *code)
{
  uint16_t val;
  uint8_t cmd;
  bc->stack_pointer = 0;
  bc->code = code;
  
  for(;;)
  {
    cmd = *code;
    code++;
    val = cmd;
    val &=0x0f0;	/* put upper four bit as upper 4bit of the 12bit value into val */
    val <<= 4;
    switch(cmd&15)
    {
      case BC_CMD_LOAD_12BIT:
	val |= *code;
	code++;
	bc_push_on_stack(bc, val);
	break;
      case BC_CMD_CALL_BUILDIN:
	val |= *code;
	code++;
	bc_buildin_list[val](bc);
	break;
      case BC_CMD_CALL_BUILDIN_POP_STACK:
	val |= *code;
	code++;
	bc_buildin_list[val](bc);
	bc_pop_from_stack(bc);
	break;
      case BC_CMD_BRANCH:
	val |= *code;
	code++;
	if ( val < 0x0800 )
	{
	  code += val;
	}
	else
	{
	  val = 0x1000 - val;
	  code -= val;
	}
	
      default: /* assume 0x0f, extended command */
	switch( cmd )
	{
	  case BC_CMD_LOAD_0:
	    bc_push_on_stack(bc, 0);
	    break;
	  case BC_CMD_LOAD_1:
	    bc_push_on_stack(bc, 1);
	    break;
	  case BC_CMD_LOAD_16BIT:
	    val = *code;
	    code++;
	    val <<= 8;
	    val |= *code;
	    code++;
	    bc_push_on_stack(bc, val);
	    break;
	  case BC_CMD_RETURN_FROM_PROCEDURE:
	    if ( bc->stack_pointer == 0 )
	      return;	/* stop execution */
	    code = bc->code+bc_pop_from_stack(bc);
	    break;
	  case BC_CMD_JUMP_NOT_ZERO:
	    val = *code;
	    code++;
	    val <<= 8;
	    val |= *code;
	    code++;
	  
	    if ( bc_pop_from_stack(bc) != 0 )
	      code = bc->code+val;
	    break;
	

	  case BC_CMD_JUMP_ZERO:
	    val = *code;
	    code++;
	    val <<= 8;
	    val |= *code;
	    code++;
	  
	    if ( bc_pop_from_stack(bc) == 0 )
	      code = bc->code+val;
	    break;
	    
	  default:
	    break;
	}
	break;
    } /* switch() */
  } /* for(;;) */
}

/*======================================================*/

/* put top of stack into register a, reduce stack */
//void bc_pop_a(bc_t *bc)
//{
//  bc->stack_pointer--;
//  bc->a = bc->stack[bc->stack_pointer];
//}

/*======================================================*/


void bc_fn_nop(bc_t *bc)
{
  bc_push_on_stack(bc, 0);
}

void bc_fn_print(bc_t *bc)
{
  bc_duplicate_stack_top_value(bc);			/* goal is to leave a value on the stack */
  printf("%u\n", bc_pop_from_stack(bc));
}

/*======================================================*/
bc_buildin_fn bc_buildin_list[] = 
{
  /* 0 */ bc_fn_nop,
  /* 1 */ bc_fn_print
};


