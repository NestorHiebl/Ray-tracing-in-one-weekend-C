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

/**
 * @brief Check whether a given ray intersects a sphere with given center and radius.
 * 
 * @param center The center of the sphere in question.
 * @param radius The radius of the sphere in question.
 * @param r The ray to check with.
 * @return Returns -1.0 if the ray does not intersect the given sphere. Otherwise returns the factor with
 * which the ray direction is to be multiplied in order to get the intersection point.
 */
double hit_sphere(point3_t center, double radius, ray_t r) {
    // A vector from the sphere center to the origin, or (A - C)
    vec3_t oc = vec3_sub(r.origin, center);

    double a = vec3_len_squared(r.direction); 

    double half_b = vec3_dot(oc, r.direction);

    double c = vec3_len_squared(oc) - (radius * radius);
 
    double discriminant = (half_b * half_b) - (a * c);

    if (discriminant < 0) {
        return -1.0;
    } else {
        // Fully compute the quadratic formula
        return ((-half_b - sqrt(discriminant)) / a);
    } 
}

color_t ray_color(ray_t r) {
    double t = hit_sphere((point3_t) {0, 0, -1}, 0.5, r);
    if (t > 0.0) {
        // Subtracting the sphere's center from a point on its surface yields a non-normalized surface normal
        vec3_t N = vec3_sub(ray_at(r, t), (vec3_t) { 0.0, 0.0, -1.0});
        
        // Normalize it
        N = vec3_unit_vec(N);
        return scale_color((color_t) { .r = N.x + 1.0, .g = N.y + 1.0, .b = N.z + 1.0}, 0.5);
    }

    vec3_t unit_direction = vec3_unit_vec(r.direction);
    t = 0.5 * (unit_direction.y + 1.0);
    return add_color(scale_color((color_t) {1.0, 1.0, 1.0}, (1.0 - t)), scale_color((color_t) {0.5, 0.7, 1.0}, t));
}
