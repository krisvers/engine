#ifndef KENGINE_CONTAINERS_TEXTURE_H
#define KENGINE_CONTAINERS_TEXTURE_H

#include <defines.h>
#include <containers/dynarray.h>

typedef struct texture {
	u64 bytesize;
	u32 width;
	u32 height;
	u8 * buffer;
} texture_t;

typedef struct textureAtlas {
	u64 bytesize;
	// this buffer is an owned copy by the atlas in which a texture in textures points to the start of their texture
	u8 * buffer;
	// dynarray_t<texture_t>
	dynarray_t * textures;
} texture_atlas_t;

texture_t KAPI * texture_create(u32 width, u32 height, u64 bytesize, u8 * buffer);
void KAPI texture_destroy(texture_t * texture);

#endif