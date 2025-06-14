/*

  hbshape2u8g2
  
  converts the JSON output from harfbuzz library hb-shape to u8g2 binary format
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2025, olikraus@gmail.com
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
  
  
  Background:
    https://github.com/olikraus/u8g2/issues/2656
    https://github.com/harfbuzz/harfbuzz
    https://github.com/harfbuzz/harfbuzz/discussions/5362
  
  Example:
    hb-shape unifont-16.0.04.otf "नमस्ते" --font-size=16 --output-format=json | ./hbshape2u8g2 "नमस्ते" teststr
    This will generate a c-code vector, which can be passed to u8g2_DrawHB() function  
    

*/

#include "co.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long* utf8_to_unicode(const char *utf8_str, size_t *out_len) {
    size_t i = 0, j = 0, capacity = 16;
    unsigned long *unicode_array = (unsigned long *)malloc(capacity * sizeof(unsigned long));
    if (!unicode_array) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }
    if ( out_len == NULL ) {
        fprintf(stderr, "internal error, out_len is NULL\n");
        return NULL;
    }

    while (utf8_str[i] != '\0') {
        unsigned char c = utf8_str[i];
        unsigned long codepoint = 0;
        size_t bytes = 0;

        if ((c & 0x80) == 0) {
            codepoint = c;
            bytes = 1;
        } else if ((c & 0xE0) == 0xC0) {
            codepoint = c & 0x1F;
            bytes = 2;
        } else if ((c & 0xF0) == 0xE0) {
            codepoint = c & 0x0F;
            bytes = 3;
        } else if ((c & 0xF8) == 0xF0) {
            codepoint = c & 0x07;
            bytes = 4;
        } else {
            fprintf(stderr, "Invalid UTF-8 start byte: 0x%X\n", c);
            free(unicode_array);
            return NULL;
        }

        // Check for continuation bytes
        for (size_t k = 1; k < bytes; ++k) {
            c = utf8_str[i + k];
            if ((c & 0xC0) != 0x80) {
                fprintf(stderr, "Invalid UTF-8 continuation byte: 0x%X\n", c);
                free(unicode_array);
                return NULL;
            }
            codepoint = (codepoint << 6) | (c & 0x3F);
        }

        // Resize array if needed
        if (j >= capacity) {
            capacity *= 2;
            unsigned long *new_array = (unsigned long *)realloc(unicode_array, capacity * sizeof(unsigned long));
            if (!new_array) {
                fprintf(stderr, "realloc failed\n");
                free(unicode_array);
                return NULL;
            }
            unicode_array = new_array;
        }

        unicode_array[j++] = codepoint;
        i += bytes;
    }

    *out_len = j;
    return unicode_array;
}


int cpMapGetIntVal(cco o, const char *key)
{
  cco member;
  member = coMapGet(o, key);
  if ( member == NULL )
  {
    printf("cpMapGetIntVal: key %s not found\n", key);
    return 0; // key doesn't exist
  }
  if ( coIsDbl(member) == 0 )
    return 0; // not a double
  return (int)coDblGet(member);
}

                                      // NULL if key doesn't exist in the map


int main(int argc, char **argv)
{
    co jsonco;
    FILE *jsonfp;
    unsigned long *unicode_list = NULL;
    size_t unicode_cnt = 0;
  
    
    if ( argc < 3 )
    {
            printf("%s input-utf8-text c-identifier [hb-shape.json]\n", argv[0]);
            printf("Convert json result from hb-shape into binary code, which is written to stdout.\n");
            printf("If hb-shape.json is not provided, then %s will try to read from stdin.\n", argv[0]);
            printf("The input-utf8-text must be the same for this tool and for hb-shape.\n");
            printf("Example: hb-shape font.ttf input-utf8-text --font-size=x --output-format=json | %s input-utf8-text identifier\n", argv[0]);
            return 1;
    }
    
    unicode_list = utf8_to_unicode(argv[1], &unicode_cnt);
    
    if ( unicode_list == NULL )
      return 1;
    
    if ( argc >= 4 )
    {
      jsonfp = fopen(argv[3], "rb");
      if ( jsonfp == NULL )
      {
              perror(argv[3]);
              return 2;
      }
    }
    else
    {
      jsonfp = stdin;
    }
    
    jsonco = coReadJSONByFP(jsonfp);
	
	if ( jsonco == NULL )
	{
		printf( "result is NULL\n");
	}
	else if ( !coIsVector(jsonco) )
	{
		printf( "input is not a vector\n");
                coPrint(jsonco);
	}
	else
	{
            long i;
            cco element;
            int x = 0;
            int y = 0;
            int ax;
            int ay;
            int dx;
            int dy;
            // int xb;
            // int yb;
            int cl;
            int cl_last = -1;
            int cl_offset = 0;
            int cl_x = x;
            int cl_y = y;
            int old_pen_x = 0;
            int old_pen_y = 0;
            int pen_x = 0;
            int pen_y = 0;
            uint16_t encoding;
          
            printf("static const unsigned char %s[] U8X8_PROGMEM = {  // \"%s\"\n", argv[2], argv[1]);
            for(i = 0; i < coVectorSize(jsonco); i++ )
            {
              element = coVectorGet(jsonco, i);
              if ( coIsMap(element) )
              {
                encoding = cpMapGetIntVal(element, "g")-1;
                ax = cpMapGetIntVal(element, "ax");
                ay = cpMapGetIntVal(element, "ay");
                dx = cpMapGetIntVal(element, "dx");
                dy = cpMapGetIntVal(element, "dy");
                //xb = cpMapGetIntVal(element, "xb");
                //yb = cpMapGetIntVal(element, "yb");
                cl = cpMapGetIntVal(element, "cl");
                if ( cl_last != cl )
                {
                  cl_last = cl;
                  cl_offset = 0;
                  cl_x = x;
                  cl_y = y;
                }
                else
                {
                  cl_offset++;
                }
                if ( cl_last + cl_offset < unicode_cnt )
                {
                  /*
                    The following statement include several assumption, which may not be valid in some cases
                      * the cluster index can be used as an index into the original input text
                      * no glyph merging has happend (so for example ff is not merged into a new glyph)
                  */
                  encoding = unicode_list[cl_last + cl_offset]; 
                  old_pen_x = pen_x;
                  old_pen_y = pen_y;
                  pen_x = cl_x+dx;
                  pen_y = cl_y+dy;

                  printf("  ");
                  printf("0x%02x, 0x%02x, ", (encoding >> 8) & 255, encoding & 255);
                  printf("0x%02x, 0x%02x, ", pen_x - old_pen_x, pen_y - old_pen_y);
                  printf("// u8g2_DrawGlyph(&u8g2, %d, %d, %u);\n", pen_x, pen_y, encoding);
                }
                x += ax;
                y += ay;
              } // if map
            } // for
            printf("  0x00, 0x00  // end of binary\n");
            printf("};\n");
            
	} // is vector
        
    free(unicode_list);
    if ( jsonfp != stdin )
      fclose(jsonfp);
    coDelete(jsonco);
    return 0;
}

