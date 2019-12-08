#ifndef _INIT_ALL_H
#define _INIT_ALL_H

// GPIOB and GPIOA mapping pins for SPI
#define PB_MOSI 5
#define PB_SCK  3
#define PB_DC   4
#define PA_CS   12
#define PA_RST  15

// Function declaration
void delay_cycles(uint32_t);
void spi_pins_init(void);
void spi_init(SPI_TypeDef *SPIx);
void spi_write8(SPI_TypeDef *SPIx, uint8_t dat);
void spi_write16(SPI_TypeDef *SPIx, uint16_t dat);
void spi_cmd(SPI_TypeDef *SPIx, uint8_t cmd);
void ili9341_spi_init(SPI_TypeDef *SPIx);

#endif /* _INIT_ALL_H */
