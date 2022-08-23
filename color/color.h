#ifndef COLOR
#define COLOR

#include <stdio.h>

typedef struct {
    double r;
    double g;
    double b;
} color_t;

void write_color(FILE* file, color_t pixel_color);

color_t scale_color(color_t c, double s);

color_t add_color(color_t c1, color_t c2);

#endif
