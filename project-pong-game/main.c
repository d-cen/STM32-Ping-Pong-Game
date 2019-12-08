#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "display.h"
#include "objects.h"


int main(void)
{
    // Initialize SPI for LCD screen
    spi_pins_init();

    // Initialize GPIO for push button
    enable_buttons();

    // Start the game
    while (1)
    {
        // Set the LCD screen
        set_lcd(ILI9341_MAGENTA);

        // User choose difficulty (3 mode)
        int mode = select_mode();

        // Constant collision check
        start_game(mode);

        // End game
        set_lcd(ILI9341_RED);
        set_restart();
        delay_cycles(20000);
        nano_wait(10000000);
    }

    return 0;
}
