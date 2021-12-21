# 
# U8g2 codebuilder
# 

CFLAGS = -g -Wall 
#CFLAGS = -O4 -Wall

SRC = codebuild.c

OBJ = $(SRC:.c=.o)

codebuild: $(SRC)
	# Remove previous controller_list
	sed -i '/\/\* display_controller_list_start \*\//,/\/\* display_controller_list_end \*\//{//!d}'  ./codebuild.c

	# Get the latest controller_list
	sed -n '/\/\* display_controller_list_start \*\//,/\/\* display_controller_list_end \*\//{p;}'  ../../../../tools/codebuild/codebuild.c | sed '1d;$d' > controller_list.txt
	sed -i '/\/\* display_controller_list_start \*\// r controller_list.txt' ./codebuild.c
	rm controller_list.txt

	# Remove the extra line /* display_controller_list_end */
	sed -i '0,/\/\* display_controller_list_end \*\// s///' ./codebuild.c
	sed -i 'N;s/\n\/\* display_controller_list_end \*\//\/\* display_controller_list_end \*\//' ./codebuild.c

	$(CC) -Wall -g $(LDFLAGS) $(SRC) -o codebuild

clean:	
	-rm ./codebuild

build: codebuild
	./codebuild

install: build
	cp u8g2_d_setup.c u8g2_d_memory.c ../../../../csrc/

	# Remove previous C++ headers
	sed -i '/\/\* generated code (codebuild), u8g2 project \*\//,/#endif \/\/ U8X8_USE_PINS/{//!d}'  ../../port/U8g2lib.h
	sed -i '/\/\* generated code (codebuild), u8g2 project \*\//,/#endif \/\/ U8X8_USE_PINS/{//!d}'  ../../port/U8x8lib.h

	# Copy C++ headers into file 
	sed -i '/\/\* generated code (codebuild), u8g2 project \*\// r U8g2lib.h' ../../port/U8g2lib.h
	sed -i '/\/\* generated code (codebuild), u8g2 project \*\// r U8x8lib.h' ../../port/U8x8lib.h

	# Remove the extra line /* generated code (codebuild), u8g2 project */
	sed -i '0,/\/\* generated code (codebuild), u8g2 project \*\// s///' ../../port/U8g2lib.h
	sed -i '0,/\/\* generated code (codebuild), u8g2 project \*\// s///' ../../port/U8x8lib.h
	sed -i 'N;s/\n\/\* generated code (codebuild), u8g2 project \*\//\/\* generated code (codebuild), u8g2 project \*\//' ../../port/U8g2lib.h
	sed -i 'N;s/\n\/\* generated code (codebuild), u8g2 project \*\//\/\* generated code (codebuild), u8g2 project \*\//' ../../port/U8x8lib.h
