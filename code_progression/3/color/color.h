#ifndef COLOR
#define COLOR

#include <stdio.h>

typedef struct {
    double r;
    double g;
    double b;
} color_t;

void write_color(FILE* file, color_t pixel_color);

#endif
