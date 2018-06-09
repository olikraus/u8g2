CC = avr-gcc
OBJCPY = avr-objcopy
SIZE = avr-size
MCU = at90usb1286
F_CPU = 16000000
U8G2_SRC = ../../../csrc
AVR_LIB_SRC = ../lib
CFLAGS = \
	-mmcu=$(MCU) \
	-DF_CPU=$(F_CPU)UL \
	-Os \
	-std=gnu99 \
	-Werror \
	-ffunction-sections \
	-fdata-sections \
	-I$(U8G2_SRC)/ \
	-I$(AVR_LIB_SRC)
LDFLAGS = \
	-Wl,--gc-sections \
	-mmcu=$(MCU)

SRC = $(shell ls $(U8G2_SRC)/*.c) $(shell ls $(AVR_LIB_SRC)/*.c) main.c

OBJ = $(SRC:.c=.o)

main.hex: main.elf
	$(OBJCPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature main.elf main.hex

main.elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

size: main.elf
	$(SIZE) --mcu=$(MCU) --format=avr main.elf

clean:
	rm -f $(OBJ) main.elf main.hex
