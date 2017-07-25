/*

  ugl_bc.c
  
  items can be at three places
    1) map
    2) inventory
    3) hero
    
  map --> inventory 	take
  inventory --> map	drop
  inventory --> hero	equip
  hero --> inventory	unequip
  hero --> map		drop


  inputs:
    hDir()			// direction into which the hero wants to walk, had waked or looks
    iDir()			// direction into which the item/creatue/missel wants to go, went or looks
    hX()			// hero X position
    hY()			// hero Y position
    posByH		// set current position to the position of the hero
    posByI			// set current position to the position of the current item
    posAppyDir(dir)	// change the possition going one step into the specified direction 
    
*/

#include "ugl_bc.h"
#include <stdio.h>
#include <assert.h>

#ifndef UGL_TEST
#include "item.h"
#include "map.h"
#endif



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
  return bc->return_stack[bc->return_stack_pointer];
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

#define BC_DBG_OUT_POS(pos) printf("%05d ", (int)(pos))
#define BC_DBG_OUT_HEX(c) printf("0x%02x ", (int)(c))
#define BC_DBG_OUT_STR(str) printf("%s ", (str))
#define BC_DBG_OUT_NUM(n) printf("%d ", (int)(n))
#define BC_DBG_OUT_NUM3(n) printf("%03d ", (int)(n))
#define BC_DBG_OUT_CR() printf("\n")

void bc_exec(bc_t *bc, uint8_t *code, uint16_t pos)
{
  uint16_t val;
  uint8_t cmd;
  
  bc_init(bc);
  bc->code = code;
  bc->code_pos = pos;
  
  for(;;)
  {
    cmd = bc->code[bc->code_pos];
    BC_DBG_OUT_POS(bc->code_pos);
    BC_DBG_OUT_NUM3(bc->arg_stack_pointer);
    BC_DBG_OUT_HEX(cmd);
    bc->code_pos++;
    val = cmd;
    val &=0x0f0;	/* put upper four bit as upper 4bit of the 12bit value into val */
    val <<= 4;
    switch(cmd&15)
    {
      case BC_CMD_LOAD_12BIT:
	BC_DBG_OUT_STR("LOAD12");
	val |= bc->code[bc->code_pos];
	BC_DBG_OUT_NUM(val);
	bc->code_pos++;
	bc_push_on_arg_stack(bc, val);
	BC_DBG_OUT_CR();
	break;
      case BC_CMD_CALL_BUILDIN:
	BC_DBG_OUT_STR("CALL BUILDIN");
	val |= bc->code[bc->code_pos];
	BC_DBG_OUT_NUM(val);
	bc->code_pos++;
	BC_DBG_OUT_CR();
	bc_buildin_list[val](bc);
	break;
      case BC_CMD_CALL_BUILDIN_POP_STACK:
	BC_DBG_OUT_STR("CALL BUILDIN POP ARG STACK");
	val |= bc->code[bc->code_pos];
	BC_DBG_OUT_NUM(val);
	bc->code_pos++;
	BC_DBG_OUT_CR();
	bc_buildin_list[val](bc);
	bc_pop_from_arg_stack(bc);
	break;
      case BC_CMD_BRANCH:
	BC_DBG_OUT_STR("BRANCH");
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
	BC_DBG_OUT_POS(bc->code_pos);
	BC_DBG_OUT_CR();
      case BC_CMD_POP_ARG_STACK:
	cmd >>= 4;	/* in this case we need the lower 4 bit */
	BC_DBG_OUT_STR("POP ARG STACK");	    
	cmd++;		/* cmd is reused as a counter for the number of stack pops */
	BC_DBG_OUT_NUM(cmd);
	do
	{
	  bc_pop_from_arg_stack(bc);
	  cmd--;
	} while( cmd > 0 );
	BC_DBG_OUT_CR();
	break;
      case BC_CMD_PUSH_ARG_STACK:
	cmd >>= 4;	/* in this case we need the lower 4 bit */
	BC_DBG_OUT_STR("PUSH ARG STACK");	    
	cmd++;		/* cmd is reused as a counter for the number of stack pops */
	BC_DBG_OUT_NUM(cmd);
	do
	{
	  bc_push_on_arg_stack(bc, 0);
	  cmd--;
	} while( cmd > 0 );
	BC_DBG_OUT_CR();
	break;
      case  BC_CMD_CALL_PROCEDURE:
	BC_DBG_OUT_STR("CALL PROC");
	cmd >>= 4;	/* in this case we need the lower 4 bit--> number of args */
	BC_DBG_OUT_NUM(cmd);	/* output number of args */
      
	val = bc->code[bc->code_pos];
	bc->code_pos++;
	val <<= 8;
	val |= bc->code[bc->code_pos];
	bc->code_pos++;
      
	bc_push_on_return_stack(bc, bc->code_pos);	/* return position */
	bc_push_on_return_stack(bc, bc->arg_stack_pointer - cmd -1);				/* store the start pos of the return value and the args */
	bc->code_pos = val;
	BC_DBG_OUT_NUM(bc->code_pos);
	BC_DBG_OUT_CR();
	
	break;
      default: /* assume 0x0f, extended command */
	switch( cmd )
	{
	  case BC_CMD_LOAD_0:
	    BC_DBG_OUT_STR("LOAD#0");
	    bc_push_on_arg_stack(bc, 0);
	    BC_DBG_OUT_CR();
	    break;
	  case BC_CMD_LOAD_1:
	    BC_DBG_OUT_STR("LOAD#1");
	    bc_push_on_arg_stack(bc, 1);
	    BC_DBG_OUT_CR();
	    break;
	  case BC_CMD_LOAD_16BIT:
	    BC_DBG_OUT_STR("LOAD16");
	    val = bc->code[bc->code_pos];
	    bc->code_pos++;
	    val <<= 8;
	    val |= bc->code[bc->code_pos];
	    bc->code_pos++;
	    bc_push_on_arg_stack(bc, val);
	    BC_DBG_OUT_NUM(val);
	    BC_DBG_OUT_CR();
	    break;
	  case BC_CMD_RETURN_FROM_PROCEDURE:
	    BC_DBG_OUT_STR("RETURN to");
	    if ( bc->return_stack_pointer == 0 )
	    {
	      BC_DBG_OUT_STR("exit");
	      BC_DBG_OUT_CR();
	      
	      BC_DBG_OUT_STR("arg stack: ");
	      BC_DBG_OUT_NUM(bc->arg_stack_pointer);
	      BC_DBG_OUT_CR();
	      BC_DBG_OUT_STR("return stack: ");
	      BC_DBG_OUT_NUM(bc->return_stack_pointer);
	      BC_DBG_OUT_CR();
	      return;	/* stop execution */
	    }
	    //bc_push_on_arg_stack(bc, bc_pop_from_return_stack(bc));	/* copy return value on arg stack */
	    bc->arg_stack_pointer = bc_pop_from_return_stack(bc) + 1; /* restore the arg stack pointer, leave return value on stack */
	    bc->code_pos = bc_pop_from_return_stack(bc);
	    BC_DBG_OUT_NUM(bc->code_pos);
	    BC_DBG_OUT_CR();
	    break;
	  case BC_CMD_JUMP_NOT_ZERO:
	    BC_DBG_OUT_STR("JUMP NZ");
	    val = bc->code[bc->code_pos];
	    bc->code_pos++;
	    val <<= 8;
	    val |= bc->code[bc->code_pos];
	    bc->code_pos++;
	  
	    if ( bc_pop_from_arg_stack(bc) != 0 )
	      bc->code_pos = val;
	    break;
	    BC_DBG_OUT_NUM(bc->code_pos);
	    BC_DBG_OUT_CR();
	  case BC_CMD_JUMP_ZERO:
	    BC_DBG_OUT_STR("JUMP Z");
	    val = bc->code[bc->code_pos];
	    bc->code_pos++;
	    val <<= 8;
	    val |= bc->code[bc->code_pos];
	    bc->code_pos++;
	  
	    if ( bc_pop_from_arg_stack(bc) == 0 )
	      bc->code_pos = val;
	    BC_DBG_OUT_NUM(bc->code_pos);
	    BC_DBG_OUT_CR();
	    break;
	  case  BC_CMD_CALL_PROCEDURE:
	    BC_DBG_OUT_STR("CALL PROC");
	    val = bc->code[bc->code_pos];
	    bc->code_pos++;
	    val <<= 8;
	    val |= bc->code[bc->code_pos];
	    bc->code_pos++;
	  
	    bc_push_on_return_stack(bc, bc->code_pos);	/* return position */
	    bc_push_on_return_stack(bc, 0);				/* return value */
	    bc->code_pos = val;
	    BC_DBG_OUT_NUM(bc->code_pos);
	    BC_DBG_OUT_CR();
	    
	    break;
/*	  
	  case BC_CMD_POP_ARG_STACK:
	    BC_DBG_OUT_STR("POP ARG STACK");
	    BC_DBG_OUT_CR();
	    bc_pop_from_arg_stack(bc);
	    break;
*/
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

/* return a pointer to a variable on the arg stack within the current stack frame */
/* pos = 0 is the return value of a user function, pos = 1... are the args for that function */
uint16_t *bc_get_stack_frame_address(bc_t *bc, uint8_t pos)
{
  return bc->arg_stack + bc->return_stack[bc->return_stack_pointer-1] + pos ;
}

void bc_fn_nop(bc_t *bc)
{
  bc_push_on_arg_stack(bc, 0);
}

/* description: "return" sets the return value of a procedure. If used inside an expresion, it returns its argument */
/* assign return value for a user defined function */
/* identical to arg(0) */
void bc_fn_return(bc_t *bc)
{
  bc_duplicate_arg_stack_top_value(bc);			/* goal is to leave a value on the stack */
  *bc_get_stack_frame_address(bc, 0) = bc_pop_from_arg_stack(bc);
  
  /*
  v = bc_pop_from_arg_stack(bc);
  bc_push_on_arg_stack(bc, v);  
  bc_pop_from_return_stack(bc);
  bc_push_on_return_stack(bc, v);
  */
}



void bc_fn_print(bc_t *bc)
{
  bc_duplicate_arg_stack_top_value(bc);			/* goal is to leave a value on the stack */
  printf("%u\n", bc_pop_from_arg_stack(bc));
}

/* return an argument of a user defined procedure */
void bc_fn_arg1(bc_t *bc)
{
  bc_push_on_arg_stack(bc, *bc_get_stack_frame_address(bc, bc_pop_from_arg_stack(bc)));
}

/* remove the top element from the stack and return the same */
void bc_fn_arg2(bc_t *bc)
{
  uint16_t v = bc_pop_from_arg_stack(bc);		/* the value, which should be assigned */
  *bc_get_stack_frame_address(bc, bc_pop_from_arg_stack(bc)) =  v;
  bc_push_on_arg_stack(bc, v); /* push the value back on the stack */
}



void bc_fn_add(bc_t *bc)
{
  uint16_t v;
  v = bc_pop_from_arg_stack(bc);
  v += bc_pop_from_arg_stack(bc);
  bc_push_on_arg_stack(bc, v);
}

#ifndef UGL_TEST
pos_t bc_pos;
#endif

void bc_fn_setPos(bc_t *bc)
{
  uint8_t x, y;
  y = bc_pop_from_arg_stack(bc);
  x = bc_pop_from_arg_stack(bc);
#ifndef UGL_TEST
  bc_pos.x = x;
  bc_pos.y = y;
#endif
  bc_push_on_arg_stack(bc, 0);
}

void bc_fn_setItemPos(bc_t *bc)
{
  uint8_t i;
  i = bc_pop_from_arg_stack(bc);
#ifndef UGL_TEST
  pool_GetItem(i)->pos = bc_pos;
  printf("item %d new x=%d y=%d\n", i, bc_pos.x, bc_pos.y);
#endif
  bc_push_on_arg_stack(bc, i);  
}

void bc_fn_setItemDir(bc_t *bc)
{
  uint8_t i;
  uint8_t dir;
  dir = bc_pop_from_arg_stack(bc);
  i = bc_pop_from_arg_stack(bc);
#ifndef UGL_TEST
  pool_GetItem(i)->dir = dir;
  printf("item %d dir=%d\n", i, dir);
#endif
  bc_push_on_arg_stack(bc, i);  
}


/*======================================================*/
bc_buildin_fn bc_buildin_list[] = 
{
  /* 0 */ bc_fn_nop,
  /* 1 */ bc_fn_return,
  /* 2 */ bc_fn_arg1,		/* one argument */
  /* 3 */ bc_fn_arg2,		/* two arguments */
  /* 4 */ bc_fn_add,
  /* 5 */ bc_fn_print,
  /* 6 */ bc_fn_setPos,	/* two args: x & y: assign x/y to the pos temp var */
  /* 7 */ bc_fn_setItemPos,	/* one args: item, assign pos temp var to item */
  /* 8 */ bc_fn_setItemDir,		/* two args, item and dir */
};


