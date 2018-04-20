/*

  u8log.c
  


*/

#include <stdint.h>

typedef struct u8log_struct u8log_t;


/* redraw the specified line. */
typedef void (*u8log_cb)(u8log_t * u8log);

struct u8log_struct
{
  /* configuration */
  void *aux_data;		/* pointer to u8x8 or u8g2 */
  uint8_t width, height;	/* size of the terminal */
  u8log_cb cb;			/* callback redraw function */
  uint8_t *screen_buffer;	/* size must be width*heigh bytes */
  uint8_t is_redraw_line_for_each_char;
  
  
  /* internal data */
  //uint8_t last_x, last_y;	/* position of the last printed char */
  uint8_t cursor_x, cursor_y;  /* position of the cursor, might be off screen */
  uint8_t redraw_line;	/* redraw specific line if is_redraw_line is not 0 */
  uint8_t is_redraw_line;
  uint8_t is_redraw_all;
};
typedef struct u8log_struct u8log_t;

/*
static uint8_t u8log_is_on_screen(u8log_t *u8log, uint8_t x, uint8_t y)
{
  if ( x >= u8log->width )
    return 0;
  if ( y >= u8log->height )
    return 0;
  return 1;
}
*/

static void u8log_clear_screen(u8log_t *u8log)
{
  uint8_t *dest = u8log->screen_buffer;
  uint16_t cnt = u8log->height;
  cnt *= u8log->width;
  do
  {
    *dest++ = ' ';
    cnt--;
  } while( cnt > 0 );
  
}


/* scroll the content of the complete buffer, set redraw_line to 255 */
static void u8log_scroll_up(u8log_t *u8log)
{
  uint8_t *dest = u8log->screen_buffer;
  uint8_t *src = dest+u8log->width;
  uint16_t cnt = u8log->height;
  cnt--;
  cnt *= u8log->width;
  do
  {
    *dest++ = *src++;
    cnt--;
  } while( cnt > 0 );
  cnt = u8log->width;
  do
  {
    *dest++ = ' ';
    cnt--;
  } while(cnt > 0);
  u8log->is_redraw_all = 1;
}

/*
  Place the cursor on the screen. This will also scroll, if required 
*/
static void u8log_cursor_on_screen(u8log_t *u8log)
{
  if ( u8log->cursor_x >= u8log->width )
  {
    u8log->cursor_x = 0;
    u8log->cursor_y++;
  }
  while ( u8log->cursor_y >= u8log->height )
  {
    u8log_scroll_up(u8log);
    u8log->cursor_y--;
  }
}

/*
  Write a printable, single char on the screen, do any kind of scrolling
*/
static void u8log_write_to_screen(u8log_t *u8log, uint8_t c)
{
  u8log_cursor_on_screen(u8log);
  u8log->screen_buffer[u8log->cursor_y * u8log->width + u8log->cursor_x] = c;
  u8log->cursor_x++;
  
  if ( u8log->is_redraw_line_for_each_char )
  {
    u8log->is_redraw_line = 1;
    u8log->redraw_line = u8log->cursor_y;
  }
}

/*
  Handle control codes or write the char to the screen.
  Supported control codes are:
  
    \n		10		Goto first position of the next line. Line is marked for redraw.
    \r		13		Goto first position in the same line. Line is marked for redraw.
    \t		9		Jump to the next tab position
    \f		12		Clear the screen and mark redraw for whole screen
    any other char	Write char to screen. Line redraw mark depends on 
				is_redraw_line_for_each_char flag.
*/
void u8log_write_char(u8log_t *u8log, uint8_t c)
{
  switch(c)
  {
    case '\n':	// 10
      u8log->is_redraw_line = 1;
      u8log->redraw_line = u8log->cursor_y;
      u8log->cursor_y++;
      u8log->cursor_x = 0;
      break;	
    case '\r':	// 13
      u8log->is_redraw_line = 1;
      u8log->redraw_line = u8log->cursor_y;
      u8log->cursor_x = 0;
      break;
    case '\t':	// 9
      u8log->cursor_x = (u8log->cursor_x + 8) & 0xf8;
      break;
    case '\f':	// 12
      u8log_clear_screen(u8log);
      u8log->is_redraw_all = 1;
      break;
    default:
      u8log_write_to_screen(u8log, c);
      break;
  }
}

void u8log_init(u8log_t *u8log, uint8_t width, uint8_t height, uint8_t *buf)
{
  memset(u8log, 0, sizeof(u8log_t));
  u8log->width = width;
  u8log->height = height;
  u8log->screen_buffer = buf;
}

