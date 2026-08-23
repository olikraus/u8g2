CFLAGS = -g -Wall -I../../../csrc/.

SRC = $(shell ls ../../../csrc/*.c) main.c

OBJ = $(SRC:.c=.o)

main: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main

clean:
	-rm -f main $(OBJ)
