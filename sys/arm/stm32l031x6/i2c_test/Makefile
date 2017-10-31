#
#  Generic and Simple GNU ARM Makefile 
#
#  Desinged for the gnu-arm-none-eabi tool chain
#
# 	Features
#		- create hex file
#		- create assembler listing (.dis)
#
#	Limitations
#		- only C-files supported
#		- no automatic dependency checking (call 'make clean' if any .h files are changed)
#
#	Targets:
#		make
#			create hex file, no upload
#		make upload
#			create and upload hex file
#		make clean
#			delete all generated files
#
#  Note:
#  	Display list make database: make -p -f/dev/null | less
#
#================================================
# External tools

# The base directory of gcc-arm-none-eabi
# Can be empty on Ubuntu and installed gcc-arm-none-eabi
# If set, GCCBINPATH must contain a "/" at the end.
GCCBINPATH:=/usr/bin/

#================================================
# Project Information

# The name for the project
TARGETNAME:=i2c_test

# The source files of the project
CSRC:=$(wildcard *.c) 
SSRC:=$(wildcard ../stm32l0xx/src/*.s)

# The CPU architecture (will be used for -mcpu)
# for the LPC824, can we use "cortex-m0plus"?
MCPU:=cortex-m0plus

# Include directory for the system include files
SYSINC:=../stm32l0xx/inc
SYSSRC:=$(wildcard ../stm32l0xx/src/*.c)

# Include directory for the u8g2 include files
U8G2INC:=../../../../csrc/
U8G2SRC:=$(wildcard ../../../../csrc/*.c)

# directory for FatFS
#FFINC:=../fatfs
#FFSRC:=$(wildcard ../fatfs/*.c)

# Directory for the linker script
LDSCRIPTDIR:=.
# Name of the linker script (must be the "keep" script, because the other script is not always working)
LDSCRIPT:=stm32l031x6.ld

#================================================
# Main part of the Makefile starts here. Usually no changes are needed.

# Internal Variable Names
LIBNAME:=$(TARGETNAME).a
ELFNAME:=$(TARGETNAME).elf
HEXNAME:=$(TARGETNAME).hex
DISNAME:=$(TARGETNAME).dis
MAPNAME:=$(TARGETNAME).map
OBJ:=$(CSRC:.c=.o) $(SSRC:.s=.o) $(SYSSRC:.c=.o) $(U8G2SRC:.c=.o)
# $(SYSSRC:.c=.o)  $(FFSRC:.c=.o)

# Replace standard build tools by arm tools
AS:=$(GCCBINPATH)arm-none-eabi-as
CC:=$(GCCBINPATH)arm-none-eabi-gcc
AR:=$(GCCBINPATH)arm-none-eabi-ar
OBJCOPY:=$(GCCBINPATH)arm-none-eabi-objcopy
OBJDUMP:=$(GCCBINPATH)arm-none-eabi-objdump
SIZE:=$(GCCBINPATH)arm-none-eabi-size

# Common flags
COMMON_FLAGS = -mthumb -mcpu=$(MCPU)
COMMON_FLAGS += -DSTM32L031xx
COMMON_FLAGS += -Wall -I. -I$(SYSINC) -I$(U8G2INC) 
# define stack size (defaults to 0x0100)  
# COMMON_FLAGS += -D__STACK_SIZE=0x0100
# COMMON_FLAGS += -Os -flto
COMMON_FLAGS += -Os 
# COMMON_FLAGS += -fstack-protector 
# COMMON_FLAGS += -finstrument-functions
# Do not use stand libs startup code. Uncomment this for gcclib procedures
# memcpy still works, but might be required for __aeabi_uidiv
# COMMON_FLAGS +=  -nostdlib
# remove unused data and function
#COMMON_FLAGS += -ffunction-sections -fdata-sections -fshort-wchar
COMMON_FLAGS += -ffunction-sections -fdata-sections
# C flags
CFLAGS:=$(COMMON_FLAGS) -std=gnu99 
# LD flags
# remove unreferenced procedures and variables, but __isr_vector
GC:=-Wl,--gc-sections  -Wl,--undefined=__isr_vector
MAP:=-Wl,-Map=$(MAPNAME)
LFLAGS:=$(COMMON_FLAGS) $(GC) $(MAP)
#LDLIBS:=--specs=nosys.specs -lc -lc -lnosys -L$(LDSCRIPTDIR) -T $(LDSCRIPT)
LDLIBS:=--specs=nosys.specs -L$(LDSCRIPTDIR) -T $(LDSCRIPT)

# Additional Suffixes
.SUFFIXES: .elf .hex .bin .dis

# Targets
.PHONY: all
all: $(DISNAME) $(HEXNAME)
	$(SIZE) $(ELFNAME)

.PHONY: upload
upload: $(DISNAME) $(HEXNAME) $(ELFNAME)
	stm32flash -e 255 -g 0 -w $(HEXNAME) -v /dev/ttyUSB0
	$(SIZE) $(ELFNAME)

.PHONY: clean
clean:
	$(RM) $(OBJ) $(HEXNAME) $(ELFNAME) $(LIBNAME) $(DISNAME) $(MAPNAME) libssp.a libssp_nonshared.a

# implicit rules
.elf.hex:
	$(OBJCOPY) -O ihex $< $@



# explicit rules

$(ELFNAME): $(LIBNAME)($(OBJ)) libssp.a libssp_nonshared.a
	$(LINK.o) $(LFLAGS) $(LIBNAME) $(LDLIBS) -o $@

$(DISNAME): $(ELFNAME)
	$(OBJDUMP) -D -S $< > $@

# create empty ssp libs for -fstack-protector-all -fstack-protector
libssp.a:
	$(AR) rcs $@
	
libssp_nonshared.a:
	$(AR) rcs $@

	
