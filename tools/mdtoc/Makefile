# 
# Markdown TOC Generator
# type "make wiki" and commit & push wiki 
# 
# works within ubuntu and min-gw (win7) environment

CFLAGS = -g -DBUILD2 -Wall -I../../../csrc/
#CFLAGS = -O4 -Wall

SRC = mdtoc.c

OBJ = $(SRC:.c=.o)

mdtoc: $(SRC)
	$(CC) -Wall -g $(LDFLAGS) $(SRC) -o mdtoc

clean:	
	-rm tmp.md ./mdtoc

wiki:	mdtoc
	./mdtoc  ../../../u8g2.wiki/*.md