
from building import *
import rtconfig

# get current directory
cwd     = GetCurrentDir()

# The set of source files associated with this SConscript file.
src     = Glob('sys/rt-thread/port/u8g2_port.c')
src    += Glob('csrc/*.c')

if(GetDepend('U8G2_USE_CPP')):
	src    += Glob('sys/rt-thread/port/*.cpp')

if(not GetDepend('U8G2_USE_CPP')):

	if GetDepend('U8G2_USING_SW_I2C_SSD1306'):
		src    += Glob('sys/rt-thread/examples/ssd1306_12864_sw_i2c_example.c')

	if GetDepend('U8G2_USING_HW_I2C_SSD1306'):
		src    += Glob('sys/rt-thread/examples/ssd1306_12864_hw_i2c_example.c')

	if GetDepend('U8G2_USING_SW_SPI_SSD1306'):
		src    += Glob('sys/rt-thread/examples/ssd1306_12864_4wire_sw_spi_example.c')

	if GetDepend('U8G2_USING_HW_SPI_SSD1306'):
		src    += Glob('sys/rt-thread/examples/ssd1306_12864_4wire_hw_spi_example.c')

	if GetDepend('U8G2_USING_8080_ST7920'):
		src    += Glob('sys/rt-thread/examples/st7920_12864_8080_example.c')

	if GetDepend('U8G2_USING_I2C_YL40'):
		src    += Glob('sys/rt-thread/examples/yl_40_example.c')

else:

	# Basic Examples
	if GetDepend('U8G2_USING_SW_I2C_SSD1306'):
		src    += Glob('sys/rt-thread/examples/ssd1306_12864_sw_i2c_example.cpp')

	if GetDepend('U8G2_USING_HW_I2C_SSD1306'):
		src    += Glob('sys/rt-thread/examples/ssd1306_12864_hw_i2c_example.cpp')

	if GetDepend('U8G2_USING_SW_SPI_SSD1306'):
		src    += Glob('sys/rt-thread/examples/ssd1306_12864_4wire_sw_spi_example.cpp')

	if GetDepend('U8G2_USING_HW_SPI_SSD1306'):
		src    += Glob('sys/rt-thread/examples/ssd1306_12864_4wire_hw_spi_example.cpp')

	if GetDepend('U8G2_USING_8080_ST7920'):
		src    += Glob('sys/rt-thread/examples/st7920_12864_8080_example.cpp')

	if GetDepend('U8G2_USING_I2C_YL40'):
		src    += Glob('sys/rt-thread/examples/yl_40_example.c')

path    = [cwd + '/sys/rt-thread/port']
path   += [cwd + '/csrc']

LOCAL_CCFLAGS = ''

group = DefineGroup('U8G2', src, depend = ['PKG_USING_U8G2'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS, CPPDEFINES=['RTT_U8G2'])

Return('group')
