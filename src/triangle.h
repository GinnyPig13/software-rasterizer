#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

struct face
{
    int a;
    int b;
    int c;
};

struct triangle
{
    struct vec2 points[3];
};


#endif