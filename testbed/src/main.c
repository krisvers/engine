#include <stdio.h>
#include <dlfcn.h>

#define LIB_FILE "./engine/bin/engine.so"
#define SYMBOL "main"

int main(void) {
	void * handle;
	int (* entry)(void);

	handle = dlopen(LIB_FILE, RTLD_NOW);
	if (handle == NULL) {
		printf("%s\n", dlerror());
		return -1;
	}

	*((void **) &entry) = dlsym(handle, SYMBOL);
	if (entry == NULL) {
		printf("%s\n", dlerror());
		return -1;
	}

	return entry();
}
