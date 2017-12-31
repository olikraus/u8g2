CFLAGS = -g -Wall -I../../../csrc/.  `sdl-config --cflags` -fsanitize=address -fsanitize=undefined -fsanitize=bounds-strict -fsanitize=object-size

SRC = $(shell ls ../../../csrc/*.c) ../../bitmap/common/u8x8_d_bitmap.c $(shell ls ../common/*.c ) main.c 

OBJ = $(SRC:.c=.o)

helloworld: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o u8g2_sdl

clean:	
	-rm $(OBJ) u8g2_sdl

