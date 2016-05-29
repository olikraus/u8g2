# works within ubuntu and min-gw (win7) environment

CFLAGS = -g -DBUILD2 -DU8G2_USE_LARGE_FONTS -DU8G2_16BIT -Wall -I../../../csrc/
#CFLAGS = -O4 -Wall

SRC1 = build.c
SRC2 = build.c u8x8_font_list.c u8g2_font_list.c u8g2_d_tga.c $(shell ls ../../../csrc/*.c) 

OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)

ASM1 = $(SRC1:.c=.s)
ASM2 = $(SRC2:.c=.s)

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<


build1: $(SRC1)
	$(CC) -Wall -g $(LDFLAGS) $(SRC1) -o build1
#	./build1

build2: $(OBJ2) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ2) -o build2

clean:	
	-rm $(OBJ1) $(ASM1) build1 $(OBJ2) $(ASM2) build2

test:	
	./build1 