CFLAGS = -g -Wall -I../../../csrc/.  `sdl2-config --cflags`

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) $(shell ls *.c )

OBJ = $(SRC:.c=.o)

u8g2_sdl: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl2-config --libs` -o u8g2_sdl

ui.o: ui.h ui_u8g2.h

ui_u8g2.o: ui.h ui_u8g2.h

main.o: ui.h ui_u8g2.h


clean:	
	-rm $(OBJ) u8g2_sdl

