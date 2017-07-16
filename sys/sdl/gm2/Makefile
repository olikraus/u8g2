CFLAGS = -g -Wall -Wpointer-arith -I. -I../../../csrc/. -I../../tga/mapgen/. -I../../../tools/ugl/ `sdl-config --cflags` 

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) ../../tga/mapgen/map.c  main.c item.c
SRC += ../../../tools/ugl/ugl_bc.c

OBJ = $(SRC:.c=.o)

gm: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o gm
	

clean:	
	-rm $(OBJ) u8g2_sdl

