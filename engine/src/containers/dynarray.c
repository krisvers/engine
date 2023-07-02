#include <containers/dynarray.h>
#include <core/mem.h>

void * _dynarray_create(u64 length, u64 stride) {
	u64 header_size = DYNARRAY_FIELD_LENGTH * sizeof(u64);
	u64 array_size = length * stride;
	u64 * new_array = kmalloc(header_size + array_size, MEMORY_TAG_DYNAMIC_ARRAY);

	new_array[DYNARRAY_CAPACITY] = length;
	new_array[DYNARRAY_LENGTH] = 0;
	new_array[DYNARRAY_STRIDE] = stride;

	return (void *) new_array + DYNARRAY_FIELD_LENGTH;
}
