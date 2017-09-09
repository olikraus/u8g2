CFLAGS = -g -Wall -I../../../csrc/ -ICImg
CXXFLAGS = $(CFLAGS)
LDFLAGS = -lpthread
CSRC = $(wildcard ls ../../../csrc/*.c) ../common/u8x8_d_bitmap.c
CPPSRC = main.cpp
OBJ = $(CSRC:.c=.o) $(CPPSRC:.cpp=.o)

cimg_annotate_screenshot: $(OBJ) CImg
	$(CXX) $(CFLAGS) $(LDFLAGS) $(OBJ) -o cimg_annotate_screenshot

CImg:
	git clone --depth=1 https://github.com/dtschump/CImg.git

clean:
	-rm $(OBJ) cimg_annotate_screenshot

