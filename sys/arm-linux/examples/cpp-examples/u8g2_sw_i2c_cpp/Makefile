
# Chosse proper compiler for your PI
# NanoPi:             arm-linux-gnueabi-gcc
# Raspberry Pi Zero:  arm-linux-gnueabi-gcc

# Raspberry Pi 2:     arm-linux-gnueabihf-gcc
# OrangePi Zero:      arm-linux-gnueabihf-gcc
# NanoPi NEO:         arm-linux-gnueabihf-gcc
# NanoPi NEO Plus 2:  arm-linux-gnueabihf-gcc

# C-SKY Linux:  csky-linux-gcc

CC=arm-linux-gnueabi-gcc
CXX=arm-linux-gnueabi-g++

# IP Address of your PI
PI=pi@raspberrypi.local

TARGET=u8g2_sw_i2c_cpp
IDIR= -I ../../../drivers -I ../../../../../csrc -I ../../../port
CSRCDIR=../../../../../csrc
CXXSRCDIR=../../../port
OBJDIR=../../../obj
OUTDIR=../../../bin
LDIR= -L ../../../lib
LIBS=  -lm

CFLAGS= $(IDIR) -W -Wall -D __ARM_LINUX__

OBJ+=u8g2_sw_i2c.cpp.o\
	../../../port/u8g2port.o\
	../../../drivers/gpio.o\
	../../../drivers/spi.o\
	../../../drivers/i2c.o\
	$(OBJDIR)/Print.cpp.o\
	$(OBJDIR)/U8x8lib.cpp.o\
	$(OBJDIR)/U8g2lib.cpp.o\

OBJ+=$(patsubst $(CSRCDIR)/%.c,$(OBJDIR)/%.o, $(wildcard $(CSRCDIR)/*.c))

all: directories $(TARGET) 

directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OUTDIR)

$(TARGET):$(OBJ)
	@echo Generating $(TARGET) ...
	@$(CXX) -o $(OUTDIR)/$@ $(OBJ) $(LDIR) $(LIBS)

u8g2_sw_i2c.cpp.o: u8g2_sw_i2c.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LDIR) $(LIBS)

$(OBJDIR)/%.cpp.o: $(CXXSRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(LDIR) $(LIBS)

$(OBJDIR)/%.o: $(CSRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDIR) $(LIBS)

clean:
	@echo RM -rf $(OBJDIR)/
	@rm -rf $(OBJ)
	@rm -rf $(OBJDIR)
	
	@echo RM -rf $(OUTDIR)/
	@rm -rf $(OUTDIR)

upload:
	scp $(OUTDIR)/$(TARGET) $(PI):~/

run:
	ssh $(PI)
