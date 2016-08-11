rem make.bat
rem
rem Create lpc810_led_driver.hex file on windows operating system.
rem Requires https://launchpad.net/gcc-arm-embedded/ “Win32 Installer”
rem
set GNUARMPATH="C:\Program Files (x86)\GNU Tools ARM Embedded"
set GCC=%GNUARMPATH%\bin\arm-none-eabi-gcc.exe
set OBJCOPY=%GNUARMPATH%\bin\arm-none-eabi-objcopy.exe
set CCFLAGS=-DCORE_M0PLUS -Os -flto -mthumb -mcpu=cortex-m0 -Wall -ffunction-sections -fdata-sections -std=gnu99
set LDFLAGS=-Wl,--gc-sections  -Wl,--undefined=arm_stack_area -Wl,--undefined=__isr_vector
set LIBS=--specs=nano.specs -lc -lc -lnosys -L. -T lpc810.ld
set GCCINC=-I. -I..\lpc_chip_8xx_lib\inc
set EXTDIR=..\lpc_chip_8xx_lib\src
set EXTSRC=%EXTDIR%\acmp_8xx.c %EXTDIR%\sct_8xx.c %EXTDIR%\swm_8xx.c %EXTDIR%\gpio_8xx.c %EXTDIR%\pmu_8xx.c 
set EXTSRC=%EXTSRC% %EXTDIR%\iocon_8xx.c  %EXTDIR%\syscon_8xx.c
%GCC% %GCCINC% %CCFLAGS% lpc810_led_driver.c %EXTSRC% %LDFLAGS% %LIBS% -o lpc810_led_driver.elf
%OBJCOPY% -O ihex lpc810_led_driver.elf lpc810_led_driver.hex

