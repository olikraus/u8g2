U8G2_COMMON=../../../csrc
PLATFORM_COMMON=../common

CFLAGS = -g -Wall -I$(U8G2_COMMON)/. -I$(PLATFORM_COMMON)/. 


SRC = $(shell ls $(U8G2_COMMON)/*.c) $(shell ls $(PLATFORM_COMMON)/*.c) main.c 

OBJ = $(SRC:.c=.o) 

helloworld: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -li2c -o helloworld

clean:	
	-rm -f $(OBJ) helloworld

