CFLAGS = -g -Wall
#CFLAGS = -O4 -Wall

SRC = png2bin.c

OBJ = $(SRC:.c=.o)

png2bin: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o png2bin -lpng

clean:	
	-rm $(OBJ) png2bin

test:	
	./png2bin *.png