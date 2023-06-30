#include <stdio.h>
#include <dlfcn.h>
#include <core/logger.h>

#define LIB_FILE "./engine/bin/engine.so"

#define OPEN(name)								\
	*((void **) &name) = dlsym(handle, #name);	\
	if (name == NULL) {							\
		printf("%s\n", dlerror());				\
		return -1;								\
	}
#define OPEN_SYMBOL(name, symbol) \
	*((void **) &name) = dlsym(handle, #symbol);\
	if (name == NULL) {							\
		printf("%s\n", dlerror());				\
		return -1;								\
	}

int main(void) {
	void * handle;
	int (* entry)(void);

	handle = dlopen(LIB_FILE, RTLD_NOW);
	if (handle == NULL) {
		printf("%s\n", dlerror());
		return -1;
	}

	OPEN_SYMBOL(entry, main);

	printf("testbed: executing entry point\n");
	entry();
	printf("testbed: entry point exited\n");

	return 0;
}
