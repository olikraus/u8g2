CFLAGS = -g -Wall -I../../../csrc/.  `sdl-config --cflags`

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) main.c 

OBJ = $(SRC:.c=.o)

helloworld: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o u8g2_sdl

clean:	
	-rm $(OBJ) u8g2_sdl

