#include "u8g2_port.h"
#include <string.h>

#define MAX_RETRY 3

#if defined U8G2_USE_HW_I2C
static struct rt_i2c_bus_device *i2c_bus = RT_NULL;
#endif

#if defined U8G2_USE_HW_SPI
static struct rt_spi_device u8g2_spi_dev;


static inline void u8g2_port_pin_mode(uint8_t pin, rt_uint8_t mode)
{
    if(pin != U8X8_PIN_NONE)
    {
        rt_pin_mode(pin, mode);
    }
}

static inline void u8g2_port_pin_write(uint8_t pin, uint8_t value)
{
    if(pin != U8X8_PIN_NONE)
    {
        rt_pin_write(pin, value);
    }
}

static inline int u8g2_port_pin_read(uint8_t pin)
{
    if(pin != U8X8_PIN_NONE)
    {
        return rt_pin_read(pin);
    }

    return 0;
}

int rt_hw_spi_config(uint8_t spi_mode, uint32_t max_hz, uint8_t cs_pin )
{
    rt_err_t res;

    // Attach Device
    u8g2_port_pin_mode(cs_pin, PIN_MODE_OUTPUT);
    // spi cs signal will be controlled directly using gpio
    if(u8g2_spi_dev.bus == RT_NULL)
    {
        res = rt_spi_bus_attach_device(&u8g2_spi_dev, U8G2_SPI_DEVICE_NAME, U8G2_SPI_BUS_NAME, RT_NULL);
        if (res != RT_EOK)
        {
            rt_kprintf("[u8g2] Failed to attach device %s\n", U8G2_SPI_DEVICE_NAME);
            return res;
        }
        else
        {
            rt_kprintf("[u8g2] Attach device to %s\n", U8G2_SPI_DEVICE_NAME);
        }
    }
    else
    {
        rt_kprintf("[u8g2] Found device %s\n", U8G2_SPI_DEVICE_NAME);
    }

    // Set device SPI Mode
    struct rt_spi_configuration cfg;
    cfg.data_width = 8;
    switch(spi_mode)
    {
        case 0: cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB; break;
        case 1: cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_1 | RT_SPI_MSB; break;
        case 2: cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_2 | RT_SPI_MSB; break;
        case 3: cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_3 | RT_SPI_MSB; break;
    }
    cfg.max_hz = max_hz; /* 20M,SPI max 42MHz,ssd1351 4-wire spi */
    rt_spi_configure(&u8g2_spi_dev, &cfg);

    return RT_EOK;
}
#endif /* U8G2_USE_HW_SPI */

uint8_t u8x8_gpio_and_delay_rtthread(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t i;
    switch(msg)
    {
        case U8X8_MSG_DELAY_NANO:               // delay arg_int * 1 nano second
            __asm__ volatile("nop");
            break;

        case U8X8_MSG_DELAY_100NANO:            // delay arg_int * 100 nano seconds
            __asm__ volatile("nop");
            break;

        case U8X8_MSG_DELAY_10MICRO:            // delay arg_int * 10 micro seconds
            for (uint16_t n = 0; n < 320; n++)
            {
                __asm__ volatile("nop");
            }
        break;

        case U8X8_MSG_DELAY_MILLI:              // delay arg_int * 1 milli second
            rt_thread_mdelay(arg_int);
            break;

        case U8X8_MSG_GPIO_AND_DELAY_INIT:  
            // Function which implements a delay, arg_int contains the amount of ms  

            // set spi pin mode 
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_SPI_CLOCK], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_SPI_DATA], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_RESET], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_DC], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_CS], PIN_MODE_OUTPUT);

            // set i2c pin mode
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_I2C_DATA], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_I2C_CLOCK], PIN_MODE_OUTPUT);

            // set 8080 pin mode
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_D0], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_D1], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_D2], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_D3], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_D4], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_D5], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_D6], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_D7], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_E], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_DC], PIN_MODE_OUTPUT);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_RESET], PIN_MODE_OUTPUT);

            // set menu pin mode
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_MENU_HOME], PIN_MODE_INPUT_PULLUP);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_MENU_SELECT], PIN_MODE_INPUT_PULLUP);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_MENU_PREV], PIN_MODE_INPUT_PULLUP);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_MENU_NEXT], PIN_MODE_INPUT_PULLUP);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_MENU_UP], PIN_MODE_INPUT_PULLUP);
            u8g2_port_pin_mode(u8x8->pins[U8X8_PIN_MENU_DOWN], PIN_MODE_INPUT_PULLUP);

            // set value
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_SPI_CLOCK], 1);
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_SPI_DATA], 1);
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_RESET], 1);
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_DC], 1);
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_CS], 1);
            break;

        case U8X8_MSG_DELAY_I2C:
            // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
            for (uint16_t n = 0; n < (arg_int<=2?160:40); n++)
            {
                __asm__ volatile("nop");
            }
            break;

        //case U8X8_MSG_GPIO_D0:                // D0 or SPI clock pin: Output level in arg_int
        //case U8X8_MSG_GPIO_SPI_CLOCK:

        //case U8X8_MSG_GPIO_D1:                // D1 or SPI data pin: Output level in arg_int
        //case U8X8_MSG_GPIO_SPI_DATA:

        case U8X8_MSG_GPIO_D2:                  // D2 pin: Output level in arg_int
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_D2], arg_int);
            break;

        case U8X8_MSG_GPIO_D3:                  // D3 pin: Output level in arg_int
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_D3], arg_int);
            break;

        case U8X8_MSG_GPIO_D4:                  // D4 pin: Output level in arg_int
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_D4], arg_int);
            break;

        case U8X8_MSG_GPIO_D5:                  // D5 pin: Output level in arg_int
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_D5], arg_int);
            break;

        case U8X8_MSG_GPIO_D6:                  // D6 pin: Output level in arg_int
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_D6], arg_int);
            break;

        case U8X8_MSG_GPIO_D7:                  // D7 pin: Output level in arg_int
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_D7], arg_int);
            break;

        case U8X8_MSG_GPIO_E:                   // E/WR pin: Output level in arg_int
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_E], arg_int);
            break;

        case U8X8_MSG_GPIO_I2C_CLOCK:
            // arg_int=0: Output low at I2C clock pin
            // arg_int=1: Input dir with pullup high for I2C clock pin
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_I2C_CLOCK], arg_int);
            break;

        case U8X8_MSG_GPIO_I2C_DATA:
            // arg_int=0: Output low at I2C data pin
            // arg_int=1: Input dir with pullup high for I2C data pin
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_I2C_DATA], arg_int);
            break;

        case U8X8_MSG_GPIO_SPI_CLOCK:  
            // Function to define the logic level of the clockline  
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_SPI_CLOCK], arg_int);
            break;

        case U8X8_MSG_GPIO_SPI_DATA:
            // Function to define the logic level of the data line to the display  
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_SPI_DATA], arg_int);
            break;

        case U8X8_MSG_GPIO_CS:
            // Function to define the logic level of the CS line  
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_CS], arg_int);
            break;

        case U8X8_MSG_GPIO_DC:
            // Function to define the logic level of the Data/ Command line  
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_DC], arg_int);
            break;

        case U8X8_MSG_GPIO_RESET:
            // Function to define the logic level of the RESET line
            u8g2_port_pin_write(u8x8->pins[U8X8_PIN_RESET], arg_int);
            break;

        default:
            if ( msg >= U8X8_MSG_GPIO(0) )
            {
                i = u8x8_GetPinValue(u8x8, msg);
                if ( i != U8X8_PIN_NONE )
                {
                    if ( u8x8_GetPinIndex(u8x8, msg) < U8X8_PIN_OUTPUT_CNT )
                    {
                        u8g2_port_pin_write(i, arg_int);
                    }
                    else
                    {
                        if ( u8x8_GetPinIndex(u8x8, msg) == U8X8_PIN_OUTPUT_CNT )
                        {
                            // call yield() for the first pin only, u8x8 will always request all the pins, so this should be ok
                            // yield();
                        }
                        u8x8_SetGPIOResult(u8x8, u8g2_port_pin_read(i) == 0 ? 0 : 1);
                    }
                }
                break;
            }
            return 0;
    }
    return 1;
}

#if defined U8G2_USE_HW_I2C
uint8_t u8x8_byte_rtthread_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    struct rt_i2c_msg msgs;
    static uint8_t buffer[32];
    static uint8_t buf_idx;
    uint8_t *data;

    rt_uint8_t t = 0;
    switch(msg)
    {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;
            while( arg_int > 0 )
            {
                buffer[buf_idx++] = *data;
                data++;
                arg_int--;
            }
            break;

        case U8X8_MSG_BYTE_INIT:
            i2c_bus = rt_i2c_bus_device_find(U8G2_I2C_DEVICE_NAME);
            if (i2c_bus == RT_NULL)
            {
                rt_kprintf("[u8g2] Failed to find bus %s\n", U8G2_I2C_DEVICE_NAME);
                return 0;
            }
            break;

        case U8X8_MSG_BYTE_SET_DC:
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            buf_idx = 0;
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            if (i2c_bus == RT_NULL)
            {
                rt_kprintf("[u8g2] Failed to find bus %s\n", U8G2_I2C_DEVICE_NAME);
                return 0;
            }
            // I2C Data Transfer
            msgs.addr  = u8x8_GetI2CAddress(u8x8)>>1;
            msgs.flags = RT_I2C_WR;
            msgs.buf   = buffer;
            msgs.len   = buf_idx;
            while(rt_i2c_transfer(i2c_bus, &msgs, 1) != 1 && t < MAX_RETRY)
            {
                t++;
            };
            if(t >= MAX_RETRY)
            {
                return 0;
            }
            break;

        default:
            return 0;
    }
    return 1;
}
#endif /* U8G2_USE_HW_I2C */

#if defined U8G2_USE_HW_SPI
uint8_t u8x8_byte_rtthread_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    struct rt_spi_message spi_msg;
    switch(msg) 
    {
        case U8X8_MSG_BYTE_SEND:
            spi_msg.send_buf   = arg_ptr;
            spi_msg.recv_buf   = RT_NULL;
            spi_msg.length     = arg_int;
            spi_msg.cs_take    = 0;
            spi_msg.cs_release = 0;
            spi_msg.next       = RT_NULL;
            rt_spi_transfer_message(&u8g2_spi_dev, &spi_msg);
            break;

        case U8X8_MSG_BYTE_INIT:
            /* SPI mode has to be mapped to the mode of the current controller, at least Uno, Due, 101 have different SPI_MODEx values */
            /*   0: clock active high, data out on falling edge, clock default value is zero, takover on rising edge */
            /*   1: clock active high, data out on rising edge, clock default value is zero, takover on falling edge */
            /*   2: clock active low, data out on rising edge */
            /*   3: clock active low, data out on falling edge */
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
            rt_hw_spi_config(u8x8->display_info->spi_mode, u8x8->display_info->sck_clock_hz, u8x8->pins[U8X8_PIN_CS]);
            break;

        case U8X8_MSG_BYTE_SET_DC:
            u8x8_gpio_SetDC(u8x8, arg_int);
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);  
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
            break;

        default:
            return 0;
    }
    return 1;
}
#endif /* U8G2_USE_HW_SPI */
