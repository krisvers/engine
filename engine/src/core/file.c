#include <defines.h>
#include <core/file.h>
#include <core/mem.h>
#include <core/string.h>
#include <core/logger.h>
#include <platform/platform.h>

b8 file_open(file_t * file, char * filename, file_operation_enum op) {
	if (platform_file_open(&file->fd, filename, op) != 0) {
		KERROR("[file_open(file, filename, op)]");
		KERROR("failed to open file %s", filename);
		return FALSE;
	}
	file->op = op;
	file->buffer = (u8 *) NULL;
	file->path = kstrdup(filename);
	file->length = 0;
	file->stringified = FALSE;
	if (!file_last_mod(file)) {
		KERROR("[file_open(file, filename, op)]");
		KERROR("failed to save file %s mod status", filename);
		return FALSE;
	}
	file->modified = FALSE;

	return TRUE;
}

b8 file_close(file_t * file) {
	if (platform_file_close(file->fd) != 0) {
		KERROR("[file_close(file)]");
		KERROR("failed to close file %s", file->path);
		return FALSE;
	}

	if (file->buffer != NULL) {
		kfree(file->buffer, file->length, MEMORY_TAG_FILE);
	}

	if (file->path != NULL) {
		kfree(file->path, kstrlen(file->path) + 1, MEMORY_TAG_STRING);
	}

	kmemzero(&file->fd, sizeof(file_desc_t));

	return TRUE;
}

b8 file_length(file_t * file) {
	if (platform_file_length(&file->length, file->fd) != 0) {
		KERROR("[file_length(file)]");
		KERROR("failed to retrieve length of file %s", file->path);
		return FALSE;
	}

	return TRUE;
}

b8 file_read(file_t * file) {
	file->stringified = FALSE;
	u64 len = file->length;
	if (!file_length(file)) {
		KERROR("[file_read(file)]");
		KERROR("failed to get file length of %s", file->path);
		return FALSE;
	}
	if (file->buffer != NULL) {
		kfree(file->buffer, file->length, MEMORY_TAG_FILE);
	}
	file->buffer = (u8 *) kmalloc(file->length, MEMORY_TAG_FILE);
	if (platform_file_read(file->fd, file->length, file->buffer) != 0) {
		KERROR("[file_read(file)]");
		KERROR("failed to read from file %s", file->path);
		return FALSE;
	}

	return TRUE;
}

b8 file_write(file_t * file) {
	if (file->buffer == NULL) {
		KERROR("[file_write(file)]");
		KERROR("buffer is empty for file %s", file->path);
		return FALSE;
	}

	if (file->stringified) {
		if (platform_file_write(file->fd, file->length - 1, file->buffer) != 0) {
			KERROR("[file_write(file)]");
			KERROR("failed to write to file %s", file->path);
			return FALSE;
		}
		return TRUE;
	}
	if (platform_file_write(file->fd, file->length, file->buffer) != 0) {
		KERROR("[file_write(file)]");
		KERROR("failed to write to file %s", file->path);
		return FALSE;
	}

	return TRUE;
}

b8 file_resize(file_t * file, u64 length) {
	if (file->buffer != NULL) {
		kfree(file->buffer, file->length, MEMORY_TAG_FILE);
	}
	file->length = length;
	file->buffer = (u8 *) kmalloc(file->length, MEMORY_TAG_FILE);

	return TRUE;
}

b8 file_empty(file_t * file) {
	if (file->buffer == NULL) {
		KWARN("[file_empty(file)]");
		KWARN("trying to empty an empty file");
		return FALSE;
	}
	
	kfree(file->buffer, file->length, MEMORY_TAG_FILE);

	return TRUE;
}

b8 file_last_mod(file_t * file) {
	f64 mod;
	if (platform_file_last_modification(&mod, file->fd, file->path) != 0) {
		KERROR("[file_last_mod(file)]");
		KERROR("failed to get last modification of file %s", file->path);
		return FALSE;
	}
	if (mod != file->last_mod) {
		file->modified = TRUE;
		file->last_mod = mod;
	}

	return TRUE;
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

b8 file_destringify(file_t * file) {
	if (!file->stringified) {
		KWARN("[file_destringify(file)]");
		KWARN("file has already been destringified");
	}

	if (file->length <= 1) {
		KWARN("[file_destringify(file)]");
		KWARN("file cannot be destringified");
		return FALSE;
	}
	
	file->stringified = FALSE;

	u8 * buf = file->buffer;
	file->buffer = kmalloc(file->length - 1, MEMORY_TAG_FILE);
	kmemcpy(file->buffer, buf, file->length - 1);

	kfree(buf, file->length, MEMORY_TAG_FILE);

	return TRUE;
}