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
