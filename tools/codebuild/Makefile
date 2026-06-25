# 
# U8g2 codebuilder
# 

CFLAGS = -g -Wall 
#CFLAGS = -O4 -Wall

SRC = codebuild.c

OBJ = $(SRC:.c=.o)

codebuild: $(SRC)
	$(CC) -Wall -g $(LDFLAGS) $(SRC) -o codebuild

clean:
	-rm ./codebuild
	-rm ./*.md
	-rm ./*.h
	-rm ./u8g2*

build: codebuild
	./codebuild
