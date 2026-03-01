#include <stdio.h>
#include "pico/stdlib.h"
#include "u8g2.h"
#include "u8g2_pico_hal.h"

u8g2_t u8g2;

/* Static lifetime config */
static const u8g2_pico_config_t display_cfg = {
    .interface = U8G2_PICO_IF_HW_SPI,
    .hw_spi = {
        .spi       = spi0,
        .baudrate = 4000000,
        .pin_sck  = 2,
        .pin_mosi = 3,
        .pin_cs   = 5,
        .pin_dc   = 4,
        .pin_rst  = 11,
    },
};

int main(void)
{
    stdio_init_all();

    u8g2_pico_init(
        &u8g2,
        u8g2_Setup_st7305_168x384_f,
        &display_cfg
    );

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB24_tr);
    u8g2_DrawStr(&u8g2, 100, 100, "Hello World");
    u8g2_SendBuffer(&u8g2);
    sleep_ms(100);
    while (true) {
        sleep_ms(1000);
    }
}
