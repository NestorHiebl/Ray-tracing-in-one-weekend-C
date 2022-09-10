#ifndef SPHERE
#define SPHERE

#include "../hittable.h"

typedef struct {
    point3_t center;
    double radius;
} sphere_t;

int sphere_hit(hittable_ptr ptr, ray_t r, double t_min, double t_max, hit_record_t *rec);

hittable_t sphere_to_hittable(sphere_t *sphere);

#endif
