/*

  mui.c
  
  Monochrome minimal user interface: Core library.

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2021, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

  

  "mui.c" is a graphical user interface, developed as part of u8g2.
  However "mui.c" is independent of u8g2 and can be used without u8g2 code.
  The glue code between "mui.c" and u8g2 is located in "mui_u8g2.c"

  c: cmd
  i:  ID0
  j: ID1
  xy: Position (x and y)
  /text/: some text. The text can start with any delimiter (except 0 and |), but also has to end with the same delimiter
  a: Single char argument
  u: Single char argument with the user interface form number

  "Uu" the interface                                                    --> no ID
  
  Manual ID:
  "Fijxy"  Generic field: Places field with id ii at x/y        --> ID=ij
  "Bijxy/text/"   Generic field (Button) with Text   --> ID=ij
  "Tiixya/text/"  Generic field with argument and text --> ID = ij
  "Aiixya"
  
  Fixed ID:
  "Si" the style                                                        --> ID=@i
  "Lxy/labeltext/"  Places a text at the specified position, field with   -     -> ID=.L, .l
  "Gxyu/menutext/"  Go to the specified menu without placing the user interface form on the stack       --> ID=.G, .g
  
  
  cijxy
  cijxy/text/
  cijxya/text/
  
  cxy/text/
  cxya/text/
  
*/



#include "mui.h"





//#define mui_get_fds_char(s) ((uint8_t)(*s))

//#include <stdio.h>
//#define MUI_DEBUG(...) printf(__VA_ARGS__)
#define MUI_DEBUG(...)

uint8_t mui_get_fds_char(fds_t *s)
{
  //return (uint8_t)(*s);
  return (uint8_t)mui_pgm_read(s);
}


/*
  s must point to a valid command within FDS
*/
static size_t mui_fds_get_cmd_size_without_text(fds_t *s) MUI_NOINLINE;
static size_t mui_fds_get_cmd_size_without_text(fds_t *s)
{
  uint8_t c = mui_get_fds_char(s);
  c &= 0xdf; /* consider upper and lower case */
  switch(c)
  {
    case 'U': return 2;         // User Form: CMD  (1 Byte), Form-Id (1 Byte)
    case 'S': return 2;         // Style: CMD (1 Byte), Style Id (1 Byte)
    case 'D': return 3;         // Data within Text: CMD (1 Byte), ID (2 Bytes), Text (does not count here)
    case 'Z': return 3;         // Zero field without x, y, arg & text: CMD (1 Byte), ID (2 Bytes)
    case 'F': return 5;         // Field without arg & text: CMD (1 Byte), ID (2 Bytes), X, Y
    case 'B': return 5;         // Field with text: CMD (1 Byte), ID (2 Bytes), X, Y, Text (does not count here)
    case 'T': return 6;         // Field with arg & text: CMD (1 Byte), ID (2 Bytes), X, Y, Arg, Text (does not count here)
    case 'A': return 6;         // Field with arg (no text): CMD (1 Byte), ID (2 Bytes), X, Y, Arg, Text
    case 'L': return 3;          // Text Label: CMD (1 Byte), X, Y (same as 'B' but with fixed ID '.L', MUIF_LABEL, MUI_LABEL)
    case 'G': return 4;         // Goto Btutton: CMD (1Byte), X, Y, Arg, Text  (same as 'T' but with fixed ID '.G', MUIF_GOTO, MUI_GOTO)
    case 0: return 0;
  }
  return 1;
}



/*
  s must point to the string delimiter start: first '/' for "B00ab/ok/"
    - '/' actually is 0xff
    - return the total size of the string, including the delimiter
    - copies the content of the string ("ok") to the ui text buffer

*/
static size_t mui_fds_parse_text(mui_t *ui, fds_t *s)
{
  uint8_t i = 0;
  ui->delimiter = mui_get_fds_char(s);
  uint8_t c;
  fds_t *t = s;
  
  //printf("mui_fds_parse_text del=%d\n", delimiter);
#ifdef MUI_CHECK_EOFDS
  if ( ui->delimiter == 0 )
    return 0;
#endif 
  t++;
  for( ;; )
  {
    c = mui_get_fds_char(t);
  //printf("mui_fds_parse_text i=%d, c=%c\n", i, c);
#ifdef MUI_CHECK_EOFDS
    if ( c == 0 )
      break;
#endif 
    if ( c == ui->delimiter )
    {
      t++;
      break;
    }
    if ( i < MUI_MAX_TEXT_LEN )
    {
      ui->text[i++] = c;
    }
    t++;
  }
  ui->text[i] = '\0' ;
  return t-s;
}

/*
  get the first token within a text argument.
  The text argument may look like this:
    "B00ab/banana|apple|peach|cherry/"
  The outer delimiter "/" is not fixed and can be any char except "|" and "\0"
  The inner delimiter "|" is fixed. It must be the pipe symbol.
  This function will place "banana" into ui->text if the result is not 0

  if ( mui_fds_first_token(ui) )
  {
    do 
    {
      // handle token in ui->text
    } while ( mui_fds_next_token(ui) )
  }

*/

uint8_t mui_fds_first_token(mui_t *ui)
{
  ui->token = ui->fds;
  ui->token += mui_fds_get_cmd_size_without_text(ui->fds);
  ui->delimiter = mui_get_fds_char(ui->token);
  ui->token++;  // place ui->token on the first char of the token
  return mui_fds_next_token(ui);
}

/*
  The inner token delimiter "|" is fixed. It must be the pipe symbol.
*/
uint8_t mui_fds_next_token(mui_t *ui)
{
  uint8_t c;
  uint8_t i = 0;
  // printf("mui_fds_next_token: call, ui->token=%p\n", ui->token);
  for( ;; )
  {
    c = mui_get_fds_char(ui->token);
    // printf("mui_fds_next_token: i=%d c=%c\n", i, c);
#ifdef MUI_CHECK_EOFDS
    if ( c == 0 )
      break;
#endif 
    if ( c == ui->delimiter )
      break;
    if ( c == '|'  )
    {
      ui->token++;  // place ui->token on the first char of the next token
      break;
    }
    
    if ( i < MUI_MAX_TEXT_LEN )
    {
      ui->text[i++] = c;
    }
    
    ui->token++;
  }
  ui->text[i] = '\0' ;
  if ( i == 0 )
    return 0;   // no further token found
  return 1;  // token placed in ui->text
}

/*
  find nth token ('|' delimiter), return 0 if n exceeds the number of tokens, 1 otherwise
  the result is stored in ui->text
*/
uint8_t mui_fds_get_nth_token(mui_t *ui, uint8_t n)
{  
  // printf("mui_fds_get_nth_token: call, n=%d\n", n);
  if ( mui_fds_first_token(ui) )
  {
    do 
    {
      if ( n == 0 )
      {
        // printf("mui_fds_get_nth_token: found");
        return 1;
      }
      n--;
    } while ( mui_fds_next_token(ui) );
  }
  //printf("mui_fds_get_nth_token: NOT found\n");
  return 0;
}

uint8_t mui_fds_get_token_cnt(mui_t *ui)
{
  uint8_t n = 0;
  if ( mui_fds_first_token(ui) )
  {
    do 
    {
      n++;
    } while ( mui_fds_next_token(ui) );
  }
  return n;
}


#define mui_fds_is_text(c) ( (c) == 'U' || (c) == 'S' || (c) == 'F' || (c) == 'A' || (c) == 'Z' ? 0 : 1 )

/*
  s must point to a valid command within FDS
  return
    The complete length of the command (including any text part)
  sideeffect:
    Any existing text part will be copied into ui->text
    ui->text will be assigned to empty string if there is no text argument
*/
static size_t mui_fds_get_cmd_size(mui_t *ui, fds_t *s) MUI_NOINLINE;
static size_t mui_fds_get_cmd_size(mui_t *ui, fds_t *s)
{
  size_t l = mui_fds_get_cmd_size_without_text(s);
  uint8_t c = mui_get_fds_char(s);
 ui->text[0] = '\0' ;   /* always reset the text buffer */
 if ( mui_fds_is_text(c) )
  {
    l += mui_fds_parse_text(ui, s+l);
  }
  return l;
}


/*
  mui_Init() will setup the menu system but will not activate or display anything.
  Use mui_GotoForm() after this command, then use mui_Draw() to draw the menu on a display.
*/
void mui_Init(mui_t *ui, void *graphics_data, fds_t *fds, muif_t *muif_tlist, size_t muif_tcnt)
{
  memset(ui, 0, sizeof(mui_t));
  ui->root_fds = fds;
  //ui->current_form_fds = NULL;   // not required, because there was a memset before
  ui->muif_tlist = muif_tlist;
  ui->muif_tcnt = muif_tcnt;
  ui->graphics_data = graphics_data;
  ui->last_form_stack_pos = -1;
}

static int mui_find_uif(mui_t *ui, uint8_t id0, uint8_t id1)
{
  size_t i;
  for( i = 0; i < ui->muif_tcnt; i++ )
  {
    /*
      if ( ui->muif_tlist[i].id0 == id0 )
        if ( ui->muif_tlist[i].id1 == id1 )
          return i;
    */
      if ( muif_get_id0(ui->muif_tlist+i) == id0 )
        if ( muif_get_id1(ui->muif_tlist+i) == id1 )
          return i;
  }
  return -1;
}


/*
  assumes a valid position in ui->fds and calculates all the other variables
  some fields are always calculated like the ui->cmd and ui->len field
  other member vars are calculated only if the return value is 1
  will return 1 if the field id was found.
  will return 0 if the field id was not found in uif or if ui->fds points to something else than a field
*/
static uint8_t mui_prepare_current_field(mui_t *ui) MUI_NOINLINE;
static uint8_t mui_prepare_current_field(mui_t *ui)
{
  int muif_tidx;

  ui->uif = NULL;
  ui->dflags = 0;    
  ui->id0 = 0;
  ui->id1 = 0;
  ui->arg = 0;

  /* calculate the length of the command and copy the text argument */
  /* this will also clear the text in cases where there is no text argument */
  ui->len = mui_fds_get_cmd_size(ui, ui->fds); 
  //printf("mui_prepare_current_field len=%d\n", ui->len);

  /* get the command and check whether end of form is reached */
  ui->cmd = mui_get_fds_char(ui->fds);
  //printf("mui_prepare_current_field cmd='%c' len=%d\n", ui->cmd, ui->len);
  
  /* Copy the cmd also to second id value. This is required for some commands, others will overwrite this below */
  ui->id1 = ui->cmd;
  
  /* now make the command uppercase so that both, upper and lower case are considered */
  ui->cmd &= 0xdf; /* consider upper and lower case */
  
  if ( ui->cmd == 'U' || ui->cmd == 0 )
    return 0;

  /* calculate the dynamic flags */
  if ( ui->fds == ui->cursor_focus_fds )
    ui->dflags |= MUIF_DFLAG_IS_CURSOR_FOCUS;
  if ( ui->fds == ui->touch_focus_fds )
    ui->dflags |= MUIF_DFLAG_IS_TOUCH_FOCUS;
  

  /* get the id0 and id1 values */
  if  ( ui->cmd == 'F' || ui->cmd == 'B' || ui->cmd == 'T' || ui->cmd == 'A' )
  {
      ui->id0 = mui_get_fds_char(ui->fds+1);
      ui->id1 = mui_get_fds_char(ui->fds+2);
      ui->x = mui_get_fds_char(ui->fds+3);
      ui->y = mui_get_fds_char(ui->fds+4);
      if ( ui->cmd == 'A' || ui->cmd == 'T' )
      {
        ui->arg = mui_get_fds_char(ui->fds+5);
      }
  }
  else if ( ui->cmd == 'D' || ui->cmd == 'Z' )
  {
      ui->id0 = mui_get_fds_char(ui->fds+1);
      ui->id1 = mui_get_fds_char(ui->fds+2);
  }
  else if ( ui->cmd == 'S' )
  {
      ui->id0 = 'S';
      ui->id1 = mui_get_fds_char(ui->fds+1);
  }
  else
  {
      ui->id0 = '.';
      /* note that ui->id1 contains the original cmd value */
      ui->x = mui_get_fds_char(ui->fds+1);
      ui->y = mui_get_fds_char(ui->fds+2);
      if ( ui->cmd == 'G' || ui->cmd == 'M' )  /* this is also true for 'g' or 'm' */
      {
        ui->arg = mui_get_fds_char(ui->fds+3);
      }
  }

  //MUI_DEBUG("mui_prepare_current_field cmd='%c' len=%d arg=%d\n", ui->cmd, ui->len, ui->arg);

  
  /* find the field  */
  muif_tidx = mui_find_uif(ui, ui->id0, ui->id1);
  //printf("mui_prepare_current_field: muif_tidx=%d\n", muif_tidx);
  if ( muif_tidx >= 0 )
  {
    ui->uif = ui->muif_tlist + muif_tidx;
    return 1;
  }
  return 0;
}

/* 
  assumes that ui->fds has been assigned correctly 
  and that ui->target_fds and ui->tmp_fds had been cleared if required

  Usually do not call this function directly, instead use mui_loop_over_form

*/

static void mui_inner_loop_over_form(mui_t *ui, uint8_t (*task)(mui_t *ui)) MUI_NOINLINE;
static void mui_inner_loop_over_form(mui_t *ui, uint8_t (*task)(mui_t *ui))
{
  uint8_t cmd;

  //MUI_DEBUG("mui_inner_loop_over_form start %p\n", task);
  
  ui->fds += mui_fds_get_cmd_size(ui, ui->fds);      // skip the first entry, it is U always
  for(;;)
  {    
    //printf("fds=%p *fds='%c'\n", ui->fds, ui->fds[0]);
    /* get the command and check whether end of form is reached */
    cmd = mui_get_fds_char(ui->fds);
    if ( cmd == 'U' || cmd == 0 )
      break;
    if ( mui_prepare_current_field(ui) )  /* side effect: calculate ui->len */
      if ( task(ui) )         /* call the task, which was provided as argument to this function */
      {
        //MUI_DEBUG("mui_inner_loop_over_form break by task\n");
        break;
      }
    ui->fds += ui->len;
  }
  
  //MUI_DEBUG("mui_inner_loop_over_form end %p\n", task);
}

static void mui_loop_over_form(mui_t *ui, uint8_t (*task)(mui_t *ui)) MUI_NOINLINE;
static void mui_loop_over_form(mui_t *ui, uint8_t (*task)(mui_t *ui))
{
  if ( mui_IsFormActive(ui) == 0 )
    return;
  
  ui->fds = ui->current_form_fds;
  ui->target_fds = NULL;
  ui->tmp_fds = NULL;
  
  mui_inner_loop_over_form(ui, task);  
}

/*
  n is the form number
*/
static fds_t *mui_find_form(mui_t *ui, uint8_t n)
{
  fds_t *fds = ui->root_fds;
  uint8_t cmd;
  
  for( ;; )
  {
    cmd = mui_get_fds_char(fds);
    if ( cmd == 0 )
      break;
    if ( cmd == 'U'  )
    {
      if (   mui_get_fds_char(fds+1) == n )
      {
        return fds;
      }
      /* not found, just coninue */
    }
    
    fds += mui_fds_get_cmd_size(ui, fds);
  }
  return NULL;
}

/* === task procedures (arguments for mui_loop_over_form) === */
/* ui->fds contains the current field */

static uint8_t mui_task_draw(mui_t *ui)
{
  //printf("mui_task_draw fds=%p uif=%p text=%s\n", ui->fds, ui->uif, ui->text);
  muif_get_cb(ui->uif)(ui, MUIF_MSG_DRAW);
  return 0;     /* continue with the loop */
}

static uint8_t mui_task_form_start(mui_t *ui)
{
  muif_get_cb(ui->uif)(ui, MUIF_MSG_FORM_START);
  return 0;     /* continue with the loop */
}

static uint8_t mui_task_form_end(mui_t *ui)
{
  muif_get_cb(ui->uif)(ui, MUIF_MSG_FORM_END);
  return 0;     /* continue with the loop */
}

static uint8_t mui_uif_is_cursor_selectable(mui_t *ui) MUI_NOINLINE;
static uint8_t mui_uif_is_cursor_selectable(mui_t *ui)
{
  if ( muif_get_cflags(ui->uif) & MUIF_CFLAG_IS_CURSOR_SELECTABLE )
  {
    return 1;
  }
  return 0;
}

static uint8_t mui_task_find_prev_cursor_uif(mui_t *ui)
{
  //if ( muif_get_cflags(ui->uif) & MUIF_CFLAG_IS_CURSOR_SELECTABLE )
  if ( mui_uif_is_cursor_selectable(ui) )
  {
    if ( ui->fds == ui->cursor_focus_fds )
    {
      ui->target_fds = ui->tmp_fds;
      return 1;         /* stop looping */
    }
    ui->tmp_fds = ui->fds;
  }
  return 0;     /* continue with the loop */
}

static uint8_t mui_task_find_first_cursor_uif(mui_t *ui)
{
  //if ( muif_get_cflags(ui->uif) & MUIF_CFLAG_IS_CURSOR_SELECTABLE )
  if ( mui_uif_is_cursor_selectable(ui) )
  {
    // if ( ui->target_fds == NULL )
    // {
      ui->target_fds = ui->fds;
      return 1;         /* stop looping */
    // }
  }
  return 0;     /* continue with the loop */
}

static uint8_t mui_task_find_last_cursor_uif(mui_t *ui)
{
  //if ( muif_get_cflags(ui->uif) & MUIF_CFLAG_IS_CURSOR_SELECTABLE )
  if ( mui_uif_is_cursor_selectable(ui) )
  {
    //ui->cursor_focus_position++;
    ui->target_fds = ui->fds;
  }
  return 0;     /* continue with the loop */
}

static uint8_t mui_task_find_next_cursor_uif(mui_t *ui)
{
  //if ( muif_get_cflags(ui->uif) & MUIF_CFLAG_IS_CURSOR_SELECTABLE )
  if ( mui_uif_is_cursor_selectable(ui) )
  {
    if ( ui->tmp_fds != NULL )
    {
      ui->target_fds = ui->fds;        
      ui->tmp_fds = NULL;
      return 1;         /* stop looping */
    }
    if ( ui->fds == ui->cursor_focus_fds )
    {
      ui->tmp_fds = ui->fds;
    }
  }
  return 0;     /* continue with the loop */
}

static uint8_t mui_task_get_current_cursor_focus_position(mui_t *ui)
{
  //if ( muif_get_cflags(ui->uif) & MUIF_CFLAG_IS_CURSOR_SELECTABLE )
  if ( mui_uif_is_cursor_selectable(ui) )
  {
    if ( ui->fds == ui->cursor_focus_fds )
      return 1;         /* stop looping */
    ui->tmp8++;
  }
  return 0;     /* continue with the loop */
}

#ifdef OBSOLETE
static uint8_t mui_task_read_nth_selectable_field(mui_t *ui)
{
  //if ( muif_get_cflags(ui->uif) & MUIF_CFLAG_IS_CURSOR_SELECTABLE )
  if ( mui_uif_is_cursor_selectable(ui) )
  {
    if ( ui->tmp8 == 0 )
      return 1;         /* stop looping */
    ui->tmp8--;
  }
  return 0;     /* continue with the loop */
}
#endif

static uint8_t mui_task_find_execute_on_select_field(mui_t *ui)
{
  if ( muif_get_cflags(ui->uif) & MUIF_CFLAG_IS_EXECUTE_ON_SELECT )
  {
      ui->target_fds = ui->fds;
      return 1;         /* stop looping */
  }
  return 0;     /* continue with the loop */
}


/* === utility functions for the user API === */

static uint8_t mui_send_cursor_msg(mui_t *ui, uint8_t msg) MUI_NOINLINE;
static uint8_t mui_send_cursor_msg(mui_t *ui, uint8_t msg)
{
  if ( ui->cursor_focus_fds )
  {
    ui->fds = ui->cursor_focus_fds;
    if ( mui_prepare_current_field(ui) )
      return muif_get_cb(ui->uif)(ui, msg);
  }
  return 0; /* not called, msg not handled */
}

/* === user API === */

/* 
  returns the field pos which has the current focus 
  If the first selectable field has the focus, then 0 will be returned
  Unselectable fields (for example labels) are skipped by this count.
  If no fields are selectable, then 0 is returned

  The return value can be used as last argument for mui_EnterForm or mui_GotoForm

  WARNING: This function will destroy current fds and field information.
*/
uint8_t mui_GetCurrentCursorFocusPosition(mui_t *ui)
{
  //fds_t *fds = ui->fds;
  ui->tmp8 = 0;  
  mui_loop_over_form(ui, mui_task_get_current_cursor_focus_position);
  //ui->fds = fds;
  return ui->tmp8;
}


void mui_Draw(mui_t *ui)
{
  mui_loop_over_form(ui, mui_task_draw);
}

static void mui_next_field(mui_t *ui)
{
  mui_loop_over_form(ui, mui_task_find_next_cursor_uif);
  // ui->cursor_focus_position++;
  ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
  if ( ui->target_fds == NULL )
  {
    mui_loop_over_form(ui, mui_task_find_first_cursor_uif);
    ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
    // ui->cursor_focus_position = 0;
  }
}


/*
  this function will overwrite the ui field related member variables
  nth_token can be 0 if the fiel text is not a option list
  the result is stored in ui->text
  
  token delimiter is '|' (pipe symbol)
  
  fds:  The start of a field (MUI_DATA)
  nth_token: The position of the token, which should be returned
*/
uint8_t mui_GetSelectableFieldTextOption(mui_t *ui, fds_t *fds, uint8_t nth_token)
{
  if ( fds != NULL )
  {
    fds_t *fds_backup = ui->fds;                                // backup the current fds, so that this function can be called inside a task loop 
    int len = ui->len;          // backup length of the current command, 26 sep 2021: probably this is not required any more
    uint8_t is_found;
    
    ui->fds = fds;
    // at this point ui->fds contains the field which contains the tokens  
    // now get the opion string out of the text field. nth_token can be 0 if this is no opion string
    is_found = mui_fds_get_nth_token(ui, nth_token);          // return value is ignored here
    
    ui->fds = fds_backup;                        // restore the previous fds position
    ui->len = len;
    // result is stored in ui->text
    return is_found;
  }
  ui->text[0] = '\0';
  return 0;
}

uint8_t mui_GetSelectableFieldOptionCnt(mui_t *ui, fds_t *fds)
{
  if ( fds != NULL )
  {
    fds_t *fds_backup = ui->fds;                                // backup the current fds, so that this function can be called inside a task loop 
    int len = ui->len;          // backup length of the current command   26 sep 2021: probably this is not required any more
    uint8_t cnt = 0;
    
    ui->fds = fds;
    // at this point ui->fds contains the field which contains the tokens  
    // now get the opion string out of the text field. nth_token can be 0 if this is no opion string
    cnt = mui_fds_get_token_cnt(ui); 
    
    ui->fds = fds_backup;                        // restore the previous fds position
    ui->len = len;
    // result is stored in ui->text
    return cnt;
  }
  ui->text[0] = '\0';
  return 0;
}



//static void mui_send_cursor_enter_msg(mui_t *ui) MUI_NOINLINE;
static uint8_t mui_send_cursor_enter_msg(mui_t *ui)
{
  ui->is_mud = 0;
  return mui_send_cursor_msg(ui, MUIF_MSG_CURSOR_ENTER);
}

/* 
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
  mui_EnterForm is similar to mui_GotoForm and differes only in the second argument (which is the form id instead of the fds pointer)
*/
void mui_EnterForm(mui_t *ui, fds_t *fds, uint8_t initial_cursor_position)
{
  /* exit any previous form, will not do anything if there is no current form */
  mui_LeaveForm(ui);
  
  /* clean focus fields */
  ui->touch_focus_fds = NULL;
  ui->cursor_focus_fds = NULL;
  
  /* reset all the scoll values */
  ui->form_scroll_top = 0;
  ui->form_scroll_visible = 0;
  ui->form_scroll_total = 0;
  
  /* assign the form, which should be entered */
  ui->current_form_fds = fds;
  
  /* inform all fields that we start a new form */
  MUI_DEBUG("mui_EnterForm: form_start, initial_cursor_position=%d\n", initial_cursor_position);
  mui_loop_over_form(ui, mui_task_form_start);
  
  /* assign initional cursor focus */
  MUI_DEBUG("mui_EnterForm: find_first_cursor_uif\n");
  mui_loop_over_form(ui, mui_task_find_first_cursor_uif);  
  ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
  MUI_DEBUG("mui_EnterForm: find_first_cursor_uif target_fds=%p\n", ui->target_fds);
  
  while( initial_cursor_position > 0 )
  {
    mui_NextField(ui);          // mui_next_field(ui) is not sufficient in case of scrolling
    initial_cursor_position--;
  }
  
  while( mui_send_cursor_enter_msg(ui) == 255 )
  {
    mui_NextField(ui);          // mui_next_field(ui) is not sufficient in case of scrolling
  }
}

/* input: current_form_fds */
/*
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
void mui_LeaveForm(mui_t *ui)
{
  if ( mui_IsFormActive(ui) == 0 )
    return;
  
  mui_send_cursor_msg(ui, MUIF_MSG_CURSOR_LEAVE);
  ui->cursor_focus_fds = NULL;
  
  /* inform all fields that we leave the form */
  MUI_DEBUG("mui_LeaveForm: form_end\n");
  mui_loop_over_form(ui, mui_task_form_end);  
  ui->current_form_fds = NULL;
}

/* 0: error, form not found */
/*
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
uint8_t mui_GotoForm(mui_t *ui, uint8_t form_id, uint8_t initial_cursor_position)
{
  fds_t *fds = mui_find_form(ui, form_id);
  if ( fds == NULL )
    return 0;
  /* EnterForm will also leave any previous form */
  mui_EnterForm(ui, fds, initial_cursor_position);
  return 1;
}

/* 
  Save current form+cursor position. Used together with mui_RestoreForm 
*/
void mui_SaveFormWithCursorPosition(mui_t *ui, uint8_t cursor_pos)
{
  if ( mui_IsFormActive(ui) == 0 )
    return;
  
  if ( ui->last_form_stack_pos < MUI_MENU_LAST_FORM_STACK_SIZE-1 )
    ui->last_form_stack_pos++;
  else  // discard the oldes entry
  {
    uint8_t i;
    for( i = 0; i < ui->last_form_stack_pos; i++ )
    {
      ui->last_form_id[i] = ui->last_form_id[i+1];
      ui->last_form_cursor_focus_position[i] = ui->last_form_cursor_focus_position[i+1];
    }
  }
  
  ui->last_form_fds = ui->cursor_focus_fds;             // 25 Aug 2024: I think this is not required, u8g2 data function will store the value manually in last_form_fds
  ui->last_form_id[ui->last_form_stack_pos] = mui_get_fds_char(ui->current_form_fds+1);
  ui->last_form_cursor_focus_position[ui->last_form_stack_pos] = cursor_pos;
}

/* 
  Save current form+cursor position. Simplified version, which will not work with pseudo scrolling forms
*/
void mui_SaveForm(mui_t *ui)
{
  mui_SaveFormWithCursorPosition(ui, mui_GetCurrentCursorFocusPosition(ui));
}


/*
  Restore previous saved form and cursor position.
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
  returns 0 if there is no form to restore
*/
uint8_t mui_RestoreForm(mui_t *ui)
{
  MUI_DEBUG("mui_RestoreForm last_form_stack_pos=%d\n", ui->last_form_stack_pos);
  if ( ui->last_form_stack_pos >= 0 )
  {
    uint8_t form_id = ui->last_form_id[ui->last_form_stack_pos];
    uint8_t focus = ui->last_form_cursor_focus_position[ui->last_form_stack_pos];
    ui->last_form_stack_pos--;
    return mui_GotoForm(ui, form_id, focus);
    //ui->last_form_fds = NULL;
  }
  return 0;
}

/*
  Save a cursor position for mui_GotoFormAutoCursorPosition command
*/
void mui_SaveCursorPosition(mui_t *ui, uint8_t cursor_position)
{
  uint8_t form_id = mui_get_fds_char(ui->current_form_fds+1);
  uint8_t i;
  MUI_DEBUG("mui_SaveCursorPosition form_id=%d cursor_position=%d\n", form_id, cursor_position);
  
  for( i = 0; i < MUI_MENU_CACHE_CNT; i++ )
  {
    if ( form_id == ui->menu_form_id[i] )
    {
      ui->menu_form_last_added = i;
      break;
    }
  }
  if ( i >= MUI_MENU_CACHE_CNT )
  {
      ui->menu_form_last_added++ ;
      if ( ui->menu_form_last_added >= MUI_MENU_CACHE_CNT )
        ui->menu_form_last_added = 0;
  }
    
  /*
  if ( form_id == ui->menu_form_id[0] )
    ui->menu_form_last_added = 0;
  else if ( form_id == ui->menu_form_id[1] )
    ui->menu_form_last_added = 1;
  else 
    ui->menu_form_last_added ^= 1;
  */
  ui->menu_form_id[ui->menu_form_last_added] = form_id;
  ui->menu_form_cursor_focus_position[ui->menu_form_last_added] = cursor_position;
  MUI_DEBUG("mui_SaveCursorPosition ui->menu_form_last_added=%d \n", ui->menu_form_last_added);
}

/*
  Similar to mui_GotoForm, but will jump to previously stored cursor location (mui_SaveCursorPosition) or 0 if the cursor position was not saved.
*/
uint8_t mui_GotoFormAutoCursorPosition(mui_t *ui, uint8_t form_id)
{
  uint8_t cursor_position = 0;
  if ( form_id == ui->menu_form_id[0] )
    cursor_position = ui->menu_form_cursor_focus_position[0];
  if ( form_id == ui->menu_form_id[1] )
    cursor_position = ui->menu_form_cursor_focus_position[1];
  MUI_DEBUG("mui_GotoFormAutoCursorPosition form_id=%d cursor_position=%d\n", form_id, cursor_position);
  return mui_GotoForm(ui, form_id, cursor_position);
}

/*
  return current form id or -1 if the menu system is inactive
*/
int mui_GetCurrentFormId(mui_t *ui)
{
  if ( mui_IsFormActive(ui) == 0 )
    return -1;
  return mui_get_fds_char(ui->current_form_fds+1);
}

/*
  updates "ui->cursor_focus_fds"
*/
/*
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
void mui_NextField(mui_t *ui)
{
  do 
  {
    if ( mui_send_cursor_msg(ui, MUIF_MSG_EVENT_NEXT) )
      return;
    mui_send_cursor_msg(ui, MUIF_MSG_CURSOR_LEAVE);
    mui_next_field(ui);
  } while ( mui_send_cursor_enter_msg(ui) == 255 );
}

/*
  updates "ui->cursor_focus_fds"
*/
/*
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
void mui_PrevField(mui_t *ui)
{
  do
  {
    if ( mui_send_cursor_msg(ui, MUIF_MSG_EVENT_PREV) )
      return;
    mui_send_cursor_msg(ui, MUIF_MSG_CURSOR_LEAVE);
 
    mui_loop_over_form(ui, mui_task_find_prev_cursor_uif);
    ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
    if ( ui->target_fds == NULL )
    {
      //ui->cursor_focus_position = 0;
      mui_loop_over_form(ui, mui_task_find_last_cursor_uif);
      ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
    }
  } while( mui_send_cursor_enter_msg(ui) == 255 );
}


void mui_SendSelect(mui_t *ui)
{
  mui_send_cursor_msg(ui, MUIF_MSG_CURSOR_SELECT);  
}

/*
  Same as mui_SendSelect(), but will try to find a field, which is marked as "execute on select" (MUIF_EXECUTE_ON_SELECT_BUTTON).
  If such a field exists, then this field is executed, otherwise the current field will receive the select message.

  MUIF_EXECUTE_ON_SELECT_BUTTON is set by muif macro MUIF_EXECUTE_ON_SELECT_BUTTON
  
  used by MUIInputVersatileRotaryEncoder.ino example
*/
void mui_SendSelectWithExecuteOnSelectFieldSearch(mui_t *ui)
{
  mui_loop_over_form(ui, mui_task_find_execute_on_select_field);  /* Is there a exec on select field? */
  if ( ui->target_fds != NULL )       /* yes, found, ui->fds already points to the field */
  {
    fds_t *exec_on_select_field = ui->target_fds;
    mui_send_cursor_msg(ui, MUIF_MSG_CURSOR_LEAVE);
    ui->cursor_focus_fds = exec_on_select_field;    /* more cursor on the "exec on select" field */
    mui_send_cursor_enter_msg(ui);      
    mui_send_cursor_msg(ui, MUIF_MSG_CURSOR_SELECT);  
  }
  else
  {
    /* no "exec on select" field found, just send the select message to the field */
    mui_send_cursor_msg(ui, MUIF_MSG_CURSOR_SELECT);  
  }
}


void mui_SendValueIncrement(mui_t *ui)
{
  mui_send_cursor_msg(ui, MUIF_MSG_VALUE_INCREMENT);  
}

void mui_SendValueDecrement(mui_t *ui)
{
  mui_send_cursor_msg(ui, MUIF_MSG_VALUE_DECREMENT);  
}
