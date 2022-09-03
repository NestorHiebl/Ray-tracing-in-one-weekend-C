#ifndef CAMERA_H
#define CAMERA_H
#include "../vec3/vec3.h"
#include "../ray/ray.h"

typedef struct {
    double aspect_ratio;
    double viewport_height;
    double viewport_width;
    double focal_len;

    point3_t origin;
    vec3_t horizontal;
    vec3_t vertical;
    vec3_t lower_left_corner;
} camera_t;

vec3_t calculate_lower_left_corner(point3_t origin, vec3_t horizontal, vec3_t vertical, double focal_len);

ray_t get_ray(camera_t camera, double horizontal_comp, double vertical_comp);

#endif
