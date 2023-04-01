#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define ARRAY_OF_MESH_VERTICES 8
extern struct vec3 mesh_vertices[ARRAY_OF_MESH_VERTICES];

#define N_MESH_FACES (6 * 2)
extern struct face mesh_faces[N_MESH_FACES];

#endif