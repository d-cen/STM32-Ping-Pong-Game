#include "mario.h"

unsigned int counter =0;
int up = 0;

void setup_gpio()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~(0x3 << 16);
    GPIOC->MODER |= (0x1 << 16);
    GPIOC->OSPEEDR &= ~(0 << 16);
    GPIOC->OSPEEDR |= (0x3 << 16);
    GPIOC->OTYPER &= ~(0x1 << 8);
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->ARR = 72000000 / 10000 - 1;
    TIM2->PSC = 10;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] = 1<< TIM2_IRQn;
}

void TIM2_IRQHandler()
{
    TIM2->SR &= ~TIM_SR_UIF;
    counter++;
}


void TIM3_IRQHandler()
{
    if(up == 1)
    {
        GPIOC->ODR &= ~(0x1 << 8);
    }
    else
    {
        GPIOC->ODR &= ~(0x1 << 8);
        GPIOC->ODR |= (0x1 << 8);
    }
    if (up == 1)
    {
        up = 0;
    }
    else
    {
        up = 1;
    }
    TIM3->SR &= ~TIM_SR_UIF;
}

void delay(unsigned int ms)
{
    ms += counter;
    while(counter < ms);
}

int check_condition(void)
{
    if (GPIOC->IDR & GPIO_IDR_0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void playFreq(unsigned int freq)
{
    TIM3->PSC = 2000000 / freq - 1;
    TIM3->ARR = 72;
    TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;
    if(freq)
    {
        NVIC->ISER[0] = 1<< TIM3_IRQn;
    }
    else
    {
        NVIC->ISER[0] = 0<< TIM3_IRQn;
        up = 0;
        GPIOC->ODR &= ~(0x1 << 8);
        GPIOC->ODR |= (0x1 << 8);
    }
}

void mario(void)
{
    // Play music
    playFreq(NOTE_4MI);
    delay(WAIT*3);
    playFreq(0);
    delay(WAIT*1);
    if (check_condition() == 1)
    {
        return;
    }

    playFreq(NOTE_4MI);
    delay(WAIT*3);
    playFreq(0);
    delay(WAIT*5);
    if (check_condition() == 1)
    {
        return;
    }
    playFreq(NOTE_4MI);
    delay(WAIT*7);
    playFreq(0);
    delay(WAIT*5);
    if (check_condition() == 1)
    {
        return;
    }

    playFreq(NOTE_4DO);
    delay(WAIT*3);
    playFreq(0);
    delay(WAIT*1);
    if (check_condition() == 1)
    {
        return;
    }

    playFreq(NOTE_4MI);
    delay(WAIT*3);
    playFreq(0);
    delay(WAIT*1);
    if (check_condition() == 1)
    {
        return;
    }


    playFreq(NOTE_4SO);
    delay(WAIT*3);
    playFreq(0);
    delay(WAIT*13);
    if (check_condition() == 1)
    {
        return;
    }


    playFreq(NOTE_3SO);
    delay(WAIT*7);
    playFreq(0);
    delay(WAIT*9);
    if (check_condition() == 1)
    {
        return;
    }


    for (int i = 0; i < 2; i++)
    {
        playFreq(NOTE_4DO);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*9);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_3SO);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*9);
        if (check_condition() == 1)
        {
            break;
        }

        playFreq(NOTE_3MI);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*9);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_3LY);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*5);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_3SI);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*5);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_3SI);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*1);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_3LY);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*5);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_3SO);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*1);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_4MI);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*1);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_4SO);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*1);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_4LY);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*5);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_4FA);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*5);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_4SO);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*5);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_4MI);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*5);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_4DO);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*1);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_4RE);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*1);
        if (check_condition() == 1)
        {
            break;
        }


        playFreq(NOTE_3SI);
        delay(WAIT*3);
        playFreq(0);
        delay(WAIT*13);
        if (check_condition() == 1)
        {
            break;
        }
    }
}




