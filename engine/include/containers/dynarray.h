#ifndef KENGINE_CONTAINERS_DYNARRAY
#define KENGINE_CONTAINERS_DYNARRAY

#include <defines.h>

typedef struct dynArray {
	u64 capacity;
	u64 length;
	u64 stride;
	void * array;
} dynarray_t;

dynarray_t KAPI * _dynarray_create(u64 length, u64 stride);
void KAPI _dynarray_destroy(dynarray_t * array);

dynarray_t KAPI * _dynarray_resize(dynarray_t * array);

dynarray_t KAPI * _dynarray_push(dynarray_t * array, void * value_ptr);
void KAPI _dynarray_pop(dynarray_t * array, void * dst);

dynarray_t KAPI * _dynarray_insert_at(dynarray_t * array, u64 index, void * value_ptr);
dynarray_t KAPI * _dynarray_pop_at(dynarray_t * array, u64 index, void * dst);

#define DYNARRAY_DEFAULT_CAPACITY 1
#define DYNARRAY_RESIZE_FACTOR 2

#define dynarray_at(dynarray, i, T) ((T *) dynarray->array)[i]

#define dynarray_create(T) \
	_dynarray_create(DYNARRAY_DEFAULT_CAPACITY, sizeof(T));

#define dynarray_reserve(T, cap) \
	_dynarray_create(cap, sizeof(T));

#define dynarray_destroy(array) _dynarray_destroy(array);

#define dynarray_push(array, value, T)					\
	{													\
		T tmp = value;									\
		array = _dynarray_push(array, (void *) &tmp);	\
	}

#define dynarray_push_from(array, ptr)					\
	{													\
		array = _dynarray_push(array, (void *) ptr);	\
	}

#define dynarray_pop(array, value_ptr) \
	_dynarray_pop(array, value_ptr);

#define dynarray_insert_at(array, index, value, T)					\
	{																\
		T tmp = value;												\
		array = _dynarray_insert_at(array, index, (void *) &tmp);	\
	}

#define dynarray_remove_at(array, index, T)						\
	{															\
		T tmp;													\
		array = _dynarray_pop_at(array, index, (void *) &tmp);	\
	}

#define dynarray_pop_at(array, index, value_ptr) \
	_dynarray_pop_at(array, index, value_ptr);

#define dynarray_clear(array) \
	_dynarray_field_set(array, DYNARRAY_LENGTH, 0)

#define dynarray_merge(arra, arrb, T)							\
	{															\
		for (u64 i = 0; i < arrb->length; ++i) {				\
			dynarray_push(arra, ((T *) arrb->array)[i], T);	\
		}														\
	}

#define dynarray_merge_array(arra, arrb, lenb)			\
	{													\
		for (u64 i = 0; i < lenb; ++i) {				\
			dynarray_push(arra, arrb[i]);				\
		}												\
	}

#endif
