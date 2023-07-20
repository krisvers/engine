#ifndef KENGINE_SCENE_SCENE_H
#define KENGINE_SCENE_SCENE_H

#include <defines.h>
#include <core/file.h>
#include <containers/mesh.h>
#include <containers/texture.h>

typedef struct sceneOptions {
	b8 _unused;
} scene_options_t;

typedef struct scene {
	mesh_t * mesh;
	texture_atlas_t * atlas;
	scene_options_t options;
} scene_t;

scene_t KAPI * scene_create(void);
void KAPI scene_destroy(scene_t * scene);
void KAPI scene_push_mesh(scene_t * scene, mesh_t * mesh);
void KAPI scene_push_texture(scene_t * scene, texture_t * texture);
void KAPI scene_push_texture_atlas(scene_t * scene, texture_atlas_t * atlas);
void KAPI scene_push_mesh_with_texture(scene_t * scene, mesh_t * mesh, texture_t * texture);
void KAPI scene_push_mesh_with_texture_atlas(scene_t * scene, mesh_t * mesh, texture_atlas_t * atlas);
void KAPI scene_load(scene_t * scene, file_t * file);
void KAPI scene_save(scene_t * scene, file_t * file);

#endif