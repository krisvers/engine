#include <core/file.h>
#include <core/mem.h>
#include <core/logger.h>
#include <platform/platform.h>

void file_open(file_t * file, char * filename, file_operation_enum op) {
	file->fd = platform_file_open(filename, op);
	file->op = op;
	file->buffer = NULL;
	file->length = 0;
}

void file_close(file_t * file) {
	if (file->buffer != NULL) {
		kfree(file->buffer, file->length, MEMORY_TAG_FILE);
	}
	platform_file_close(file->fd);
	file->fd = 0;
}

void file_length(file_t * file) {
	file->length = platform_file_length(file->fd);
}

void file_read(file_t * file) {
	if (file->buffer != NULL) {
		kfree(file->buffer, file->length, MEMORY_TAG_FILE);
	}
	file_length(file);
	file->buffer = kmalloc(file->length, MEMORY_TAG_FILE);
	platform_file_read(file->fd, file->length, file->buffer);
}

void file_write(file_t * file) {
	if (file->buffer == NULL) {
		KERROR("[file_write(file)]");
		KERROR("given file is invalid");
		return;
	}

	platform_file_write(file->fd, file->length, file->buffer);
}

void file_resize(file_t * file, u64 length) {
	if (file->buffer != NULL) {
		kfree(file->buffer, file->length, MEMORY_TAG_FILE);
	}
	file->length = length;
	file->buffer = kmalloc(file->length, MEMORY_TAG_FILE);
}

void file_empty(file_t * file) {
	if (file->buffer == NULL) {
		KWARN("[file_empty(file)]");
		KWARN("trying to empty an empty file");
		return;
	}
	
	kfree(file->buffer, file->length, MEMORY_TAG_FILE);
}