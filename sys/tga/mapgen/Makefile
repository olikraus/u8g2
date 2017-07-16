CFLAGS = -g -Wall -I../../../csrc/.  -I../../../tools/ugl/. -DU8G2_16BIT -DUGL_TEST

SRC = $(shell ls ../../../csrc/*.c) mapgen.c u8g2_d_tga.c
SRC += ../../../tools/ugl/ugl_arrays.c  
SRC += ../../../tools/ugl/ugl_error.c  
SRC += ../../../tools/ugl/ugl_parse.c
SRC += ../../../tools/ugl/ugl_bc.c

OBJ = $(SRC:.c=.o)

map.c: mapgen gm.map
	./mapgen -o map.c gm.map

mapgen: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ)  -o mapgen

clean:	
	-rm $(OBJ) mapgen map.c

