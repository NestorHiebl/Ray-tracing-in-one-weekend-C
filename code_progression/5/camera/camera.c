#include "camera.h"
#include "../vec3/vec3.h"
#include "../ray/ray.h"

vec3_t calculate_lower_left_corner(point3_t origin, vec3_t horizontal, vec3_t vertical, double focal_len) {
    vec3_t retval = origin;

    retval = vec3_sub(retval, vec3_scalar_div(horizontal, 2));
    retval = vec3_sub(retval, vec3_scalar_div(vertical, 2));
    retval = vec3_sub(retval, (vec3_t) {.x = 0, .y = 0, .z = focal_len});

    return retval;
}

ray_t get_ray(camera_t camera, double horizontal_comp, double vertical_comp) {
    ray_t retval = { .origin = camera.origin, .direction = {0} };

    retval.direction = vec3_add(
        vec3_scalar_mul(camera.horizontal, horizontal_comp), 
        vec3_scalar_mul(camera.vertical, vertical_comp)
    );

    retval.direction = vec3_add(retval.direction, camera.lower_left_corner);
    retval.direction = vec3_sub(retval.direction, camera.origin);

    return retval;
}
