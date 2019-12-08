#ifndef _DISPLAY_OBJ
#define _DISPLAY_OBJ

// Check smaller item
#define min(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b;      \
  })

typedef struct Object {
    uint16_t object_x, object_y, thick_x, thick_y, color;
} Object;

// Function Declaration
Object *object_init(uint16_t object_x, uint16_t object_y, uint16_t thick_x, uint16_t thick_y, uint16_t color);
void show_object(Object *object);

#endif
