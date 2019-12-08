#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include "init.h"


void __attribute__((optimize("O0"))) delay_cycles(uint32_t cyc) {
  uint32_t d_i;
  for (d_i = 0; d_i < cyc; ++d_i) {
      asm("NOP");
  }
}

void spi_pins_init(void)
{
    // Enable the peripherals
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // MOSI and SCK to alternate function mode
    // DC to output mode
    GPIOB->MODER &= ~((0x3 << (PB_MOSI * 2)) | (0x3 << (PB_SCK * 2)) | (0x3 << (PB_DC * 2)));
    GPIOB->MODER |= ((0x2 << (PB_MOSI * 2)) | (0x2 << (PB_SCK * 2)) | (0x1 << (PB_DC * 2)));
    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL3 | GPIO_AFRL_AFRL5);

    // Set pull-down resistors
    GPIOB->PUPDR &= ~((0x3 << (PB_MOSI * 2) | 0x3 << (PB_SCK * 2)) | (0x3 << (PB_DC * 2)));
    GPIOB->PUPDR |= ((0x1 << (PB_MOSI * 2) | 0x1 << (PB_SCK * 2)));

    // Set output type to push-pull
    GPIOB->OTYPER &= ~((0x1 << PB_MOSI) | (0x1 << PB_SCK) | (0x1 << PB_DC));

    // Set high-speed; 50MHz maximum
    GPIOB->OSPEEDR |= ((0x3 << (PB_MOSI * 2)) | (0x3 << (PB_SCK * 2)) | (0x3 << (PB_DC * 2)));

    // Initialize GPIOA
    GPIOA->MODER &= ~((0x3 << (PA_CS * 2)) | (0x3 << (PA_RST * 2)));
    GPIOA->MODER |= ((0x1 << (PA_CS * 2)) | (0x1 << (PA_RST * 2)));
    GPIOA->OTYPER &= ~((0x1 << PA_CS) | (0x1 << PA_RST));
    GPIOA->PUPDR |= ((0x3 << (PA_CS * 2)) | (0x3 << (PA_RST * 2)));

    // Set initial ODR to '1' means ignore pin
    GPIOA->ODR |= (1 << PA_CS);
    GPIOA->ODR |= (1 << PB_DC);
    GPIOA->ODR |= (1 << PB_SCK);

    // Reset the display
    GPIOA->ODR &= ~(1 << PA_RST);
    delay_cycles(2000000);
    GPIOA->ODR |= (1 << PA_RST);
    delay_cycles(2000000);

    // Initial SPI
    spi_init(SPI1);

    // Pull CS to low
    GPIOA->ODR &= ~(1 << PA_CS);

    //Initialize display
    ili9341_spi_init(SPI1);
}

void spi_init(SPI_TypeDef *SPIx)
{
    // Reset the peripheral
    SPIx->CR1 &= ~(SPI_CR1_SPE);
    if (SPIx == SPI1)
    {
        RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
        RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
    }

    // Set clock polarity
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    SPI1->CR1 |= (SPI_CR1_CPOL | SPI_CR1_CPHA);

    // Set STM32 as host device
    SPI1->CR1 |= SPI_CR1_MSTR;

    // Set software CS pin and internal CS signal
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;

    // Enable peripheral
    SPI1->CR1 |= SPI_CR1_SPE;
}

void spi_write8(SPI_TypeDef *SPIx, uint8_t dat)
{
    // Wait 'Transmit Buffer Empty'
    while (!(SPIx->SR & SPI_SR_TXE)){
    };

    // Send data
    *(uint8_t*) & (SPIx->DR) = dat; // Cast to pointer of an 8-bit integer
}

void spi_write16(SPI_TypeDef *SPIx, uint16_t dat)
{
    // Wait 'Transmit Buffer Empty'
    while (!(SPIx->SR & SPI_SR_TXE)){
    };

    // Send data
    dat = (((dat & 0x00FF) << 8) | ((dat & 0xFF00) >> 8));
    *(uint16_t*) & (SPIx->DR) = dat; // Cast to pointer of an 16-bit integer
}

void spi_cmd(SPI_TypeDef *SPIx, uint8_t cmd)
{
    // Wait 'Busy' flag to clear
    while ((SPIx->SR & SPI_SR_BSY)){
    };

    // Pull D/C pin low during transmission
    GPIOB->ODR &= ~(1 << PB_DC);
    spi_write8(SPIx, cmd);

    // Pull D/C back high after transmission
    while ((SPIx->SR & SPI_SR_BSY)){
    };
    GPIOB->ODR |= (1 << PB_DC);
}

void ili9341_spi_init(SPI_TypeDef *SPIx)
{
    // Issue a series of initialization commands from the Adafruit library for a simple 'known good' test.
    spi_cmd(SPIx, 0xEF);
    spi_write8(SPIx, 0x03);
    spi_write8(SPIx, 0x80);
    spi_write8(SPIx, 0x02);
    spi_cmd(SPIx, 0xCF);
    spi_write8(SPIx, 0x00);
    spi_write8(SPIx, 0xC1);
    spi_write8(SPIx, 0x30);
    spi_cmd(SPIx, 0xED);
    spi_write8(SPIx, 0x64);
    spi_write8(SPIx, 0x03);
    spi_write8(SPIx, 0x12);
    spi_write8(SPIx, 0x81);
    spi_cmd(SPIx, 0xE8);
    spi_write8(SPIx, 0x85);
    spi_write8(SPIx, 0x00);
    spi_write8(SPIx, 0x78);
    spi_cmd(SPIx, 0xCB);
    spi_write8(SPIx, 0x39);
    spi_write8(SPIx, 0x2C);
    spi_write8(SPIx, 0x00);
    spi_write8(SPIx, 0x34);
    spi_write8(SPIx, 0x02);
    spi_cmd(SPIx, 0xF7);
    spi_write8(SPIx, 0x20);
    spi_cmd(SPIx, 0xEA);
    spi_write8(SPIx, 0x00);
    spi_write8(SPIx, 0x00);

    // PWCTR1
    spi_cmd(SPIx, 0xC0);
    spi_write8(SPIx, 0x23);

    // PWCTR2
    spi_cmd(SPIx, 0xC1);
    spi_write8(SPIx, 0x10);

    // VMCTR1
    spi_cmd(SPIx, 0xC5);
    spi_write8(SPIx, 0x3E);
    spi_write8(SPIx, 0x28);

    // VMCTR2
    spi_cmd(SPIx, 0xC7);
    spi_write8(SPIx, 0x86);

    // MADCTL
    spi_cmd(SPIx, 0x36);
    spi_write8(SPIx, 0x48);

    // VSCRSADD
    spi_cmd(SPIx, 0x37);
    spi_write8(SPIx, 0x00);

    // PIXFMT
    spi_cmd(SPIx, 0x3A);
    spi_write8(SPIx, 0x55);

    // FRMCTR1
    spi_cmd(SPIx, 0xB1);
    spi_write8(SPIx, 0x00);
    spi_write8(SPIx, 0x18);

    // DFUNCTR
    spi_cmd(SPIx, 0xB6);
    spi_write8(SPIx, 0x08);
    spi_write8(SPIx, 0x82);
    spi_write8(SPIx, 0x27);
    spi_cmd(SPIx, 0xF2);
    spi_write8(SPIx, 0x00);

    // GAMMASET
    spi_cmd(SPIx, 0x26);
    spi_write8(SPIx, 0x01);

    // (Actual gamma settings)
    spi_cmd(SPIx, 0xE0);
    spi_write8(SPIx, 0x0F);
    spi_write8(SPIx, 0x31);
    spi_write8(SPIx, 0x2B);
    spi_write8(SPIx, 0x0C);
    spi_write8(SPIx, 0x0E);
    spi_write8(SPIx, 0x08);
    spi_write8(SPIx, 0x4E);
    spi_write8(SPIx, 0xF1);
    spi_write8(SPIx, 0x37);
    spi_write8(SPIx, 0x07);
    spi_write8(SPIx, 0x10);
    spi_write8(SPIx, 0x03);
    spi_write8(SPIx, 0x0E);
    spi_write8(SPIx, 0x09);
    spi_write8(SPIx, 0x00);
    spi_cmd(SPIx, 0xE1);
    spi_write8(SPIx, 0x00);
    spi_write8(SPIx, 0x0E);
    spi_write8(SPIx, 0x14);
    spi_write8(SPIx, 0x03);
    spi_write8(SPIx, 0x11);
    spi_write8(SPIx, 0x07);
    spi_write8(SPIx, 0x31);
    spi_write8(SPIx, 0xC1);
    spi_write8(SPIx, 0x48);
    spi_write8(SPIx, 0x08);
    spi_write8(SPIx, 0x0F);
    spi_write8(SPIx, 0x0C);
    spi_write8(SPIx, 0x31);
    spi_write8(SPIx, 0x36);
    spi_write8(SPIx, 0x0F);

    // Exit sleep mode
    spi_cmd(SPIx, 0x11);
    delay_cycles(2000000);

    // Display on
    spi_cmd(SPIx, 0x29);
    delay_cycles(2000000);

    // 'Normal' display mode
    spi_cmd(SPIx, 0x13);
}








