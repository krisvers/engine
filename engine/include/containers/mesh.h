#ifndef KENGINE_CONTAINERS_MESH_H
#define KENGINE_CONTAINERS_MESH_H

#include <containers/dynarray.h>
#include <containers/vertex.h>

typedef struct indice {
    u32 x;
    u32 y;
    u32 z;
} indice_t;

typedef struct mesh {
    // dynarray_t<vertex_t>
	dynarray_t * vertices;
    // dynarray_t<indice_t>
	dynarray_t * indices;
} mesh_t;

mesh_t KAPI * mesh_create(void);
void KAPI mesh_destroy(mesh_t * mesh);
void KAPI mesh_push_vertex(mesh_t * mesh, vertex_t * vertex);
void KAPI mesh_push_vertex_value(mesh_t * mesh, vertex_t vertex);
void KAPI mesh_pop_vertex(mesh_t * mesh, vertex_t * out_vertex);
void KAPI mesh_push_indices(mesh_t * mesh, indice_t * indices);
void KAPI mesh_push_indices_value(mesh_t * mesh, indice_t indices);
void KAPI mesh_pop_indices(mesh_t * mesh, indice_t * out_indices);

#endif