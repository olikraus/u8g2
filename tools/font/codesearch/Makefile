# works within ubuntu and min-gw (win7) environment

CFLAGS = -g -DU8G2_USE_LARGE_FONTS -DU8G2_16BIT -Wall -I../../../csrc/
#CFLAGS = -O4 -Wall

SRC2 = codesearch.c ../build/u8g2_font_list.c $(shell ls ../../../csrc/*.c) 

OBJ2 = $(SRC2:.c=.o)

ASM2 = $(SRC2:.c=.s)

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<



codesearch: $(OBJ2) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ2) -o codesearch

clean:	
	-rm $(OBJ2) $(ASM2) codesearch

