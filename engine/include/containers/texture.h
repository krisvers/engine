#ifndef KENGINE_CONTAINERS_TEXTURE_H
#define KENGINE_CONTAINERS_TEXTURE_H

#include <defines.h>
#include <containers/dynarray.h>

typedef enum textureFormatEnum {
	TEXTURE_FORMAT_RGB,
	TEXTURE_FORMAT_BGR,
	TEXTURE_FORMAT_ARGB,
	TEXTURE_FORMAT_ABGR,
	TEXTURE_FORMAT_RGBA,
	TEXTURE_FORMAT_BGRA,
} texture_format_enum;

typedef struct texture {
	u64 bytesize;
	u32 width;
	u32 height;
	u8 * buffer;
	texture_format_enum format;
} texture_t;

typedef struct textureAtlas {
	u64 bytesize;
	// this buffer is an owned copy by the atlas in which a texture in textures points to the start of their texture
	u8 * buffer;
	// dynarray_t<texture_t>
	dynarray_t * textures;
} texture_atlas_t;

#define texture_atlas_at(atlas, i) (&((texture_t *) atlas->textures->array)[i])

texture_t KAPI * texture_create(u32 width, u32 height, u64 bytesize, u8 * buffer);
texture_t KAPI * texture_create_empty(void);
void KAPI texture_destroy(texture_t * texture);
texture_atlas_t KAPI * texture_atlas_create(void);
void KAPI texture_atlas_stitch(texture_atlas_t * atlas, texture_t * texture);
void KAPI texture_atlas_destroy(texture_atlas_t * atlas);

#endif