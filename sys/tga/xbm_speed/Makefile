CFLAGS = -g -pg -Wall -I../../../csrc/.  `sdl2-config --cflags` -DU8G2_16BIT

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) main.c 

OBJ = $(SRC:.c=.o)

u8g2_tga: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl2-config --libs` -o u8g2_tga

clean:	
	-rm $(OBJ) u8g2_tga

