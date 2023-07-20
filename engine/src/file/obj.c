#include <file/obj.h>
#include <core/file.h>
#include <core/mem.h>
#include <core/logger.h>

static int parse(char * string) {

}

void obj_parse(file_t * file, mesh_t * mesh) {
	// step one: split into cstring array for each new line
	char ** lines = kmalloc(file->length + 1, MEMORY_TAG_STRING);
	kmemcpy(lines, file->buffer, file->length);
	u64 line_num = 0;
	{
		for (u64 i = 0; i < file->length; ++i) {
			if (file->buffer[i] == '\n') {
				lines[line_num] = (char *) lines + (i + 1);
				file->buffer[i] = '\0';
				++line_num;
			}
		}
	}

	for (u64 i = 0; i < line_num; ++i) {
		KLOG("%s", lines[i]);
	}
	 
	// step two: parse valid lines
	// step three: push valid parts into the mesh indices and vertices
}