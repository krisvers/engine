#include <file/tga.h>
#include <containers/texture.h>
#include <core/file.h>
#include <core/mem.h>

tga_t * tga_create(texture_t * texture) {
	tga_t * tga = kmalloc(sizeof(tga_t), MEMORY_TAG_TEXTURE);
	kmemcpy(&tga->texture, texture, sizeof(texture_t));

	tga->texture.buffer = kmalloc(texture->bytesize, MEMORY_TAG_TEXTURE);
	kmemcpy(tga->texture.buffer, texture->buffer, texture->bytesize);

	kmemzero(&tga->header, TGA_HEADER_SIZE);
	tga->header.img_type = 2;
	tga->header.img_w = texture->width;
	tga->header.img_h = texture->height;
	tga->header.bpp = 32;
	tga->header.img_desc = 8;

	return tga;
}

tga_t * tga_create_empty(void) {
	tga_t * tga = kmalloc(sizeof(tga_t), MEMORY_TAG_TEXTURE);
	kmemzero(tga, sizeof(tga_t));

	return tga;
}

void tga_load(tga_t * tga, file_t * file) {
	kmemcpy(&tga->header, file->buffer, TGA_HEADER_SIZE);
}

void tga_destroy(tga_t * tga) {
	kfree(&tga->texture.buffer, tga->texture.bytesize, MEMORY_TAG_TEXTURE);
	kfree(tga, sizeof(tga_t), MEMORY_TAG_TEXTURE);
}