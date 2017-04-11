/*

  term.h

  LPC11U3x GPS Logger (https://github.com/olikraus/lpc11u3x-gps-logger)

  Copyright (c) 2016, olikraus@gmail.com
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

*/

#ifndef _TERM_H
#define _TERM_H

#include <stdint.h>

/*
  SCROLL msg also includes NEWLINE, this means NEWLINE and SCROLL will not appear at the same time
*/
#define TERM_MSG_PRE_CHAR 1
#define TERM_MSG_POST_CHAR 2
#define TERM_MSG_PRE_NEWLINE 3
#define TERM_MSG_POST_NEWLINE 4
#define TERM_MSG_PRE_SCROLL 5
#define TERM_MSG_POST_SCROLL 6


typedef struct _term_struct term_t;
typedef void (*term_show_cb)(term_t *term, uint8_t msg);

struct _term_struct
{
  uint8_t width;	/* width of the screen buffer */
  uint8_t height;	/* height of the screen buffer */
  uint8_t cursor_x;
  uint8_t cursor_y;
  char *screen;		/* pointer to width*height bytes */
  term_show_cb show;
};


void term_Init(term_t *term, uint8_t width, uint8_t height, char *memory, term_show_cb show);
void term_Home(term_t *term);
void term_Clear(term_t *term);
void term_NewLine(term_t *term);
void term_WriteString(term_t *term, const char *str);

#endif

