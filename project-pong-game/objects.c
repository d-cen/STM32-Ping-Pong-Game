#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include "objects.h"
#include "init.h"
#include "display.h"
#include "assert.h"


Object *object_init(uint16_t object_x, uint16_t object_y, uint16_t thick_x, uint16_t thick_y, uint16_t color)
{
    assert(object_x >= 0);
    assert(object_x + thick_x < X_AVA_MAX);
    assert(object_y >= 0);
    assert(object_y + thick_y < Y_AVA_MAX);

    Object *object = malloc(sizeof(object));
    object->object_x = object_x;
    object->object_y = object_y;
    object->color = color;
    object->thick_x = thick_x;
    object->thick_y = thick_y;

    return object;
}

void show_object(Object *object) {
    if ((object->object_x + object->thick_x > 240 - 1) || (object->object_y + object->thick_y > 320 - 1))
    {
        return;
    }
    // Set column range.
    spi_cmd(SPI1, 0x2A);
    spi_write16(SPI1, (uint16_t) min(object->object_x, 240 - object->thick_x ));
    spi_write16(SPI1, (uint16_t) min(object->object_x + object->thick_x - 1, 240 - 1) );

    // Set row range.
    spi_cmd(SPI1, 0x2B);
    spi_write16(SPI1, (uint16_t) min(object->object_y, 320 - object->thick_y)  );
    spi_write16(SPI1, (uint16_t) min(object->object_y + object->thick_y - 1, 320  - 1) );

    // Set 'write to RAM'
    spi_cmd(SPI1, 0x2C);
    uint16_t i = 0;
    for (i = 0; i < (object->thick_x * object->thick_y); i++)
    {
        spi_write16(SPI1, object->color);
    }
}
