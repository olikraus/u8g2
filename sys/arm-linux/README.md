[U8g2 for arm-linux](https://github.com/wuhanstudio/u8g2-arm-linux) has been modified to use
[c-periphery](https://github.com/vsergeev/c-periphery) userspace library.
* Deprecated sysfs method is no longer used for GPIO thus increasing speed and stability, but you can still use sysfs for legacy kernels.
* GPIO, I2C and SPI can be closed and unallocated.
* Overall performance should be better.

## Download U8g2 project
* `cd ~/`
* `git clone --depth 1 https://github.com/sgjava/u8g2.git`

## Modify source as needed
Currently GPIO device, I2C and SPI buses are hard coded in u8g2port.c.
* Change values as needed
* `nano ~/u8g2/sys/arm-linux/port/u8g2port.c`
* Change example (SPI 4 wire hardware for instance)
* `nano ~/u8g2/sys/arm-linux/examples/c-examples/u8g2_4wire_hw_spi\u8g2_4wire_hw_spi.c`
* Change the DC and RESET as needed (NanoPi Duo here using tx1 and rx1)
* `#define OLED_SPI_PIN_RES            199`
* `#define OLED_SPI_PIN_DC             198`

## Build source with GPIO character device
* `cd ~/u8g2/sys/arm-linux`
* `make clean`
* `make CPPFLAGS=-DPERIPHERY_GPIO_CDEV_SUPPORT=1 CC=gcc CXX=g++`

## Build source with GPIO sysfs
* `cd ~/u8g2/sys/arm-linux`
* `make clean`
* `make CC=gcc CXX=g++`
