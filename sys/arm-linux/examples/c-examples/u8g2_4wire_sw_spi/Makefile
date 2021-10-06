
# Chosse proper compiler for your PI
# NanoPi:             arm-linux-gnueabi-gcc
# Raspberry Pi Zero:  arm-linux-gnueabi-gcc

# Raspberry Pi 2:     arm-linux-gnueabihf-gcc
# OrangePi Zero:      arm-linux-gnueabihf-gcc
# NanoPi NEO:         arm-linux-gnueabihf-gcc
# NanoPi NEO Plus 2:  arm-linux-gnueabihf-gcc

# C-SKY Linux:  csky-linux-gcc

CC=arm-linux-gnueabi-gcc

# IP Address of your PI
PI=pi@raspberrypi.local

TARGET=u8g2_4wire_sw_spi
IDIR= -I ../../../drivers -I ../../../../../csrc -I ../../../port
SRCDIR=../../../../../csrc
OBJDIR=../../../obj
OUTDIR=../../../bin
LDIR= -L ../../../lib
LIBS=  -lm

CFLAGS= $(IDIR) -W -Wall

OBJ+=u8g2_4wire_sw_spi.o\
	../../../port/u8g2port.o\
	../../../drivers/gpio.o\
	../../../drivers/spi.o\
	../../../drivers/i2c.o\
	$(OBJDIR)/mui.o\
	$(OBJDIR)/mui_u8g2.o\
	$(OBJDIR)/u8g2_bitmap.o\
	$(OBJDIR)/u8g2_box.o\
	$(OBJDIR)/u8g2_buffer.o\
	$(OBJDIR)/u8g2_button.o\
	$(OBJDIR)/u8g2_circle.o\
	$(OBJDIR)/u8g2_cleardisplay.o\
	$(OBJDIR)/u8g2_d_memory.o\
	$(OBJDIR)/u8g2_d_setup.o\
	$(OBJDIR)/u8g2_font.o\
	$(OBJDIR)/u8g2_fonts.o\
	$(OBJDIR)/u8g2_hvline.o\
	$(OBJDIR)/u8g2_input_value.o\
	$(OBJDIR)/u8g2_intersection.o\
	$(OBJDIR)/u8g2_kerning.o\
	$(OBJDIR)/u8g2_line.o\
	$(OBJDIR)/u8g2_ll_hvline.o\
	$(OBJDIR)/u8g2_message.o\
	$(OBJDIR)/u8g2_polygon.o\
	$(OBJDIR)/u8g2_selection_list.o\
	$(OBJDIR)/u8g2_setup.o\
	$(OBJDIR)/u8log.o\
	$(OBJDIR)/u8log_u8g2.o\
	$(OBJDIR)/u8log_u8x8.o\
	$(OBJDIR)/u8x8_8x8.o\
	$(OBJDIR)/u8x8_byte.o\
	$(OBJDIR)/u8x8_cad.o\
	$(OBJDIR)/u8x8_capture.o\
	$(OBJDIR)/u8x8_d_a2printer.o\
	$(OBJDIR)/u8x8_debounce.o\
	$(OBJDIR)/u8x8_d_gu800.o\
	$(OBJDIR)/u8x8_d_hd44102.o\
	$(OBJDIR)/u8x8_d_il3820_296x128.o\
	$(OBJDIR)/u8x8_d_ist3020.o\
	$(OBJDIR)/u8x8_d_ist7920.o\
	$(OBJDIR)/u8x8_d_ks0108.o\
	$(OBJDIR)/u8x8_d_lc7981.o\
	$(OBJDIR)/u8x8_d_ld7032_60x32.o\
	$(OBJDIR)/u8x8_d_ls013b7dh03.o\
	$(OBJDIR)/u8x8_d_max7219.o\
	$(OBJDIR)/u8x8_d_pcd8544_84x48.o\
	$(OBJDIR)/u8x8_d_pcf8812.o\
	$(OBJDIR)/u8x8_d_pcf8814_hx1230.o\
	$(OBJDIR)/u8x8_d_s1d15721.o\
	$(OBJDIR)/u8x8_d_s1d15e06.o\
	$(OBJDIR)/u8x8_d_sbn1661.o\
	$(OBJDIR)/u8x8_d_sed1330.o\
	$(OBJDIR)/u8x8_d_sh1106_64x32.o\
	$(OBJDIR)/u8x8_d_sh1106_72x40.o\
	$(OBJDIR)/u8x8_d_sh1107.o\
	$(OBJDIR)/u8x8_d_sh1108.o\
	$(OBJDIR)/u8x8_d_sh1122.o\
	$(OBJDIR)/u8x8_d_ssd1305.o\
	$(OBJDIR)/u8x8_d_ssd1306_128x32.o\
	$(OBJDIR)/u8x8_d_ssd1306_128x64_noname.o\
	$(OBJDIR)/u8x8_d_ssd1306_2040x16.o\
	$(OBJDIR)/u8x8_d_ssd1306_48x64.o\
	$(OBJDIR)/u8x8_d_ssd1306_64x32.o\
	$(OBJDIR)/u8x8_d_ssd1306_64x48.o\
	$(OBJDIR)/u8x8_d_ssd1306_72x40.o\
	$(OBJDIR)/u8x8_d_ssd1306_96x16.o\
	$(OBJDIR)/u8x8_d_ssd1309.o\
	$(OBJDIR)/u8x8_d_ssd1316.o\
	$(OBJDIR)/u8x8_d_ssd1317.o\
	$(OBJDIR)/u8x8_d_ssd1318.o\
	$(OBJDIR)/u8x8_d_ssd1320.o\
	$(OBJDIR)/u8x8_d_ssd1322.o\
	$(OBJDIR)/u8x8_d_ssd1325.o\
	$(OBJDIR)/u8x8_d_ssd1326.o\
	$(OBJDIR)/u8x8_d_ssd1327.o\
	$(OBJDIR)/u8x8_d_ssd1329.o\
	$(OBJDIR)/u8x8_d_ssd1606_172x72.o\
	$(OBJDIR)/u8x8_d_ssd1607_200x200.o\
	$(OBJDIR)/u8x8_d_st7511.o\
	$(OBJDIR)/u8x8_d_st75256.o\
	$(OBJDIR)/u8x8_d_st7528.o\
	$(OBJDIR)/u8x8_d_st75320.o\
	$(OBJDIR)/u8x8_d_st7565.o\
	$(OBJDIR)/u8x8_d_st7567.o\
	$(OBJDIR)/u8x8_d_st7571.o\
	$(OBJDIR)/u8x8_d_st7586s_erc240160.o\
	$(OBJDIR)/u8x8_d_st7586s_s028hn118a.o\
	$(OBJDIR)/u8x8_d_st7586s_ymc240160.o\
	$(OBJDIR)/u8x8_d_st7588.o\
	$(OBJDIR)/u8x8_d_st7920.o\
	$(OBJDIR)/u8x8_d_stdio.o\
	$(OBJDIR)/u8x8_d_t6963.o\
	$(OBJDIR)/u8x8_d_uc1601.o\
	$(OBJDIR)/u8x8_d_uc1604.o\
	$(OBJDIR)/u8x8_d_uc1608.o\
	$(OBJDIR)/u8x8_d_uc1609.o\
	$(OBJDIR)/u8x8_d_uc1610.o\
	$(OBJDIR)/u8x8_d_uc1611.o\
	$(OBJDIR)/u8x8_d_uc1617.o\
	$(OBJDIR)/u8x8_d_uc1638.o\
	$(OBJDIR)/u8x8_d_uc1701_dogs102.o\
	$(OBJDIR)/u8x8_d_uc1701_mini12864.o\
	$(OBJDIR)/u8x8_debounce.o\
	$(OBJDIR)/u8x8_display.o\
	$(OBJDIR)/u8x8_fonts.o\
	$(OBJDIR)/u8x8_gpio.o\
	$(OBJDIR)/u8x8_input_value.o\
	$(OBJDIR)/u8x8_message.o\
	$(OBJDIR)/u8x8_selection_list.o\
	$(OBJDIR)/u8x8_setup.o\
	$(OBJDIR)/u8x8_string.o\
	$(OBJDIR)/u8x8_u16toa.o\
	$(OBJDIR)/u8x8_u8toa.o\

all: directories $(TARGET) 

directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OUTDIR)

$(TARGET):$(OBJ)
	@echo Generating $(TARGET) ...
	@$(CC) -o $(OUTDIR)/$@ $(OBJ) $(LDIR) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
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
