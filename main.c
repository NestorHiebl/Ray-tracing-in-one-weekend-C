#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define IMG_WIDTH 256
#define IMG_HEIGHT 256

void print_usage() {
    printf( "Usage:\n\t"
            "raytracer [FILE]\n\t"
            "Where FILE is a filename ending with .ppm"
          );
}

int validate_filename(const char *filename) {
    size_t len = strlen(filename);
    if ((len > FILENAME_MAX) || (len < 5)) {
        return 0;
    }

    if (strcmp(&filename[len - 4 /* Length of .ppm extension */], ".ppm") == 0) {
        return 1;
    }

    return 0;
}

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
        for (int i = 0; i < IMG_WIDTH; i++) {
            double r = ((double) i / (IMG_WIDTH - 1));
            double g = ((double) j / (IMG_HEIGHT - 1));
            double b = 0.25;

            int ir = (int) (255.999 * r);
            int ig = (int) (255.999 * g);
            int ib = (int) (255.999 * b);

            fprintf(output_file, "%d %d %d\n", ir, ig, ib);
        }
    }

    fflush(output_file);
    fclose(output_file);
}