#ifndef VECTOR_H
#define VECTOR_H

struct vec2
{
    float x;
    float y;
};

struct vec3
{
    float x;
    float y;
    float z;
};

struct vec3 rotate_x(struct vec3 v, float angle);
struct vec3 rotate_y(struct vec3 v, float angle);
struct vec3 rotate_z(struct vec3 v, float angle);

#endif