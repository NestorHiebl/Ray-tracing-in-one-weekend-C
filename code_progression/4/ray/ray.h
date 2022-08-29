#ifndef RAY_H
#define RAY_H

#include "../vec3/vec3.h"
#include "../color/color.h"

typedef struct {
    point3_t origin;
    vec3_t direction;
} ray_t;

point3_t ray_at(ray_t r, double t);

color_t ray_color(ray_t r);

#endif
