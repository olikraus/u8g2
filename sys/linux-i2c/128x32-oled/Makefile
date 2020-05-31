
CFLAGS = -g -Wall -I../../../csrc/. -I../common/. 

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c) main.c 

OBJ = $(SRC:.c=.o) 

helloworld: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -li2c -o helloworld

clean:	
	-rm -f $(OBJ) helloworld

