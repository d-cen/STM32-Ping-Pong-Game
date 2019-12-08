#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "display.h"
#include "objects.h"
#include "mario.h"
#include "score.h"

// Paddle locations
int pad1_x, pad2_x, dx;

// Ball locations
int ball_x, ball_y, ball_dx, ball_dy;

// Score
int score1, score2;

// Push buttons status
int arr[4] = {0};

void set_lcd(uint32_t color)
{
    // Set column range
    spi_cmd(SPI1, 0x2A);
    spi_write16(SPI1, 0x0000);
    spi_write16(SPI1, (uint16_t)(COLUMN_NUM));

    // Set row range
    spi_cmd(SPI1, 0x2B);
    spi_write16(SPI1, (uint16_t)(0));
    spi_write16(SPI1, (uint16_t)(ROW_NUM));

    // Set 'write to RAM'
    spi_cmd(SPI1, 0x2C);
    uint16_t i = 0, j = 0;
    for (i = 0; i < COLUMN_NUM; i++)
    {
        for (j = 0; j < ROW_NUM; j++)
        {
          spi_write16(SPI1, color);
        }
    }
}

void enable_buttons(void)
{
    // Setup 4 buttons as input on PA0, PA2, PC0, PC2
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;
    GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER2);
    GPIOC->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER2);
}

void getButton(void)
{
    if (GPIOA->IDR & GPIO_IDR_0)
    {
        arr[0] = 1;
        arr[1] = 0;
        arr[2] = 0;
    }
    else if (GPIOA->IDR & GPIO_IDR_1)
    {
        arr[0] = 0;
        arr[1] = 1;
        arr[2] = 0;
    }
    else if (GPIOC->IDR & GPIO_IDR_0)
    {
        arr[0] = 0;
        arr[1] = 0;
        arr[2] = 1;
    }
    else if (GPIOC->IDR & GPIO_IDR_1)
    {
        arr[3] = 1;
    }
}

int select_mode(void)
{
    // Create while loop until level chosen
    int mode = 0;
    while (arr[3] == 0)
    {
        getButton();
        if (arr[0] == 1)
        {
            mode = 12000000;
            disp_bound(280, ILI9341_RED);
            disp_bound(180, ILI9341_WHITE);
            disp_bound(80, ILI9341_WHITE);
            disp_level(ILI9341_WHITE);
        }
        else if (arr[1] == 1)
        {
            mode = 7000000;
            disp_bound(280, ILI9341_WHITE);
            disp_bound(180, ILI9341_RED);
            disp_bound(80, ILI9341_WHITE);
            disp_level(ILI9341_WHITE);
        }
        else if (arr[2] == 1)
        {
            mode = 5000000;
            disp_bound(280, ILI9341_WHITE);
            disp_bound(180, ILI9341_WHITE);
            disp_bound(80, ILI9341_RED);
            disp_level(ILI9341_WHITE);
        }
        else
        {
            mode = 12000000;
            disp_bound(280, ILI9341_WHITE);
            disp_bound(180, ILI9341_WHITE);
            disp_bound(80, ILI9341_WHITE);
            disp_level(ILI9341_WHITE);
        }
    }
    return mode;
}

void disp_bound(unsigned int x, uint16_t color)
{
    Object *tmp1 = object_init(50, x, 140, 5, color);
    show_object(tmp1);
    free(tmp1);
    int i, j;
    for (i = 1; i < 10; i++)
    {
        j = x - (5 * i);
        Object *tmp3 = object_init(50, j, 5, 5 ,color);
        show_object(tmp3);
        free(tmp3);
        Object *tmp4 = object_init(185, j, 5, 5 ,color);
        show_object(tmp4);
        free(tmp4);
    }
    Object *tmp2 = object_init(50, x - 50, 140, 5 ,color);
    show_object(tmp2);
    free(tmp2);
}

void disp_level(uint16_t color)
{
    print_easy(color);
    print_norm(color);
    print_hard(color);
}

void start_game(int mode)
{
    // Initialize paddle location
    pad1_x = 100;
    pad2_x = 100;
    dx = 2;

    // Initialize ball locations
    ball_x = 120;
    ball_y = 160;
    ball_dx = 1;
    ball_dy = -1;

    // Initialize score
    score1 = 3;
    score2 = 3;

    // Setup first paddle
    set_lcd(BACKGROUND);
    Object *pad1 = object_init(pad1_x, 1, 40, 5, PADCOL);
    show_object(pad1);
    free(pad1);

    // Setup second paddle
    Object *pad2 = object_init(pad2_x, 314, 40, 5, PADCOL);
    show_object(pad2);
    free(pad2);

    // Setup ball
    Object *ball = object_init(ball_x, ball_y, 5, 5, BALLCOL);
    show_object(ball);
    free(ball);

    nano_wait(120000000);

    // Paddle movement
    while (1)
    {
        arr[0] = 0;
        arr[1] = 0;
        arr[2] = 0;
        arr[3] = 0;
        getButton();
        if (arr[0] == 1 && pad1_x > 2)
        {
            Object *oldPad1 = object_init(pad1_x, 1, 40, 5, BACKGROUND);
            show_object(oldPad1);
            free(oldPad1);
            Object *newPad1 = object_init(pad1_x - dx, 1, 40, 5, PADCOL);
            show_object(newPad1);
            free(newPad1);
            pad1_x = pad1_x - dx;
        }
        else if (arr[1] == 1 && pad1_x < 198)
        {
            Object *oldPad1 = object_init(pad1_x, 1, 40, 5, BACKGROUND);
            show_object(oldPad1);
            free(oldPad1);
            Object *newPad1 = object_init(pad1_x + dx, 1, 40, 5, PADCOL);
            show_object(newPad1);
            free(newPad1);
            pad1_x = pad1_x + dx;
        }
        else if (arr[2] == 1 && pad2_x > 2)
        {
            Object *oldPad2 = object_init(pad2_x, 314, 40, 5, BACKGROUND);
            show_object(oldPad2);
            free(oldPad2);
            Object *newPad2 = object_init(pad2_x - dx, 314, 40, 5, PADCOL);
            show_object(newPad2);
            free(newPad2);
            pad2_x = pad2_x - dx;
        }
        else if (arr[3] == 1 && pad2_x < 198)
        {
            Object *oldPad2 = object_init(pad2_x, 314, 40, 5, BACKGROUND);
            show_object(oldPad2);
            free(oldPad2);
            Object *newPad2 = object_init(pad2_x + dx, 314, 40, 5, PADCOL);
            show_object(newPad2);
            free(newPad2);
            pad2_x = pad2_x + dx;
        }
        // Ball collision
        ball_collision();

        // Update ball position
        Object *ball = object_init(ball_x, ball_y, 5, 5, BALLCOL);
        show_object(ball);
        free(ball);
        nano_wait(mode);

        // Break loop when score reached
        if(score1 == 0 || score2 == 0)
        {
            break;
        }
    }
    delay_cycles(60000);
}

void ball_collision(void)
{
    // Ball mask
    Object *oldBall = object_init(ball_x, ball_y, 5, 5, BACKGROUND);
    show_object(oldBall);
    free(oldBall);

    // Update ball movement
    ball_x += ball_dx;
    ball_y += ball_dy;
    //pad3_dx = 1;

    // Check collision
    if ((ball_x >= pad1_x && ball_x <= (pad1_x + 40)) && ball_y <= 6)
    {
        ball_y = 6;
        ball_dy *= (-1);
    }
    else if ((ball_x >= pad2_x && ball_x <= (pad2_x + 40)) && ball_y >= 310 - 1)
    {
        ball_y = 310 - 1;
        ball_dy *= (-1);
    }

    // Score detection
    if (ball_y <= 0)
    {
        ball_x = 120;
        ball_y = 160;
        ball_dy *= (-1);
        score1--;
        Object *ball = object_init(ball_x, ball_y, 5, 5, BALLCOL);
        show_object(ball);
        free(ball);
        delay_cycles(7000000);
        show_score(score1, score2);
    }
    else if (ball_y >= 315)
    {
        ball_x = 120;
        ball_y = 160;;
        ball_dy *= (-1);
        score2--;
        Object *ball = object_init(ball_x, ball_y, 5, 5, BALLCOL);
        show_object(ball);
        free(ball);
        delay_cycles(7000000);
        show_score(score1, score2);
    }

    // Side detection
    if (ball_x <= 0)
    {
        ball_x = 0;
        ball_dx *= (-1);
    }
    else if (ball_x >= 235 - 1)
    {
        ball_x = 235 - 1;
        ball_dx *= (-1);
    }
}

void set_restart(void)
{
    // Close the display
    disp_bound(180, ILI9341_WHITE);
    print_again(PADCOL);

    // Play "Mario" music
    setup_gpio();
    mario();

    // Close sound ouput
    GPIOC->MODER &= ~(0x3 << 16);
}

void print_easy(uint16_t color)
{
    // Print "EASY"
    Object *tmp1 = object_init(171, 240, 5, 35, color);
    show_object(tmp1);
    free(tmp1);
    Object *tmp2 = object_init(151, 240, 20, 5, color);
    show_object(tmp2);
    free(tmp2);
    Object *tmp3 = object_init(151, 255, 20, 5, color);
    show_object(tmp3);
    free(tmp3);
    Object *tmp4 = object_init(151, 270, 20, 5, color);
    show_object(tmp4);
    free(tmp4);
    Object *tmp5 = object_init(141, 240, 5, 35, color);
    show_object(tmp5);
    free(tmp5);
    Object *tmp6 = object_init(121, 240, 5, 35, color);
    show_object(tmp6);
    free(tmp6);
    Object *tmp7 = object_init(121, 270, 20, 5, color);
    show_object(tmp7);
    free(tmp7);
    Object *tmp8 = object_init(121, 255, 20, 5, color);
    show_object(tmp8);
    free(tmp8);
    Object *tmp9 = object_init(111, 257, 5, 15, color);
    show_object(tmp9);
    free(tmp9);
    Object *tmp10 = object_init(91, 270, 20, 5, color);
    show_object(tmp10);
    free(tmp10);
    Object *tmp11 = object_init(94, 240, 20, 5, color);
    show_object(tmp11);
    free(tmp11);
    Object *tmp12 = object_init(96, 255, 15, 5, color);
    show_object(tmp12);
    free(tmp12);
    Object *tmp13 = object_init(91, 244, 5, 15, color);
    show_object(tmp13);
    free(tmp13);
    Object *tmp14 = object_init(81, 260, 5, 15, color);
    show_object(tmp14);
    free(tmp14);
    Object *tmp15 = object_init(61, 260, 5, 15, color);
    show_object(tmp15);
    free(tmp15);
    Object *tmp16 = object_init(71, 240, 5, 18, color);
    show_object(tmp16);
    free(tmp16);
    Object *tmp17 = object_init(64, 258, 18, 5, color);
    show_object(tmp17);
    free(tmp17);
}

void print_norm(uint16_t color)
{
    // Print "NORM"
    Object *tmp1 = object_init(171, 140, 5, 35, color);
    show_object(tmp1);
    free(tmp1);
    Object *tmp2 = object_init(151, 140, 5, 35, color);
    show_object(tmp2);
    free(tmp2);
    Object *tmp3 = object_init(151, 153, 13, 5, color);
    show_object(tmp3);
    free(tmp3);
    Object *tmp4 = object_init(162, 157, 13, 5, color);
    show_object(tmp4);
    free(tmp4);
    Object *tmp5 = object_init(141, 140, 5, 35, color);
    show_object(tmp5);
    free(tmp5);
    Object *tmp6 = object_init(121, 140, 20, 5, color);
    show_object(tmp6);
    free(tmp6);
    Object *tmp7 = object_init(121, 170, 20, 5, color);
    show_object(tmp7);
    free(tmp7);
    Object *tmp8 = object_init(121, 140, 5, 35, color);
    show_object(tmp8);
    free(tmp8);
    Object *tmp9 = object_init(111, 140, 5, 35, color);
    show_object(tmp9);
    free(tmp9);
    Object *tmp10 = object_init(91, 170, 20, 5, color);
    show_object(tmp10);
    free(tmp10);
    Object *tmp11 = object_init(91, 155, 20, 5, color);
    show_object(tmp11);
    free(tmp11);
    Object *tmp12 = object_init(91, 155, 5, 17, color);
    show_object(tmp12);
    free(tmp12);
    Object *tmp13 = object_init(95, 140, 5, 15, color);
    show_object(tmp13);
    free(tmp13);
    Object *tmp14 = object_init(81, 140, 5, 35, color);
    show_object(tmp14);
    free(tmp14);
    Object *tmp15 = object_init(61, 140, 5, 35, color);
    show_object(tmp15);
    free(tmp15);
    Object *tmp16 = object_init(71, 160, 5, 5, color);
    show_object(tmp16);
    free(tmp16);
    Object *tmp17 = object_init(61, 165, 20, 5, color);
    show_object(tmp17);
    free(tmp17);
}

void print_hard(uint16_t color)
{
    // Print "HARD"
    Object *tmp1 = object_init(171, 40, 5, 35, color);
    show_object(tmp1);
    free(tmp1);
    Object *tmp2 = object_init(151, 40, 5, 35, color);
    show_object(tmp2);
    free(tmp2);
    Object *tmp3 = object_init(151, 55, 20, 5, color);
    show_object(tmp3);
    free(tmp3);
    Object *tmp4 = object_init(141, 40, 5, 35, color);
    show_object(tmp4);
    free(tmp4);
    Object *tmp5 = object_init(121, 40, 5, 35, color);
    show_object(tmp5);
    free(tmp5);
    Object *tmp6 = object_init(121, 70, 20, 5, color);
    show_object(tmp6);
    free(tmp6);
    Object *tmp7 = object_init(121, 55, 20, 5, color);
    show_object(tmp7);
    free(tmp7);
    Object *tmp8 = object_init(111, 40, 5, 35, color);
    show_object(tmp8);
    free(tmp8);
    Object *tmp9 = object_init(91, 70, 20, 5, color);
    show_object(tmp9);
    free(tmp9);
    Object *tmp10 = object_init(91, 55, 20, 5, color);
    show_object(tmp10);
    free(tmp10);
    Object *tmp11 = object_init(91, 55, 5, 17, color);
    show_object(tmp11);
    free(tmp11);
    Object *tmp12 = object_init(95, 40, 5, 15, color);
    show_object(tmp12);
    free(tmp12);
    Object *tmp13 = object_init(81, 40, 5, 35, color);
    show_object(tmp13);
    free(tmp13);
    Object *tmp14 = object_init(61, 42, 5, 30, color);
    show_object(tmp14);
    free(tmp14);
    Object *tmp15 = object_init(64, 40, 20, 5, color);
    show_object(tmp15);
    free(tmp15);
    Object *tmp16 = object_init(64, 70, 20, 5, color);
    show_object(tmp16);
    free(tmp16);
}

void print_again(uint16_t color)
{
    // Print "AGAIN"
    Object *tmp1 = object_init(170, 140, 5, 35, color);
    show_object(tmp1);
    free(tmp1);
    Object *tmp2 = object_init(155, 140, 5, 35, color);
    show_object(tmp2);
    free(tmp2);
    Object *tmp3 = object_init(155, 170, 15, 5, color);
    show_object(tmp3);
    free(tmp3);
    Object *tmp4 = object_init(155, 160, 15, 5, color);
    show_object(tmp4);
    free(tmp4);
    Object *tmp5 = object_init(145, 140, 5, 35, color);
    show_object(tmp5);
    free(tmp5);
    Object *tmp6 = object_init(130, 140, 15, 5, color);
    show_object(tmp6);
    free(tmp6);
    Object *tmp7 = object_init(130, 170, 15, 5, color);
    show_object(tmp7);
    free(tmp7);
    Object *tmp8 = object_init(130, 140, 5, 15, color);
    show_object(tmp8);
    free(tmp8);
    Object *tmp9 = object_init(130, 155, 10, 5, color);
    show_object(tmp9);
    free(tmp9);
    Object *tmp10 = object_init(120, 140, 5, 35, color);
    show_object(tmp10);
    free(tmp10);
    Object *tmp11 = object_init(105, 140, 5, 35, color);
    show_object(tmp11);
    free(tmp11);
    Object *tmp12 = object_init(105, 170, 15, 5, color);
    show_object(tmp12);
    free(tmp12);
    Object *tmp13 = object_init(105, 160, 15, 5, color);
    show_object(tmp13);
    free(tmp13);
    Object *tmp14 = object_init(95, 140, 5, 35, color);
    show_object(tmp14);
    free(tmp14);
    Object *tmp15 = object_init(85, 140, 5, 35, color);
    show_object(tmp15);
    free(tmp15);
    Object *tmp16 = object_init(65, 140, 5, 35, color);
    show_object(tmp16);
    free(tmp16);
    Object *tmp17 = object_init(65, 153, 13, 5, color);
    show_object(tmp17);
    free(tmp17);
    Object *tmp18 = object_init(75, 157, 10, 5, color);
    show_object(tmp18);
    free(tmp18);
}

