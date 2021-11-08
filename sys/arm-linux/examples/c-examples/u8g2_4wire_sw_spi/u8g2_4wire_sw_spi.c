#include "u8g2port.h"

// GPIO chip number for character device
#define GPIO_CHIP_NUM 0

#define OLED_SPI_PIN_MOSI           13
#define OLED_SPI_PIN_SCK            26

// Use a different pin here rather than default SPI pins, which may cause issue
#define OLED_SPI_PIN_RES            25
#define OLED_SPI_PIN_DC             24
#define OLED_SPI_PIN_CS             23

int main(void)
{
    u8g2_t u8g2;

    // Initialization
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_arm_linux_gpio_and_delay);
    // Bus arg doesn't matter for software SPI
    init_user_data(&u8g2, GPIO_CHIP_NUM, 0);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_RESET, OLED_SPI_PIN_RES);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_DC, OLED_SPI_PIN_DC);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_SPI_DATA, OLED_SPI_PIN_MOSI);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_SPI_CLOCK, OLED_SPI_PIN_SCK);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_CS, OLED_SPI_PIN_CS);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    // Draw    
    /* full buffer example, setup procedure ends in _f */
    u8g2_ClearBuffer(&u8g2);

    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&u8g2, 1, 18, "U8g2 SW SPI");

    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603 );

    u8g2_SendBuffer(&u8g2);

    printf("Initialized ...\n");
    sleep_ms(5000);
    u8g2_SetPowerSave(&u8g2, 1);
    // Close and deallocate GPIO resources
    done_user_data(&u8g2);
    printf("Done\n");


    return 0;
}
