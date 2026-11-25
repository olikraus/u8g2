# 2026-10-04：沿用上游主机程序布局，构建并运行缓冲矩形回归验证。
CC = gcc
CFLAGS = -g -Wall -Wextra -Wno-unused-parameter -I../../../csrc/.
SRC = $(wildcard ../../../csrc/*.c) main.c
OBJ = $(SRC:.c=.o)

u8g2_frame_xor: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

test: u8g2_frame_xor
	./u8g2_frame_xor

clean:
	-rm -f $(OBJ) u8g2_frame_xor
