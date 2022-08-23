#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage();
int validate_filename(const char *filename);

void print_usage() {
    printf( "Usage:\n\t"
            "raytracer [FILE]\n\t"
            "Where FILE is a filename ending with .ppm\n"
          );
}

int validate_filename(const char *filename) {
    if (filename == NULL) {
        return 0;
    }

    size_t len = strlen(filename);
    if ((len > FILENAME_MAX) || (len < 5)) {
        return 0;
    }

    if (strcmp(&filename[len - 4 /* Length of .ppm extension */], ".ppm") == 0) {
        return 1;
    }

    return 0;
}