#ifndef HITTABLE
#define HITTABLE

#include "./ray/ray.h"

typedef void* hittable_ptr;

typedef struct {
    point3_t p;
    vec3_t normal;
    double t;
} hit_record_t;

typedef struct {
    hittable_ptr ptr;
    size_t size;

    int (*hit) (hittable_ptr, ray_t, double, double, hit_record_t*);
} hittable_t;

#endif
