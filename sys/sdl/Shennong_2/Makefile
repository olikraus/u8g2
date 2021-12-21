# CC = gcc-4.8
# CFLAGS = -g -pg -Wall -I../../../csrc/.  -DNO_SDL
CFLAGS = -g -pg -Wall -I../../../csrc/.  `sdl2-config --cflags`
  
SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) main.c 

OBJ = $(SRC:.c=.o)

helloworld: $(OBJ) 
#	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o u8g2_sdl
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl2-config --libs` -o u8g2_sdl

clean:	
	-rm $(OBJ) u8g2_sdl

