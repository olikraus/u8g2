CFLAGS = -g -Wall
#CFLAGS = -O4 -Wall

SRC = png2bdf.c

OBJ = $(SRC:.c=.o)

png2bdf: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o png2bdf -lpng

clean:	
	-rm $(OBJ) png2bdf

test:	
	./png2bdf *.png