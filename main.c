#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "utils.h"
#include "vec3/vec3.h"
#include "color/color.h"
#include "ray/ray.h"
#include "camera/camera.h"

#define ASPECT_RATIO (16.0 / 9.0)
#define IMG_WIDTH 1080
#define IMG_HEIGHT ((int) (IMG_WIDTH / ASPECT_RATIO))

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

    camera_t camera;

    camera.aspect_ratio = ASPECT_RATIO;
    camera.viewport_height = 2.0;
    camera.viewport_width = camera.aspect_ratio * camera.viewport_height;
    camera.focal_len = 1.0;

    camera.origin = (point3_t) { 0, 0, 0 };

    camera.horizontal = (vec3_t) { .x = camera.viewport_width, .y = 0, .z = 0 };
    camera.vertical = (vec3_t) { .x = 0, .y = camera.viewport_height, .z = 0 };
    camera.lower_left_corner = calculate_lower_left_corner(camera.origin, camera.horizontal, camera.vertical, camera.focal_len);

    fprintf(output_file, "P3\n%d %d\n255\n", IMG_WIDTH, IMG_HEIGHT);

    for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
        printf("\rScanlines remaining: %d ", j);
        for (int i = 0; i < IMG_WIDTH; i++) {
            double u = ((double) i / (IMG_WIDTH - 1));
            double v = ((double) j / (IMG_HEIGHT - 1));

            ray_t r = get_ray(camera, u, v);

            color_t pixel_color = ray_color(r);

            write_color(output_file, pixel_color);
        }
    }

    printf("\nDone.\n");
    fflush(output_file);
    fclose(output_file);
}
