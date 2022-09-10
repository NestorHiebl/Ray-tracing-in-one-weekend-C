#include "sphere.h"
#include <math.h>


/**
 * @brief Check whether a given ray intersects a sphere with given center and radius.
 * 
 * @param center The center of the sphere in question.
 * @param radius The radius of the sphere in question.
 * @param r The ray to check with.
 * @return Returns -1.0 if the ray does not intersect the given sphere. Otherwise returns the factor with
 * which the ray direction is to be multiplied in order to get the intersection point.
 */
int sphere_hit(hittable_ptr ptr, ray_t r, double t_min, double t_max, hit_record_t *rec) {
    if ((ptr == NULL) || (rec == NULL)) {
        return -1;
    }

    sphere_t *sphere_ptr = (sphere_t*) ptr;

    // A vector from the sphere center to the origin, or (A - C)
    vec3_t oc = vec3_sub(r.origin, sphere_ptr->center);

    double a = vec3_len_squared(r.direction); 

    double half_b = vec3_dot(oc, r.direction);

    double c = vec3_len_squared(oc) - (sphere_ptr->radius * sphere_ptr->radius);
 
    double discriminant = (half_b * half_b) - (a * c);

    if (discriminant < 0) {
        return 0;
    }

    double sqrtd = sqrt(discriminant);

    // Find the nearest root in the acceptable range
    double root = (-half_b - sqrtd) / a;

    if ((root < t_min) || (t_max < root)) {
        root = (-half_b + sqrtd) / a;
     
        if ((root < t_min) || (t_max < root)) {
            return 0;
        }
    }

    rec->t = root;
    rec->p = ray_at(r, root);
    rec->normal = vec3_scalar_div(vec3_sub(rec->p, sphere_ptr->center), sphere_ptr->radius);

    return 1;
}

hittable_t sphere_to_hittable(sphere_t *sphere) {
    if (sphere == NULL) {
        return (hittable_t) { .ptr = NULL, .size = 0, .hit = NULL };
    }

    return (hittable_t) {
        .ptr = sphere,
        .size = sizeof(sphere_t),
        .hit = &sphere_hit
    };
}
