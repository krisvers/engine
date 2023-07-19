#include <containers/vertex.h>
#include <core/mem.h>
#include <math/linmath.h>

vertex_t * vertex_create(vec3 pos, vec3 col, vec2 uv, u64 tex) {
    vertex_t * vert = (vertex_t *) kmalloc(sizeof(vertex_t), MEMORY_TAG_MESH);
    vert->position[0] = pos[0];
    vert->position[1] = pos[1];
    vert->position[2] = pos[2];
    vert->color[0] = col[0];
    vert->color[1] = col[1];
    vert->color[2] = col[2];
	vert->uv[0] = uv[0];
	vert->uv[1] = uv[1];
	vert->tex = tex;
    return vert;
}

void vertex_destroy(vertex_t * vertex) {
    kfree((void *) vertex, sizeof(vertex_t), MEMORY_TAG_MESH);
}