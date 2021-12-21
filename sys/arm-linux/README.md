[U8g2 for arm-linux](https://github.com/wuhanstudio/u8g2-arm-linux) has been modified to use
[c-periphery](https://github.com/vsergeev/c-periphery) userspace library.
* Deprecated sysfs method is no longer used for GPIO thus increasing speed and stability, but you can still use sysfs for legacy kernels.
* GPIO, I2C and SPI can be closed and unallocated.
* Overall performance should be better.
* Run as non-root user.
* Thread safe and multiple display capable.
* For Java check out [Java UIO U8g2](https://github.com/sgjava/javauio/tree/main/u8g2)
which uses arm-linux port.

## Non-root access
If you want to access devices without root do the following (you can try udev
rules instead if you wish):
* `sudo groupadd u8g2`
* `sudo usermod -a -G u8g2 username` (Use a non-root username)
* `sudo nano /etc/rc.local`
<pre><code>chown -R root:u8g2 /dev/gpiochip*
chmod -R ug+rw /dev/gpiochip*
chown -R root:u8g2 /dev/i2c*
chmod -R ug+rw /dev/i2c*
chown -R root:u8g2 /dev/spidev*
chmod -R ug+rw /dev/spidev*</code></pre>
* `sudo reboot`

## Download U8g2 project
* `cd ~/`
* `git clone --depth 1 https://github.com/olikraus/u8g2.git`

## Modify source as needed
* Change example (SPI 4 wire hardware for instance)
* `nano ~/u8g2/sys/arm-linux/examples/c-examples/u8g2_4wire_hw_spi/u8g2_4wire_hw_spi.c`
* Change the GPIO chip number (0 uses /dev/gpiochip0)
* `#define GPIO_CHIP_NUM 0`
* Change the SPI bus number (0x10 uses /dev/spidev1.0)
* `#define SPI_BUS 0x10`
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

## Multiple display example using pthreads
After building U8g2 using instructions above.
* `nano ~/u8g2/sys/arm-linux/examples/c-examples/u8g2_sw_i2c_thread/u8g2_sw_i2c_thread.c`
* Change display_1, display_2 and display_3 variables. Obviously you can have only two displays, so modify the code accordingly. This uses software I2C since most SBCs will not have three hardware I2C controllers and changing addresses on displays usually required soldering resistor.
* `cd ~/u8g2/sys/arm-linux`
* `make CPPFLAGS=-DPERIPHERY_GPIO_CDEV_SUPPORT=1 CC=gcc CXX=g++`
* `cd bin`
* `./u8g2_sw_i2c_thread`
