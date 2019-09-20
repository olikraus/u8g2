# 
# hex2lpc: Linux flash utility for LPC8xx devices
# 

CFLAGS = -g  -pg -Wall 
#CFLAGS = -O4 -Wall

SRC = hex2lpc.c

OBJ = $(SRC:.c=.o)

hex2lpc: $(SRC)
	$(CC) -Wall -g $(LDFLAGS) $(SRC) -o hex2lpc

clean:	
	-rm ./hex2lpc

