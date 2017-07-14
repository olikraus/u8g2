CFLAGS = -g -Wall -Wpointer-arith -I../../../csrc/. -I../../tga/mapgen/.  `sdl-config --cflags` 

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) ../../tga/mapgen/map.c  main.c item.c

OBJ = $(SRC:.c=.o)

gm: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o gm
	

clean:	
	-rm $(OBJ) u8g2_sdl

