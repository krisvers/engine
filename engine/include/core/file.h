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
	u64 length;
} file_t;

void KAPI file_open(file_t * file, char * filename, file_operation_enum op);
void KAPI file_close(file_t * file);
void KAPI file_length(file_t * file);
void KAPI file_read(file_t * file);
void KAPI file_write(file_t * file);
void KAPI file_resize(file_t * file, u64 length);
void KAPI file_empty(file_t * file);

#endif