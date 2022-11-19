CFLAGS = -g -Wall -Wno-unused-but-set-variable
#CFLAGS = -O4 -Wall

SRC = bmf2bdf.c

OBJ = $(SRC:.c=.o)

bmf2bdf: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o bmf2bdf

clean:	
	-rm $(OBJ) bmf2bdf

test:	
	./bmf2bdf 24sans-t.bmf >24sans_t.bdf
	../bdfconv/bdfconv -f 1 -v 24sans_t.bdf -n 24sans_t -d ../bdf/7x14.bdf
	convert -trim -flip bdf.tga 24sans_t.png
