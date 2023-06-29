#include <utils/mem.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void * allocate(size_t size) {
	void * p = malloc(size);
	if (p == NULL) {
		printf("[from allocate(size)]\nfailed to allocate %zu bytes\n", size);
		abort();
	}

	return p;
}

void * reallocate(void * ptr, size_t size) {
	if (ptr == NULL) {
		printf("[from reallocate(ptr, size)]\npassed pointer is NULL\n");
		abort();
	}

	void * p = realloc(ptr, size);
	if (p == NULL) {
		printf("[from reallocate(ptr, size)]\nfailed to reallocate ptr %p to be %zu bytes\n", ptr, size);
		abort();
	}

	return p;
}

void * destroy(void * ptr) {
	if (ptr == NULL) {
		printf("[from destroy(ptr)]\npassed pointer is NULL\n");
		abort();
	}

	free(ptr);
	return NULL;
}
