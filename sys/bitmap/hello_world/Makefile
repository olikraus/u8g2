CFLAGS = -g -Wall -I../../../csrc/.

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) main.c

OBJ = $(SRC:.c=.o)

hello_world: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

clean:
	-rm -f $(OBJ) hello_world *.tga

