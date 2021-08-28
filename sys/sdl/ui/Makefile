CFLAGS = -g -Wall -I../../../csrc/.  `sdl2-config --cflags`

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) $(shell ls *.c )

OBJ = $(SRC:.c=.o)

u8g2_sdl: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl2-config --libs` -o u8g2_sdl

mui.o: mui.h mui_u8g2.h

mui_u8g2.o: mui.h mui_u8g2.h

main.o: mui.h mui_u8g2.h


clean:	
	-rm $(OBJ) u8g2_sdl

