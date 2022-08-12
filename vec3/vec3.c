#include "vec3.h"

vec3_t vec3_add(vec3_t v, vec3_t u) {
    vec3_t retval = {
        .x = v.x + u.x,
        .y = v.y + u.y,
        .z = v.z + u.z
        };

    return retval;
}

vec3_t vec3_sub(vec3_t v, vec3_t u) {
    vec3_t retval = {
        .x = v.x - u.x,
        .y = v.y - u.y,
        .z = v.z - u.z
        };

    return retval;
}

vec3_t vec3_scalar_mul(vec3_t v, double s) {
    vec3_t retval = {
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s
        };

    return retval;
}

vec3_t vec3_scalar_div(vec3_t v, double s) {
    vec3_t retval = {
        .x = v.x / s,
        .y = v.y / s,
        .z = v.z / s
        };

    return retval;
}

double vec3_dot(vec3_t v, vec3_t u) {
    return (v.x * u.x) + (v.y * u.y) + (v.z * u.z);
}

vec3_t vec3_cross(vec3_t v, vec3_t u) {
    vec3_t retval = {
        .x = (v.y * u.z) - (v.z * u.y),
        .y = (v.z * u.x) - (v.x * u.z),
        .z = (v.x * u.y) - (v.y * u.x)
    };

    return retval;
}

vec3_t vec3_unit_vec(vec3_t v) {
    return vec3_scalar_div(v, vec3_len(v));
}

double vec3_len(vec3_t v) {
    return sqrt(vec3_len_squared(v));
}

double vec3_len_squared(vec3_t v) {
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}