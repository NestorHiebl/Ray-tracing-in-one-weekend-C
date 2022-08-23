#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "utils.h"
#include "vec3/vec3.h"
#include "color/color.h"

#define IMG_WIDTH 256
#define IMG_HEIGHT 256

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid or no arguments supplied. See usage below:\n");
        print_usage();
        exit(1);
    }

    if (!validate_filename(argv[1])) {
        fprintf(stderr, "Invalid filename argument supplied. See usage below:\n");
        print_usage();
        exit(1);
    }

    FILE *output_file = fopen(argv[1], "w");

    if (output_file == NULL) {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        perror(NULL);
        exit(1);
    }

    fprintf(output_file, "P3\n%d %d\n255\n", IMG_WIDTH, IMG_HEIGHT);

    for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
        printf("\rScanlines remaining: %d ", j);
        for (int i = 0; i < IMG_WIDTH; i++) {
            color_t pixel_color = {
                .r = ((double) i / (IMG_WIDTH - 1)),
                .g = ((double) j / (IMG_HEIGHT - 1)),
                .b = 0.25
            };

            write_color(output_file, pixel_color);
        }
    }

    printf("\nDone.\n");
    fflush(output_file);
    fclose(output_file);
}
