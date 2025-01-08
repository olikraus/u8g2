# DMA Performance Test with STM32L031

| Constructor | SysClk | Transfer | FPS | BSS (RAM) |
|---|---|---|---|---|
|    uc1609_slg19264_f | 2MHz |   SW SPI     |     0.3      |    1692 |
|   uc1609_slg19264_f  | 2MHz | HW SPI       |   4.7       |    1692 |
|    uc1609_slg19264_f  | 2MHz | DMA SPI    |      5.0    |     1948 |
|    uc1609_slg19264_1 | 2MHz |  SW SPI      |    0.3      |     348 |
|    uc1609_slg19264_1 | 2MHz |  HW SPI      |    2.6      |    348 |
|    uc1609_slg19264_1 | 2MHz |  DMA SPI     |     2.6      |  604 |

| Constructor | SysClk | Transfer | FPS | BSS (RAM) |
|---|---|---|---|---|
|    uc1609_slg19264_f | 32MHz |   SW SPI     |     6.0      |    1692 |
|   uc1609_slg19264_f | 32MHz  | HW SPI       |   73.8       |    1692 |
|    uc1609_slg19264_f | 32MHz  | DMA SPI    |      76.7    |     1948 |
|    uc1609_slg19264_1 | 32MHz |  SW SPI      |    5.6      |     348 |
|    uc1609_slg19264_1 | 32MHz |  HW SPI      |    39.6     |    348 |
|    uc1609_slg19264_1 | 32MHz |  DMA SPI     |     40.7      |  604 |

See also https://github.com/olikraus/u8g2/issues/2564

![https://raw.githubusercontent.com/olikraus/u8g2/refs/heads/master/sys/arm/stm32l031x4/u8g2_dma_spi_2mhz/stm32l031_schematic.jpg](https://raw.githubusercontent.com/olikraus/u8g2/refs/heads/master/sys/arm/stm32l031x4/u8g2_dma_spi_2mhz/stm32l031_schematic.jpg)

| GPIO | Display | Color |
|---|---|---|
|    PA14 | CD    |        brown |
|    PA13| CS      |      yellow |
|    PA7| MOSI     |      green |
|    PA6| Reset     |     white |
|    PA5| SCK         |   purple |

![https://raw.githubusercontent.com/olikraus/u8g2/refs/heads/master/sys/arm/stm32l031x4/u8g2_dma_spi_2mhz/stm32l031_wired.jpg](https://raw.githubusercontent.com/olikraus/u8g2/refs/heads/master/sys/arm/stm32l031x4/u8g2_dma_spi_2mhz/stm32l031_wired.jpg)

