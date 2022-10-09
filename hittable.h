#ifndef HITTABLE
#define HITTABLE

#include "./ray/ray.h"

#include <stdbool.h>

typedef void* raw_hittable_data;

typedef struct {
    point3_t p;
    vec3_t normal;
    double t;

    bool front_face;
} hit_record_t;

typedef struct {
    raw_hittable_data ptr;
    size_t size;

    int (*hit) (raw_hittable_data, ray_t, double, double, hit_record_t*);
} hittable_t;

void hit_record_set_face_normal(hit_record_t *hit, ray_t r, vec3_t outward_normal) {
    if (hit == NULL) {
        return;
    }

    hit->front_face = vec3_dot(r.direction, outward_normal) < 0;
    hit->normal = hit->front_face ? outward_normal : vec3_scalar_mul(outward_normal, -1);    
}
#endif
