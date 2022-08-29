#include "ray.h"
#include "../vec3/vec3.h"

point3_t ray_at(ray_t r, double t) {
    vec3_t tb = vec3_scalar_mul(r.direction, t);

    point3_t retval = {
        .x = r.origin.x + tb.x,
        .y = r.origin.y + tb.y,
        .z = r.origin.z + tb.z,
    };

    return retval;
}

color_t ray_color(ray_t r) {
    vec3_t unit_direction = vec3_unit_vec(r.direction);

    double t = 0.5 * (unit_direction.y + 1.0);

    return add_color(scale_color((color_t) {1.0, 1.0, 1.0}, (1.0 - t)), scale_color((color_t) {0.5, 0.7, 1.0}, t));
}
