#include "color.h"
#include <stdio.h>

void write_color(FILE* file, color_t pixel_color) {
    if (file == NULL) {
        fprintf(stderr, "Error: invalid file passed to \"write_color\"");
        return;
    }

    fprintf(
            file,
            "%d %d %d\n", 
            ((int) (255.999 * pixel_color.r)),
            ((int) (255.999 * pixel_color.g)),
            ((int) (255.999 * pixel_color.b))
            );
}

color_t scale_color(color_t c, double s) {
    color_t retval = {
        .r = c.r * s,
        .g = c.g * s,
        .b = c.b * s
    };

    return retval;
}

color_t add_color(color_t c1, color_t c2) {
    color_t retval = {
        .r = c1.r + c2.r,
        .g = c1.g + c2.g,
        .b = c1.b + c2.b,
    };

    return retval;
}
