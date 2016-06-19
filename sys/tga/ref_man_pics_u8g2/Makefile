CFLAGS = -g -Wall -I../../../csrc/.  `sdl-config --cflags` -DU8G2_16BIT -DU8G2_REF_MAN_PIC


SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) main.c 

OBJ = $(SRC:.c=.o)

u8g2_tga: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o u8g2_tga

clean:	
	-rm $(OBJ) u8g2_tga

