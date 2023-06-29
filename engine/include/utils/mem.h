#ifndef UTILS_MEM_H
#define UTILS_MEM_H

#include <stddef.h>

void * allocate(size_t size);
void * reallocate(void * ptr, size_t size);
void * destroy(void * ptr);

#endif
