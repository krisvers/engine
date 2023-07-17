#include <containers/texture.h>
#include <containers/dynarray.h>
#include <core/mem.h>

texture_t * texture_create(u32 width, u32 height, u64 bytesize, u8 * buffer) {
	texture_t * texture = kmalloc(sizeof(texture_t), MEMORY_TAG_TEXTURE);
	texture->width = width;
	texture->height = height;
	texture->bytesize = bytesize;
	texture->buffer = kmalloc(bytesize, MEMORY_TAG_TEXTURE);
	kmemcpy(texture->buffer, buffer, bytesize);

	return texture;
}

void texture_destroy(texture_t * texture) {
	if (texture->buffer != NULL) {
		kfree(texture->buffer, texture->bytesize, MEMORY_TAG_TEXTURE);
	}

	kfree(texture, sizeof(texture_t), MEMORY_TAG_TEXTURE);
}