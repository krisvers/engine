#ifndef KTESTBED_PLATFORM_H
#define KTESTBED_PLATFORM_H

#include <defines.h>

#ifdef KPLATFORM_LINUX

#define KPLATFORM_API

#include <dlfcn.h>
#define LIB_FILE "./engine/bin/engine.so"
#define LIB_HANDLE void *
#define OPEN(name)								\
	*((void **) &name) = dlsym(handle, #name);	\
	if (name == NULL) {							\
		printf("%s\n", dlerror());				\
		return -1;								\
	}
#define OPEN_SYMBOL(name, symbol) 				\
	*((void **) &name) = dlsym(handle, #symbol);\
	if (name == NULL) {							\
		printf("%s\n", dlerror());				\
		return -1;								\
	}
#define OPEN_LIB(file)							\
	dlopen(file, RTLD_NOW)
#define PRINT_ERROR() printf("%s\n", dlerror())

#elif KPLATFORM_WINDOWS

#include <windows.h>
#define LIB_FILE "./engine/bin/engine.dll"
#define LIB_HANDLE HINSTANCE
#define OPEN(name)												\
	*((void **) &name) = GetProcAddress(handle, #name);			\
	if (name == NULL) {											\
		fprintf(stderr, "Failed to load symbol %s\n", #name);	\
		return -1;												\
	}
#define OPEN_SYMBOL(name, symbol)								\
	*((void **) &name) = GetProcAddress(handle, #symbol);		\
	if (name == NULL) {											\
		fprintf(stderr, "Failed to load symbol %s\n", #symbol);	\
		return -1;												\
	}
#define OPEN_LIB(file)											\
	LoadLibrary(file)
#define PRINT_ERROR()

#endif

#endif
