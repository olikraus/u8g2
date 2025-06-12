CFLAGS = -g -Wall
#CFLAGS = -O4 -Wall

SRC = co.c hbshape2u8g2.c

OBJ = $(SRC:.c=.o)

hbshape2u8g2: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o hbshape2u8g2

clean:	
	-rm $(OBJ) hbshape2u8g2

test:	
	./hbshape2u8g2