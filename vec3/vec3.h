#ifndef VEC3
#define VEC3

typedef struct {
    double x;
    double y;
    double z;
} vec3_t;

vec3_t vec3_add(vec3_t v, vec3_t u);
vec3_t vec3_sub(vec3_t v, vec3_t u);
vec3_t vec3_scalar_mul(vec3_t v, double s);
vec3_t vec3_scalar_div(vec3_t v, double s);

double vec3_dot(vec3_t v, vec3_t u);
vec3_t vec3_cross(vec3_t v, vec3_t u);
vec3_t vec3_unit_vec(vec3_t v);

double vec3_len(vec3_t v);
double vec3_len_squared(vec3_t v);
#endif
