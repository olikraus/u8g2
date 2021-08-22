/*
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
  "Aiixya/text/"  Generic field with argument and text --> ID = ij
  
  Fixed ID:
  "Si" the style                                                        --> ID=@i
  "Lxy/labeltext/"  Places a text at the specified position, field with   -     -> ID=.L, .l
  "Mxya/labeltext/"                                                                                             --> ID=.M
  "Jxyu/menutext/"  Jump button to user interface form u, current u is placed on a stack                --> ID=.J
  "Xxy/menutext/"  Go to the u which is placed on the top of the stack                                                  --> ID=.X
  "Gxyu/menutext/"  Go to the specified menu without placing the user interface form on the stack       --> ID=.G, .g
  
  
  cijxy
  cijxy/text/
  cijxya/text/
  
  cxy/text/
  cxya/text/
  
*/



#include "ui.h"





//#define ui_get_fds_char(s) ((uint8_t)(*s))

uint8_t ui_get_fds_char(fds_t s)
{
  return (uint8_t)(*s);
}


/*
  s must point to a valid command within FDS
*/
size_t ui_fds_get_cmd_size_without_text(ui_t *ui, fds_t s)
{
  uint8_t c = ui_get_fds_char(s);
  c &= 0xdf; /* consider upper and lower case */
  switch(c)
  {
    case 'U': return 2;
    case 'S': return 2;
    case 'F': return 5;
    case 'B': return 5;
    case 'A': return 6;
    case 'L': return 3;
    case 'M': return 4;
    case 'X': return 3;
    case 'J': return 4;
    case 'G': return 4;  
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
size_t ui_fds_parse_text(ui_t *ui, fds_t s)
{
  uint8_t i = 0;
  ui->delimiter = ui_get_fds_char(s);
  uint8_t c;
  fds_t t = s;
  
  //printf("ui_fds_parse_text del=%d\n", delimiter);
#ifdef UI_CHECK_EOFDS
  if ( ui->delimiter == 0 )
    return 0;
#endif 
  t++;
  for( ;; )
  {
    c = ui_get_fds_char(t);
  //printf("ui_fds_parse_text i=%d, c=%c\n", i, c);
#ifdef UI_CHECK_EOFDS
    if ( c == 0 )
      break;
#endif 
    if ( c == ui->delimiter )
    {
      t++;
      break;
    }
    if ( i < UI_MAX_TEXT_LEN )
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

  if ( ui_fds_first_token(ui) )
  {
    do 
    {
      // handle token in ui->text
    } while ( ui_fds_next_token(ui) )
  }

*/
uint8_t ui_fds_first_token(ui_t *ui)
{
  ui->token = ui->fds;
  ui->token += ui_fds_get_cmd_size_without_text(ui, ui->fds);
  ui->delimiter = ui_get_fds_char(ui->token);
  ui->token++;  // place ui->token on the first char of the token
  return ui_fds_next_token(ui);
}


uint8_t ui_fds_next_token(ui_t *ui)
{
  uint8_t c;
  uint8_t i = 0;
  // printf("ui_fds_next_token: call, ui->token=%p\n", ui->token);
  for( ;; )
  {
    c = ui_get_fds_char(ui->token);
    // printf("ui_fds_next_token: i=%d c=%c\n", i, c);
#ifdef UI_CHECK_EOFDS
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
    
    if ( i < UI_MAX_TEXT_LEN )
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
  find nth token, return 0 if n exceeds the number of tokens, 1 otherwise
  the result is stored in ui->text
*/
uint8_t ui_fds_get_nth_token(ui_t *ui, uint8_t n)
{  
  // printf("ui_fds_get_nth_token: call, n=%d\n", n);
  if ( ui_fds_first_token(ui) )
  {
    do 
    {
      if ( n == 0 )
      {
        // printf("ui_fds_get_nth_token: found");
        return 1;
      }
      n--;
    } while ( ui_fds_next_token(ui) );
  }
  //printf("ui_fds_get_nth_token: NOT found\n");
  return 0;
}

uint8_t ui_fds_get_token_cnt(ui_t *ui)
{
  uint8_t n = 0;
  if ( ui_fds_first_token(ui) )
  {
    do 
    {
      n++;
    } while ( ui_fds_next_token(ui) );
  }
  return n;
}


#define ui_fds_is_text(c) ( (c) == 'U' || (c) == 'S' || (c) == 'F' ? 0 : 1 )

/*
  s must point to a valid command within FDS
  return
    The complete length of the command (including any text part)
  sideeffect:
    Any existing text part will be copied into ui->text
    ui->text will be assigned to empty string if there is no text argument
*/
size_t ui_fds_get_cmd_size(ui_t *ui, fds_t s)
{
  size_t l = ui_fds_get_cmd_size_without_text(ui, s);
  uint8_t c = ui_get_fds_char(s);
 ui->text[0] = '\0' ;   /* always reset the text buffer */
 if ( ui_fds_is_text(c) )
  {
    l += ui_fds_parse_text(ui, s+l);
  }
  return l;
}



void ui_Init(ui_t *ui, void *graphics_data, fds_t fds, uif_t *uif_list, size_t uif_cnt)
{
  memset(ui, 0, sizeof(ui_t));
  ui->root_fds = fds;
  ui->current_form_fds = fds;
  ui->uif_list = uif_list;
  ui->uif_cnt = uif_cnt;
  ui->graphics_data = graphics_data;
}

ssize_t ui_find_uif(ui_t *ui, uint8_t id0, uint8_t id1)
{
  ssize_t i;
  for( i = 0; i < ui->uif_cnt; i++ )
  {
      if ( ui->uif_list[i].id0 == id0 )
        if ( ui->uif_list[i].id1 == id1 )
          return i;
  }
  return -1;
}


/*
  assumes a valid position in ui->fds and calculates all the other variables
  some fields are alway calculated like the ui->cmd and ui->len field
  other member vars are calculated only if the return value is 1
  will return 1 if the field id was found.
  will return 0 if the field id was not found in uif or if ui->fds points to something else than a field
*/
uint8_t ui_prepare_current_field(ui_t *ui)
{
  ssize_t uif_idx;

  ui->uif = NULL;
  ui->dflags = 0;    
  ui->id0 = 0;
  ui->id1 = 0;
  ui->arg = 0;

  /* calculate the length of the command and copy the text argument */
  /* this will also clear the text in cases where there is no text argument */
  ui->len = ui_fds_get_cmd_size(ui, ui->fds); 

  /* get the command and check whether end of form is reached */
  ui->cmd = ui_get_fds_char(ui->fds);
  
  /* Copy the cmd also to second id value. This is required for some commands, others will overwrite this below */
  ui->id1 = ui->cmd;
  
  /* now make the command uppercase so that both, upper and lower case are considered */
  ui->cmd &= 0xdf; /* consider upper and lower case */
  
  if ( ui->cmd == 'U' || ui->cmd == 0 )
    return 0;

  /* calculate the dynamic flags */
  if ( ui->fds == ui->cursor_focus_fds )
    ui->dflags |= UIF_DFLAG_IS_CURSOR_FOCUS;
  if ( ui->fds == ui->touch_focus_fds )
    ui->dflags |= UIF_DFLAG_IS_TOUCH_FOCUS;
  

  /* get the id0 and id1 values */
  if  ( ui->cmd == 'F' || ui->cmd == 'B' || ui->cmd == 'A' )
  {
      ui->id0 = ui_get_fds_char(ui->fds+1);
      ui->id1 = ui_get_fds_char(ui->fds+2);
      ui->x = ui_get_fds_char(ui->fds+3);
      ui->y = ui_get_fds_char(ui->fds+4);
      if ( ui->cmd == 'A' )
      {
        ui->arg = ui_get_fds_char(ui->fds+5);
      }
  }
  else if ( ui->cmd == 'S' )
  {
      ui->id0 = '@';
      ui->id1 = ui_get_fds_char(ui->fds+1);
  }
  else
  {
      ui->id0 = '.';
      /* note that ui->id1 contains the original cmd value */
      ui->x = ui_get_fds_char(ui->fds+1);
      ui->y = ui_get_fds_char(ui->fds+2);
      if ( ui->cmd == 'G' || ui->cmd == 'M' )  /* this is also true for 'g' or 'm' */
      {
        ui->arg = ui_get_fds_char(ui->fds+3);
      }
  }
  
  /* find the field  */
  uif_idx = ui_find_uif(ui, ui->id0, ui->id1);
  //printf("ui_prepare_current_field: uif_idx=%d\n", uif_idx);
  if ( uif_idx >= 0 )
  {
    ui->uif = ui->uif_list + uif_idx;
    return 1;
  }
  return 0;
}

void ui_loop_over_form(ui_t *ui, uint8_t (*task)(ui_t *ui))
{
  uint8_t cmd;
  
  if ( ui->current_form_fds == NULL )
    return;
  
  ui->fds = ui->current_form_fds;
  ui->target_fds = NULL;
  ui->tmp_fds = NULL;
  
  ui->fds += ui_fds_get_cmd_size(ui, ui->fds);      // skip the first entry, it is U always
  for(;;)
  {    
    //printf("fds=%p *fds=%d\n", ui->fds, ui->fds[0]);
    /* get the command and check whether end of form is reached */
    cmd = ui_get_fds_char(ui->fds);
    if ( cmd == 'U' || cmd == 0 )
      break;
    if ( ui_prepare_current_field(ui) )
      if ( task(ui) )         /* call the task, which was provided as argument to this function */
        break;
    ui->fds += ui->len;
  }
  //printf("ui_loop_over_form ended\n");
}

/*
  n is the form number
*/
fds_t ui_find_form(ui_t *ui, uint8_t n)
{
  fds_t fds = ui->root_fds;
  uint8_t cmd;
  
  for( ;; )
  {
    cmd = ui_get_fds_char(fds);
    if ( cmd == 0 )
      break;
    if ( cmd == 'U'  )
    {
      if (   ui_get_fds_char(fds+1) == n )
      {
        return fds;
      }
      /* not found, just coninue */
    }
    
    fds += ui_fds_get_cmd_size(ui, fds);
  }
  return NULL;
}

/* === task procedures (arguments for ui_loop_over_form === */
/* ui->fds contains the current field */

uint8_t ui_task_draw(ui_t *ui)
{
  //printf("ui_task_draw fds=%p uif=%p text=%s\n", ui->fds, ui->uif, ui->text);
  uif_get_cb(ui->uif)(ui, UIF_MSG_DRAW);
  return 0;     /* continue with the loop */
}

uint8_t ui_task_form_start(ui_t *ui)
{
  uif_get_cb(ui->uif)(ui, UIF_MSG_FORM_START);
  return 0;     /* continue with the loop */
}

uint8_t ui_task_form_end(ui_t *ui)
{
  uif_get_cb(ui->uif)(ui, UIF_MSG_FORM_END);
  return 0;     /* continue with the loop */
}


uint8_t ui_task_find_prev_cursor_uif(ui_t *ui)
{
  if ( uif_get_cflags(ui->uif) & UIF_CFLAG_IS_CURSOR_SELECTABLE )
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

uint8_t ui_task_find_first_cursor_uif(ui_t *ui)
{
  if ( uif_get_cflags(ui->uif) & UIF_CFLAG_IS_CURSOR_SELECTABLE )
  {
    // if ( ui->target_fds == NULL )
    // {
      ui->target_fds = ui->fds;
      return 1;         /* stop looping */
    // }
  }
  return 0;     /* continue with the loop */
}

uint8_t ui_task_find_last_cursor_uif(ui_t *ui)
{
  if ( uif_get_cflags(ui->uif) & UIF_CFLAG_IS_CURSOR_SELECTABLE )
  {
    //ui->cursor_focus_position++;
    ui->target_fds = ui->fds;
  }
  return 0;     /* continue with the loop */
}

uint8_t ui_task_find_next_cursor_uif(ui_t *ui)
{
  if ( uif_get_cflags(ui->uif) & UIF_CFLAG_IS_CURSOR_SELECTABLE )
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

uint8_t ui_task_get_current_cursor_focus_position(ui_t *ui)
{
  if ( uif_get_cflags(ui->uif) & UIF_CFLAG_IS_CURSOR_SELECTABLE )
  {
    if ( ui->fds == ui->cursor_focus_fds )
      return 1;         /* stop looping */
    ui->tmp8++;
  }
  return 0;     /* continue with the loop */
}

/* === utility functions for the user API === */

void ui_send_cursor_msg(ui_t *ui, uint8_t msg)
{
  if ( ui->cursor_focus_fds )
  {
    ui->fds = ui->cursor_focus_fds;
    if ( ui_prepare_current_field(ui) )
      uif_get_cb(ui->uif)(ui, msg);
  }
}

/* === user API === */

/* 
  returns the field pos which has the current focus 
  If the first selectable field has the focus, then 0 will be returned
  Unselectable fields (for example labels) are skipped by this count.
  If no fields are selectable, then 0 is returned
*/
uint8_t ui_GetCurrentCursorFocusPosition(ui_t *ui)
{
  ui->tmp8 = 0;
  ui_loop_over_form(ui, ui_task_get_current_cursor_focus_position);
  return ui->tmp8;
}


void ui_Draw(ui_t *ui)
{
  ui_loop_over_form(ui, ui_task_draw);
}

void ui_next_field(ui_t *ui)
{
  ui_loop_over_form(ui, ui_task_find_next_cursor_uif);
  // ui->cursor_focus_position++;
  ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
  if ( ui->target_fds == NULL )
  {
    ui_loop_over_form(ui, ui_task_find_first_cursor_uif);
    ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
    // ui->cursor_focus_position = 0;
  }
}

/* 
  input: current_form_fds 
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
void ui_EnterForm(ui_t *ui, uint8_t initial_cursor_position)
{
  /* clean focus fields */
  ui->touch_focus_fds = NULL;
  ui->cursor_focus_fds = NULL;
  
  /* inform all fields that we start a new form */
  ui_loop_over_form(ui, ui_task_form_start);
  
  /* assign initional cursor focus */
  ui_loop_over_form(ui, ui_task_find_first_cursor_uif);
  
  
  while( initial_cursor_position > 0 )
  {
    ui_next_field(ui);
    initial_cursor_position--;
  }
  
  ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
  ui_send_cursor_msg(ui, UIF_MSG_CURSOR_ENTER);
}

/* input: current_form_fds */
/*
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
void ui_LeaveForm(ui_t *ui)
{
  ui_send_cursor_msg(ui, UIF_MSG_CURSOR_LEAVE);
  ui->cursor_focus_fds = NULL;
  
  /* inform all fields that we leave the form */
  ui_loop_over_form(ui, ui_task_form_end);  
  ui->current_form_fds = NULL;
}

/* 0: error, form not found */
/*
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
uint8_t ui_GotoForm(ui_t *ui, uint8_t form_id, uint8_t initial_cursor_position)
{
  fds_t fds = ui_find_form(ui, form_id);
  if ( fds == NULL )
    return 0;
  ui_LeaveForm(ui);
  ui->current_form_fds = fds;
  ui_EnterForm(ui, initial_cursor_position);
  return 1;
}

void ui_SaveForm(ui_t *ui)
{
  if ( ui->current_form_fds == NULL )
    return;

  ui->last_form_id = ui_get_fds_char(ui->current_form_fds+1);
  ui->last_form_cursor_focus_position = ui_GetCurrentCursorFocusPosition(ui);
}

/*
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
void ui_RestoreForm(ui_t *ui)
{
  ui_GotoForm(ui, ui->last_form_id, ui->last_form_cursor_focus_position);
}

/*
  updates "ui->cursor_focus_fds"
*/
/*
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
void ui_NextField(ui_t *ui)
{
  ui_send_cursor_msg(ui, UIF_MSG_CURSOR_LEAVE);
  ui_next_field(ui);
  ui_send_cursor_msg(ui, UIF_MSG_CURSOR_ENTER);
}

/*
  updates "ui->cursor_focus_fds"
*/
/*
  if called from a field function, then the current field variables are destroyed, so that call should be the last call in the field callback.
*/
void ui_PrevField(ui_t *ui)
{
  ui_send_cursor_msg(ui, UIF_MSG_CURSOR_LEAVE);
  
  ui_loop_over_form(ui, ui_task_find_prev_cursor_uif);
  //ui->cursor_focus_position--;
  ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
  if ( ui->target_fds == NULL )
  {
    //ui->cursor_focus_position = 0;
    ui_loop_over_form(ui, ui_task_find_last_cursor_uif);
    ui->cursor_focus_fds = ui->target_fds;      // NULL is ok  
  }
  
  ui_send_cursor_msg(ui, UIF_MSG_CURSOR_ENTER);
}


void ui_SendSelect(ui_t *ui)
{
  ui_send_cursor_msg(ui, UIF_MSG_CURSOR_SELECT);  
}

