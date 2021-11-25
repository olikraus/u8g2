
#include "luat_base.h"

typedef struct luat_u8g2_conf
{
    size_t pinType; // I2C_SW = 1, I2C_HW = 2, SPI_SW_3PIN = 3, SPI_SW_4PIN = 4, SPI_HW_4PIN=5, P8080 = 6
    size_t w;
    size_t h;
    size_t pin0;
    size_t pin1;
    size_t pin2;
    size_t pin3;
    size_t pin4;
    size_t pin5;
    size_t pin6;
    size_t pin7;
    char* cname; // 控制器名称, 例如SSD1306
    void* ptr;
} luat_u8g2_conf_t;

int luat_u8g2_setup(luat_u8g2_conf_t *conf);

int luat_u8g2_close(luat_u8g2_conf_t *conf);

