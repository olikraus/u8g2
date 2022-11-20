
.SUFFIXES: .bmf .bdf .png

CFLAGS = -g -Wall -Wno-unused-but-set-variable
#CFLAGS = -O4 -Wall

SRC = bmf2bdf.c
OBJ = $(SRC:.c=.o)

BMF:=$(wildcard *.bmf) 
PNG:=$(BMF:.bmf=.png)


bmf2bdf: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o bmf2bdf

png: bmf2bdf $(PNG)

clean:	
	-rm $(OBJ) bmf2bdf *.bdf *.png *.tga

test:	
	./bmf2bdf -p 24sans-t.bmf >24sans_t.bdf
	../bdfconv/bdfconv -f 1 -v 24sans_t.bdf -n 24sans_t -d ../bdf/7x14.bdf
	convert -trim -flip bdf.tga 24sans_t.png

.bmf.png:
	./bmf2bdf -u $< > $*.bdf
	- ../bdfconv/bdfconv -f 1 -v -m '32-10000' $*.bdf -n $* -d ../bdf/7x14.bdf -a -t
	- convert -trim -flip bdf.tga $@
	- rm bdf.tga
	./bmf2bdf -u -p -x 0 -y 0 -l 127 $< > $*-p.bdf
        # strange: glyph 255 does not work
	- ../bdfconv/bdfconv -f 1 -v -m '32-10000,~255' $*-p.bdf -n $*-p -d ../bdf/7x14.bdf -a -t
	- convert -trim -flip bdf.tga $*-p.png
	- rm bdf.tga


