# works within ubuntu and min-gw (win7) environment
# win7 exe uploaded on google drive

CFLAGS = -g -Wall
#CFLAGS = -O4 -Wall

SRC = main.c bdf_font.c bdf_glyph.c bdf_parser.c bdf_map.c bdf_rle.c bdf_tga.c fd.c bdf_8x8.c

OBJ = $(SRC:.c=.o)
ASM = $(SRC:.c=.s)

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<

bdfconv: $(OBJ) $(ASM)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o bdfconv

clean:	
	-rm $(OBJ) $(ASM) bdfconv

test:	
	./bdfconv 

greek:
	./bdfconv -f 1 -v -m '32-127,$$370-$$3ff' ../bdf/unifont.bdf -n unifont_greek -o unifont_greek.c -d ../bdf/7x13.bdf
	
helvb18:
	./bdfconv -f 1 -m '32-127' ../bdf/helvB18.bdf -o helvb18_tr.c -n u8g2_font_helvB18_tr

helvb18n:
	./bdfconv -v -f 1 -m '32,48-57' ../bdf/helvB18.bdf -o helvb18_tn.c -n u8g2_font_helvB18_tn -d ../bdf/helvB18.bdf

artos:
	./bdfconv -b 3 -d ../bdf/7x13.bdf ../bdf/ArtosSerif-8.bdf -a -v
	
#	http://stackoverflow.com/questions/713397/what-is-a-minimal-set-of-unicode-characters-for-reasonable-japanese-support