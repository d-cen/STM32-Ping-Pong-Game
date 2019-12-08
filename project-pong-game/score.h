#ifndef _SCORING
#define _SCORING

// Function pointer
void (*cmd)(char b);
void (*data)(char b);
void (*display1)(const char *);
void (*display2)(const char *);

// Function declaration
void norm_spi_cmd(char b);
void norm_spi_data(char b);
void generic_lcd_startup(void);
void dma_init_lcd(void);
void show_score(int, int);
void dma_display1(const char *s);
void dma_display2(const char *s);

#endif
