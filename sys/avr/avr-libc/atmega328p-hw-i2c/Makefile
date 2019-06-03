CC = avr-gcc
OBJCPY = avr-objcopy
SIZE = avr-size
MCU = atmega328p
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
	-DAVR_USE_HW_I2C 
LDFLAGS = \
	-Wl,--gc-sections \
	-mmcu=$(MCU)
AVRDUDE=avrdude
PORT=/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A4008fhY-if00-port0

SRC = $(shell ls $(U8G2_SRC)/*.c) $(shell ls ../lib/*.c) $(shell ls ../lib/avr-hw-i2c/*.c) $(shell ls ./*.c)

OBJ = $(SRC:.c=.o)

main.hex: main.elf
	$(OBJCPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature main.elf main.hex

main.elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

size: main.elf
	$(SIZE) --mcu=$(MCU) --format=avr main.elf

clean:
	rm -f $(OBJ) main.elf main.hex

# Example for Arduino Duemilanove
upload: main.hex
	$(AVRDUDE) -p$(MCU) -carduino -P$(PORT) -b57600 -D -Uflash:w:main.hex:i
