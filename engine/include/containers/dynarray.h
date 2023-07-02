#ifndef KENGINE_CONTAINERS_DYNARRAY
#define KENGINE_CONTAINERS_DYNARRAY

#include <defines.h>

enum {
	DYNARRAY_CAPACITY,
	DYNARRAY_LENGTH,
	DYNARRAY_STRIDE,
	DYNARRAY_FIELD_LENGTH
};

void KAPI * _dynarray_create(u64 length, u64 stride);
void KAPI * _dynarray_destroy(void * array);

u64 KAPI _dynarray_field_get(void * array, u64 field);
void KAPI _dynarray_field_set(void * array, u64 field, u64 value);

void KAPI * _dynarray_resize(void * array);

void KAPI * _dynarray_push(void * array, const void * value_ptr);
void KAPI _dynarray_pop(void * array, void * dst);

void KAPI * _dynarray_push_at(void * array, u64 index, const void * value_ptr);
void KAPI _dynarray_pop_at(void * array, u64 index, void * dst);

#define DYNARRAY_DEFAULT_CAPACITY 1
#define DYNARRAY_RESIZE_FACTOR 2

#define dynarray_create(type) \
	_dynarray_create(DYNARRAY_DEFAULT_CAPACITY, sizeof(type));

#define dynarray_reserve(type, cap) \
	_dynarray_create(cap, sizeof(type));

#define dynarray_destroy(array) _dynarray_destroy(array);

#define dynarray_push(array, value)				\
	{											\
		typeof(value) tmp = value;				\
		array = _dynarray_push(array, &tmp);	\
	}

#define dynarray_pop(array, value_ptr) \
	_dynarray_pop(array, value_ptr);

#define dynarray_insert_at(array, index, value)				\
	{														\
		typeof(value) tmp = value;							\
		array = _dynarray_insert_at(array, index, &tmp);	\
	}

#define dynarray_pop_at(array, index, value_ptr) \
	_dynarray_pop_at(array, index, value_ptr);

#define dynarray_clear(array) \
	_dynarray_field_set(array, DYNARRAY_LENGTH, 0)

#define dynarray_capacity(array) \
	_dynarray_field_get(array, DYNARRAY_CAPACITY)

#define dynarray_stride(array) \
	_dynarray_field_get(array, DYNARRAY_STRIDE)

#define dynarray_length_set(array, value) \
	_dynarray_field_set(array, DYNARRAY_LENGTH, value);

#endif
