CFLAGS = -g -Wall -Wpointer-arith -I../../../csrc/.  `sdl2-config --cflags`

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) main.c 

OBJ = $(SRC:.c=.o)

helloworld: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl2-config --libs` -o u8g2_sdl
	
mapgen: mapgen.c 
	$(CC) $(CFLAGS) $(LDFLAGS) mapgen.c -o mapgen


clean:	
	-rm $(OBJ) u8g2_sdl

