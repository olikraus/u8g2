CFLAGS = -g -Wall -I../../../csrc/.  `sdl-config --cflags` -DU8G2_16BIT

SRC = $(shell ls ../../../csrc/*.c) mapgen.c u8g2_d_tga.c

OBJ = $(SRC:.c=.o)

mapgen: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o mapgen

clean:	
	-rm $(OBJ) mapgen

