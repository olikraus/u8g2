# works within ubuntu and min-gw (win7) environment
# win7 exe uploaded on google drive

CFLAGS = -g -Wall
#CFLAGS = -O4 -Wall

SRC = main.c bdf_font.c bdf_glyph.c bdf_parser.c bdf_map.c bdf_rle.c bdf_tga.c fd.c bdf_8x8.c

OBJ = $(SRC:.c=.o)
ASM = $(SRC:.c=.s)

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<

bdfconv: $(OBJ) $(ASM)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o bdfconv

clean:	
	-rm $(OBJ) $(ASM) bdfconv

test:	
	./bdfconv 