#ifndef KENGINE_CORE_FILE_H
#define KENGINE_CORE_FILE_H

#include <defines.h>
#include <platform/platform.h>

typedef enum fileOperationEnum {
	FILE_READ,
	FILE_WRITE,
	FILE_READB,
	FILE_WRITEB,
	FILE_APPEND,
	FILE_APPENDB,
	FILE_AUTO,
} file_operation_enum;

typedef struct file {
	file_desc_t fd;
	file_operation_enum op;
	u8 * buffer;
	char * path;
	u64 length;
	f64 last_mod;
	b8 modified;
	b8 stringified;
} file_t;

b8 KAPI file_open(file_t * file, char * filename, file_operation_enum op);
b8 KAPI file_close(file_t * file);
b8 KAPI file_length(file_t * file);
b8 KAPI file_read(file_t * file);
b8 KAPI file_write(file_t * file);
b8 KAPI file_resize(file_t * file, u64 length);
b8 KAPI file_empty(file_t * file);
b8 KAPI file_last_mod(file_t * file);
void KAPI file_stringify(file_t * file);
b8 KAPI file_destringify(file_t * file);
file_t KAPI * file_alloc(void);
void KAPI file_dealloc(file_t * file);

#endif