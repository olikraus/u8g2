
CFLAGS = -O -I../hal -I../../../csrc -DU8X8_USE_PINS -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections

SRC = $(wildcard *.c) $(wildcard ../hal/*.c) $(wildcard ../../../csrc/*.c)

OBJ = $(SRC:.c=.o)


main: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o main

clean:	
	-rm $(OBJ) main

