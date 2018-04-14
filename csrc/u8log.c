/*

  u8log.c
  


*/

#include <stdint.h>

typedef struct u8log_struct u8log_t;


/* redraw the specified line. If line == 255 then redraw everything */
typedef void (*u8log_cb)(u8log_t * u8log, uint8_t line);

struct u8log_struct
{
  void *aux_data;		/* pointer to u8x8 or u8g2 */
  u8log_cb cb;			/* callback redraw function */
  uint8_t *screen_buffer;	/* size must be width*heigh bytes */
  uint8_t width, height;	/* size of the terminal */
  uint8_t last_x, last_y;	/* position of the last printed char */
  uint8_t cursor_x, cursor_y;  /* position of the cursor, might be off screen */
  uint8_t redraw_line;
};
typedef struct u8log_struct u8log_t;

uint8_t u8log_is_on_screen(u8log_t *u8log, uint8_t x, uint8_t y)
{
  if ( x >= u8log->width )
    return 0;
  if ( y >= u8log->height )
    return 0;
  return 1;
}

void u8log_scroll_up(u8log_t *u8log)
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
  u8log->redraw_line = 255;
}

void u8log_cursor_on_screen(u8log_t *u8log)
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

void u8log_write_to_screen(u8log_t *u8log, uint8_t c)
{
  u8log_cursor_on_screen(u8log);
  u8log->screen_buffer[u8log->cursor_y * u8log->width + u8log->cursor_x] = c;
  u8log->cursor_x++;
}

