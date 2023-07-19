#include <containers/texture.h>
#include <containers/dynarray.h>
#include <core/mem.h>
#include <core/logger.h>
#include <core/assert.h>

texture_t * texture_create(u32 width, u32 height, u64 bytesize, u8 * buffer) {
	texture_t * texture = (texture_t *) kmalloc(sizeof(texture_t), MEMORY_TAG_TEXTURE);
	texture->width = width;
	texture->height = height;
	texture->bytesize = bytesize;
	texture->buffer = (u8 *) kmalloc(bytesize, MEMORY_TAG_TEXTURE);
	kmemcpy(texture->buffer, buffer, bytesize);

	return texture;
}

texture_t * texture_create_empty(void) {
	texture_t * texture = (texture_t *) kmalloc(sizeof(texture_t), MEMORY_TAG_TEXTURE);
	texture->width = 0;
	texture->height = 0;
	texture->bytesize = 0;
	texture->format = (texture_format_enum) 0;
	texture->buffer = (u8 *) NULL;

	return texture;
}

void texture_destroy(texture_t * texture) {
	if (texture->buffer != NULL) {
		kfree(texture->buffer, texture->bytesize, MEMORY_TAG_TEXTURE);
	}

	kfree(texture, sizeof(texture_t), MEMORY_TAG_TEXTURE);
}

texture_atlas_t * texture_atlas_create(void) {
	texture_atlas_t * atlas = (texture_atlas_t *) kmalloc(sizeof(texture_atlas_t), MEMORY_TAG_TEXTURE);
	atlas->bytesize = 0;
	atlas->buffer = (u8 *) NULL;
	atlas->textures = dynarray_create(texture_t);

	return atlas;
}

void texture_atlas_stitch(texture_atlas_t * atlas, texture_t * texture) {
	if (atlas->buffer != NULL) {
		u8 * buffer = atlas->buffer;
		atlas->buffer = (u8 *) kmalloc(atlas->bytesize + texture->bytesize, MEMORY_TAG_TEXTURE);
		kmemcpy(atlas->buffer, buffer, atlas->bytesize);
		kfree(buffer, texture->bytesize, MEMORY_TAG_TEXTURE);
	} else {
		atlas->buffer = (u8 *) kmalloc(atlas->bytesize + texture->bytesize, MEMORY_TAG_TEXTURE);
	}

	kmemcpy(atlas->buffer + atlas->bytesize, texture->buffer, texture->bytesize);
	texture_t tex;
	tex.bytesize = texture->bytesize;
	tex.width = texture->width;
	tex.height = texture->height;
	tex.buffer = atlas->buffer + atlas->bytesize;
	tex.format = TEXTURE_FORMAT_BGRA;
	dynarray_push(atlas->textures, tex, texture_t);
	atlas->bytesize += texture->bytesize;
}

void texture_atlas_destroy(texture_atlas_t * atlas) {
	KASSERT(atlas != NULL);
	dynarray_destroy(atlas->textures);
	kfree(atlas->buffer, atlas->bytesize, MEMORY_TAG_TEXTURE);
	kfree(atlas, sizeof(texture_atlas_t), MEMORY_TAG_TEXTURE);
}