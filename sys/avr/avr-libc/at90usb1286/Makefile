CC = avr-gcc
OBJCPY = avr-objcopy
SIZE = avr-size
MCU = at90usb1286
F_CPU = 16000000
U8G2_SRC = ../../../../csrc
CFLAGS = \
	-mmcu=$(MCU) \
	-DF_CPU=$(F_CPU)UL \
	-Os \
	-std=gnu99 \
	-Werror \
	-ffunction-sections \
	-fdata-sections \
	-I$(U8G2_SRC)/ \
	-I../lib/ \
	-DAVR_USE_HW_SPI \
	-DSCK_DDR=DDRB \
	-DSCK_BIT=1 \
	-DMOSI_DDR=DDRB \
	-DMOSI_BIT=2
LDFLAGS = \
	-Wl,--gc-sections \
	-mmcu=$(MCU)
TEENSY_LOADER = teensy_loader_cli

SRC = $(shell ls $(U8G2_SRC)/*.c) $(shell ls ../lib/*.c) $(shell ls ./*.c)

OBJ = $(SRC:.c=.o)

main.hex: main.elf
	$(OBJCPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature main.elf main.hex

main.elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

size: main.elf
	$(SIZE) --mcu=$(MCU) --format=avr main.elf

clean:
	rm -f $(OBJ) main.elf main.hex

# Example for Teensy 2.0++
upload: main.hex
	$(TEENSY_LOADER) -mmcu=$(MCU) -w -v main.hex
