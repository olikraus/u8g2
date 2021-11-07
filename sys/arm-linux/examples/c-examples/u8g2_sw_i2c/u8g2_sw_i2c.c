#include "u8g2port.h"

// GPIO chip number for character device
#define GPIO_CHIP_NUM 0
#define OLED_I2C_PIN_SCL                    20
#define OLED_I2C_PIN_SDA                    21

int main(void)
{
    u8g2_t u8g2;

    // Initialization
    u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_arm_linux_gpio_and_delay);
    u8g2_SetGPIOChip(&u8g2, GPIO_CHIP_NUM);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, OLED_I2C_PIN_SCL);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, OLED_I2C_PIN_SDA);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_RESET, U8X8_PIN_NONE);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    // Draw
    /* full buffer example, setup procedure ends in _f */
    u8g2_ClearBuffer(&u8g2);

    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&u8g2, 1, 18, "U8g2 SW I2C");

    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603 );

    u8g2_SendBuffer(&u8g2);

    printf("Initialized ...\n");
    sleep_ms(5000);
    u8g2_SetPowerSave(&u8g2, 1);
    // Close and deallocate GPIO resources
    done_pins();
    printf("Done\n");

    return 0;
}
