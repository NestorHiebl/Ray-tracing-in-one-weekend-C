#include "ray.h"
#include "../vec3/vec3.h"
#include <math.h>

point3_t ray_at(ray_t r, double t) {
    vec3_t tb = vec3_scalar_mul(r.direction, t);

    point3_t retval = {
        .x = r.origin.x + tb.x,
        .y = r.origin.y + tb.y,
        .z = r.origin.z + tb.z,
    };

    return retval;
}

double hit_sphere(point3_t center, double radius, ray_t r) {
    // A vector from the sphere center to the origin, or (A - C)
    vec3_t oc = vec3_sub(r.origin, center);

    // b * b
    double a = vec3_dot(r.direction, r.direction);

    // 2b * (A - C)
    double b = 2.0 * vec3_dot(oc, r.direction);

    // (A - C) * (A - C) - (r * r)
    double c = vec3_dot(oc, oc) - (radius * radius);

    // b^2 - 4ac
    double discriminant = (b * b) - (4 * a * c);

    if (discriminant < 0) {
        return -1.0;
    } else {
        return ((-b - sqrt(discriminant)) / (2.0 * a));
    } 
}

color_t ray_color(ray_t r) {
    double t = hit_sphere((point3_t) {0, 0, -1}, 0.5, r);
    if (t > 0.0) {
        vec3_t N = vec3_unit_vec(vec3_sub(ray_at(r, t), (vec3_t) { 0.0, 0.0, -1.0}));
        return scale_color((color_t) { .r = N.x + 1.0, .g = N.y + 1.0, .b = N.z + 1.0}, 0.5);
    }

    vec3_t unit_direction = vec3_unit_vec(r.direction);
    t = 0.5 * (unit_direction.y + 1.0);
    return add_color(scale_color((color_t) {1.0, 1.0, 1.0}, (1.0 - t)), scale_color((color_t) {0.5, 0.7, 1.0}, t));
}
