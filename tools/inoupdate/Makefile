# 
# .ino Example Updater
# 
# works within ubuntu and min-gw (win7) environment

CFLAGS = -g -Wall 
#CFLAGS = -O4 -Wall

SRC = inoupdate.c

OBJ = $(SRC:.c=.o)

inoupdate: $(SRC)
	$(CC) -Wall -g $(LDFLAGS) $(SRC) -o inoupdate

clean:	
	-rm ./inoupdate

ino:	inoupdate
	echo "tbd"