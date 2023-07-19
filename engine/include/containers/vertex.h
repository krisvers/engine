#ifndef KENGINE_CONTAINERS_VERTEX_H
#define KENGINE_CONTAINERS_VERTEX_H

#include <defines.h>
#include <math/linmath.h>

#define VERTEX_POSITION_OFFSET (0)
#define VERTEX_POSITION_BYTE_SIZE (sizeof(vec3))
#define VERTEX_POSITION_SIZE (VERTEX_POSITION_BYTE_SIZE / sizeof(float))
#define VERTEX_COLOR_OFFSET (VERTEX_POSITION_OFFSET + VERTEX_POSITION_BYTE_SIZE)
#define VERTEX_COLOR_BYTE_SIZE (sizeof(vec3))
#define VERTEX_COLOR_SIZE (VERTEX_COLOR_BYTE_SIZE / sizeof(float))
#define VERTEX_UV_OFFSET (VERTEX_COLOR_OFFSET + VERTEX_COLOR_BYTE_SIZE)
#define VERTEX_UV_BYTE_SIZE (sizeof(vec3))
#define VERTEX_UV_SIZE (VERTEX_UV_BYTE_SIZE / sizeof(float))
#define VERTEX_TEX_OFFSET (VERTEX_UV_OFFSET + VERTEX_UV_BYTE_SIZE)
#define VERTEX_TEX_BYTE_SIZE (sizeof(u64))
#define VERTEX_TEX_SIZE (VERTEX_UV_BYTE_SIZE / sizeof(u8))

typedef struct vertex {
	vec3 position;
	vec3 color;
	vec2 uv;
	u64 tex;
} vertex_t;

vertex_t KAPI * vertex_create(vec3 pos, vec3 col, vec2 uv, u64 tex);
void KAPI vertex_destroy(vertex_t * vertex);

#endif