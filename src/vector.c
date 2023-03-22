#include <math.h>
#include "vector.h"

struct vec3 rotate_x(struct vec3 v, float angle) {
    struct vec3 rotated_vector = {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}

struct vec3 rotate_y(struct vec3 v, float angle) {
    struct vec3 rotated_vector = {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}

struct vec3 rotate_z(struct vec3 v, float angle) {
    struct vec3 rotated_vector = {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z
    };
    return rotated_vector;
}