#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "score.h"
#include "display.h"

void (*cmd)(char b) = 0;
void (*data)(char b) = 0;
void (*display1)(const char *) = 0;
void (*display2)(const char *) = 0;

uint16_t dispmem[34] = {
        0x080 + 0,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x080 + 64,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
};

void norm_spi_cmd(char b)
{
    while((SPI2->SR & SPI_SR_TXE) == 0);
    SPI2->DR = b;
}

void norm_spi_data(char b)
{
    while((SPI2->SR & SPI_SR_TXE) == 0);
    SPI2->DR = 0x200 + b;
}


void generic_lcd_startup(void)
{
    nano_wait(100000000); // Give it 100ms to initialize
    cmd(0x38);  // 0011 NF00 N=1, F=0: two lines
    cmd(0x0c);  // 0000 1DCB: display on, no cursor, no blink
    cmd(0x01);  // clear entire display
    nano_wait(6200000); // clear takes 6.2ms to complete
    cmd(0x02);  // put the cursor in the home position
    cmd(0x06);  // 0000 01IS: set display to increment
}


void dma_init_lcd(void)
{
    // SPI initial for LCD
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(0xcf000000);
    GPIOB->MODER |= 0x8a000000;
    GPIOB->AFR[1] &= ~0xf0ff000;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    SPI2->CR1 &= ~SPI_CR1_SPE;
    SPI2->CR1 |= SPI_CR1_MSTR;
    SPI2->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2;
    SPI2->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;
    SPI2->CR2 |= SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_0;
    SPI2->CR2 &= ~SPI_CR2_DS_1 & ~SPI_CR2_DS_2;
    SPI2->CR1 |= SPI_CR1_SPE;
    generic_lcd_startup();

    // DMA SPI initialization
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel5->CCR &= ~DMA_CCR_EN;
    DMA1_Channel5->CMAR = (uint32_t) dispmem;
    DMA1_Channel5->CPAR = (uint32_t) &(SPI2->DR);
    DMA1_Channel5->CNDTR = 34;
    DMA1_Channel5->CCR |= DMA_CCR_DIR;
    DMA1_Channel5->CCR |= DMA_CCR_MSIZE_0;
    DMA1_Channel5->CCR |= DMA_CCR_PSIZE_0;
    DMA1_Channel5->CCR |= DMA_CCR_MINC;
    DMA1_Channel5->CCR |= DMA_CCR_CIRC;
    DMA1_Channel5->CCR &= ~DMA_CCR_PL;
    SPI2->CR1 &= ~SPI_CR1_SPE;
    SPI2->CR2 |= SPI_CR2_TXDMAEN;
    SPI2->CR1 |= SPI_CR1_SPE;
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}

void dma_display1(const char *s)
{
        int x;
        for(x=0; x<16; x+=1){
            if (s[x]){
                dispmem[x+1] =s[x] +  0x200;
            }
            else{
                break;
            }
        }
        for(   ; x<16; x+=1){
            dispmem[x+1] = 0x220;
        }
}

void dma_display2(const char *s)
{
        int x;
        for(x=0; x<16; x+=1){
            if (s[x]){
                dispmem[x+18] =s[x] +  0x200;
            }
            else{
                break;
            }
        }
        for(   ; x<16; x+=1){
            dispmem[x+18] = 0x220;
        }
}

void show_score(int score1, int score2)
{
    // Configure the function pointers
    cmd = norm_spi_cmd;
    data = norm_spi_data;
    display1 = dma_display1;
    display2 = dma_display2;

    // Initialize the display
    dma_init_lcd();

    // Write text
    const char *p13 = "                Player 2 score:0             ";
    const char *p12 = "                Player 2 score:1             ";
    const char *p11 = "                Player 2 score:2             ";
    const char *p10 = "                Player 2 win                 ";
    const char *p23 = "                Player 1 score:0             ";
    const char *p22 = "                Player 1 score:1             ";
    const char *p21 = "                Player 1 score:2             ";
    const char *p20 = "                Player 1 win                 ";

    int offset = 0;
    while(offset < 17)
    {
        if(score1 == 3)
        {
            display2(&p13[offset]);
        }
        if(score2 == 3)
        {
            display1(&p23[offset]);
        }
        if(score1 == 2)
        {
            display2(&p12[offset]);
        }
        if(score1 == 1)
        {
            display2(&p11[offset]);
        }
        if(score1 == 0)
        {
            display2(&p10[offset]);
        }
        if(score2 == 2)
        {
            display1(&p22[offset]);
        }
        if(score2 == 1)
        {
            display1(&p21[offset]);
        }
        if(score2 == 0)
        {
            display1(&p20[offset]);
        }
        nano_wait(120000000);
        offset += 1;
    }
}

