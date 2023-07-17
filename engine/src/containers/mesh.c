#include <containers/mesh.h>
#include <containers/vertex.h>
#include <containers/dynarray.h>
#include <core/mem.h>
#include <math/linmath.h>

mesh_t * mesh_create(void) {
	mesh_t * mesh = kmalloc(sizeof(mesh_t), MEMORY_TAG_MESH);
	mesh->vertices = dynarray_create(vertex_t);
	mesh->indices = dynarray_create(indice_t);

	return mesh;
}

void mesh_destroy(mesh_t * mesh) {
	dynarray_destroy(mesh->vertices);
	dynarray_destroy(mesh->indices);
	kfree(mesh, sizeof(mesh_t), MEMORY_TAG_MESH);
}

void mesh_push_vertex(mesh_t * mesh, vertex_t * vertex) {
	dynarray_push_from(mesh->vertices, vertex);
}

void mesh_push_vertex_value(mesh_t * mesh, vertex_t vertex) {
	dynarray_push(mesh->vertices, vertex);
}

void mesh_pop_vertex(mesh_t * mesh, vertex_t * out_vertex) {
	dynarray_pop(mesh->vertices, out_vertex);
}

void mesh_push_indices(mesh_t * mesh, indice_t * indices) {
	dynarray_push_from(mesh->indices, indices);
}

void mesh_push_indices_value(mesh_t * mesh, indice_t indices) {
	dynarray_push(mesh->indices, indices);
}

void mesh_pop_indices(mesh_t * mesh, indice_t * out_indices) {
	dynarray_pop(mesh->indices, out_indices);
}

void mesh_merge(mesh_t * mesha, mesh_t * meshb) {

}