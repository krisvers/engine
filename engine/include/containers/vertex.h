#ifndef KENGINE_CONTAINERS_VERTEX_H
#define KENGINE_CONTAINERS_VERTEX_H

#include <defines.h>
#include <math/linmath.h>

#define VERTEX_POSITION_OFFSET ((const void *) 0)
#define VERTEX_POSITION_BYTE_SIZE (sizeof(vec3))
#define VERTEX_POSITION_SIZE (VERTEX_POSITION_BYTE_SIZE / sizeof(float))
#define VERTEX_COLOR_OFFSET (VERTEX_POSITION_OFFSET + VERTEX_POSITION_BYTE_SIZE)
#define VERTEX_COLOR_BYTE_SIZE (sizeof(vec3))
#define VERTEX_COLOR_SIZE (VERTEX_COLOR_BYTE_SIZE / sizeof(float))

typedef struct vertex {
	vec3 position;
	vec3 color;
} vertex_t;

vertex_t KAPI * vertex_create(vec3 pos, vec3 col);
void KAPI vertex_destroy(vertex_t * vertex);

#endif