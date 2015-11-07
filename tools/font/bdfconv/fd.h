
#ifndef _FD_H
#define _FD_H

#include <stdint.h>

/* font decode */
struct fd_struct
{
  unsigned target_x;
  unsigned target_y;
  unsigned is_transparent;
  
  unsigned x;						/* local coordinates, (0,0) is upper left */
  unsigned y;
  unsigned glyph_width;	
  unsigned glyph_height;
  
  const uint8_t *decode_ptr;			/* pointer to the compressed data */
  unsigned decode_bit_pos;			/* bitpos inside a byte of the compressed data */
  unsigned decode_byte;
  
  uint8_t glyph_cnt;
  uint8_t bits_per_0;
  uint8_t bits_per_1;
  uint8_t bits_per_char_width;
  uint8_t bits_per_char_height;
  uint8_t bits_per_char_x;
  uint8_t bits_per_char_y;
  uint8_t bits_per_delta_x;
  uint8_t char_width;
  uint8_t char_height;
  uint8_t char_descent;
  unsigned capital_a_pos;
  unsigned small_a_pos;
  
  uint8_t *font;
  
};
typedef struct fd_struct fd_t;

void fd_init(fd_t *fd);
void fd_set_font(fd_t *fd, uint8_t *font);
unsigned fd_draw_glyph(fd_t *fd, unsigned x, unsigned y, uint8_t encoding);
unsigned fd_draw_string(fd_t *fd, unsigned x, unsigned y, const char *s);



#endif /* _FD_H */