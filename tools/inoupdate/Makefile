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
	find ../../sys/arduino/. -name "*.ino" -exec ./inoupdate page_buffer.ino "// U8g2 Contructor List (Picture Loop Page Buffer)" "// End of constructor list" {} \;
	find ../../sys/arduino/. -name "*.ino" -exec ./inoupdate frame_buffer.ino "// U8g2 Contructor List (Frame Buffer)" "// End of constructor list" {} \;
	find ../../sys/arduino/. -name "*.ino" -exec ./inoupdate u8x8.ino "// U8x8 Contructor List" "// End of constructor list" {} \;	
	