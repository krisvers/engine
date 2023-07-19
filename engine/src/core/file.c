#include <core/file.h>
#include <core/mem.h>
#include <core/string.h>
#include <core/logger.h>
#include <platform/platform.h>

void file_open(file_t * file, char * filename, file_operation_enum op) {
	file->fd = platform_file_open(filename, op);
	file->op = op;
	file->buffer = (u8 *) NULL;
	file->path = kstrdup(filename);
	file->length = 0;
	file->stringified = FALSE;
	file_last_mod(file);
	file->modified = FALSE;
}

void file_close(file_t * file) {
	if (file->buffer != NULL) {
		kfree(file->buffer, file->length, MEMORY_TAG_FILE);
	}
	if (file->path != NULL) {
		kfree(file->path, kstrlen(file->path) + 1, MEMORY_TAG_STRING);
	}
	platform_file_close(file->fd);
	file->fd = 0;
}

void file_length(file_t * file) {
	file->length = platform_file_length(file->fd);
}

void file_read(file_t * file) {
	file->stringified = FALSE;
	u64 len = file->length;
	file_length(file);
	if (file->buffer != NULL) {
		kfree(file->buffer, file->length, MEMORY_TAG_FILE);
	}
	file->buffer = (u8 *) kmalloc(file->length, MEMORY_TAG_FILE);
	platform_file_read(file->fd, file->length, file->buffer);
}

void file_write(file_t * file) {
	if (file->buffer == NULL) {
		KERROR("[file_write(file)]");
		KERROR("given file is invalid");
		return;
	}

	if (file->stringified) {
		platform_file_write(file->fd, file->length - 1, file->buffer);
		return;
	}
	platform_file_write(file->fd, file->length, file->buffer);
}

void file_resize(file_t * file, u64 length) {
	if (file->buffer != NULL) {
		kfree(file->buffer, file->length, MEMORY_TAG_FILE);
	}
	file->length = length;
	file->buffer = (u8 *) kmalloc(file->length, MEMORY_TAG_FILE);
}

void file_empty(file_t * file) {
	if (file->buffer == NULL) {
		KWARN("[file_empty(file)]");
		KWARN("trying to empty an empty file");
		return;
	}
	
	kfree(file->buffer, file->length, MEMORY_TAG_FILE);
}

void file_last_mod(file_t * file) {
	f64 mod = platform_file_last_modification(file->fd, file->path);
	if (mod != file->last_mod) {
		file->modified = TRUE;
		file->last_mod = mod;
	}
}

file_t * file_alloc(void) {
	file_t * file = kmalloc(sizeof(file_t), MEMORY_TAG_FILE);
	return file;
}

void file_dealloc(file_t * file) {
	if (file->buffer != NULL) {
		file_empty(file);
	}
	kfree(file, sizeof(file), MEMORY_TAG_FILE);
}

void file_stringify(file_t * file) {
	if (file->stringified) {
		KWARN("[file_stringify(file)]");
		KWARN("file has already been stringified");
	}

	file->stringified = TRUE;

	u8 * buf = file->buffer;
	file->buffer = kmalloc(file->length + 1, MEMORY_TAG_FILE);
	kmemcpy(file->buffer, buf, file->length);

	kfree(buf, file->length, MEMORY_TAG_FILE);
	file->buffer[file->length] = '\0';
}

void file_destringify(file_t * file) {
	if (!file->stringified) {
		KWARN("[file_destringify(file)]");
		KWARN("file has already been destringified");
	}

	if (file->length <= 1) {
		KWARN("[file_destringify(file)]");
		KWARN("file cannot be destringified");
		return;
	}
	
	file->stringified = FALSE;

	u8 * buf = file->buffer;
	file->buffer = kmalloc(file->length - 1, MEMORY_TAG_FILE);
	kmemcpy(file->buffer, buf, file->length - 1);

	kfree(buf, file->length, MEMORY_TAG_FILE);
}