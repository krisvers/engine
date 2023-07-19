#include <file/tga.h>
#include <containers/texture.h>
#include <core/file.h>
#include <core/mem.h>
#include <core/logger.h>

tga_t * tga_create(texture_t * texture) {
	tga_t * tga = (tga_t *) kmalloc(sizeof(tga_t), MEMORY_TAG_TEXTURE);
	kmemcpy(&tga->texture, texture, sizeof(texture_t));

	tga->texture.buffer = (u8 *) kmalloc(texture->bytesize, MEMORY_TAG_TEXTURE);
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
	tga_t * tga = (tga_t *) kmalloc(sizeof(tga_t), MEMORY_TAG_TEXTURE);
	kmemzero(tga, sizeof(tga_t));

	return tga;
}

void tga_empty(tga_t * tga) {
	if (tga->texture.buffer != NULL) {
		kfree(tga->texture.buffer, tga->texture.bytesize, MEMORY_TAG_TEXTURE);
		tga->texture.buffer = (u8 *) NULL;
	}

	kmemzero(tga, sizeof(tga_t));
}

#define U8(buf, i) *(((u8 *) buf) + i)
#define U16(buf, i) *(((u8 *) buf) + i) | (*(((u8 *) buf) + i + 1) << 8)

void tga_load(tga_t * tga, file_t * file) {
	u8 * buf = file->buffer;
	tga->header.id_len = U8(buf, 0);
	tga->header.color_map_type = U8(buf, 1);
	tga->header.img_type = U8(buf, 2);
	tga->header.color_map_origin = U16(buf, 3);
	tga->header.color_map_length = U16(buf, 5);
	tga->header.color_map_depth = U8(buf, 7);
	tga->header.img_x_origin = U16(buf, 8);
	tga->header.img_y_origin = U16(buf, 10);
	tga->header.img_w = U16(buf, 12);
	tga->header.img_h = U16(buf, 14);
	tga->header.bpp = U8(buf, 16);
	tga->header.img_desc = U8(buf, 17);
	tga->texture.width = tga->header.img_w;
	tga->texture.height = tga->header.img_h;
	tga->texture.bytesize = (tga->header.img_w * tga->header.img_h * (tga->header.bpp / 8));
	tga->texture.buffer = (u8 *) kmalloc(tga->texture.bytesize, MEMORY_TAG_TEXTURE);
	tga->texture.format = TEXTURE_FORMAT_BGRA;
	krmemcpy(tga->texture.buffer, file->buffer + TGA_HEADER_SIZE, tga->header.img_w, tga->header.img_h * (tga->header.bpp / 8));
}

#define U8_SET(buf, i, val) { U8(buf, i) = val; }
#define U16_SET(buf, i, val) { *(((u8 *) buf) + i) = val & 0xFF; *(((u8 *) buf) + i + 1) = (val >> 8); }

void tga_save(tga_t * tga, file_t * file) {
	file_resize(file, tga->texture.bytesize + TGA_HEADER_SIZE);
	u8 * buf = file->buffer;
	U8_SET(buf, 0, tga->header.id_len);
	U8_SET(buf, 1, tga->header.color_map_type);
	U8_SET(buf, 2, tga->header.img_type);
	U16_SET(buf, 3, tga->header.color_map_origin);
	U16_SET(buf, 5, tga->header.color_map_length);
	U8_SET(buf, 7, tga->header.color_map_depth);
	U16_SET(buf, 8, tga->header.img_x_origin);
	U16_SET(buf, 10, tga->header.img_y_origin);
	U16_SET(buf, 12, tga->header.img_w);
	U16_SET(buf, 14, tga->header.img_h);
	U8_SET(buf, 16, tga->header.bpp);
	U8_SET(buf, 17, tga->header.img_desc);
	krmemcpy(file->buffer + TGA_HEADER_SIZE, tga->texture.buffer, tga->header.img_w, tga->header.img_h * (tga->header.bpp / 8));
}

void tga_destroy(tga_t * tga) {
	kfree(&tga->texture.buffer, tga->texture.bytesize, MEMORY_TAG_TEXTURE);
	kfree(tga, sizeof(tga_t), MEMORY_TAG_TEXTURE);
}