CFLAGS = -g -Wall -I../../../csrc/.  `sdl-config --cflags` -DU8G2_16BIT

SRC = $(shell ls ../../../csrc/*.c) mapgen.c u8g2_d_tga.c

OBJ = $(SRC:.c=.o)

map.c: mapgen gm.map
	./mapgen -o map.c gm.map

mapgen: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o mapgen

clean:	
	-rm $(OBJ) mapgen map.c

