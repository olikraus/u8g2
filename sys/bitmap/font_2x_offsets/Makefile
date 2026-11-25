CC ?= cc
CFLAGS ?= -O2 -g -Wall -Wextra
CPPFLAGS += -I../../../csrc -DU8G2_USE_LARGE_FONTS
TARGET = font_2x_offsets$(EXEEXT)
CSRC = ../../../csrc
SOURCES = u8g2_setup.c u8g2_buffer.c u8g2_ll_hvline.c u8g2_hvline.c \
          u8g2_line.c u8g2_box.c u8g2_circle.c u8g2_intersection.c \
          u8g2_font.c u8g2_kerning.c u8g2_fonts.c u8x8_8x8.c \
          u8x8_display.c u8x8_cad.c u8x8_capture.c u8x8_setup.c \
          u8x8_gpio.c u8x8_byte.c u8x8_u16toa.c u8x8_u8toa.c

.PHONY: all test clean
all: $(TARGET)

$(TARGET): main.c $(addprefix $(CSRC)/,$(SOURCES)) $(CSRC)/u8g2.h $(CSRC)/u8x8.h
	$(CC) $(CPPFLAGS) $(CFLAGS) main.c $(addprefix $(CSRC)/,$(SOURCES)) $(LDFLAGS) -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)
