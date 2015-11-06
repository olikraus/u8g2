CFLAGS = -I../../csrc -g -Wall
#CFLAGS = -O4 -Wall

SRC = $(shell ls ../../csrc/*.c 2>/dev/null) test.c

OBJ = $(SRC:.c=.o)

stdio: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o stdio

clean:	
	-rm $(OBJ) stdio

test:	
	./stdio