#include <containers/dynarray.h>
#include <core/mem.h>
#include <core/logger.h>
#include <defines.h>

dynarray_t * _dynarray_create(u64 length, u64 stride) {
	dynarray_t * new_array = (dynarray_t *) kmalloc(sizeof(dynarray_t), MEMORY_TAG_DYNAMIC_ARRAY);
	new_array->array = kmalloc(length * stride, MEMORY_TAG_DYNAMIC_ARRAY);

	new_array->capacity = length;
	new_array->length = 0;
	new_array->stride = stride;

	return new_array;
}

void _dynarray_destroy(dynarray_t * array) {
	kfree(array->array, array->stride * array->capacity, MEMORY_TAG_DYNAMIC_ARRAY);
	kfree(array, sizeof(dynarray_t), MEMORY_TAG_DYNAMIC_ARRAY);
}

dynarray_t * _dynarray_resize(dynarray_t * array) {
	dynarray_t * tmp = _dynarray_create(
		(DYNARRAY_RESIZE_FACTOR * array->capacity),
		array->stride
	);
	kmemcpy(tmp->array, array->array, array->capacity * array->stride);

	tmp->stride = array->stride;
	tmp->length = array->length;
	_dynarray_destroy(array);
	return tmp;
}

dynarray_t * _dynarray_push(dynarray_t * array, void * value_ptr) {
	if (array->length >= array->capacity) {
		array = _dynarray_resize(array);
	}

	kmemcpy((void *) (((u64) array->array) + array->length * array->stride), value_ptr, array->stride);
	++array->length;
	return array;
}

void _dynarray_pop(dynarray_t * array, void * dst) {
	kmemcpy(dst, (void *) (((u64) array->array) + (array->length - 1) * array->stride), array->stride);
	--array->length;
}

dynarray_t * _dynarray_pop_at(dynarray_t * array, u64 index, void * dst) {
	if (index >= array->length) {
		KERROR("[_dynarray_pop_at(array, index, dst)]");
		KERROR("index is outside of the bounds for this array! length: %llu, index: %llu", array->length, index);
		return array;
	}

	kmemcpy(dst, (void *) (((u64) array->array) + (index * array->stride)), array->stride);

	if (index != array->length - 1) {
		kmemcpy(
			(void *) (((u64) array->array) + (index + array->stride)),
			(void *) (((u64) array->array) + (index + 1) * array->stride),
			array->stride * (array->length - index)
		);
	}

	--array->length;
	return array;
}

dynarray_t * _dynarray_insert_at(dynarray_t * array, u64 index, void * value_ptr) {
	if (index >= array->length) {
		KERROR("[_dynarray_insert_at(array, index, value_ptr)]");
		KERROR("index is outside of the bounds for this array! length: %llu, index: %llu", array->length, index);
		return array;
	}
	if (array->length >= array->capacity) {
		array = _dynarray_resize(array);
	}

	if (index != array->length - 1) {
		kmemcpy(
			(void *) (((u64) array->array) + (index + 1) * array->stride),
			(void *) (((u64) array->array) + (index * array->stride)),
			array->stride * (array->length - index)
		);
	}

	kmemcpy((void *) (((u64) array->array) + (index * array->stride)), value_ptr, array->stride);

	++array->length;
	return array;
}