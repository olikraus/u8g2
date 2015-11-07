# works within ubuntu and min-gw (win7) environment

CFLAGS = -g -Wall
#CFLAGS = -O4 -Wall

SRC = build.c

OBJ = $(SRC:.c=.o)
ASM = $(SRC:.c=.s)

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<

build: $(OBJ) $(ASM)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o build

clean:	
	-rm $(OBJ) $(ASM) build

test:	
	./build 