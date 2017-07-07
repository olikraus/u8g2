/*

  ugl_bc.c

*/

#include "ugl_bc.h"
#include <stdio.h>
#include <assert.h>

void bc_push_on_arg_stack(bc_t *bc, uint16_t val)
{
  bc->arg_stack[bc->arg_stack_pointer]  = val;
  bc->arg_stack_pointer++;
}

uint16_t bc_pop_from_arg_stack(bc_t *bc)
{
  assert( bc->arg_stack_pointer > 0 );
  bc->arg_stack_pointer--;
  return bc->arg_stack[bc->arg_stack_pointer] ;
}

void bc_duplicate_arg_stack_top_value(bc_t *bc)
{
  bc->arg_stack[bc->arg_stack_pointer] = bc->arg_stack[bc->arg_stack_pointer-1]; 
  bc->arg_stack_pointer++;
}

void bc_push_on_return_stack(bc_t *bc, uint16_t val)
{
  bc->return_stack[bc->return_stack_pointer]  = val;
  bc->return_stack_pointer++;
}

uint16_t bc_pop_from_return_stack(bc_t *bc)
{
  assert( bc->return_stack_pointer > 0 );
  bc->return_stack_pointer--;
  return bc->return_stack[bc->return_stack_pointer] ;
}

uint16_t bc_get_value(uint8_t *code)
{
  uint16_t val;
  val = *code;
  code++;
  val <<= 8;
  val |= *code;
  code++;
  return val;
}

void bc_init(bc_t *bc)
{
  bc->arg_stack_pointer = 0;
  bc->return_stack_pointer = 0;
}

void bc_exec(bc_t *bc, uint8_t *code)
{
  uint16_t val;
  uint8_t cmd;
  
  bc_init(bc);
  bc->code = code;
  bc->code_pos = 0;
  
  for(;;)
  {
    cmd = bc->code[bc->code_pos];
    bc->code_pos++;
    val = cmd;
    val &=0x0f0;	/* put upper four bit as upper 4bit of the 12bit value into val */
    val <<= 4;
    switch(cmd&15)
    {
      case BC_CMD_LOAD_12BIT:
	val |= bc->code[bc->code_pos];
	bc->code_pos++;
	bc_push_on_arg_stack(bc, val);
	break;
      case BC_CMD_CALL_BUILDIN:
	val |= bc->code[bc->code_pos];
	bc->code_pos++;
	bc_buildin_list[val](bc);
	break;
      case BC_CMD_CALL_BUILDIN_POP_STACK:
	val |= bc->code[bc->code_pos];
	bc->code_pos++;
	bc_buildin_list[val](bc);
	bc_pop_from_arg_stack(bc);
	break;
      case BC_CMD_BRANCH:
	val |= bc->code[bc->code_pos];
	bc->code_pos++;
	if ( val < 0x0800 )
	{
	  bc->code_pos += val;
	}
	else
	{
	  val = 0x1000 - val;
	  bc->code_pos -= val;
	}
	
      default: /* assume 0x0f, extended command */
	switch( cmd )
	{
	  case BC_CMD_LOAD_0:
	    bc_push_on_arg_stack(bc, 0);
	    break;
	  case BC_CMD_LOAD_1:
	    bc_push_on_arg_stack(bc, 1);
	    break;
	  case BC_CMD_LOAD_16BIT:
	    val = bc->code[bc->code_pos];
	    bc->code_pos++;
	    val <<= 8;
	    val |= bc->code[bc->code_pos];
	    bc->code_pos++;
	    bc_push_on_arg_stack(bc, val);
	    break;
	  case BC_CMD_RETURN_FROM_PROCEDURE:
	    if ( bc->return_stack_pointer == 0 )
	      return;	/* stop execution */
	    bc_push_on_arg_stack(bc, bc_pop_from_return_stack(bc));	/* copy return value on arg stack */
	    bc->code_pos = bc_pop_from_return_stack(bc);
	    break;
	  case BC_CMD_JUMP_NOT_ZERO:
	    val = bc->code[bc->code_pos];
	    bc->code_pos++;
	    val <<= 8;
	    val |= bc->code[bc->code_pos];
	    bc->code_pos++;
	  
	    if ( bc_pop_from_arg_stack(bc) != 0 )
	      bc->code_pos = val;
	    break;
	

	  case BC_CMD_JUMP_ZERO:
	    val = bc->code[bc->code_pos];
	    bc->code_pos++;
	    val <<= 8;
	    val |= bc->code[bc->code_pos];
	    bc->code_pos++;
	  
	    if ( bc_pop_from_arg_stack(bc) == 0 )
	      bc->code_pos = val;
	    break;
	  case  BC_CMD_CALL_PROCEDURE:
	    val = bc->code[bc->code_pos];
	    bc->code_pos++;
	    val <<= 8;
	    val |= bc->code[bc->code_pos];
	    bc->code_pos++;
	  
	    bc_push_on_return_stack(bc, bc->code_pos);	/* return position */
	    bc_push_on_return_stack(bc, 0);				/* return value */
	    bc->code_pos = val;
	    
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
  bc_push_on_arg_stack(bc, 0);
}

void bc_fn_print(bc_t *bc)
{
  bc_duplicate_arg_stack_top_value(bc);			/* goal is to leave a value on the stack */
  printf("%u\n", bc_pop_from_arg_stack(bc));
}

void bc_fn_add(bc_t *bc)
{
  uint16_t v;
  v = bc_pop_from_arg_stack(bc);
  v += bc_pop_from_arg_stack(bc);
  bc_push_on_arg_stack(bc, v);
}

/*======================================================*/
bc_buildin_fn bc_buildin_list[] = 
{
  /* 0 */ bc_fn_nop,
  /* 1 */ bc_fn_print,
  /* 2 */ bc_fn_add
};


