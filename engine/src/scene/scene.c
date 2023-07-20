#include <scene/scene.h>
#include <core/mem.h>
#include <core/logger.h>
#include <file/tga.h>
#include <containers/mesh.h>
#include <containers/texture.h>
#include <containers/dynarray.h>

scene_t * scene_create(void) {
	scene_t * scene = kmalloc(sizeof(scene_t), MEMORY_TAG_SCENE);
	scene->atlas = texture_atlas_create();
	scene->mesh = mesh_create();

	return scene;
}

void scene_destroy(scene_t * scene) {
	texture_atlas_destroy(scene->atlas);
	mesh_destroy(scene->mesh);
	kfree(scene, sizeof(scene_t), MEMORY_TAG_SCENE);
}

void scene_push_mesh(scene_t * scene, mesh_t * mesh) {
	// TODO: fix tex ids
	dynarray_merge(scene->mesh->vertices, mesh->vertices, vertex_t);
	dynarray_merge(scene->mesh->indices, mesh->indices, indice_t);
}

void scene_push_texture(scene_t * scene, texture_t * texture) {
	// TODO: fix tex ids
	texture_atlas_stitch(scene->atlas, texture);
}

void scene_push_texture_atlas(scene_t * scene, texture_atlas_t * atlas) {
	// TODO: fix tex ids
	for (u64 i = 0; i < atlas->textures->length; ++i) {
		texture_atlas_stitch(scene->atlas, &((texture_t *) atlas->textures->array)[i]);
	}
}

void scene_push_mesh_with_texture(scene_t * scene, mesh_t * mesh, texture_t * texture) {
	scene_push_mesh(scene, mesh);
	scene_push_texture(scene, texture);
}

void scene_push_mesh_with_texture_atlas(scene_t * scene, mesh_t * mesh, texture_atlas_t * atlas) {
	scene_push_mesh(scene, mesh);
	scene_push_texture_atlas(scene, atlas);
}

#define SCENE_HEADER_SIZE 28

typedef struct sceneSerializableHeader {
	char sig[4];
	u64 textures;
	u64 vertices;
	u64 indices;
} scene_serializable_header_t;

typedef struct sceneSerializable {
	scene_serializable_header_t header;
} scene_serializable_t;

void scene_load(scene_t * scene, file_t * file) {

}

void scene_save(scene_t * scene, file_t * file) {
	scene_serializable_t serialize;
	serialize.header.sig[0] = 'K';	serialize.header.sig[1] = 'S';	serialize.header.sig[2] = 'C'; serialize.header.sig[3] = 'N';

	serialize.header.textures = scene->atlas->textures->length;
	serialize.header.vertices = scene->mesh->vertices->length;
	serialize.header.indices = scene->mesh->indices->length;

	file_resize(file, SCENE_HEADER_SIZE);

	kmemcpy(file->buffer, &serialize, SCENE_HEADER_SIZE);

	file_write(file);
}